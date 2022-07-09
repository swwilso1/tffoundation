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

extern "C"
{
    void simple_test_handler(int signum);
    void sigaction_test_handler(int signum, siginfo_t * info, void * vp);
}

bool simple_test_handler_called{false};
bool sigaction_test_handler_called{false};

void simple_test_handler(int signum)
{
    (void)signum;
    simple_test_handler_called = true;
    return;
}

void sigaction_test_handler(int signum, siginfo_t * info, void * vp)
{
    (void)signum;
    (void)info;
    (void)vp;
    sigaction_test_handler_called = true;
    return;
}

TEST(SignalHandler, simple_constuctor_tests)
{
    {
        struct sigaction foo
        {};
        foo.sa_handler = simple_test_handler;
        SignalHandler handler{SIGBUS, foo, false, true};
        EXPECT_EQ(handler.get_simple_handler(), simple_test_handler);
        EXPECT_EQ(handler.get_number(), SIGBUS);
        EXPECT_FALSE(handler.did_exist_before());
        EXPECT_TRUE(handler.is_unrelated());
    }

    {
        SignalHandler handler{SIGBUS, simple_test_handler, true, true};
        EXPECT_EQ(handler.get_simple_handler(), simple_test_handler);
        EXPECT_EQ(handler.get_number(), SIGBUS);
        EXPECT_TRUE(handler.did_exist_before());
        EXPECT_TRUE(handler.is_unrelated());
    }

    {
        SignalHandler handler{SIGBUS, sigaction_test_handler, true, false};
        EXPECT_EQ(handler.get_sigaction_handler(), sigaction_test_handler);
        EXPECT_EQ(handler.get_number(), SIGBUS);
        EXPECT_TRUE(handler.did_exist_before());
        EXPECT_FALSE(handler.is_unrelated());
    }
}

TEST(SignalHandler, comparison_tests)
{
    SignalHandler handler1{SIGCHLD, simple_test_handler, false, false};

    {
        SignalHandler handler2{SIGCHLD, simple_test_handler, false, false};
        EXPECT_TRUE(handler1 == handler2);
        EXPECT_FALSE(handler1 != handler2);
    }

    {
        SignalHandler handler2{SIGCHLD, sigaction_test_handler, false, false};
        EXPECT_FALSE(handler1 == handler2);
        EXPECT_TRUE(handler1 != handler2);
    }

    {
        SignalHandler handler2{SIGCHLD, simple_test_handler, false, true};
        EXPECT_FALSE(handler1 == handler2);
        EXPECT_TRUE(handler1 != handler2);
    }

    {
        SignalHandler handler2{SIGCHLD, simple_test_handler, true, false};
        EXPECT_FALSE(handler1 == handler2);
        EXPECT_TRUE(handler1 != handler2);
    }

    {
        SignalHandler handler2{SIGCHLD, simple_test_handler, true, true};
        EXPECT_FALSE(handler1 == handler2);
        EXPECT_TRUE(handler1 != handler2);
    }

    {
        SignalHandler handler2{SIGCHLD, simple_test_handler, false, false};
        handler2.set_flags(static_cast<int>(SignalHandlerFlag::RESTART_SYSTEM_CALLS) |
                           static_cast<int>(SignalHandlerFlag::NO_DEFER));
        EXPECT_FALSE(handler1 == handler2);
        EXPECT_TRUE(handler1 != handler2);
    }
}

TEST(SignalHandler, flag_tests)
{
    SignalHandler handler{SIGHUP, simple_test_handler, false, true};
    handler.add_flag(SignalHandlerFlag::NO_CHILD_WAIT);
    auto handler_flag_set = handler.get_flags();
    EXPECT_EQ(handler_flag_set.size(), 1);
    EXPECT_TRUE(handler_flag_set.contains(SignalHandlerFlag::NO_CHILD_WAIT));
    EXPECT_TRUE(handler.uses_flag(SignalHandlerFlag::NO_CHILD_WAIT));

    handler.add_flag(SignalHandlerFlag::NO_CHILD_STOP);
    handler_flag_set = handler.get_flags();
    EXPECT_EQ(handler_flag_set.size(), 2);
    EXPECT_TRUE(handler_flag_set.contains(SignalHandlerFlag::NO_CHILD_STOP));
    EXPECT_TRUE(handler.uses_flag(SignalHandlerFlag::NO_CHILD_STOP));

    handler.add_flag(SignalHandlerFlag::ON_STACK);
    handler_flag_set = handler.get_flags();
    EXPECT_EQ(handler_flag_set.size(), 3);
    EXPECT_TRUE(handler_flag_set.contains(SignalHandlerFlag::ON_STACK));
    EXPECT_TRUE(handler.uses_flag(SignalHandlerFlag::ON_STACK));

    SignalHandler handler2{SIGHUP, sigaction_test_handler, false, true};
    handler_flag_set = handler2.get_flags();
    EXPECT_GE(handler_flag_set.size(), 1);
    EXPECT_TRUE(handler_flag_set.contains(SignalHandlerFlag::SIGACTION));
    EXPECT_TRUE(handler2.uses_flag(SignalHandlerFlag::SIGACTION));
}

TEST(SignalHandler, functor_tests)
{
    {
        SignalHandler handler(SIGHUP, simple_test_handler, false, false);
        handler(nullptr, nullptr);
        EXPECT_TRUE(simple_test_handler_called);
        simple_test_handler_called = false;
    }

    {
        SignalHandler handler{SIGHUP, sigaction_test_handler};
        handler(nullptr, nullptr);
        EXPECT_TRUE(sigaction_test_handler_called);
        sigaction_test_handler_called = false;
    }
}
