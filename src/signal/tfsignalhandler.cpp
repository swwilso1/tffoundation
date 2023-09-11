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

#include "tfplatform.hpp"
#define NEEDS_ALGORITHM
#define NEEDS_CSTRING
#include "tfheaders.hpp"
#include "tfsignalhandler.hpp"
#include "tfformatterfactory.hpp"

namespace TF::Foundation
{

    int recover_system_flag_from_api_flag(const SignalHandlerFlag & flag)
    {
        int system_flag{};
        switch (flag)
        {
            case SignalHandlerFlag::NO_CHILD_STOP:
                system_flag = SA_NOCLDSTOP;
                break;
            case SignalHandlerFlag::NO_CHILD_WAIT:
                system_flag = SA_NOCLDWAIT;
                break;
            case SignalHandlerFlag::ON_STACK:
                system_flag = SA_ONSTACK;
                break;
            case SignalHandlerFlag::NO_DEFER:
                system_flag = SA_NODEFER;
                break;
            case SignalHandlerFlag::RESET_HANDLER_TO_DEFAULT:
                system_flag = static_cast<int>(SA_RESETHAND);
                break;
            case SignalHandlerFlag::RESTART_SYSTEM_CALLS:
                system_flag = SA_RESTART;
                break;
            case SignalHandlerFlag::SIGACTION:
                system_flag = SA_SIGINFO;
                break;
        }
        return system_flag;
    }

    std::vector<SignalHandler::handler_flag> initialize_handler_flag_list()
    {
        std::vector<SignalHandler::handler_flag> list;
        list.emplace_back(SignalHandlerFlag::NO_CHILD_STOP);
        list.emplace_back(SignalHandlerFlag::NO_CHILD_WAIT);
        list.emplace_back(SignalHandlerFlag::ON_STACK);
        list.emplace_back(SignalHandlerFlag::NO_DEFER);
        list.emplace_back(SignalHandlerFlag::RESET_HANDLER_TO_DEFAULT);
        list.emplace_back(SignalHandlerFlag::RESTART_SYSTEM_CALLS);
        list.emplace_back(SignalHandlerFlag::SIGACTION);
        return list;
    }

    std::vector<SignalHandler::handler_flag> SignalHandler::s_handler_flags = initialize_handler_flag_list();

    SignalHandler::SignalHandler(int number, const signal_struct & s, bool existed, bool unrelated) :
        m_signal_number{number}, m_action{s}, m_existed_before{existed}, m_unrelated{unrelated}
    {}

    SignalHandler::SignalHandler(int number, simple_signal_handler h, bool existed, bool unrelated) :
        m_signal_number{number}, m_existed_before{existed}, m_unrelated{unrelated}
    {
        std::memset(&m_action, 0, sizeof(m_action));
        sigemptyset(&m_action.sa_mask);
        m_action.sa_handler = h;
    }

    SignalHandler::SignalHandler(int number, sigaction_signal_handler h, bool existed, bool unrelated) :
        m_signal_number{number}, m_existed_before{existed}, m_unrelated{unrelated}
    {
        std::memset(&m_action, 0, sizeof(m_action));
        sigemptyset(&m_action.sa_mask);
        add_flag(handler_flag::SIGACTION);
        m_action.sa_sigaction = h;
    }

    bool SignalHandler::operator==(const SignalHandler & h) const
    {
        if (this == &h)
        {
            return true;
        }

        return m_signal_number == h.m_signal_number && memcmp(&m_action, &h.m_action, sizeof(signal_struct)) == 0 &&
               m_existed_before == h.m_existed_before && m_unrelated == h.m_unrelated;
    }

    bool SignalHandler::operator!=(const SignalHandler & h) const
    {
        return ! (*this == h);
    }

    int SignalHandler::get_number() const
    {
        return m_signal_number;
    }

    void SignalHandler::add_flag(handler_flag flag)
    {
        m_action.sa_flags |= recover_system_flag_from_api_flag(flag);
    }

    bool has_flag(int flags, SignalHandler::handler_flag flag)
    {
        if ((flags & static_cast<int>(flag)) == static_cast<int>(flag))
        {
            return true;
        }
        return false;
    }

    void SignalHandler::set_flags(int flags)
    {
        auto set_flag = [this, flags](handler_flag flag) {
            if (has_flag(flags, flag))
            {
                add_flag(flag);
            }
        };
        std::for_each(s_handler_flags.cbegin(), s_handler_flags.cend(), [set_flag](handler_flag flag) -> void {
            set_flag(flag);
        });
    }

    std::unordered_set<SignalHandler::handler_flag> SignalHandler::get_flags() const
    {
        std::unordered_set<SignalHandler::handler_flag> signal_flags;
        std::for_each(s_handler_flags.cbegin(), s_handler_flags.cend(),
                      [this, &signal_flags](handler_flag flag) -> void {
                          if (uses_flag(flag))
                          {
                              signal_flags.emplace(flag);
                          }
                      });
        return signal_flags;
    }

    bool SignalHandler::uses_flag(handler_flag flag) const
    {
        auto system_flag = recover_system_flag_from_api_flag(flag);
        return (m_action.sa_flags & system_flag) == system_flag;
    }

    simple_signal_handler SignalHandler::get_simple_handler() const
    {
        return m_action.sa_handler;
    }

    sigaction_signal_handler SignalHandler::get_sigaction_handler() const
    {
        return m_action.sa_sigaction;
    }

    SignalHandler::signal_struct SignalHandler::get_structure() const
    {
        return m_action;
    }

    void SignalHandler::operator()(siginfo_t * info, void * vp) const
    {
        if (is_siginfo_handler())
        {
            if (m_action.sa_sigaction != nullptr)
            {
                (*m_action.sa_sigaction)(m_signal_number, info, vp);
            }
        }
        else
        {
            if (m_action.sa_handler != nullptr)
            {
                (*m_action.sa_handler)(m_signal_number);
            }
        }
    }

    bool SignalHandler::is_siginfo_handler() const
    {
        if ((m_action.sa_flags & SA_SIGINFO) == SA_SIGINFO)
        {
            return true;
        }
        return false;
    }

    std::ostream & SignalHandler::description(std::ostream & o) const
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("SignalHandler");
            formatter->addClassMember("m_signal_number", m_signal_number);
            formatter->addClassMember("m_action", m_action);
            formatter->addClassMember("m_existed_before", m_existed_before);
            formatter->addClassMember("m_unrelated", m_unrelated);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    std::ostream & operator<<(std::ostream & o, const SignalHandler & h)
    {
        return h.description(o);
    }

} // namespace TF::Foundation

std::ostream & operator<<(std::ostream & o, const TF::Foundation::SignalHandler::signal_struct & s)
{
    TF::Foundation::ClassFormatter * formatter = TF::Foundation::FormatterFactory::getFormatter();
    if (formatter != nullptr)
    {
        formatter->setClassName("struct sigaction");
#if defined(TFMACOS)
        // Linux needs a << overload for a sigset_t.  Constraining to macOS for now.
        formatter->addClassMember("sa_mask", s.sa_mask);
#endif
        formatter->addClassMember("sa_flags", s.sa_flags);
        if ((s.sa_flags & SA_SIGINFO) == SA_SIGINFO)
        {
            formatter->addClassMember("sa_sigaction", s.sa_sigaction);
        }
        else
        {
            formatter->addClassMember("sa_handler", s.sa_handler);
        }
    }
    return o;
}
