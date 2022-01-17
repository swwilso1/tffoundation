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


using namespace TF::Foundation;

TEST(DateFormatter, EraTest)
{
    SystemDateFormatter formatter("G");
    SystemDate now;
    EXPECT_EQ(formatter.stringFromDate(now), "AD");

    SystemDateFormatter formatter2("GG");
    EXPECT_EQ(formatter2.stringFromDate(now), "AD");

    SystemDateFormatter formatter3("GGG");
    EXPECT_EQ(formatter3.stringFromDate(now), "AD");

    SystemDateFormatter formatter4("GGGG");
    EXPECT_EQ(formatter4.stringFromDate(now), "Anno Domini");

    SystemDateFormatter formatter5("GGGGG");
    EXPECT_EQ(formatter5.stringFromDate(now), "A");
}

// From this point on the tests depend on the epoch of the system clock
// of the system running the test.   These tests were written for a system
// running under the Unix epoch.

TEST(DateFormatter, YearTest)
{
    SystemDateFormatter formatter("y");
    std::chrono::nanoseconds ns(1509136984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "2017");

    SystemDateFormatter formatter2("yy");
    EXPECT_EQ(formatter2.stringFromDate(d), "17");

    SystemDateFormatter formatter3("yyy");
    EXPECT_EQ(formatter3.stringFromDate(d), "2017");

    SystemDateFormatter formatter4("yyyy");
    EXPECT_EQ(formatter4.stringFromDate(d), "2017");

    SystemDateFormatter formatter5("yyyyy");
    EXPECT_EQ(formatter5.stringFromDate(d), "02017");
}


TEST(DateFormatter, QuarterTest)
{
    SystemDateFormatter formatter("Q");
    std::chrono::nanoseconds ns(1509136984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "4");

    SystemDateFormatter formatter2("QQ");
    EXPECT_EQ(formatter2.stringFromDate(d), "04");

    SystemDateFormatter formatter3("QQQ");
    EXPECT_EQ(formatter3.stringFromDate(d), "Q4");

    SystemDateFormatter formatter4("QQQQ");
    EXPECT_EQ(formatter4.stringFromDate(d), "4th quarter");
}


TEST(DateFormatter, MonthTest)
{
    SystemDateFormatter formatter("M");
    std::chrono::nanoseconds ns(1505136984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "9");

    SystemDateFormatter formatter2("MM");
    EXPECT_EQ(formatter2.stringFromDate(d), "09");

    SystemDateFormatter formatter3("MMM");
    EXPECT_EQ(formatter3.stringFromDate(d), "Sept");

    SystemDateFormatter formatter4("MMMM");
    EXPECT_EQ(formatter4.stringFromDate(d), "September");

    SystemDateFormatter formatter5("MMMMM");
    EXPECT_EQ(formatter5.stringFromDate(d), "S");
}


TEST(DateFormatter, DayOfMonthTest)
{
    SystemDateFormatter formatter("d");
    std::chrono::nanoseconds ns(1505016984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "9");

    SystemDateFormatter formatter2("dd");
    EXPECT_EQ(formatter2.stringFromDate(d), "09");
}


TEST(DateFormatter, DayOfYearTest)
{
    SystemDateFormatter formatter("D");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "5");

    SystemDateFormatter formatter2("DD");
    EXPECT_EQ(formatter2.stringFromDate(d), "05");

    SystemDateFormatter formatter3("DDD");
    EXPECT_EQ(formatter3.stringFromDate(d), "005");
}


TEST(DateFormatter, WeekDayTest)
{
    SystemDateFormatter formatter("E");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "Tues");

    SystemDateFormatter formatter2("EE");
    EXPECT_EQ(formatter2.stringFromDate(d), "Tues");

    SystemDateFormatter formatter3("EEE");
    EXPECT_EQ(formatter3.stringFromDate(d), "Tues");

    SystemDateFormatter formatter4("EEEE");
    EXPECT_EQ(formatter4.stringFromDate(d), "Tuesday");

    SystemDateFormatter formatter5("EEEEE");
    EXPECT_EQ(formatter5.stringFromDate(d), "T");
}


