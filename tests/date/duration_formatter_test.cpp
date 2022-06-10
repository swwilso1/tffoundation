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

TEST(DurationFormatter, compile)
{
    std::chrono::seconds s{10};
    DurationFormatter formatter("sss");
    EXPECT_EQ(formatter.string_from_duration(s), "010");
}

TEST(DurationFormatter, basic_symbol_tests)
{
    {
        // Test normal symbols, independent of the duration.
        std::chrono::seconds s{4942924923}; // This value doesn't matter for this test.
        DurationFormatter formatter{"abczefg;ijklv"};
        EXPECT_EQ(formatter.string_from_duration(s), "abczefg;ijklv");
    }

    // The following tests all exercise the 1 symbol form of the formatting spec var.

    {
        // Test microsecond symbol
        std::chrono::microseconds m{456};
        DurationFormatter formatter{"U"};
        EXPECT_EQ(formatter.string_from_duration(m), "456");
    }

    {
        // Test the millisecond symbol
        std::chrono::milliseconds m{345};
        DurationFormatter formatter{"S"};
        EXPECT_EQ(formatter.string_from_duration(m), "345");
    }

    {
        // Test the seconds symbol
        std::chrono::seconds m{59};
        DurationFormatter formatter{"s"};
        EXPECT_EQ(formatter.string_from_duration(m), "59");
    }

    {
        // Test the minutes symbol
        std::chrono::minutes m{45};
        DurationFormatter formatter{"m"};
        EXPECT_EQ(formatter.string_from_duration(m), "45");
    }

    {
        // Test the hours symbol
        std::chrono::hours h{23};
        DurationFormatter formatter{"h"};
        EXPECT_EQ(formatter.string_from_duration(h), "23");
    }

    {
        // Test the days symbol
        std::chrono::days d{6};
        DurationFormatter formatter{"d"};
        EXPECT_EQ(formatter.string_from_duration(d), "6");
    }

    {
        // Test the weeks symbol
        std::chrono::weeks w{3};
        DurationFormatter formatter{"W"};
        EXPECT_EQ(formatter.string_from_duration(w), "3");
    }

    {
        // Test the months symbol
        std::chrono::months m{11};
        DurationFormatter formatter{"M"};
        EXPECT_EQ(formatter.string_from_duration(m), "11");
    }

    {
        // Test the years symbol
        std::chrono::years y{5};
        DurationFormatter formatter{"Y"};
        EXPECT_EQ(formatter.string_from_duration(y), "5");
    }

    {
        // Test the decades symbol
        decades d{9};
        DurationFormatter formatter{"D"};
        EXPECT_EQ(formatter.string_from_duration(d), "9");
    }

    {
        // Test the centuries symbol
        centuries c{3};
        DurationFormatter formatter{"C"};
        EXPECT_EQ(formatter.string_from_duration(c), "3");
    }

    {
        // Test the millenia symbol
        millenia m{23};
        DurationFormatter formatter{"G"};
        EXPECT_EQ(formatter.string_from_duration(m), "23");
    }
}

