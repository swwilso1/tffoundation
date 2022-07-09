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

TEST(Process, simple_test)
{
    Process this_process{};
    EXPECT_TRUE(this_process.is_alive());
}

TEST(Process, launch_test)
{
    Process ls_process{"/bin/ls"};
    ls_process.launch();
    while (ls_process.is_alive())
    {
        Sleep(std::chrono::milliseconds(100));
        ls_process.update_exit_status();
    }
    auto & stdout_handle = ls_process.handle_for_standard_out();
    auto data = stdout_handle.readToEndOfFile();
    EXPECT_GT(data.length(), 0);
    EXPECT_FALSE(ls_process.is_alive());
    EXPECT_EQ(ls_process.stop_code(), 0);
}
