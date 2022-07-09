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
#include <unistd.h>
#include <sys/resource.h>
#if defined(TFLINUX)
#    include <sys/types.h>
#    include <sys/wait.h>
#endif
#define NEEDS_SYSTEM_ERROR
#include "tfheaders.hpp"
#include "tfprocess.hpp"
#include "tfsignalmanager.hpp"
#include "tfcommandline.hpp"

namespace TF::Foundation
{

    Process::Process()
    {
        m_process_id = getpid();
    }

    void Process::launch()
    {
        struct rlimit resource_limit
        {};

        auto api_result = getrlimit(RLIMIT_NOFILE, &resource_limit);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Did not get file descriptor limit"};
        }

        auto signal_manager = SignalManager::get_manager();
        signal_manager->ignore_signal(SIGPIPE);
        if (! signal_manager->is_handling_signals())
        {
            signal_manager->handle_signals();
        }

        auto process_id = fork();

        if (process_id < 0)
        {
            throw std::system_error{errno, std::system_category(), "fork failed"};
        }
        else if (process_id == 0)
        {
            // The child process.
            std::vector<String> argv_list{};

            if (! convert_command_line_to_vector(m_command_line, argv_list))
            {
                throw std::runtime_error{"Unable to convert command line for process launch"};
            }

            // Execv requires an array of char** with the arguments.
            char ** execv_argv = new char *[argv_list.size() + 1];
            std::vector<String>::size_type i{0};
            std::for_each(argv_list.cbegin(), argv_list.cend(), [execv_argv, &i](const String & s) -> void {
                auto cStr = s.cStr();
                auto length = std::strlen(cStr.get());
                execv_argv[i] = new char[length + 1];
                std::memcpy(execv_argv[i], cStr.get(), length);
                execv_argv[i][length] = 0;
                ++i;
            });

            // Now set up the file descriptors to read/write to/from the parent.
            dup2(m_standard_in.read_handle(), STDIN_FILENO);
            dup2(m_standard_out.write_handle(), STDOUT_FILENO);
            dup2(m_standard_err.write_handle(), STDERR_FILENO);

            // By convention STDIN/STDOUT/STDERRR have 0/1/2 file descriptor values.
            if (resource_limit.rlim_max == RLIM_INFINITY)
            {
                resource_limit.rlim_max = 1024;
            }
            for (rlim_t j = STDERR_FILENO + 1; j < resource_limit.rlim_max; j++)
            {
                close(static_cast<int>(j));
            }

            execv_argv[argv_list.size()] = nullptr;
            execv(*execv_argv, execv_argv);

            // Anything past here is an error condition.  We clean up the memory usage and
            // then throw.
            i = 0;
            std::for_each(argv_list.cbegin(), argv_list.cend(), [execv_argv, &i](const String & s) -> void {
                (void)s;
                delete[] execv_argv[i];
            });

            delete[] execv_argv;
            throw std::runtime_error{"execv failed"};
        }
        else
        {
            // The parent case.

            // First handle a fast-exiting child.

            // Stop handling SIGCHLD in this thread.
            auto thread_signal_mask = SignalManager::get_thread_signal_mask();
            thread_signal_mask.add_signal(SIGCHLD);
            SignalManager::set_thread_signal_mask(thread_signal_mask);

            // Set the mask so we can restore it later.
            thread_signal_mask.remove_signal(SIGCHLD);

            int child_status{};
            auto return_id = waitpid(process_id, &child_status, WNOHANG);

            // Restore the thread mask so that we handle SIGCHLD again.
            SignalManager::set_thread_signal_mask(thread_signal_mask);

            if (return_id == process_id || return_id < 0)
            {
                if (WIFEXITED(child_status))
                {
                    m_stop_code = WEXITSTATUS(child_status);
                }
                else if (WIFSIGNALED(child_status))
                {
                    m_stop_code = WTERMSIG(child_status);
                }
                else if (WIFSTOPPED(child_status))
                {
                    m_stop_code = WSTOPSIG(child_status);
                }

                if (m_stop_code != 0)
                {
                    throw std::runtime_error{"child failed to launch"};
                }
            }

            // Now close the unused ends of the pipes.
            m_standard_in.close_for_reading();
            m_standard_out.close_for_writing();
            m_standard_err.close_for_writing();
        }

        m_process_id = process_id;
    }

    void Process::launch(const string_type & cl)
    {
        m_command_line = cl;
        launch();
    }

    void Process::kill() const
    {
        this->signal(SIGKILL);
    }

    void Process::pause() const
    {
        this->signal(SIGSTOP);
    }

    void Process::resume() const
    {
        this->signal(SIGCONT);
    }

    void Process::signal(int s) const
    {
        ::kill(m_process_id, s);
    }

    void Process::update_exit_status()
    {
        int child_status{};
        auto return_id = waitpid(m_process_id, &child_status, WNOHANG);
        if (return_id == m_process_id)
        {
            if (WIFEXITED(child_status))
            {
                m_stop_code = WEXITSTATUS(child_status);
            }
            else if (WIFSIGNALED(child_status))
            {
                m_stop_code = WTERMSIG(child_status);
            }
            else if (WIFSTOPPED(child_status))
            {
                m_stop_code = WSTOPSIG(child_status);
            }
        }
    }

    int Process::get_id() const
    {
        return m_process_id;
    }

    int Process::get_parent_id() const
    {
        if (m_process_id == getpid())
        {
            return getppid();
        }
        return getpid();
    }

    Process::string_type Process::get_command_line() const
    {
        return m_command_line;
    }

    bool Process::is_alive() const
    {
        if (m_process_id <= 0)
        {
            return false;
        }
        auto kill_result = ::kill(m_process_id, 0);
        if (kill_result == 0)
        {
            return true;
        }
        if (kill_result < 0 && errno == EPERM)
        {
            return true;
        }
        return false;
    }

    FileHandle & Process::handle_for_standard_in()
    {
        return m_standard_in.file_handle_for_writing();
    }

    FileHandle & Process::handle_for_standard_out()
    {
        return m_standard_out.file_handle_for_reading();
    }

    FileHandle & Process::handle_for_standard_err()
    {
        return m_standard_err.file_handle_for_reading();
    }

    std::ostream & Process::description(std::ostream & o) const
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("Process");
            formatter->addClassMember("m_process_id", m_process_id);
            formatter->addClassMember("m_stop_code", m_stop_code);
            formatter->addClassMember("m_command_line", m_command_line);
            formatter->addClassMember("m_standard_in", m_standard_in);
            formatter->addClassMember("m_standard_out", m_standard_out);
            formatter->addClassMember("m_standard_err", m_standard_err);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    std::ostream & operator<<(std::ostream & o, const Process & p)
    {
        return p.description(o);
    }

} // namespace TF::Foundation
