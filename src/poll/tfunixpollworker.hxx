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

#ifndef TFUNIXPOLLWORKER_HXX
#define TFUNIXPOLLWORKER_HXX

#define NEEDS_POLL_H
#include "tfheaders.hpp"
#include "tfpollevent.hpp"
#include "tfpollworker.hxx"

namespace TF
{

    namespace Foundation
    {

        template<>
        struct PollWorker<int>
        {
            using handle_type = int;
            using entry_type = HandleEntry<int>;
            using handle_list_type = std::vector<entry_type *>;

            template<typename Rep, typename Period>
            static handle_list_type &wait_for(const std::chrono::duration<Rep, Period> &duration,
                                              handle_list_type &handles)
            {
                struct pollfd *poll_array {nullptr};

                // Do not do anything for an empty list.
                if(handles.size() == 0)
                {
                    return handles;
                }

                poll_array = new struct pollfd[handles.size()];

                // pollfd structs are C structs so we can memset them to initialize;
                memset(poll_array, 0, handles.size() * sizeof(struct pollfd));

                handle_list_type::size_type i = 0;
                for(auto &entry : handles)
                {
                    poll_array[i].fd = entry->handle;
                    if((entry->events_to_watch & static_cast<int>(PollEvent::Read)) ==
                       static_cast<int>(PollEvent::Read))
                    {
                        poll_array[i].events = POLLIN;
                    }

                    if((entry->events_to_watch & static_cast<int>(PollEvent::Write)) ==
                       static_cast<int>(PollEvent::Write))
                    {
                        poll_array[i].events |= POLLOUT;
                    }
                    i++;
                }

                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

                bool keep_going {true};

                while(keep_going)
                {
                    auto poll_start = std::chrono::steady_clock::now();

                    auto poll_api_result = poll(poll_array, handles.size(), milliseconds.count());

                    auto poll_end = std::chrono::steady_clock::now();

                    if(poll_api_result > 0)
                    {
                        i = 0;
                        for(auto &entry : handles)
                        {
                            entry->events_set = 0;
                            if((poll_array[i].revents & POLLIN) == POLLIN)
                            {
                                entry->events_set |= static_cast<int>(PollEvent::Read);
                            }

                            if((poll_array[i].revents & POLLOUT) == POLLOUT)
                            {
                                entry->events_set |= static_cast<int>(PollEvent::Write);
                            }

                            if(((poll_array[i].revents & POLLERR) == POLLERR) ||
                               ((poll_array[i].revents & POLLHUP) == POLLHUP))
                            {
                                entry->events_set |= static_cast<int>(PollEvent::Except);
                            }
                        }

                        keep_going = false;
                    }
                    else if(poll_api_result < 0)
                    {
                        if(errno == EINTR)
                        {
                            milliseconds -=
                                std::chrono::duration_cast<std::chrono::milliseconds>(poll_end - poll_start);
                            if(milliseconds.count() > 0)
                            {
                                continue;
                            }
                        }
                        keep_going = false;
                    }
                    else
                    {
                        keep_going = false;
                    }
                }

                delete[] poll_array;
                return handles;
            }


            template<typename Clock, typename Duration>
            static handle_list_type &wait_until(const std::chrono::time_point<Clock, Duration> &abs_time,
                                                handle_list_type &handles)
            {
                auto now = std::chrono::steady_clock::now();
                auto later = std::chrono::time_point_cast<decltype(now)>(abs_time);
                auto duration = later - now;
                return wait_for(duration, handles);
            }
        };

    }    // namespace Foundation

}    // namespace TF

#endif    // TFUNIXPOLLWORKER_HXX
