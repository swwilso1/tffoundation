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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "tfdatecomponent.hxx"

using namespace std::chrono;

namespace TF
{

    namespace Foundation
    {
        template<class Clock>
        void DateComponents<Clock>::retrieveComponents(const typename clock::time_point & p)
        {
            nanoseconds nanos = duration_cast<nanoseconds>(p.time_since_epoch());
            seconds secs = duration_cast<seconds>(nanos);
            std::time_t theTime = secs.count();
            fractionOfSecond = static_cast<decltype(fractionOfSecond)>(nanos.count() % 1000000000);
            struct tm * brokenDownTime = std::localtime(&theTime);
            if (brokenDownTime != nullptr)
            {
                components = *brokenDownTime;
            }
        }

        template<class Clock>
        typename DateComponents<Clock>::date DateComponents<Clock>::getDate() const
        {
            struct tm componentsCopy = components;
            std::time_t theTime = mktime(&componentsCopy);
            if (theTime == static_cast<std::time_t>(-1))
                throw std::runtime_error("Unable to convert value in components to a Date object");
            seconds secs(theTime);
            nanoseconds nanos = duration_cast<nanoseconds>(secs);
            nanoseconds fractionalNanos(fractionOfSecond);
            return date(duration_cast<typename Clock::duration>(nanos + fractionalNanos));
        }

        template<class Clock>
        DateComponents<Clock>::DateComponents()
        {
            std::memset(reinterpret_cast<void *>(&components), 0, sizeof(struct tm));
            fractionOfSecond = 0;
        }

        template<class Clock>
        DateComponents<Clock>::DateComponents(const DateComponents & c)
        {
            components = c.components;
            fractionOfSecond = c.fractionOfSecond;
        }

        template<class Clock>
        DateComponents<Clock>::DateComponents(DateComponents && c)
        {
            components = c.components;
            fractionOfSecond = c.fractionOfSecond;
        }

        template<class Clock>
        DateComponents<Clock>::DateComponents(const date & d)
        {
            retrieveComponents(d.pointInTime());
        }

        template<class Clock>
        DateComponents<Clock> & DateComponents<Clock>::operator=(const DateComponents & c)
        {
            if (this != &c)
            {
                components = c.components;
                fractionOfSecond = c.fractionOfSecond;
            }

            return *this;
        }

        template<class Clock>
        DateComponents<Clock> & DateComponents<Clock>::operator=(DateComponents && c)
        {
            components = c.components;
            fractionOfSecond = c.fractionOfSecond;
        }

        template<class Clock>
        typename DateComponents<Clock>::size_type DateComponents<Clock>::fractionalSeconds() const
        {
            return fractionOfSecond;
        }

        template<class Clock>
        void DateComponents<Clock>::setFractionalSecond(size_type fs)
        {
            fractionOfSecond = fs;
        }

        template<class Clock>
        int DateComponents<Clock>::second() const
        {
            return components.tm_sec;
        }

        template<class Clock>
        void DateComponents<Clock>::setSecond(int s)
        {
            if (s < 0 || s > 60)
                throw std::range_error("Argument outside of valid range (0-60)");
            components.tm_sec = s;
        }

        template<class Clock>
        int DateComponents<Clock>::minute() const
        {
            return components.tm_min;
        }

        template<class Clock>
        void DateComponents<Clock>::setMinute(int m)
        {
            if (m < 0 || m > 59)
                throw std::range_error("Argument outside of valid range (0-59)");
            components.tm_min = m;
        }

        template<class Clock>
        int DateComponents<Clock>::hour() const
        {
            return components.tm_hour;
        }

        template<class Clock>
        void DateComponents<Clock>::setHour(int h)
        {
            if (h < 0 || h > 23)
                throw std::range_error("Argument outside of valid range (0-23)");
            components.tm_hour = h;
        }

        template<class Clock>
        int DateComponents<Clock>::dayOfMonth() const
        {
            return components.tm_mday;
        }

        template<class Clock>
        void DateComponents<Clock>::setDayOfMonth(int d)
        {
            if (d < 1 || d > 31)
                throw std::range_error("Argument outside of valid range (1-31)");
            components.tm_mday = d;
        }

