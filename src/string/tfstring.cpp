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

#define NEEDS_CSTRING
#define NEEDS_CSTDARG
#define NEEDS_IOSTREAM
#define NEEDS_SSTREAM
#define NEEDS_STRING
#define NEEDS_STACK
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfstring.hpp"
#include "tfasciistringencoder.hpp"
#include "tfutf8stringencoder.hpp"
#include "tfutf16stringencoder.hpp"
#include "tfutf32stringencoder.hpp"

namespace TF
{

    namespace Foundation
    {

#pragma mark - Constructor methods

        String::String()
        {
            core = std::make_shared<core_type>();
        }

        String::String(const String& s)
        {
            core = s.core;
        }


        String::String(String &&s) noexcept
        {
            core = s.core;
        }


        String::String(const char *str)
        {
            size_type theLength = std::strlen(str);

            auto theArray = new unicode_point_type[theLength];
            for(size_type i = 0; i < theLength; i++)
                theArray[i] = static_cast<unicode_point_type>(*(str + i));

            core = std::make_shared<core_type>(theArray,theLength);
        }


        String::String(const char *str, size_type length)
        {
            auto theArray = new unicode_point_type[length];

            for(size_type i = 0; i < length; i++)
                theArray[i] = static_cast<unicode_point_type>(*(str + i));

            core = std::make_shared<core_type>(theArray, length);
        }



        String::String(const std::string& s)
        {
            size_type theNumberOfCodes = s.size();
            const char *cString = s.c_str();

            auto theArray = new unicode_point_type[theNumberOfCodes];

            for(size_type i = 0; i < theNumberOfCodes; i++)
                theArray[i] = static_cast<unicode_point_type>(*(cString + i));

            core = std::make_shared<core_type>(theArray, theNumberOfCodes);
        }


        // The argument should be encoded UTF-8.
        String::String(const unsigned char *str, size_type length)
        {
            UTF8StringEncoder encoder;
            auto tmp = str;

            if(! encoder.checkStringForCorrectness(str, length))
                throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

            size_type theNumberOfCodes = encoder.numberOfCharacters(str, length);

            auto theArray = new unicode_point_type[theNumberOfCodes];

            auto queryResult = encoder.hasByteOrderMark(str, length);
            auto bomLength = encoder.lengthOfByteOrderMarkInBytes();
            if(queryResult.first)
            {
                tmp += bomLength;
                length -= bomLength;
            }

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult = encoder.nextCodePoint(tmp, length, queryResult.second);
                theArray[i] = operationResult.first;
                tmp += operationResult.second;
                length -= operationResult.second;
            }

            core = std::make_shared<core_type>(theArray, theNumberOfCodes);
        }


        // The argument should be encoded UTF-16
        String::String(const unsigned short *str, size_type length)
        {
            UTF16StringEncoder encoder;
            auto tmp = reinterpret_cast<const unsigned char *>(str);
            size_type byteLength = length * sizeof(const unsigned short);

            if(! encoder.checkStringForCorrectness(tmp, byteLength))
                throw std::runtime_error("String UTF-16 constructor cannot create string from bad UTF-16");

            size_type theNumberOfCodes = encoder.numberOfCharacters(tmp, byteLength);

            auto theArray = new unicode_point_type[theNumberOfCodes];

            auto queryResult = encoder.hasByteOrderMark(tmp, byteLength);
            auto bomLength = encoder.lengthOfByteOrderMarkInBytes();
            if(queryResult.first)
            {
                tmp += bomLength;
                byteLength -= bomLength;
            }

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult = encoder.nextCodePoint(tmp, byteLength, queryResult.second);
                theArray[i] = operationResult.first;
                tmp += operationResult.second;
                byteLength -= operationResult.second;
            }

