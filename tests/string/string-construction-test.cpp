/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 Tectiform Inc.

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
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;


TEST(StringTest, DefaultConstructorTest)
{
    String s1;
    EXPECT_TRUE(s1.length() == 0);
}


#define TEST_STRING "test string"

TEST(StringTest, CStyleStringConstructor1)
{
    String s1(TEST_STRING);
    auto ptr = s1.c_str();
    EXPECT_TRUE(strncmp(TEST_STRING, ptr.get(), s1.length()) == 0);
}

#define TEST2_STRING "test 2 string with a bunch more characters in the string"

TEST(StringTest, CStyleStringConstructor2)
{
    String s(TEST2_STRING);
    auto ptr = s.c_str();
    EXPECT_TRUE(strncmp(TEST2_STRING, ptr.get(), s.length()) == 0);
}



