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

#ifndef TFSLEEP_HPP
#define TFSLEEP_HPP

#define NEEDS_CHRONO
#define NEEDS_CTIME
#define NEEDS_SYSTEM_ERROR
#include "tfheaders.hpp"
#include "tfplatform.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF::Foundation
{

    template<typename Rep, typename Period>
    void Sleep(std::chrono::duration<Rep, Period> wait_time)
    {
#if defined(TFUNIX)
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(wait_time);
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(wait_time);
        auto remaining_nanoseconds = nanoseconds - std::chrono::duration_cast<std::chrono::nanoseconds>(seconds);
        struct timespec nano_sleep_time
        {};
        struct timespec time_remaining
        {};
        bool finished{false};

        nano_sleep_time.tv_sec = seconds.count();
        nano_sleep_time.tv_nsec = remaining_nanoseconds.count();

        while (! finished)
        {
            auto api_result = nanosleep(&nano_sleep_time, &time_remaining);
            if (api_result < 0 && errno == EINTR)
            {
                nano_sleep_time = time_remaining;
            }
            else if (api_result < 0 || api_result > 0)
            {
                throw std::system_error{errno, std::system_category(), "nanosleep failed"};
            }
            else
            {
                finished = true;
            }
        }
#else
#    error "Sleep needs platform specific implementation"
#endif
    }

} // namespace TF::Foundation

#endif // TFSLEEP_HPP
