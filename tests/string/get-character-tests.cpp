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

using namespace TF::Foundation;

#include "gtest/gtest.h"

TEST(StringTest, BracketOperatorTest)
{
    String s = String::initWithASCIIEncodedUnicode("Hello \\:004435\\:003848\\:101234");
    EXPECT_EQ(s[0], 'H');
    EXPECT_EQ(s[1], 'e');
    EXPECT_EQ(s[2], 'l');
    EXPECT_EQ(s[3], 'l');
    EXPECT_EQ(s[4], 'o');
    EXPECT_EQ(s[5], ' ');
    EXPECT_EQ(s[6], 17461);
    EXPECT_EQ(s[7], 14408);
    EXPECT_EQ(s[8], 1053236);
}

TEST(StringTest, CharacterAtIndexTest)
{
    String s = String::initWithASCIIEncodedUnicode("Hello \\:004435\\:003848\\:101234");
    EXPECT_EQ(s.characterAtIndex(0), 'H');
    EXPECT_EQ(s.characterAtIndex(1), 'e');
    EXPECT_EQ(s.characterAtIndex(2), 'l');
    EXPECT_EQ(s.characterAtIndex(3), 'l');
    EXPECT_EQ(s.characterAtIndex(4), 'o');
    EXPECT_EQ(s.characterAtIndex(5), ' ');
    EXPECT_EQ(s.characterAtIndex(6), 17461);
    EXPECT_EQ(s.characterAtIndex(7), 14408);
    EXPECT_EQ(s.characterAtIndex(8), 1053236);
}

TEST(StringTest, CharactersInRangeTest1)
{
    String s("The large house on the left.");
    String t = s.getCharactersInRange(Range(0, 1));
    EXPECT_EQ(t, "T");
}

TEST(StringTest, CharactersInRangeTest2)
{
    String s("The large house on the left.");
    String t = s.getCharactersInRange(Range(0, 10));
    EXPECT_EQ(t, "The large ");
}

TEST(StringTest, CharactersInRangeTest3)
{
    String s("The large house on the left.");
    String t = s.getCharactersInRange(Range(5, 10));
    EXPECT_EQ(t, "arge house");
}

TEST(StringTest, CharactersInRangeTest4)
{
    String s = String::initWithASCIIEncodedUnicode("The large ho\\:000129use on the left.");
    String t = s.getCharactersInRange(Range(5, 10));
    String u = String::initWithASCIIEncodedUnicode("arge ho\\:000129us");
    EXPECT_EQ(t, u);
}
