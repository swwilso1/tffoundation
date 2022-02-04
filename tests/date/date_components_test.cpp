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

#include "TFFoundation.hpp"
#include "gtest/gtest.h"
#include <chrono>

using namespace TF::Foundation;

// These tests will fail if the system uses a different epoch.

TEST(DateComponentsTest, MomentTest)
{
    std::chrono::nanoseconds ns(1509136984185151026L);
    SystemDate::duration d = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDateComponents dc(d);

    EXPECT_EQ(dc.second(), 4);
    EXPECT_EQ(dc.minute(), 43);
    EXPECT_EQ(dc.hour(), 14);
    EXPECT_EQ(dc.dayOfMonth(), 27);
    EXPECT_EQ(dc.monthOfYear(), MonthOfYear::October);
    EXPECT_EQ(dc.year(), 2017);
    EXPECT_EQ(dc.dayOfWeek(), DayOfWeek::Friday);
    EXPECT_EQ(dc.dayOfYear(), 299);
    EXPECT_TRUE(dc.isDaylightSavingsTime());
    EXPECT_FALSE(dc.isLeapYear());
}

TEST(DateComponentsTest, LeapYearTest)
{
    std::chrono::nanoseconds ns(1580136984185151026L);
    SystemDate::duration d = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDateComponents dc(d);
    EXPECT_TRUE(dc.isLeapYear());
}

TEST(DateComponentsTest, RecoverDateTest)
{
    SystemDate d;
    SystemDateComponents dc(d);
    SystemDate e = dc.getDate();
    EXPECT_TRUE(d == e);
}
