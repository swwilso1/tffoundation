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

#include "tfsignalset.hpp"

namespace TF::Foundation
{
    std::vector<int> get_signal_set_list()
    {
        std::vector<int> sig_list{};
        sig_list.emplace_back(SIGHUP);
        sig_list.emplace_back(SIGINT);
        sig_list.emplace_back(SIGQUIT);
        sig_list.emplace_back(SIGILL);
        sig_list.emplace_back(SIGTRAP);
        sig_list.emplace_back(SIGABRT);
        sig_list.emplace_back(SIGEMT);
        sig_list.emplace_back(SIGFPE);
        sig_list.emplace_back(SIGKILL);
        sig_list.emplace_back(SIGBUS);
        sig_list.emplace_back(SIGSEGV);
        sig_list.emplace_back(SIGSYS);
        sig_list.emplace_back(SIGPIPE);
        sig_list.emplace_back(SIGALRM);
        sig_list.emplace_back(SIGTERM);
        sig_list.emplace_back(SIGURG);
        sig_list.emplace_back(SIGSTOP);
        sig_list.emplace_back(SIGTSTP);
        sig_list.emplace_back(SIGCONT);
        sig_list.emplace_back(SIGCHLD);
        sig_list.emplace_back(SIGTTIN);
        sig_list.emplace_back(SIGTTOU);
        sig_list.emplace_back(SIGIO);
        sig_list.emplace_back(SIGXCPU);
        sig_list.emplace_back(SIGXFSZ);
        sig_list.emplace_back(SIGVTALRM);
        sig_list.emplace_back(SIGPROF);
        sig_list.emplace_back(SIGWINCH);
        sig_list.emplace_back(SIGINFO);
        sig_list.emplace_back(SIGUSR1);
        sig_list.emplace_back(SIGUSR2);
        return sig_list;
    }

    std::vector<int> SignalSet::s_signal_list = get_signal_set_list();

    SignalSet::SignalSet()
    {
        clear();
    }

    SignalSet SignalSet::get_empty_set()
    {
        return SignalSet{};
    }

    SignalSet SignalSet::get_full_set()
    {
        SignalSet set{};
        set.fill_with_all_signals();
        return set;
    }

    void SignalSet::add_signal(int sig)
    {
        auto api_result = sigaddset(&m_signal_set, sig);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "sigaddset failed");
        }
    }

    void SignalSet::remove_signal(int sig)
    {
        auto api_result = sigdelset(&m_signal_set, sig);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "sigdelset failed");
        }
    }

    void SignalSet::clear()
    {
        auto api_result = sigemptyset(&m_signal_set);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "sigemptyset failed");
        }
    }

    void SignalSet::fill_with_all_signals()
    {
        auto api_result = sigfillset(&m_signal_set);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "sigfillset failed");
        }
    }

    bool SignalSet::is_member(int sig) const
    {
        return sigismember(&m_signal_set, sig) == 1 ? true : false;
    }

    SignalSet::set_type SignalSet::get_raw_set() const
    {
        return m_signal_set;
    }

    std::unordered_set<int> SignalSet::signals_in_set() const
    {
        std::unordered_set<int> signals{};
        std::for_each(s_signal_list.cbegin(), s_signal_list.cend(), [&signals, this](const int & sig) -> void {
            if (is_member(sig))
            {
                signals.emplace(sig);
            }
        });
        return signals;
    }

    std::unordered_set<int>::size_type SignalSet::size() const
    {
        std::unordered_set<int>::size_type counter{0};
        std::for_each(s_signal_list.cbegin(), s_signal_list.cend(), [&counter, this](const int & sig) -> void {
            if (is_member(sig))
            {
                counter++;
            }
        });
        return counter;
    }

} // namespace TF::Foundation
