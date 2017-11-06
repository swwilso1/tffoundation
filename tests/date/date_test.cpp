/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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

TEST(DateTest, NotEqualTest)
{
    SystemDate d;
    SystemDate e;
    EXPECT_NE(d, e);
}


TEST(DateTest, CopyConstructorTest)
{
    SystemDate d;
    SystemDate e(d);
    EXPECT_EQ(d, e);
}


TEST(DateTest, DurationConstructorTest)
{
    SystemDate::duration dur(1219293949);
    SystemDate d(dur);
    SystemDate e;
    EXPECT_LT(d, e);
}


TEST(DateTest, AssignmentOperatorTest)
{
    SystemDate d;
    SystemDate e;
    e = d;
    EXPECT_EQ(d, e);
}


TEST(DateTest, OperatorEqualTest)
{
    SystemDate::duration dur(129293);
    SystemDate d(dur);
    SystemDate e(dur);
    EXPECT_TRUE(d == e);
}


TEST(DateTest, OperatorNotEqualTest)
{
    SystemDate::duration dur(129293334);
    SystemDate d(dur);
    SystemDate e(dur + SystemDate::duration(10));
    EXPECT_TRUE(d != e);
}


TEST(DateTest, OperatorLessThanTest)
{
    SystemDate::duration dur(239239);
    SystemDate d(dur);
    SystemDate e(dur + SystemDate::duration(3489239));
    EXPECT_TRUE(d < e);
}


TEST(DateTest, OperatorLessThanEqualToTest)
{
    SystemDate::duration dur(2492392399239L);
    SystemDate d(dur - SystemDate::duration(10));
    SystemDate e(dur);
    EXPECT_TRUE(d <= e);
    EXPECT_TRUE((d + SystemDate::duration(10)) <= e);
}


TEST(DateTest, OperatorGreaterThanEqualToTest)
{
    SystemDate::duration dur(2349239239);
    SystemDate d(dur);
    SystemDate e(dur + SystemDate::duration(10));
    EXPECT_TRUE( e >= d);
    EXPECT_TRUE( e >= (d + SystemDate::duration(10)));
}


TEST(DateTest, OperatorGreaterThanTest)
{
    SystemDate::duration dur(2939239);
    SystemDate d(dur);
    SystemDate e;
    EXPECT_TRUE(e > d);
}


TEST(DateTest, OperatorPlusTest)
{
    SystemDate d;
    SystemDate e(d + SystemDate::duration(10));
    EXPECT_EQ(e, d + SystemDate::duration(10));
}


TEST(DateTest, OperatorMinusTest)
{
    SystemDate d;
    SystemDate e(d - SystemDate::duration(100));
    EXPECT_EQ(e, d - SystemDate::duration(100));
}


TEST(DateTest, OperatorPlusEqualTest)
{
    SystemDate d;
    SystemDate e(d + SystemDate::duration(100));
    d += SystemDate::duration(100);
    EXPECT_EQ(d, e);
}


TEST(DateTest, OperatorMinusEqualTest)
{
    SystemDate d;
    SystemDate e(d - SystemDate::duration(100));
    d -= SystemDate::duration(100);
    EXPECT_EQ(d, e);
}


TEST(DateTest, DateMinusTest)
{
    SystemDate d;
    SystemDate e(d - SystemDate::duration(100));
    auto diff = d - e;
    EXPECT_EQ(diff.count(), 100);
}


//TEST(DateTest, OstreamTest)
//{
//    SystemDate d;
//    std::cout << d << std::endl;
//}
