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

#ifndef TFSIGNALMANAGER_HPP
#define TFSIGNALMANAGER_HPP

#define NEEDS_UNORDERED_MAP
#define NEEDS_MUTEX
#define NEEDS_THREAD
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfsignalhandlerheaders.hpp"
#include "tfsignalhandler.hpp"
#include "tfsignalset.hpp"
#include "tfthreadcontroller.hpp"
#include "tfpipe.hpp"

namespace TF::Foundation
{
    extern "C"
    {
        void signal_manager_handler_function(int, siginfo_t *, void *);
    }

    /**
     * The SignalManager class implements an interface that coordinates the installation and
     * handling of POSIX process signals.  The class allows for signal handler chaining where
     * the manager can maintain multiple signal handler functions for the same signal.  This
     * facility allows for the integration of programs that use this manager with other library
     * subsystems that also require signal handlers.  This case can happen in interesting places,
     * such as integrating a software layer as native code into the JVM.
     *
     * The manager also provides an interface for setting process and thread signal masks.
     */
    class SignalManager : public AllocatorInterface
    {
    public:
        using handler_type = SignalHandler;

        using signal_set_type = SignalSet;

        using signal_manager_pointer = std::shared_ptr<SignalManager>;

        /**
         * @brief destructor
         */
        ~SignalManager();

        /**
         * @brief static method to return a reference to the signal manager singleton.
         * @return a shared pointer to the singleton instance.
         */
        static signal_manager_pointer get_manager();

        /**
         * @brief method to register a handler with the signal manager.
         * @param handler the handler object.
         */
        void install_handler(const handler_type & handler);

        /**
         * @brief method to register a handler from a signal number and OS struct.
         * @param sig the signal number
         * @param st the OS struct
         * @param external true if the handler is considered external to the manager.
         */
        void install_handler_from_signal_and_struct(int sig, handler_type::signal_struct & st, bool external = false);

        /**
         * @brief method to register a handler from a simple signal handler function pointer.
         * @param sig the signal number
         * @param handler the signal handler function pointer (void (*)(int signum)).
         * @param external true if the handler is considered external to the manager.
         */
        void install_handler_from_signal_and_function(int sig, simple_signal_handler handler, bool external = false);

        /**
         * @brief method to register a handler from a sigaction/SA_SIGINFO handler function pointer.
         * @param sig the signal number
         * @param handler the signal handler function pointer (void (*)(int, siginfo_t *, void *)).
         * @param external true if the handler is considered external to the manager.
         */
        void install_handler_from_signal_and_function(int sig, sigaction_signal_handler handler, bool external = false);

        /**
         * @brief method to actually install the handler as the active handler in the system.
         * @param handler the handler to install
         * @return the previously installed handler.  The previous handler might be SIG_DFL, or SIG_IGN.
         */
        handler_type install_handler_in_system(const handler_type & handler);

        /**
         * @brief method to remove a handler from the manager system.
         * @param handler the handler object.
         */
        void uninstall_handler(const handler_type & handler);

        /**
         * @brief method to remove a handler from the manager system using a simple function pointer
         * (void (*)(int sig)) and a signal.
         * @param sig the signal number
         * @param handler the handler function pointer
         */
        void uninstall_handler_from_signal_and_function(int sig, simple_signal_handler handler);

        /**
         * @brief method to remove a handler from the manager system using a sigaction/SA_SIGINFO
         * (void (*)(int, siginfo_t *, void *)) function pointer.
         * @param sig the signal number
         * @param handler the handler function pointer.
         */
        void uninstall_handler_from_signal_and_function(int sig, sigaction_signal_handler handler);

        /**
         * @brief method to remove all managed handlers from the manager.
         */
        void clear_handlers();

        /**
         * @brief method to register the signal manager handler system as the handler for @e sig.
         * @param sig the signal number
         */
        void handle_signal(int sig);

        /**
         * @brief method to stop having the manager handler system from handling the signal @e sig.
         * @param sig the signal number
         */
        void do_not_handle_signal(int sig);

        /**
         * @brief method to have the manager handler system install SIG_IGN for signal @e sig.
         * @param sig the signal number
         */
        void ignore_signal(int sig);

        /**
         * @brief method to have the manager call all chained handlers for signal @e s.
         * @param s the signal number
         * @param info a pointer to the siginfo_t struct for the signal.
         * @param vp a pointer to the ucontext_t struct for the signal.
         */
        void call_handlers(int s, siginfo_t * info, void * vp);

