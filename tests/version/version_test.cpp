/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#include <sstream>
#include "gtest/gtest.h"
#include "TFFoundation.hpp"

using namespace TF::Foundation;

TEST(VersionTest, basic_test)
{
    Version version{1, 2, 3};
    EXPECT_EQ(version.major, 1);
    EXPECT_EQ(version.minor, 2);
    EXPECT_EQ(version.release, 3);
    EXPECT_EQ(version.patch, 0);

    Version v2{3, 4, 5, 6};
    std::stringstream stream{};
    stream << v2;
    EXPECT_EQ(stream.str(), "3.4.5.6");
}

TEST(VersionTest, comparison_test)
{
    {
        Version a{}, b{};
        EXPECT_TRUE(a == b);
        EXPECT_FALSE(a != b);
    }

    {
        Version a{1, 0, 0}, b{1, 0, 0};
        EXPECT_EQ(a, b);
    }

    {
        Version a{1, 0, 1}, b{1, 0, 0};
        EXPECT_GT(a, b);
    }
}

TEST(VersionTest, as_string_test)
{
    Version version{1, 2, 3};
    EXPECT_EQ(version.as_string(), String{"1.2.3"});

    Version version2{1, 2, 3, 4};
    EXPECT_EQ(version2.as_string(), String{"1.2.3.4"});
}
