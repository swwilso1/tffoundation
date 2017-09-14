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
#include <string>
#include <iostream>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;


TEST(StringTest, DefaultConstructorTest)
{
    String s1;
    EXPECT_TRUE(s1.length() == 0);
}


#define TEST_STRING "test string"

TEST(StringTest, CStyleStringConstructor1Test)
{
    String s1(TEST_STRING);
    auto ptr = s1.c_str();
    EXPECT_TRUE(strncmp(TEST_STRING, ptr.get(), s1.length()) == 0);
}

#define TEST2_STRING "test 2 string with a bunch more characters in the string"

TEST(StringTest, CStyleStringConstructor2Test)
{
    String s(TEST2_STRING);
    auto ptr = s.c_str();
    EXPECT_TRUE(strncmp(TEST2_STRING, ptr.get(), s.length()) == 0);
}


#define HELLO_WORLD "hello world"

TEST(StringTest, CStringConstructorWithLengthTest)
{
    auto tmp = new char[40];
    std::memcpy(reinterpret_cast<void *>(tmp), reinterpret_cast<void *>(
                    const_cast<char *>(HELLO_WORLD)),
        strlen(HELLO_WORLD));
    String s(tmp, strlen(HELLO_WORLD));
    auto ptr = s.c_str();
    EXPECT_TRUE(strncmp(HELLO_WORLD, ptr.get(), s.length()) == 0);
}


TEST(StringTest, CXXStyleStringConstructorTest)
{
    std::string ss(HELLO_WORLD);
    String s(ss);
    auto ptr = s.c_str();
    EXPECT_TRUE(strncmp(HELLO_WORLD, ptr.get(), s.length()) == 0);
}



TEST(StringTest, UTF8StringConstructorTest)
{
    unsigned char tmp[10];

    tmp[0] = 0xE2;
    tmp[1] = 0x98;
    tmp[2] = 0x83;
    tmp[3] = 0xE2;
    tmp[4] = 0x98;
    tmp[5] = 0x84;
    tmp[6] = 0xF0;
    tmp[7] = 0x9D;
    tmp[8] = 0x8C;
    tmp[9] = 0x86;

    String s(tmp, 10);

    EXPECT_TRUE(s.length() == 3);
}


TEST(StringTest, UTF16StringConstructorTest)
{
    unsigned short tmp [7];

    tmp[0] = 0xFEFF;
    tmp[1] = 0x3E6;
    tmp[2] = 0x419;
    tmp[3] = 0x46A;
    tmp[4] = 0x5D0;
    tmp[5] = 0xD800;
    tmp[6] = 0xDF88;

    String s(tmp, 7);

    EXPECT_TRUE(s.length() == 5);
}


TEST(StringTest, UTF32StringConstructorTest)
{
    unsigned int tmp[7];

    tmp[0] = 0xFEFF;
    tmp[1] = 0x10388;
    tmp[2] = 0x5D0;
    tmp[3] = 0xD800;
    tmp[4] = 0x435;
    tmp[5] = 0x2939;
    tmp[6] = 0x10302;

    String s(tmp, 7);

    EXPECT_TRUE(s.length() == 6);
}


TEST(StringTest, CopyConstructorTest)
{
    String s("foo bar bat");
    String t(s);

    EXPECT_TRUE(s == t);
}


TEST(StringTest, RValueConstructorTest)
{
    String s(String::initWithFormat("foo bar"));
    auto ptr = s.c_str();
    EXPECT_TRUE(strncmp("foo bar", ptr.get(), s.length()) == 0);
}

