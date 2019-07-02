/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

namespace TF
{

    namespace Foundation
    {

        template<class Clock>
        Date<Clock>::Date()
        {
            theTime = clock::now();
        }


        template<class Clock>
        Date<Clock>::Date(const Date &d)
        {
            theTime = d.theTime;
        }


        template<class Clock>
        Date<Clock>::Date(Date &&d)
        {
            theTime = d.theTime;
        }


        template<class Clock>
        Date<Clock>::Date(const duration &d) : theTime(typename clock::time_point(d))
        {
        }


        template<class Clock>
        Date<Clock> &Date<Clock>::operator=(const Date &d)
        {
            if(this != &d)
            {
                theTime = d.theTime;
            }
            return *this;
        }


        template<class Clock>
        Date<Clock> &Date<Clock>::operator=(Date &&d)
        {
            theTime = d.theTime;
            return *this;
        }


        template<class Clock>
        bool Date<Clock>::operator==(const Date &d) const
        {
            return theTime == d.theTime;
        }


        template<class Clock>
        bool Date<Clock>::operator!=(const Date &d) const
        {
            return theTime != d.theTime;
        }


        template<class Clock>
        bool Date<Clock>::operator<(const Date &d) const
        {
            return theTime < d.theTime;
        }


        template<class Clock>
        bool Date<Clock>::operator<=(const Date &d) const
        {
            return theTime <= d.theTime;
        }


        template<class Clock>
        bool Date<Clock>::operator>=(const Date &d) const
        {
            return theTime >= d.theTime;
        }


        template<class Clock>
        bool Date<Clock>::operator>(const Date &d) const
        {
            return theTime > d.theTime;
        }


        template<class Clock>
        Date<Clock> Date<Clock>::operator+(const duration &d) const
        {
            duration epicDuration = theTime.time_since_epoch();
            return Date(epicDuration + d);
        }


        template<class Clock>
        Date<Clock> Date<Clock>::operator-(const duration &d) const
        {
            duration epicDuration = theTime.time_since_epoch();
            return Date(epicDuration - d);
        }


        template<class Clock>
        Date<Clock> &Date<Clock>::operator+=(const duration &d)
        {
            Date newDate = *this + d;
            theTime = newDate.theTime;
            return *this;
        }


        template<class Clock>
        Date<Clock> &Date<Clock>::operator-=(const duration &d)
        {
            duration epicDuration = theTime.time_since_epoch();
            duration newDuration = epicDuration - d;
            theTime = typename clock::time_point(newDuration);
            return *this;
        }


        template<class Clock>
        typename Date<Clock>::duration Date<Clock>::operator-(const Date &d) const
        {
            return theTime - d.theTime;
        }


        template<class Clock>
        std::ostream &Date<Clock>::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("Date");
                formatter->addClassMember<time_point>("theTime", theTime);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        template<class Clock>
        std::ostream &operator<<(std::ostream &o, const Date<Clock> &d)
        {
            return d.description(o);
        }


    }    // namespace Foundation


}    // namespace TF