        template<class Clock>
        MonthOfYear DateComponents<Clock>::monthOfYear() const
        {
            MonthOfYear theMonth;

            switch (components.tm_mon)
            {
                case 0:
                    theMonth = MonthOfYear::January;
                    break;
                case 1:
                    theMonth = MonthOfYear::February;
                    break;
                case 2:
                    theMonth = MonthOfYear::March;
                    break;
                case 3:
                    theMonth = MonthOfYear::April;
                    break;
                case 4:
                    theMonth = MonthOfYear::May;
                    break;
                case 5:
                    theMonth = MonthOfYear::June;
                    break;
                case 6:
                    theMonth = MonthOfYear::July;
                    break;
                case 7:
                    theMonth = MonthOfYear::August;
                    break;
                case 8:
                    theMonth = MonthOfYear::September;
                    break;
                case 9:
                    theMonth = MonthOfYear::October;
                    break;
                case 10:
                    theMonth = MonthOfYear::November;
                    break;
                case 11:
                    theMonth = MonthOfYear::December;
                    break;
            }

            return theMonth;
        }

        template<class Clock>
        void DateComponents<Clock>::setMonthOfYear(MonthOfYear m)
        {
            switch (m)
            {
                case MonthOfYear::January:
                    components.tm_mon = 0;
                    break;
                case MonthOfYear::February:
                    components.tm_mon = 1;
                    break;
                case MonthOfYear::March:
                    components.tm_mon = 2;
                    break;
                case MonthOfYear::April:
                    components.tm_mon = 3;
                    break;
                case MonthOfYear::May:
                    components.tm_mon = 4;
                    break;
                case MonthOfYear::June:
                    components.tm_mon = 5;
                    break;
                case MonthOfYear::July:
                    components.tm_mon = 6;
                    break;
                case MonthOfYear::August:
                    components.tm_mon = 7;
                    break;
                case MonthOfYear::September:
                    components.tm_mon = 8;
                    break;
                case MonthOfYear::October:
                    components.tm_mon = 9;
                    break;
                case MonthOfYear::November:
                    components.tm_mon = 10;
                    break;
                case MonthOfYear::December:
                    components.tm_mon = 11;
                    break;
            }
        }

        template<class Clock>
        int DateComponents<Clock>::year() const
        {
            // Not sure the 1900 used here is portable.
            return components.tm_year + 1900;
        }

        template<class Clock>
        void DateComponents<Clock>::setYear(int y)
        {
            if ((y - 1900) < 0)
                throw std::range_error("Argument lies outside valid range");
            components.tm_year = y - 1900;
        }

        template<class Clock>
        DayOfWeek DateComponents<Clock>::dayOfWeek() const
        {
            DayOfWeek theDay;
            switch (components.tm_wday)
            {
                case 0:
                    theDay = DayOfWeek::Sunday;
                    break;
                case 1:
                    theDay = DayOfWeek::Monday;
                    break;
                case 2:
                    theDay = DayOfWeek::Tuesday;
                    break;
                case 3:
                    theDay = DayOfWeek::Wednesday;
                    break;
                case 4:
                    theDay = DayOfWeek::Thursday;
                    break;
                case 5:
                    theDay = DayOfWeek::Friday;
                    break;
                case 6:
                    theDay = DayOfWeek::Saturday;
                    break;
            }

            return theDay;
        }

        template<class Clock>
        int DateComponents<Clock>::dayOfYear() const
        {
            return components.tm_yday;
        }

        template<class Clock>
        bool DateComponents<Clock>::isDaylightSavingsTime() const
        {
            if (components.tm_isdst > 0)
                return true;
            return false;
        }

        template<class Clock>
        bool DateComponents<Clock>::isLeapYear() const
        {
            int theYear = year();
            if (theYear % 400 == 0)
                return true;
            if (theYear % 100 == 0)
                return false;
            if (theYear % 4 == 0)
                return true;
            return false;
        }

        template<class Clock>
        std::ostream & DateComponents<Clock>::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("DateComponents");
                formatter->addClassMember<struct tm>("components", components);
                formatter->addClassMember<size_type>("fractionOfSecond", fractionOfSecond);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        template<class Clock>
        std::ostream & operator<<(std::ostream & o, const DateComponents<Clock> & d)
        {
            return d.description(o);
        }

    } // namespace Foundation

} // namespace TF
