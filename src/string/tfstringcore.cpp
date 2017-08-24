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

#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfstringcore.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        StringCore::StringCore()
        {
            theCodes = nullptr;
            numberOfCodes = 0;
        }


        StringCore::StringCore(const StringCore &c)
        {
            theCodes = nullptr;
            numberOfCodes = 0;

            if(c.numberOfCodes > 0)
            {
                theCodes = new unicode_point_type[c.numberOfCodes];
                for(size_type i = 0; i < c.numberOfCodes; i++)
                    *(theCodes + i) = *(c.theCodes + i);
                numberOfCodes = c.numberOfCodes;
            }
        }


        StringCore::StringCore(StringCore &&c) noexcept
        {
            theCodes = c.theCodes;
            numberOfCodes = c.numberOfCodes;

            c.theCodes = nullptr;
            c.numberOfCodes = 0;
        }


        StringCore::StringCore(size_type i)
        {
            theCodes = nullptr;
            numberOfCodes = 0;

            if(i > 0)
            {
                theCodes = new unicode_point_type[i];
                numberOfCodes = i;
            }
        }


        StringCore::StringCore(const unicode_point_type *t, size_type i)
        {
            theCodes = nullptr;
            numberOfCodes = 0;

            if(t != nullptr && i > 0)
            {
                theCodes = new unicode_point_type[i];
                for(size_type j = 0; j < i; j++)
                    *(theCodes + j) = *(t + j);
                numberOfCodes = i;
            }
        }


        StringCore::~StringCore()
        {
            if(theCodes != nullptr && numberOfCodes > 0)
                delete[] theCodes;
            theCodes = nullptr;
            numberOfCodes = 0;
        }



        StringCore& StringCore::operator=(const StringCore &c)
        {
            if(this != &c)
            {
                if (theCodes != nullptr && numberOfCodes > 0)
                {
                    delete[] theCodes;
                    theCodes = nullptr;
                    numberOfCodes = 0;
                }

                if (c.numberOfCodes > 0)
                {
                    theCodes = new unicode_point_type[c.numberOfCodes];
                    for (size_type i = 0; i < c.numberOfCodes; i++)
                    {
                        *(theCodes + i) = *(c.theCodes + i);
                    }
                    numberOfCodes = c.numberOfCodes;
                }
            }

            return *this;
        }


        StringCore& StringCore::operator=(StringCore &&c) noexcept
        {
            if(this != &c)
            {
                if (theCodes != nullptr && numberOfCodes > 0)
                {
                    delete[] theCodes;
                    theCodes = nullptr;
                    numberOfCodes = 0;
                }

                theCodes = c.theCodes;
                numberOfCodes = c.numberOfCodes;

                c.theCodes = nullptr;
                c.numberOfCodes = 0;
            }

            return *this;
        }


        bool StringCore::operator==(const StringCore &c) const
        {
            if(this != &c)
            {
                if(numberOfCodes != c.numberOfCodes)
                    return false;

                for(size_type i = 0; i < numberOfCodes; i++)
                {
                    if(*(theCodes + i) != *(c.theCodes + i))
                        return false;
                }
            }

            return true;
        }


        bool StringCore::operator!=(const StringCore &c) const
        {
            if(*this == c)
                return false;
            return true;
        }


        StringCore::unicode_point_type& StringCore::operator[](size_type i)
        {
            if(numberOfCodes == 0 || i > numberOfCodes)
                throw std::range_error("index not contained within length of string");

            return *(theCodes + i);
        }

        StringCore::size_type StringCore::length() const
        {
            return numberOfCodes;
        }


        StringCore::unicode_point_type * StringCore::data()
        {
            return theCodes;
        }


        std::ostream& StringCore::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("StringCore");
                formatter->addClassMember<unicode_point_type>("theCodes", theCodes, numberOfCodes);
                formatter->addClassMember<size_type>("numberOfCodes",numberOfCodes);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream& operator<<(std::ostream &o, const StringCore &c)
        {
            return c.description(o);
        }
    }

}