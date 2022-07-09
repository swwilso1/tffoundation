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

#ifndef TFSIGNALSET_HPP
#define TFSIGNALSET_HPP

#define NEEDS_UNORDERED_SET
#define NEEDS_VECTOR
#define NEEDS_CSIGNAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF::Foundation
{

    /**
     * SignalSet represents a set of OS Signals. The programmer can use
     * the set in conjunction with the @e SignalManager to manipulate the
     * process or thread signal handler mask.
     */
    class SignalSet
    {
    public:
        using set_type = sigset_t;

        /**
         * @brief default constructor that gives and empty signal set.
         */
        SignalSet();

        /**
         * @brief constructor from raw OS signal set type.
         * @param set the OS signal set.
         */
        explicit SignalSet(const set_type & set) : m_signal_set{set} {}

        /**
         * @brief static method to return an empty set.
         * @return an empty signal set.
         */
        static SignalSet get_empty_set();

        /**
         * @brief static method to return a full set.
         * @return a full signal set (all available
         *   signals are in the set).
         */
        static SignalSet get_full_set();

        /**
         * @brief method to add a signal to the set.
         * @param sig the signal
         */
        void add_signal(int sig);

        /**
         * @bief method to remove a signal from the set
         * @param sig the signal to remove.
         */
        void remove_signal(int sig);

        /**
         * @brief method to remove all signals from an existing set.
         */
        void clear();

        /**
         * @brief method to fill a signal set with all existing signals.
         */
        void fill_with_all_signals();

        /**
         * @brief method to check if a signal is a member of the set.
         * @param sig the signal
         * @return true if @e sig is a member of the set.
         */
        [[nodiscard]] bool is_member(int sig) const;

        /**
         * @brief method to get the underlying OS signal handler set
         * object.
         * @return the underlying object.
         */
        [[nodiscard]] set_type get_raw_set() const;

        /**
         * @brief method to get a copy of the signals in the set, usually for
         * signal enumeration purposes.
         * @return an unordered_set of the signals in the set.
         */
        [[nodiscard]] std::unordered_set<int> signals_in_set() const;

        /**
         * @brief method to return the number of signals in the set.
         * @return the number of signals.
         */
        [[nodiscard]] std::unordered_set<int>::size_type size() const;

    private:
        set_type m_signal_set{};

        /**
         * A static list of the available signals for the system.
         */
        static std::vector<int> s_signal_list;
    };

} // namespace TF::Foundation

#endif // TFSIGNALSET_HPP