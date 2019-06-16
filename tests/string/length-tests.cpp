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

using namespace TF::Foundation;

#include "gtest/gtest.h"


TEST(StringTest, LengthTest1)
{
    String s("The happy red fox jumped over the brown log.");
    EXPECT_EQ(s.length(),44);
}

TEST(StringTest, LengthTest2)
{
    String s = String::initWithASCIIEncodedUnicode(
            "The happy red \\:002343 fox j\\:103323umped o\\:003344ver the brown log.");
    EXPECT_EQ(s.length(),48);
}


TEST(StringTest, ByteLengthTest1)
{
    String s("The happy red fox jumped over the brown log.");
    EXPECT_EQ(s.numberOfBytes(),44);
}


TEST(StringTest, ByteLengthTest2)
{
    String s = String::initWithASCIIEncodedUnicode(
            "The happy red \\:002343 fox j\\:103323umped o\\:003344ver the brown log.");
    EXPECT_EQ(s.numberOfBytes(),55);
}


