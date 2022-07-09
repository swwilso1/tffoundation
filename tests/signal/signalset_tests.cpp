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

TEST(SignalSet, default_constructor)
{
    // A basic set should have no signals
    SignalSet set{};
    EXPECT_EQ(set.size(), 0);
}

TEST(SignalSet, empty_set_test)
{
    auto set = SignalSet::get_empty_set();
    EXPECT_EQ(set.size(), 0);
}

TEST(SignalSet, full_set_test)
{
    auto set = SignalSet::get_full_set();
    EXPECT_GT(set.size(), 0);
}

TEST(SignalSet, set_membership)
{
    SignalSet set{};
    set.add_signal(SIGHUP);
    EXPECT_TRUE(set.is_member(SIGHUP));
    EXPECT_EQ(set.size(), 1);

    {
        auto signals_in_set = set.signals_in_set();
        EXPECT_TRUE(signals_in_set.contains(SIGHUP));
    }

    set.add_signal(SIGBUS);
    set.add_signal(SIGCHLD);
    EXPECT_TRUE(set.is_member(SIGBUS));
    EXPECT_TRUE(set.is_member(SIGCHLD));
    EXPECT_EQ(set.size(), 3);

    {
        auto signals_in_set = set.signals_in_set();
        EXPECT_TRUE(signals_in_set.contains(SIGHUP));
        EXPECT_TRUE(signals_in_set.contains(SIGBUS));
        EXPECT_TRUE(signals_in_set.contains(SIGCHLD));
    }
}

TEST(SignalSet, remove_signal_tests)
{
    auto set = SignalSet::get_full_set();
    auto set_size = set.size();

    set.remove_signal(SIGHUP);
    EXPECT_EQ(set.size(), set_size - 1);
    EXPECT_FALSE(set.is_member(SIGHUP));
    {
        auto signals_in_set = set.signals_in_set();
        EXPECT_FALSE(signals_in_set.contains(SIGHUP));
    }

    set.remove_signal(SIGSTOP);
    set.remove_signal(SIGSEGV);
    EXPECT_EQ(set.size(), set_size - 3);
    EXPECT_FALSE(set.is_member(SIGSTOP));
    EXPECT_FALSE(set.is_member(SIGSEGV));
    {
        auto signals_in_set = set.signals_in_set();
        EXPECT_FALSE(signals_in_set.contains(SIGSTOP));
        EXPECT_FALSE(signals_in_set.contains(SIGSEGV));
        EXPECT_FALSE(signals_in_set.contains(SIGHUP));
    }
}

TEST(SignalSet, fill_and_clear_tests)
{
    SignalSet set{};
    EXPECT_EQ(set.size(), 0);

    set.fill_with_all_signals();
    EXPECT_GT(set.size(), 0);
    {
        auto sigs_in_set = set.signals_in_set();
        EXPECT_GT(sigs_in_set.size(), 0);
        EXPECT_EQ(sigs_in_set.size(), set.size());
    }

    set.clear();
    EXPECT_EQ(set.size(), 0);

    set.add_signal(SIGTERM);
    EXPECT_EQ(set.size(), 1);
    EXPECT_TRUE(set.is_member(SIGTERM));
    EXPECT_FALSE(set.is_member(SIGHUP));
}
