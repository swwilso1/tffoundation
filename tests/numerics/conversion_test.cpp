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

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(ConversionTest, IntConversionTest)
{
    auto result = Conversion::convertStringToInt("-2147483648");
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), -2147483648);

    result = Conversion::convertStringToInt("-2147483649");
    EXPECT_TRUE(result);
    EXPECT_NE(result.value(), -2147483649);

    result = Conversion::convertStringToInt("2147483647");
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), 2147483647);

    result = Conversion::convertStringToInt("Hello World");
    EXPECT_FALSE(result);

    result = Conversion::convertStringToInt("3.3444");
    EXPECT_FALSE(result);
}

TEST(ConversionTest, DoubleConversionTest)
{
    auto result = Conversion::convertStringToDouble("3.1459");
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), 3.1459);

    result = Conversion::convertStringToDouble("-5848483393.34949");
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), -5848483393.34949);

    result = Conversion::convertStringToDouble("4,2939");
    EXPECT_FALSE(result);

    result = Conversion::convertStringToDouble("Hello World");
    EXPECT_FALSE(result);

    result = Conversion::convertStringToDouble("48248.244.2");
    EXPECT_FALSE(result);
}

TEST(ConversionTest, BoolConversionTest)
{
    auto result = Conversion::convertStringToBool("true");
    EXPECT_TRUE(result);
    EXPECT_TRUE(result.value());

    result = Conversion::convertStringToBool("True");
    EXPECT_TRUE(result);
    EXPECT_TRUE(result.value());

    result = Conversion::convertStringToBool("TrUE");
    EXPECT_TRUE(result);
    EXPECT_TRUE(result.value());

    result = Conversion::convertStringToBool("1");
    EXPECT_TRUE(result);
    EXPECT_TRUE(result.value());

    result = Conversion::convertStringToBool("false");
    EXPECT_TRUE(result);
    EXPECT_FALSE(result.value());

    result = Conversion::convertStringToBool("False");
    EXPECT_TRUE(result);
    EXPECT_FALSE(result.value());

    result = Conversion::convertStringToBool("FaLsE");
    EXPECT_TRUE(result);
    EXPECT_FALSE(result.value());

    result = Conversion::convertStringToBool("0");
    EXPECT_TRUE(result);
    EXPECT_FALSE(result.value());

    result = Conversion::convertStringToBool("Foo");
    EXPECT_FALSE(result);
}
