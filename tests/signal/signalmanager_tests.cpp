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
#include <unistd.h>

using namespace TF::Foundation;

TEST(SignalManager, singleton)
{
    auto signal_manager = SignalManager::get_manager();
    EXPECT_EQ(signal_manager->get_number_of_installed_handlers(), 0);
}

extern "C"
{
    void simple_signal_handler_function(int signum);
    void sigaction_signal_handler_function(int signum, siginfo_t *, void * vp);
}

struct SignalHandlerResult
{
    bool handled_signal{false};
    int signal_number{0};

    void handle_signal(int sig)
    {
        handled_signal = true;
        signal_number = sig;
    }

    void reset()
    {
        handled_signal = false;
        signal_number = 0;
    }

    bool successfully_handled_signal(int sig)
    {
        return handled_signal && signal_number == sig;
    }
};

SignalHandlerResult simple_handler_result{};
SignalHandlerResult sigaction_handler_result{};

void simple_signal_handler_function(int signum)
{
    simple_handler_result.handle_signal(signum);
}

void sigaction_signal_handler_function(int signum, siginfo_t *, void *)
{
    sigaction_handler_result.handle_signal(signum);
}

void signal_parent(int signum)
{
    auto process_id = fork();
    if (process_id == 0)
    {
        // We are running in the child.
        auto parent_process_id = getppid();

        // Send the parent a SIGUSR1 signal.
        kill(parent_process_id, signum);

        // Now let the child die.  We have to call _exit rather than exit()
        // because we do not want the child to try and tear down the signal
        // manager signal handler thread.  That thread no longer exists after
        // a fork and calling thread.join() will cause a crash.
        _exit(0);
    }
}

TEST(SignalManager, basic_functionality)
{
    auto signal_manager = SignalManager::get_manager();
    auto signal = SIGUSR1;
    signal_manager->install_handler_from_signal_and_function(signal, simple_signal_handler_function);
    signal_manager->install_handler_from_signal_and_function(signal, sigaction_signal_handler_function);
    EXPECT_EQ(signal_manager->get_number_of_installed_handlers(), 2);

    signal_manager->handle_signals();

    // Sleep just to ensure the handler thread is running;
    sleep(1);

    signal_parent(signal);

    while ((! simple_handler_result.successfully_handled_signal(signal)) &&
           (! sigaction_handler_result.successfully_handled_signal(signal)))
    {
        sleep(1);
    }

    EXPECT_TRUE(simple_handler_result.successfully_handled_signal(signal));
    EXPECT_TRUE(sigaction_handler_result.successfully_handled_signal(signal));

    simple_handler_result.reset();
    sigaction_handler_result.reset();
}

TEST(SignalManager, signal_masks)
{
    // Here we test that the process ignores signals that we set in the signal mask.
    SignalManager::signal_set_type simple_set{};
    auto signal = SIGUSR2;
    simple_set.add_signal(signal);
    auto previous_signal_set = SignalManager::set_thread_signal_mask(simple_set);

    signal_parent(signal);

    {
        SystemDate start_of_sleep{};
        sleep(2);
        SystemDate end_of_sleep{};

        auto sleep_duration = end_of_sleep - start_of_sleep;
        EXPECT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(sleep_duration).count(), 2000);
    }

    // Check that removing the signal from the signal mask causes the sleep to interrupt (ie not masking
    // the signal).  Since we are not blocking SIGUSR2 we need a handler.
    auto signal_manager = SignalManager::get_manager();
    signal_manager->install_handler_from_signal_and_function(signal, sigaction_signal_handler_function);

    SignalManager::set_thread_signal_mask(previous_signal_set);

    signal_parent(signal);

    {
        SystemDate start_of_sleep{};
        sleep(2);
        SystemDate end_of_sleep{};

        auto sleep_duration = end_of_sleep - start_of_sleep;
        EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(sleep_duration).count(), 2000);
    }
}
