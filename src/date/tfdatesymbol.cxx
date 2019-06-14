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


#include "tfdatesymbol.hxx"

namespace TF
{

    namespace Foundation
    {

        template<class Clock>
        DateSymbol<Clock>::DateSymbol(const DateSymbol &s)
        {
            count = s.count;
            className = s.className;
            theCharacter = s.theCharacter;
        }


        template<class Clock>
        DateSymbol <Clock> &DateSymbol<Clock>::operator=(const DateSymbol &s)
        {
            if (this != &s)
            {
                count = s.count;
                className = s.className;
                theCharacter = s.theCharacter;
            }

            return *this;
        }


        template<class Clock>
        typename DateSymbol<Clock>::string_type DateSymbol<Clock>::convert(const DateComponents <Clock> &c)
        {
            return string_type("");
        }


        template<class Clock>
        int DateSymbol<Clock>::convert(const string_type &s)
        {
            return 0;
        }



        template<class Clock>
        std::ostream& DateSymbol<Clock>::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName(className.stlString());
                formatter->addClassMember<int>("count", count);
                formatter->addClassMember<char>("theCharacter",theCharacter);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        template<class Clock>
        std::ostream& operator<<(std::ostream &o, const DateSymbol<Clock> &s)
        {
            return s.description(o);
        }


        template<class Clock>
        class EraSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            EraSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "EraSymbol"; }

            EraSymbol(const EraSymbol &s) : DateSymbol<Clock>(s) {}

            ~EraSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Era; }

            static void useCommonEra() { commonEra = true; }
            static void useAnnoDomini() { commonEra = false; }

        private:

            static bool commonEra;
        };


        template<class Clock>
        bool EraSymbol<Clock>::commonEra = false;

        template<class Clock>
        typename EraSymbol<Clock>::string_type EraSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            if(c.year() >= 1)
            {
                // AD/CE
                if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 3)
                    return commonEra ? string_type("CE") : string_type("AD");
                else if(DateSymbol<Clock>::count == 4)
                    return commonEra ? string_type("Common Era") : string_type("Anno Domini");
                else if(DateSymbol<Clock>::count == 5)
                    return commonEra ? string_type("C") : string_type("A");
            }
            else
            {
                if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 3)
                    return commonEra ? string_type("BCE") : string_type("BC");
                else if(DateSymbol<Clock>::count == 4)
                    return commonEra ? string_type("Before Common Era") : string_type("Before Christ");
                else if(DateSymbol<Clock>::count == 5)
                    return string_type("B");
            }
        }


        template<class Clock>
        int EraSymbol<Clock>::minimumCharactersToExpect()
        {
            switch(DateSymbol<Clock>::count)
            {
                case 1:
                case 2:
                case 3:
                    return 2; // 2 for CE/AD/BC
                case 4:
                    return 10; // 10 for 'Common Era'
                case 5:
                    return 1;
            }

            return 0;
        }


        template<class Clock>
        int EraSymbol<Clock>::maximumCharactersToExpect()
        {
            switch(DateSymbol<Clock>::count)
            {
                case 1:
                case 2:
                case 3:
                    return 3; // 3  for BCE
                case 4:
                    return 17; // 17 for 'Before Common Era'
                case 5:
                    return 1;
            }

            return 0;
        }


        template<class Clock>
        bool EraSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            if(v == "BC")
                return true;

            if(v == "AD")
                return true;

            if(v == "CE")
                return true;

            if(v == "BCE")
                return true;

            if(v == "Common Era")
                return true;

            if(v == "Anno Domini")
                return true;

            if( v == "Before Common Era")
                return true;

            if(v == "Before Christ")
                return true;

            if(v == "A")
                return true;

            if(v == "B")
                return true;

            if(v == "C")
                return true;

            return false;
        }


        template<class Clock>
        class YearSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            YearSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "YearSymbol"; }

            YearSymbol(const YearSymbol &s) : DateSymbol<Clock>(s) {}

            ~YearSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Year; }
        };


        template<class Clock>
        typename YearSymbol<Clock>::string_type YearSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            switch(DateSymbol<Clock>::count)
            {
                case 1:
                    accumulator << c.year();
                    break;
                case 2:
                {
                    int theTwo = c.year() % 100;
                    accumulator.width(2);
                    accumulator.fill('0');
                    accumulator << theTwo;
                }
                break;
                default:
                    accumulator.width(DateSymbol<Clock>::count);
                    accumulator.fill('0');
                    accumulator << c.year();
            }

            return accumulator.str();
        }


        template<class Clock>
        int YearSymbol<Clock>::convert(const string_type &s)
        {
            return std::atoi(s.cStr().get());
        }


        template<class Clock>
        int YearSymbol<Clock>::minimumCharactersToExpect()
        {
            switch(DateSymbol<Clock>::count)
            {
                case 1:
                    return 4; // at least four because our year value should not go below 1900.
                case 2:
                    return 2;
                default:
                    return DateSymbol<Clock>::count;
            }

            return 0;
        }


        template<class Clock>
        int YearSymbol<Clock>::maximumCharactersToExpect()
        {
            switch(DateSymbol<Clock>::count)
            {
                case 1:
                    return 4; // Not happy with a maximum of four, but don't know how to tell otherwise.
                case 2:
                    return 2;
                default:
                    return DateSymbol<Clock>::count;
            }

            return 0;
        }


        template<class Clock>
        bool YearSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            // Check to make sure all the characters are digit characters.
            for(auto c : v)
            {
                if(! (c >= '0' && c <= '9'))
                    return false;
            }

            // Now check that the value length is the correct length.
            if(DateSymbol<Clock>::count == 1)
            {
                if(v.length() != 4)
                    return false; // Not happy with four.
            }

            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }


            if(DateSymbol<Clock>::count != v.length())
                return false;

            return true;
        }


        template<class Clock>
        class QuarterSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            QuarterSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "QuarterSymbol"; }

            QuarterSymbol(const QuarterSymbol &s) : DateSymbol<Clock>(s) {}

            ~QuarterSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Quarter; }
        };


