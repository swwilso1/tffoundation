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
#include "tfdurationsymbol.hpp"
#include "tfconversion.hpp"

namespace TF::Foundation
{
    static bool characters_are_all_digits(const DurationSymbol::string_type & s)
    {
        for (auto c : s)
        {
            if (! (c >= '0' && c <= '9'))
            {
                return false;
            }
        }
        return true;
    }

    int32_t DurationSymbol::max_characters_to_expect() const
    {
        return 0;
    }

    DurationSymbol::semantic_type DurationSymbol::semantic_value() const
    {
        return semantic_type::Normal;
    }

    DurationSymbol::string_type DurationSymbol::convert(const DurationComponents & c) const
    {
        (void)c;
        return {};
    }

    int32_t DurationSymbol::convert(const string_type & s) const
    {
        auto cresult = Conversion::convertStringToInt(s);
        if (cresult)
        {
            return static_cast<int32_t>(cresult.value());
        }
        return {};
    }

    bool DurationSymbol::valid_value_for_symbol(const string_type & s) const
    {
        if (! characters_are_all_digits(s))
        {
            return false;
        }

        return s.length() == static_cast<string_type::size_type>(count);
    }

    DurationSymbol::string_type DurationSymbol::class_name() const
    {
        return "DurationSymbol";
    }

    std::ostream & DurationSymbol::description(std::ostream & o) const
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName(class_name().stlString());
            formatter->addClassMember<int>("count", count);
            formatter->addClassMember<string_type>("value", value);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    std::ostream & operator<<(std::ostream & o, const DurationSymbol & s)
    {
        return s.description(o);
    }

    class SpecializedDurationSymbol : public DurationSymbol
    {
    public:
        using generic_int_result_function = std::function<int32_t()>;
        using semantic_value_function = std::function<DurationSymbol::semantic_type()>;
        using get_component_function = std::function<int32_t(const DurationComponents & c)>;
        using class_name_function = std::function<DurationSymbol::string_type()>;

        SpecializedDurationSymbol(generic_int_result_function max_chars_function,
                                  semantic_value_function semantic_function,
                                  get_component_function get_components_function,
                                  class_name_function get_class_name_function) :
            m_max_chars_function{max_chars_function},
            m_semantic_function{semantic_function}, m_get_component_function{get_components_function},
            m_get_class_name_function{get_class_name_function}
        {}

        int32_t max_characters_to_expect() const override
        {
            return m_max_chars_function();
        }

        semantic_type semantic_value() const override
        {
            return m_semantic_function();
        }

        string_type convert(const DurationComponents & c) const override
        {
            std::stringstream accumulator;
            switch (count)
            {
                case 1:
                    accumulator << m_get_component_function(c);
                    break;
                    //                case 2:
                    //                    {
                    //                        auto mod = m_get_component_function(c) % 100;
                    //                        accumulator.width(2);
                    //                        accumulator.fill('0');
                    //                        accumulator << mod;
                    //                    }
                    //                    break;
                default:
                    {
                        auto component_value = m_get_component_function(c);
                        int32_t order = 0;
                        auto tmp = component_value;
                        while (tmp > 0)
                        {
                            order++;
                            tmp /= 10;
                        }

                        auto reported_value = component_value;
                        if (order > count)
                        {

                            int32_t divisor = 1;
                            for (int32_t i = 0; i < order - count; i++)
                            {
                                divisor *= 10;
                            }
                            reported_value = component_value < divisor ? component_value : component_value / divisor;
                        }

                        accumulator.width(count);
                        accumulator.fill('0');
                        accumulator << reported_value;
                    }
            }

            return accumulator.str();
        }

        string_type class_name() const override
        {
            return m_get_class_name_function();
        }

    private:
        generic_int_result_function m_max_chars_function{};
        semantic_value_function m_semantic_function{};
        get_component_function m_get_component_function{};
        class_name_function m_get_class_name_function{};
    };

    DurationSymbol * DurationSymbolFactory::symbol_for_millenia()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 50;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Millenia;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_millenia());
            },
            []() -> DurationSymbol::string_type {
                return "MilleniaClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_century()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 2;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Century;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_centuries());
            },
            []() -> DurationSymbol::string_type {
                return "CenturiesClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_decade()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 1;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Decade;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_decades());
            },
            []() -> DurationSymbol::string_type {
                return "DecadesClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_year()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 1;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Year;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_years());
            },
            []() -> DurationSymbol::string_type {
                return "YearsClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_month()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 2;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Month;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_months());
            },
            []() -> DurationSymbol::string_type {
                return "MonthsClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_week()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 1;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Week;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_weeks());
            },
            []() -> DurationSymbol::string_type {
                return "WeeksClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_day()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 1;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Day;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_days());
            },
            []() -> DurationSymbol::string_type {
                return "DaysClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_hour()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 2;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Hour;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_hours());
            },
            []() -> DurationSymbol::string_type {
                return "HoursClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_minute()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 2;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Minute;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_minutes());
            },
            []() -> DurationSymbol::string_type {
                return "MinutesClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_second()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 2;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Second;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_seconds());
            },
            []() -> DurationSymbol::string_type {
                return "SecondsClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_milliseconds()
    {
        return new SpecializedDurationSymbol(
            []() -> int32_t {
                return 3;
            },
            []() -> DurationSymbol::semantic_type {
                return DurationSymbol::semantic_type::Millisecond;
            },
            [](const DurationComponents & c) -> int32_t {
                return static_cast<int32_t>(c.get_milliseconds());
            },
            []() -> DurationSymbol::string_type {
                return "MillisecondsClass";
            });
    }

    DurationSymbol * DurationSymbolFactory::symbol_for_microsecond()
    {
        return new SpecializedDurationSymbol{[]() -> int32_t {
                                                 return 10;
                                             },
                                             []() -> DurationSymbol::semantic_type {
                                                 return DurationSymbol::semantic_type::Microsecond;
                                             },
                                             [](const DurationComponents & c) -> int32_t {
                                                 return static_cast<int32_t>(c.get_microseconds());
                                             },
                                             []() -> DurationSymbol::string_type {
                                                 return "MicrosecondsClass";
                                             }};
    }

    class NormalSymbol : public DurationSymbol
    {
    public:
        NormalSymbol(const DurationSymbol::string_type & s)
        {
            value = s;
        }

        int32_t max_characters_to_expect() const override
        {
            return 1;
        }

        semantic_type semantic_value() const override
        {
            return DurationSymbol::semantic_type::Normal;
        }

        string_type convert(const DurationComponents & c) const override
        {
            (void)c;
            return value;
        }

        int32_t convert(const string_type & s) const override
        {
            return static_cast<int32_t>(s[0]);
        }

        bool valid_value_for_symbol(const string_type & s) const override
        {
            return s == value;
        }

        string_type class_name() const override
        {
            return "NormalClass";
        }
    };

    DurationSymbol * DurationSymbolFactory::symbol_for_normal(const DurationSymbol::string_type & s)
    {
        return new NormalSymbol(s);
    }

} // namespace TF::Foundation
