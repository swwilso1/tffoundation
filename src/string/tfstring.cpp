/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

        bool String::useObjectFormattingOutput = false;

#pragma mark - Constructor methods

        String::String()
        {
            core = std::make_shared<core_type>(0);
        }

        String::String(const String &s)
        {
            core = std::make_shared<core_type>(*s.core);
        }


        String::String(String &&s) noexcept
        {
            core = s.core;
        }


        String::String(const char *str)
        {
            UTF8StringEncoder encoder;
            const UTF8StringEncoder::char_type *tmp = reinterpret_cast<const UTF8StringEncoder::char_type *>(str);
            auto theLength = strlen(str);

            if(!encoder.checkStringForCorrectness(tmp, theLength))
                throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

            core = std::make_shared<core_type>(tmp, theLength);
        }


        String::String(const char *str, size_type length)
        {
            UTF8StringEncoder encoder;
            const UTF8StringEncoder::char_type *tmp = reinterpret_cast<const UTF8StringEncoder::char_type *>(str);

            if(!encoder.checkStringForCorrectness(tmp, length))
                throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

            core = std::make_shared<core_type>(tmp, length);
        }


        String::String(const std::string &s)
        {
            ASCIIStringEncoder asciiEncoder;
            auto stringLength = s.size();
            auto str = s.c_str();
            auto charactersInString =
                asciiEncoder.numberOfCharacters(reinterpret_cast<const char_type *>(str), stringLength);

            auto characters = new unicode_point_type[charactersInString];
            auto endian = asciiEncoder.thisSystemEndianness();

            char_type *tmp = reinterpret_cast<char_type *>(const_cast<char *>(str));
            size_type i = 0;

            // Use the ASCII encoder to convert the argument to unicode points.
            while(stringLength > 0)
            {
                auto result = asciiEncoder.nextCodePoint(tmp, stringLength, endian);
                characters[i++] = result.first;
                tmp += result.second;
                stringLength -= result.second;
            }

            UTF8StringEncoder utf8Encoder;

            // Now convert the unicode points to UTF-8
            size_type bytesRequiredForUTF8 = 0;

            for(size_type i = 0; i < charactersInString; ++i)
            {
                bytesRequiredForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(characters[i]);
            }

            auto theArray = new char_type[bytesRequiredForUTF8];
            tmp = theArray;
            auto tmpLength = bytesRequiredForUTF8;

            for(size_type i = 0; i < charactersInString; ++i)
            {
                auto result = utf8Encoder.encodeCodePoint(tmp, tmpLength, characters[i], endian);
                tmp += result;
                tmpLength -= result;
            }

            delete[] characters;

            core = std::make_shared<core_type>(theArray, bytesRequiredForUTF8);

            delete[] theArray;
        }


        // The argument should be encoded UTF-8.
        String::String(const unsigned char *str, size_type length)
        {
            UTF8StringEncoder encoder;

            if(!encoder.checkStringForCorrectness(str, length))
                throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

            core = std::make_shared<core_type>(str, length);
        }


        // The argument should be encoded UTF-16
        String::String(const unsigned short *str, size_type length)
        {
            UTF16StringEncoder encoder;
            UTF8StringEncoder utf8Encoder;
            auto tmp = reinterpret_cast<const unsigned char *>(str);
            size_type byteLength = length * sizeof(const unsigned short);

            if(!encoder.checkStringForCorrectness(tmp, byteLength))
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

            size_type bytesNeededForUTF8 = 0;

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult = encoder.nextCodePoint(tmp, byteLength, queryResult.second);
                theArray[i] = operationResult.first;
                bytesNeededForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(theArray[i]);
                tmp += operationResult.second;
                byteLength -= operationResult.second;
            }

            auto charArray = new char_type[bytesNeededForUTF8];

            auto tmp2 = charArray;
            auto bytesLeft = bytesNeededForUTF8;

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult =
                    utf8Encoder.encodeCodePoint(tmp2, bytesLeft, theArray[i], utf8Encoder.thisSystemEndianness());
                tmp2 += operationResult;
                bytesLeft -= operationResult;
            }

            delete[] theArray;

            core = std::make_shared<core_type>(charArray, bytesNeededForUTF8);

            delete[] charArray;
        }


        String::String(const unsigned int *str, size_type length)
        {
            UTF32StringEncoder encoder;
            UTF8StringEncoder utf8Encoder;
            auto tmp = reinterpret_cast<const unsigned char *>(str);
            size_type byteLength = length * sizeof(const unsigned int);

            if(!encoder.checkStringForCorrectness(tmp, byteLength))
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

            size_type bytesNeededForUTF8 = 0;

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult = encoder.nextCodePoint(tmp, byteLength, queryResult.second);
                theArray[i] = operationResult.first;
                bytesNeededForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(theArray[i]);
                tmp += operationResult.second;
                byteLength -= operationResult.second;
            }

            auto charArray = new char_type[bytesNeededForUTF8];

            auto tmp2 = charArray;
            auto bytesLeft = bytesNeededForUTF8;

            for(size_type i = 0; i < theNumberOfCodes; i++)
            {
                auto operationResult =
                    utf8Encoder.encodeCodePoint(tmp2, bytesLeft, theArray[i], utf8Encoder.thisSystemEndianness());
                tmp2 += operationResult;
                bytesLeft -= operationResult;
            }

            delete[] theArray;

            core = std::make_shared<core_type>(charArray, bytesNeededForUTF8);

            delete[] charArray;
        }


        String::String(const unsigned int c)
        {
            UTF8StringEncoder utf8StringEncoder;
            auto bytesNeededForUTF8 = utf8StringEncoder.bytesNeededForRepresentationOfCode(c);

            auto charArray = new char_type[bytesNeededForUTF8];
            utf8StringEncoder.encodeCodePoint(charArray, bytesNeededForUTF8, c, StringEncoder::thisSystemEndianness());

            core = std::make_shared<core_type>(charArray, bytesNeededForUTF8);

            delete[] charArray;
        }


        String String::deepCopy() const
        {
            if(core->length() == 0)
                return String();

            String s(core->data(), core->length());
            return s;
        }


        String String::initWithFormat(const char *format, ...)
        {
            String result;

            // Set up the variable argument list
            va_list argList;
            va_start(argList, format);
            result = initWithFormat(format, &argList);
            va_end(argList);
            return result;
        }


