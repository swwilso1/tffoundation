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

#define NEEDS_CSTRING
#define NEEDS_CMATH
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfutf16stringencoder.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        const UTF16StringEncoder::size_type UTF16StringEncoder::byteOrderMarkLength = 2;


        StringEncoder *UTF16StringEncoder::clone(void)
        {
            return new UTF16StringEncoder;
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::basicCodeLengthInBytes(void)
        {
            return sizeof(data_type);
        }


        bool UTF16StringEncoder::hasFixedCodeLength(void)
        {
            return false;
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::numberOfBytesRequiredForLargestCharacterValue(void)
        {
            return 2 * sizeof(data_type);
        }


        bool UTF16StringEncoder::canUseByteOrderMark(void)
        {
            return true;
        }


        bool UTF16StringEncoder::usesByteOrderMark(void)
        {
            return true;
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::lengthOfByteOrderMarkInBytes(void)
        {
            return byteOrderMarkLength;
        }


        void UTF16StringEncoder::writeByteOrderMark(char_type *string, size_type length)
        {
            if(length < byteOrderMarkLength)
                throw std::runtime_error("array length less than size of byte order mark");
            data_type *stringStart = reinterpret_cast<data_type *>(string);
            *stringStart = 0xFEFF;
        }


        UTF16StringEncoder::byte_order_query_type UTF16StringEncoder::hasByteOrderMark(const char_type *string,
                                                                                       size_type length)
        {
            endian theStringEndianness = Endian::BigEndian;
            bool doesHaveBOM = false;

            if(string == NULL || length == 0)
                return std::make_pair(false, this->thisSystemEndianness());

            const unsigned char *bytes = reinterpret_cast<const unsigned char *>(string);
            if(*bytes == 0xFE && *(bytes + 1) == 0xFF)
            {
                // FE FF is the UTF-16 big endian byte order mark
                theStringEndianness = Endian::BigEndian;
                doesHaveBOM = true;
            }
            else if(*bytes == 0xFF && *(bytes + 1) == 0xFE)
            {
                // FF FE is the UTF-16 little endian byte order mark
                theStringEndianness = Endian::LittleEndian;
                doesHaveBOM = true;
            }

            return std::make_pair(doesHaveBOM, theStringEndianness);
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::numberOfCharacters(const char_type *string, size_type length)
        {
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(string, length);
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            size_type characterCounter = 0;

            const char_type *tmp = stringQueryResult.first ? string + this->lengthOfByteOrderMarkInBytes() : string;
            const char_type *end = string + length;
            size_type bytesLeft = stringQueryResult.first ? length - lengthOfByteOrderMarkInBytes() : length;

            while(tmp < end)
            {
                theNext = this->nextCodePoint(tmp, bytesLeft, stringQueryResult.second);
                tmp += theNext.second;
                bytesLeft -= theNext.second;
                characterCounter++;
            }

            return characterCounter;
        }


        bool UTF16StringEncoder::checkStringForCorrectness(const char_type *string, size_type length)
        {
            const data_type *utf16 = reinterpret_cast<const data_type *>(string);
            size_type utf16Length = length / sizeof(data_type);
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(string, length);

            if(stringQueryResult.first)
            {
                utf16++;
                utf16Length--;
            }

            for(size_type i = 0; i < utf16Length; i++)
            {
                data_type code = this->correctUTF16CodeForPlatform(*(utf16 + i), stringQueryResult.second);

                // Check the code to see if it is the first code in a surrogate pair.
                // The high surrogate of a surrogate pair has a value
                // that lies betwee 0xD800 and 0xDBFF inclusive
                if(code >= 0xD800 && code <= 0xDBFF)
                {
                    // We have a high surrogate, first check to see
                    // if we any more characters left in the string
                    if(i == (utf16Length - 1))
                        return false;

                    data_type nextCode = this->correctUTF16CodeForPlatform(*(utf16 + i + 1), stringQueryResult.second);

                    // The low surrogate of the surrogate pair should have
                    // a value between 0xDC00 and 0xDFFF inclusive
                    if(nextCode < 0xDC00 || nextCode > 0xDFFF)
                        return false;
                    i++;
                }
            }
            return true;
        }


        std::pair<UTF16StringEncoder::parent_type::unicode_point_type, UTF16StringEncoder::size_type>
            UTF16StringEncoder::nextCodePoint(const char_type *string, size_type length, Endian endian)
        {
            const data_type *utf16 = reinterpret_cast<const data_type *>(string);
            size_type utf16Length = length / sizeof(data_type);
            parent_type::unicode_point_type theCode;
            size_type theSize;

            if(utf16Length == 0)
                throw std::runtime_error("unable to compute nextCodePoint for empty string");

            data_type code = this->correctUTF16CodeForPlatform(*utf16, endian);

            if(code >= 0xD800 && code <= 0xDBFF)
            {
                if(utf16Length <= 1)
                    throw std::runtime_error("character array does not have second value of surrogate pair");

                data_type nextCode = this->correctUTF16CodeForPlatform(*(utf16 + 1), endian);

                if(nextCode < 0xDC00 || nextCode > 0xDFFF)
                    throw std::runtime_error("next surrogate pair value not in surrogate pair range");

                theCode = this->convertSurrogatePairToCodePoint(code, nextCode);
                theSize = 2 * sizeof(data_type);
            }
            else
            {
                theCode = code;
                theSize = sizeof(data_type);
            }

            return std::make_pair(theCode, theSize);
        }


        std::pair<UTF16StringEncoder::parent_type::unicode_point_type, UTF16StringEncoder::size_type>
            UTF16StringEncoder::nextCode(const char_type *string, size_type length, Endian endian)
        {
            if(length < sizeof(data_type))
                throw std::runtime_error("nextCode unable to calculate next code point, length of data array less "
                                         " than size of data type");
            const data_type *theCodes = reinterpret_cast<const data_type *>(string);
            data_type theCode = *theCodes;
            parent_type::unicode_point_type finalCode = this->correctUTF16CodeForPlatform(theCode, endian);
            return std::make_pair(finalCode, sizeof(data_type));
        }


        UTF16StringEncoder::parent_type::unicode_point_type
            UTF16StringEncoder::unicodeCodePointForCharacterAtIndex(const char_type *string, size_type length,
                                                                    size_type index)
        {
            const char_type *tmp = string;
            size_type stringLength = length;
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(string, length);
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            size_type i = 0;

            if(stringQueryResult.first)
            {
                tmp += this->lengthOfByteOrderMarkInBytes();
                stringLength -= this->lengthOfByteOrderMarkInBytes();
            }

            theNext = this->nextCodePoint(tmp, stringLength, stringQueryResult.second);

            while(i < index)
            {
                tmp += theNext.second;
                stringLength -= theNext.second;
                i++;
                theNext = this->nextCodePoint(tmp, stringLength, stringQueryResult.second);
            }

            return theNext.first;
        }


        UTF16StringEncoder::size_type
            UTF16StringEncoder::bytesNeededForRepresentationOfCode(parent_type::unicode_point_type code)
        {
            if(code <= 0xFFFF)
                return sizeof(data_type);
            return 2 * sizeof(data_type);
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::encodeCodePoint(char_type *string, size_type length,
                                                                          parent_type::unicode_point_type code,
                                                                          Endian endian)
        {
            size_type theBytesUsed = 0;

            data_type *tmp = reinterpret_cast<data_type *>(string);
            size_type stringLength = length / sizeof(data_type);

            if(code > 0xFFFF)
            {
                /*
                 This algorithm for calculating surrogate pairs comes from
                 http://www.unicode.org/faq//utf_bom.html
                 Q: What’s the algorithm to convert from UTF-16 to character codes?
                 Q: Isn’t there a simpler way to do this?
                 */
                // constants
                const unsigned int LEAD_OFFSET = 0xD800 - (0x10000 >> 10);

                // computations
                data_type lead = LEAD_OFFSET + (code >> 10);
                data_type trail = 0xDC00 + (code & 0x3FF);

                if(stringLength < 2)
                    throw std::runtime_error("encodeCodePoint asked to encode unicode point that generates a surrogate"
                                             " pair, not given enough space to encode");

                *tmp = this->correctUTF16CodeForPlatform(lead, endian);
                *(tmp + 1) = this->correctUTF16CodeForPlatform(trail, endian);
                theBytesUsed = 2 * sizeof(data_type);
            }
            else
            {
                if(stringLength < 1)
                    throw std::runtime_error("encodeCodePoint not given enough space to encode character");

                *tmp = this->correctUTF16CodeForPlatform(code, endian);
                theBytesUsed = sizeof(data_type);
            }

            return theBytesUsed;
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::arrayIndexOfCharacterAtCharacterIndex(const char_type *string,
                                                                                                size_type length,
                                                                                                size_type index)
        {
            const char_type *tmp = string;
            size_type stringLength = length;
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(string, length);
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            size_type i = 0;

            if(stringQueryResult.first)
            {
                tmp += this->lengthOfByteOrderMarkInBytes();
                stringLength -= this->lengthOfByteOrderMarkInBytes();
            }

            theNext = this->nextCodePoint(tmp, stringLength, stringQueryResult.second);

            while(i < index)
            {
                tmp += theNext.second;
                stringLength -= theNext.second;
                i++;
                if(i < index)
                    theNext = this->nextCodePoint(tmp, stringLength, stringQueryResult.second);
            }

            return tmp - string;
        }


        UTF16StringEncoder::size_type
            UTF16StringEncoder::numberOfBytesToCaptureCharactersInRange(const char_type *string, size_type length,
                                                                        const range_type &range)
        {
            if(!this->doesRangeOfCharactersLieInString(string, length, range))
                throw std::range_error("numberOfBytesToCaptureCharactersInRange given string and range where "
                                       "the character range lies outside the string");

            size_type stringIndex = arrayIndexOfCharacterAtCharacterIndex(string, length, range.position);
            size_type endIndex = arrayIndexOfCharacterAtCharacterIndex(string, length, range.position + range.length);

            return endIndex - stringIndex;
        }


        bool UTF16StringEncoder::containsCharacterWithZeroValue(const char_type *string, size_type length)
        {
            const char_type *tmp = string;
            size_type theLength = length;
            byte_order_query_type stringQueryResults = this->hasByteOrderMark(string, length);
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            if(stringQueryResults.first)
            {
                tmp += this->lengthOfByteOrderMarkInBytes();
                theLength -= this->lengthOfByteOrderMarkInBytes();
            }

            while(theLength > 0)
            {
                theNext = this->nextCodePoint(tmp, theLength, stringQueryResults.second);
                if(theNext.first == 0)
                    return true;
                tmp += theNext.second;
                theLength -= theNext.second;
            }

            return false;
        }


        bool UTF16StringEncoder::containsCharacterNotInASCIIRange(const char_type *string, size_type length)
        {
            const char_type *tmp = string;
            size_type theLength = length;
            byte_order_query_type stringQueryResults = this->hasByteOrderMark(string, length);
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            if(stringQueryResults.first)
            {
                tmp += this->lengthOfByteOrderMarkInBytes();
                theLength -= this->lengthOfByteOrderMarkInBytes();
            }

            while(theLength > 0)
            {
                theNext = this->nextCodePoint(tmp, theLength, stringQueryResults.second);
                if(theNext.first == 0 || theNext.first > 127)
                    return true;
                tmp += theNext.second;
                theLength -= theNext.second;
            }

            return false;
        }


        UTF16StringEncoder::range_type
            UTF16StringEncoder::findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                 const char_type *substringStart,
                                                                 size_type substringLength)
        {
            range_type result;

            if(substringLength > stringLength)
                return result;

            const data_type *theStringStart = reinterpret_cast<const data_type *>(stringStart);
            size_type theStringLength = stringLength / sizeof(data_type);
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(stringStart, stringLength);

            const data_type *theSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            size_type theSubstringLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResult = this->hasByteOrderMark(substringStart, substringLength);

            if(stringQueryResult.first)
            {
                theStringStart++;
                theStringLength--;
            }

            if(substringQueryResult.first)
            {
                theSubstringStart++;
                theSubstringLength--;
            }

            for(size_type i = 0; i < theStringLength; i++)
            {
                if(*(theStringStart + i) == *theSubstringStart)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(theSubstringLength > (theStringLength - i))
                        return result;

                    for(; j < (i + theSubstringLength); j++, k++)
                    {
                        if(*(theStringStart + j) != *(theSubstringStart + k))
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                        continue;
                    else
                    {
                        result.position = i;
                        result.length = substringLength;
                        return result;
                    }
                }
            }

            return result;
        }


        UTF16StringEncoder::range_array_type
            UTF16StringEncoder::findByteRangesOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                  const char_type *substringStart,
                                                                  size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
                return rangeArray;

            const data_type *theStringStart = reinterpret_cast<const data_type *>(stringStart);
            size_type theStringLength = stringLength / sizeof(data_type);

            const data_type *theSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            size_type theSubstringLength = substringLength / sizeof(data_type);

            for(size_type i = 0; i < theStringLength; i++)
            {
                if(*(theStringStart + i) == *theSubstringStart)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(theSubstringLength > (theStringLength - i))
                        return rangeArray;

                    for(; j < (i + theSubstringLength); i++)
                    {
                        if(*(theStringStart + j) != *(theSubstringStart + k))
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                        continue;
                    else
                    {
                        range_type genericRange(i, theSubstringLength);
                        rangeArray.push_back(genericRange);
                        i = j - 1;
                        continue;
                    }
                }
            }

            return rangeArray;
        }


        UTF16StringEncoder::range_type UTF16StringEncoder::findCharacterRangeForSubstringInString(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_type result;

            if(substringLength > stringLength)
                return result;

            size_type stringNumberOfCharacters = numberOfCharacters(stringStart, stringLength);
            size_type substringNumberOfCharacters = numberOfCharacters(substringStart, substringLength);

            parent_type::unicode_point_type subCode =
                unicodeCodePointForCharacterAtIndex(substringStart, substringLength, 0);

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                parent_type::unicode_point_type stringCode =
                    unicodeCodePointForCharacterAtIndex(stringStart, stringLength, i);

                if(stringCode == subCode)
                {
                    size_type j = i + 1, k = 1;
                    result.position = i;
                    bool aborted = false;

                    if(substringNumberOfCharacters > (stringNumberOfCharacters - i))
                        return Range();

                    for(; j < (i + substringNumberOfCharacters); j++, k++)
                    {
                        parent_type::unicode_point_type stringCode2 =
                            unicodeCodePointForCharacterAtIndex(stringStart, stringLength, j);
                        parent_type::unicode_point_type subCode2 =
                            unicodeCodePointForCharacterAtIndex(substringStart, substringLength, k);

                        if(stringCode2 != subCode2)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                    {
                        result.position = 0;
                        continue;
                    }
                    else
                    {
                        result.length = substringNumberOfCharacters;
                        return result;
                    }
                }
            }

            return result;
        }


        UTF16StringEncoder::range_array_type UTF16StringEncoder::findCharacterRangesForSubstringInString(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
                return rangeArray;

            size_type stringNumberOfCharacters = numberOfCharacters(stringStart, stringLength);
            size_type substringNumberOfCharacters = numberOfCharacters(substringStart, substringLength);

            parent_type::unicode_point_type subCode =
                unicodeCodePointForCharacterAtIndex(substringStart, substringLength, 0);

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                parent_type::unicode_point_type stringCode =
                    unicodeCodePointForCharacterAtIndex(stringStart, stringLength, i);

                if(stringCode == subCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringNumberOfCharacters > (stringNumberOfCharacters - i))
                        return rangeArray;

                    for(; j < (i + substringNumberOfCharacters); j++, k++)
                    {
                        parent_type::unicode_point_type stringCode2 =
                            unicodeCodePointForCharacterAtIndex(stringStart, stringLength, j);
                        parent_type::unicode_point_type subCode2 =
                            unicodeCodePointForCharacterAtIndex(substringStart, substringLength, k);

                        if(stringCode2 != subCode2)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                    {
                        continue;
                    }
                    else
                    {
                        range_type theRange(i, substringNumberOfCharacters);
                        rangeArray.push_back(theRange);
                    }
                }
            }

            return rangeArray;
        }


        UTF16StringEncoder::range_array_type UTF16StringEncoder::findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);

            if(substringLength > stringLength)
            {
                if(stringLength > 0)
                {
                    range_type theRange(0, stringNumberOfCharacters);
                    rangeArray.push_back(theRange);
                }
                return rangeArray;
            }

            size_type substringNumberOfCharacters = this->numberOfCharacters(substringStart, substringLength);

            parent_type::unicode_point_type subCode =
                this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, 0);

            size_type location = 0;
            size_type length = 0;

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                parent_type::unicode_point_type stringCode =
                    this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, i);

                if(stringCode == subCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringNumberOfCharacters > (stringNumberOfCharacters - i))
                    {
                        length += stringNumberOfCharacters - i;
                        range_type theRange(location, length);
                        rangeArray.push_back(theRange);
                        return rangeArray;
                    }

                    for(; j < (i + substringNumberOfCharacters); j++, k++)
                    {
                        parent_type::unicode_point_type stringCode2 =
                            this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, j);
                        parent_type::unicode_point_type subCode2 =
                            this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, k);

                        if(stringCode2 != subCode2)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                    {
                        length++;
                        continue;
                    }
                    else
                    {
                        if(length > 0)
                        {
                            range_type theRange(location, length);
                            rangeArray.push_back(theRange);
                            length = 0;
                        }
                        location = i + substringNumberOfCharacters;
                        i = j - 1;
                        continue;
                    }
                }
                else
                {
                    length++;
                }
            }

            if(length > 0)
            {
                range_type theRange(location, length);
                rangeArray.push_back(theRange);
            }

            return rangeArray;
        }


        void UTF16StringEncoder::convertStringCharacters(char_type *string, size_type length, StringCase convertToCase)
        {
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(string, length);
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            char_type *tmp = string;
            size_type theLength = length;

            if(stringQueryResult.first)
            {
                tmp += this->lengthOfByteOrderMarkInBytes();
                theLength -= this->lengthOfByteOrderMarkInBytes();
            }

            while(theLength > 0)
            {
                theNext = this->nextCodePoint(tmp, theLength, stringQueryResult.second);
                if(theNext.first > 0 && theNext.first < 128)
                {
                    if(convertToCase == StringCase::UpperCase)
                    {
                        if(theNext.first >= 97 && theNext.first <= 122)
                        {
                            theNext.first -= 32;
                            this->encodeCodePoint(tmp, theLength, theNext.first, stringQueryResult.second);
                        }
                    }
                    if(convertToCase == StringCase::LowerCase)
                    {
                        if(theNext.first >= 65 && theNext.first <= 90)
                        {
                            theNext.first += 32;
                            this->encodeCodePoint(tmp, theLength, theNext.first, stringQueryResult.second);
                        }
                    }
                }

                tmp += theNext.second;
                theLength -= theNext.second;
            }
        }


        UTF16StringEncoder::size_type UTF16StringEncoder::computeArraySizeInBytesForStringByReplacingSubstrings(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength, const char_type *replaceStringStart, size_type replaceStringLength,
            range_array_type &ranges)
        {
            size_type newsize = 0;

            const data_type *mainCharacterStart = reinterpret_cast<const data_type *>(stringStart);
            size_type mainCodeLength = stringLength / sizeof(data_type);
            byte_order_query_type mainQueryResponse = hasByteOrderMark(stringStart, stringLength);

            const data_type *substringCharacterStart = reinterpret_cast<const data_type *>(substringStart);
            size_type substringCodeLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResult = hasByteOrderMark(substringStart, substringLength);

            size_type replaceStringCodeLength = replaceStringLength / sizeof(data_type);
            byte_order_query_type replaceStringQueryResult = hasByteOrderMark(replaceStringStart, replaceStringLength);

            if(substringCodeLength > mainCodeLength)
            {
                if(mainCodeLength > 0)
                    return stringLength;
                return 0;
            }

            if(mainQueryResponse.first)
            {
                mainCharacterStart += 1;
                mainCodeLength -= 1;
            }

            if(substringQueryResult.first)
            {
                substringCharacterStart += 1;
                substringCodeLength -= 1;
            }

            if(replaceStringQueryResult.first)
            {
                replaceStringCodeLength -= 1;
            }

            data_type mainCode;
            data_type substringCode;
            data_type substringStartCode;

            substringStartCode = *substringCharacterStart;
            substringStartCode = this->correctUTF16CodeForPlatform(substringStartCode, substringQueryResult.second);

            for(size_type i = 0; i < mainCodeLength; i++)
            {
                mainCode = *(mainCharacterStart + i);
                mainCode = this->correctUTF16CodeForPlatform(mainCode, mainQueryResponse.second);

                if(mainCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringCodeLength > (mainCodeLength - i))
                        return ((newsize + mainCodeLength - i) * sizeof(data_type)) +
                               this->lengthOfByteOrderMarkInBytes();

                    for(; j < (i + substringCodeLength); j++, k++)
                    {
                        mainCode = *(mainCharacterStart + j);
                        mainCode = this->correctUTF16CodeForPlatform(mainCode, mainQueryResponse.second);

                        substringCode = *(substringCharacterStart + k);
                        substringCode = this->correctUTF16CodeForPlatform(substringCode, substringQueryResult.second);

                        if(mainCode != substringCode)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                    {
                        newsize++;
                        continue;
                    }
                    else
                    {
                        range_type theRange(i, j - i);
                        ranges.push_back(theRange);
                        newsize += replaceStringCodeLength;
                        i = j - 1;
                        continue;
                    }
                }

                newsize++;
            }

            return ((newsize) * sizeof(data_type)) + this->lengthOfByteOrderMarkInBytes();
        }


        void UTF16StringEncoder::replaceOccurancesOfStringWithString(
            const char_type *originalStringStart, size_type originalStringLength, char_type *newStringStart,
            size_type newStringLength, const char_type *replacementStringStart, size_type replacementStringLength,
            range_array_type &substringRanges)
        {
            bool endOfRanges = false;

            if(substringRanges.size() == 0)
                endOfRanges = true;

            auto rangeIter = substringRanges.begin();

            const data_type *theOriginalStringStart = reinterpret_cast<const data_type *>(originalStringStart);
            size_type theOriginalCodeLength = originalStringLength / sizeof(data_type);
            byte_order_query_type originalStringQueryResult =
                this->hasByteOrderMark(originalStringStart, originalStringLength);

            const data_type *theReplacementStringStart = reinterpret_cast<const data_type *>(replacementStringStart);
            size_type theReplacementCodeLength = replacementStringLength / sizeof(data_type);
            byte_order_query_type replacementStringQueryResult =
                this->hasByteOrderMark(replacementStringStart, replacementStringLength);

            data_type *theNewStringTmp = reinterpret_cast<data_type *>(newStringStart);

            if(originalStringQueryResult.first)
            {
                theOriginalStringStart += 1;
                theOriginalCodeLength -= 1;
            }

            if(replacementStringQueryResult.first)
            {
                theReplacementStringStart += 1;
                theReplacementCodeLength -= 1;
            }

            // Since we are the UTF-16 encoder, we need to write the byte order mark to the first code
            // of the new array.
            *theNewStringTmp = 0xFEFF;
            theNewStringTmp += 1;


            for(size_type i = 0; i < theOriginalCodeLength; i++)
            {
                if(!endOfRanges && i == rangeIter->position)
                {
                    for(size_type j = 0; j < theReplacementCodeLength; j++)
                    {
                        *theNewStringTmp = this->correctUTF16CodeForPlatform(*(theReplacementStringStart + j),
                                                                             replacementStringQueryResult.second);
                        theNewStringTmp++;
                    }
                    i += rangeIter->length - 1;
                    rangeIter++;
                    if(rangeIter == substringRanges.end())
                        endOfRanges = true;
                }
                else
                {
                    *theNewStringTmp = this->correctUTF16CodeForPlatform(*(theOriginalStringStart + i),
                                                                         originalStringQueryResult.second);
                    theNewStringTmp++;
                }
            }
        }


        UTF16StringEncoder::parent_type::unicode_point_type
            UTF16StringEncoder::correctValueForPlatform(const char_type *string, size_type length, Endian endian)
        {
            if(length < sizeof(data_type))
                throw std::runtime_error("correctValueForPlatform not given enough bytes to change the encoding for"
                                         " the platform");

            const data_type *firstCode = reinterpret_cast<const data_type *>(string);
            data_type theCode;
            theCode = this->correctUTF16CodeForPlatform(*firstCode, endian);

            if(theCode >= 0xD800 && theCode <= 0xDBFF)
            {
                if(length < (2 * sizeof(data_type)))
                    throw std::runtime_error("correctValueForPlatform not given enought bytes to change endian encoding"
                                             " for surrogate pair value");

                firstCode++;
                data_type secondCode = this->correctUTF16CodeForPlatform(*firstCode, endian);

                if(secondCode < 0xDC00 || secondCode > 0xDFFF)
                    throw std::runtime_error("correctValueForPlatform given invalid value for second value of surrogate"
                                             " pair");

                return this->convertSurrogatePairToCodePoint(theCode, secondCode);
            }

            return theCode;
        }


        bool UTF16StringEncoder::operator==(const StringEncoder &e)
        {
            return this->getEncoderID() == e.getEncoderID();
        }


        std::ostream &UTF16StringEncoder::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("UTF16StringEncoder");
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        UTF16StringEncoder::parent_type::unicode_point_type
            UTF16StringEncoder::convertSurrogatePairToCodePoint(data_type highSurrogate, data_type lowSurrogate)
        {
            /*
             This algorithm for calculating a UTF-32 code point from surrogate pairs comes from
             http://www.unicode.org/faq//utf_bom.html
             Q: What’s the algorithm to convert from UTF-16 to character codes?
             Q: Isn’t there a simpler way to do this?
             */

            // constants
            const parent_type::unicode_point_type SURROGATE_OFFSET = 0x10000 - (0xD800 << 10) - 0xDC00;

            parent_type::unicode_point_type codePoint = (highSurrogate << 10) + lowSurrogate + SURROGATE_OFFSET;
            return codePoint;
        }


        UTF16StringEncoder::data_type UTF16StringEncoder::correctUTF16CodeForPlatform(data_type theCode, Endian endian)
        {
            if(this->thisSystemEndianness() != endian)
            {
                char tmp;
                char *vTmp = reinterpret_cast<char *>(&theCode);
                tmp = *(vTmp);
                *(vTmp) = *(vTmp + 1);
                *(vTmp + 1) = tmp;
            }

            return theCode;
        }

        std::string UTF16StringEncoder::getEncoderID() const
        {
            return std::string("371D4C11-1DB0-49EB-A26C-C05A64BAA3E6");
        }

    }    // namespace Foundation

}    // namespace TF
