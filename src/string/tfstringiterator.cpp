/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2016 Tectiform Inc.

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

#include "tfstringiterator.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        StringIterator::StringIterator()
        {
            currentIndex = 0;
        }


        StringIterator::StringIterator(const StringIterator &i)
        {
            core = i.core;
            currentIndex = i.currentIndex;
        }


        StringIterator::StringIterator(StringIterator &&i) noexcept
        {
            core = i.core;
            currentIndex = i.currentIndex;
        }


        StringIterator::StringIterator(const core_pointer_type &c, size_type index)
        {
            core = c;
            currentIndex = index;
        }


        StringIterator &StringIterator::operator=(const StringIterator &i)
        {
            core = i.core;
            currentIndex = i.currentIndex;
        }


        StringIterator &StringIterator::operator=(StringIterator &&i) noexcept
        {
            core = i.core;
            currentIndex = i.currentIndex;
        }


        StringIterator& StringIterator::operator++()
        {
            currentIndex++;
            return *this;
        }


        StringIterator StringIterator::operator++(int)
        {
            StringIterator newIterator(core, currentIndex);
            currentIndex++;
            return newIterator;
        }


        StringIterator& StringIterator::operator--()
        {
            if(currentIndex > 0)
                currentIndex--;
            return *this;
        }


        StringIterator StringIterator::operator--(int)
        {
            StringIterator newIterator(core, currentIndex);
            if(currentIndex > 0)
                currentIndex--;
            return newIterator;
        }


        StringIterator::unicode_point_type StringIterator::operator*()
        {
            return (*core)[currentIndex];
        }


        bool StringIterator::operator==(const StringIterator &i) const
        {
            if(*core != *i.core)
                return false;

            return currentIndex == i.currentIndex;
        }


        bool StringIterator::operator!=(const StringIterator &i) const
        {
            if(*this == i)
                return false;
            return true;
        }


        std::ostream& StringIterator::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("StringIterator");
                formatter->addClassMember("core_type","core",*core);
                formatter->addClassMember<size_type>("currentIndex",currentIndex);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream& operator<<(std::ostream &o, const StringIterator &i)
        {
            return i.description(o);
        }

    } // Foundation

} // TF

