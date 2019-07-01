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

TEST(StringTest, RangeOfStringTest)
{
    String s("Permission is hereby granted");
    auto theRange = s.rangeOfString(String("hereby"));
    EXPECT_EQ(theRange, Range(14, 6));
}


TEST(StringTest, RangeOfStringTest2)
{
    String s = String::initWithASCIIEncodedUnicode("Might as well \\:004443 jump");
    auto theRange = s.rangeOfString(String::initWithASCIIEncodedUnicode("\\:004443 j"));
    EXPECT_EQ(theRange, Range(14, 3));
}


TEST(StringTest, RangeOfCStringTest)
{
    String s("Permission is hereby granted");
    auto theRange = s.rangeOfString("hereby");
    EXPECT_EQ(theRange, Range(14, 6));
}


TEST(StringTest, RangeOfSTLStringTest)
{
    String s("Permission is hereby granted");
    auto theRange = s.rangeOfString(std::string("hereby"));
    EXPECT_EQ(theRange, Range(14, 6));
}


TEST(StringTest, RangesOfStringTest)
{
    String s("the dog ate the homework after the cat licked the bowl of the food");
    auto theRanges = s.rangesOfString(String("the"));
    EXPECT_EQ(theRanges.size(), 5);
    EXPECT_EQ(theRanges[0], Range(0, 3));
    EXPECT_EQ(theRanges[1], Range(12, 3));
    EXPECT_EQ(theRanges[2], Range(31, 3));
    EXPECT_EQ(theRanges[3], Range(46, 3));
    EXPECT_EQ(theRanges[4], Range(58, 3));
}


TEST(StringTest, RangesOfCStyleStringTest)
{
    String s("the dog ate the homework after the cat licked the bowl of the food");
    auto theRanges = s.rangesOfString("the");
    EXPECT_EQ(theRanges.size(), 5);
    EXPECT_EQ(theRanges[0], Range(0, 3));
    EXPECT_EQ(theRanges[1], Range(12, 3));
    EXPECT_EQ(theRanges[2], Range(31, 3));
    EXPECT_EQ(theRanges[3], Range(46, 3));
    EXPECT_EQ(theRanges[4], Range(58, 3));
}


TEST(StringTest, RangesOfSTLStringTest)
{
    String s("the dog ate the homework after the cat licked the bowl of the food");
    auto theRanges = s.rangesOfString(std::string("the"));
    EXPECT_EQ(theRanges.size(), 5);
    EXPECT_EQ(theRanges[0], Range(0, 3));
    EXPECT_EQ(theRanges[1], Range(12, 3));
    EXPECT_EQ(theRanges[2], Range(31, 3));
    EXPECT_EQ(theRanges[3], Range(46, 3));
    EXPECT_EQ(theRanges[4], Range(58, 3));
}


TEST(StringTest, RangeOfUnicodeStringTest)
{
    String s =
        String::initWithASCIIEncodedUnicode("Terrence \\:00006F\\:000066 Eragon, Howard \\:00006F\\:000066 Manchester");
    auto theRanges = s.rangesOfString("of");
    EXPECT_EQ(theRanges.size(), 2);
    EXPECT_EQ(theRanges[0], Range(9, 2));
    EXPECT_EQ(theRanges[1], Range(27, 2));
}
