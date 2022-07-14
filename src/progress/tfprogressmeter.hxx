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

#ifndef TFPROGRESSMETER_HXX
#define TFPROGRESSMETER_HXX

#define NEEDS_TYPE_TRAITS
#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF::Foundation
{

    /**
     * ProgressMeter provides a class to track the progress of an algorithm/process/etc... that functions
     * like a ProgressBar.  In this case, the class does not write to an output stream, but calls a
     * callback function (could be implemented with Signals/Slots at some point in the future) when a
     * progress event happens.
     * Progress events are just a notification from the meter object that indicates that the task has progressed
     * to another percentage point towards 100% completion.
     * @tparam INTEGER the integer type used to track progress.
     */
    template<typename INTEGER, typename = std::enable_if_t<std::is_integral<INTEGER>::value>>
    class ProgressMeter : public AllocatorInterface
    {
    public:
        using size_type = INTEGER;
        using meter_callback = std::function<void(size_type percentage)>;

        /**
         * @brief constructor to initialize the meter with the total expected ticks and the callback function.
         * @param total the max ticks.
         * @param callback the callback function
         */
        ProgressMeter(size_type total, meter_callback callback) : m_total{total}, m_callback{callback} {}

        /**
         * @brief method to increment the tracked progress value by 1.
         */
        void increment()
        {
            lock_type lock(m_mutex);
            m_current++;
        }

        /**
         * @brief method to increment the tracked progress by a delta value
         * @param offset the value to increment the bar.
         */
        void increment_by(size_type offset)
        {
            lock_type lock(m_mutex);
            m_current += offset;
        }

        /**
         * @brief method to notify the observer of the meter that progress has happened.
         * @param force_notify true if the method should invoke a call to the callback function even if
         * the meter has not progressed one or more percentage points.
         *
         * When @e force_notify is false, the notify function will only invoke the callback function
         * if the meter has recorded progress in percentage points.
         */
        void notify(bool force_notify = false)
        {
            lock_type lock(m_mutex);

            if (m_current > m_total)
            {
                m_current = m_total;
            }

            auto percent = static_cast<size_type>(static_cast<float>(m_current) / static_cast<float>(m_total) * 100.0);

            if (percent > m_past_percent || force_notify)
            {
                m_callback(percent);
            }

            m_past_percent = percent;
        }

    private:
        using lock_type = std::lock_guard<std::mutex>;

        size_type m_total{1};
        size_type m_current{0};
        size_type m_past_percent{0};
        meter_callback m_callback{};
        std::mutex m_mutex{};
    };

} // namespace TF::Foundation

#endif // TFPROGRESSMETER_HXX
