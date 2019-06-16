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

TEST(StringTest, ReplacingOccurencesOfSubstringTest)
{
    String s("Mostly cloudy, 46F");
    auto t = s.stringByReplacingOccurencesOfStringWithString(String("cloudy"), String("sunny"));
    EXPECT_EQ(t, "Mostly sunny, 46F");
}


TEST(StringTest, ReplacingOccurencesOfSubstringTest2)
{
    String s("Mostly cloudy, 46F");
    auto t = s.stringByReplacingOccurencesOfStringWithString("cloudy", "sunny");
    EXPECT_EQ(t, "Mostly sunny, 46F");
}


TEST(StringTest, ReplacingOccurencesOfSubstringTest3)
{
    String s("Mostly cloudy, 46F");
    auto t = s.stringByReplacingOccurencesOfStringWithString(std::string("cloudy"), std::string("sunny"));
    EXPECT_EQ(t, "Mostly sunny, 46F");
}


TEST(StringTest, ReplacingOccurencesOfSubstringTest4)
{
    String s("Mostly cloudy, 46F.  Should stay cloudy all day");
    auto t = s.stringByReplacingOccurencesOfStringWithString(String("cloudy"), String("sunny"));
    EXPECT_EQ(t, "Mostly sunny, 46F.  Should stay sunny all day");
}



TEST(StringTest, ReplacingOccurencesOfSubstringTest5)
{
    String s = String::initWithASCIIEncodedUnicode("Mostly \\:006689\\:005854\\:104432\\:004241, 46F");
    auto t = s.stringByReplacingOccurencesOfStringWithString(
            String::initWithASCIIEncodedUnicode("\\:006689\\:005854\\:104432\\:004241"), "sunny");
    EXPECT_EQ(t, "Mostly sunny, 46F");
}


TEST(StringTest, ReplaceCharactersInRangeTest)
{
    String s("Mostly cloudy with a chance for meatballs.");
    auto t = s.stringByReplacingCharactersInRangeWithString(Range(7,6), String("sunny"));
    EXPECT_EQ(t, "Mostly sunny with a chance for meatballs.");
}


TEST(StringTest, ReplaceCharactersInRangeTest2)
{
    String s("Mostly cloudy with a chance for meatballs.");
    auto t = s.stringByReplacingCharactersInRangeWithString(Range(7,6), "sunny");
    EXPECT_EQ(t, "Mostly sunny with a chance for meatballs.");
}


TEST(StringTest, ReplaceCharactersInRangeTest3)
{
    String s("Mostly cloudy with a chance for meatballs.");
    auto t = s.stringByReplacingCharactersInRangeWithString(Range(7,6), std::string("sunny"));
    EXPECT_EQ(t, "Mostly sunny with a chance for meatballs.");
}


TEST(StringTest, ReplaceCharactersInRangeTest4)
{
    String s = String::initWithASCIIEncodedUnicode(
            "Mostly \\:006689\\:005854\\:104432\\:004241 with a chance for meatballs.");
    auto t = s.stringByReplacingCharactersInRangeWithString(Range(7,4), String("sunny"));
    EXPECT_EQ(t, "Mostly sunny with a chance for meatballs.");
}






