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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(MultiQueue, compile_test)
{
    MultiQueue<int> m{};
    EXPECT_TRUE(m.empty());
}

TEST(MultiQueue, basic_operations_test)
{
    MultiQueue<int> q{};
    EXPECT_TRUE(q.empty());

    q.push_back(10);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 1);

    q.push_back(20);
    EXPECT_EQ(q.size(), 2);

    auto front_value = q.front();
    EXPECT_EQ(front_value, 10);

    q.pop_front();
    EXPECT_EQ(q.size(), 1);

    front_value = q.front();
    EXPECT_EQ(front_value, 20);

    q.pop_front();
    EXPECT_TRUE(q.empty());
}

TEST(MultiQueue, fill_empty_fill_test)
{
    MultiQueue<int> q{};

    // First fill the queue
    for (int i = 0; i < 100; i++)
    {
        q.push_back(i);
        EXPECT_EQ(q.size(), i + 1);
    }

    EXPECT_FALSE(q.empty());

    // Now drain the queue
    int i = 0;
    while (! q.empty())
    {
        auto value = q.front();
        EXPECT_EQ(value, i++);
        q.pop_front();
    }

    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);

    // Now add more to the queue.
    for (i = 100; i < 200; i++)
    {
        q.push_back(i);
        EXPECT_EQ(q.size(), i - 100 + 1);
    }

    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 100);

    // Now drain the queue again.
    i = 100;
    while (! q.empty())
    {
        auto value = q.front();
        EXPECT_EQ(value, i++);
        q.pop_front();
    }

    EXPECT_TRUE(q.empty());
}

TEST(MultiQueue, duplication_test)
{
    MultiQueue<int> q{};
    EXPECT_TRUE(q.empty());

    for (int i = 0; i < 10; i++)
    {
        q.push_back(i);
        EXPECT_EQ(q.size(), i + 1);
    }

    EXPECT_FALSE(q.empty());

    auto duplicate_q = q.fork();

    int i = 0;
    while (! q.empty())
    {
        auto value = q.front();
        EXPECT_EQ(value, i++);
        q.pop_front();
    }
    EXPECT_TRUE(q.empty());
    EXPECT_FALSE(duplicate_q->empty());
    EXPECT_EQ(duplicate_q->size(), 10);

    i = 0;
    while (! duplicate_q->empty())
    {
        auto value = duplicate_q->front();
        EXPECT_EQ(value, i++);
        duplicate_q->pop_front();
    }

    EXPECT_TRUE(duplicate_q->empty());

    // Now fill up the queue again.
    for (i = 10; i < 1000; i++)
    {
        q.push_back(i);
        EXPECT_EQ(q.size(), i - 10 + 1);
    }

    EXPECT_FALSE(q.empty());
    EXPECT_FALSE(duplicate_q->empty());
    EXPECT_EQ(q.size(), 990);
    EXPECT_EQ(duplicate_q->size(), 990);

    // Read from the duplicate queue.
    i = 10;
    while (! duplicate_q->empty())
    {
        auto value = duplicate_q->front();
        EXPECT_EQ(value, i++);
        duplicate_q->pop_front();
    }

    EXPECT_TRUE(duplicate_q->empty());
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(duplicate_q->size(), 0);
    EXPECT_EQ(q.size(), 990);

    // Read from the first q.
    i = 10;
    while (! q.empty())
    {
        auto value = q.front();
        EXPECT_EQ(value, i++);
        q.pop_front();
    }

    EXPECT_TRUE(duplicate_q->empty());
    EXPECT_EQ(duplicate_q->size(), 0);
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);

    delete duplicate_q;
}