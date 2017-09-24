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
SOFTWARE.


******************************************************************************/

#include <cstring>
#include <string>
#include <iostream>
#include <utility>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;


TEST(StringTest, AppendFormatTest)
{
    String s("Hello");
    String t = s.stringByAppendingFormat(" World");
    EXPECT_EQ(t, "Hello World");
}


TEST(StringTest, AppendFormatTest2)
{
    String s("foo");
    String t = s.stringByAppendingFormat("%d bar \\:002341", 10);
    EXPECT_EQ(t, "foo10 bar \\:002341");
}


TEST(StringTest, AppendStringTest)
{
    String s("foo");
    String t("bar");
    String u = s.stringByAppendingString(t);
    EXPECT_EQ(u, "foobar");
}


TEST(StringTest, AppendCStringTest)
{
    String s("foo");
    String u = s.stringByAppendingString("bar");
    EXPECT_EQ(u, "foobar");
}


TEST(StringTest, AppendSTLStringTest)
{
    String s("foo");
    std::string b("bar");
    String u = s.stringByAppendingString(b);
    EXPECT_EQ(u, "foobar");
}


TEST(StringTest, ConcatenateStringTest)
{
    unsigned int arr[5];

    arr[0] = 0xFEFF;
    arr[1] = 0x5848;
    arr[2] = 0x103333;
    arr[3] = 0xFFFF;
    arr[4] = 'J';

    unsigned int barr[5];

    barr[0] = 0xFEFF;
    barr[1] = 0x0034;
    barr[2] = 0x0200;
    barr[3] = 0x1123;
    barr[4] = 'K';

    String s(arr, 5);
    String t(barr, 5);
    String u = String::concatenateStrings(s, t);
    EXPECT_EQ(u, "\\:005848\\:103333\\:00FFFFJ4\\:000200\\:001123K");
}


TEST(StringTest, OperatorPlusCStringTest)
{
    String s("foo");
    String t = s + "bar";
    EXPECT_EQ(t, "foobar");
}


TEST(StringTest, OperatorPlusSTLStringTest)
{
    String s("foo");
    String t = s + std::string("bar");
    EXPECT_EQ(t, "foobar");
}


TEST(StringTest, OperatorPlusStringTest)
{
    String s("foo");
    String t = s + String("bar");
    EXPECT_EQ(t, "foobar");
}






