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

#ifndef TFDURATIONSYMBOL_HPP
#define TFDURATIONSYMBOL_HPP

#define NEEDS_OSTREAM
#define NEEDS_SSTREAM
#define NEEDS_CMATH
#define NEEDS_CSTDLIB
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdurationcomponent.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    enum class SymbolSemanticMeaning
    {
        Millenia,
        Century,
        Decade,
        Year,
        Month,
        Week,
        Day,
        Hour,
        Minute,
        Second,
        Millisecond,
        Microsecond,
        Normal
    };

    class DurationSymbol : public AllocatorInterface
    {
    public:
        using string_type = String;

        using semantic_type = SymbolSemanticMeaning;

        int32_t count{1};

        string_type value{};

        virtual ~DurationSymbol() {}

        virtual int32_t max_characters_to_expect() const;

        virtual semantic_type semantic_value() const;

        virtual string_type convert(const DurationComponents & c) const;

        virtual int32_t convert(const string_type & s) const;

        virtual bool valid_value_for_symbol(const string_type & s) const;

        virtual string_type class_name() const;

        virtual std::ostream & description(std::ostream & o) const;
    };

    std::ostream & operator<<(std::ostream & o, const DurationSymbol & s);

    class DurationSymbolFactory : public AllocatorInterface
    {
    public:
        static DurationSymbol * symbol_for_millenia();
        static DurationSymbol * symbol_for_century();
        static DurationSymbol * symbol_for_decade();
        static DurationSymbol * symbol_for_year();
        static DurationSymbol * symbol_for_month();
        static DurationSymbol * symbol_for_week();
        static DurationSymbol * symbol_for_day();
        static DurationSymbol * symbol_for_hour();
        static DurationSymbol * symbol_for_minute();
        static DurationSymbol * symbol_for_second();
        static DurationSymbol * symbol_for_milliseconds();
        static DurationSymbol * symbol_for_microsecond();
        static DurationSymbol * symbol_for_normal(const DurationSymbol::string_type & s);
    };

} // namespace TF::Foundation

#endif // TFDURATIONSYMBOL_HPP
