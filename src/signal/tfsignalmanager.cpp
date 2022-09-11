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

#include "tfplatformheaders.h"
#define NEEDS_ALGORITHM
#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tfsignalmanager.hpp"

#define USE_POLL_STRATEGY
#include "tfpoll.hpp"

#if defined(TFLINUX)
#	define DO_NOT_USE_RESULT (void)!
#else
#	define DO_NOT_USE_RESULT
#endif

namespace TF::Foundation
{

    SignalManager::signal_manager_pointer SignalManager::s_manager{nullptr};
    bool SignalManager::s_initialized_the_manager{false};
    std::mutex SignalManager::s_manager_mutex{};

    SignalManager::~SignalManager()
    {
        if (m_handling_signals)
        {
            stop_handling_signals();
        }
    }

    SignalManager::signal_manager_pointer SignalManager::get_manager()
    {
        auto_lock_type lock(s_manager_mutex);
        if (! s_initialized_the_manager)
        {
            s_manager = signal_manager_pointer(new SignalManager{});
            s_initialized_the_manager = true;
        }
        return s_manager;
    }

    void SignalManager::install_handler(const handler_type & handler)
    {
        auto_lock_type lock{m_mutex};
        auto & handler_map = m_signal_map[handler.get_number()];
        handler_map.insert(std::make_pair(reinterpret_cast<void *>(handler.get_simple_handler()), handler));
        if (m_handling_signals)
        {
            install_handler_in_system(signal_manager_handler(handler.get_number()));
        }
    }

    void SignalManager::install_handler_from_signal_and_struct(int sig, handler_type::signal_struct & st, bool external)
    {
        install_handler(handler_type{sig, st, false, external});
    }

    void SignalManager::install_handler_from_signal_and_function(int sig, simple_signal_handler handler, bool external)
    {
        install_handler(handler_type{sig, handler, false, external});
    }

    void SignalManager::install_handler_from_signal_and_function(int sig, sigaction_signal_handler handler,
                                                                 bool external)
    {
        install_handler(handler_type{sig, handler, false, external});
    }

    SignalManager::handler_type SignalManager::install_handler_in_system(const handler_type & handler)
    {
        handler_type::signal_struct original_handler{};
        handler_type::signal_struct new_handler = handler.get_structure();

        auto sigaction_result = sigaction(handler.get_number(), &new_handler, &original_handler);
        if (sigaction_result != 0)
        {
            throw std::system_error{errno, std::system_category(), "unable to install signal handler"};
        }

        return handler_type{handler.get_number(), original_handler, true, true};
    }

    void SignalManager::uninstall_handler(const handler_type & handler)
    {
        uninstall_handler_from_handler_function(handler.get_number(),
                                                reinterpret_cast<void *>(handler.get_simple_handler()));
    }

    void SignalManager::uninstall_handler_from_signal_and_function(int sig, simple_signal_handler handler)
    {
        uninstall_handler_from_handler_function(sig, reinterpret_cast<void *>(handler));
    }

    void SignalManager::uninstall_handler_from_signal_and_function(int sig, sigaction_signal_handler handler)
    {
        uninstall_handler_from_handler_function(sig, reinterpret_cast<void *>(handler));
    }

    void SignalManager::clear_handlers()
    {
        auto_lock_type lock{m_mutex};
        std::for_each(m_signal_map.begin(), m_signal_map.end(), [](auto & i) -> void {
            i.second.clear();
        });
        m_signal_map.clear();
    }

    void SignalManager::handle_signal(int sig)
    {
        install_handler_in_system(signal_manager_handler(sig));
    }

    void SignalManager::do_not_handle_signal(int sig)
    {
        auto_lock_type lock{m_mutex};
        auto & handler_map = m_signal_map[sig];
        bool found_existing_before_handler{false};

        for (auto & entry : handler_map)
        {
            if (entry.second.did_exist_before())
            {
                install_handler_in_system(entry.second);
                found_existing_before_handler = true;
                break;
            }
        }

        if (! found_existing_before_handler)
        {
            install_handler_in_system(ignore_handler(sig));
        }
    }

    void SignalManager::ignore_signal(int sig)
    {
        install_handler_in_system(ignore_handler(sig));
    }

