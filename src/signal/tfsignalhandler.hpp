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

#ifndef TFSIGNALHANDLER_HPP
#define TFSIGNALHANDLER_HPP

#define NEEDS_UNORDERED_SET
#define NEEDS_VECTOR
#define NEEDS_OSTREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfsignalhandlerheaders.hpp"

namespace TF::Foundation
{
    extern "C"
    {
        /**
         * Type definition for a simple (non-sigaction/non-SA_SIGINFO) signal handler
         * function.
         */
        using simple_signal_handler = void (*)(int signum);

        /**
         * Type definition for a sigaction/SA_SIGINFO signal handler function.
         */
        using sigaction_signal_handler = void (*)(int, siginfo_t *, void *);
    }

    /**
     * Enumeration of signal handler flags.  These exist solely for the purpose
     * of readability.  For example, the flag NO_CHILD_STOP communicates a
     * handlers slightly more clearly than SA_NOCLDSTOP.
     */
    enum class SignalHandlerFlag
    {
        NO_CHILD_STOP = 0x1,
        NO_CHILD_WAIT = 0x2,
        ON_STACK = 0x4,
        NO_DEFER = 0x8,
        RESET_HANDLER_TO_DEFAULT = 0x10,
        RESTART_SYSTEM_CALLS = 0x20,
        SIGACTION = 0x40
    };

    /**
     * SignalHandler encapsulates the lower level details of a signal handler representation.
     * The class captures the signal number, the handler function pointer, and the signal flags
     * for the represent a handler for a signal.
     */
    class SignalHandler : public AllocatorInterface
    {
    public:
        /**
         * Raw OS level signal structure.
         */
        using signal_struct = struct sigaction;

        /**
         * The signal handler flag representation.
         */
        using handler_flag = SignalHandlerFlag;

        /**
         * @brief constructor for creating a handler from a signal number and an existing raw
         * OS signal handler structure.
         * @param number the signal number
         * @param s the raw OS handler structure.
         * @param existed true if the handler existed before installing a handler with the
         * SignalManager class.
         * @param unrelated true if the handler was installed in a way unrelated to the SignalManager
         */
        SignalHandler(int number, const signal_struct & s, bool existed = false, bool unrelated = false);

        /**
         * @brief constructor creating a handler from a signal number and a simple signal handler function.
         * @param number the signal number
         * @param h the handler function pointer
         * @param existed true if the handler existed before installing a handler with the
         * SignalManager class.
         * @param unrelated true if the handler was installed in a way unrelated to the SignalManager
         */
        SignalHandler(int number, simple_signal_handler h, bool existed = false, bool unrelated = false);

        /**
         * @brief constructor creating a handler from a signal number and a sigaction/SA_SIGINFO signal handler
         * function.
         * @param number the signal number
         * @param h the handler function pointer
         * @param existed true if the handler existed before installing a handler with the
         * SignalManager class.
         * @param unrelated true if the handler was installed in a way unrelated to the SignalManager
         */
        SignalHandler(int number, sigaction_signal_handler h, bool existed = false, bool unrelated = false);

        /**
         * @brief equality comparison operator.
         * @param h the other signal handler
         * @return true if the two handlers are the same.
         */
        bool operator==(const SignalHandler & h) const;

        /**
         * @brief inequality comparison operator.
         * @param h the other signal handler.
         * @return true if the two handlers are not the same.
         */
        bool operator!=(const SignalHandler & h) const;

        /**
         * @brief method to get the signal number that the handler handles.
         * @return the signal number.
         */
        int get_number() const;

        /**
         * @brief method to get the 'unrelated' property.  A handler is unrelated
         * when it comes from outside the library signal handler mechanisms.
         * @return true if the handler comes for outside the library handler mechanisms.
         */
        bool is_unrelated() const
        {
            return m_unrelated;
        }

        /**
         * @brief method to get the 'existed before' property.  A handler existed before
         * if the handler was in use prior the SignalManager class handling signals.
         * @return true if the handler existed before the use SignalManager.
         */
        bool did_exist_before() const
        {
            return m_existed_before;
        }

        /**
         * @brief method to add a flag option to the handler.  Flags control the behavior
         * of the signal system.
         * @param flag the flag.
         */
        void add_flag(handler_flag flag);

        /**
         * @brief method to set flags for the handler.
         * @param flags a group of handler flags.
         *
         * This method allows the binary or of the flags into a signal argument such as the following:
         *
         * handler.set_flags(NO_CHILD_WAIT | NO_DEFER | RESTART_SYSTEM_CALLS)
         */
        void set_flags(int flags);

        /**
         * @brief method to get a set of the currently active flags set in the handler.
         * @return the set of active flags.
         */
        std::unordered_set<handler_flag> get_flags() const;

        /**
         * @brief method to check if a handler flag is set in a handler.
         * @param flag the handler flag.
         * @return true if the handler flag is set and false otherwise.
         */
        bool uses_flag(handler_flag flag) const;

        /**
         * @brief method to get the simple handler function pointer.
         * @return the function pointer.
         */
        simple_signal_handler get_simple_handler() const;

        /**
         * @brief method to get the sigaction/SA_SIGINFO function pointer.
         * @return the function pointer
         */
        sigaction_signal_handler get_sigaction_handler() const;

        /**
         * @brief method to get the raw OS signal handler structure.
         * @return the OS signal handler structure.
         */
        signal_struct get_structure() const;

        /**
         * @brief functor method for calling the handler object as a signal handler.
         * @param info a siginfo_t handler delivered by the Kernel to the signal handler.
         * @param vp the vp (ucontext_t) pointer delivered by the Kernel to the handler.
         */
        void operator()(siginfo_t * info, void * vp) const;

        /**
         * @brief method for writing the handler details to a stream.
         * @param o the stream object
         * @return the stream object after writing the handler details to the stream.
         */
        std::ostream & description(std::ostream & o) const;

    private:
        /**
         * @brief method to check if the handler is a sigaction/SA_SIGINFO handler.
         * @return true the handler is a sigaction/SA_SIGACTION handler.
         */
        bool is_siginfo_handler() const;

        int m_signal_number{};
        signal_struct m_action{};
        bool m_existed_before{false};
        bool m_unrelated{false};

        /**
         * A static list of the handler flags supported by the running system.
         */
        static std::vector<handler_flag> s_handler_flags;
    };

    /**
     * @brief overloaded << operator for SignalHandler objects.
     * @param o the stream object
     * @param h the handler object
     * @return @e o the stream after writing the contents to the stream.
     */
    std::ostream & operator<<(std::ostream & o, const SignalHandler & h);

} // namespace TF::Foundation

/**
 * @brief overloaded << operator for raw OS signal handler structs.
 * @param o the stream object
 * @param s the OS signal handler struct.
 * @return
 */
std::ostream & operator<<(std::ostream & o, const TF::Foundation::SignalHandler::signal_struct & s);

#endif // TFSIGNALHANDLER_HPP
