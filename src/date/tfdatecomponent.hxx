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

#ifndef TFDATECOMPONENT_HPP
#define TFDATECOMPONENT_HPP

#define NEEDS_CHRONO
#define NEEDS_CTIME
#define NEEDS_OSTREAM
#define NEEDS_CSTRING
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdate.hxx"
#include "tfdatetypes.hpp"

namespace TF
{

    namespace Foundation
    {

        template<class Clock>
        class DateComponents : public AllocatorInterface
        {
        public:
            using clock = Clock;

            using date = Date<clock>;

            using duration = typename Clock::duration;

            using rep = typename Clock::rep;

            using period = typename Clock::period;

            using time_point = typename Clock::time_point;

            using size_type = size_t;

            /**
             * @brief default constructor
             */
            DateComponents();

            /**
             * @brief copy constructor
             */
            DateComponents(const DateComponents & c);

            /**
             * @brief rvalue (move) constructor
             */
            DateComponents(DateComponents && c);

            /**
             * @brief Date constructor
             */
            DateComponents(const date & d);

            /**
             * @brief destructor
             */
            ~DateComponents() override {}

            /**
             * @brief assignment operator
             */
            DateComponents & operator=(const DateComponents & c);

            /**
             * @brief rvalue (move) assignment operator
             */
            DateComponents & operator=(DateComponents && c);

            /**
             * @brief gives the fractional value of seconds.
             * @return an integer representing the fractional value of the seconds.
             */
            size_type fractionalSeconds() const;

            /**
             * @brief set the fractional second of the components object
             * @param fs the fractional second.
             */
            void setFractionalSecond(size_type fs);

            /**
             * @brief give the value of the seconds between 0-60.
             * @return seconds
             *
             * The extra second (60) allows for leap seconds.
             */
            int second() const;

            /**
             * @brief set the seconds value (0-60) for the components object.
             * @param s the seconds value
             */
            void setSecond(int s);

            /**
             * @brief return the minutes between 0 - 59.
             * @return minutes
             */
            int minute() const;

            /**
             * @brief set the minute value (0-59) for the components object.
             * @param m the minutes
             */
            void setMinute(int m);

            /**
             * @brief return the hour of the day between 0 - 23
             * @return hour
             */
            int hour() const;

            /**
             * @brief set the hour value (0-23) for the components object.
             * @param h the hour.
             */
            void setHour(int h);

            /**
             * @brief return the day of the month 1-31
             * @return day of month
             */
            int dayOfMonth() const;

            /**
             * @brief set the day of the month (1-31) for the components object.
             * @param d the day.
             */
            void setDayOfMonth(int d);

            /**
             * @brief return the month of the year January - December
             * @return month
             */
            MonthOfYear monthOfYear() const;

            /**
             * @brief set the month of the year for the components object.
             * @param m the month.
             */
            void setMonthOfYear(MonthOfYear m);

            /**
             * @brief return the year
             * @brief the year.
             */
            int year() const;

            /**
             * @brief set the year for the components object.
             * @param y the year.
             */
            void setYear(int y);

            /**
             * @brief return the day of the week Sunday - Saturday
             * @return the day of the week
             */
            DayOfWeek dayOfWeek() const;

            /**
             * @brief return the day in the year (0 - 365) where 0 indicates 1 Jan.
             * @return the day in the year.
             */
            int dayOfYear() const;

            /**
             * @brief check if daylight savings time is active.
             * return true if the daylight savings time is active, false otherwise.
             */
            bool isDaylightSavingsTime() const;

            /**
             * @brief check if the current year is a leap year.
             * @return true if the date represents a leap year, false otherwise.
             */
            bool isLeapYear() const;

            /**
             * @brief return a Date object representing the time contained in the Date Components object.
             * @return the date object.
             */
            date getDate() const;

            std::ostream & description(std::ostream & o) const;

        private:
            void retrieveComponents(const typename clock::time_point & p);

            /**
             * @brief the broken down date into components.
             */
            struct tm components{};

            /**
             * @brief the fractional part of the seconds.
             */
            size_type fractionOfSecond{0};
        };

        template<class Clock>
        std::ostream & operator<<(std::ostream & o, const DateComponents<Clock> & c);

    } // namespace Foundation

} // namespace TF

#include "tfdatecomponent.cxx"

#endif // TFDATECOMPONENT_HPP
