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
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(StringTest, CapitalizedStringTest)
{
    String s("hello how are you?");
    EXPECT_EQ(s.capitalizedString(), "Hello How Are You?");
}

TEST(StringTest, LowercaseStringTest)
{
    String s("HELLO HOW ARE YOU?");
    EXPECT_EQ(s.lowercaseString(), "hello how are you?");
}

TEST(StringTest, UppercaseStringTest)
{
    String s("hello how are you?");
    EXPECT_EQ(s.uppercaseString(), "HELLO HOW ARE YOU?");
}
