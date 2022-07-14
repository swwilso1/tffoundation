/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "tfthreadpool.hpp"
#include "tfsleep.hpp"
#include "tflog.hpp"

namespace TF::Foundation
{

    ThreadPool::ThreadPool(int threads) : m_number_of_pool_threads{threads}
    {
        m_pool_code = [this](const manager_ptr & manager) -> void {
            bool first_iteration{true};
            while (! manager->controller.shouldStop())
            {
                // For the first iteration, we may not have any jobs and want to make
                // sure we put the thread id into the available thread queue.  After the
                // first iteration we always want this value to be false.
                bool processed_queue_jobs{first_iteration};

                // Check the queue of jobs.
                while (! manager->jobs.empty())
                {
                    auto job = manager->jobs.front();
                    manager->set_working(true);
                    try
                    {
                        job();
                    }
                    catch (...)
                    {
                        LOG(LogPriority::Error, "Pool thread %d job generated an exception, dropping job.",
                            manager->id);
                    }
                    manager->set_working(false);
                    manager->jobs.pop();

                    processed_queue_jobs = true;

                    // Check if we need to stop.
                    if (manager->controller.shouldStop())
                    {
                        break;
                    }
                }

                // Check again if we need to stop.
                if (manager->controller.shouldStop())
                {
                    continue;
                }

                if (processed_queue_jobs)
                {
                    // We have finished our jobs, so now put our id
                    // in the available threads set.
                    std::lock_guard<std::mutex> lock(m_available_threads_set.mutex);
                    m_available_threads_set.data_structure.insert(manager->id);
                }

                // Wait for something to do.  Eventually wake up and check
                // the job queue.
                manager->controller.wait_for(std::chrono::milliseconds(100));

                if (first_iteration)
                {
                    first_iteration = false;
                }
            }
        };
    }

    void ThreadPool::start()
    {
        auto scheduler_thread_code = [this](const manager_ptr & manager) -> void {
            while (! manager->controller.shouldStop())
            {
                // See if we have any jobs.
                if (! m_job_queue.empty())
                {
                    std::lock_guard<std::mutex> set_lock(m_available_threads_set.mutex);
                    std::lock_guard<std::mutex> pool_lock(m_pool_threads.mutex);

                    // See if we have any threads to do the job.
                    if (! m_available_threads_set.data_structure.empty())
                    {
                        auto job = m_job_queue.front();

                        // See if we have enough threads to do the job.
                        if (m_available_threads_set.data_structure.size() >= job.job_functions.size())
                        {
                            // We have enough threads to run the job.
                            std::for_each(job.job_functions.begin(), job.job_functions.end(),
                                          [this](auto function) -> void {
                                              // Now for each job, get a thread and then put the job function in the
                                              // thread's job queue.

                                              auto id = *m_available_threads_set.data_structure.begin();
                                              auto manager = m_pool_threads.data_structure[id];
                                              manager->jobs.push(function);
                                              m_available_threads_set.data_structure.erase(id);
                                          });

                            // Remove the job from the queue.
                            m_job_queue.pop();
                        }
                    }
                }
                else
                {
                    // The job queue is empty, so wait a bit to see if we get a job.
                    manager->controller.wait_for(std::chrono::milliseconds{100});
                }
            }
        };

        // Now start the pool threads.
        for (int i = 0; i < m_number_of_pool_threads; i++)
        {
            add_pool_thread();
        }

        // Finally start the scheduler thread.
        m_scheduler = std::make_shared<ThreadManager>();
        m_scheduler->thread = std::thread{scheduler_thread_code, m_scheduler};
    }

    void ThreadPool::stop()
    {
        std::lock_guard<std::mutex> pool_lock(m_pool_threads.mutex);

        // Send the stop signal to all threads.
        std::for_each(m_pool_threads.data_structure.begin(), m_pool_threads.data_structure.end(),
                      [](auto & entry) -> void {
                          entry.second->controller.signalStop();
                          entry.second->controller.signal();
                      });

        m_scheduler->controller.signalStop();
        m_scheduler->controller.signal();

        // Try joining.
        std::for_each(m_pool_threads.data_structure.begin(), m_pool_threads.data_structure.end(),
                      [](auto & entry) -> void {
                          entry.second->thread.join();
                      });
        m_scheduler->thread.join();

        // We joined all the threads, now remove the pool entries.
        m_pool_threads.data_structure.clear();
    }

    void ThreadPool::add_job(const pool_job & job)
    {
        m_job_queue.push(job);
    }

    void ThreadPool::add_pool_threads(int threads)
    {
        for (int i{0}; i < threads; i++)
        {
            add_pool_thread();
        }
    }

    void ThreadPool::remove_pool_threads(int threads)
    {
        while (threads > 0)
        {
            bool removed_a_thread{false};
            {
                std::lock_guard<std::mutex> set_lock(m_available_threads_set.mutex);
                if (! m_available_threads_set.data_structure.empty())
                {
                    std::lock_guard<std::mutex> pool_lock(m_pool_threads.mutex);
                    auto id = *m_available_threads_set.data_structure.begin();
                    auto manager = m_pool_threads.data_structure[id];
                    manager->controller.signalStop();
                    manager->controller.signal();
                    manager->thread.join();
                    m_pool_threads.data_structure.erase(id);
                    m_available_threads_set.data_structure.erase(id);
                    --threads;
                    removed_a_thread = true;
                }
            }

            if (! removed_a_thread)
            {
                Sleep(std::chrono::milliseconds(150));
            }
        }
    }

    int ThreadPool::get_percent_in_use()
    {
        std::lock_guard<std::mutex> set_lock(m_available_threads_set.mutex);
        std::lock_guard<std::mutex> pool_lock(m_pool_threads.mutex);

        auto percent =
            (static_cast<double>(m_pool_threads.data_structure.size() - m_available_threads_set.data_structure.size()) /
             static_cast<double>(m_pool_threads.data_structure.size())) *
            100;

        return static_cast<int>(percent);
    }

    bool ThreadPool::is_idle()
    {
        std::lock_guard<std::mutex> set_lock(m_available_threads_set.mutex);
        std::lock_guard<std::mutex> pool_lock(m_pool_threads.mutex);

        return m_job_queue.empty() &&
               m_pool_threads.data_structure.size() == m_available_threads_set.data_structure.size();
    }

    void ThreadPool::add_pool_thread()
    {
        // Create the thread object.
        std::shared_ptr<ThreadManager> manager = std::make_shared<ThreadManager>();
        manager->id = m_next_thread_id++;

        // Stick the thread in the pool.
        std::lock_guard<std::mutex> pool_lock(m_pool_threads.mutex);
        m_pool_threads.data_structure.insert(std::make_pair(manager->id, manager));

        // Start the thread running.
        manager->thread = std::thread{m_pool_code, manager};
    }

} // namespace TF::Foundation