    void SignalManager::call_handlers(int s, siginfo_t * info, void * vp)
    {
        auto_lock_type lock(m_mutex);
        auto & handler_map = m_signal_map[s];
        std::for_each(handler_map.cbegin(), handler_map.cend(), [info, vp](auto & i) -> void {
            i.second(info, vp);
        });
    }

    void SignalManager::call_unrelated_or_existing_before_handlers(int s, siginfo_t * info, void * vp)
    {
        auto_lock_type lock(m_mutex);
        auto & handler_map = m_signal_map[s];
        std::for_each(handler_map.cbegin(), handler_map.cend(), [info, vp](auto & i) -> void {
            if (i.second.did_exist_before() || i.second.is_unrelated())
            {
                i.second(info, vp);
            }
        });
    }

    bool SignalManager::has_unrelated_or_existing_before_handlers(int s)
    {
        auto_lock_type lock(m_mutex);
        auto & handler_map = m_signal_map[s];

        return std::any_of(handler_map.cbegin(), handler_map.cend(), [](auto & i) -> bool {
            return i.second.did_exist_before() || i.second.is_unrelated();
        });
    }

    void SignalManager::handle_signals()
    {
        if (! m_handling_signals)
        {
            // Set up the signal handling thread.
            auto handler_function = [this]() -> void {
                Poller poller{};
                SignalSet all_signal_set{};
                all_signal_set.fill_with_all_signals();
                set_thread_signal_mask(all_signal_set);
                auto & read_handle = m_pipe.file_handle_for_reading();
                poller.add_handle(read_handle.fileDescriptor(), PollEvent::Read,
                                  [this, &read_handle](int handle) -> void {
                                      (void)handle; // we will use read_handle rather than handle directly

                                      int sig{0};
                                      siginfo_t info;
                                      ucontext_t context;

                                      auto data = read_handle.readDataOfLength(sizeof(int));
                                      if (data.length() != sizeof(int))
                                      {
                                          return;
                                      }

                                      memcpy(&sig, data.bytes(), data.length());

                                      data = read_handle.readDataOfLength(sizeof(siginfo_t));
                                      if (data.length() != sizeof(siginfo_t))
                                      {
                                          return;
                                      }

                                      memcpy(&info, data.bytes(), data.length());

                                      data = read_handle.readDataOfLength(sizeof(ucontext_t));
                                      if (data.length() != sizeof(ucontext_t))
                                      {
                                          return;
                                      }

                                      memcpy(&context, data.bytes(), data.length());

                                      call_handlers(sig, &info, &context);
                                  });
                while (! m_thread_controller.shouldStop())
                {
                    poller.wait_for(std::chrono::microseconds{300});
                }
            };
            m_handler_thread = std::thread{handler_function};

            // Now register the signals in with the system.
            std::for_each(m_signal_map.begin(), m_signal_map.end(), [this](auto & entry) -> void {
                if (entry.second.size() > 0)
                {
                    auto original_handler = install_handler_in_system(signal_manager_handler(entry.first));
                    void * original_handler_function = reinterpret_cast<void *>(original_handler.get_simple_handler());
                    if ((! entry.second.contains(original_handler_function)) &&
                        (original_handler_function != SIG_DFL) && (original_handler_function != SIG_IGN) &&
                        (original_handler_function != signal_manager_handler_function))
                    {
                        entry.second.insert(std::make_pair(original_handler_function, original_handler));
                    }
                }
            });

            m_handling_signals = true;
        }
    }

    void SignalManager::stop_handling_signals()
    {
        if (m_handling_signals)
        {
            // First stop the handler thread
            m_thread_controller.signalStop();
            m_thread_controller.signal();

            // Just in case the handler thread is trying to read something from the signal handler
            // function, we close the write end of the pipe.
            auto & write_handle = m_pipe.file_handle_for_writing();
            write_handle.closeFile();

            if (m_handler_thread.joinable())
            {
                m_handler_thread.join();
            }
            else
            {
                m_handler_thread.detach();
            }

            m_mutex.lock();

            // Get the list of currently handled signals
            std::vector<int> signal_list;
            std::for_each(m_signal_map.cbegin(), m_signal_map.cend(), [&signal_list](auto & i) -> void {
                signal_list.emplace_back(i.first);
            });

            // We need to unlock the mutex so that do_not_handle_signal can use the mutex.
            m_mutex.unlock();

            // Stop handling those signals
            std::for_each(signal_list.cbegin(), signal_list.cend(), [this](int sig) -> void {
                do_not_handle_signal(sig);
            });

            m_handling_signals = false;
        }
    }

