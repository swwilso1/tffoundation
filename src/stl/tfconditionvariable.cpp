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

#include "tfconditionvariable.hpp"
#include "tftypes.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

#if 0
        std::ostream &ConditionVariable::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("ConditionVariable");
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const ConditionVariable &v)
        {
            return v.description(o);
        }
#endif

        std::ostream & operator<<(std::ostream & o, const std::condition_variable & c)
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("std::condition_variable");
                o << *formatter;
                delete formatter;
            }
            return o;
        }
    } // namespace Foundation

} // namespace TF
