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
#include <iostream>
#include <unistd.h>

using namespace TF::Foundation;

using progress_meter_type = ProgressMeter<uint32_t>;

TEST(ProgressMeter, compile_test)
{
    const progress_meter_type::size_type total = 32768;
    uint32_t previous_progress{0};

    progress_meter_type meter(total, [&previous_progress](uint32_t progress) -> void {
        EXPECT_GT(progress, previous_progress);
    });

    for (progress_meter_type::size_type i = 0; i < total; i++)
    {
        meter.increment();
        meter.notify();

        if ((i % 1024) == 0)
        {
            Sleep(std::chrono::milliseconds(50));
        }
    }
}