TEST(DurationFormatter, multiple_symbol_form_tests)
{
    {
        // Test the microseconds symbol
        std::chrono::microseconds m{562};
        DurationFormatter formatter{"UU"};
        EXPECT_EQ(formatter.string_from_duration(m), "56");

        // Test more microseconds
        DurationFormatter formatter2{"UUU"};
        EXPECT_EQ(formatter2.string_from_duration(m), "562");

        DurationFormatter formatter3{"UUUUUUUU"};
        EXPECT_EQ(formatter3.string_from_duration(m), "00000562");
    }

    {
        // Test the milliseconds symbol
        std::chrono::milliseconds m{988};
        DurationFormatter formatter{"SS"};
        EXPECT_EQ(formatter.string_from_duration(m), "98");

        DurationFormatter formatter2{"SSS"};
        EXPECT_EQ(formatter2.string_from_duration(m), "988");

        DurationFormatter formatter3{"SSSSS"};
        EXPECT_EQ(formatter3.string_from_duration(m), "00988");
    }

    {
        // Test the seconds symbol
        std::chrono::seconds s{46};
        DurationFormatter formatter{"ss"};
        EXPECT_EQ(formatter.string_from_duration(s), "46");

        DurationFormatter formatter2{"sssss"};
        EXPECT_EQ(formatter2.string_from_duration(s), "00046");
    }

    {
        // Test the minutes symbol
        std::chrono::minutes m{46};
        DurationFormatter formatter{"mm"};
        EXPECT_EQ(formatter.string_from_duration(m), "46");

        DurationFormatter formatter2{"mmmmm"};
        EXPECT_EQ(formatter2.string_from_duration(m), "00046");
    }

    {
        // Test the hours symbol
        std::chrono::hours h{22};
        DurationFormatter formatter{"hh"};
        EXPECT_EQ(formatter.string_from_duration(h), "22");

        DurationFormatter formatter2{"hhhhh"};
        EXPECT_EQ(formatter2.string_from_duration(h), "00022");
    }

    {
        // Test the days symbol
        std::chrono::days d{6};
        DurationFormatter formatter{"dd"};
        EXPECT_EQ(formatter.string_from_duration(d), "06");

        DurationFormatter formatter2{"ddddd"};
        EXPECT_EQ(formatter2.string_from_duration(d), "00006");
    }

    {
        // Test the weeks symbol
        std::chrono::weeks w{4};
        DurationFormatter formatter{"WW"};
        EXPECT_EQ(formatter.string_from_duration(w), "04");

        DurationFormatter formatter2{"WWWWW"};
        EXPECT_EQ(formatter2.string_from_duration(w), "00004");
    }

    {
        // Test the months symbol
        std::chrono::months m{11};
        DurationFormatter formatter{"MM"};
        EXPECT_EQ(formatter.string_from_duration(m), "11");

        DurationFormatter formatter2{"MMMMMM"};
        EXPECT_EQ(formatter2.string_from_duration(m), "000011");
    }

    {
        // Test the years symbol
        std::chrono::years y{3};
        DurationFormatter formatter{"YY"};
        EXPECT_EQ(formatter.string_from_duration(y), "03");

        DurationFormatter formatter2{"YYYY"};
        EXPECT_EQ(formatter2.string_from_duration(y), "0003");
    }

    {
        // Test the decades symbol
        decades d{7};
        DurationFormatter formatter{"DD"};
        EXPECT_EQ(formatter.string_from_duration(d), "07");

        DurationFormatter formatter2{"0000DDDD0000"};
        EXPECT_EQ(formatter2.string_from_duration(d), "000000070000");
    }

    {
        // Test the centuries symbol
        centuries c{9};
        DurationFormatter formatter{"CC"};
        EXPECT_EQ(formatter.string_from_duration(c), "09");

        DurationFormatter formatter2{"***CCCCC***"};
        EXPECT_EQ(formatter2.string_from_duration(c), "***00009***");
    }

    {
        // Test the millenia symbol
        millenia m{9430};
        DurationFormatter formatter{"GG"};
        EXPECT_EQ(formatter.string_from_duration(m), "94");

        DurationFormatter formatter2{"GGGGG"};
        EXPECT_EQ(formatter2.string_from_duration(m), "09430");
    }
}

TEST(DurationFormatter, duration_from_string)
{
    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("00:00:00.001");
        decltype(d) one{1};
        EXPECT_EQ(d.count(), one.count());
    }

    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("00:00:01.111");
        decltype(d) one{1111};
        EXPECT_EQ(d.count(), one.count());
    }

    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("00:00:11.111");
        decltype(d) one{11111};
        EXPECT_EQ(d.count(), one.count());
    }

    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("00:01:11.111");
        decltype(d) one{71111};
        EXPECT_EQ(d.count(), one.count());
    }

    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("00:11:11.111");
        decltype(d) one{671111};
        EXPECT_EQ(d.count(), one.count());
    }

    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("01:11:11.111");
        decltype(d) one{4271111};
        EXPECT_EQ(d.count(), one.count());
    }

    {
        DurationFormatter formatter("hh:mm:ss.SSS");
        auto d = formatter.duration_from_string<int, std::milli>("11:11:11.111");
        decltype(d) one{40271111};
        EXPECT_EQ(d.count(), one.count());
    }
}
