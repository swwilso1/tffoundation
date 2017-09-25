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


TEST(StringTest, CompareTest)
{
    String s("abcdef");
    String t("acdefg");
    EXPECT_EQ(s.compare(t), OrderedAscending);
}


TEST(StringTest, CompareTest2)
{
    String s("abcdef");
    String t("abcdef");
    EXPECT_EQ(s.compare(t), OrderedSame);
}



TEST(StringTest, CompareTest3)
{
    String s("abcdef");
    String t("aacdef");
    EXPECT_EQ(s.compare(t), OrderedDescending);
}


TEST(StringTest, CompareTest4)
{
    String s("\\:001234\\:002332\\:004889");
    String t("\\:001234\\:002333\\:004434");
    EXPECT_EQ(s.compare(t), OrderedAscending);
}


TEST(StringTest, CompareTest5)
{
    String s("\\:001234\\:002332\\:004889");
    String t("\\:001234\\:002332\\:004889");
    EXPECT_EQ(s.compare(t), OrderedSame);
}


TEST(StringTest, CompareTest6)
{
    String s("\\:001234\\:002332\\:004435");
    String t("\\:001234\\:002332\\:004434");
    EXPECT_EQ(s.compare(t), OrderedDescending);
}


TEST(StringTest, CompareRangeWithStringTest)
{
    String s("Mostly cloud, 52 \\:0000B0F");
    EXPECT_EQ(s.compareRangeWithString(Range(0,6), "Lastly"), OrderedDescending);
}


TEST(StringTest, CompareRangeWithStringTest2)
{
    String s("Mostly cloud, 52 \\:0000B0F");
    EXPECT_EQ(s.compareRangeWithString(Range(7,5), "club"), OrderedAscending);
}


TEST(StringTest, CompareRangeWithStringTest3)
{
    String s("Mostly cloud, 52 \\:0000B0F");
    EXPECT_EQ(s.compareRangeWithString(Range(17,2), "\\:0000B0F"), OrderedSame);
}


TEST(StringTest, HasPrefixTest)
{
    String s("Mostly cloudy, 52 \\:0000B0F");
    EXPECT_TRUE(s.hasPrefix("Mostly"));
}


TEST(StringTest, HasPrefixTest2)
{
    String s("Mostly cloudy, 52 \\:0000B0F");
    EXPECT_FALSE(s.hasPrefix("cloudy2"));
}


TEST(StringTest, HasSuffixTest)
{
    String s("Mostly cloudy, 52 \\:0000B0F");
    EXPECT_TRUE(s.hasSuffix(" \\:0000B0F"));
}


TEST(StringTest, HasSuffixTest2)
{
    String s("Mostly cloudy, 52 \\:0000B0F");
    EXPECT_FALSE(s.hasPrefix("52"));
}


TEST(StringTest, IsEqualToStringTest)
{
    String s("Mostly cloudy, 52 \\:0000B0F");
    EXPECT_TRUE(s.isEqualToString("Mostly cloudy, 52 \\:0000B0F"));
}


TEST(StringTest, IsEqualToStringTest2)
{
    String s("Mostly cloudy, 52 \\:0000B0F");
    EXPECT_FALSE(s.isEqualToString("Mostly"));
}

