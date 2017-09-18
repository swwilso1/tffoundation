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


TEST(StringTest, DecimalNumberTest)
{
    int f {10};
    String s = String::initWithFormat("%d", f);
    String t("10");

    EXPECT_TRUE(s == t);
}

TEST(StringTest, DecimalNumberTest2)
{
    int f {2147483647};
    String s = String::initWithFormat("%dten", f);
    String t("2147483647ten");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, DecimalNumberTest3)
{
    int f {-1};
    String s = String::initWithFormat("%d", f);
    String t = String("-1");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, OctalNumberTest)
{
    int f{10};
    String s = String::initWithFormat("%o", f);
    String t("12");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, OctalNumberTest2)
{
    int f {2147483647};
    String s = String::initWithFormat("%ooctal", f);
    String t("17777777777octal");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, OctalNumberTest3)
{
    int f {-8};
    String s = String::initWithFormat("%o", f);
    String t("37777777770");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, LowerCaseHexNumberTest)
{
    int f {10};
    String s = String::initWithFormat("%x",f);
    String t("a");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, LowerCaseHexNumberTest2)
{
    int f {2147483647};
    String s = String::initWithFormat("%xhex", f);
    String t = String("7fffffffhex");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, LowerCaseHexNumberTest3)
{
    int f {-16};
    String s = String::initWithFormat("%x", f);
    String t = String("fffffff0");
    EXPECT_TRUE(s == t);
}



TEST(StringTest, UpperCaseHexNumberTest)
{
    int f {10};
    String s = String::initWithFormat("%X",f);
    String t("A");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, UpperCaseHexNumberTest2)
{
    int f {2147483647};
    String s = String::initWithFormat("%Xhex", f);
    String t("7FFFFFFFhex");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, UpperCaseHexNumberTest3)
{
    int f {-16};
    String s = String::initWithFormat("%X", f);
    String t("FFFFFFF0");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, UnsignedIntNumberTest)
{
    unsigned int f {2147483648};
    String s = String::initWithFormat("%u", f);
    String t("2147483648");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, UnsignedIntNumberTest2)
{
    unsigned int f {4294967295};
    String s = String::initWithFormat("%u", f);
    String t("4294967295");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, UnsignedLongNumberTest)
{
    unsigned long f {4294967296};
    String s = String::initWithFormat("%U", f);
    String t("4294967296");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, UnsignedLongNumberTest2)
{
    unsigned long f {18446744073709551615UL};
    String s = String::initWithFormat("%U", f);
    String t("18446744073709551615");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, SingleCharacterTest)
{
    char c = '9';
    String s = String::initWithFormat("%c", c);
    String t("9");
    EXPECT_EQ(s,t);
}


TEST(StringTest, CStyleStringTest)
{
    String s = String::initWithFormat("%s", "Hello World");
    String t("Hello World");
    EXPECT_EQ(s,t);
}


TEST(StringTest, STLStyleStringTest)
{
    std::string j("Hello World");
    String s = String::initWithFormat("%S", j);
    String t("Hello World");
    EXPECT_EQ(s,t);
}


TEST(StringTest, FRealNumberTest)
{
    float f {3.141561};
    String s = String::initWithFormat("%f", f);
    String t("3.141561");
    EXPECT_EQ(s,t);
}


TEST(StringTest, GRealNumberTest)
{
    float f {3.141561};
    String s = String::initWithFormat("%g", f);
    String t("3.14156");
    EXPECT_EQ(s,t);
}


TEST(StringTest, ERealNumberTest)
{
    auto f {3.141561};
    String s = String::initWithFormat("%e", f);
    String t("3.141561e+00");
    EXPECT_EQ(s,t);
}


TEST(StringTest, StringConverterTest)
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
    String t = String::initWithFormat("%@", &s);
    String u("\\:002603\\:002604\\:01D306");
    EXPECT_EQ(t, u);
}


TEST(StringTest, StringConverterTest2)
{
    String s("Hello World!");
    String t = String::initWithFormat("%@",s);
    String u("Hello World!");
    EXPECT_EQ(t,u);
}


TEST(StringTest, ZeroModifierTest)
{
    String s = String::initWithFormat("%04d", 1);
    String t("0001");
    EXPECT_EQ(s,t);
}



