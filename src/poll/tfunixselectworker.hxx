/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

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

#ifndef TFUNIXSELECTWORKER_HXX
#define TFUNIXSELECTWORKER_HXX

#define NEEDS_SYS_SELECT_H
#include "tfheaders.hpp"
#include "tfpollevent.hpp"
#include "tfpollworker.hxx"

namespace TF
{

    namespace Foundation
    {

        /**
         * Template specialization for Unix file descriptors that makes use
         * of the select() system call to wait for file descriptor events.
         */
        template<>
        struct PollWorker<int>
        {
            using handle_type = int;
            using entry_type = HandleEntry<int>;
            using handle_list_type = std::vector<entry_type *>;

            template<typename Rep, typename Period>
            static handle_list_type & wait_for(const std::chrono::duration<Rep, Period> & duration,
                                               handle_list_type & handles)
            {
                struct timeval timeout
                {};
                struct timeval * tval{nullptr};

                fd_set read_set{};
                fd_set write_set{};
                fd_set exception_set{};

                fd_set * rset{nullptr};
                fd_set * wset{nullptr};
                fd_set * eset{nullptr};

                handle_type largest_handle = 0;

                // Do not do any work if we have no handles.
                if (handles.size() == 0)
                {
                    return handles;
                }

                for (auto & entry : handles)
                {
                    if ((entry->events_to_watch & static_cast<int>(PollEvent::Read)) ==
                        static_cast<int>(PollEvent::Read))
                    {
                        FD_SET(entry->handle, &read_set);
                        rset = &read_set;
                    }

                    if ((entry->events_to_watch & static_cast<int>(PollEvent::Write)) ==
                        static_cast<int>(PollEvent::Write))
                    {
                        FD_SET(entry->handle, &write_set);
                        wset = &write_set;
                    }

                    if ((entry->events_to_watch & static_cast<int>(PollEvent::Except)) ==
                        static_cast<int>(PollEvent::Except))
                    {
                        FD_SET(entry->handle, &exception_set);
                        eset = &exception_set;
                    }

                    if (entry->handle > largest_handle)
                    {
                        largest_handle = entry->handle;
                    }
                }

                auto duration_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
                auto duration_in_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
                auto time_left_in_microseconds = duration_in_microseconds;

                timeout.tv_sec = duration_in_seconds.count();
                timeout.tv_usec = duration_in_microseconds.count() -
                                  std::chrono::duration_cast<std::chrono::microseconds>(duration_in_seconds).count();

                tval = &timeout;

                bool keep_going{true};

                while (keep_going)
                {
                    auto poll_start_time = std::chrono::steady_clock::now();

                    auto select_api_result = select(largest_handle + 1, rset, wset, eset, tval);

                    auto poll_end_time = std::chrono::steady_clock::now();

                    if (select_api_result > 0)
                    {
                        for (auto & entry : handles)
                        {
                            entry->events_set = 0;

                            if ((entry->events_to_watch & static_cast<int>(PollEvent::Read)) ==
                                static_cast<int>(PollEvent::Read))
                            {
                                if (FD_ISSET(entry->handle, &read_set))
                                {
                                    entry->events_set |= static_cast<int>(PollEvent::Read);
                                }
                            }

                            if ((entry->events_to_watch & static_cast<int>(PollEvent::Write)) ==
                                static_cast<int>(PollEvent::Write))
                            {
                                if (FD_ISSET(entry->handle, &write_set))
                                {
                                    entry->events_set |= static_cast<int>(PollEvent::Write);
                                }
                            }

                            if ((entry->events_to_watch & static_cast<int>(PollEvent::Except)) ==
                                static_cast<int>(PollEvent::Except))
                            {
                                if (FD_ISSET(entry->handle, &exception_set))
                                {
                                    entry->events_set |= static_cast<int>(PollEvent::Except);
                                }
                            }
                        }

                        keep_going = false;
                    }
                    else if (select_api_result < 0)
                    {
                        if (errno == EINTR)
                        {
                            auto microseconds_used =
                                std::chrono::duration_cast<std::chrono::microseconds>(poll_end_time - poll_start_time);
                            time_left_in_microseconds -= microseconds_used;

                            auto time_left_in_seconds =
                                std::chrono::duration_cast<std::chrono::seconds>(time_left_in_microseconds);
                            timeout.tv_sec = time_left_in_seconds.count();
                            timeout.tv_usec =
                                time_left_in_microseconds.count() -
                                std::chrono::duration_cast<std::chrono::microseconds>(time_left_in_seconds).count();
                            if (time_left_in_microseconds.count() > 0)
                            {
                                continue;
                            }
                            keep_going = false;
                        }
                    }
                    else
                    {
                        keep_going = false;
                    }
                }

                return handles;
            }

            template<typename Clock, typename Duration>
            static handle_list_type & wait_until(const std::chrono::time_point<Clock, Duration> & abs_time,
                                                 handle_list_type & handles)
            {
                auto now = std::chrono::steady_clock::now();
                auto later = std::chrono::time_point_cast<decltype(now)>(abs_time);
                auto duration = later - now;
                return wait_for(duration, handles);
            }
        };

    } // namespace Foundation

} // namespace TF

#endif // TFUNIXSELECTWORKER_HXX
