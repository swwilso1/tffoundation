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

#ifndef TFCONVERSION_HPP
#define TFCONVERSION_HPP

#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * @class Conversion serves a collector class for algorithms converting one type to another.
         */
        class Conversion
        {
        public:
            /** @brief string type */
            using string_type = String;

            /**
             * @brief method to convert a string to an int.
             * @param s the string representing an integer number.
             * @return a Result object with the int template parameter.
             * If successful the Result value property holds the integer.
             */
            static std::optional<int> convertStringToInt(const string_type &s);

            /**
             * @brief method to convert a string to a double.
             * @param s the string representing a floating point number.
             * @return a Result object with the double template parameter.
             * If successful the Result value property holds the double.
             */
            static std::optional<double> convertStringToDouble(const string_type &s);

            /**
             * @brief method to convert a string to a bool
             *
             * The method will convert any variation of true, True, TrUe, 1
             * to true and false False, FaLse, and 0 to false.
             *
             * @param s the string representing the boolean value.
             * @return a Result object with the bool template parameter.
             * If successful the Result value property holds the bool.
             */
            static std::optional<bool> convertStringToBool(const string_type &s);
        };

    }    // namespace Foundation
}    // namespace TF

#endif    // TFCONVERSION_HPP
