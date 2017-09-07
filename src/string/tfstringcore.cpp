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
#define NEEDS_CSTRING
#include "tfheaders.hpp"
#include "tfstringcore.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        StringCore::StringCore()
        {
            theBytes = nullptr;
            numberOfBytes = 0;
        }


        StringCore::StringCore(const StringCore &c)
        {
            theBytes = nullptr;
            numberOfBytes = 0;

            if(c.numberOfBytes > 0)
            {
                theBytes = new char_type[c.numberOfBytes];
                std::memcpy(reinterpret_cast<void *>(theBytes),
                        reinterpret_cast<void *>(c.theBytes), c.numberOfBytes * sizeof(char_type));
                numberOfBytes = c.numberOfBytes;
            }
        }


        StringCore::StringCore(StringCore &&c) noexcept
        {
            theBytes = c.theBytes;
            numberOfBytes = c.numberOfBytes;

            c.theBytes = nullptr;
            c.numberOfBytes = 0;
        }


        StringCore::StringCore(size_type i)
        {
            theBytes = nullptr;
            numberOfBytes = 0;

            if(i > 0)
            {
                theBytes = new char_type[i];
                numberOfBytes = i;
            }
        }


        StringCore::StringCore(const char_type *t, size_type i)
        {
            theBytes = nullptr;
            numberOfBytes = 0;

            if(t != nullptr && i > 0)
            {
                theBytes = new char_type[i];
                std::memcpy(reinterpret_cast<void *>(theBytes),
                        reinterpret_cast<void *>(
                                const_cast<char_type *>(t)), i * sizeof(char_type));
                numberOfBytes = i;
            }
        }


        StringCore::~StringCore()
        {
            if(theBytes != nullptr && numberOfBytes > 0)
                delete[] theBytes;
            theBytes = nullptr;
            numberOfBytes = 0;
        }



        StringCore& StringCore::operator=(const StringCore &c)
        {
            if(this != &c)
            {
                if (theBytes != nullptr && numberOfBytes > 0)
                {
                    delete[] theBytes;
                    theBytes = nullptr;
                    numberOfBytes = 0;
                }

                if (c.numberOfBytes > 0)
                {
                    theBytes = new char_type[c.numberOfBytes];
                    std::memcpy(reinterpret_cast<void *>(theBytes),
                            reinterpret_cast<void *>(c.theBytes), c.numberOfBytes * sizeof(char_type));
                    numberOfBytes = c.numberOfBytes;
                }
            }

            return *this;
        }


        StringCore& StringCore::operator=(StringCore &&c) noexcept
        {
            if(this != &c)
            {
                if (theBytes != nullptr && numberOfBytes > 0)
                {
                    delete[] theBytes;
                    theBytes = nullptr;
                    numberOfBytes = 0;
                }

                theBytes = c.theBytes;
                numberOfBytes = c.numberOfBytes;

                c.theBytes = nullptr;
                c.numberOfBytes = 0;
            }

            return *this;
        }


        bool StringCore::operator==(const StringCore &c) const
        {
            if(this != &c)
            {
                if(numberOfBytes != c.numberOfBytes)
                    return false;

                for(size_type i = 0; i < numberOfBytes; i++)
                {
                    if(*(theBytes + i) != *(c.theBytes + i))
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


        StringCore::size_type StringCore::length() const
        {
            return numberOfBytes;
        }


        StringCore::char_type * StringCore::data()
        {
            return theBytes;
        }


        std::ostream& StringCore::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("StringCore");
                formatter->addClassMember<char_type>("theBytes", theBytes, numberOfBytes);
                formatter->addClassMember<size_type>("numberOfBytes",numberOfBytes);
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