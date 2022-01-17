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

#include "tftypes.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        template<class T, class Container>
        std::ostream &operator<<(std::ostream &o, const std::queue<T, Container> &q)
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                std::queue<T, Container> &modSelf = const_cast<std::queue<T, Container> &>(q);

                formatter->setClassName("Queue");
                Size_t theSize = q.size();
                for(Size_t i = 0; i < theSize; i++)
                {
                    auto theValue = modSelf.front();
                    formatter->addClassMember<T>(i, theValue);
                    modSelf.pop();
                    modSelf.push(theValue);
                }
                o << *formatter;
                delete formatter;
            }
            return o;
        }

    }    // namespace Foundation

}    // namespace TF
