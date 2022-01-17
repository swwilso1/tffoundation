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
#include "tfheaders.hpp"
#include "tfconversion.hpp"

namespace TF
{

    namespace Foundation
    {

        Result<int> Conversion::convertStringToInt(const string_type &s)
        {
            Result<int> result;
            auto theCStr = s.cStr();
            char *endptr = nullptr;
            result.value = (int)strtol(theCStr.get(), &endptr, 10);

            auto length = endptr - theCStr.get();

            /* strtol can return 0 in the case of an error and 0 in the case of
             * a successful conversion from a string with 0 as contents.  To
             * detect the failure case, we check the string for 0 as well. */
            if(result.value == 0 && s != "0")
            {
                result.succeeded = false;
            }
            else if(length < strlen(theCStr.get()))
            {
                result.succeeded = false;
            }
            else
            {
                result.succeeded = true;
            }

            return result;
        }


        Result<double> Conversion::convertStringToDouble(const string_type &s)
        {
            Result<double> result;
            auto theCStr = s.cStr();
            const char *theStr = theCStr.get();
            size_t theLength = strlen(theStr);
            char *theEnd = nullptr;
            result.value = strtod(theStr, &theEnd);
            if((result.value == 0.0 && theStr == theEnd) || ((theEnd - theStr) < theLength))
            {
                result.succeeded = false;
            }
            else
            {
                result.succeeded = true;
            }

            return result;
        }


        Result<bool> Conversion::convertStringToBool(const string_type &s)
        {
            Result<bool> result;
            auto lowerValue = s.lowercaseString();
            result.succeeded = true;
            if(lowerValue == "true" || lowerValue == "1")
                result.value = true;
            else if(lowerValue == "false" || lowerValue == "0")
                result.value = false;
            else
            {
                result.succeeded = false;
            }

            return result;
        }

    }    // namespace Foundation

}    // namespace TF
