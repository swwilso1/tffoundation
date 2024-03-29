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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#define NEEDS_ARRAY
#define NEEDS_CSTRING
#define NEEDS_CSTDARG
#define NEEDS_IOSTREAM
#define NEEDS_SSTREAM
#define NEEDS_STRING
#define NEEDS_STACK
#define NEEDS_STDEXCEPT
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tfstring.hpp"
#include "tfasciistringencoder.hpp"
#include "tfjsonstringencoder.hpp"
#include "tfutf8stringencoder.hpp"
#include "tfutf16stringencoder.hpp"
#include "tfutf32stringencoder.hpp"
#include "tfwindows1252encoder.hpp"

namespace TF::Foundation
{

    bool String::useObjectFormattingOutput = false;

#pragma mark - Constructor methods

    String::String()
    {
        core = std::make_shared<core_type>(0);
    }

    String::String(const String & s)
    {
        core = std::make_shared<core_type>(*s.core);
    }

    String::String(String && s) noexcept
    {
        core = s.core;
    }

    String::String(const char * str)
    {
        static UTF8StringEncoder encoder;
        const auto * tmp = reinterpret_cast<const UTF8StringEncoder::char_type *>(str);
        auto theLength = strlen(str);

        if (! encoder.checkStringForCorrectness(tmp, theLength))
            throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

        core = std::make_shared<core_type>(tmp, theLength);
    }

    String::String(const char * str, size_type length)
    {
        static UTF8StringEncoder encoder;
        const auto * tmp = reinterpret_cast<const UTF8StringEncoder::char_type *>(str);

        if (! encoder.checkStringForCorrectness(tmp, length))
            throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

        core = std::make_shared<core_type>(tmp, length);
    }

    String::String(const std::string & s)
    {
        ASCIIStringEncoder asciiEncoder;
        auto stringLength = s.size();
        auto str = s.c_str();
        auto charactersInString =
            asciiEncoder.numberOfCharacters(reinterpret_cast<const char_type *>(str), stringLength);

        auto characters = std::make_unique<unicode_point_type[]>(charactersInString);
        auto endian = ASCIIStringEncoder::thisSystemEndianness();

        auto * tmp = reinterpret_cast<char_type *>(const_cast<char *>(str));
        size_type i = 0;

        // Use the ASCII encoder to convert the argument to unicode points.
        while (stringLength > 0)
        {
            auto result = asciiEncoder.nextCodePoint(tmp, stringLength, endian);
            characters[i++] = result.first;
            tmp += result.second;
            stringLength -= result.second;
        }

        UTF8StringEncoder utf8Encoder;

        // Now convert the unicode points to UTF-8
        size_type bytesRequiredForUTF8 = 0;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            bytesRequiredForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(characters[j]);
        }

