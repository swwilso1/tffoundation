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
#include <utility>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;


TEST(StringTest, GetCStringTest)
{
    String s("abcd\\:000080ef\\:004432");
    auto value = s.cStr();
    EXPECT_EQ(strncmp(value.get(), "abcd\\:000080ef\\:004432", 22), 0);
}


TEST(StringTest, GetCStringTest2)
{
    unsigned int s[5];

    s[0] = 0xFEFF;
    s[1] = 0x9483;
    s[2] = 0x100332;
    s[3] = 0x000081;
    s[4] = 'a';

    String t(s, 5);
    auto value = t.cStr();
    EXPECT_EQ(strncmp(value.get(), "\\:009483\\:100332\\:000081a", 25), 0);
}


TEST(StringTest, GetSTLStringTest)
{
    String s("abcd\\:000080ef\\:004432");
    auto value = s.stlString();
    EXPECT_EQ(value, "abcd\\:000080ef\\:004432");
}


TEST(StringTest, GetSTLStringTest2)
{
    unsigned int s[5];

    s[0] = 0xFEFF;
    s[1] = 0x9483;
    s[2] = 0x100332;
    s[3] = 0x000081;
    s[4] = 'a';

    String t(s, 5);
    auto value = t.stlString();
    EXPECT_EQ(value, "\\:009483\\:100332\\:000081a");
}
