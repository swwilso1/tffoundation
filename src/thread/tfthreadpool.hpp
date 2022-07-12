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

#ifndef TFTHREADPOOL_HPP
#define TFTHREADPOOL_HPP

#define NEEDS_THREAD
#define NEEDS_MUTEX
#define NEEDS_FUNCTIONAL
#define NEEDS_UNORDERED_MAP
#define NEEDS_UNORDERED_SET
#define NEEDS_VECTOR
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfthreadsafequeue.hxx"
#include "tfthreadcontroller.hpp"

namespace TF::Foundation
{

    /**
     * ThreadPoolJob objects contain functions that should be executed on
     * an independent thread that make up the actions for a job.
     *
     * This multi-function capability allows for jobs that require more than one
     * thread to accomplish a task.  For example, a reader/writer thread pair,
     * or multiple readers/one writer, etc...
     */
    struct ThreadPoolJob
    {
        using job_function_type = std::function<void()>;
        using job_function_list = std::vector<job_function_type>;

        job_function_list job_functions{};
    };

    /**
     * ThreadPool contains the code/interface for a pool of threads that can process
     * arbitrary jobs of work.
     *
     * Jobs are submitted to the job queue and the scheduler will schedule the jobs on
     * available threads.
     */
    class ThreadPool : public AllocatorInterface
    {
    public:
        using pool_job = ThreadPoolJob;

        /**
         * @brief constructor that specifies the number of threads for the thread pool.
         * @param threads the number of threads to run in the pool.
         */
        explicit ThreadPool(int threads);

        /**
         * @brief method to start running the pool and scheduler threads.
         */
        void start();

        /**
         * @brief method to stop running the pool and scheduler threads.
         */
        void stop();

        /**
         * @brief method to add job to the pool execution queue.
         * @param job
         */
        void add_job(const pool_job & job);

        /**
         * @brief method to add more threads to the pool.
         * @param threads the number of new threads to add to the pool.
         */
        void add_pool_threads(int threads);

        /**
         * @brief method to remove some number of running threads from the pool
         * @param threads the number of threads to remove.
         *
         * If @e threads is more than the number of running threads in the pool,
         * the method does nothing.
         *
         * This function makes no guarantees on the timing of the pool scale down.
         */
        void remove_pool_threads(int threads);

        /**
         * @brief method to get the number of threads running in the pool.
         * @return the number of actively running threads.
         */
        int get_percent_in_use();

        /**
         * @brief method to see check to see if pool has work to do.
         * @return true if the pool has no work.
         */
        bool is_idle();

        /**
         * @brief method to return the number of threads in the pool.
         * @return the number of threads
         */
        std::size_t threads_in_use()
        {
            std::lock_guard<std::mutex> pool_lock{m_pool_threads.mutex};
            return m_pool_threads.data_structure.size();
        }

    private:
        using job_queue = ThreadSafeQueue<pool_job::job_function_type>;

        /**
         * ThreadManager contains the details for managing a pool thread or control
         * thread.
         */
        struct ThreadManager
        {
            int id{0};
            std::thread thread{};
            ThreadController controller{};
            job_queue jobs{};

            bool is_working()
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_working;
            }

            void set_working(bool working)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_working = working;
            }

        private:
            bool m_working{false};
            std::mutex m_mutex;
        };

        template<typename DS>
        struct LockedDataStructure
        {
            std::mutex mutex{};
            DS data_structure{};
        };

        using manager_ptr = std::shared_ptr<ThreadManager>;

        /**
         * @brief helper function to add a thread to the pool.
         */
        void add_pool_thread();

        int m_number_of_pool_threads{1};
        int m_next_thread_id{0};
        manager_ptr m_scheduler{};
        LockedDataStructure<std::unordered_map<int, manager_ptr>> m_pool_threads{};
        LockedDataStructure<std::unordered_set<int>> m_available_threads_set{};
        ThreadSafeQueue<int> m_available_threads_queue{};
        ThreadSafeQueue<pool_job> m_job_queue{};
        std::function<void(const manager_ptr &)> m_pool_code{};
    };

} // namespace TF::Foundation

#endif // TFTHREADPOOL_HPP