            core = std::make_shared<core_type>(theArray, theNumberOfCodes);
        }


        String::String(const unsigned int *str, size_type length)
        {
            UTF32StringEncoder encoder;
            auto tmp = reinterpret_cast<const unsigned char *>(str);
            size_type byteLength = length * sizeof(const unsigned short);

            if(! encoder.checkStringForCorrectness(tmp, byteLength))
                throw std::runtime_error("String UTF-32 constructor cannot create string from bad UTF-32");

            size_type theNumberOfCodes = encoder.numberOfCharacters(tmp, byteLength);

            auto theArray = new unicode_point_type[theNumberOfCodes];

            auto queryResult = encoder.hasByteOrderMark(tmp, byteLength);
            auto bomLength = encoder.lengthOfByteOrderMarkInBytes();
            if(queryResult.first)
            {
                tmp += bomLength;
                byteLength -= bomLength;
            }

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult = encoder.nextCodePoint(tmp, byteLength, queryResult.second);
                theArray[i] = operationResult.first;
                tmp += operationResult.second;
                byteLength -= operationResult.second;
            }

            core = std::make_shared<core_type>(theArray, theNumberOfCodes);
        }


        String String::initWithFormat(const char *format,...)
        {
            String result;

            // Set up the variable argument list
            va_list argList;
            va_start(argList,format);
            result = initWithFormat(format, &argList);
            va_end(argList);
            return result;
        }


#        define CLEAR_CONTROL_SETTINGS() \
        { \
            processingFormatCode = false; \
            processingPrecision = false; \
            needsZeroPadding = false; \
            hasLLModifier = false; \
            hasLModifier = false; \
            hasCapitalLModifier = false; \
            hasHModifier = false; \
            hasHHModifier = false; \
            hasFieldWidth = false; \
            hasPrecision = false; \
            needsLeftAdjustment = false; \
            fieldWidth = 0; \
            precision = 0; \
            counter++; \
        }


