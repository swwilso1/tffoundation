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

namespace TF::Foundation
{

    ThreadPool::ThreadPool(int threads) : m_number_of_pool_threads{threads} {}

    void ThreadPool::start()
    {
        auto pool_thread_code = [this](const manager_ptr & manager) -> void {
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
                    job();
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
                    // in the available threads queue.
                    m_available_threads_queue.push(manager->id);
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

        auto scheduler_thread_code = [this](const manager_ptr & manager) -> void {
            while (! manager->controller.shouldStop())
            {
                // See if we have any jobs.
                if (! m_job_queue.empty())
                {
                    // See if we have any threads to do the job.
                    if (! m_available_threads_queue.empty())
                    {
                        auto job = m_job_queue.front();

                        // See if we have enough threads to do the job.
                        if (m_available_threads_queue.size() >= job.job_functions.size())
                        {
                            // We have enough threads to run the job.
                            std::for_each(job.job_functions.begin(), job.job_functions.end(),
                                          [this](auto function) -> void {
                                              // Now for each job, get a thread and then put the job function in the
                                              // thread's job queue.
                                              auto thread_id = m_available_threads_queue.front();
                                              auto manager = m_pool_threads[thread_id];
                                              manager->jobs.push(function);
                                              m_available_threads_queue.pop();
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
            std::shared_ptr<ThreadManager> manager = std::make_shared<ThreadManager>();
            manager->id = i;

            m_pool_threads.insert(std::make_pair(i, manager));
            manager->thread = std::thread{pool_thread_code, manager};
        }

        // Finally start the scheduler thread.
        m_scheduler = std::make_shared<ThreadManager>();
        m_scheduler->thread = std::thread{scheduler_thread_code, m_scheduler};
    }

    void ThreadPool::stop()
    {
        // Send the stop signal to all threads.
        std::for_each(m_pool_threads.begin(), m_pool_threads.end(), [](auto & entry) -> void {
            entry.second->controller.signalStop();
            entry.second->controller.signal();
        });

        m_scheduler->controller.signalStop();
        m_scheduler->controller.signal();

        // Try joining.
        std::for_each(m_pool_threads.begin(), m_pool_threads.end(), [](auto & entry) -> void {
            entry.second->thread.join();
        });
        m_scheduler->thread.join();

        // We joined all the threads, now remove the pool entries.
        m_pool_threads.clear();
    }

    void ThreadPool::add_job(const pool_job & job)
    {
        m_job_queue.push(job);
    }

} // namespace TF::Foundation