TEST(DateFormatter, PeriodTest)
{
    SystemDateFormatter formatter("a");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "AM");

    SystemDateFormatter formatter2("aaa");
    EXPECT_EQ(formatter2.stringFromDate(d), "AM");

    SystemDateFormatter formatter3("aaaa");
    EXPECT_EQ(formatter3.stringFromDate(d), "AM");

    SystemDateFormatter formatter4("aaaaa");
    EXPECT_EQ(formatter4.stringFromDate(d), "A");
}


TEST(DateFormatter, StandardHourTest)
{
    SystemDateFormatter formatter("h");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "2");

    SystemDateFormatter formatter2("hh");
    EXPECT_EQ(formatter2.stringFromDate(d), "02");
}

TEST(DateFormatter, MilitaryHourTest)
{
    SystemDateFormatter formatter("H");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "2");

    SystemDateFormatter formatter2("HH");
    EXPECT_EQ(formatter2.stringFromDate(d), "02");
}


TEST(DateFormatter, NonstandardHourTest)
{
    SystemDateFormatter formatter("K");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "2");

    SystemDateFormatter formatter2("KK");
    EXPECT_EQ(formatter2.stringFromDate(d), "02");
}


TEST(DateFormatter, NonstandardMilitaryHourTest)
{
    SystemDateFormatter formatter("k");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "3");

    SystemDateFormatter formatter2("kk");
    EXPECT_EQ(formatter2.stringFromDate(d), "03");
}


TEST(DateFormatter, MinuteTest)
{
    SystemDateFormatter formatter("m");
    std::chrono::nanoseconds ns(1420531684185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "8");

    SystemDateFormatter formatter2("mm");
    EXPECT_EQ(formatter2.stringFromDate(d), "08");
}


TEST(DateFormatter, SecondTest)
{
    SystemDateFormatter formatter("s");
    std::chrono::nanoseconds ns(1420536966185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "6");

    SystemDateFormatter formatter2("ss");
    EXPECT_EQ(formatter2.stringFromDate(d), "06");
}


// Helper function for FractionalSecondTest.  Some machines have system clocks
// with a duration period in the microseconds rather than the nanoseconds.
// We use this function to recover the period denominator to check for microseconds.
template<typename T>
std::intmax_t period_check(void)
{
    return T::den;
}


TEST(DateFormatter, FractionalSecondTest)
{
    SystemDateFormatter formatter("SSSSS");
    std::chrono::nanoseconds ns(1420536984185151026L);
    SystemDate::duration dur = std::chrono::duration_cast<SystemDate::duration>(ns);
    SystemDate d(dur);
    EXPECT_EQ(formatter.stringFromDate(d), "18515");

    SystemDateFormatter formatter2("SSSSSS");
    EXPECT_EQ(formatter2.stringFromDate(d), "185151");

    SystemDateFormatter formatter3("SSSSSSS");
    EXPECT_EQ(formatter3.stringFromDate(d), "1851510");

    // If the system clock is not a nanosecond clock, then the following two tests
    // will fail due a lack of precision in the clock.   So only test
    // if the duration period is in the nanoseconds.
    if(period_check<SystemDate::duration::period>() >= 1000000000L)
    {
        SystemDateFormatter formatter4("SSSSSSSS");
        EXPECT_EQ(formatter4.stringFromDate(d), "18515103");

        SystemDateFormatter formatter5("SSSSSSSSS");
        EXPECT_EQ(formatter5.stringFromDate(d), "185151026");
    }
}


TEST(DateFormatter, DateFromStringTest)
{
    SystemDateFormatter formatter("yyyy-MM-dd HH:mm:ss:SSSS");
    SystemDate d;

    auto theFormattedDate = formatter.stringFromDate(d);

    auto theRecoveredDate = formatter.dateFromString(theFormattedDate);
    auto theSecondFormattedDate = formatter.stringFromDate(d);

    EXPECT_TRUE(theFormattedDate == theSecondFormattedDate);
}


TEST(DateFormatter, DateFromStringWithSingleDigitsTest)
{
    SystemDateFormatter formatter("yyyy-M-d H:m:s:SSSS");
    SystemDate d;

    auto theFormattedDate = formatter.stringFromDate(d);

    EXPECT_ANY_THROW(formatter.dateFromString(theFormattedDate));
}
