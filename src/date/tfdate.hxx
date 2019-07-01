/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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

#ifndef TFDATE_HPP
#define TFDATE_HPP

#define NEEDS_OSTREAM
#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfchronostream.hxx"


namespace TF
{

    namespace Foundation
    {

        template<class Clock>
        class Date : public AllocatorInterface
        {
        public:
            using clock = Clock;

            using duration = typename Clock::duration;

            using rep = typename Clock::rep;

            using period = typename Clock::period;

            using time_point = typename Clock::time_point;

            /**
             * @brief default constructor that initializes the Date object
             * with the current date/time.
             */
            Date();


            /**
             * @brief copy constructor
             */
            Date(const Date &d);


            /**
             * @brief rvalue(move) constructor
             */
            Date(Date &&d);


            /**
             * @brief duration constructor
             * @return
             */
            Date(const duration &d);


            /**
             * @brief destructor
             */
            ~Date() override
            {
            }


            /**
             * @brief assignment operator
             * @param d the date to assign
             * @return the current date updated with the contents of @e d.
             */
            Date &operator=(const Date &d);


            /**
             * @brief rvalue (move) assignment operator
             * @param d the date to move
             * @return the current date updated with the contents of @e d.
             */
            Date &operator=(Date &&d);


            /**
             * @brief equality operator
             * @param d the date to compare
             * @return true if the two dates refer to the same point in time and false otherwise.
             */
            bool operator==(const Date &d) const;


            /**
             * @brief inequality operator
             * @param d the date to compare
             * @return true if the dates to not refer to the same point in time and false otherwise.
             */
            bool operator!=(const Date &d) const;


            /**
             * @brief less-than operator
             * @param d the date to compare
             * @return true if the current date occurs prior to @e d in time and false otherwise.
             */
            bool operator<(const Date &d) const;

            /**
             * @brief less-than-equal-to operator
             * @param d the date to compare.
             * @return true if the two dates refer to the same point in time, or if the current date occurs
             * prior to @e in time, otherwise false.
             */
            bool operator<=(const Date &d) const;


            /**
             * @brief greater-than-equal-to operator
             * @param d the date to compare.
             * @return true if the two dates refer to the same point in time, or if the current date occurs
             * after @e d in time, otherwise false.
             */
            bool operator>=(const Date &d) const;


            /**
             * @brief greater-than operator
             * @param d the date to compare
             * @return @return true if the current date occurs after @e d in time, otherwise false.
             */
            bool operator>(const Date &d) const;


            /**
             * @brief overloaded + operator for duration types.
             * @param d the duration
             * @return the new date offset by the duration @e d.
             */
            Date operator+(const duration &d) const;


            /**
             * @brief overloaded - operator for duration types
             * @param d the duration
             * @return the new date less the duration @e d.
             */
            Date operator-(const duration &d) const;


            /**
             * @brief overloaded += operator for duration types
             * @param d the duration
             * @return this date object updated by the offset duration @e d.
             */
            Date &operator+=(const duration &d);


            /**
             * @brief overloaded -= operator for duration types
             * @param d the duration
             * @return this date object moved to an earlier point in time by the duration @e d.
             */
            Date &operator-=(const duration &d);


            /**
             * @brief subtract one point in time from another
             * @param d the date to subtract
             * @return the duration of time between the two dates.
             */
            duration operator-(const Date &d) const;


            /**
             * @brief get the time_point object for the date.
             * @return the time_point object.
             */
            time_point pointInTime() const
            {
                return theTime;
            }


            /**
             * @brief stream output function
             * @param o the stream
             * @return @e o
             */
            std::ostream &description(std::ostream &o) const;

        private:
            time_point theTime;
        };


        template<class Clock>
        std::ostream &operator<<(std::ostream &o, const Date<Clock> &d);

    }    // namespace Foundation

}    // namespace TF

#include "tfdate.cxx"

#endif    // TFDATE_HPP
