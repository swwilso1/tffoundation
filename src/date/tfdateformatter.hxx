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

#ifndef TFDATEFORMATTER_HXX
#define TFDATEFORMATTER_HXX

#define NEEDS_OSTREAM
#define NEEDS_DEQUE
#define NEEDS_STACK
#define NEEDS_SSTREAM
#define NEEDS_CSTDLIB
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdatesymbol.hxx"
#include "tfdate.hxx"
#include "tfstring.hpp"


namespace TF
{

    namespace Foundation
    {

        template<class Clock>
        class DateFormatter : public AllocatorInterface
        {
        public:
            using symbol = DateSymbol<Clock>;

            using date = Date<Clock>;

            using string_type = String;

            DateFormatter();

            DateFormatter(const DateFormatter &f);

            DateFormatter(const string_type &f);

            ~DateFormatter();

            date dateFromString(const string_type &s);

            string_type stringFromDate(const date &d);

            std::ostream &description(std::ostream &o) const;

        private:
            using queue_type = std::deque<DateSymbol<Clock> *>;

            using stack_type = std::stack<DateSymbol<Clock> *, queue_type>;

            using symbol_factory_type = DateSymbolFactory<Clock>;

            void processDateFormat(const string_type &f);

            void addValueToComponentsForSymbol(const string_type &value, DateComponents<Clock> &components,
                                               DateSymbol<Clock> *symbol);

            queue_type queue;

            string_type formatString;
        };

    }    // namespace Foundation

}    // namespace TF


#include "tfdateformatter.cxx"

#endif    // TFDATEFORMATTER_HXX
