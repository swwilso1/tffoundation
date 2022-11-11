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

#ifndef TFPROCESS_HPP
#define TFPROCESS_HPP

#define NEEDS_OSTREAM
#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfpipe.hpp"
#include "tfsleep.hpp"

namespace TF::Foundation
{

    /**
     * The Process class represents a process running in the system. The process
     * can represent the currently running process, some other arbitrary process,
     * or can launch a process. Whe the Process object launches a new process,
     * the object can return file handles to the new process' stdin/stdout/stderr
     * streams.  In addition the Process object can signal, kill, pause, and
     * resume the running process.
     */
    class Process : public AllocatorInterface
    {
    public:
        using string_type = String;

        /**
         * @brief default constructor that will create an object representing the currently
         * running process.
         */
        Process();

        /**
         * @brief constructor that takes a process id and creates an object representing
         * that process.
         * @param id
         */
        explicit Process(int id) : m_process_id{id} {}

        /**
         * @brief constructor that takes a command line argument.
         * @param cl the command line argument.
         */
        explicit Process(const string_type & cl) : m_command_line(cl) {}

        /**
         * @brief destructor
         */
        ~Process() {}

        /**
         * @brief method to launch a process that has a command-line.
         */
        void launch();

        /**
         * @brief method to launch a process with a given command-line.
         * @param cl
         */
        void launch(const string_type & cl);

        /**
         * @brief method to send a SIGKILL signal to the process.
         */
        void kill() const;

        /**
         * @brief method to send a SIGSTOP signal to the process.
         */
        void pause() const;

        /**
         * @brief method to send a SIGCONT signal to the process.
         */
        void resume() const;

        /**
         * @brief method to send an arbitrary signal to the process.
         * @param s
         */
        void signal(int s) const;

        /**
         * Method to get a process' exit status.
         */
        void update_exit_status();

        /**
         * Method to wait till a process terminates.
         *
         * The method returns when the process finishes.
         */
        void wait();

        /**
         * @brief method to wait for a specified amount of time to see if the process finishes.
         * @tparam Rep the type used for the duration representation
         * @tparam Period the type used for the duration period.
         * @param wait_time the duration
         * @return true if the process finished and false otherwise.
         */
        template<class Rep, class Period>
        [[nodiscard]] auto wait_for(const std::chrono::duration<Rep, Period> & wait_time) -> bool
        {
            auto right_now = std::chrono::high_resolution_clock::now();
            auto later = right_now + wait_time;
            return wait_until(later);
        }

        /**
         * @brief method to wait for the process to finish until time reaches a specified clock time.
         * @tparam Clock the type of the clock.
         * @tparam Duration the type of the duration of the clock.
         * @param timeout_time the the time
         * @return true if the process finished and false otherwise.
         */
        template<class Clock, class Duration>
        [[nodiscard]] auto wait_until(const std::chrono::time_point<Clock, Duration> & timeout_time) -> bool
        {
            try
            {
                while (is_alive())
                {
                    Sleep(std::chrono::milliseconds(50));
                    update_exit_status();
                    auto right_now = Clock::now();
                    auto right_now_duration = std::chrono::time_point_cast<Duration>(right_now);
                    if (right_now_duration >= timeout_time)
                    {
                        return ! is_alive();
                    }
                }
            }
            catch (...)
            {
                return false;
            }
            return true;
        }

        /**
         * @brief method to force the launch code to close all possible file descriptors
         * after fork() but before exec().
         * @param close_all true if the launch code should close all possible descriptors.
         */
        void set_close_all_descriptors(bool close_all)
        {
            m_close_all_possible_descriptors = close_all;
        }

        /**
         * @brief method to return the raw process id.
         * @return the raw process id.
         */
        int get_id() const;

        /**
         * @brief method to return the process' parent process
         * id.
         * @return the parent id.
         *
         * This method works only for launched processes or Process
         * objects that represent the currently running process.
         */
        int get_parent_id() const;

        /**
         * @brief method to get the command-line used to launch a process.
         * @return the command-line
         */
        string_type get_command_line() const;

        /**
         * @brief method to check if a process is currently running.
         * @return true if the process is alive.
         */
        [[nodiscard]] bool is_alive() const;

        /**
         * @brief method to return the process exit code.
         * @return the exit code.
         */
        int stop_code() const
        {
            return m_stop_code;
        }

        /**
         * @brief method to get a handle for writing to the
         * process's standard input stream.
         * @return the file handle suitable for writing to stdin.
         */
        FileHandle & handle_for_standard_in();

        /**
         * @brief method to get a handle for reading from the
         * process's standard output stream.
         * @return the file handle suitable for reading from stdout.
         */
        FileHandle & handle_for_standard_out();

        /**
         * @brief method to get a handle for reading from the
         * process's standard err stream.
         * @return the file handle suitable for reading from stderr.
         */
        FileHandle & handle_for_standard_err();

        /**
         * @brief method for writing the process object to a stream.
         * @param o the stream
         * @return the stream after writing the process details to the stream.
         */
        std::ostream & description(std::ostream & o) const;

    private:
        int m_process_id{};
        int m_stop_code{};
        bool m_close_all_possible_descriptors{};
        string_type m_command_line{};
        Pipe m_standard_in{};
        Pipe m_standard_out{};
        Pipe m_standard_err{};

        void update_stop_code_from_status(int status);
    };

    /**
     * @brief overloaded << operator for writing a Process object to a stream.
     * @param o the stream
     * @param p the process object
     * @return @e o the stream after writing @e p to the stream.
     */
    std::ostream & operator<<(std::ostream & o, const Process & p);

} // namespace TF::Foundation

#endif // TFPROCESS_HPP
