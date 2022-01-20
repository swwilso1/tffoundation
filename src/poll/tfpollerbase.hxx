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

#ifndef TFPOLLERBASE_HXX
#define TFPOLLERBASE_HXX

#define NEEDS_VECTOR
#define NEEDS_CHRONO
#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfpollevent.hpp"
#include "tfpollworker.hxx"

namespace TF
{

    namespace Foundation
    {

        template<typename PollHandle, typename Worker = PollWorker<PollHandle>>
        class PollerBase : public AllocatorInterface
        {
        public:
            using handle_type = PollHandle;

            using event_type = PollEvent;

            using callback_type = std::function<void(PollHandle handle)>;

            ~PollerBase()
            {
                for(auto entry : m_handles)
                {
                    delete entry;
                }
            }

            void add_handle(const handle_type &h, int event, callback_type callback = nullptr)
            {
                handle_entry_type *entry = new handle_entry_type;
                entry->handle = h;

                if((event & static_cast<int>(event_type::Read)) == static_cast<int>(event_type::Read))
                {
                    entry->events_to_watch |= static_cast<int>(event_type::Read);
                    entry->read_callback = callback;
                }

                if((event & static_cast<int>(event_type::Write)) == static_cast<int>(event_type::Write))
                {
                    entry->events_to_watch |= static_cast<int>(event_type::Write);
                    entry->write_callback = callback;
                }

                if((event & static_cast<int>(event_type::Except)) == static_cast<int>(event_type::Except))
                {
                    entry->events_to_watch |= static_cast<int>(event_type::Except);
                    entry->exception_callback = callback;
                }

                m_handles.emplace_back(entry);
            }

            void add_handle(const handle_type &h, event_type event, callback_type callback = nullptr)
            {
                add_handle(h, static_cast<int>(event), callback);
            }

            template<typename Rep, typename Period>
            bool wait_for(const std::chrono::duration<Rep, Period> &wait_duration)
            {
                bool something_ready {false};
                (void)worker_type::wait_for(wait_duration, m_handles);
                for(auto &entry : m_handles)
                {
                    auto hentry = dynamic_cast<handle_entry_type &>(*entry);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Read, hentry.read_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Write, hentry.write_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Except, hentry.exception_callback);
                }

                return something_ready;
            }

            template<typename Clock, typename Duration>
            bool wait_until(const std::chrono::time_point<Clock, Duration> &abs_time)
            {
                bool something_ready {false};
                (void)worker_type::wait_until(abs_time, m_handles);
                for(auto &entry : m_handles)
                {
                    auto hentry = dynamic_cast<handle_entry_type &>(*entry);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Read, hentry.read_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Write, hentry.write_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Except, hentry.exception_callback);
                }
                return something_ready;
            }

            bool handle_set_for_event(const handle_type &h, event_type pevent)
            {
                for(auto &entry : m_handles)
                {
                    if(entry->handle == h)
                    {
                        if((entry->events_set | static_cast<int>(pevent)) == static_cast<int>(pevent))
                        {
                            return true;
                        }
                        break;
                    }
                }
                return false;
            }

            void clear()
            {
                for(auto entry : m_handles)
                {
                    delete entry;
                }
                m_handles.clear();
            }

        private:
            using worker_type = Worker;

            struct HandleEntry : public worker_type::entry_type
            {
                callback_type read_callback;
                callback_type write_callback;
                callback_type exception_callback;

                HandleEntry()
                    : worker_type::entry_type(), read_callback {nullptr}, write_callback {nullptr}, exception_callback {
                                                                                                        nullptr}
                {
                }
            };

            using handle_entry_type = HandleEntry;
            using handle_list_type = typename worker_type::handle_list_type;

            bool check_entry_and_dispatch(handle_entry_type &entry, event_type pevent, callback_type callback)
            {
                bool ready = false;
                if(event_set_for(entry.events_to_watch, pevent) && event_set_for(entry.events_set, pevent))
                {
                    if(callback)
                    {
                        callback(entry.handle);
                    }
                    ready = true;
                }

                return ready;
            }

            handle_list_type m_handles;
        };

    }    // namespace Foundation

}    // namespace TF

#endif    // TFPOLLERBASE_HXX