#       define SET_QUARTER(quarter, phrase) \
        { \
           if(DateSymbol<Clock>::count == 1 || DateSymbol<Clock>::count == 5) \
               accumulator << quarter; \
           else if(DateSymbol<Clock>::count == 2) \
           { \
               accumulator.width(2); \
               accumulator.fill('0'); \
               accumulator << quarter; \
           } \
           else if(DateSymbol<Clock>::count == 3) \
               accumulator << "Q" << quarter; \
           else if(DateSymbol<Clock>::count == 4) \
               accumulator << phrase; \
        }


        template<class Clock>
        typename QuarterSymbol<Clock>::string_type QuarterSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            auto monthOfYear = c.monthOfYear();
            if(monthOfYear >= MonthOfYear::January && monthOfYear <= MonthOfYear::March)
                SET_QUARTER(1, "1st quarter")
            else if(monthOfYear >= MonthOfYear::April && monthOfYear <= MonthOfYear::June)
                SET_QUARTER(2, "2nd quarter")
            else if(monthOfYear >= MonthOfYear::July && monthOfYear <= MonthOfYear::September)
                SET_QUARTER(3, "3rd quarter")
            else
                SET_QUARTER(4, "4th quarter")

            return accumulator.str();
        }


        template<class Clock>
        int QuarterSymbol<Clock>::minimumCharactersToExpect()
        {
            if(DateSymbol<Clock>::count == 1 || DateSymbol<Clock>::count == 5)
                return 1;
            else if(DateSymbol<Clock>::count == 2 || DateSymbol<Clock>::count == 3)
                return 2;
            else if(DateSymbol<Clock>::count == 4)
                return 11; // 11 characters for 'xth quarter'

            return 0;
        }


        template<class Clock>
        int QuarterSymbol<Clock>::maximumCharactersToExpect()
        {
            return minimumCharactersToExpect();
        }


        template<class Clock>
        bool QuarterSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            if(DateSymbol<Clock>::count == 1 || DateSymbol<Clock>::count == 5)
            {
                if(v == "1")
                    return true;
                if(v == "2")
                    return true;
                if(v == "3")
                    return true;
                if(v == "4")
                    return true;
            }
            else if(DateSymbol<Clock>::count == 2)
            {
                if(v == "01")
                    return true;
                if(v == "02")
                    return true;
                if(v == "03")
                    return true;
                if(v == "04")
                    return true;
            }
            else if(DateSymbol<Clock>::count == 3)
            {
                if(v == "Q1")
                    return true;
                if(v == "Q2")
                    return true;
                if(v == "Q3")
                    return true;
                if(v == "Q4")
                    return true;
            }
            else if(DateSymbol<Clock>::count == 4)
            {
                if(v == "1st quarter")
                    return true;
                if(v == "2nd quarter")
                    return true;
                if(v == "3rd quarter")
                    return true;
                if(v == "4th quarter")
                    return true;
            }

            return false;
        }


        template<class Clock>
        class MonthSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            MonthSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "MonthSymbol"; }

            MonthSymbol(const MonthSymbol &s) : DateSymbol<Clock>(s) {}

            ~MonthSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Month; }
        };


        template<class Clock>
        typename MonthSymbol<Clock>::string_type MonthSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                switch (c.monthOfYear())
                {
                    case MonthOfYear::January:
                        accumulator << 1;
                        break;
                    case MonthOfYear::February:
                        accumulator << 2;
                        break;
                    case MonthOfYear::March:
                        accumulator << 3;
                        break;
                    case MonthOfYear::April:
                        accumulator << 4;
                        break;
                    case MonthOfYear::May:
                        accumulator << 5;
                        break;
                    case MonthOfYear::June:
                        accumulator << 6;
                        break;
                    case MonthOfYear::July:
                        accumulator << 7;
                        break;
                    case MonthOfYear::August:
                        accumulator << 8;
                        break;
                    case MonthOfYear::September:
                        accumulator << 9;
                        break;
                    case MonthOfYear::October:
                        accumulator << 10;
                        break;
                    case MonthOfYear::November:
                        accumulator << 11;
                        break;
                    case MonthOfYear::December:
                        accumulator << 12;
                        break;
                }
            }
            else if(DateSymbol<Clock>::count == 3)
            {
                switch (c.monthOfYear())
                {
                    case MonthOfYear::January:
                        accumulator << "Jan";
                        break;
                    case MonthOfYear::February:
                        accumulator << "Feb";
                        break;
                    case MonthOfYear::March:
                        accumulator << "Mar";
                        break;
                    case MonthOfYear::April:
                        accumulator << "Apr";
                        break;
                    case MonthOfYear::May:
                        accumulator << "May";
                        break;
                    case MonthOfYear::June:
                        accumulator << "June";
                        break;
                    case MonthOfYear::July:
                        accumulator << "July";
                        break;
                    case MonthOfYear::August:
                        accumulator << "Aug";
                        break;
                    case MonthOfYear::September:
                        accumulator << "Sept";
                        break;
                    case MonthOfYear::October:
                        accumulator << "Oct";
                        break;
                    case MonthOfYear::November:
                        accumulator << "Nov";
                        break;
                    case MonthOfYear::December:
                        accumulator << "Dec";
                        break;
                }
            }
            else if(DateSymbol<Clock>::count == 4)
            {
                switch (c.monthOfYear())
                {
                    case MonthOfYear::January:
                        accumulator << "January";
                        break;
                    case MonthOfYear::February:
                        accumulator << "February";
                        break;
                    case MonthOfYear::March:
                        accumulator << "March";
                        break;
                    case MonthOfYear::April:
                        accumulator << "April";
                        break;
                    case MonthOfYear::May:
                        accumulator << "May";
                        break;
                    case MonthOfYear::June:
                        accumulator << "June";
                        break;
                    case MonthOfYear::July:
                        accumulator << "July";
                        break;
                    case MonthOfYear::August:
                        accumulator << "August";
                        break;
                    case MonthOfYear::September:
                        accumulator << "September";
                        break;
                    case MonthOfYear::October:
                        accumulator << "October";
                        break;
                    case MonthOfYear::November:
                        accumulator << "November";
                        break;
                    case MonthOfYear::December:
                        accumulator << "December";
                        break;
                }
            }
            else if(DateSymbol<Clock>::count == 5)
            {
                switch (c.monthOfYear())
                {
                    case MonthOfYear::January:
                        accumulator << 'J';
                        break;
                    case MonthOfYear::February:
                        accumulator << 'F';
                        break;
                    case MonthOfYear::March:
                        accumulator << 'M';
                        break;
                    case MonthOfYear::April:
                        accumulator << 'A';
                        break;
                    case MonthOfYear::May:
                        accumulator << 'M';
                        break;
                    case MonthOfYear::June:
                        accumulator << 'J';
                        break;
                    case MonthOfYear::July:
                        accumulator << 'J';
                        break;
                    case MonthOfYear::August:
                        accumulator << 'A';
                        break;
                    case MonthOfYear::September:
                        accumulator << 'S';
                        break;
                    case MonthOfYear::October:
                        accumulator << 'O';
                        break;
                    case MonthOfYear::November:
                        accumulator << 'N';
                        break;
                    case MonthOfYear::December:
                        accumulator << 'D';
                        break;
                }
            }

            return accumulator.str();
        }


        template<class Clock>
        int MonthSymbol<Clock>::convert(const string_type &s)
        {
            auto theCount = DateSymbol<Clock>::count;

            switch(theCount)
            {
                case 1:
                case 2:
                {
                    auto theValue = std::atoi(s.cStr().get());
                    return theValue;
                }
                    break;
                case 3:
                {
                    if(s == "Jan")
                        return 1;
                    if(s == "Feb")
                        return 2;
                    if(s == "Mar")
                        return 3;
                    if(s == "Apr")
                        return 4;
                    if(s == "May")
                        return 5;
                    if(s == "June")
                        return 6;
                    if(s == "July")
                        return 7;
                    if(s == "Aug")
                        return 8;
                    if(s == "Sept")
                        return 9;
                    if(s == "Oct")
                        return 10;
                    if(s == "Nov")
                        return 11;
                    if(s == "Dec")
                        return 12;
                }
                break;
                case 4:
                {
                    if(s == "January")
                        return 1;
                    if(s == "February")
                        return 2;
                    if(s == "March")
                        return 3;
                    if(s == "April")
                        return 4;
                    if(s == "May")
                        return 5;
                    if(s == "June")
                        return 6;
                    if(s == "July")
                        return 7;
                    if(s == "August")
                        return 8;
                    if(s == "September")
                        return 9;
                    if(s == "October")
                        return 10;
                    if(s == "November")
                        return 11;
                    if(s == "December")
                        return 12;
                }
                break;
                case 5:
                    if(s == "F")
                        return 2;
                    if(s == "S")
                        return 9;
                    if(s == "N")
                        return 11;
                    if(s == "O")
                        return 10;
                    if(s == "D")
                        return 12;
                    throw std::runtime_error("Insufficient information to reconstruct month value");
            }
            return 0;
        }


        template<class Clock>
        int MonthSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            if(theCount == 3)
                return 3;
            if(theCount == 4)
                return 3;
            if(theCount == 5)
                return 1;
            
            return 0;
        }


        template<class Clock>
        int MonthSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount == 1)
                return 2; // Double digit month value.
            if(theCount == 2)
                return 2; // always 2 characters.
            if(theCount == 3)
                return 4; // June/July/Sept all have 4 characters
            if(theCount == 4)
                return 9; // September has 9 characters
            if(theCount == 5)
                return 1;

            return 0;
        }


        template<class Clock>
        bool MonthSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount <= 2)
            {
                if(theCount == 2 && v.length() == 1)
                    return false;

                auto theValue = std::atoi(v.cStr().get());
                if(theValue >= 1 && theValue <= 12)
                    return true;
            }
            if(theCount == 3)
            {
                if(v == "Jan")
                    return true;
                if(v == "Feb")
                    return true;
                if(v == "Mar")
                    return true;
                if(v == "Apr")
                    return true;
                if(v == "May")
                    return true;
                if(v == "June")
                    return true;
                if(v == "July")
                    return true;
                if(v == "Aug")
                    return true;
                if(v == "Sept")
                    return true;
                if(v == "Oct")
                    return true;
                if(v == "Nov")
                    return true;
                if(v == "Dec")
                    return true;
            }
            if(theCount == 4)
            {
                if(v == "January")
                    return true;
                if(v == "February")
                    return true;
                if(v == "March")
                    return true;
                if(v == "April")
                    return true;
                if(v == "May")
                    return true;
                if(v == "June")
                    return true;
                if(v == "July")
                    return true;
                if(v == "August")
                    return true;
                if(v == "September")
                    return true;
                if(v == "October")
                    return true;
                if(v == "November")
                    return true;
                if(v == "December")
                    return true;
            }
            if(theCount == 5)
            {
                if(v == "J")
                    return true;
                if(v == "F")
                    return true;
                if(v == "M")
                    return true;
                if(v == "A")
                    return true;
                if(v == "S")
                    return true;
                if(v == "N")
                    return true;
                if(v == "D")
                    return true;
            }

            return false;
        }


        template<class Clock>
        class DayOfMonthSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            DayOfMonthSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "DayOfMonthSymbol"; }

            DayOfMonthSymbol(const DayOfMonthSymbol &s) : DateSymbol<Clock>(s) {}

            ~DayOfMonthSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::DayOfMonth; }
        };


        template<class Clock>
        typename DayOfMonthSymbol<Clock>::string_type DayOfMonthSymbol<Clock>::convert(
                const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                accumulator << c.dayOfMonth();
            }

            return accumulator.str();
        }


        template<class Clock>
        int DayOfMonthSymbol<Clock>::convert(const string_type &s)
        {
            return std::atoi(s.cStr().get());
        }


        template<class Clock>
        int DayOfMonthSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount ==  1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int DayOfMonthSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount ==  1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool DayOfMonthSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());
            auto theCount = DateSymbol<Clock>::count;

            if(theCount == 2)
            {
                if(v.length() == 1)
                    return false;
            }

            if(theValue >= 1 && theValue <= 31)
                return true;
            return false;
        }


        template<class Clock>
        class DayOfYearSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            DayOfYearSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "DayOfYearSymbol"; }

            DayOfYearSymbol(const DayOfYearSymbol &s) : DateSymbol<Clock>(s) {}

            ~DayOfYearSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::DayOfYear; }
        };


        template<class Clock>
        typename DayOfYearSymbol<Clock>::string_type DayOfYearSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 3)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                else if(DateSymbol<Clock>::count == 3)
                {
                    accumulator.width(3);
                    accumulator.fill('0');
                }
                accumulator << c.dayOfYear();
            }
            return accumulator.str();
        }


        template<class Clock>
        int DayOfYearSymbol<Clock>::convert(const string_type &s)
        {
            return std::atoi(s.cStr().get());
        }


        template<class Clock>
        int DayOfYearSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            if(theCount == 3)
                return 3;

            return 0;
        }


        template<class Clock>
        int DayOfYearSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 3)
                return 3;

            return 0;
        }


        template<class Clock>
        bool DayOfYearSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());
            auto theCount = DateSymbol<Clock>::count;

            if(theCount == 2)
            {
                if(v.length() < 2)
                    return false;
            }
            else if(theCount == 3)
            {
                if(v.length() < 3)
                    return false;
            }

            if(theValue >= 1 && theValue <= 365)
                return true;
            if(theValue == 0 && (v == "0"
                || v == "00"
                || v == "000"))
                return true;
            return false;
        }


        template<class Clock>
        class DayOfWeekSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            DayOfWeekSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "DayOfWeekSymbol"; }

            DayOfWeekSymbol(const DayOfWeekSymbol &s) : DateSymbol<Clock>(s) {}

            ~DayOfWeekSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::DayOfWeek; }
        };


        template<class Clock>
        typename DayOfWeekSymbol<Clock>::string_type DayOfWeekSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;

            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 3)
            {
                switch(c.dayOfWeek())
                {
                    case DayOfWeek::Sunday:
                        accumulator << "Sun";
                        break;
                    case DayOfWeek::Monday:
                        accumulator << "Mon";
                        break;
                    case DayOfWeek::Tuesday:
                        accumulator << "Tues";
                        break;
                    case DayOfWeek::Wednesday:
                        accumulator << "Wed";
                        break;
                    case DayOfWeek::Thursday:
                        accumulator << "Thurs";
                        break;
                    case DayOfWeek::Friday:
                        accumulator << "Fri";
                        break;
                    case DayOfWeek::Saturday:
                        accumulator << "Sat";
                        break;
                }
            }
            else if(DateSymbol<Clock>::count == 4)
            {
                switch(c.dayOfWeek())
                {
                    case DayOfWeek::Sunday:
                        accumulator << "Sunday";
                        break;
                    case DayOfWeek::Monday:
                        accumulator << "Monday";
                        break;
                    case DayOfWeek::Tuesday:
                        accumulator << "Tuesday";
                        break;
                    case DayOfWeek::Wednesday:
                        accumulator << "Wednesday";
                        break;
                    case DayOfWeek::Thursday:
                        accumulator << "Thursday";
                        break;
                    case DayOfWeek::Friday:
                        accumulator << "Friday";
                        break;
                    case DayOfWeek::Saturday:
                        accumulator << "Saturday";
                        break;
                }
            }
            else if(DateSymbol<Clock>::count == 5)
            {
                switch(c.dayOfWeek())
                {
                    case DayOfWeek::Sunday:
                        accumulator << "S";
                        break;
                    case DayOfWeek::Monday:
                        accumulator << "M";
                        break;
                    case DayOfWeek::Tuesday:
                        accumulator << "T";
                        break;
                    case DayOfWeek::Wednesday:
                        accumulator << "W";
                        break;
                    case DayOfWeek::Thursday:
                        accumulator << "T";
                        break;
                    case DayOfWeek::Friday:
                        accumulator << "F";
                        break;
                }
            }

            return accumulator.str();
        }


        template<class Clock>
        int DayOfWeekSymbol<Clock>::convert(const string_type &s)
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 3)
            {
                if(s == "Sun")
                    return 0;
                if(s == "Mon")
                    return 1;
                if(s == "Tues")
                    return 2;
                if(s == "Wed")
                    return 3;
                if(s == "Thurs")
                    return 4;
                if(s == "Fri")
                    return 5;
                if(s == "Sat")
                    return 6;
            }
            else if(theCount == 4)
            {
                if(s == "Sunday")
                    return 0;
                if(s == "Monday")
                    return 1;
                if(s == "Tuesday")
                    return 2;
                if(s == "Wednesday")
                    return 3;
                if(s == "Thursday")
                    return 4;
                if(s == "Friday")
                    return 6;
                if(s == "Saturday")
                    return 7;
            }
            else if(theCount == 5)
            {
                if(s == "M")
                    return 1;
                if(s == "W")
                    return 3;
                if(s == "F")
                    return 4;
                throw std::runtime_error("Insufficient information to reconstruct day of week");
            }
            return 0;
        }


        template<class Clock>
        int DayOfWeekSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 3)
                return 3;
            if(theCount == 4)
                return 6;
            if(theCount == 5)
                return 1;
            return 0;
        }


        template<class Clock>
        int DayOfWeekSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 3)
                return 5;
            if(theCount == 4)
                return 9;
            if(theCount == 5)
                return 1;
            return 0;
        }


        template<class Clock>
        bool DayOfWeekSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 3)
            {
                if(v == "Sun")
                    return true;
                if(v == "Mon")
                    return true;
                if(v == "Tues")
                    return true;
                if(v == "Wed")
                    return true;
                if(v == "Thurs")
                    return true;
                if(v == "Fri")
                    return true;
                if(v == "Sat")
                    return true;
            }
            if(theCount == 4)
            {
                if(v == "Sunday")
                    return true;
                if(v == "Monday")
                    return true;
                if(v == "Tuesday")
                    return true;
                if(v == "Wednesday")
                    return true;
                if(v == "Thursday")
                    return true;
                if(v == "Friday")
                    return true;
                if(v == "Saturday")
                    return true;
            }
            if(theCount == 5)
            {
                if(v == "S")
                    return true;
                if(v == "M")
                    return true;
                if(v == "T")
                    return true;
                if(v == "W")
                    return true;
                if(v == "F")
                    return true;
            }
            return false;
        }


        template<class Clock>
        class PeriodSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            PeriodSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "PeriodSymbol"; }

            PeriodSymbol(const PeriodSymbol &s) : DateSymbol<Clock>(s) {}

            ~PeriodSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Period; }
        };


        template<class Clock>
        typename PeriodSymbol<Clock>::string_type PeriodSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 4)
            {
                if(c.hour() < 12)
                    accumulator << "AM";
                else
                    accumulator << "PM";
            }
            else if(DateSymbol<Clock>::count == 5)
            {
                if(c.hour() < 12)
                    accumulator << "A";
                else
                    accumulator << "P";
            }
            return accumulator.str();
        }


        template<class Clock>
        int PeriodSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 4)
                return 2;
            if(theCount == 5)
                return 1;
            return 0;
        }


        template<class Clock>
        int PeriodSymbol<Clock>::maximumCharactersToExpect()
        {
            return minimumCharactersToExpect();
        }


        template<class Clock>
        bool PeriodSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theCount = DateSymbol<Clock>::count;

            if(theCount >= 1 && theCount <= 4)
            {
                if(v == "AM")
                    return true;
                if(v == "PM")
                    return true;
            }
            if(theCount == 5)
            {
                if(v == "A")
                    return true;
                if(v == "P")
                    return true;
            }
            return false;
        }


        template<class Clock>
        class NormalHourSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            NormalHourSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "NormalHourSymbol"; }

            NormalHourSymbol(const NormalHourSymbol &s) : DateSymbol<Clock>(s) {}

            ~NormalHourSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::NormalHour; }
        };


        template<class Clock>
        typename NormalHourSymbol<Clock>::string_type NormalHourSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2);
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }

                // Hour is in range [0,23]
                auto hour = (c.hour() % 12);
                hour = hour == 0 ? 12 : hour;
                accumulator << hour;
            }
            return accumulator.str();
        }


        template<class Clock>
        int NormalHourSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int NormalHourSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool NormalHourSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());

            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }

            if(theValue >= 1 && theValue <= 12)
                return true;
            return false;
        }


        template<class Clock>
        class MilitaryHourSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            MilitaryHourSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "MilitaryHourSymbol"; }

            MilitaryHourSymbol(const MilitaryHourSymbol &s) : DateSymbol<Clock>(s) {}

            ~MilitaryHourSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::MilitaryHour; }
        };


        template<class Clock>
        typename MilitaryHourSymbol<Clock>::string_type MilitaryHourSymbol<Clock>::convert(
                const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                accumulator << c.hour();
            }
            return accumulator.str();
        }


        template<class Clock>
        int MilitaryHourSymbol<Clock>::convert(const string_type &s)
        {
            auto theValue = std::atoi(s.cStr().get());
            return theValue;
        }


        template<class Clock>
        int MilitaryHourSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int MilitaryHourSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool MilitaryHourSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());

            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }

            if(theValue >= 1 && theValue <= 23)
                return true;
            if(theValue == 0 && (v == "0" || v == "00"))
                return true;
            return false;
        }


        template<class Clock>
        class NonstandardHourSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            NonstandardHourSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "NonstandardHourSymbol"; }

            NonstandardHourSymbol(const NonstandardHourSymbol &s) : DateSymbol<Clock>(s) {}

            ~NonstandardHourSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::NonStandardHour; }
        };


        template<class Clock>
        typename NonstandardHourSymbol<Clock>::string_type NonstandardHourSymbol<Clock>::convert(
                const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                accumulator << (c.hour() % 12);
            }
            return accumulator.str();
        }


        template<class Clock>
        int NonstandardHourSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int NonstandardHourSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool NonstandardHourSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());

            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }

            if(theValue >= 1 && theValue <= 11)
                return true;
            if(theValue == 0 && (v == "0" || v == "00"))
                return true;
            return false;
        }


        template<class Clock>
        class NonstandardMilitaryHourSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            NonstandardMilitaryHourSymbol() : DateSymbol<Clock>()
            { DateSymbol<Clock>::className = "NonstandardMilitaryHourSymbol"; }

            NonstandardMilitaryHourSymbol(const NonstandardMilitaryHourSymbol &s) : DateSymbol<Clock>(s) {}

            ~NonstandardMilitaryHourSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::NonStandardMilitaryHour; }
        };


        template<class Clock>
        typename NonstandardMilitaryHourSymbol<Clock>::string_type NonstandardMilitaryHourSymbol<Clock>::convert(
                const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                accumulator << c.hour() + 1;
            }
            return accumulator.str();
        }


        template<class Clock>
        int NonstandardMilitaryHourSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int NonstandardMilitaryHourSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool NonstandardMilitaryHourSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());
            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }
            if(theValue >= 1 && theValue <= 24)
                return true;
            return false;
        }


        template<class Clock>
        class MinuteSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            MinuteSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "MinuteSymbol"; }

            MinuteSymbol(const MinuteSymbol &s) : MinuteSymbol<Clock>(s) {}

            ~MinuteSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Minute; }
        };


        template<class Clock>
        typename MinuteSymbol<Clock>::string_type MinuteSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                accumulator << c.minute();
            }
            return accumulator.str();
        }


        template<class Clock>
        int MinuteSymbol<Clock>::convert(const string_type &s)
        {
            return std::atoi(s.cStr().get());
        }


        template<class Clock>
        int MinuteSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int MinuteSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool MinuteSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());

            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }

            if(theValue >= 1 && theValue <= 59)
                return true;
            if(theValue == 0 && (v == "0" || v == "00"))
                return true;
            return false;
        }


        template<class Clock>
        class SecondSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            SecondSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "SecondSymbol"; }

            SecondSymbol(const SecondSymbol &s) : DateSymbol<Clock>(s) {}

            ~SecondSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Second; }
        };


        template<class Clock>
        typename SecondSymbol<Clock>::string_type SecondSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            if(DateSymbol<Clock>::count >= 1 && DateSymbol<Clock>::count <= 2)
            {
                if(DateSymbol<Clock>::count == 2)
                {
                    accumulator.width(2);
                    accumulator.fill('0');
                }
                accumulator << c.second();
            }
            return accumulator.str();
        }


        template<class Clock>
        int SecondSymbol<Clock>::convert(const string_type &s)
        {
            return std::atoi(s.cStr().get());
        }


        template<class Clock>
        int SecondSymbol<Clock>::minimumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 1;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        int SecondSymbol<Clock>::maximumCharactersToExpect()
        {
            auto theCount = DateSymbol<Clock>::count;
            if(theCount == 1)
                return 2;
            if(theCount == 2)
                return 2;
            return 0;
        }


        template<class Clock>
        bool SecondSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());

            if(DateSymbol<Clock>::count == 2)
            {
                if(v.length() != 2)
                    return false;
            }

            if(theValue >= 1 && theValue <= 60)
                return true;
            if(theValue == 0 && (v == "0" || v == "00"))
                return true;
            return false;
        }


        template<class Clock>
        class FractionalSecondSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            FractionalSecondSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "FractionalSecondSymbol"; }

            FractionalSecondSymbol(const FractionalSecondSymbol &s) : DateSymbol<Clock>(s) {}

            ~FractionalSecondSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;
            int convert(const string_type &s) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::FractionalSecond; }
        };


        template<class Clock>
        typename FractionalSecondSymbol<Clock>::string_type FractionalSecondSymbol<Clock>::convert(
                const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            std::stringstream converter;
            auto fSecs = c.fractionalSeconds();

            converter << fSecs;

            auto length = converter.str().length();

            if(DateSymbol<Clock>::count > length)
                accumulator << fSecs;
            else
            {
                long divisor = 1;

                for(int i = 0; i < (length - DateSymbol<Clock>::count); i++)
                    divisor *= 10;

                double dividedVal = static_cast<double>(fSecs) / divisor;

                auto rounded = std::lround(dividedVal);

                accumulator << rounded;
            }

            return accumulator.str();
        }


        template<class Clock>
        int FractionalSecondSymbol<Clock>::convert(const string_type &s)
        {
            return std::atoi(s.cStr().get());
        }


        template<class Clock>
        int FractionalSecondSymbol<Clock>::minimumCharactersToExpect()
        {
            return 1;
        }


        template<class Clock>
        int FractionalSecondSymbol<Clock>::maximumCharactersToExpect()
        {
            return DateSymbol<Clock>::count;
        }


        template<class Clock>
        bool FractionalSecondSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            auto theValue = std::atoi(v.cStr().get());

            if(v.length() != DateSymbol<Clock>::count)
                return false;

            if(theValue > 0)
                return true;
            if(theValue == 0 && v == "0")
                return true;
            return false;
        }


        template<class Clock>
        class NormalSymbol : public DateSymbol<Clock>
        {
        public:

            using string_type = String;
            using semantic_type = SymbolSemantic;

            NormalSymbol() : DateSymbol<Clock>() { DateSymbol<Clock>::className = "NormalSymbol"; }

            NormalSymbol(const NormalSymbol &s) : DateSymbol<Clock>(s) {}

            ~NormalSymbol() {}

            string_type convert(const DateComponents<Clock> &c) override;

            int minimumCharactersToExpect() override;
            int maximumCharactersToExpect() override;
            bool validValueForSymbol(const string_type &v) override;
            semantic_type semanticValue() const override { return SymbolSemantic::Normal; }
        };


        template<class Clock>
        typename NormalSymbol<Clock>::string_type NormalSymbol<Clock>::convert(const DateComponents<Clock> &c)
        {
            std::stringstream accumulator;
            accumulator << DateSymbol<Clock>::theCharacter;
            return accumulator.str();
        }


        template<class Clock>
        int NormalSymbol<Clock>::minimumCharactersToExpect()
        {
            return 1;
        }


        template<class Clock>
        int NormalSymbol<Clock>::maximumCharactersToExpect()
        {
            return 1;
        }


        template<class Clock>
        bool NormalSymbol<Clock>::validValueForSymbol(const string_type &v)
        {
            if(v.length() == 1)
                return true;
            return false;
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForEra()
        {
            return new EraSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForYear()
        {
            return new YearSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForQuarter()
        {
            return new QuarterSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForMonth()
        {
            return new MonthSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForDayOfMonth()
        {
            return new DayOfMonthSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForDayOfYear()
        {
            return new DayOfYearSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForDayOfWeek()
        {
            return new DayOfWeekSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForPeriod()
        {
            return new PeriodSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForStandardHour()
        {
            return new NormalHourSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForMilitaryHour()
        {
            return new MilitaryHourSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForNonStandardHour()
        {
            return new NonstandardHourSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForNonStandardMilitaryHour()
        {
            return new NonstandardMilitaryHourSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForMinute()
        {
            return new MinuteSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForStandardSecond()
        {
            return new SecondSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForFractionalSecond()
        {
            return new FractionalSecondSymbol<Clock>();
        }


        template<class Clock>
        DateSymbol<Clock> * DateSymbolFactory<Clock>::symbolForNormalCharacter()
        {
            return new NormalSymbol<Clock>();
        }

    } // Foundation

} // TF


