/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

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
SOFTWARE.


******************************************************************************/

#include "tfprogressbar.hpp"

#define NEEDS_CSTRING
#define NEEDS_IOMANIP
#include "tfheaders.hpp"

namespace TF
{

    namespace Foundation
    {

        ProgressBar::ProgressBar(size_type width) : m_width{width}
        {
            init(width);
        }

        ProgressBar::ProgressBar(size_type width, size_type total) : m_width(width), m_total{total}
        {
            init(width);
        }

        void ProgressBar::init(size_type width)
        {
            m_meter = std::make_unique<char>(width + 1);
            std::memset(m_meter.get(), '.', sizeof(char) * width);
            *(m_meter.get() + width) = '\0';
        }

        void ProgressBar::draw(std::ostream & o, bool force_draw)
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

        ProgressBar::size_type ProgressBar::calculate_length(size_type percent)
        {
            return static_cast<size_type>(percent * m_width / 100.0);
        }

    } // namespace Foundation

} // namespace TF
