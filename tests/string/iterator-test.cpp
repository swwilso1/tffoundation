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
SOFTWARE.


******************************************************************************/

#include <cstring>
#include <string>
#include <iostream>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(StringTest, EmptyStringIteratorsTest)
{
    String s;
    auto b = s.begin();
    auto e = s.end();
    EXPECT_EQ(b, e);
}

TEST(StringTest, BeginEndIteratorsTest)
{
    String s("Hello World");
    auto b = s.begin();
    auto e = s.end();
    EXPECT_NE(b, e);
}

TEST(StringTest, IteratorValueTest)
{
    String s("Hello World");
    auto b = s.begin();
    EXPECT_EQ(*b, 'H');

    ++b;
    EXPECT_EQ(*b, 'e');

    ++b;
    EXPECT_EQ(*b, 'l');

    ++b;
    EXPECT_EQ(*b, 'l');

    ++b;
    EXPECT_EQ(*b, 'o');

    ++b;
    EXPECT_EQ(*b, ' ');

    ++b;
    EXPECT_EQ(*b, 'W');

    ++b;
    EXPECT_EQ(*b, 'o');

    ++b;
    EXPECT_EQ(*b, 'r');

    ++b;
    EXPECT_EQ(*b, 'l');

    ++b;
    EXPECT_EQ(*b, 'd');

    ++b;
    EXPECT_EQ(b, s.end());
}

TEST(StringTest, UnicodeValueTest)
{
    String s = String::initWithASCIIEncodedUnicode("a\\:002387\\:002231");
    auto b = s.begin();

    EXPECT_EQ(*b, 'a');

    ++b;
    EXPECT_EQ(*b, 9095);

    ++b;
    EXPECT_EQ(*b, 8753);

    ++b;
    EXPECT_EQ(b, s.end());
}

TEST(StringTest, SimpleForLoopTest)
{
    String s = String::initWithASCIIEncodedUnicode("abcdefg\\:10FFFAq\\:005422");
    for (auto i = s.begin(); i != s.end(); ++i)
    {
        EXPECT_NE(i, s.end());
    }
}

TEST(StringTest, StrongerForLoopTest)
{
    String s = String::initWithASCIIEncodedUnicode("\\:003293aqqweiewi\\:10ADD3");
    for (auto u : s)
    {
        EXPECT_NE(u, 0);
    }
}