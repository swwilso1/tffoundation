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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(DurationComponent, simple_tests)
{
    {
        std::chrono::microseconds us{889};
        DurationComponents components{us};
        DurationComponents components_std{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 889};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::milliseconds ms{350};
        DurationComponents components(ms);
        DurationComponents components_std(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::seconds s{10};
        DurationComponents components{s};
        DurationComponents components_std(0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::minutes m{20};
        DurationComponents components{m};
        DurationComponents components_std(0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::hours h{23};
        DurationComponents components{h};
        DurationComponents components_std(0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::days d{6};
        DurationComponents components{d};
        DurationComponents components_std(0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::weeks w{2};
        DurationComponents components{w};
        DurationComponents components_std(0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::months m{11};
        DurationComponents components{m};
        DurationComponents components_std(0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::years y{5};
        DurationComponents components{y};
        DurationComponents components_std(0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        decades d{8};
        DurationComponents components{d};
        DurationComponents components_std(0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        centuries c{3};
        DurationComponents components{c};
        DurationComponents components_std(0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }

    {
        millenia m{20};
        DurationComponents components{m};
        DurationComponents components_std(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        EXPECT_EQ(components, components_std);
    }
}

TEST(DurationComponent, just_over_threshold_tests)
{
    {
        std::chrono::microseconds us{1000005};
        DurationComponents components{us};
        DurationComponents components_std{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::seconds s{126};
        DurationComponents components{s};
        DurationComponents components_std{0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::minutes m{67};
        DurationComponents components{m};
        DurationComponents components_std{0, 0, 0, 0, 0, 0, 0, 1, 7, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::hours h{27};
        DurationComponents components{h};
        DurationComponents components_std{0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::days d{9};
        DurationComponents components{d};
        DurationComponents components_std{0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::weeks w{6};
        DurationComponents components{w};
        DurationComponents components_std{0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::months m{26};
        DurationComponents components{m};
        DurationComponents components_std{0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        std::chrono::years y{35};
        DurationComponents components{y};
        DurationComponents components_std{0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        decades d{12};
        DurationComponents components{d};
        DurationComponents components_std{0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }

    {
        centuries c{15};
        DurationComponents components{c};
        DurationComponents components_std{1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_EQ(components, components_std);
    }
}