    size_t SignalManager::get_number_of_installed_handlers()
    {
        auto_lock_type lock{m_mutex};
        size_t counter{0};
        std::for_each(m_signal_map.cbegin(), m_signal_map.cend(), [&counter](auto & i) -> void {
            counter += i.second.size();
        });
        return counter;
    }

    SignalManager::handler_type SignalManager::signal_manager_handler(int sig)
    {
        return handler_type{sig, signal_manager_handler_function};
    }

    SignalManager::handler_type SignalManager::ignore_handler(int sig)
    {
        return handler_type{sig, SIG_IGN};
    }

    SignalManager::handler_type SignalManager::default_handler(int sig)
    {
        return handler_type{sig, SIG_DFL};
    }

    void SignalManager::uninstall_handler_from_handler_function(int sig, void * function)
    {
        auto_lock_type lock{m_mutex};
        auto & handler_map = m_signal_map[sig];

        if (handler_map.contains(function))
        {
            handler_map.erase(function);
        }

        if (handler_map.empty())
        {
            m_signal_map.erase(sig);
        }
    }

    SignalManager::signal_set_type SignalManager::get_process_signal_mask()
    {
        sigset_t current_mask{};
        auto api_result = sigprocmask(SIG_SETMASK, nullptr, &current_mask);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "sigprocmask failed");
        }
        return signal_set_type{current_mask};
    }

    SignalManager::signal_set_type SignalManager::set_process_signal_mask(const signal_set_type & set)
    {
        sigset_t current_mask{};
        sigset_t new_mask{set.get_raw_set()};
        auto api_result = sigprocmask(SIG_SETMASK, &new_mask, &current_mask);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "sigprocmask failed");
        }
        return signal_set_type{current_mask};
    }

    SignalManager::signal_set_type SignalManager::get_thread_signal_mask()
    {
        sigset_t current_mask{};
        auto api_result = pthread_sigmask(SIG_SETMASK, nullptr, &current_mask);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "pthread_sigmask failed");
        }
        return signal_set_type{current_mask};
    }

    SignalManager::signal_set_type SignalManager::set_thread_signal_mask(const signal_set_type & set)
    {
        sigset_t current_mask{};
        sigset_t new_mask{set.get_raw_set()};
        auto api_result = pthread_sigmask(SIG_SETMASK, &new_mask, &current_mask);
        if (api_result != 0)
        {
            throw std::system_error(errno, std::system_category(), "pthread_sigmask failed");
        }
        return signal_set_type{current_mask};
    }

    void signal_manager_handler_function(int s, siginfo_t * info, void * vp)
    {
        auto manager = SignalManager::get_manager();
        bool shutdown{false};

        if (s == SIGHUP || s == SIGSEGV || s == SIGBUS)
        {
            // The handler cannot recover from a SIGSEGV/SIGBUS signal, so we handle them
            // directly here. For now treat SIGHUP the same way.
            if (manager->has_unrelated_or_existing_before_handlers(s))
            {
                manager->call_unrelated_or_existing_before_handlers(s, info, vp);
            }
            else
            {
                manager->call_handlers(s, info, vp);
                shutdown = true;
            }
        }
        else
        {
            // Here we do not want to directly handle the signal, but to signal the handler
            // thread that the signal arrived.
            auto write_handle = manager->m_pipe.write_handle();

            // Write the signal number, info, and context object to the pipe that communicates
            // with the signal handler thread.
            DO_NOT_USE_RESULT write(write_handle, &s, sizeof(s));
            DO_NOT_USE_RESULT write(write_handle, info, sizeof(siginfo_t));
            DO_NOT_USE_RESULT write(write_handle, vp, sizeof(ucontext_t));
        }

        if (shutdown)
        {
            exit(0);
        }

        manager->install_handler_in_system(manager->signal_manager_handler(s));
    }

} // namespace TF::Foundation
