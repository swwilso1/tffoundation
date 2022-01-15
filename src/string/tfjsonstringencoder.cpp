/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 Tectiform Inc.

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
#define NEEDS_CMATH
#define NEEDS_VECTOR
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfjsonstringencoder.hpp"
#include "tfutf16stringencoder.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        const JSONStringEncoder::size_type JSONStringEncoder::byteOrderMarkLength = 3;


        StringEncoder *JSONStringEncoder::clone()
        {
            return new JSONStringEncoder;
        }


        JSONStringEncoder::size_type JSONStringEncoder::numberOfBytesRequiredForLargestCharacterValue()
        {
            // In JSON you can have an 'encoded' unicode character.
            // That character can be a value in the range 0 - D7FF or E000 - FFFF,
            // or a surrogate pair with high surrogate in the D800 - DBFF range
            // and the low surrogate in the DC00 - DFFF range.  In the encoding
            // of a U+10XXXX code point, there will be a high surrogate and low
            // surrogate.  Each encoded unicode character is prefix with '\u', so
            // the largest encoded character would have the form \uXXXX\uXXXX.
            return 12 * sizeof(data_type);
        }


        std::ostream &JSONStringEncoder::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("JSONStringEncoder");
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::pair<bool, JSONStringEncoder::unicode_point_type> JSONStringEncoder::calculateTheUPoint(const char_type *s,
                                                                                                     size_type length)
        {
            if(s == nullptr || length < 4)
            {
                return std::make_pair(false, 0);
            }

            bool aborted = false;
            parent_type::unicode_point_type the_code = 0;
            size_type modifier;
            for(size_type i = 0, k = 3; i < 4; i++, k--)
            {
                auto base = std::pow(16.0, static_cast<double>(k));
                if(*(s + i) >= 48 && *(s + i) <= 57)
                {
                    // Integer value in the range 0-9
                    modifier = static_cast<size_type>(*(s + i) - 48);
                }
                else if(*(s + i) >= 65 && *(s + i) <= 70)
                {
                    // Gives integer value 10-15 (A-F)
                    modifier = static_cast<size_type>(*(s + i) - 55);
                }
                else if(*(s + i) >= 97 && *(s + i) <= 102)
                {
                    // Gives integer value 10-15 (a-f)
                    modifier = static_cast<size_type>(*(s + i) - 87);
                }
                else
                {
                    // Found a non-allowed character, just move on to
                    // the ':' character.
                    aborted = true;
                    break;
                }

                the_code += base * modifier;
                if(the_code > 0xFFFF)
                {
                    throw std::range_error("calculateTheUPoint calculated a Unicode point value that "
                                           "lies out-of-range");
                }
            }

            if(aborted)
            {
                return std::make_pair(false, 0);
            }

            return std::make_pair(true, the_code);
        }


        std::pair<bool, JSONStringEncoder::EscapedCodeStatus>
            JSONStringEncoder::calculateTheEscapedCode(const char_type *s, size_type length)
        {
            EscapedCodeStatus code_status;

            if(s == nullptr || length <= 0)
            {
                return std::make_pair(false, code_status);
            }

            auto value = *s;
            auto return_value = std::make_pair(true, code_status);

            if(value == '\\')
            {
                code_status.m_code = static_cast<unicode_point_type>('\\');
            }
            else if(value == '/')
            {
                code_status.m_code = static_cast<unicode_point_type>('/');
            }
            else if(value == '"')
            {
                code_status.m_code = static_cast<unicode_point_type>('"');
            }
            else if(value == 'b')
            {
                // \b (backspace)
                code_status.m_code = 8;
            }
            else if(value == 'f')
            {
                // \f (formfeed)
                code_status.m_code = 12;
            }
            else if(value == 'n')
            {
                // \n (linefeed)
                code_status.m_code = 10;
            }
            else if(value == 'r')
            {
                // \r (carriage return)
                code_status.m_code = 13;
            }
            else if(value == 't')
            {
                // \t (horizontal tab)
                code_status.m_code = 9;
            }
            else if(value == 'u')
            {
                auto rval = calculateTheUPoint(s + 1, length - 1);
                code_status.m_code = rval.second;
                code_status.m_escaped_unicode = true;
                return_value.first = rval.first;
            }
            else
            {
                return_value.first = false;
            }

            return_value.second = code_status;
            return return_value;
        }


        JSONStringEncoder::size_type JSONStringEncoder::bytesToExpectInUTF8Sequence(const data_type *s,
                                                                                    size_type length)
        {
            if(s == NULL || length == 0)
            {
                return 0;
            }

            auto value = *s;

            if(value == '\\')
            {
                // We need to calculate the character code for the escaped character.
                auto rval = calculateTheEscapedCode(s + 1, length - 1);
                if(!rval.first)
                {
                    throw std::runtime_error(
                        "bytesToExpectInJSONSequence encountered an incorrectly escaped character");
                }

                if(!rval.second.m_escaped_unicode)
                {
                    return 2;
                }

                // At this point we have an escaped \uXXXX for code value.  The value could
                // be a valid value in the (0, 0xD7FF) and (0xE000, 0xFFF) range.  The value
                // could also be a high surrogate from a surrogate pair. Check now to figure
                // out which it is.
                auto code = rval.second.m_code;
                if((code >= 0 && code <= 0xD7FF) || (code >= 0xE000 && code <= 0xFFFF))
                {
                    return 6;
                }
                else if(code >= 0xD800 && code <= 0xDBFF)
                {
                    // The code value is a high surrogate. Check to see if we can grab
                    // a low surrogate.
                    auto low_surrogate_rval = calculateTheEscapedCode(s + 7, length - 7);
                    if(!low_surrogate_rval.first)
                    {
                        // We had an error decoding any follow-on escaped value.  This puts us
                        // in an awkward spot.
                        throw std::runtime_error("bytesToExpectInJSONSequence encountered a mal-formed high surrogate");
                    }

                    // The next value has to be a low surrogate in the range (0xDC00, 0xDFFF).
                    if(low_surrogate_rval.second.m_code >= 0xDC00 && low_surrogate_rval.second.m_code <= 0xDFFF)
                    {
                        return 12;
                    }

                    throw std::runtime_error("bytesToExpectInJSONSequence encountered a mal-formed surrogate pair");
                }
                else if(code >= 0xDC00 && code <= 0xDFFF)
                {
                    // We encountered a low surrogate (without a high surrogate).
                    throw std::runtime_error("bytesToExpectInJSONSequence encountered an unpaired low surrogate");
                }

                throw std::runtime_error("bytesToExpectInJSONSequence encountered an invalid escape sequence");
            }

            return parent_type::bytesToExpectInUTF8Sequence(s, length);
        }


        JSONStringEncoder::parent_type::unicode_point_type
            JSONStringEncoder::convertUTF8SequenceToUnicodePoint(const data_type *string, size_type length)
        {
            if(string == nullptr || length <= 0)
            {
                throw std::runtime_error("convertJSONSequenceToUnicodePoint given invalid arguments");
            }

            if(*string == '\\')
            {
                // We have an escaped value in the JSON, calculate the escaped code if possible.
                auto rval = calculateTheEscapedCode(string + 1, length - 1);
                if(!rval.first)
                {
                    throw std::runtime_error(
                        "bytesToExpectInJSONSequence encountered an incorrectly escaped character");
                }

                if(!rval.second.m_escaped_unicode)
                {
                    return rval.second.m_code;
                }

                // At this point we have an escaped \uXXXX for code value.  The value could
                // be a valid value in the (0, 0xD7FF) and (0xE000, 0xFFF) range.  The value
                // could also be a high surrogate from a surrogate pair. Check now to figure
                // out which it is.
                auto code = rval.second.m_code;
                if((code >= 0 && code <= 0xD7FF) || (code >= 0xE000 && code <= 0xFFFF))
                {
                    return code;
                }
                else if(code >= 0xD800 && code <= 0xDBFF)
                {
                    // The code value is a high surrogate. Check to see if we can grab
                    // a low surrogate.
                    auto low_surrogate_rval = calculateTheEscapedCode(string + 7, length - 7);
                    if(!low_surrogate_rval.first)
                    {
                        // We had an error decoding any follow-on escaped value.  This puts us
                        // in an awkward spot.
                        throw std::runtime_error("bytesToExpectInJSONSequence encountered a mal-formed high surrogate");
                    }

                    // The next value has to be a low surrogate in the range (0xDC00, 0xDFFF).
                    if(low_surrogate_rval.second.m_code >= 0xDC00 && low_surrogate_rval.second.m_code <= 0xDFFF)
                    {
                        // use the UTF-16 encoder to convert the surrogate pair to a code value.
                        static UTF16StringEncoder utf16_encoder;
                        return utf16_encoder.convertSurrogatePairToCodePoint(code, low_surrogate_rval.second.m_code);
                    }

                    throw std::runtime_error("bytesToExpectInJSONSequence encountered a mal-formed surrogate pair");
                }
                else if(code >= 0xDC00 && code <= 0xDFFF)
                {
                    // We encountered a low surrogate (without a high surrogate).
                    throw std::runtime_error("bytesToExpectInJSONSequence encountered an unpaired low surrogate");
                }

                throw std::runtime_error("bytesToExpectInJSONSequence encountered an invalid escape sequence");
            }

            return parent_type::convertUTF8SequenceToUnicodePoint(string, length);
        }


        std::string JSONStringEncoder::getEncoderID() const
        {
            return std::string("AC579979-0E2F-4F6B-A6DE-5B1AB203697E");
        }

    }    // namespace Foundation

}    // namespace TF
