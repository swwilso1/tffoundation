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

#include <vector>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(CommandLine, args_to_command_line_test)
{
    {
        const char * argv[] = {"Simple", "Test", "Line"};
        auto joined_argv = convert_args_to_command_line(3, argv);
        EXPECT_EQ(joined_argv, "Simple Test Line");
    }

    {
        const char * argv[] = {"Simple", "'quoted arg'", "test"};
        auto joined_argv = convert_args_to_command_line(3, argv);
        EXPECT_EQ(joined_argv, "Simple 'quoted arg' test");
    }

    {
        const char * argv[] = {"Different", "'Quoted", "Arg'", "Thing"};
        auto joined_argv = convert_args_to_command_line(4, argv);
        EXPECT_EQ(joined_argv, "Different 'Quoted Arg' Thing");
    }
}

TEST(CommandLine, command_line_to_vector_tests)
{
    {
        String command_line{"Simple test line"};
        std::vector<String> list{};
        auto result = convert_command_line_to_vector(command_line, list);
        EXPECT_TRUE(result);
        EXPECT_EQ(list[0], "Simple");
        EXPECT_EQ(list[1], "test");
        EXPECT_EQ(list[2], "line");
    }

    {
        String command_line{"Arg with 'quote foo'"};
        std::vector<String> list{};
        auto result = convert_command_line_to_vector(command_line, list);
        EXPECT_TRUE(result);
        EXPECT_EQ(list[0], "Arg");
        EXPECT_EQ(list[1], "with");
        EXPECT_EQ(list[2], "quote foo");
    }
}