        /**
         * @brief method to call only unrelated or previously existing handlers for a signal.
         * @param s the signal number
         * @param info a pointer to the siginfo_t struct for the signal
         * @param vp a pointer to the ucontext_t struct for the signal.
         */
        void call_unrelated_or_existing_before_handlers(int s, siginfo_t * info, void * vp);

        /**
         * @brief method to check whether the signal manager mechanism has previously existing
         * handlers or unrelated handlers.
         * @param s the signal number
         * @return true if a previously existing handler or an unrelated handler exists.
         */
        bool has_unrelated_or_existing_before_handlers(int s);

        /**
         * @brief method to register the signal manager system as the signal handler for
         * all managed signals.
         */
        void handle_signals();

        /**
         * @brief method to stop the signal manager system from handling signals.
         */
        void stop_handling_signals();

        /**
         * @brief method to get a boolean indicating whether or not the manager has
         * started handling signals.
         * @return true if the manager is handling signals and false otherwise.
         */
        bool is_handling_signals() const
        {
            return m_handling_signals;
        }

        /**
         * @brief method to get the number of installed signal handlers
         * @return the number of installed handlers
         */
        size_t get_number_of_installed_handlers();

        /**
         * @brief method to get the current process signal mask set
         * @return the process signal mask set
         */
        static signal_set_type get_process_signal_mask();

        /**
         * @brief method to set the process signal mask set
         * @param set the new signal mask set
         * @return the original signal mask set
         */
        static signal_set_type set_process_signal_mask(const signal_set_type & set);

        /**
         * @brief method to get a thread's signal mask set.
         * @return the thread signal mask set.
         */
        static signal_set_type get_thread_signal_mask();

        /**
         * @brief method to set a thread's signal mask set.
         * @param set the new thread signal mask set.
         * @return the old thread signal mask set.
         */
        static signal_set_type set_thread_signal_mask(const signal_set_type & set);

    private:
        using handler_function_map = std::unordered_map<void *, handler_type>;
        using signal_handler_map = std::unordered_map<int, handler_function_map>;
        using auto_lock_type = std::lock_guard<std::mutex>;

        /**
         * @brief default constructor, restricted to implement singleton pattern.
         */
        SignalManager() = default;

        /**
         * @brief helper method to get the handler object that contains the signal manager's
         * handler function pointer for @e sig
         * @param sig the signal number
         * @return the signal manager's handler object.
         */
        static handler_type signal_manager_handler(int sig);

        /**
         * @brief helper method to get a handler object that represents SIG_IGN for @e sig.
         * @param sig the signal number
         * @return the SIG_IGN handler object.
         */
        static handler_type ignore_handler(int sig);

        /**
         * @brief helper method to get a handler object that represents SIG_DFL for @e sig.
         * @param sig the signal number.
         * @return the SIG_DFL handler object.
         */
        static handler_type default_handler(int sig);

        /**
         * @brief helper method to remove the handler information for @e sig that contains
         * @e function.
         * @param sig the signal number
         * @param function the handler function pointer. The handler function pointer can
         * reference either a simple handler (void (*)(int signum)) or a sigaction/SA_SIGINFO
         * (void (*)(int, siginfo_t *, void *))
         */
        void uninstall_handler_from_handler_function(int sig, void * function);

        /**
         * Internally the signal manager system uses a separate thread to run the signal handling
         * functions.  Using a separate thread for running the handler functions removes the restrictions
         * of needing to avoid non-signal-safe system APIs because the will not actually run in the
         * the signal handler.  The handler only receives the signal and forwards the message to the
         * signal handling thread.  Currently SIGSEGV, SIGBUS, and SIGHUP do not behave this way.
         * These handlers *do* execute in the signal handler.  The SIGSEGV/SIGBUS handlers have to
         * execute in the handler because the OS might terminate the process after the process handles
         * the signal.
         */
        ThreadController m_thread_controller{};
        std::mutex m_mutex{};
        std::thread m_handler_thread{};
        signal_handler_map m_signal_map{};
        bool m_handling_signals{false};
        Pipe m_pipe{};

        /**
         * Static objects used to manage the singleton.
         */
        static signal_manager_pointer s_manager;
        static bool s_initialized_the_manager;
        static std::mutex s_manager_mutex;

        friend void signal_manager_handler_function(int, siginfo_t *, void *);
    };

} // namespace TF::Foundation

#endif // TFSIGNALMANAGER_HPP
