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
#include "tfdatesymbol.hxx"
#include "tfdatetypes.hpp"

namespace TF
{

    namespace Foundation
    {

#define RESET_PROCESSING()                         \
    {                                              \
        processingEra = false;                     \
        processingYear = false;                    \
        processingQuarter = false;                 \
        processingMonth = false;                   \
        processingMonth = false;                   \
        processingDayOfMonth = false;              \
        processingDayOfYear = false;               \
        processingDayOfWeek = false;               \
        processingPeriod = false;                  \
        processingStandardHour = false;            \
        processingMilitaryHour = false;            \
        processingNonstandardHour = false;         \
        processingNonstandardMilitaryHour = false; \
        processingMinute = false;                  \
        processingStandardSecond = false;          \
        processingFractionalSecond = false;        \
    }

#define PUT_SYMBOL_IN_QUEUE()       \
    if (theStack.size() > 0)        \
    {                               \
        theSymbol = theStack.top(); \
        theStack.pop();             \
        queue.push_back(theSymbol); \
        RESET_PROCESSING();         \
    }

#define INCREMENT_COUNT()       \
    theSymbol = theStack.top(); \
    theSymbol->incrementCount()

#define PUSH_NEW_SYMBOL(processor, method)     \
    processor = true;                          \
    theSymbol = symbol_factory_type::method(); \
    theStack.push(theSymbol)

#define MANAGE_STATE(processor, method)     \
    if (processor)                          \
    {                                       \
        INCREMENT_COUNT();                  \
    }                                       \
    else                                    \
    {                                       \
        PUT_SYMBOL_IN_QUEUE();              \
        PUSH_NEW_SYMBOL(processor, method); \
    }

        template<class Clock>
        void DateFormatter<Clock>::processDateFormat(const string_type & f)
        {
            bool processingEra = false;
            bool processingYear = false;
            bool processingQuarter = false;
            bool processingMonth = false;
            bool processingDayOfMonth = false;
            bool processingDayOfYear = false;
            bool processingDayOfWeek = false;
            bool processingPeriod = false;
            bool processingStandardHour = false;
            bool processingMilitaryHour = false;
            bool processingNonstandardHour = false;
            bool processingNonstandardMilitaryHour = false;
            bool processingMinute = false;
            bool processingStandardSecond = false;
            bool processingFractionalSecond = false;

            stack_type theStack;
            symbol * theSymbol = nullptr;

            auto tmp = f.cStr();
            const char * theFormat = tmp.get();

            while (*theFormat != '\0')
            {
                switch (*theFormat)
                {
                    case 'G':
                        MANAGE_STATE(processingEra, symbolForEra);
                        break;
                    case 'y':
                        MANAGE_STATE(processingYear, symbolForYear);
                        break;
                    case 'Q':
                        MANAGE_STATE(processingQuarter, symbolForQuarter);
                        break;
                    case 'M':
                        MANAGE_STATE(processingMonth, symbolForMonth);
                        break;
                    case 'd':
                        MANAGE_STATE(processingDayOfMonth, symbolForDayOfMonth);
                        break;
                    case 'D':
                        MANAGE_STATE(processingDayOfYear, symbolForDayOfYear);
                        break;
                    case 'E':
                        MANAGE_STATE(processingDayOfWeek, symbolForDayOfWeek);
                        break;
                    case 'a':
                        MANAGE_STATE(processingPeriod, symbolForPeriod);
                        break;
                    case 'h':
                        MANAGE_STATE(processingStandardHour, symbolForStandardHour);
                        break;
                    case 'H':
                        MANAGE_STATE(processingMilitaryHour, symbolForMilitaryHour);
                        break;
                    case 'K':
                        MANAGE_STATE(processingNonstandardHour, symbolForNonStandardHour);
                        break;
                    case 'k':
                        MANAGE_STATE(processingNonstandardMilitaryHour, symbolForNonStandardMilitaryHour);
                        break;
                    case 'm':
                        MANAGE_STATE(processingMinute, symbolForMinute);
                        break;
                    case 's':
                        MANAGE_STATE(processingStandardSecond, symbolForStandardSecond);
                        break;
                    case 'S':
                        MANAGE_STATE(processingFractionalSecond, symbolForFractionalSecond);
                        break;
                    default:
                        PUT_SYMBOL_IN_QUEUE();
                        theSymbol = symbol_factory_type::symbolForNormalCharacter();
                        theSymbol->setCharacter(*theFormat);
                        queue.push_back(theSymbol);
                        break;
                }

                theFormat++;
            }

            if (theStack.size() > 0)
                PUT_SYMBOL_IN_QUEUE();
        }

        template<class Clock>
        DateFormatter<Clock>::DateFormatter()
        {
            formatString = "MM-dd-yyyy hh:mm:ss a";
            processDateFormat(formatString);
        }

        template<class Clock>
        DateFormatter<Clock>::DateFormatter(const DateFormatter & f)
        {
            formatString = f.formatString;
            processDateFormat(formatString);
        }

        template<class Clock>
        DateFormatter<Clock>::DateFormatter(const string_type & f)
        {
            formatString = f;
            processDateFormat(formatString);
        }

