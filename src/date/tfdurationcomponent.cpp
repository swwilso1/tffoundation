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
               m_milliseconds == c.m_milliseconds && m_microseconds == c.m_microseconds;
    }

    bool DurationComponents::operator!=(const DurationComponents & c) const
    {
        return ! (*this == c);
    }

    int32_t DurationComponents::get_microseconds() const
    {
        return m_microseconds;
    }

    void DurationComponents::set_microseconds(int32_t f)
    {
        m_microseconds = f > 1000 ? 1000 : f;
    }

    int32_t DurationComponents::get_milliseconds() const
    {
        return m_milliseconds;
    }

    void DurationComponents::set_milliseconds(int32_t f)
    {
        m_milliseconds = f > 1000 ? 1000 : f;
    }

    int32_t DurationComponents::get_seconds() const
    {
        return m_seconds;
    }

    void DurationComponents::set_seconds(int32_t s)
    {
        m_seconds = s > 60 ? 60 : s;
    }

    int32_t DurationComponents::get_minutes() const
    {
        return m_minutes;
    }

    void DurationComponents::set_minutes(int32_t m)
    {
        m_minutes = m > 60 ? 60 : m;
    }

    int32_t DurationComponents::get_hours() const
    {
        return m_hours;
    }

    void DurationComponents::set_hours(int32_t h)
    {
        m_hours = h > 24 ? 24 : h;
    }

    int32_t DurationComponents::get_days() const
    {
        return m_days;
    }

    void DurationComponents::set_days(int32_t d)
    {
        m_days = d > 7 ? 7 : d;
    }

    int32_t DurationComponents::get_weeks() const
    {
        return m_weeks;
    }

    void DurationComponents::set_weeks(int32_t w)
    {
        // TODO: This may be incorrect...
        m_weeks = w > 5 ? 5 : w;
    }

    int32_t DurationComponents::get_months() const
    {
        return m_months;
    }

    void DurationComponents::set_months(int32_t m)
    {
        m_months = m > 12 ? 12 : m;
    }

    int32_t DurationComponents::get_years() const
    {
        return m_years;
    }

    void DurationComponents::set_years(int32_t y)
    {
        m_years = y > 10 ? 10 : y;
    }

    int32_t DurationComponents::get_decades() const
    {
        return m_decades;
    }

    void DurationComponents::set_decades(int32_t d)
    {
        m_decades = d > 10 ? 10 : d;
    }

    int32_t DurationComponents::get_centuries() const
    {
        return m_centuries;
    }

    void DurationComponents::set_centuries(int32_t c)
    {
        m_centuries = c > 10 ? 10 : c;
    }

    int32_t DurationComponents::get_millenia() const
    {
        return m_millenia;
    }

    void DurationComponents::set_millenia(int32_t m)
    {
        m_millenia = m;
    }

} // namespace TF::Foundation
