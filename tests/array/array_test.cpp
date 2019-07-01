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

TEST(ArrayTest, DefaultConstructorTest)
{
    Array<int> a;
    EXPECT_EQ(0, a.size());
    EXPECT_EQ(nullptr, a.data());
}


TEST(ArrayTest, RawArrayConstructorTest)
{
    int a[10];
    for(int i = 0; i < 10; i++)
        a[i] = i;

    Array<int> b(a, 10);

    for(int i = 0; i < 10; i++)
        EXPECT_EQ(i, b[i]);
}


TEST(ArrayTest, SizeConstructorTest)
{
    Array<int> a(10);
    EXPECT_EQ(10, a.size());
    EXPECT_NE(nullptr, a.data());
}


TEST(ArrayTest, LargeSizeConstructorTest)
{
    unsigned int i = 2147483646;
    Array<char> a(i);
    EXPECT_EQ(i, a.size());
    EXPECT_NE(nullptr, a.data());
}


TEST(ArrayTest, CopyConstructorTest)
{
    Array<int> a(10);
    for(int i = 0; i < 10; i++)
        a[i] = i;

    Array<int> b(a);

    EXPECT_EQ(10, b.size());

    for(int i = 0; i < 10; i++)
        EXPECT_EQ(i, b[i]);
}


TEST(ArrayTest, RValueConstructorTest)
{
    Array<int> a = Array<int>(10);

    EXPECT_EQ(10, a.size());
}


TEST(ArrayTest, AssignmentOperatorTest)
{
    Array<int> a(256);

    for(int i = 0; i < 256; i++)
        a[i] = i;

    Array<int> b(100);

    for(int i = 0; i < 100; i++)
        b[i] = i * 2;

    b = a;

    EXPECT_NE(100, b.size());
    EXPECT_EQ(256, b.size());

    for(int i = 0; i < 256; i++)
        EXPECT_EQ(i, b[i]);
}


Array<int> rvalueAssignmentOperatorTestHelper()
{
    Array<int> a(100);

    for(int i = 0; i < 100; i++)
        a[i] = i * 2;

    return a;
}


TEST(ArrayTest, RValueAssignmentOperatorTest)
{
    Array<int> a(200);

    for(int i = 0; i < 200; i++)
        a[i] = i * 8;

    a = rvalueAssignmentOperatorTestHelper();

    EXPECT_EQ(100, a.size());

    for(int i = 0; i < 100; i++)
        EXPECT_EQ(i * 2, a[i]);
}


TEST(ArrayTest, NonConstIteratorTest)
{
    Array<int> a(200);

    for(int i = 0; i < 200; i++)
        a[i] = i;

    int i = 0;
    for(auto iter = a.begin(); iter != a.end(); iter++, i++)
        EXPECT_EQ(i, *iter);
}


TEST(ArrayTest, NonConstIteratorAutoTest)
{
    Array<double> a(300);

    for(int i = 0; i < 300; i++)
        a[i] = static_cast<double>(i);

    int i = 0;
    for(auto e : a)
    {
        EXPECT_EQ(static_cast<double>(i), e);
        i++;
    }
}


TEST(ArrayTest, AtTest)
{
    Array<char> a(20);

    for(int i = 0; i < 20; i++)
        a[i] = static_cast<char>(i);

    for(int i = 0; i < 20; i++)
        EXPECT_EQ(i, a.at(i));
}


TEST(ArrayTest, FrontTest)
{
    Array<char> a(25);

    for(int i = 0; i < 25; i++)
        a[i] = static_cast<char>(i);

    EXPECT_EQ(0, a.front());
}


TEST(ArrayTest, BackTest)
{
    Array<char> a(25);

    for(int i = 0; i < 25; i++)
        a[i] = static_cast<char>(i);

    EXPECT_EQ(24, a.back());
}


TEST(ArrayTest, DataTest)
{
    Array<char> a(25);

    for(int i = 0; i < 25; i++)
        a[i] = static_cast<char>(i);

    char *theData = a.data();

    for(int i = 0; i < 25; i++)
        EXPECT_EQ(i, *(theData + i));
}


TEST(ArrayTest, EmptyTest)
{
    Array<int> a;
    Array<int> b(1);

    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(b.empty());
}


TEST(ArrayTest, SizeTest)
{
    Array<int> a;
    Array<double> b(5000);

    EXPECT_EQ(0, a.size());
    EXPECT_EQ(5000, b.size());
}


TEST(ArrayTest, FillTest)
{
    Array<double> d(5000);

    d.fill(3.14);

    for(int i = 0; i < 5000; i++)
        EXPECT_EQ(3.14, d[i]);
}


TEST(ArrayTest, SwapTest)
{
    Array<int> a(40);
    Array<int> b(100);

    for(int i = 0; i < 40; i++)
        a[i] = i;

    for(int i = 0; i < 100; i++)
        b[i] = i;

    a.swap(b);

    EXPECT_EQ(100, a.size());
    EXPECT_EQ(40, b.size());

    for(int i = 0; i < 100; i++)
        EXPECT_EQ(i, a[i]);

    for(int i = 0; i < 40; i++)
        EXPECT_EQ(i, b[i]);
}


TEST(ArrayTest, OperatorDataTest)
{
    Array<unsigned short> a(4);

    a.fill(1);

    Data d = a;

    EXPECT_EQ(4 * sizeof(unsigned short), d.length());

    EXPECT_TRUE((d[0] == 1 && d[1] == 0) || (d[0] == 0 && d[1] == 1));
    EXPECT_TRUE((d[2] == 1 && d[3] == 0) || (d[2] == 0 && d[3] == 1));
}