        template<class Clock>
        DateFormatter<Clock>::~DateFormatter()
        {
            while (queue.size() > 0)
            {
                auto theSymbol = queue.front();
                queue.pop_front();
                delete theSymbol;
            }
        }

        template<class Clock>
        void DateFormatter<Clock>::addValueToComponentsForSymbol(const string_type & value,
                                                                 DateComponents<Clock> & components,
                                                                 DateSymbol<Clock> * symbol)
        {
            if (symbol == nullptr)
                return;

            switch (symbol->semanticValue())
            {
                case SymbolSemantic::Year:
                    {
                        auto theValue = symbol->convert(value);
                        components.setYear(theValue);
                    }
                    break;
                case SymbolSemantic::Month:
                    {
                        auto theValue = symbol->convert(value);
                        MonthOfYear theMonth;
                        switch (theValue)
                        {
                            case 1:
                                theMonth = MonthOfYear::January;
                                break;
                            case 2:
                                theMonth = MonthOfYear::February;
                                break;
                            case 3:
                                theMonth = MonthOfYear::March;
                                break;
                            case 4:
                                theMonth = MonthOfYear::April;
                                break;
                            case 5:
                                theMonth = MonthOfYear::May;
                                break;
                            case 6:
                                theMonth = MonthOfYear::June;
                                break;
                            case 7:
                                theMonth = MonthOfYear::July;
                                break;
                            case 8:
                                theMonth = MonthOfYear::August;
                                break;
                            case 9:
                                theMonth = MonthOfYear::September;
                                break;
                            case 10:
                                theMonth = MonthOfYear::October;
                                break;
                            case 11:
                                theMonth = MonthOfYear::November;
                                break;
                            case 12:
                                theMonth = MonthOfYear::December;
                                break;
                        }
                        components.setMonthOfYear(theMonth);
                    }
                    break;
                case SymbolSemantic::DayOfMonth:
                    {
                        auto theValue = symbol->convert(value);
                        components.setDayOfMonth(theValue);
                    }
                    break;
                case SymbolSemantic::MilitaryHour:
                    {
                        auto theValue = symbol->convert(value);
                        components.setHour(theValue);
                    }
                    break;
                case SymbolSemantic::Minute:
                    {
                        auto theValue = symbol->convert(value);
                        components.setMinute(theValue);
                    }
                    break;
                case SymbolSemantic::Second:
                    {
                        auto theValue = symbol->convert(value);
                        components.setSecond(theValue);
                    }
                    break;
                case SymbolSemantic::FractionalSecond:
                    {
                        auto theValue = symbol->convert(value);
                        components.setFractionalSecond(static_cast<size_type>(theValue));
                    }
                default:
                    break;
            }
        }

        template<class Clock>
        typename DateFormatter<Clock>::date DateFormatter<Clock>::dateFromString(const string_type & s)
        {
            bool hasYear = false;
            bool hasMonth = false;
            bool hasDay = false;
            bool hasHour = false;
            bool hasMinute = false;
            bool hasSecond = false;

            for (auto symbol : queue)
            {
                switch (symbol->semanticValue())
                {
                    case SymbolSemantic::Year:
                        hasYear = true;
                        break;
                    case SymbolSemantic::Month:
                        if (symbol->getCount() == 2)
                            hasMonth = true;
                        break;
                    case SymbolSemantic::DayOfMonth:
                        if (symbol->getCount() == 2)
                            hasDay = true;
                        break;
                    case SymbolSemantic::MilitaryHour:
                        if (symbol->getCount() == 2)
                            hasHour = true;
                        break;
                    case SymbolSemantic::Minute:
                        if (symbol->getCount() == 2)
                            hasMinute = true;
                        break;
                    case SymbolSemantic::Second:
                        if (symbol->getCount() == 2)
                            hasSecond = true;
                        break;
                    default:
                        break;
                }
            }

            if (! (hasYear && hasMonth && hasDay && hasHour && hasMinute && hasSecond))
                throw std::runtime_error("date format does not have enough information to reconstruct a date");

            DateComponents<Clock> components;
            auto cPtr = s.cStr();
            auto tmp = cPtr.get();

            for (auto symbol : queue)
            {
                std::stringstream accumulator;

                for (int i = 0; i < symbol->maximumCharactersToExpect(); i++)
                {
                    if (*tmp == '\0')
                    {
                        string_type str = accumulator.str();
                        if (symbol->validValueForSymbol(str))
                            addValueToComponentsForSymbol(str, components, symbol);
                        break;
                    }

                    accumulator << *tmp++;

                    string_type str = accumulator.str();

                    if (symbol->validValueForSymbol(str))
                    {
                        addValueToComponentsForSymbol(str, components, symbol);
                        break;
                    }
                }
            }

            return components.getDate();
        }

        template<class Clock>
        typename DateFormatter<Clock>::string_type DateFormatter<Clock>::stringFromDate(const date & d)
        {
            std::stringstream accumulator;
            DateComponents<Clock> c(d);

            for (auto symbol : queue)
            {
                accumulator << symbol->convert(c);
            }

            return string_type(accumulator.str());
        }

        template<class Clock>
        std::ostream & DateFormatter<Clock>::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("DateFormatter");
                o << *formatter;
                delete formatter;
            }
            return o;
        }

    } // namespace Foundation

} // namespace TF
