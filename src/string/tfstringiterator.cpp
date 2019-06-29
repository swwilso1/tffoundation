/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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
            theEncoder = nullptr;
            currentIndex = 0;
        }


        StringIterator::StringIterator(const StringIterator &i)
        {
            core = i.core;
            theEncoder = i.theEncoder;
            currentIndex = i.currentIndex;
        }


        StringIterator::StringIterator(StringIterator &&i) noexcept
        {
            core = i.core;
            theEncoder = i.theEncoder;
            currentIndex = i.currentIndex;
        }


        StringIterator::StringIterator(encoder_pointer_type  e, const core_pointer_type &c, size_type index)
        {
            core = c;
            theEncoder = e;
            currentIndex = index;
        }


        StringIterator &StringIterator::operator=(const StringIterator &i)
        {
            if(this != &i)
            {
                core = i.core;
                theEncoder = i.theEncoder;
                currentIndex = i.currentIndex;
            }
            return *this;
        }


        StringIterator &StringIterator::operator=(StringIterator &&i) noexcept
        {
            if(this != &i)
            {
                core = i.core;
                theEncoder = i.theEncoder;
                currentIndex = i.currentIndex;
            }
            return *this;
        }


        StringIterator& StringIterator::operator++()
        {
            auto nextCodePoint = getNextCodePoint();
            currentIndex += nextCodePoint.second;
            return *this;
        }


        StringIterator StringIterator::operator++(int)
        {
            StringIterator newIterator(theEncoder, core, currentIndex);
            auto nextCodePoint = getNextCodePoint();
            currentIndex += nextCodePoint.second;
            return newIterator;
        }


        std::pair<StringIterator::unicode_point_type, StringIterator::size_type> StringIterator::getNextCodePoint()
        {
            auto theEndian = theEncoder->thisSystemEndianness();

            if(currentIndex == core->length())
                throw std::runtime_error("End of iteration in getNextCodePoint");

            return theEncoder->nextCodePoint(core->data() + currentIndex, core->length()
                    - currentIndex, theEndian);
        }


        StringIterator::unicode_point_type StringIterator::operator*()
        {
            auto nextCodePoint = getNextCodePoint();
            return nextCodePoint.first;
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
                formatter->addClassMember("encoder","theEncoder",theEncoder);
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