#define CLEAR_CONTROL_SETTINGS()                                                                                       \
    {                                                                                                                  \
        processingFormatCode = false;                                                                                  \
        processingPrecision = false;                                                                                   \
        needsZeroPadding = false;                                                                                      \
        hasLLModifier = false;                                                                                         \
        hasLModifier = false;                                                                                          \
        hasCapitalLModifier = false;                                                                                   \
        hasHModifier = false;                                                                                          \
        hasHHModifier = false;                                                                                         \
        hasFieldWidth = false;                                                                                         \
        hasPrecision = false;                                                                                          \
        needsLeftAdjustment = false;                                                                                   \
        fieldWidth = 0;                                                                                                \
        precision = 0;                                                                                                 \
        counter++;                                                                                                     \
    }


#define ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(value)                                                                    \
    if(processingPrecision)                                                                                            \
    {                                                                                                                  \
        precision = (precision * 10) + value;                                                                          \
        if(!hasPrecision)                                                                                              \
            hasPrecision = true;                                                                                       \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        fieldWidth = (fieldWidth * 10) + value;                                                                        \
        if(!hasFieldWidth)                                                                                             \
            hasFieldWidth = true;                                                                                      \
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
                    case '%':    // Activate code processing mode
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
                    case 'i':    // Same as 'd' modifier
                    case 'd':    // Output a base 10 (decimal) number
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                                char arg;
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
                                short arg;

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
                    case 'o':    // Output an octal number
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                                unsigned char arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::oct << arg;
                                else
                                    value << std::oct << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned short arg;
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
                    case 'x':    // Output a hexidecimal number in lower-case
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                                unsigned char arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::nouppercase << std::hex << arg;
                                else
                                    value << std::nouppercase << std::hex << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned short arg;
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
                    case 'X':    // Output a hexidecimal number in upper-case
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                                unsigned char arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << std::uppercase << std::hex << arg;
                                else
                                    value << std::uppercase << std::hex << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned short arg;
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
                    case 'u':    // Output an unsigned int.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                                unsigned char arg;
                                arg = va_arg(*argList, unsigned int);
                                if(needsLeftAdjustment)
                                    value << std::left << arg;
                                else
                                    value << arg;
                            }
                            else if(hasHModifier)
                            {
                                unsigned short arg;
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
                    case 'U':    // Output an unsigned long int.
                        if(processingFormatCode)
                        {
                            unsigned long arg;
                            std::stringstream value;

                            arg = va_arg(*argList, unsigned long);

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                    case 'c':    // Output a single character.
                        if(processingFormatCode)
                        {
                            int arg = va_arg(*argList, int);
                            auto c = static_cast<char>(arg);
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                    case 's':    // Output a C-Style string
                        if(processingFormatCode)
                        {
                            char *s = va_arg(*argList, char *);
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                    case 'S':    // Output a std::string
                        if(processingFormatCode)
                        {
                            std::string *s = va_arg(*argList, std::string *);
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
                    case 'f':    // Output a real number with 6 digits of precision.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
                                if(hasPrecision)
                                    value.precision(precision);
                                else
                                    value.precision(6);
                            }
                            else
                            {
                                if(hasPrecision)
                                    value.precision(precision);
                                else
                                    value.precision(6);
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
                                double arg = va_arg(*argList, double);
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
                                double arg = va_arg(*argList, double);
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
                    case 'g':    // Output a real number with up-to 6 digits of precision.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
                                if(hasPrecision)
                                    value.precision(precision);
                                else
                                    value.precision(6);
                            }
                            else
                            {
                                if(hasPrecision)
                                    value.precision(precision);
                                else
                                    value.precision(6);
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
                                double arg = va_arg(*argList, double);
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
                                double arg = va_arg(*argList, double);
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
                    case 'e':    // Output a real number with up-to 6 digits of precision in scientific notation.
                        if(processingFormatCode)
                        {
                            std::stringstream value;

                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
                                if(hasPrecision)
                                    value.precision(precision);
                                else
                                    value.precision(6);
                            }
                            else
                            {
                                if(hasPrecision)
                                    value.precision(precision);
                                else
                                    value.precision(6);
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
                                double arg = va_arg(*argList, double);
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
                                double arg = va_arg(*argList, double);
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
                    case 'p':    // A void pointer.
                        if(processingFormatCode)
                        {
                            auto arg = reinterpret_cast<void *>(va_arg(*argList, void *));
                            std::stringstream value;
                            if(hasFieldWidth)
                            {
                                value.width(fieldWidth);
                                if(needsZeroPadding)
                                    value.fill('0');
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
                    case '@':    // Output a String object.
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
                                    if(needsZeroPadding)
                                        accumulator.fill('0');
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
                    case '0':    // Check to see if we need to turn on needsZeroPadding.
                        if(processingFormatCode)
                        {
                            if(!hasFieldWidth && !processingPrecision)
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
                    case '1':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(1);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '2':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(2);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '3':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(3);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '4':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(4);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '5':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(5);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '6':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(6);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '7':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(7);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '8':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(8);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '9':    // Check to see if we need augment fieldWidth.
                        if(processingFormatCode)
                        {
                            ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(9);
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case '.':    // Check to see if we have a precision setting.
                        if(processingFormatCode)
                        {
                            processingPrecision = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'l':    // Check to see if we need to turn on hasLModifier or hasLLModifier.
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
                    case 'L':    // Check to see if we need to turn on hasCapitalLModifier.
                        if(processingFormatCode)
                        {
                            hasCapitalLModifier = true;
                        }
                        else
                        {
                            accumulator << *fmt;
                        }
                        break;
                    case 'h':    // Check to see if we need to turn on hasHModifier or hasHHModifier.
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
                    case '-':    // Turn on left justification.
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


        String String::initWithASCIIEncodedUnicode(const char *str)
        {
            String theString;

            ASCIIStringEncoder asciiEncoder;
            auto stringLength = std::strlen(str);
            auto charactersInString =
                asciiEncoder.numberOfCharacters(reinterpret_cast<const char_type *>(str), stringLength);

            auto characters = new unicode_point_type[charactersInString];
            auto endian = asciiEncoder.thisSystemEndianness();

            char_type *tmp = reinterpret_cast<char_type *>(const_cast<char *>(str));
            size_type i = 0;

            // Use the ASCII encoder to convert the argument to unicode points.
            while(stringLength > 0)
            {
                auto result = asciiEncoder.nextCodePoint(tmp, stringLength, endian);
                characters[i++] = result.first;
                tmp += result.second;
                stringLength -= result.second;
            }

            UTF8StringEncoder utf8Encoder;

            // Now convert the unicode points to UTF-8
            size_type bytesRequiredForUTF8 = 0;

            for(size_type i = 0; i < charactersInString; ++i)
            {
                bytesRequiredForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(characters[i]);
            }

            auto theArray = new char_type[bytesRequiredForUTF8];
            tmp = theArray;
            auto tmpLength = bytesRequiredForUTF8;

            for(size_type i = 0; i < charactersInString; ++i)
            {
                auto result = utf8Encoder.encodeCodePoint(tmp, tmpLength, characters[i], endian);
                tmp += result;
                tmpLength -= result;
            }

            delete[] characters;

            theString.core = std::make_shared<core_type>(theArray, bytesRequiredForUTF8);

            delete[] theArray;

            return theString;
        }


#if 1    // Disabled for now

        String::iterator String::begin(void)
        {
            std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
            return iterator(encoder, core, 0);
        }


        String::iterator String::begin(void) const
        {
            std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
            return iterator(encoder, core, 0);
        }


        String::iterator String::end(void)
        {
            std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
            return iterator(encoder, core, core->length());
        }


        String::iterator String::end(void) const
        {
            std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
            return iterator(encoder, core, core->length());
        }

#endif    // Disabled iterator methods


        bool String::operator==(const String &s) const
        {
            return *core == *s.core;
        }


        bool String::operator!=(const String &s) const
        {
            if(*this == s)
                return false;
            return true;
        }


        String &String::operator=(const String &s)
        {
            core = std::make_shared<core_type>(*s.core);
            return *this;
        }


        String &String::operator=(String &&s) noexcept
        {
            core = s.core;
            return *this;
        }


        String::size_type String::length() const
        {
            UTF8StringEncoder encoder;
            if(core->length() == 0)
                return 0;
            return encoder.numberOfCharacters(core->data(), core->length());
        }


        String::size_type String::numberOfBytes() const
        {
            return core->length();
        }


        String::unicode_point_type String::operator[](size_type i) const
        {
            UTF8StringEncoder encoder;
            return encoder.unicodeCodePointForCharacterAtIndex(core->data(), core->length(), i);
        }


        String::unicode_point_type String::characterAtIndex(size_type i) const
        {
            UTF8StringEncoder encoder;
            return encoder.unicodeCodePointForCharacterAtIndex(core->data(), core->length(), i);
        }


        String String::getCharactersInRange(const range_type &range) const
        {
            UTF8StringEncoder encoder;

            if(!encoder.doesRangeOfCharactersLieInString(core->data(), core->length(), range))
                throw std::range_error("getCharactersInRange given range outside of string");

            auto numberOfBytesForCharacters =
                encoder.numberOfBytesToCaptureCharactersInRange(core->data(), core->length(), range);

            auto theIndex = encoder.arrayIndexOfCharacterAtCharacterIndex(core->data(), core->length(), range.position);

            auto tmp = core->data();

            tmp += theIndex;

            String s(tmp, numberOfBytesForCharacters);
            return s;
        }


        std::unique_ptr<const char> String::cStr() const
        {
            ASCIIStringEncoder encoder;

            if(core->length() == 0)
                throw std::runtime_error("cStr unable to create string from empty string");

            auto asciiData = convertToThisEncoding(*this, &encoder);

            auto theBytes = new char[asciiData.length() + 1];
            std::memcpy(reinterpret_cast<void *>(theBytes),
                        reinterpret_cast<void *>(const_cast<char *>(asciiData.bytes())),
                        asciiData.length() * sizeof(char));

            theBytes[asciiData.length()] = '\0';

            return std::unique_ptr<const char>(theBytes);
        }


        std::string String::stlString() const
        {
            std::unique_ptr<const char> cStr = this->cStr();
            return std::string(cStr.get());
        }


        String String::stringByAppendingFormat(const char *format, ...) const
        {
            String result;
            va_list argList;
            va_start(argList, format);
            result = stringByAppendingFormat(format, &argList);
            va_end(argList);
            return result;
        }


        String String::stringByAppendingFormat(const char *format, va_list *argList) const
        {
            String formatString;
            String newString;

            formatString = initWithFormat(format, argList);
            newString = concatenateStrings(*this, formatString);

            return newString;
        }


        String String::stringByAppendingString(const String &str) const
        {
            return concatenateStrings(*this, str);
        }


        String String::concatenateStrings(const String &s1, const String &s2)
        {
            auto newArrayLength = s1.core->length() + s2.core->length();

            auto theArray = new char_type[newArrayLength];

            std::memcpy(reinterpret_cast<void *>(theArray), reinterpret_cast<void *>(s1.core->data()),
                        s1.core->length() * sizeof(char_type));

            std::memcpy(reinterpret_cast<void *>(theArray + s1.core->length()),
                        reinterpret_cast<void *>(s2.core->data()), s2.core->length() * sizeof(char));

            String s(theArray, newArrayLength);

            delete[] theArray;

            return s;
        }


        String String::operator+(const String &s) const
        {
            String copy = this->deepCopy();
            return String::initWithFormat("%@%@", &copy, &s);
        }


        String String::operator+(const char c) const
        {
            String copy = this->deepCopy();
            return String::initWithFormat("%@%c", &copy, c);
        }


        String &String::operator+=(const String &s)
        {
            *this = concatenateStrings(*this, s);
            return *this;
        }


        String &String::operator+=(const char c)
        {
            *this = *this + c;
            return *this;
        }


        String String::substringFromIndex(size_type i) const
        {
            UTF8StringEncoder encoder;
            if(i > (this->length() - 1))
                throw std::runtime_error("substringFromIndex index larger than length of string");

            auto index = encoder.arrayIndexOfCharacterAtCharacterIndex(core->data(), core->length(), i);
            auto theLength = core->length() - index;

            String s(core->data() + index, theLength);
            return s;
        }


        String String::substringWithRange(const range_type &range) const
        {
            UTF8StringEncoder encoder;
            if(!encoder.doesRangeOfCharactersLieInString(core->data(), core->length(), range))
                throw std::range_error("substringWithRange given range of characters that lies outside of the string");

            auto bytesNeededForRange =
                encoder.numberOfBytesToCaptureCharactersInRange(core->data(), core->length(), range);

            auto theArray = new char_type[bytesNeededForRange];
            auto startIndex =
                encoder.arrayIndexOfCharacterAtCharacterIndex(core->data(), core->length(), range.position);

            std::memcpy(reinterpret_cast<void *>(theArray), reinterpret_cast<void *>(core->data() + startIndex),
                        (bytesNeededForRange) * sizeof(char_type));

            String s(theArray, bytesNeededForRange);

            delete[] theArray;

            return s;
        }


        String::string_array_type String::substringsNotInRange(const range_type &range) const
        {
            string_array_type substringArray;

            if(range.position > 0)
            {
                range_type theRange(0, range.position);
                String tmpString = this->substringWithRange(theRange);
                substringArray.push_back(tmpString);
            }

            if((range.position + range.length) < this->length())
            {
                size_type theLength = this->length() - (range.position + range.length);
                range_type theRange(range.position + range.length, theLength);
                String tmpString = this->substringWithRange(theRange);
                substringArray.push_back(tmpString);
            }

            return substringArray;
        }


        String String::substringToIndex(size_type i) const
        {
            UTF8StringEncoder encoder;

            if(i > (this->length() - 1))
                throw std::range_error("substringToIndex given index greater than length of string");

            range_type range(0, i);

            auto bytesNeededForRange =
                encoder.numberOfBytesToCaptureCharactersInRange(core->data(), core->length(), range);

            auto theArray = new char_type[bytesNeededForRange];

            for(size_type j = 0; j < bytesNeededForRange; j++)
                *(theArray + j) = *(core->data() + j);

            String s(theArray, bytesNeededForRange);

            delete[] theArray;

            return s;
        }


        String::string_array_type String::substringsThatDoNotMatchString(const String &str) const
        {
            range_array_type rangesOfSubStrings;
            string_array_type theSubStrings;
            UTF8StringEncoder theEncoder;

            rangesOfSubStrings = theEncoder.findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
                core->data(), core->length(), str.core->data(), str.core->length());

            for(auto &theRange : rangesOfSubStrings)
            {
                theSubStrings.push_back(this->substringWithRange(theRange));
            }

            return theSubStrings;
        }


        String::string_array_type String::split(const String &splitString) const
        {
            return this->substringsThatDoNotMatchString(splitString);
        }


        String::range_type String::rangeOfString(const String &str) const
        {
            range_type theRange;
            UTF8StringEncoder theEncoder;

            theRange = theEncoder.findCharacterRangeForSubstringInString(core->data(), core->length(), str.core->data(),
                                                                         str.core->length());

            return theRange;
        }


        String::range_array_type String::rangesOfString(const String &str) const
        {
            range_array_type theRanges;
            UTF8StringEncoder theEncoder;

            theRanges = theEncoder.findCharacterRangesForSubstringInString(core->data(), core->length(),
                                                                           str.core->data(), str.core->length());

            return theRanges;
        }


        String String::stringByReplacingOccurencesOfStringWithString(const String &original,
                                                                     const String &replacement) const
        {
            range_array_type originalStringRanges;
            UTF8StringEncoder theEncoder;

            auto bytesNeededForNewString = theEncoder.computeArraySizeInBytesForStringByReplacingSubstrings(
                core->data(), core->length(), original.core->data(), original.core->length(), replacement.core->data(),
                replacement.core->length(), originalStringRanges);

            auto newString = new char_type[bytesNeededForNewString];

            theEncoder.replaceOccurancesOfStringWithString(core->data(), core->length(), newString,
                                                           bytesNeededForNewString, replacement.core->data(),
                                                           replacement.core->length(), originalStringRanges);

            String theResultString(newString, bytesNeededForNewString);

            delete[] newString;

            return theResultString;
        }


        String String::stringByReplacingCharactersInRangeWithString(const range_type &range, const String &str) const
        {
            UTF8StringEncoder theEncoder;

            if(!theEncoder.doesRangeOfCharactersLieInString(core->data(), core->length(), range))
                throw std::out_of_range("Range argument lies outside range of string.");

            String theNewString;

            size_type totalLength = this->length();

            if(range.position == 0)
            {
                // The string represented by range is at the beginning of this string.
                // Calculate the range of the remaining string.
                range_type newRange(range.length, totalLength - range.length);
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
                range_type remainingRange(remainingLocation, totalLength - remainingLocation);
                String thePrecedingString = this->substringWithRange(precedingRange);
                String theRemainingString = this->substringWithRange(remainingRange);
                theNewString = theNewString.stringByAppendingString(thePrecedingString);
                theNewString = theNewString.stringByAppendingString(str);
                theNewString = theNewString.stringByAppendingString(theRemainingString);
            }

            return theNewString;
        }


        ComparisonResult String::compare(const String &str) const
        {
            UTF8StringEncoder encoder;

            return encoder.compareStrings(core->data(), core->length(), str.core->data(), str.core->length());
        }


        ComparisonResult String::compareRangeWithString(const range_type &range, const String &str) const
        {
            String rangeOfThisString = this->substringWithRange(range);
            return rangeOfThisString.compare(str);
        }


        bool String::hasPrefix(const String &str) const
        {
            if(this->length() < str.length())
                return false;

            range_type initialRangeOfThisString(0, str.length());
            String thePrefixOfThisString = this->substringWithRange(initialRangeOfThisString);

            if(thePrefixOfThisString.compare(str) == OrderedSame)
                return true;

            return false;
        }


        bool String::hasSuffix(const String &str) const
        {
            if(this->length() < str.length())
                return false;

            range_type endRangeOfThisString(this->length() - str.length(), str.length());
            String theSuffixOfThisString = this->substringWithRange(endRangeOfThisString);

            if(theSuffixOfThisString.compare(str) == OrderedSame)
                return true;

            return false;
        }


        bool String::isEqualToString(const String &str) const
        {
            if(this->compare(str) == OrderedSame)
                return true;
            return false;
        }


        String String::capitalizedString() const
        {
            UTF8StringEncoder encoder;

            String theCopy = this->deepCopy();

            if(theCopy.core->length() == 0)
                return theCopy;

            auto tmp = theCopy.core->data();
            auto bytesLeft = theCopy.core->length();
            auto thisEndian = encoder.thisSystemEndianness();

            bool nextCharShouldBeCapital = true;

            while(bytesLeft > 0)
            {
                auto opResult = encoder.nextCodePoint(tmp, bytesLeft, thisEndian);

                if(nextCharShouldBeCapital)
                {
                    if(opResult.first >= 97 && opResult.first <= 122)
                        *tmp -= 32;
                    nextCharShouldBeCapital = false;
                }

                if(opResult.first == 32)
                    nextCharShouldBeCapital = true;
                else if(opResult.first >= 65 && opResult.first <= 90)
                    *tmp += 32;

                tmp += opResult.second;
                bytesLeft -= opResult.second;
            }

            return theCopy;
        }


        String String::lowercaseString() const
        {
            UTF8StringEncoder encoder;

            String theCopy = this->deepCopy();

            if(theCopy.core->length() == 0)
                return theCopy;

            encoder.convertStringCharacters(theCopy.core->data(), theCopy.core->length(), StringCase::LowerCase);

            return theCopy;
        }


        String String::uppercaseString() const
        {
            UTF8StringEncoder encoder;

            String theCopy = this->deepCopy();

            if(theCopy.core->length() == 0)
                return theCopy;

            encoder.convertStringCharacters(theCopy.core->data(), theCopy.core->length(), StringCase::UpperCase);

            return theCopy;
        }


        std::string String::getAsASCIIEncodedSTLString(void) const
        {
            auto data = getAsDataInASCIIEncoding();
            char *newData = new char[data.length() + 1];
            memcpy(newData, data.bytes(), data.length());
            newData[data.length()] = 0x0;
            std::string newString(newData);
            delete[] newData;
            return newString;
        }


        String::data_type String::getAsData() const
        {
            data_type data(reinterpret_cast<char *>(core->data()), core->length());
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
                theNumberOfBytesToEncodeTheSubstringInThisEncoding += encoder->lengthOfByteOrderMarkInBytes();
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

        String::data_type String::getAsDataInASCIIEncoding() const
        {
            ASCIIStringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


        String::data_type String::getAsDataInUTF8Encoding() const
        {
            UTF8StringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


        String::data_type String::getAsDataInUTF16Encoding() const
        {
            UTF16StringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


        String::data_type String::getAsDataInUTF32Encoding(void) const
        {
            UTF32StringEncoder encoder;
            return convertToThisEncoding(*this, &encoder);
        }


#ifdef TFTESTS
        char_type *String::bytes(void)
        {
            return theBytes;
        }


        StringEncoder *String::encoder(void)
        {
            return theEncoder;
        }
#endif /* TFTESTS */


        std::ostream &String::description(std::ostream &o) const
        {
            if(useObjectFormattingOutput)
            {
                ClassFormatter *formatter = FormatterFactory::getFormatter();
                if(formatter != nullptr)
                {
                    formatter->setClassName("String");
                    formatter->addClassMember<core_type>("core", *(core.get()));
                    o << *formatter;
                    delete formatter;
                }
            }
            else
            {
                data_type asciiData = getAsDataInASCIIEncoding();

                // We have the ASCII formatted data, but we need to have it in
                // a form we can write to the stream.

                if(asciiData.length() > 0)
                {
                    auto tmp = new char[asciiData.length() + 1];
                    memcpy(reinterpret_cast<void *>(tmp),
                           reinterpret_cast<void *>(const_cast<char *>(asciiData.bytes())),
                           asciiData.length() * sizeof(char));
                    tmp[asciiData.length()] = '\0';

                    o << tmp;

                    delete[] tmp;
                }
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const String &s)
        {
            return s.description(o);
        }


        ComparisonResult compareStrings(const String &stringOne, const String &stringTwo, void *context)
        {
            return stringOne.compare(stringTwo);
        }


        bool operator==(const char *s, const String &t)
        {
            return t == s;
        }


        bool operator==(const std::string &s, const String &t)
        {
            return t == s;
        }


        bool operator<(const String &a, const String &b)
        {
            ComparisonResult result = compareStrings(a, b, nullptr);
            if(result == OrderedAscending)
                return true;
            return false;
        }


        bool operator>(const String &a, const String &b)
        {
            ComparisonResult result = compareStrings(a, b, nullptr);
            if(result == OrderedDescending)
                return true;
            return false;
        }


        String operator+(const char *a, const String &b)
        {
            String theA(a);
            return theA + b;
        }


        String operator+(const std::string &a, const String &b)
        {
            String theA(a);
            return theA + b;
        }

    }    // namespace Foundation

}    // namespace TF
