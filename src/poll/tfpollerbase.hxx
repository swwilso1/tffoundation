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

        /**
         * Class that implements a generic polling interface on a specific handle type.
         * The poll algorithm is deferred to a worker type that implements specializations
         * of the worker interface for the handle type.
         * @tparam PollHandle
         * @tparam Worker
         */
        template<typename PollHandle, typename Worker = PollWorker<PollHandle>>
        class PollerBase : public AllocatorInterface
        {
        public:
            using handle_type = PollHandle;

            using event_type = PollEvent;

            using callback_type = std::function<void(PollHandle handle)>;

            ~PollerBase()
            {
                for (auto entry : m_handles)
                {
                    delete entry;
                }
            }

            /**
             * @brief method to add a handle/event/callback combination to the list of handles
             * to observe.
             * @param h the handle
             * @param event bitmask of the events to watch.
             * @param callback the callback function to call when a handle matches the event.
             */
            void add_handle(const handle_type & h, int event, callback_type callback = nullptr)
            {
                handle_entry_type * entry = new handle_entry_type;
                entry->handle = h;

                if ((event & static_cast<int>(event_type::Read)) == static_cast<int>(event_type::Read))
                {
                    entry->events_to_watch |= static_cast<int>(event_type::Read);
                    entry->read_callback = callback;
                }

                if ((event & static_cast<int>(event_type::Write)) == static_cast<int>(event_type::Write))
                {
                    entry->events_to_watch |= static_cast<int>(event_type::Write);
                    entry->write_callback = callback;
                }

                if ((event & static_cast<int>(event_type::Except)) == static_cast<int>(event_type::Except))
                {
                    entry->events_to_watch |= static_cast<int>(event_type::Except);
                    entry->exception_callback = callback;
                }

                m_handles.emplace_back(entry);
            }

            /**
             * @brief method to watch a handle for a specific event.
             * @param h the handle
             * @param event the event.
             * @param callback the callback function to call when handle matches the @e event.
             */
            void add_handle(const handle_type & h, event_type event, callback_type callback = nullptr)
            {
                add_handle(h, static_cast<int>(event), callback);
            }

            /**
             * @briev method to wait for a specified period of time for once of the
             * handles in the handle list to match an event.
             * @tparam Rep type representing number of ticks in a duration
             * @tparam Period a ratio type representing the number of second fractions per tick.
             * @param wait_duration the time interval to wait
             * @return true if a handle matches an event during the interval and false otherwise.
             */
            template<typename Rep, typename Period>
            bool wait_for(const std::chrono::duration<Rep, Period> & wait_duration)
            {
                bool something_ready{false};
                (void)worker_type::wait_for(wait_duration, m_handles);
                for (auto & entry : m_handles)
                {
                    auto hentry = dynamic_cast<handle_entry_type &>(*entry);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Read, hentry.read_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Write, hentry.write_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Except, hentry.exception_callback);
                }

                return something_ready;
            }

            /**
             * @brief method to wait for handles to match events until specific time
             * is met.
             * @tparam Clock the clock that measures the time point.
             * @tparam Duration a duration type used to measure the time since the epoch.
             * @param abs_time the time point to wait until.
             * @return true if a handle matched an event during the wait period.
             */
            template<typename Clock, typename Duration>
            bool wait_until(const std::chrono::time_point<Clock, Duration> & abs_time)
            {
                bool something_ready{false};

                auto now = std::chrono::time_point_cast<decltype(abs_time)>(Clock::now());
                if (abs_time <= now)
                {
                    return false;
                }

                (void)worker_type::wait_until(abs_time, m_handles);
                for (auto & entry : m_handles)
                {
                    auto hentry = dynamic_cast<handle_entry_type &>(*entry);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Read, hentry.read_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Write, hentry.write_callback);
                    something_ready |= check_entry_and_dispatch(hentry, event_type::Except, hentry.exception_callback);
                }
                return something_ready;
            }

            /**
             * @brief method to see if a handle matched an event after a call to wait_for
             * or wait_until.
             * @param h the handle
             * @param pevent the event to match
             * @return true if @e h matched @e pevent and false otherwise.
             */
            bool handle_set_for_event(const handle_type & h, event_type pevent)
            {
                for (auto & entry : m_handles)
                {
                    if (entry->handle == h)
                    {
                        if ((entry->events_set | static_cast<int>(pevent)) == static_cast<int>(pevent))
                        {
                            return true;
                        }
                        break;
                    }
                }
                return false;
            }

            /**
             * @brief method to remove all handles in the poller object.
             */
            void clear()
            {
                for (auto entry : m_handles)
                {
                    delete entry;
                }
                m_handles.clear();
            }

        private:
            using worker_type = Worker;

            /**
             * An entry class that holds the callback pointers for each event type
             * for each handle type.
             */
            struct HandleEntry : public worker_type::entry_type
            {
                callback_type read_callback;
                callback_type write_callback;
                callback_type exception_callback;

                HandleEntry() :
                    worker_type::entry_type(), read_callback{nullptr}, write_callback{nullptr}, exception_callback{
                                                                                                    nullptr}
                {}
            };

            using handle_entry_type = HandleEntry;
            using handle_list_type = typename worker_type::handle_list_type;

            /**
             * @brief helper method to check if a handle matched an entry and the callback to
             * call if the entry matched.
             * @param entry the entry from the list of entries.
             * @param pevent the event
             * @param callback the callback function.
             * @return true if the handle matched the event.
             *
             * The method will call the callback function if the handle matched the event.
             */
            bool check_entry_and_dispatch(handle_entry_type & entry, event_type pevent, callback_type callback)
            {
                bool ready = false;
                if (event_set_for(entry.events_to_watch, pevent) && event_set_for(entry.events_set, pevent))
                {
                    if (callback)
                    {
                        callback(entry.handle);
                    }
                    ready = true;
                }

                return ready;
            }

            handle_list_type m_handles;
        };

    } // namespace Foundation

} // namespace TF

#endif // TFPOLLERBASE_HXX
