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

        template<typename PollHandle>
        struct HandleEntry
        {
            using handle_type = PollHandle;

            handle_type handle;
            int events_to_watch;
            int events_set;

            HandleEntry() : handle {}, events_to_watch {0}, events_set {0}
            {
            }
            virtual ~HandleEntry()
            {
            }
        };

        template<typename PollHandle>
        struct PollWorker
        {
            using handle_type = PollHandle;
            using entry_type = HandleEntry<PollHandle>;
            using handle_list_type = std::vector<entry_type *>;

            template<typename Rep, typename Period>
            static handle_list_type &wait_for(const std::chrono::duration<Rep, Period> &duration,
                                              handle_list_type &handles)
            {
                return handles;
            }

            template<typename Clock, typename Duration>
            static handle_list_type &wait_until(const std::chrono::time_point<Clock, Duration> &abs_time,
                                                handle_list_type &handles)
            {
                return handles;
            }
        };

    }    // namespace Foundation

}    // namespace TF

#endif    // TFPOLLWORKER_HXX
