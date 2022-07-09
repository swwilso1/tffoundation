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

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

template<typename Rep, typename Period>
void test_sleep(std::chrono::duration<Rep, Period> sleep_duration, std::chrono::duration<Rep, Period> upper_duration)
{
    SystemDate before_sleep{};
    Sleep(sleep_duration);
    SystemDate after_sleep{};
    auto time_slept = after_sleep - before_sleep;
    std::chrono::duration<Rep, Period> time_slept_dur =
        std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(time_slept);
    EXPECT_GE(time_slept_dur.count(), sleep_duration.count());
    EXPECT_LT(time_slept_dur.count(), upper_duration.count());
}

TEST(Sleep, simple_sleeps)
{

    test_sleep(std::chrono::microseconds(300), std::chrono::microseconds(1000));
    test_sleep(std::chrono::milliseconds(200), std::chrono::milliseconds(1000));
    test_sleep(std::chrono::milliseconds(1500), std::chrono::milliseconds(2000));
    test_sleep(std::chrono::seconds(1), std::chrono::seconds(2));
}