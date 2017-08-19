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

namespace TF
{

    namespace Foundation
    {

#pragma mark - Constructor methods

        String::String()
        {
            theCodes = code_array_type(nullptr);
            theNumberOfCodes = 0;
        }

        String::String(const String& s)
        {
            theCodes = s.theCodes;
            theNumberOfCodes = s.theNumberOfCodes;
        }


        String::String(String &&s) noexcept
        {
            theCodes = s.theCodes;
            theNumberOfCodes = s.theNumberOfCodes;
        }


        String::String(const char *str)
        {
            size_type theLength = std::strlen(str);

            unicode_point_type *theArray = new unicode_point_type[theLength];
            for(size_type i = 0; i < theLength; i++)
                theArray[i] = static_cast<unicode_point_type>(*(str + i));

            theNumberOfCodes = theLength;
            theCodes = code_array_type(theArray);
        }


        String::String(const char *str, size_type length)
        {
            theNumberOfCodes = length;

            unicode_point_type *theArray = new unicode_point_type[length];

            for(size_type i = 0; i < length; i++)
                theArray[i] = static_cast<unicode_point_type>(*(str + i));

            theCodes = code_array_type(theArray);
        }



        String::String(const std::string& s)
        {
            theNumberOfCodes = s.size();
            const char *cString = s.c_str();

            unicode_point_type *theArray = new unicode_point_type[theNumberOfCodes];

            for(size_type i = 0; i < theNumberOfCodes; i++)
                theArray[i] = static_cast<unicode_point_type>(*(cString + i));

            theCodes = code_array_type(theArray);
        }


        String::String(const unsigned char *str, size_type length)
        {
        }


        String::String(const unsigned short *str, size_type length)
        {
        }


        String::String(const unsigned int *str, size_type length)
        {
        }


        String::~String()
        {
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
                            char c = static_cast<char>(arg);
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
                            if(s)
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
                            void *arg = reinterpret_cast<void *>(va_arg(*argList, void *));
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



#if 0 // Disabled for now
        String::iterator String::begin(void)
        {
        }


        String::iterator String::begin(void) const
        {
        }


        String::iterator String::end(void)
        {
        }


        String::iterator String::end(void) const
        {
        }
#endif // Disabled iterator methods


        bool String::operator==(const char *s) const
        {
            return false;
        }


        bool String::operator==(const std::string& s) const
        {
            return false;
        }


        bool String::operator==(const String& s) const
        {
            return false;
        }


        bool String::operator!=(const String& s) const
        {
            return false;
        }


        String& String::operator=(const String& s)
        {
            return *this;
        }


        String& String::operator=(String &&s) noexcept
        {
            return *this;
        }


        String::size_type String::length(void) const
        {
            return 0;
        }


        String::size_type String::numberOfBytes(void)
        {
            return 0;
        }


        String::unicode_point_type String::operator[](size_type i) const
        {
            return 0;
        }


        String::unicode_point_type String::characterAtIndex(size_type i)
        {
            return 0;
        }


        String String::getCharactersInRange(range_type& range)
        {
            String s;
            return s;
        }


        std::shared_ptr<const char> String::c_str(void)
        {
            return std::shared_ptr<const char>(nullptr);
        }


        std::string String::stlString(void)
        {
            return std::string();
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
            String result;
            return result;
        }


        String String::substringFromIndex(size_type i)
        {
            String result;
            return result;
        }


        String String::substringWithRange(range_type& range)
        {
            String result;
            return result;
        }


        String::string_array_type String::substringsNotInRange(range_type& range)
        {
            string_array_type array;
            return array;
        }


        String String::substringToIndex(size_type i)
        {
            String result;
            return result;
        }


        String::string_array_type String::substringsThatDoNotMatchString(const String& str)
        {
            string_array_type  array;
            return array;
        }


        String::string_array_type String::split(const String& splitString)
        {
            return this->substringsThatDoNotMatchString(splitString);
        }


        String::range_type String::rangeOfString(const String& str)
        {
            range_type r;
            return r;
        }


        String::range_array_type String::rangesOfString(const String& str)
        {
            range_array_type array;
            return array;
        }


        String String::stringByReplacingOccurencesOfStringWithString(
            const String& original, const String& replacement)
        {
            String result;
            return result;
        }


        String String::stringByReplacingCharactersInRangeWithString(range_type& range, const String& str)
        {
            String result;
            return result;
        }


        ComparisonResult String::compare(const String& str) const
        {
            return OrderedSame;
        }


        ComparisonResult String::compareRangeWithString(range_type& range, const String& str)
        {
            return OrderedSame;
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


        String String::capitalizedString(void)
        {
            String result;
            return result;
        }


        String String::lowercaseString(void)
        {
            String result;
            return result;
        }


        String String::uppercaseString(void)
        {
            String result;
            return result;
        }


        String::data_type String::getAsData(void)
        {
            data_type data;
            return data;
        }


        String::data_type String::getAsDataInASCIIEncoding(void)
        {
            data_type data;
            return data;
        }


        String::data_type String::getAsDataInUTF8Encoding(void)
        {
            data_type data;
            return data;
        }


        String::data_type String::getAsDataInUTF16Encoding(void)
        {
            data_type data;
            return data;
        }


        String::data_type String::getAsDataInUTF32Encoding(void)
        {
            data_type data;
            return data;
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

