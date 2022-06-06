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

#include "tfdurationcomponent.hpp"

namespace TF::Foundation
{

    bool DurationComponents::operator==(const DurationComponents & c) const
    {
        if (this == &c)
        {
            return true;
        }

        return m_millenia == c.m_millenia && m_centuries == c.m_centuries && m_decades == c.m_decades &&
               m_years == c.m_years && m_months == c.m_months && m_weeks == c.m_weeks && m_days == c.m_days &&
               m_hours == c.m_hours && m_minutes == c.m_minutes && m_seconds == c.m_seconds &&
               m_microseconds == c.m_microseconds;
    }

    bool DurationComponents::operator!=(const DurationComponents & c) const
    {
        return ! (*this == c);
    }

    DurationComponents::size_type DurationComponents::get_microseconds() const
    {
        return m_microseconds;
    }

    void DurationComponents::set_microseconds(size_type f)
    {
        m_microseconds = f;
    }

    uint8_t DurationComponents::get_seconds() const
    {
        return m_seconds;
    }

    DurationComponents::size_type DurationComponents::get_milliseconds() const
    {
        std::chrono::microseconds us{m_microseconds};
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(us);
        return static_cast<size_type>(ms.count());
    }

    void DurationComponents::set_milliseconds(size_type f)
    {
        std::chrono::milliseconds ms{f};
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(ms);
        m_microseconds = static_cast<decltype(m_microseconds)>(us.count());
    }

    void DurationComponents::set_seconds(uint8_t s)
    {
        m_seconds = s > 60 ? 60 : s;
    }

    uint8_t DurationComponents::get_minutes() const
    {
        return m_minutes;
    }

    void DurationComponents::set_minutes(uint8_t m)
    {
        m_minutes = m > 60 ? 60 : m;
    }

    uint8_t DurationComponents::get_hours() const
    {
        return m_hours;
    }

    void DurationComponents::set_hours(uint8_t h)
    {
        m_hours = h > 24 ? 24 : h;
    }

    uint8_t DurationComponents::get_days() const
    {
        return m_days;
    }

    void DurationComponents::set_days(uint8_t d)
    {
        m_days = d > 7 ? 7 : d;
    }

    uint8_t DurationComponents::get_weeks() const
    {
        return m_weeks;
    }

    void DurationComponents::set_weeks(uint8_t w)
    {
        // TODO: This may be incorrect...
        m_weeks = w > 5 ? 5 : w;
    }

    uint8_t DurationComponents::get_months() const
    {
        return m_months;
    }

    void DurationComponents::set_months(uint8_t m)
    {
        m_months = m > 12 ? 12 : m;
    }

    uint8_t DurationComponents::get_years() const
    {
        return m_years;
    }

    void DurationComponents::set_years(uint8_t y)
    {
        m_years = y > 10 ? 10 : y;
    }

    uint8_t DurationComponents::get_decades() const
    {
        return m_decades;
    }

    void DurationComponents::set_decades(uint8_t d)
    {
        m_decades = d > 10 ? 10 : d;
    }

    uint8_t DurationComponents::get_centuries() const
    {
        return m_centuries;
    }

    void DurationComponents::set_centuries(uint8_t c)
    {
        m_centuries = c > 10 ? 10 : c;
    }

    DurationComponents::size_type DurationComponents::get_millenia() const
    {
        return m_millenia;
    }

    void DurationComponents::set_millenia(size_type m)
    {
        m_millenia = m;
    }

} // namespace TF::Foundation
