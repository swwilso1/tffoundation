/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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


TEST(StringTest, EqualityTest1)
{
    String s("foo");
    String t("foo");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, EqualityTest2)
{
    String s("\\:004848ac\\:003434");
    String t("\\:004848ac\\:003434");
    EXPECT_TRUE(s == t);
}


TEST(StringTest, InequalityTest1)
{
    String s("foo");
    String t("bar");
    EXPECT_TRUE(s != t);
}


TEST(StringTest, InequalityTest2)
{
    String s("\\:004848ac\\:003434");
    String t("foo\\:003434");
    EXPECT_TRUE(s != t);
}


TEST(StringTest, CStringEqualityTest)
{
    EXPECT_TRUE(String("Hello World") == "Hello World");
}


TEST(StringTest, CStringEqualityTest2)
{
    EXPECT_TRUE("Hello World" == String("Hello World"));
}


TEST(StringTest, StdStringEqualityTest)
{
    EXPECT_TRUE(String("Hello World") == std::string("Hello World"));
}


TEST(StringTest, StdStringEqualityTest2)
{
    EXPECT_TRUE(std::string("Hello World") == String("Hello World"));
}