#        define ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(value) \
        if(processingPrecision) \
        { \
            precision = (precision * 10) + value; \
            if(! hasPrecision) \
                hasPrecision = true; \
        } \
        else \
        { \
            fieldWidth = (fieldWidth * 10) + value; \
            if(! hasFieldWidth) \
                hasFieldWidth = true; \
        }

        String String::initWithFormat(const char *format, va_list *argList)
        {
            bool processingFormatCode = false;
            bool processingPrecision = false;
            bool needsZeroPadding = false;
            bool hasLLModifier = false;
            bool hasLModifier = false;
            bool hasCapitalLModifier = false;
            bool hasHModifier = false;
            bool hasHHModifier = false;
            bool hasFieldWidth = false;
            bool hasPrecision = false;
            bool needsLeftAdjustment = false;

            int fieldWidth = 0;
            int precision = 0;

            int counter = 0;

            std::stringstream accumulator;

            // Now parse the format string.   The format string must be
            // a C-Style (ie ends with '\0' (null-terminated) string.
            // Standard printf style arguments are accepted and one
            // extra form.  %@ now indicates a String argument.

            const char *fmt = format;

            while(*fmt != '\0')
            {

                // The easiest thing to do here will be to use stl strings
                // to collect the format valid characters and the conversion values.
                // At the end we can drop it all into a UTF-16 string.
                switch(*fmt)
                {
                    case '%': // Activate code processing mode
                        if(processingFormatCode)
                        {
                            accumulator << *fmt;
                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            processingFormatCode = true;
                        }
                        break;
                    case 'i': // Same as 'd' modifier
                    case 'd': // Output a base 10 (decimal) number
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                            }


                            if(hasLLModifier)
                            {
                                long long arg;
                                arg = va_arg(*argList, long long);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }
                            else if(hasLModifier)
                            {
                                long arg;
                                arg = va_arg(*argList, long);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }
                            else if(hasHHModifier)
                            {
                                int arg;
                                arg = va_arg(*argList, int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }
                            else if(hasHModifier)
                            {
                                int arg;
                                arg = va_arg(*argList, int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }
                            else
                            {
                                int arg;
                                arg = va_arg(*argList, int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'o': // Output an octal number
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                            }

                            if(hasLLModifier)
                            {
                                unsigned long long arg;
                                arg = va_arg(*argList, unsigned long long);
                                if(needsLeftAdjustment)
                                    value << std::left << std::oct << arg;
                                else
                                    value << std::oct << arg;
                            }
                            else if(hasLModifier)
                            {
                                unsigned long arg;
                                arg = va_arg(*argList, unsigned long);
                                if(needsLeftAdjustment)
                                    value << std::left << std::oct << arg;
                                else
                                    value << std::oct << arg;
                            }
                            else if(hasHHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::oct << arg;
                                else
                                    value << std::oct << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::oct << arg;
                                else
                                    value << std::oct << arg;
                            }
                            else
                            {
                                int arg;
                                arg = va_arg(*argList, int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::oct << arg;
                                else
                                    value << std::oct << arg;
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'x': // Output a hexidecimal number in lower-case
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                            }

                            if(hasLLModifier)
                            {
                                unsigned long long arg;
                                arg = va_arg(*argList, unsigned long long);
                                if(needsLeftAdjustment)
                                    value << std::left << std::nouppercase << std::hex << arg;
                                else
                                    value << std::nouppercase << std::hex << arg;
                            }
                            else if(hasLModifier)
                            {
                                unsigned long arg;
                                arg = va_arg(*argList, unsigned long);
                                if(needsLeftAdjustment)
                                    value << std::left << std::nouppercase << std::hex << arg;
                                else
                                    value << std::nouppercase << std::hex << arg;
                            }
                            else if(hasHHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::nouppercase << std::hex << arg;
                                else
                                    value << std::nouppercase << std::hex << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::nouppercase << std::hex << arg;
                                else
                                    value << std::nouppercase << std::hex << arg;
                            }
                            else
                            {
                                int arg;
                                arg = va_arg(*argList, int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::nouppercase << std::hex << arg;
                                else
                                    value << std::nouppercase << std::hex << arg;
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'X': // Output a hexidecimal number in upper-case
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                            }

                            if(hasLLModifier)
                            {
                                unsigned long long arg;
                                arg = va_arg(*argList, unsigned long long);
                                if(needsLeftAdjustment)
                                    value << std::left << std::uppercase << std::hex << arg;
                                else
                                    value << std::uppercase << std::hex << arg;
                            }
                            else if(hasLModifier)
                            {
                                unsigned long arg;
                                arg = va_arg(*argList, unsigned long);
                                if(needsLeftAdjustment)
                                    value << std::left << std::uppercase << std::hex << arg;
                                else
                                    value << std::uppercase << std::hex << arg;
                            }
                            else if(hasHHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::uppercase << std::hex << arg;
                                else
                                    value << std::uppercase << std::hex << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::uppercase << std::hex << arg;
                                else
                                    value << std::uppercase << std::hex << arg;
                            }
                            else
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::uppercase << std::hex << arg;
                                else
                                    value << std::uppercase << std::hex << arg;
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'u': // Output an unsigned int.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                            }

                            if(hasLLModifier)
                            {
                                unsigned long long arg;
                                arg = va_arg(*argList, unsigned long long);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else if(hasLModifier)
                            {
                                unsigned long arg;
                                arg = va_arg(*argList, unsigned long);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else if(hasHHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else
                            {
                                unsigned int arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'U': // Output an unsigned long int.
                        if(processingFormatCode)
                        {
                            unsigned long arg;
                            std::stringstream value;

                            arg = va_arg(*argList, unsigned long);

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else
                            {
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'c': // Output a single character.
                        if(processingFormatCode)
                        {
                            int arg = va_arg(*argList, int);
                            auto c = static_cast<char>(arg);
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                                if(needsLeftAdjustment)
                                    value << std::left << c;
                                else
                                    value << c;
                            }
                            else
                            {
                                if(needsLeftAdjustment)
                                    value << std::left << c;
                                else
                                    value << c;
                            }
                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 's': // Output a C-Style string
                        if(processingFormatCode)
                        {
                            char *s = va_arg(*argList, char *);
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                            }

                            if(needsLeftAdjustment)
                                value << std::left << s;
                            else
                                value << s;
                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'S': // Output a std::string
                        if(processingFormatCode)
                        {
                            std::string *s = va_arg(*argList,std::string *);
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
                            }
                            if(s != nullptr)
                            {
                                if(needsLeftAdjustment)
                                    value << std::left << *s;
                                else
                                    value << *s;
                            }
                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'f': // Output a real number with 6 digits of precision.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                                if(hasPrecision) value.precision(precision);
                                else value.precision(6);
                            }
                            else
                            {
                                if(hasPrecision) value.precision(precision);
                                else value.precision(6);
                            }

                            if(hasCapitalLModifier)
                            {
                                long double arg = va_arg(*argList, long double);
                                if(needsLeftAdjustment)
                                    value << std::left << std::fixed << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << std::fixed << arg;
                                    else
                                        value << std::fixed << arg;
                                }
                            }
                            else if(hasLModifier)
                            {
                                double arg = va_arg(*argList,double);
                                if(needsLeftAdjustment)
                                    value << std::left << std::fixed << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << std::fixed << arg;
                                    else
                                        value << std::fixed << arg;
                                }
                            }
                            else
                            {
                                double arg = va_arg(*argList,double);
                                if(needsLeftAdjustment)
                                    value << std::left << std::fixed << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << std::fixed << arg;
                                    else
                                        value << std::fixed << arg;
                                }
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'g': // Output a real number with up-to 6 digits of precision.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                                if(hasPrecision) value.precision(precision);
                                else value.precision(6);
                            }
                            else
                            {
                                if(hasPrecision) value.precision(precision);
                                else value.precision(6);
                            }

                            if(hasCapitalLModifier)
                            {
                                long double arg = va_arg(*argList, long double);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }
                            else if(hasLModifier)
                            {
                                double arg = va_arg(*argList,double);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }
                            else
                            {
                                double arg = va_arg(*argList,double);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << arg;
                                    else
                                        value << arg;
                                }
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'e': // Output a real number with up-to 6 digits of precision in scientific notation.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                                if(hasPrecision) value.precision(precision);
                                else value.precision(6);
                            }
                            else
                            {
                                if(hasPrecision) value.precision(precision);
                                else value.precision(6);
                            }


                            if(hasCapitalLModifier)
                            {
                                long double arg = va_arg(*argList, long double);
                                if(needsLeftAdjustment)
                                    value << std::left << std::scientific << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << std::scientific << arg;
                                    else
                                        value << std::scientific << arg;
                                }
                            }
                            else if(hasLModifier)
                            {
                                double arg = va_arg(*argList,double);
                                if(needsLeftAdjustment)
                                    value << std::left << std::scientific << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << std::scientific << arg;
                                    else
                                        value << std::scientific << arg;
                                }
                            }
                            else
                            {
                                double arg = va_arg(*argList,double);
                                if(needsLeftAdjustment)
                                    value << std::left << std::scientific << arg;
                                else
                                {
                                    if(arg < 0 && needsZeroPadding)
                                        value << std::internal << std::scientific << arg;
                                    else
                                        value << std::scientific << arg;
                                }
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'p': // A void pointer.
                        if(processingFormatCode)
                        {
                            auto arg = reinterpret_cast<void *>(va_arg(*argList, void *));
                            std::stringstream value;
                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding) value.fill('0');
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else
                            {
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '@': // Output a String object.
                        if(processingFormatCode)
                        {

                            String *arg = va_arg(*argList, String *);
                            std::stringstream value;


                            if(needsLeftAdjustment)
                                value << std::left << *arg;
                            else
                                value << *arg;

                            if(hasFieldWidth)
                            {
                                if(fieldWidth > value.str().size())
                                {
                                    accumulator.width(fieldWidth);
                                    if(needsZeroPadding) accumulator.fill('0');
                                }
                            }

                            accumulator << value.str();

                            CLEAR_CONTROL_SETTINGS();
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '0': // Check to see if we need to turn on needsZeroPadding.
                        if(processingFormatCode)
                        {
                            if(! hasFieldWidth && ! processingPrecision)
                            {
                                needsZeroPadding = true;
                            }
                            else
                            {
                                ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(0);
                            }
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '1': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(1);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '2': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(2);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '3': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(3);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '4': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(4);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '5': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(5);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '6': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(6);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '7': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(7);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '8': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(8);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '9': // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(9);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '.': // Check to see if we have a precision setting.
                        if(processingFormatCode)
                        {
                            processingPrecision = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'l': // Check to see if we need to turn on hasLModifier or hasLLModifier.
                        if(processingFormatCode)
                        {
                            if(hasLModifier)
                                hasLLModifier = true;
                            else
                                hasLModifier = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'L': // Check to see if we need to turn on hasCapitalLModifier.
                        if(processingFormatCode)
                        {
                            hasCapitalLModifier = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'h': // Check to see if we need to turn on hasHModifier or hasHHModifier.
                        if(processingFormatCode)
                        {
                            if(hasHModifier)
                                hasHHModifier = true;
                            else
                                hasHModifier = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '-': // Turn on left justification.
                        if(processingFormatCode)
                        {
                            needsLeftAdjustment = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    default:
                        accumulator << *fmt;
                }

                fmt++;
            }

            return String(accumulator.str().c_str());
        }



#if 1 // Disabled for now
        String::iterator String::begin(void)
        {
            return iterator(core, 0);
        }


        String::iterator String::begin(void) const
        {
            return iterator(core, 0);
        }


        String::iterator String::end(void)
        {
            return iterator(core, core->length() + 1);
        }


        String::iterator String::end(void) const
        {
            return iterator(core, core->length() + 1);
        }
#endif // Disabled iterator methods


        bool String::operator==(const char *s) const
        {
            String theStr {s};
            return *this == s;
        }


        bool String::operator==(const std::string& s) const
        {
            String theStr {s};
            return *this == theStr;
        }


        bool String::operator==(const String& s) const
        {
            return *core == *s.core;
        }


        bool String::operator!=(const String& s) const
        {
            if(*this == s)
                return false;
            return true;
        }


        String& String::operator=(const String& s)
        {
            core = s.core;
            return *this;
        }


        String& String::operator=(String &&s) noexcept
        {
            core = s.core;
            return *this;
        }


        String::size_type String::length() const
        {
            return core->length();
        }


        String::size_type String::numberOfBytes()
        {
            return core->length() * sizeof(unicode_point_type);
        }


        String::unicode_point_type String::operator[](size_type i) const
        {
            return (*core)[i];
        }


        String::unicode_point_type String::characterAtIndex(size_type i)
        {
            return (*core)[i];
        }


        String String::getCharactersInRange(range_type& range)
        {
            size_type theNumberOfCodes = core->length();
            if(range.length > theNumberOfCodes)
                throw std::range_error("getCharactersInRange given range greater than the length of the string");

            size_type codesFromPositiontoEnd = theNumberOfCodes - range.position;

            if(range.length > codesFromPositiontoEnd)
                throw std::range_error("getCharactersInRange given range greater than remaining length of the string");

            auto theTmp = core->data();

            theTmp += range.position;

            String s(theTmp, range.length);
            return s;
        }


        std::unique_ptr<const char> String::c_str()
        {
            ASCIIStringEncoder encoder;

            size_type theNumberOfCodes = core->length();

            auto theTmp = core->data();

            size_type byteLength = 0;

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                byteLength += encoder.bytesNeededForRepresentationOfCode(*(theTmp + i));
            }

            auto myEndianness = encoder.thisSystemEndianness();

            auto theBytes = new char[byteLength + 1];
            auto tmp = theBytes;

            *(theBytes + byteLength) = '\0';

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto bytesUsed = encoder.encodeCodePoint(reinterpret_cast<StringEncoder::char_type *>(tmp), byteLength,
                    *(theTmp + i), myEndianness);
                tmp += bytesUsed;
                byteLength -= bytesUsed;
            }

            return std::unique_ptr<const char>(theBytes);
        }


        std::string String::stlString()
        {
            std::unique_ptr<const char> cStr = this->c_str();
            return std::string(cStr.get());
        }


        String String::stringByAppendingFormat(const char *format, ...)
        {
            String result;
            va_list argList;
            va_start(argList, format);
            result = stringByAppendingFormat(format, &argList);
            va_end(argList);
            return result;
        }


        String String::stringByAppendingFormat(const char *format, va_list *argList)
        {
            String formatString;
            String newString;

            formatString = initWithFormat(format, argList);
            newString = concatenateStrings(*this, formatString);

            return newString;
        }


        String String::stringByAppendingString(const String& str)
        {
            return concatenateStrings(*this, str);
        }


        String String::concatenateStrings(const String& s1, const String& s2)
        {
            size_type newNumberOfCodes = s1.length() + s2.length();

            auto codes = new unicode_point_type[newNumberOfCodes];

            size_type i = 0;

            for(i = 0; i < s1.length(); i++)
            {
                *(codes + i) = s1[i];
            }

            size_type j = 0;

            for(j = 0; j < s2.length(); j++)
            {
                *(codes + i + j) = s2[j];
            }


            String result(codes, newNumberOfCodes);
            return result;
        }


        String String::substringFromIndex(size_type i)
        {
            if(i > this->length())
                throw std::runtime_error("substringFromIndex index larger than length of string");

            auto newNumberOfCodes = this->length() - i;

            auto codes = new unicode_point_type[newNumberOfCodes];

            for(size_type j = 0; j < newNumberOfCodes; j++, i++)
            {
                *(codes + j) = (*this)[i];
            }

            String result(codes, newNumberOfCodes);

            return result;
        }


        String String::substringWithRange(range_type& range)
        {
            if(range.position > this->length())
                throw std::range_error("substringWithRange given range with position larger than length of string");

            auto remainingCodes = this->length() - range.position;

            if(range.length > remainingCodes)
            {
                throw std::range_error("substringWithRange given range with length larger than remaining length"
                    " of string");
            }

            auto newNumberOfCodes = range.length;

            auto codes = new unicode_point_type[newNumberOfCodes];

            size_type i = 0, j = range.position;

            for(i = 0 ; i < newNumberOfCodes; i++, j++)
            {
                *(codes + i) = (*this)[j];
            }

            String result(codes, newNumberOfCodes);

            return result;
        }


        String::string_array_type String::substringsNotInRange(range_type& range)
        {
            string_array_type substringArray;

            if(range.position > 0)
            {
                range_type theRange(0, range.position);
                String tmpString = this->substringWithRange(theRange);
                substringArray.push_back(tmpString);
            }

            if(range.position + range.length < this->length())
            {
                size_type theLength = this->length() - (range.position + range.length);
                range_type theRange(range.position + range.length, theLength);
                String tmpString = this->substringWithRange(theRange);
                substringArray.push_back(tmpString);
            }

            return substringArray;
        }


        String String::substringToIndex(size_type i)
        {
            if(i > this->length())
                throw std::range_error("substringToIndex given index greater than length of string");

            auto codes = new unicode_point_type[i];

            for(size_type j = 0; j < i; j++)
                *(codes + j) = (*this)[j];

            String result(codes, i);

            return result;
        }


        String::string_array_type String::substringsThatDoNotMatchString(const String& str)
        {
            range_array_type rangesOfSubStrings;
            string_array_type theSubStrings;
            UTF32StringEncoder theEncoder;

            unsigned char *theseBytes = reinterpret_cast<unsigned char *>(core->data());
            unsigned char *thoseBytes = reinterpret_cast<unsigned char *>(core->data());

            size_type theseByteLengths = this->length() * sizeof(unicode_point_type);
            size_type thoseByteLengths = str.length() * sizeof(unicode_point_type);

            rangesOfSubStrings = theEncoder.findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
                    theseBytes, theseByteLengths, thoseBytes, thoseByteLengths);

            for(auto &theRange : rangesOfSubStrings)
            {
                theSubStrings.push_back(this->substringWithRange(theRange));
            }

            return theSubStrings;
        }


        String::string_array_type String::split(const String& splitString)
        {
            return this->substringsThatDoNotMatchString(splitString);
        }


        String::range_type String::rangeOfString(const String& str)
        {
            range_type theRange;
            UTF32StringEncoder theEncoder;

            unsigned char *theseBytes = reinterpret_cast<unsigned char *>(core->data());
            unsigned char *thoseBytes = reinterpret_cast<unsigned char *>(str.core->data());

            size_type theseByteLengths = this->length() * sizeof(unicode_point_type);
            size_type thoseByteLengths = str.length() * sizeof(unicode_point_type);

            theRange = theEncoder.findCharacterRangeForSubstringInString(theseBytes,
                    theseByteLengths, thoseBytes, thoseByteLengths);

            return theRange;
        }


        String::range_array_type String::rangesOfString(const String& str)
        {
            range_array_type theRanges;
            UTF32StringEncoder theEncoder;

            unsigned char *theseBytes = reinterpret_cast<unsigned char *>(core->data());
            unsigned char *thoseBytes = reinterpret_cast<unsigned char *>(str.core->data());

            size_type theseByteLengths = this->length() * sizeof(unicode_point_type);
            size_type thoseByteLengths = str.length() * sizeof(unicode_point_type);

            theRanges = theEncoder.findCharacterRangesForSubstringInString(theseBytes,
                    theseByteLengths, thoseBytes, thoseByteLengths);

            return theRanges;
        }


        String String::stringByReplacingOccurencesOfStringWithString(
            const String& original, const String& replacement)
        {
            range_array_type originalStringRanges;
            size_type bytesNeededForNewString;
            UTF32StringEncoder theEncoder;

            unsigned char *theseBytes = reinterpret_cast<unsigned char *>(core->data());
            unsigned char *originalBytes = reinterpret_cast<unsigned char *>(original.core->data());
            unsigned char *replacementBytes = reinterpret_cast<unsigned char *>(replacement.core->data());

            size_type theseByteLengths = this->length() * sizeof(unicode_point_type);
            size_type originalByteLengths = original.length() * sizeof(unicode_point_type);
            size_type replacementByteLengths = replacement.length() * sizeof(unicode_point_type);

            bytesNeededForNewString = theEncoder.computeArraySizeInBytesForStringByReplacingSubstrings(
                    theseBytes, theseByteLengths, originalBytes, originalByteLengths, replacementBytes,
                    replacementByteLengths, originalStringRanges);

            unsigned char *newString = new unsigned char [bytesNeededForNewString];

            theEncoder.replaceOccurancesOfStringWithString(theseBytes, theseByteLengths, newString,
                    bytesNeededForNewString, replacementBytes,
                    replacementByteLengths, originalStringRanges);

            String theResultString(reinterpret_cast<unsigned int *>(newString),
                    bytesNeededForNewString / sizeof(unsigned int));

            delete[] newString;

            return theResultString;
        }


        String String::stringByReplacingCharactersInRangeWithString(range_type& range, const String& str)
        {
            UTF32StringEncoder theEncoder;

            unsigned char *theseBytes = reinterpret_cast<unsigned char *>(core->data());
            size_type theseByteLengths = this->length() * sizeof(unicode_point_type);

            if(! theEncoder.doesRangeOfCharactersLieInString(theseBytes, theseByteLengths, range))
                throw std::out_of_range("Range argument lies outside range of string.");

            String theNewString;

            size_type totalLength = this->length();

            if(range.position == 0)
            {
                // The string represented by range is at the beginning of this string.
                // Calculate the range of the remaining string.
                range_type newRange(range.length,
                        totalLength - range.length);
                String theRemainingString = this->substringWithRange(newRange);
                theNewString = theNewString.stringByAppendingString(str);
                theNewString = theNewString.stringByAppendingString(theRemainingString);
            }
            else if(range.position == (totalLength - range.length - 1))
            {
                // The string represented by range is at the end of this string.
                // Calculate the range of the preceding string.
                range_type newRange(0, range.position);
                String thePrecedingString = this->substringWithRange(newRange);
                theNewString = theNewString.stringByAppendingString(thePrecedingString);
                theNewString = theNewString.stringByAppendingString(str);
            }
            else
            {
                // The string represented by range is somewhere in the middle of this string, ie not at
                // either edge.  Calculate the preceding and remaining stubstrings.
                range_type precedingRange(0, range.position);
                size_type remainingLocation = range.position + range.length;
                range_type remainingRange(remainingLocation,
                        totalLength - remainingLocation);
                String thePrecedingString = this->substringWithRange(precedingRange);
                String theRemainingString = this->substringWithRange(remainingRange);
                theNewString = theNewString.stringByAppendingString(thePrecedingString);
                theNewString = theNewString.stringByAppendingString(str);
                theNewString = theNewString.stringByAppendingString(theRemainingString);
            }

            return theNewString;
        }


        ComparisonResult String::compare(const String& str) const
        {
            iterator thisIterator;
            iterator thatIterator;

            for(thisIterator = this->begin(), thatIterator = str.begin(); thisIterator != this->end() &&
                    thatIterator != str.end(); thisIterator++, thatIterator++)
            {
                if(*thisIterator < *thatIterator)
                    return OrderedAscending;
                if(*thisIterator > *thatIterator)
                    return OrderedDescending;
            }

            if(this->length() == str.length())
                return OrderedSame;
            else if(this->length() > str.length())
                return OrderedAscending;

            return OrderedDescending;
        }


        ComparisonResult String::compareRangeWithString(range_type& range, const String& str)
        {
            String rangeOfThisString = this->substringWithRange(range);
            return rangeOfThisString.compare(str);
        }


        bool String::hasPrefix(const String& str)
        {
            if(this->length() < str.length())
                return false;

            range_type initialRangeOfThisString(0, str.length());
            String thePrefixOfThisString = this->substringWithRange(initialRangeOfThisString);

            if(thePrefixOfThisString.compare(str) == OrderedSame)
                return true;

            return false;
        }


        bool String::hasSuffix(const String& str)
        {
            if(this->length() < str.length())
                return false;

            range_type endRangeOfThisString(this->length() - str.length(),
                str.length());
            String theSuffixOfThisString = this->substringWithRange(endRangeOfThisString);

            if(theSuffixOfThisString.compare(str) == OrderedSame)
                return true;

            return false;
        }


        bool String::isEqualToString(const String& str)
        {
            if(this->compare(str) == OrderedSame)
                return true;
            return false;
        }


        String String::capitalizedString()
        {
            // This initialization is probably wrong because of byte order mark rules.
            String capitalString(core->data(), core->length());
            bool nextCharShouldBeCapital = true;

            for(size_type i = 0; i < capitalString.core->length(); i++)
            {
                auto theCharacter = (*capitalString.core)[i];

                if(nextCharShouldBeCapital)
                {
                    if(theCharacter >= 97 || theCharacter <= 122)
                        theCharacter -= 32;
                    nextCharShouldBeCapital = false;
                }

                if(theCharacter == 32)
                    nextCharShouldBeCapital = true;
                else if(theCharacter >= 65 && theCharacter <= 90)
                    theCharacter += 32;
            }
            return capitalString;
        }


        String String::lowercaseString()
        {
            String lowerString(core->data(), core->length());

            for(size_type i = 0; i < lowerString.core->length(); i++)
            {
                auto theCharacter = (*lowerString.core)[i];

                if(theCharacter >= 65 && theCharacter <= 90)
                    theCharacter += 32;
            }
            return lowerString;
        }


        String String::uppercaseString()
        {
            String upperString(core->data(), core->length());

            for(size_type i = 0; i < upperString.core->length(); i++)
            {
                auto theCharacter = (*upperString.core)[i];

                if(theCharacter >= 97 && theCharacter <= 122)
                    theCharacter -= 32;
            }
            return upperString;
        }


        String::data_type String::getAsData()
        {
            data_type data(reinterpret_cast<char *>(core->data()),
                core->length() * sizeof(unicode_point_type));
            return data;
        }


        String::data_type String::convertToThisEncoding(const String &s, encoder_type *encoder)
        {
            size_type theNumberOfBytesToEncodeTheSubstringInThisEncoding = 0;
            iterator theIterator;
            unicode_point_type theCode;

            for(theIterator = s.begin(); theIterator != s.end(); theIterator++)
            {
                theCode = *theIterator;
                theNumberOfBytesToEncodeTheSubstringInThisEncoding +=
                        encoder->bytesNeededForRepresentationOfCode(theCode);
            }

            if(encoder->usesByteOrderMark())
            {
                theNumberOfBytesToEncodeTheSubstringInThisEncoding +=
                        encoder->lengthOfByteOrderMarkInBytes();
            }

            auto newSubstringBytes = new unsigned char[theNumberOfBytesToEncodeTheSubstringInThisEncoding];
            auto tmp = newSubstringBytes;
            auto bytesLeftInTheNewString = theNumberOfBytesToEncodeTheSubstringInThisEncoding;

            if(encoder->usesByteOrderMark())
            {
                encoder->writeByteOrderMark(tmp, bytesLeftInTheNewString);
                tmp += encoder->lengthOfByteOrderMarkInBytes();
                bytesLeftInTheNewString -= encoder->lengthOfByteOrderMarkInBytes();
            }

            size_type bytesUsedToEncodeTheCode;
            Endian thisEndian = encoder->thisSystemEndianness();

            for(theIterator = s.begin(); theIterator != s.end(); theIterator++)
            {
                theCode = *theIterator;
                bytesUsedToEncodeTheCode = encoder->encodeCodePoint(tmp, bytesLeftInTheNewString, theCode, thisEndian);
                tmp += bytesUsedToEncodeTheCode;
                bytesLeftInTheNewString -= bytesUsedToEncodeTheCode;
            }

            data_type data(reinterpret_cast<char *>(newSubstringBytes),
                    theNumberOfBytesToEncodeTheSubstringInThisEncoding);

            delete[] newSubstringBytes;

            return data;
        }

        String::data_type String::getAsDataInASCIIEncoding()
        {
            ASCIIStringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


        String::data_type String::getAsDataInUTF8Encoding()
        {
            UTF8StringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


        String::data_type String::getAsDataInUTF16Encoding()
        {
            UTF16StringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


        String::data_type String::getAsDataInUTF32Encoding(void)
        {
            UTF32StringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


#ifdef TFTESTS
            char_type * String::bytes(void)
            {
                return theBytes;
            }


            StringEncoder * String::encoder(void)
            {
                return theEncoder;
            }
#endif /* TFTESTS */


        std::ostream& String::description(std::ostream& o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("String");
                formatter->addClassMember<core_pointer_type>("core",core);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream& operator<<(std::ostream& o, const String& s)
        {
            return s.description(o);
        }


        ComparisonResult compareStrings(const String& stringOne, const String& stringTwo, void *context)
        {
            return stringOne.compare(stringTwo);
        }

    } // Foundation

} // TF

