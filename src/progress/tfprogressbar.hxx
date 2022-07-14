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

#ifndef TFPROGRESSBAR_HXX
#define TFPROGRESSBAR_HXX

#define NEEDS_CSTDINT
#define NEEDS_IOMANIP
#define NEEDS_OSTREAM
#define NEEDS_MEMORY
#define NEEDS_MUTEX
#define NEEDS_TYPE_TRAITS
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * ProgressBar provides a simple console program style progress meter that looks like
         *
         * [====...........] XX%
         *
         * The ProgressBar methods are safe to invoke from multiple threads.
         */
        template<typename INTEGER, typename = std::enable_if_t<std::is_integral<INTEGER>::value>>
        class ProgressBar : public AllocatorInterface
        {
        public:
            using size_type = INTEGER;

            /**
             * @brief constructor that takes the bar width parameter
             * @param width the length of the progress bar in characters
             */
            explicit ProgressBar(size_type width) : m_width(width)
            {
                init(width);
            }

            /**
             * @brief constructor that takes the bar width parameter and the total value.
             * @param width the width of the progress bar in characters
             * @param total the total number of items to track with the progress bar.
             */
            ProgressBar(size_type width, size_type total) : m_width{width}, m_total{total}
            {
                init(width);
            }

            /**
             * @brief method to set the total number.
             * @param total the total number value.
             */
            void set_total(size_type total)
            {
                m_total = total;
            }

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
             * @brief method to draw the progress bar to a stream
             * @param o the stream
             * @param force_draw true if the method should force the draw and false otherwise.
             *
             * When @e force_draw is false, the method will only actually draw the progress if the
             * bar progress has increased by a percentage point.
             */
            void draw(std::ostream & o, bool force_draw = false)
            {
                lock_type lock(m_mutex);

                if (m_current > m_total)
                {
                    m_current = m_total;
                }

                size_type percent =
                    static_cast<size_type>(static_cast<float>(m_current) / static_cast<float>(m_total) * 100.0);

                if (percent > m_past_percent || force_draw)
                {
                    size_type previous_length = calculate_length(m_past_percent);
                    size_type new_length = calculate_length(percent);

                    for (size_type i = previous_length; i < new_length; i++)
                    {
                        *(m_meter.get() + i) = '=';
                    }

                    o << "\r"
                      << "[" << m_meter.get() << "] " << std::setw(3) << std::right << percent << "%" << std::flush;
                }

                m_past_percent = percent;
            }

        private:
            using lock_type = std::lock_guard<std::mutex>;
            using unique_pointer_type = std::unique_ptr<char, std::default_delete<char[]>>;

            size_type m_width{10};
            size_type m_total{1};
            size_type m_current{0};
            size_type m_past_percent{0};
            std::mutex m_mutex{};
            unique_pointer_type m_meter{};

            /**
             * @brief helper method to initialize the ProgressBar object.
             * @param width the width of the bar in characters.
             *
             * Used in the class constructors.
             */
            void init(size_type width)
            {
                m_meter = unique_pointer_type(new char[width + 1], std::default_delete<char[]>());
                std::memset(m_meter.get(), '.', sizeof(char) * static_cast<size_t>(width));
                *(m_meter.get() + width) = '\0';
                //                auto pointer = m_meter.get();
                //                *(pointer + width) = '\0';
            }

            /**
             * @brief helper method to calculate the length of the bar given
             * a percentage.
             * @param percent the percentage of the bar
             * @return return the number of characters of progress bar that
             * represent the percentage based on the width of the progress bar.
             */
            size_type calculate_length(size_type percent)
            {
                return static_cast<size_type>(percent * m_width / 100.0);
            }
        };

    } // namespace Foundation

} // namespace TF

#endif // TFPROGRESSBAR_HXX
