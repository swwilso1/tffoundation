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
#define NEEDS_FUNCTIONAL
#define NEEDS_UNORDERED_MAP
#define NEEDS_VECTOR
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfthreadsafequeue.hxx"
#include "tfthreadcontroller.hpp"

namespace TF::Foundation
{

    struct ThreadPoolJob
    {
        using job_function_type = std::function<void()>;
        using job_function_list = std::vector<job_function_type>;

        job_function_list job_functions{};
    };

    class ThreadPool : public AllocatorInterface
    {
    public:
        using pool_job = ThreadPoolJob;

        explicit ThreadPool(int threads);

        void start();

        void stop();

        void add_job(const pool_job & job);

    private:
        using job_queue = ThreadSafeQueue<pool_job::job_function_type>;

        struct ThreadManager
        {
            int id{0};
            std::thread thread{};
            ThreadController controller{};
            job_queue jobs{};
        };

        using manager_ptr = std::shared_ptr<ThreadManager>;

        int m_number_of_pool_threads{1};
        manager_ptr m_scheduler{};
        std::unordered_map<int, manager_ptr> m_pool_threads{};
        ThreadSafeQueue<int> m_available_threads_queue{};
        ThreadSafeQueue<pool_job> m_job_queue{};
    };

} // namespace TF::Foundation

#endif // TFTHREADPOOL_HPP