        auto theArray = std::make_unique<char_type[]>(bytesRequiredForUTF8);
        tmp = theArray.get();
        auto tmpLength = bytesRequiredForUTF8;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            auto result = utf8Encoder.encodeCodePoint(tmp, tmpLength, characters[j], endian);
            tmp += result;
            tmpLength -= result;
        }

        core = std::make_shared<core_type>(theArray.get(), bytesRequiredForUTF8);
    }

    String::String(const char8_t * str)
    {
        static UTF8StringEncoder encoder;
        size_type str_length{0};
        auto * tmp = str;
        while (*tmp != 0)
        {
            str_length++;
            tmp++;
        }

        if (! encoder.checkStringForCorrectness(reinterpret_cast<const UTF8StringEncoder::char_type *>(str),
                                                str_length))
        {
            throw std::runtime_error{"String char8_t constructor cannot create string from bad UTF-8"};
        }

        core = std::make_shared<core_type>(reinterpret_cast<const unsigned char *>(str), str_length);
    };

    // The argument should be encoded UTF-8.
    String::String(const unsigned char * str, size_type length)
    {
        static UTF8StringEncoder encoder;

        if (! encoder.checkStringForCorrectness(str, length))
            throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

        core = std::make_shared<core_type>(str, length);
    }

    String::String(const char16_t * str)
    {
        size_type str_length{0};
        auto * tmp = str;
        while (*tmp != 0)
        {
            str_length++;
            tmp++;
        }

        // The str array does not contain a byte order mark.  The initialization code requires that we
        // add a byte order mark otherwise it interprets the code units as big-endian units.
        auto array = std::make_unique<unsigned short[]>(str_length + 1);

        auto first_code = array.get();
        *first_code = 0xFEFF;
        first_code++;

        std::memcpy(first_code, str, str_length * sizeof(unsigned short));

        initFromUTF16(array.get(), str_length + 1);
    }

    // The argument should be encoded UTF-16
    String::String(const unsigned short * str, size_type length)
    {
        initFromUTF16(str, length);
    }

    String::String(const char32_t * str)
    {
        size_type str_length{0};
        auto * tmp = str;
        while (*tmp != 0)
        {
            str_length++;
            tmp++;
        }

        // The str array does not contain a byte order mark.  The initialization code requires that we
        // add a byte order mark otherwise it interprets the code units as big-endian units.
        auto array = std::make_unique<unsigned int[]>(str_length + 1);

        auto first_code = array.get();
        *first_code = 0xFEFF;
        first_code++;
        std::memcpy(first_code, str, str_length * sizeof(unsigned int));

        initFromUTF32(array.get(), str_length + 1);
    }

    String::String(const unsigned int * str, size_type length)
    {
        initFromUTF32(str, length);
    }

    String::String(unsigned int c)
    {
        static UTF8StringEncoder utf8StringEncoder;
        auto bytesNeededForUTF8 = utf8StringEncoder.bytesNeededForRepresentationOfCode(c);

        auto charArray = new char_type[bytesNeededForUTF8];
        utf8StringEncoder.encodeCodePoint(charArray, bytesNeededForUTF8, c, StringEncoder::thisSystemEndianness());

        core = std::make_shared<core_type>(charArray, bytesNeededForUTF8);

        delete[] charArray;
    }

    String::String(const data_type & data)
    {
        static UTF8StringEncoder encoder;

        auto str = reinterpret_cast<const unsigned char *>(data.bytes());

        if (! encoder.checkStringForCorrectness(str, data.length()))
            throw std::runtime_error("String UTF-8 constructor cannot create string from bad UTF-8");

        core = std::make_shared<core_type>(str, data.length());
    }

    String String::deepCopy() const
    {
        if (core->length() == 0)
            return {};

        String s(core->data(), core->length());
        return s;
    }

    auto String::initWithStringView(const std::string_view & sv) -> String
    {
        std::string s{sv};
        return {s};
    }

    String String::initWithFormat(const char * format, ...)
    {
        String result;

        // Set up the variable argument list
        va_list argList;
        va_start(argList, format);
        result = initWithFormat(format, &argList);
        va_end(argList);
        return result;
    }

    String String::initWithFormat(const char * format, va_list * argList)
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

        auto add_value_to_precision_or_fieldwidth = [&processingPrecision, &precision, &hasPrecision, &fieldWidth,
                                                     &hasFieldWidth](int value) {
            if (processingPrecision)
            {
                precision = (precision * 10) + (value);
                if (! hasPrecision)
                    hasPrecision = true;
            }
            else
            {
                fieldWidth = (fieldWidth * 10) + (value);
                if (! hasFieldWidth)
                    hasFieldWidth = true;
            }
        };

        auto clear_control_settings = [&processingFormatCode, &processingPrecision, &needsZeroPadding, &hasLLModifier,
                                       &hasLModifier, &hasCapitalLModifier, &hasHModifier, &hasHHModifier,
                                       &hasFieldWidth, &hasPrecision, &needsLeftAdjustment, &fieldWidth, &precision,
                                       &counter]() {
            processingFormatCode = false;
            processingPrecision = false;
            needsZeroPadding = false;
            hasLLModifier = false;
            hasLModifier = false;
            hasCapitalLModifier = false;
            hasHModifier = false;
            hasHHModifier = false;
            hasFieldWidth = false;
            hasPrecision = false;
            needsLeftAdjustment = false;
            fieldWidth = 0;
            precision = 0;
            counter++;
        };

        // Now parse the format string.   The format string must be
        // a C-Style (ie ends with '\0' (null-terminated)) string.
        // Standard printf style arguments are accepted and one
        // extra form.  %@ now indicates a String argument.

        const char * fmt = format;

        while (*fmt != '\0')
        {

            // The easiest thing to do here will be to use stl strings
            // to collect the format valid characters and the conversion values.
            // At the end we can drop it all into a UTF-16 string.
            switch (*fmt)
            {
                case '%': // Activate code processing mode
                    if (processingFormatCode)
                    {
                        accumulator << *fmt;
                        clear_control_settings();
                    }
                    else
                    {
                        processingFormatCode = true;
                    }
                    break;
                case 'i': // Same as 'd' modifier
                case 'd': // Output a base 10 (decimal) number
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }

                        if (hasLLModifier)
                        {
                            long long arg;
                            arg = va_arg(*argList, long long);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }
                        else if (hasLModifier)
                        {
                            long arg;
                            arg = va_arg(*argList, long);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }
                        else if (hasHHModifier)
                        {
                            char arg;
                            arg = static_cast<char>(va_arg(*argList, int));
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }
                        else if (hasHModifier)
                        {
                            short arg;

                            arg = static_cast<short>(va_arg(*argList, int));
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }
                        else
                        {
                            int arg;
                            arg = va_arg(*argList, int);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'o': // Output an octal number
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }

                        if (hasLLModifier)
                        {
                            unsigned long long arg;
                            arg = va_arg(*argList, unsigned long long);
                            if (needsLeftAdjustment)
                                value << std::left << std::oct << arg;
                            else
                                value << std::oct << arg;
                        }
                        else if (hasLModifier)
                        {
                            unsigned long arg;
                            arg = va_arg(*argList, unsigned long);
                            if (needsLeftAdjustment)
                                value << std::left << std::oct << arg;
                            else
                                value << std::oct << arg;
                        }
                        else if (hasHHModifier)
                        {
                            unsigned char arg;
                            arg = static_cast<unsigned char>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << std::oct << arg;
                            else
                                value << std::oct << arg;
                        }
                        else if (hasHModifier)
                        {
                            unsigned short arg;
                            arg = static_cast<unsigned short>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << std::oct << arg;
                            else
                                value << std::oct << arg;
                        }
                        else
                        {
                            int arg;
                            arg = va_arg(*argList, int);
                            if (needsLeftAdjustment)
                                value << std::left << std::oct << arg;
                            else
                                value << std::oct << arg;
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'x': // Output a hexidecimal number in lower-case
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }

                        if (hasLLModifier)
                        {
                            unsigned long long arg;
                            arg = va_arg(*argList, unsigned long long);
                            if (needsLeftAdjustment)
                                value << std::left << std::nouppercase << std::hex << arg;
                            else
                                value << std::nouppercase << std::hex << arg;
                        }
                        else if (hasLModifier)
                        {
                            unsigned long arg;
                            arg = va_arg(*argList, unsigned long);
                            if (needsLeftAdjustment)
                                value << std::left << std::nouppercase << std::hex << arg;
                            else
                                value << std::nouppercase << std::hex << arg;
                        }
                        else if (hasHHModifier)
                        {
                            unsigned char arg;
                            arg = static_cast<unsigned char>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << std::nouppercase << std::hex << arg;
                            else
                                value << std::nouppercase << std::hex << arg;
                        }
                        else if (hasHModifier)
                        {
                            unsigned short arg;
                            arg = static_cast<unsigned short>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << std::nouppercase << std::hex << arg;
                            else
                                value << std::nouppercase << std::hex << arg;
                        }
                        else
                        {
                            int arg;
                            arg = va_arg(*argList, int);
                            if (needsLeftAdjustment)
                                value << std::left << std::nouppercase << std::hex << arg;
                            else
                                value << std::nouppercase << std::hex << arg;
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'X': // Output a hexidecimal number in upper-case
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }

                        if (hasLLModifier)
                        {
                            unsigned long long arg;
                            arg = va_arg(*argList, unsigned long long);
                            if (needsLeftAdjustment)
                                value << std::left << std::uppercase << std::hex << arg;
                            else
                                value << std::uppercase << std::hex << arg;
                        }
                        else if (hasLModifier)
                        {
                            unsigned long arg;
                            arg = va_arg(*argList, unsigned long);
                            if (needsLeftAdjustment)
                                value << std::left << std::uppercase << std::hex << arg;
                            else
                                value << std::uppercase << std::hex << arg;
                        }
                        else if (hasHHModifier)
                        {
                            unsigned char arg;
                            arg = static_cast<unsigned char>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << std::uppercase << std::hex << arg;
                            else
                                value << std::uppercase << std::hex << arg;
                        }
                        else if (hasHModifier)
                        {
                            unsigned short arg;
                            arg = static_cast<unsigned short>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << std::uppercase << std::hex << arg;
                            else
                                value << std::uppercase << std::hex << arg;
                        }
                        else
                        {
                            unsigned int arg;
                            arg = va_arg(*argList, unsigned int);
                            if (needsLeftAdjustment)
                                value << std::left << std::uppercase << std::hex << arg;
                            else
                                value << std::uppercase << std::hex << arg;
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'u': // Output an unsigned int.
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }

                        if (hasLLModifier)
                        {
                            unsigned long long arg;
                            arg = va_arg(*argList, unsigned long long);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        else if (hasLModifier)
                        {
                            unsigned long arg;
                            arg = va_arg(*argList, unsigned long);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        else if (hasHHModifier)
                        {
                            unsigned char arg;
                            arg = static_cast<unsigned char>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        else if (hasHModifier)
                        {
                            unsigned short arg;
                            arg = static_cast<unsigned short>(va_arg(*argList, unsigned int));
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        else
                        {
                            unsigned int arg;
                            arg = va_arg(*argList, unsigned int);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'U': // Output an unsigned long int.
                    if (processingFormatCode)
                    {
                        unsigned long arg;
                        std::stringstream value;

                        arg = va_arg(*argList, unsigned long);

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        else
                        {
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'c': // Output a single character.
                    if (processingFormatCode)
                    {
                        int arg = va_arg(*argList, int);
                        auto c = static_cast<char>(arg);
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                            if (needsLeftAdjustment)
                                value << std::left << c;
                            else
                                value << c;
                        }
                        else
                        {
                            if (needsLeftAdjustment)
                                value << std::left << c;
                            else
                                value << c;
                        }
                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 's': // Output a C-Style string
                    if (processingFormatCode)
                    {
                        char * s = va_arg(*argList, char *);
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }

                        if (needsLeftAdjustment)
                            value << std::left << s;
                        else
                            value << s;
                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'S': // Output a std::string
                    if (processingFormatCode)
                    {
                        std::string * s = va_arg(*argList, std::string *);
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                        }
                        if (s != nullptr)
                        {
                            if (needsLeftAdjustment)
                                value << std::left << *s;
                            else
                                value << *s;
                        }
                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'f': // Output a real number with 6 digits of precision.
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                            if (hasPrecision)
                                value.precision(precision);
                            else
                                value.precision(6);
                        }
                        else
                        {
                            if (hasPrecision)
                                value.precision(precision);
                            else
                                value.precision(6);
                        }

                        if (hasCapitalLModifier)
                        {
                            long double arg = va_arg(*argList, long double);
                            if (needsLeftAdjustment)
                                value << std::left << std::fixed << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << std::fixed << arg;
                                else
                                    value << std::fixed << arg;
                            }
                        }
                        else if (hasLModifier)
                        {
                            double arg = va_arg(*argList, double);
                            if (needsLeftAdjustment)
                                value << std::left << std::fixed << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << std::fixed << arg;
                                else
                                    value << std::fixed << arg;
                            }
                        }
                        else
                        {
                            double arg = va_arg(*argList, double);
                            if (needsLeftAdjustment)
                                value << std::left << std::fixed << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << std::fixed << arg;
                                else
                                    value << std::fixed << arg;
                            }
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'g': // Output a real number with up-to 6 digits of precision.
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                            if (hasPrecision)
                                value.precision(precision);
                            else
                                value.precision(6);
                        }
                        else
                        {
                            if (hasPrecision)
                                value.precision(precision);
                            else
                                value.precision(6);
                        }

                        if (hasCapitalLModifier)
                        {
                            long double arg = va_arg(*argList, long double);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }
                        else if (hasLModifier)
                        {
                            double arg = va_arg(*argList, double);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }
                        else
                        {
                            double arg = va_arg(*argList, double);
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << arg;
                                else
                                    value << arg;
                            }
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'e': // Output a real number with up-to 6 digits of precision in scientific notation.
                    if (processingFormatCode)
                    {
                        std::stringstream value;

                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                            if (hasPrecision)
                                value.precision(precision);
                            else
                                value.precision(6);
                        }
                        else
                        {
                            if (hasPrecision)
                                value.precision(precision);
                            else
                                value.precision(6);
                        }

                        if (hasCapitalLModifier)
                        {
                            long double arg = va_arg(*argList, long double);
                            if (needsLeftAdjustment)
                                value << std::left << std::scientific << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << std::scientific << arg;
                                else
                                    value << std::scientific << arg;
                            }
                        }
                        else if (hasLModifier)
                        {
                            double arg = va_arg(*argList, double);
                            if (needsLeftAdjustment)
                                value << std::left << std::scientific << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << std::scientific << arg;
                                else
                                    value << std::scientific << arg;
                            }
                        }
                        else
                        {
                            double arg = va_arg(*argList, double);
                            if (needsLeftAdjustment)
                                value << std::left << std::scientific << arg;
                            else
                            {
                                if (arg < 0 && needsZeroPadding)
                                    value << std::internal << std::scientific << arg;
                                else
                                    value << std::scientific << arg;
                            }
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'p': // A void pointer.
                    if (processingFormatCode)
                    {
                        auto arg = reinterpret_cast<void *>(va_arg(*argList, void *));
                        std::stringstream value;
                        if (hasFieldWidth)
                        {
                            value.width(fieldWidth);
                            if (needsZeroPadding)
                                value.fill('0');
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        else
                        {
                            if (needsLeftAdjustment)
                                value << std::left << arg;
                            else
                                value << arg;
                        }
                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '@': // Output a String object.
                    if (processingFormatCode)
                    {

                        String * arg = va_arg(*argList, String *);
                        std::stringstream value;

                        if (needsLeftAdjustment)
                            value << std::left << *arg;
                        else
                            value << *arg;

                        if (hasFieldWidth)
                        {
                            if (fieldWidth > static_cast<decltype(fieldWidth)>(value.str().size()))
                            {
                                accumulator.width(fieldWidth);
                                if (needsZeroPadding)
                                    accumulator.fill('0');
                            }
                        }

                        accumulator << value.str();

                        clear_control_settings();
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '0': // Check to see if we need to turn on needsZeroPadding.
                    if (processingFormatCode)
                    {
                        if (! hasFieldWidth && ! processingPrecision)
                        {
                            needsZeroPadding = true;
                        }
                        else
                        {
                            add_value_to_precision_or_fieldwidth(0);
                        }
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '1': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(1);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '2': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(2);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '3': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(3);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '4': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(4);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '5': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(5);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '6': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(6);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '7': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(7);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '8': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(8);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '9': // Check to see if we need augment fieldWidth.
                    if (processingFormatCode)
                    {
                        add_value_to_precision_or_fieldwidth(9);
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case '.': // Check to see if we have a precision setting.
                    if (processingFormatCode)
                    {
                        processingPrecision = true;
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'l': // Check to see if we need to turn on hasLModifier or hasLLModifier.
                    if (processingFormatCode)
                    {
                        if (hasLModifier)
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
                    if (processingFormatCode)
                    {
                        hasCapitalLModifier = true;
                    }
                    else
                    {
                        accumulator << *fmt;
                    }
                    break;
                case 'h': // Check to see if we need to turn on hasHModifier or hasHHModifier.
                    if (processingFormatCode)
                    {
                        if (hasHModifier)
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
                    if (processingFormatCode)
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

        return {accumulator.str().c_str()};
    }

    String String::initWithASCIIEncodedUnicode(const char * str)
    {
        String theString;

        static ASCIIStringEncoder asciiEncoder;
        auto stringLength = std::strlen(str);
        auto charactersInString =
            asciiEncoder.numberOfCharacters(reinterpret_cast<const char_type *>(str), stringLength);

        auto characters = std::make_unique<unicode_point_type[]>(charactersInString);
        auto endian = ASCIIStringEncoder::thisSystemEndianness();

        auto * tmp = reinterpret_cast<char_type *>(const_cast<char *>(str));
        size_type i = 0;

        // Use the ASCII encoder to convert the argument to unicode points.
        while (stringLength > 0)
        {
            auto result = asciiEncoder.nextCodePoint(tmp, stringLength, endian);
            characters[i++] = result.first;
            tmp += result.second;
            stringLength -= result.second;
        }

        static UTF8StringEncoder utf8Encoder;

        // Now convert the unicode points to UTF-8
        size_type bytesRequiredForUTF8 = 0;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            bytesRequiredForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(characters[j]);
        }

        auto theArray = std::make_unique<char_type[]>(bytesRequiredForUTF8);
        tmp = theArray.get();
        auto tmpLength = bytesRequiredForUTF8;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            auto result = utf8Encoder.encodeCodePoint(tmp, tmpLength, characters[j], endian);
            tmp += result;
            tmpLength -= result;
        }

        theString.core = std::make_shared<core_type>(theArray.get(), bytesRequiredForUTF8);

        return theString;
    }

    String String::initWithJSONEncodedUnicode(const char * str)
    {
        return initWithJSONEncodedUnicode(str, std::strlen(str));
    }

    String String::initWithJSONEncodedUnicode(const char * str, size_type length)
    {
        String theString;

        static JSONStringEncoder jsonEncoder;
        auto charactersInString = jsonEncoder.numberOfCharacters(reinterpret_cast<const char_type *>(str), length);

        auto characters = std::make_unique<unicode_point_type[]>(charactersInString);
        auto endian = JSONStringEncoder::thisSystemEndianness();

        auto * tmp = reinterpret_cast<char_type *>(const_cast<char *>(str));
        size_type i = 0;

        // Use the ASCII encoder to convert the argument to unicode points.
        while (length > 0)
        {
            auto result = jsonEncoder.nextCodePoint(tmp, length, endian);
            characters[i++] = result.first;
            tmp += result.second;
            length -= result.second;
        }

        static UTF8StringEncoder utf8Encoder;

        // Now convert the unicode points to UTF-8
        size_type bytesRequiredForUTF8 = 0;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            bytesRequiredForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(characters[j]);
        }

        auto theArray = std::make_unique<char_type[]>(bytesRequiredForUTF8);
        tmp = theArray.get();
        auto tmpLength = bytesRequiredForUTF8;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            auto result = utf8Encoder.encodeCodePoint(tmp, tmpLength, characters[j], endian);
            tmp += result;
            tmpLength -= result;
        }

        theString.core = std::make_shared<core_type>(theArray.get(), bytesRequiredForUTF8);

        return theString;
    }

    String String::initWithWindows1252(const unsigned char * str, size_type length)
    {
        String theString;

        static Windows1252StringEncoder windows1252Encoder;
        auto charactersInString =
            windows1252Encoder.numberOfCharacters(reinterpret_cast<const char_type *>(str), length);

        auto characters = std::make_unique<unicode_point_type[]>(charactersInString);
        auto endian = Windows1252StringEncoder::thisSystemEndianness();

        auto * tmp = reinterpret_cast<char_type *>(const_cast<unsigned char *>(str));
        size_type i = 0;

        // Use the Windows 1252 encoder to convert the argument to unicode points.
        while (length > 0)
        {
            auto result = windows1252Encoder.nextCodePoint(tmp, length, endian);
            characters[i++] = result.first;
            tmp += result.second;
            length -= result.second;
        }

        static UTF8StringEncoder utf8Encoder;

        // Now convert the unicode points to UTF-8
        size_type bytesRequiredForUTF8 = 0;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            bytesRequiredForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(characters[j]);
        }

        auto theArray = std::make_unique<char_type[]>(bytesRequiredForUTF8);
        tmp = theArray.get();
        auto tmpLength = bytesRequiredForUTF8;

        for (size_type j = 0; j < charactersInString; ++j)
        {
            auto result = utf8Encoder.encodeCodePoint(tmp, tmpLength, characters[j], endian);
            tmp += result;
            tmpLength -= result;
        }

        theString.core = std::make_shared<core_type>(theArray.get(), bytesRequiredForUTF8);

        return theString;
    }

#if 1 // Disabled for now

    String::iterator String::begin()
    {
        std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
        return {encoder, core, 0};
    }

    String::iterator String::begin() const
    {
        std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
        return {encoder, core, 0};
    }

    String::iterator String::end()
    {
        std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
        return {encoder, core, core->length()};
    }

    String::iterator String::end() const
    {
        std::shared_ptr<encoder_type> encoder = std::shared_ptr<encoder_type>(new UTF8StringEncoder);
        return {encoder, core, core->length()};
    }

#endif // Disabled iterator methods

    bool String::operator==(const String & s) const
    {
        return *core == *s.core;
    }

    bool String::operator!=(const String & s) const
    {
        if (*this == s)
            return false;
        return true;
    }

    String & String::operator=(const String & s)
    {
        if (this == &s)
        {
            return *this;
        }
        core = std::make_shared<core_type>(*s.core);
        return *this;
    }

    String & String::operator=(String && s) noexcept
    {
        core = s.core;
        return *this;
    }

    String::size_type String::length() const
    {
        if (core->length() == 0)
            return 0;

        auto number_of_chars = core->get_number_of_characters();
        if (number_of_chars == 0)
        {
            static UTF8StringEncoder encoder;
            number_of_chars = encoder.numberOfCharacters(core->data(), core->length());
            core->set_number_of_characters(number_of_chars);
        }
        return number_of_chars;
    }

    auto String::empty() const -> bool
    {
        if (core->length() == 0)
        {
            return true;
        }
        return false;
    }

    String::size_type String::numberOfBytes() const
    {
        return core->length();
    }

    String::unicode_point_type String::operator[](size_type i) const
    {
        static UTF8StringEncoder encoder;
        return encoder.unicodeCodePointForCharacterAtIndex(core->data(), core->length(), i);
    }

    String::unicode_point_type String::characterAtIndex(size_type i) const
    {
        static UTF8StringEncoder encoder;
        return encoder.unicodeCodePointForCharacterAtIndex(core->data(), core->length(), i);
    }

    auto String::first() const -> unicode_point_type
    {
        return characterAtIndex(0);
    }

    auto String::last() const -> unicode_point_type
    {
        auto my_length = length();
        if (my_length == 0)
        {
            throw std::runtime_error{"String contains no characters"};
        }
        return characterAtIndex(my_length - 1);
    }

    String String::getCharactersInRange(const range_type & range) const
    {
        static UTF8StringEncoder encoder;

        if (! encoder.doesRangeOfCharactersLieInString(core->data(), core->length(), range))
            throw std::range_error("getCharactersInRange given range outside of string");

        auto numberOfBytesForCharacters =
            encoder.numberOfBytesToCaptureCharactersInRange(core->data(), core->length(), range);

        auto theIndex = encoder.arrayIndexOfCharacterAtCharacterIndex(core->data(), core->length(), range.position);

        auto tmp = core->data();

        tmp += theIndex;

        String s(tmp, numberOfBytesForCharacters);
        return s;
    }

    std::unique_ptr<char[]> String::cStr() const
    {
        if (core->length() == 0)
        {
            throw std::runtime_error{"cStr unable to convert an empty string"};
        }

        auto the_ptr = std::unique_ptr<char[]>(new char[core->length() + 1]);
        auto start_of_bytes = the_ptr.get();
        std::memcpy(start_of_bytes, core->data(), core->length());
        *(start_of_bytes + core->length()) = '\0';
        return the_ptr;
#if 0
            auto the_bytes = new char[core->length() + 1];
            std::memcpy(the_bytes, core->data(), core->length());
            the_bytes[core->length()] = '\0';
            return std::unique_ptr<const char>(the_bytes);
#endif
    }

    std::string String::stlString() const
    {
        auto ascii_encoded_data = getAsDataInASCIIEncoding();
        return std::string{ascii_encoded_data.bytes(), ascii_encoded_data.length()};
    }

    auto String::stlStringInUTF8() const -> std::string
    {
        auto utf8_encoded_data = getAsDataInUTF8Encoding();
        return std::string{utf8_encoded_data.bytes(), utf8_encoded_data.length()};
    }

    String String::stringByAppendingFormat(const char * format, ...) const
    {
        String result;
        va_list argList;
        va_start(argList, format);
        result = stringByAppendingFormat(format, &argList);
        va_end(argList);
        return result;
    }

    String String::stringByAppendingFormat(const char * format, va_list * argList) const
    {
        String formatString;
        String newString;

        formatString = initWithFormat(format, argList);
        newString = concatenateStrings(*this, formatString);

        return newString;
    }

    String String::stringByAppendingString(const String & str) const
    {
        return concatenateStrings(*this, str);
    }

    String String::concatenateStrings(const String & s1, const String & s2)
    {
        auto newArrayLength = s1.core->length() + s2.core->length();

        auto theArray = new char_type[newArrayLength];

        std::memcpy(reinterpret_cast<void *>(theArray), reinterpret_cast<void *>(s1.core->data()),
                    s1.core->length() * sizeof(char_type));

        std::memcpy(reinterpret_cast<void *>(theArray + s1.core->length()), reinterpret_cast<void *>(s2.core->data()),
                    s2.core->length() * sizeof(char));

        String s(theArray, newArrayLength);

        delete[] theArray;

        return s;
    }

    String String::operator+(const String & s) const
    {
        return String::initWithFormat("%@%@", this, &s);
    }

    String String::operator+(const char c) const
    {
        return String::initWithFormat("%@%c", this, c);
    }

    String String::operator+(const unicode_point_type c) const
    {
        String s{c};
        return *this + s;
    }

    String & String::operator+=(const String & s)
    {
        *this = concatenateStrings(*this, s);
        return *this;
    }

    String & String::operator+=(const char c)
    {
        *this = *this + c;
        return *this;
    }

    String & String::operator+=(const unicode_point_type c)
    {
        *this = *this + c;
        return *this;
    }

    String String::substringFromIndex(size_type i) const
    {
        static UTF8StringEncoder encoder;
        if (i > (this->length() - 1))
            throw std::runtime_error("substringFromIndex index larger than length of string");

        auto index = encoder.arrayIndexOfCharacterAtCharacterIndex(core->data(), core->length(), i);
        auto theLength = core->length() - index;

        String s(core->data() + index, theLength);
        return s;
    }

    String String::substringWithRange(const range_type & range) const
    {
        static UTF8StringEncoder encoder;
        if (! encoder.doesRangeOfCharactersLieInString(core->data(), core->length(), range))
            throw std::range_error("substringWithRange given range of characters that lies outside of the string");

        auto bytesNeededForRange = encoder.numberOfBytesToCaptureCharactersInRange(core->data(), core->length(), range);
        if (bytesNeededForRange == 0)
        {
            // We are handling a case where range.length might be 0.  Ie we have a valid location, but only want
            // 0 parts of the original string.
            return {};
        }

        auto theArray = new char_type[bytesNeededForRange];
        auto startIndex = encoder.arrayIndexOfCharacterAtCharacterIndex(core->data(), core->length(), range.position);

        std::memcpy(reinterpret_cast<void *>(theArray), reinterpret_cast<void *>(core->data() + startIndex),
                    (bytesNeededForRange) * sizeof(char_type));

        String s(theArray, bytesNeededForRange);

        delete[] theArray;

        return s;
    }

    String::string_array_type String::substringsNotInRange(const range_type & range) const
    {
        string_array_type substringArray;

        if (range.position > 0)
        {
            range_type theRange(0, range.position);
            String tmpString = this->substringWithRange(theRange);
            substringArray.push_back(tmpString);
        }

        if ((range.position + range.length) < this->length())
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
        static UTF8StringEncoder encoder;

        if (i > (this->length() - 1))
            throw std::range_error("substringToIndex given index greater than length of string");

        range_type range(0, i);

        auto bytesNeededForRange = encoder.numberOfBytesToCaptureCharactersInRange(core->data(), core->length(), range);

        auto theArray = new char_type[bytesNeededForRange];

        for (size_type j = 0; j < bytesNeededForRange; j++)
            *(theArray + j) = *(core->data() + j);

        String s(theArray, bytesNeededForRange);

        delete[] theArray;

        return s;
    }

    String::string_array_type String::substringsThatDoNotMatchString(const String & str) const
    {
        range_array_type rangesOfSubStrings;
        string_array_type theSubStrings;
        static UTF8StringEncoder theEncoder;

        rangesOfSubStrings = theEncoder.findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
            core->data(), core->length(), str.core->data(), str.core->length());

        for (auto & theRange : rangesOfSubStrings)
        {
            theSubStrings.push_back(this->substringWithRange(theRange));
        }

        return theSubStrings;
    }

    String::string_array_type String::split(const String & splitString) const
    {
        return this->substringsThatDoNotMatchString(splitString);
    }

    String::range_type String::rangeOfString(const String & str) const
    {
        range_type theRange;
        static UTF8StringEncoder theEncoder;

        theRange = theEncoder.findCharacterRangeForSubstringInString(core->data(), core->length(), str.core->data(),
                                                                     str.core->length());

        return theRange;
    }

    String::range_array_type String::rangesOfString(const String & str) const
    {
        range_array_type theRanges;
        static UTF8StringEncoder theEncoder;

        theRanges = theEncoder.findCharacterRangesForSubstringInString(core->data(), core->length(), str.core->data(),
                                                                       str.core->length());

        return theRanges;
    }

    auto String::contains(const String & str) const -> bool
    {
        auto rng = rangeOfString(str);
        return rng.length != 0;
    }

    auto String::startsWith(const String & str) const -> bool
    {
        auto s_length = str.length();
        auto my_length = length();
        if (s_length > my_length)
        {
            return false;
        }

        if (s_length == 0)
        {
            return false;
        }

        if (s_length == my_length)
        {
            return *this == str;
        }

        auto sub = substringToIndex(s_length);

        return sub == str;
    }

    auto String::endsWith(const String & str) const -> bool
    {
        if (str.length() > length())
        {
            return false;
        }

        if (str.length() == 0)
        {
            return false;
        }

        auto sub = substringFromIndex(length() - str.length());

        return sub == str;
    }

    void String::trim()
    {
        if (empty())
        {
            return;
        }

        String return_string{*this};
        range_type characters_at_beginning{};
        range_type characters_at_end{};

        // Check the first part of the string to see if there are any space characters at the beginning of the string.
        if (return_string.first() == ' ')
        {
            characters_at_beginning.position = 0;
            size_type i{0};
            for (; i < return_string.length(); i++)
            {
                if (return_string.characterAtIndex(i) != ' ')
                {
                    break;
                }
            }

            characters_at_beginning.length = i;

            auto substrings = substringsNotInRange(characters_at_beginning);

            if (! substrings.empty())
            {
                return_string = substrings[0];
            }
            else if (i == return_string.length())
            {
                return_string = String{};
            }
        }

        if (return_string.empty())
        {
            *this = return_string;
            return;
        }

        if (return_string.last() == ' ')
        {
            size_type i = 0;
            size_type j = return_string.length() - 1;
            for (; j >= 1; j--, i++)
            {
                if (return_string.characterAtIndex(j) != ' ')
                {
                    break;
                }
            }

            // We know that return_string[0] is not ' ' from the test above.
            j++;

            characters_at_end.position = j;
            characters_at_end.length = i;

            auto substrings = return_string.substringsNotInRange(characters_at_end);
            return_string = substrings[0];
        }

        *this = return_string;
    }

    String String::stringByReplacingOccurrencesOfStringWithString(const String & original,
                                                                  const String & replacement) const
    {
        range_array_type originalStringRanges;
        static UTF8StringEncoder theEncoder;

        auto bytesNeededForNewString = theEncoder.computeArraySizeInBytesForStringByReplacingSubstrings(
            core->data(), core->length(), original.core->data(), original.core->length(), replacement.core->data(),
            replacement.core->length(), originalStringRanges);

        auto newString = new char_type[bytesNeededForNewString];

        theEncoder.replaceOccurrencesOfStringWithString(core->data(), core->length(), newString,
                                                        bytesNeededForNewString, replacement.core->data(),
                                                        replacement.core->length(), originalStringRanges);

        String theResultString(newString, bytesNeededForNewString);

        delete[] newString;

        return theResultString;
    }

    String String::stringByReplacingCharactersInRangeWithString(const range_type & range, const String & str) const
    {
        static UTF8StringEncoder theEncoder;

        if (! theEncoder.doesRangeOfCharactersLieInString(core->data(), core->length(), range))
            throw std::out_of_range("Range argument lies outside range of string.");

        String theNewString;

        size_type totalLength = this->length();

        if (range.position == 0)
        {
            // The string represented by range is at the beginning of this string.
            // Calculate the range of the remaining string.
            range_type newRange(range.length, totalLength - range.length);
            String theRemainingString = this->substringWithRange(newRange);
            theNewString = theNewString.stringByAppendingString(str);
            theNewString = theNewString.stringByAppendingString(theRemainingString);
        }
        else if (range.position == (totalLength - range.length - 1))
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
            // either edge.  Calculate the preceding and remaining substrings.
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

    ComparisonResult String::compare(const String & str) const
    {
        static UTF8StringEncoder encoder;

        return encoder.compareStrings(core->data(), core->length(), str.core->data(), str.core->length());
    }

    ComparisonResult String::compareRangeWithString(const range_type & range, const String & str) const
    {
        String rangeOfThisString = this->substringWithRange(range);
        return rangeOfThisString.compare(str);
    }

    bool String::hasPrefix(const String & str) const
    {
        if (this->length() < str.length())
            return false;

        range_type initialRangeOfThisString(0, str.length());
        String thePrefixOfThisString = this->substringWithRange(initialRangeOfThisString);

        if (thePrefixOfThisString.compare(str) == OrderedSame)
            return true;

        return false;
    }

    bool String::hasSuffix(const String & str) const
    {
        if (this->length() < str.length())
            return false;

        range_type endRangeOfThisString(this->length() - str.length(), str.length());
        String theSuffixOfThisString = this->substringWithRange(endRangeOfThisString);

        if (theSuffixOfThisString.compare(str) == OrderedSame)
            return true;

        return false;
    }

    bool String::isEqualToString(const String & str) const
    {
        if (this->compare(str) == OrderedSame)
            return true;
        return false;
    }

    String String::capitalizedString() const
    {
        static UTF8StringEncoder encoder;

        String theCopy = this->deepCopy();

        if (theCopy.core->length() == 0)
            return theCopy;

        auto tmp = theCopy.core->data();
        auto bytesLeft = theCopy.core->length();
        auto thisEndian = UTF8StringEncoder::thisSystemEndianness();

        bool nextCharShouldBeCapital = true;

        while (bytesLeft > 0)
        {
            auto opResult = encoder.nextCodePoint(tmp, bytesLeft, thisEndian);

            if (nextCharShouldBeCapital)
            {
                if (opResult.first >= 97 && opResult.first <= 122)
                    *tmp -= 32;
                nextCharShouldBeCapital = false;
            }

            if (opResult.first == 32)
                nextCharShouldBeCapital = true;
            else if (opResult.first >= 65 && opResult.first <= 90)
                *tmp += 32;

            tmp += opResult.second;
            bytesLeft -= opResult.second;
        }

        return theCopy;
    }

    String String::lowercaseString() const
    {
        static UTF8StringEncoder encoder;

        String theCopy = this->deepCopy();

        if (theCopy.core->length() == 0)
            return theCopy;

        encoder.convertStringCharacters(theCopy.core->data(), theCopy.core->length(), StringCase::LowerCase);

        return theCopy;
    }

    String String::uppercaseString() const
    {
        static UTF8StringEncoder encoder;

        String theCopy = this->deepCopy();

        if (theCopy.core->length() == 0)
            return theCopy;

        encoder.convertStringCharacters(theCopy.core->data(), theCopy.core->length(), StringCase::UpperCase);

        return theCopy;
    }

    std::string String::getAsASCIIEncodedSTLString() const
    {
        auto data = getAsDataInASCIIEncoding();
        char * newData = new char[data.length() + 1];
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

    String::data_type String::convertToThisEncoding(const String & s, encoder_type * encoder)
    {
        size_type theNumberOfBytesToEncodeTheSubstringInThisEncoding = 0;
        iterator theIterator;
        unicode_point_type theCode;

        for (theIterator = s.begin(); theIterator != s.end(); theIterator++)
        {
            theCode = *theIterator;
            theNumberOfBytesToEncodeTheSubstringInThisEncoding += encoder->bytesNeededForRepresentationOfCode(theCode);
        }

        if (encoder->usesByteOrderMark())
        {
            theNumberOfBytesToEncodeTheSubstringInThisEncoding += encoder->lengthOfByteOrderMarkInBytes();
        }

        auto newSubstringBytes = new unsigned char[theNumberOfBytesToEncodeTheSubstringInThisEncoding];
        auto tmp = newSubstringBytes;
        auto bytesLeftInTheNewString = theNumberOfBytesToEncodeTheSubstringInThisEncoding;

        if (encoder->usesByteOrderMark())
        {
            encoder->writeByteOrderMark(tmp, bytesLeftInTheNewString);
            tmp += encoder->lengthOfByteOrderMarkInBytes();
            bytesLeftInTheNewString -= encoder->lengthOfByteOrderMarkInBytes();
        }

        size_type bytesUsedToEncodeTheCode;
        Endian thisEndian = StringEncoder::thisSystemEndianness();

        for (theIterator = s.begin(); theIterator != s.end(); theIterator++)
        {
            theCode = *theIterator;
            bytesUsedToEncodeTheCode = encoder->encodeCodePoint(tmp, bytesLeftInTheNewString, theCode, thisEndian);
            tmp += bytesUsedToEncodeTheCode;
            bytesLeftInTheNewString -= bytesUsedToEncodeTheCode;
        }

        data_type data(reinterpret_cast<char *>(newSubstringBytes), theNumberOfBytesToEncodeTheSubstringInThisEncoding);

        delete[] newSubstringBytes;

        return data;
    }

    String::data_type String::getAsDataInASCIIEncoding() const
    {
        static ASCIIStringEncoder encoder;
        return convertToThisEncoding(*this, &encoder);
    }

    String::data_type String::getAsDataInJSONEncoding() const
    {
        static JSONStringEncoder encoder;
        return convertToThisEncoding(*this, &encoder);
    }

    String::data_type String::getAsDataInUTF8Encoding() const
    {
        data_type data{};
        data.append(reinterpret_cast<const char *>(core->data()), core->length());
        return data;
    }

    String::data_type String::getAsDataInUTF16Encoding() const
    {
        static UTF16StringEncoder encoder;
        return convertToThisEncoding(*this, &encoder);
    }

    String::data_type String::getAsDataInUTF32Encoding() const
    {
        static UTF32StringEncoder encoder;
        return convertToThisEncoding(*this, &encoder);
    }

    auto String::getEncoder() -> StringEncoder &
    {
        static UTF8StringEncoder encoder;
        return encoder;
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

    std::ostream & String::description(std::ostream & o) const
    {
        if (useObjectFormattingOutput)
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("String");
                formatter->addClassMember<core_type>("core", *(core.get()));
                o << *formatter;
                delete formatter;
            }
        }
        else
        {
            auto data = core->data();
            auto size = core->length();
            for (decltype(size) i = 0; i < size; i++)
            {
                o << static_cast<unsigned char>(*(data + i));
            }
        }
        return o;
    }

    void String::initFromUTF16(const unsigned short * str, size_type length)
    {
        static UTF16StringEncoder encoder;
        static UTF8StringEncoder utf8Encoder;
        auto tmp = reinterpret_cast<const unsigned char *>(str);
        size_type byteLength = length * sizeof(const unsigned short);

        if (! encoder.checkStringForCorrectness(tmp, byteLength))
            throw std::runtime_error("String UTF-16 constructor cannot create string from bad UTF-16");

        size_type theNumberOfCodes = encoder.numberOfCharacters(tmp, byteLength);

        auto theArray = std::make_unique<unicode_point_type[]>(theNumberOfCodes);

        auto queryResult = encoder.hasByteOrderMark(tmp, byteLength);
        auto bomLength = encoder.lengthOfByteOrderMarkInBytes();
        if (queryResult.first)
        {
            tmp += bomLength;
            byteLength -= bomLength;
        }

        size_type bytesNeededForUTF8 = 0;

        for (size_type i = 0; i < theNumberOfCodes; i++)
        {
            auto operationResult = encoder.nextCodePoint(tmp, byteLength, queryResult.second);
            theArray[i] = operationResult.first;
            bytesNeededForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(theArray[i]);
            tmp += operationResult.second;
            byteLength -= operationResult.second;
        }

        auto charArray = std::make_unique<char_type[]>(bytesNeededForUTF8);

        auto tmp2 = charArray.get();
        auto bytesLeft = bytesNeededForUTF8;

        for (size_type i = 0; i < theNumberOfCodes; i++)
        {
            auto operationResult =
                utf8Encoder.encodeCodePoint(tmp2, bytesLeft, theArray[i], UTF8StringEncoder::thisSystemEndianness());
            tmp2 += operationResult;
            bytesLeft -= operationResult;
        }

        core = std::make_shared<core_type>(charArray.get(), bytesNeededForUTF8);
    }

    void String::initFromUTF32(const unsigned int * str, size_type length)
    {
        static UTF32StringEncoder encoder;
        static UTF8StringEncoder utf8Encoder;
        auto tmp = reinterpret_cast<const unsigned char *>(str);
        size_type byteLength = length * sizeof(const unsigned int);

        if (! encoder.checkStringForCorrectness(tmp, byteLength))
            throw std::runtime_error("String UTF-32 constructor cannot create string from bad UTF-32");

        size_type theNumberOfCodes = encoder.numberOfCharacters(tmp, byteLength);

        auto theArray = std::make_unique<unicode_point_type[]>(theNumberOfCodes);

        auto queryResult = encoder.hasByteOrderMark(tmp, byteLength);
        auto bomLength = encoder.lengthOfByteOrderMarkInBytes();
        if (queryResult.first)
        {
            tmp += bomLength;
            byteLength -= bomLength;
        }

        size_type bytesNeededForUTF8 = 0;

        for (size_type i = 0; i < theNumberOfCodes; i++)
        {
            auto operationResult = encoder.nextCodePoint(tmp, byteLength, queryResult.second);
            theArray[i] = operationResult.first;
            bytesNeededForUTF8 += utf8Encoder.bytesNeededForRepresentationOfCode(theArray[i]);
            tmp += operationResult.second;
            byteLength -= operationResult.second;
        }

        auto charArray = std::make_unique<char_type[]>(bytesNeededForUTF8);

        auto tmp2 = charArray.get();
        auto bytesLeft = bytesNeededForUTF8;

        for (size_type i = 0; i < theNumberOfCodes; i++)
        {
            auto operationResult =
                utf8Encoder.encodeCodePoint(tmp2, bytesLeft, theArray[i], UTF8StringEncoder::thisSystemEndianness());
            tmp2 += operationResult;
            bytesLeft -= operationResult;
        }

        core = std::make_shared<core_type>(charArray.get(), bytesNeededForUTF8);
    }

    std::ostream & operator<<(std::ostream & o, const String & s)
    {
        return s.description(o);
    }

    ComparisonResult compareStrings(const String & stringOne, const String & stringTwo, void * context)
    {
        (void)context;
        return stringOne.compare(stringTwo);
    }

    bool operator==(const char * s, const String & t)
    {
        return t.operator==(s);
    }

    bool operator==(const std::string & s, const String & t)
    {
        return t.operator==(s);
    }

    auto operator==(const std::string_view & s, const String & t) -> bool
    {
        auto newS = String::initWithStringView(s);
        return newS == t;
    }

    bool operator<(const String & a, const String & b)
    {
        ComparisonResult result = compareStrings(a, b, nullptr);
        if (result == OrderedAscending)
            return true;
        return false;
    }

    bool operator>(const String & a, const String & b)
    {
        ComparisonResult result = compareStrings(a, b, nullptr);
        if (result == OrderedDescending)
            return true;
        return false;
    }

    String operator+(const char * a, const String & b)
    {
        String theA(a);
        return theA + b;
    }

    String operator+(const std::string & a, const String & b)
    {
        String theA(a);
        return theA + b;
    }

} // namespace TF::Foundation
