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

#define NEEDS_CSTDLIB
#define NEEDS_CSTRING
#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tfconversion.hpp"

namespace TF
{

    namespace Foundation
    {

        std::optional<int> Conversion::convertStringToInt(const string_type &s)
        {
            auto theCStr = s.cStr();
            char *endptr = nullptr;
            auto conversion_result = (int)strtol(theCStr.get(), &endptr, 10);

            auto length = endptr - theCStr.get();

            /* strtol can return 0 in the case of an error and 0 in the case of
             * a successful conversion from a string with 0 as contents.  To
             * detect the failure case, we check the string for 0 as well. */
            if(conversion_result == 0 && s != "0")
            {
                return std::optional<int> {};
            }
            else if(length < strlen(theCStr.get()))
            {
                return std::optional<int> {};
            }

            return std::optional<int> {conversion_result};
        }


        std::optional<double> Conversion::convertStringToDouble(const string_type &s)
        {
            auto theCStr = s.cStr();
            const char *theStr = theCStr.get();
            size_t theLength = strlen(theStr);
            char *theEnd = nullptr;
            auto conversion_result = strtod(theStr, &theEnd);
            if((conversion_result == 0.0 && theStr == theEnd) || ((theEnd - theStr) < theLength))
            {
                return std::optional<double> {};
            }

            return std::optional<double> {conversion_result};
        }


        std::optional<bool> Conversion::convertStringToBool(const string_type &s)
        {
            auto lowerValue = s.lowercaseString();
            if(lowerValue == "true" || lowerValue == "1")
            {
                return std::optional<bool> {true};
            }
            if(lowerValue == "false" || lowerValue == "0")
            {
                return std::optional<bool> {false};
            }

            return std::optional<bool> {};
        }

    }    // namespace Foundation

}    // namespace TF
