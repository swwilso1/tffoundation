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
SOFTWARE.


******************************************************************************/

#ifndef TFARGUMENTTYPE_HPP
#define TFARGUMENTTYPE_HPP

#define NEEDS_OSTREAM
#include "tfheaders.hpp"

namespace TF
{

    namespace Foundation
    {

        enum class ArgumentType
        {
            /** @brief argument represents an integer type of type int */
            Int,

            /** @brief argument represents a floating point number of type double */
            Real,

            /** @brief argument represents a string */
            String,

            /** @brief argument represents a boolean true or false */
            Bool
        };

        /**
         * @brief Overload of << operator for ArgumentType enums.
         * @param o the ostream object
         * @param type the ArgumentType enum
         * @return @e o the ostream object after writing the type info
         * to the stream.
         */
        std::ostream & operator<<(std::ostream & o, const ArgumentType & type);
    } // namespace Foundation
} // namespace TF

#endif // TFARGUMENTTYPE_HPP
