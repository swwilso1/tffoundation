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

#ifndef TFDATESYMBOL_HXX
#define TFDATESYMBOL_HXX

#define NEEDS_OSTREAM
#define NEEDS_SSTREAM
#define NEEDS_CMATH
#define NEEDS_CSTDLIB
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdatecomponent.hxx"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        enum class SymbolSemantic
        {
            Era,
            Year,
            Quarter,
            Month,
            DayOfMonth,
            DayOfYear,
            DayOfWeek,
            Period,
            NormalHour,
            MilitaryHour,
            NonStandardHour,
            NonStandardMilitaryHour,
            Minute,
            Second,
            FractionalSecond,
            Normal
        };

        /**
         * The DateFormatter class uses DateSymbol objects to represent the configuration of the date
         * format.
         * @tparam Clock
         */
        template<class Clock>
        class DateSymbol : public AllocatorInterface
        {
        public:
            using string_type = String;

            using semantic_type = SymbolSemantic;

            virtual ~DateSymbol() {}

            DateSymbol & operator=(const DateSymbol & s);

            void incrementCount()
            {
                count++;
            }
            void decrementCount()
            {
                count--;
            }

            int getCount()
            {
                return count;
            }

            virtual int minimumCharactersToExpect()
            {
                return 0;
            }
            virtual int maximumCharactersToExpect()
            {
                return 0;
            }
            virtual bool validValueForSymbol(const string_type & v)
            {
                (void)v;
                return false;
            }
            virtual semantic_type semanticValue() const
            {
                return SymbolSemantic::Normal;
            }

            void setCharacter(char c)
            {
                theCharacter = c;
            }

            virtual string_type convert(const DateComponents<Clock> & c);
            virtual int convert(const string_type & s);
            virtual std::ostream & description(std::ostream & o) const;

        protected:
            DateSymbol()
            {
                count = 1;
                className = "DateSymbol";
            }

            DateSymbol(const DateSymbol & s);

            int count;

            char theCharacter;

            string_type className;
        };

        template<class Clock>
        std::ostream & operator<<(std::ostream & o, const DateSymbol<Clock> & s);

        template<class Clock>
        class DateSymbolFactory : public AllocatorInterface
        {
        public:
            using clock = Clock;

            static DateSymbol<Clock> * symbolForEra();
            static DateSymbol<Clock> * symbolForYear();
            static DateSymbol<Clock> * symbolForQuarter();
            static DateSymbol<Clock> * symbolForMonth();
            static DateSymbol<Clock> * symbolForDayOfMonth();
            static DateSymbol<Clock> * symbolForDayOfYear();
            static DateSymbol<Clock> * symbolForDayOfWeek();
            static DateSymbol<Clock> * symbolForPeriod();
            static DateSymbol<Clock> * symbolForStandardHour();
            static DateSymbol<Clock> * symbolForMilitaryHour();
            static DateSymbol<Clock> * symbolForNonStandardHour();
            static DateSymbol<Clock> * symbolForNonStandardMilitaryHour();
            static DateSymbol<Clock> * symbolForMinute();
            static DateSymbol<Clock> * symbolForStandardSecond();
            static DateSymbol<Clock> * symbolForFractionalSecond();
            static DateSymbol<Clock> * symbolForNormalCharacter();
        };

    } // namespace Foundation

} // namespace TF

#include "tfdatesymbol.cxx"

#endif // TFDATESYMBOL_HXX
