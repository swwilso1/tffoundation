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

#ifndef TFPOLLWORKER_HXX
#define TFPOLLWORKER_HXX

#define NEEDS_VECTOR
#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * Class definition for a basic entry in the list of handles.  Captures
         * the handle value, the events to watch, and the events that matched
         * after a call to a wait function.
         * @tparam PollHandle type of the handle
         */
        template<typename PollHandle>
        struct HandleEntry
        {
            using handle_type = PollHandle;

            handle_type handle;
            int events_to_watch;
            int events_set;

            HandleEntry() : handle{}, events_to_watch{0}, events_set{0} {}
            virtual ~HandleEntry() {}
        };

        /**
         * Class definition for the code that implements the actual polling behavior.
         * Provide specializations of this class to implement a particular polling
         * strategy for a handle type.
         * @tparam PollHandle the hndle type.
         */
        template<typename PollHandle>
        struct PollWorker
        {
            using handle_type = PollHandle;
            using entry_type = HandleEntry<PollHandle>;
            using handle_list_type = std::vector<entry_type *>;

            /**
             * @brief the static method that implements the wait_for algorithm for the
             * polling strategy.
             * @tparam Rep type representing number of ticks in a duration
             * @tparam Period a ratio type representing the number of second fractions per tick.
             * @param duration the length of time to wait
             * @param handles the list of handles/events to observe.
             * @return the list of handles updated with the wait results.
             */
            template<typename Rep, typename Period>
            static handle_list_type & wait_for(const std::chrono::duration<Rep, Period> & duration,
                                               handle_list_type & handles)
            {
                return handles;
            }

            /**
             * @brief the static method that implements the wait_until algorithm for
             * the polling strategy.
             * @tparam Clock the clock that measures the time point.
             * @tparam Duration a duration type used to measure the time since the epoch.
             * @param abs_time the time point to wait until.
             * @param handles the list of handles/events to observe.
             * @return the list of handles updated with the wait results.
             */
            template<typename Clock, typename Duration>
            static handle_list_type & wait_until(const std::chrono::time_point<Clock, Duration> & abs_time,
                                                 handle_list_type & handles)
            {
                return handles;
            }
        };

    } // namespace Foundation

} // namespace TF

#endif // TFPOLLWORKER_HXX
