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

#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tfdurationformatter.hpp"

namespace TF::Foundation
{

    struct QueueLoadTracker
    {
        bool processing_millenia{false};
        bool processing_centuries{false};
        bool processing_decades{false};
        bool processing_years{false};
        bool processing_months{false};
        bool processing_weeks{false};
        bool processing_days{false};
        bool processing_hours{false};
        bool processing_minutes{false};
        bool processing_seconds{false};
        bool processing_milliseconds{false};
        bool processing_microseconds{false};
        DurationFormatter::stack_type stack{};
        DurationFormatter::queue_type & queue;

        QueueLoadTracker(DurationFormatter & d) : queue{d.m_queue} {}

        void reset_processing()
        {
            processing_millenia = false;
            processing_centuries = false;
            processing_decades = false;
            processing_years = false;
            processing_months = false;
            processing_weeks = false;
            processing_days = false;
            processing_hours = false;
            processing_minutes = false;
            processing_seconds = false;
            processing_milliseconds = false;
            processing_microseconds = false;
        }

        void put_symbol_in_queue(DurationFormatter::queue_type & q)
        {
            if (stack.size() > 0)
            {
                auto the_symbol = stack.top();
                stack.pop();
                q.push_back(the_symbol);
                reset_processing();
            }
        }

        void increment_count()
        {
            auto the_symbol = stack.top();
            the_symbol->count++;
        }
    };

    static void handle_time(QueueLoadTracker & tracker, bool & processor,
                            std::function<DurationSymbol *()> symbol_maker_function)
    {
        if (processor)
        {
            tracker.increment_count();
        }
        else
        {
            tracker.put_symbol_in_queue(tracker.queue);
            processor = true;
            auto the_symbol = symbol_maker_function();
            tracker.stack.push(the_symbol);
        }
    }

    void DurationFormatter::load_queue_from_format(const string_type & format)
    {
        QueueLoadTracker tracker{*this};

        auto tmp = format.cStr();
        const char * the_format = tmp.get();

        while (*the_format != '\0')
        {
            switch (*the_format)
            {
                case 'G': // Millenia
                    handle_time(tracker, tracker.processing_millenia, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_millenia();
                    });
                    break;
                case 'C': // Centuries
                    handle_time(tracker, tracker.processing_centuries, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_century();
                    });
                    break;
                case 'D': // Decades
                    handle_time(tracker, tracker.processing_decades, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_decade();
                    });
                    break;
                case 'Y': // Years
                    handle_time(tracker, tracker.processing_years, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_year();
                    });
                    break;
                case 'M': // Months
                    handle_time(tracker, tracker.processing_months, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_month();
                    });
                    break;
                case 'W': // Weeks
                    handle_time(tracker, tracker.processing_weeks, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_week();
                    });
                    break;
                case 'd': // days
                    handle_time(tracker, tracker.processing_days, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_day();
                    });
                    break;
                case 'h': // hours
                    handle_time(tracker, tracker.processing_hours, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_hour();
                    });
                    break;
                case 'm': // minutes
                    handle_time(tracker, tracker.processing_minutes, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_minute();
                    });
                    break;
                case 's': // seconds
                    handle_time(tracker, tracker.processing_seconds, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_second();
                    });
                    break;
                case 'S': // milliseconds
                    handle_time(tracker, tracker.processing_milliseconds, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_milliseconds();
                    });
                    break;
                case 'U': // microseconds
                    handle_time(tracker, tracker.processing_microseconds, []() -> DurationSymbol * {
                        return symbol_factory::symbol_for_microsecond();
                    });
                    break;
                default:
                    {
                        bool processing_val{false};
                        string_type char_string{static_cast<unsigned int>(*the_format)};
                        handle_time(tracker, processing_val, [char_string]() -> DurationSymbol * {
                            return symbol_factory::symbol_for_normal(char_string);
                        });
                    }
                    break;
            }
            the_format++;
        }

        // Check here for anything left on the stack.
        if (tracker.stack.size() > 0)
        {
            auto the_symbol = tracker.stack.top();
            m_queue.push_back(the_symbol);
        }
    }

    DurationFormatter::DurationFormatter(const string_type & format) : m_format_string{format}
    {
        load_queue_from_format(format);
    }

    DurationFormatter::~DurationFormatter()
    {
        while (m_queue.size() > 0)
        {
            auto first_symbol = m_queue.front();
            m_queue.pop_front();
            delete first_symbol;
        }
    }

    void DurationFormatter::update_components_from_symbol_and_value(DurationComponents & c, symbol * s,
                                                                    const string_type & value)
    {
        if (s == nullptr)
        {
            return;
        }

        auto converted_value = s->convert(value);

        switch (s->semantic_value())
        {
            case SymbolSemanticMeaning::Millenia:
                c.set_millenia(converted_value);
                break;
            case SymbolSemanticMeaning::Century:
                c.set_centuries(converted_value);
                break;
            case SymbolSemanticMeaning::Decade:
                c.set_decades(converted_value);
                break;
            case SymbolSemanticMeaning::Year:
                c.set_years(converted_value);
                break;
            case SymbolSemanticMeaning::Month:
                c.set_months(converted_value);
                break;
            case SymbolSemanticMeaning::Week:
                c.set_weeks(converted_value);
                break;
            case SymbolSemanticMeaning::Day:
                c.set_days(converted_value);
                break;
            case SymbolSemanticMeaning::Hour:
                c.set_hours(converted_value);
                break;
            case SymbolSemanticMeaning::Minute:
                c.set_minutes(converted_value);
                break;
            case SymbolSemanticMeaning::Second:
                c.set_seconds(converted_value);
                break;
            case SymbolSemanticMeaning::Millisecond:
                c.set_milliseconds(converted_value);
                break;
            case SymbolSemanticMeaning::Microsecond:
                c.set_microseconds(converted_value);
                break;
            default:
                break;
        }
    }

    std::ostream & DurationFormatter::description(std::ostream & o) const
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("DurationFormatter");
            formatter->addClassMember<string_type>("m_format_string", m_format_string);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    std::ostream & operator<<(std::ostream & o, const DurationFormatter & d)
    {
        return d.description(o);
    }

} // namespace TF::Foundation
