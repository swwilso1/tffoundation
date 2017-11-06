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

#include "tfdatetypes.hpp"

namespace TF
{

    namespace Foundation
    {

        std::ostream& operator<<(std::ostream &o, const DayOfWeek &d)
        {
            switch(d)
            {
                case DayOfWeek::Sunday:
                    o << "Sunday";
                    break;
                case DayOfWeek::Monday:
                    o << "Monday";
                    break;
                case DayOfWeek::Tuesday:
                    o << "Tuesday";
                    break;
                case DayOfWeek::Wednesday:
                    o << "Wednesday";
                    break;
                case DayOfWeek::Thursday:
                    o << "Thursday";
                    break;
                case DayOfWeek::Friday:
                    o << "Friday";
                    break;
                case DayOfWeek::Saturday:
                    o << "Saturday";
                    break;
            }

            return o;
        }


        std::ostream& operator<<(std::ostream &o, const MonthOfYear &m)
        {
            switch(m)
            {
                case MonthOfYear::January:
                    o << "January";
                    break;
                case MonthOfYear::February:
                    o << "February";
                    break;
                case MonthOfYear::March:
                    o << "March";
                    break;
                case MonthOfYear::April:
                    o << "April";
                    break;
                case MonthOfYear::May:
                    o << "May";
                    break;
                case MonthOfYear::June:
                    o << "June";
                    break;
                case MonthOfYear::July:
                    o << "July";
                    break;
                case MonthOfYear::August:
                    o << "August";
                    break;
                case MonthOfYear::September:
                    o << "September";
                    break;
                case MonthOfYear::October:
                    o << "October";
                    break;
                case MonthOfYear::November:
                    o << "November";
                    break;
                case MonthOfYear::December:
                    o << "December";
                    break;
            }

            return o;
        }


        std::ostream& operator<<(std::ostream &o, const HalfOfDay &h)
        {
            switch(h)
            {
                case HalfOfDay::AM:
                    o << "AM";
                    break;
                case HalfOfDay::PM:
                    o << "PM";
                    break;
            }

            return o;
        }


        std::ostream& operator<<(std::ostream &o, const struct tm &t)
        {
            o << "[";
            o << t.tm_sec << ",";
            o << t.tm_min << ",";
            o << t.tm_hour << ",";
            o << t.tm_mday << ",";
            o << t.tm_mon << ",";
            o << t.tm_year << ",";
            o << t.tm_wday << ",";
            o << t.tm_yday << ",";
            o << t.tm_isdst << "]";
            return o;
        }


    } // Foundation

} // TF


