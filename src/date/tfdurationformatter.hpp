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

#ifndef TFDURATIONFORMATTER_HPP
#define TFDURATIONFORMATTER_HPP

#define NEEDS_CHRONO
#define NEEDS_OSTREAM
#define NEEDS_DEQUE
#define NEEDS_STACK
#define NEEDS_SSTREAM
#define NEEDS_CSTDLIB
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfdurationsymbol.hpp"

namespace TF::Foundation
{

    class DurationFormatter : public AllocatorInterface
    {
    public:
        using symbol = DurationSymbol;

        using string_type = String;

        DurationFormatter() = default;

        DurationFormatter(const string_type & format);

        ~DurationFormatter();

        template<typename Rep, typename Period>
        std::chrono::duration<Rep, Period> duration_from_string(const string_type & s)
        {
            DurationComponents c;

            auto cPtr = s.cStr();
            auto tmp = cPtr.get();

            for (auto symbol : m_queue)
            {
                std::stringstream accumulator;

                for (int i = 0; i < symbol->max_characters_to_expect(); i++)
                {
                    if (*tmp == '\0')
                    {
                        string_type symbol_value = accumulator.str();
                        if (symbol->valid_value_for_symbol(symbol_value))
                        {
                            update_components_from_symbol_and_value(c, symbol, symbol_value);
                            break;
                        }
                    }

                    accumulator << *tmp++;

                    auto symbol_value = accumulator.str();
                    if (symbol->valid_value_for_symbol(symbol_value))
                    {
                        update_components_from_symbol_and_value(c, symbol, symbol_value);
                        break;
                    }
                }
            }

            return c.get_as_duration<Rep, Period>();
        }

        template<typename Rep, typename Period>
        string_type string_from_duration(const std::chrono::duration<Rep, Period> & d)
        {
            std::stringstream accumulator;
            DurationComponents c(d);

            for (auto the_symbol : m_queue)
            {
                accumulator << the_symbol->convert(c);
            }

            return string_type(accumulator.str());
        }

        std::ostream & description(std::ostream & o) const;

    private:
        friend struct QueueLoadTracker;

        using queue_type = std::deque<symbol *>;

        using stack_type = std::stack<symbol *, queue_type>;

        using symbol_factory = DurationSymbolFactory;

        queue_type m_queue{};

        string_type m_format_string{};

        void load_queue_from_format(const string_type & format);

        void update_components_from_symbol_and_value(DurationComponents & c, symbol * s, const string_type & value);
    };

    std::ostream & operator<<(std::ostream & o, const DurationFormatter & d);

} // namespace TF::Foundation

#endif // TFDURATIONFORMATTER_HPP
