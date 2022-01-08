/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2021 Tectiform Inc.

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


        JSONStringEncoder::~JSONStringEncoder()
        {
        }


        StringEncoder *JSONStringEncoder::clone()
        {
            return new JSONStringEncoder;
        }


        JSONStringEncoder::size_type JSONStringEncoder::basicCodeLengthInBytes()
        {
            return sizeof(data_type);
        }


        bool JSONStringEncoder::hasFixedCodeLength()
        {
            return false;
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


        bool JSONStringEncoder::canUseByteOrderMark()
        {
            return true;
        }


        bool JSONStringEncoder::usesByteOrderMark()
        {
            return false;
        }


        JSONStringEncoder::size_type JSONStringEncoder::lengthOfByteOrderMarkInBytes()
        {
            return byteOrderMarkLength;
        }


        void JSONStringEncoder::writeByteOrderMark(char_type *string, size_type length)
        {
            if(length < byteOrderMarkLength)
                throw std::runtime_error("writeByteOrderMark cannot write byte order mark, destination array too"
                                         " small");

            data_type *tmp = reinterpret_cast<data_type *>(string);

            *(tmp) = 0xEF;
            *(tmp + 1) = 0xBB;
            *(tmp + 2) = 0xBF;
        }


        JSONStringEncoder::byte_order_query_type JSONStringEncoder::hasByteOrderMark(const char_type *string,
                                                                                     size_type length)
        {
            if(length < 3)
                return std::make_pair(false, this->thisSystemEndianness());

            const data_type *tmp = reinterpret_cast<const data_type *>(string);
            if(*tmp != 0xEF)
                return std::make_pair(false, this->thisSystemEndianness());
            if(*(tmp + 1) != 0xBB)
                return std::make_pair(false, this->thisSystemEndianness());
            if(*tmp != 0xBF)
                return std::make_pair(false, this->thisSystemEndianness());

            return std::make_pair(true, this->thisSystemEndianness());
        }


        JSONStringEncoder::size_type JSONStringEncoder::numberOfCharacters(const char_type *string, size_type length)
        {
            const data_type *initialUTF8 = reinterpret_cast<const data_type *>(string);
            size_type initialUTF8Length = length;
            size_type counter = 0, incrementValue;
            bool loopFinished = false;

            // First consume the byte order mark if present
            if(initialUTF8Length >= 3)
            {
                // UTF-8 byte order mark is 0xEF 0xBB 0xBF - a 3 byte order mark sequence
                if(*initialUTF8 == 0xEF && *(initialUTF8 + 1) == 0xBB && *(initialUTF8 + 2) == 0xBF)
                {
                    initialUTF8 += 3;
                    initialUTF8Length -= 3;
                }
            }

            while(!loopFinished)
            {
                incrementValue = bytesToExpectInJSONSequence(initialUTF8, initialUTF8Length);
                initialUTF8 += incrementValue;
                counter++;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return counter;
        }


        bool JSONStringEncoder::checkStringForCorrectness(const char_type *string, size_type length)
        {
            const data_type *initialUTF8 = reinterpret_cast<const data_type *>(string);
            size_type initialUTF8Length = length;
            size_type i;
            size_type incrementValue;
            bool loopFinished = false;
            byte_order_query_type byteOrderMarkQuery = hasByteOrderMark(string, length);

            if(byteOrderMarkQuery.first)
            {
                initialUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            // Now convert the characters
            i = 0;
            while(!loopFinished)
            {
                unicode_point_type theCode;
                incrementValue = bytesToExpectInJSONSequence(initialUTF8 + i, initialUTF8Length);
                theCode = convertJSONSequenceToUnicodePoint(initialUTF8 + i, incrementValue);
                if(theCode > 0x10FFFF)
                    return false;
                i += incrementValue;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return true;
        }


        std::pair<JSONStringEncoder::parent_type::unicode_point_type, JSONStringEncoder::size_type>
            JSONStringEncoder::nextCodePoint(const char_type *string, size_type length, Endian endian)
        {
            size_type incrementValue = bytesToExpectInJSONSequence(string, length);
            parent_type::unicode_point_type theCode =
                convertJSONSequenceToUnicodePoint(reinterpret_cast<const data_type *>(string), incrementValue);
            return std::make_pair(theCode, incrementValue);
        }


        std::pair<JSONStringEncoder::parent_type::unicode_point_type, JSONStringEncoder::size_type>
            JSONStringEncoder::nextCode(const char_type *string, size_type length, Endian endian)
        {
            if(length == 0)
                throw std::runtime_error("nextCode cannot calculate next code point, length of data array is zero.");
            return std::make_pair(static_cast<parent_type::unicode_point_type>(*string), 1);
        }


        JSONStringEncoder::parent_type::unicode_point_type
            JSONStringEncoder::unicodeCodePointForCharacterAtIndex(const char_type *string, size_type length,
                                                                   size_type index)
        {
            size_type theIndex = arrayIndexOfCharacterAtCharacterIndex(string, length, index);
            size_type interval = bytesToExpectInJSONSequence(string + theIndex, length - theIndex);
            parent_type::unicode_point_type theCode =
                convertJSONSequenceToUnicodePoint(reinterpret_cast<const data_type *>(string + theIndex), interval);
            return theCode;
        }


        JSONStringEncoder::size_type JSONStringEncoder::bytesNeededForRepresentationOfCode(unicode_point_type code)
        {
            // JSON supports encoded unicode sequences of the form \uXXXX with high and low
            // surrogate support.  However, JSON is generally encoded in UTF-8, so we just
            // use UTF-8 encoding.
            return bytesNeededForUTF8RepresenationOfUnicodePoint(code);
        }


        JSONStringEncoder::size_type JSONStringEncoder::encodeCodePoint(char_type *string, size_type length,
                                                                        unicode_point_type code, Endian endian)
        {
            // JSON supports encoded unicode sequences of the form \uXXXX with high and low
            // surrogate support.  However, JSON is generally encoded in UTF-8, so we just
            // use UTF-8 encoding.

            // number values larger than 0x10FFFF have no mapping to UTF-8 characters (21-bit Unicode character space)
            if(code > 0x10FFFF)
                throw std::range_error("encodeCodePoint given a value that lies outside "
                                       "range of value unicode");

            size_type bytesNeeded = bytesNeededForUTF8RepresenationOfUnicodePoint(code);
            if(bytesNeeded > length)
                throw std::runtime_error("encodeCodePoint needs more space to encode unicode point than given");

            std::vector<data_type> bytes;

            if(code <= 0x7F)
            {
                // 1-byte sequence
                bytes.push_back(static_cast<data_type>(code));
            }
            else if(code >= 0x80 && code <= 0x7FF)
            {
                // 2-byte sequence
                data_type byte1 = 0xC0 | (code >> 6);
                data_type byte2 = 0x80 | (code & 0x3F);
                bytes.push_back(byte1);
                bytes.push_back(byte2);
            }
            else if(code >= 0x800 && code <= 0xFFFF)
            {
                // 3-byte sequence
                data_type byte1 = 0xE0 | (code >> 12);
                data_type byte2 = 0x80 | ((code & 0xFC0) >> 6);
                data_type byte3 = 0x80 | (code & 0x3F);
                bytes.push_back(byte1);
                bytes.push_back(byte2);
                bytes.push_back(byte3);
            }
            else
            {
                // 4-byte sequence
                data_type byte1 = 0xF0 | (code >> 18);
                data_type byte2 = 0x80 | ((code & 0x3F000) >> 12);
                data_type byte3 = 0x80 | ((code & 0xFC0) >> 6);
                data_type byte4 = 0x80 | (code & 0x3F);
                bytes.push_back(byte1);
                bytes.push_back(byte2);
                bytes.push_back(byte3);
                bytes.push_back(byte4);
            }

            std::vector<data_type>::iterator iter;
            size_type i = 0;
            data_type *tmp = reinterpret_cast<data_type *>(string);

            for(iter = bytes.begin(); iter != bytes.end(); iter++, i++)
            {
                *(tmp + i) = *iter;
            }

            return i;
        }


        JSONStringEncoder::size_type JSONStringEncoder::arrayIndexOfCharacterAtCharacterIndex(const char_type *string,
                                                                                              size_type length,
                                                                                              size_type index)
        {
            const data_type *initialUTF8 = reinterpret_cast<const data_type *>(string);
            size_type initialUTF8Length = length;
            size_type counter = 0;
            size_type i, incrementValue, character;
            bool loopFinished = false;
            byte_order_query_type byteOrderMarkQuery = this->hasByteOrderMark(string, length);

            if(byteOrderMarkQuery.first)
            {
                initialUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialUTF8Length -= this->lengthOfByteOrderMarkInBytes();
                counter += this->lengthOfByteOrderMarkInBytes();
            }

            if(index == 0)
                return counter;

            i = 0;
            character = 0;

            while(!loopFinished)
            {
                incrementValue = bytesToExpectInJSONSequence(initialUTF8 + i, initialUTF8Length);
                i += incrementValue;
                counter += incrementValue;
                if(++character == index)
                {
                    return counter;
                }
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            throw std::range_error("arrayIndexOfCharacterAtCharacterIndex unable to find indicated character");
        }


        JSONStringEncoder::size_type JSONStringEncoder::numberOfBytesToCaptureCharactersInRange(const char_type *string,
                                                                                                size_type length,
                                                                                                const range_type &range)
        {
            if(!this->doesRangeOfCharactersLieInString(string, length, range))
                throw std::range_error("numberOfBytesToCaptureCharactersInRange given string, and range where range"
                                       " of characters lies outside of string");

            size_type indexOfFirstCharacter =
                this->arrayIndexOfCharacterAtCharacterIndex(string, length, range.position);
            size_type indexOfLastCharacter =
                this->arrayIndexOfCharacterAtCharacterIndex(string, length, range.position + range.length);

            return indexOfLastCharacter - indexOfFirstCharacter;
        }


        bool JSONStringEncoder::containsCharacterWithZeroValue(const char_type *string, size_type length)
        {
            const data_type *initialUTF8 = reinterpret_cast<const data_type *>(string);
            size_type initialUTF8Length = length;
            size_type i;
            size_type incrementValue;
            bool loopFinished = false;
            byte_order_query_type queryResult = this->hasByteOrderMark(string, length);

            if(queryResult.first)
            {
                initialUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            // Now convert the characters
            i = 0;
            while(!loopFinished)
            {
                unicode_point_type theCode;
                incrementValue = bytesToExpectInJSONSequence(initialUTF8 + i, initialUTF8Length);
                theCode = convertJSONSequenceToUnicodePoint(initialUTF8 + i, incrementValue);
                if(theCode == 0)
                    return true;
                i += incrementValue;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return false;
        }


        bool JSONStringEncoder::containsCharacterNotInASCIIRange(const char_type *string, size_type length)
        {
            const data_type *initialUTF8 = reinterpret_cast<const data_type *>(string);
            size_type initialUTF8Length = length;
            size_type i;
            size_type incrementValue;
            bool loopFinished = false;
            byte_order_query_type queryResult = this->hasByteOrderMark(string, length);

            if(queryResult.first)
            {
                initialUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            // Now convert the characters
            i = 0;
            while(!loopFinished)
            {
                unicode_point_type theCode;
                incrementValue = bytesToExpectInJSONSequence(initialUTF8 + i, initialUTF8Length);
                theCode = convertJSONSequenceToUnicodePoint(initialUTF8 + i, incrementValue);
                if(theCode > 127 || theCode == 0)
                    return true;
                i += incrementValue;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return false;
        }


        JSONStringEncoder::range_type
            JSONStringEncoder::findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                const char_type *substringStart,
                                                                size_type substringLength)
        {
            range_type result;

            if(substringLength > stringLength)
                return result;

            checkStringForCorrectness(stringStart, stringLength);
            checkStringForCorrectness(substringStart, substringLength);

            const data_type *initialStringUTF8 = reinterpret_cast<const data_type *>(stringStart);
            const data_type *initialSubstringUTF8 = reinterpret_cast<const data_type *>(substringStart);

            size_type initialStringUTF8Length = stringLength;
            size_type initialSubstringUTF8Length = substringLength;

            byte_order_query_type stringQueryResult = this->hasByteOrderMark(stringStart, stringLength);
            byte_order_query_type substringQueryResult = this->hasByteOrderMark(substringStart, substringLength);

            if(stringQueryResult.first)
            {
                initialStringUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialStringUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            if(substringQueryResult.first)
            {
                initialSubstringUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialSubstringUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            // We can now just compare codes directly without worrying about the code contents.
            for(size_type i = 0; i < initialStringUTF8Length; i++)
            {
                if(*(initialStringUTF8 + i) == *(initialSubstringUTF8))
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if((initialSubstringUTF8Length) > (initialStringUTF8Length - i))
                    {
                        return result;
                    }

                    for(; j < (i + initialSubstringUTF8Length); j++, k++)
                    {
                        if(*(initialStringUTF8 + j) != *(initialSubstringUTF8 + k))
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
                        result.position = (stringQueryResult.first ? i + this->lengthOfByteOrderMarkInBytes() : i);
                        result.length = (j - i);
                        return result;
                    }
                }
            }

            return result;
        }


        JSONStringEncoder::range_array_type
            JSONStringEncoder::findByteRangesOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                 const char_type *substringStart,
                                                                 size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
                return rangeArray;

            checkStringForCorrectness(stringStart, stringLength);
            checkStringForCorrectness(substringStart, substringLength);

            const data_type *initialStringUTF8 = reinterpret_cast<const data_type *>(stringStart);
            const data_type *initialSubstringUTF8 = reinterpret_cast<const data_type *>(substringStart);

            size_type initialStringUTF8Length = stringLength;
            size_type initialSubstringUTF8Length = substringLength;

            byte_order_query_type stringQueryResult = this->hasByteOrderMark(stringStart, stringLength);
            byte_order_query_type substringQueryResult = this->hasByteOrderMark(substringStart, substringLength);

            if(stringQueryResult.first)
            {
                initialStringUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialStringUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            if(substringQueryResult.first)
            {
                initialSubstringUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialSubstringUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            // When iterating through the utf16 array, we can now just compare code by code without worrying about
            // the correctness of the string.
            for(size_type i = 0; i < initialStringUTF8Length; i++)
            {
                if(*(initialStringUTF8 + i) == *(initialSubstringUTF8))
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if((initialSubstringUTF8Length) > (initialStringUTF8Length - i))
                    {
                        return rangeArray;
                    }


                    for(; j < (i + initialSubstringUTF8Length); j++, k++)
                    {
                        if(*(initialStringUTF8 + j) != *(initialSubstringUTF8 + k))
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
                        range_type genericRange(stringQueryResult.first ? i + 3 : i, j - i);
                        rangeArray.push_back(genericRange);
                        i = j - 1;
                        continue;
                    }
                }
            }

            return rangeArray;
        }


        JSONStringEncoder::range_type JSONStringEncoder::findCharacterRangeForSubstringInString(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_type result;

            if(substringLength > stringLength)
                return result;

            checkStringForCorrectness(stringStart, stringLength);
            checkStringForCorrectness(substringStart, substringLength);

            size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);
            size_type substringNumberOfCharacters = this->numberOfCharacters(substringStart, substringLength);

            unicode_point_type subCode = this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, 0);

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                unicode_point_type stringCode = this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, i);

                if(stringCode == subCode)
                {
                    size_type j = i + 1, k = 1;
                    result.position = (i);
                    bool aborted = false;

                    if(substringNumberOfCharacters > (stringNumberOfCharacters - i))
                        return range_type();

                    for(; j < (i + substringNumberOfCharacters); j++, k++)
                    {
                        unicode_point_type stringCode2 =
                            this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, j);
                        unicode_point_type subCode2 =
                            this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, k);

                        if(stringCode2 != subCode2)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                    {
                        result.position = (0);
                        continue;
                    }
                    else
                    {
                        result.length = (substringNumberOfCharacters);
                        return result;
                    }
                }
            }

            return result;
        }


        JSONStringEncoder::range_array_type JSONStringEncoder::findCharacterRangesForSubstringInString(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
                return rangeArray;

            checkStringForCorrectness(stringStart, stringLength);
            checkStringForCorrectness(substringStart, substringLength);

            size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);
            size_type substringNumberOfCharacters = this->numberOfCharacters(substringStart, substringLength);

            unicode_point_type subCode = this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, 0);

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                unicode_point_type stringCode = this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, i);

                if(stringCode == subCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringNumberOfCharacters > (stringNumberOfCharacters - i))
                        return rangeArray;

                    for(; j < (i + substringNumberOfCharacters); j++, k++)
                    {
                        unicode_point_type stringCode2 =
                            this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, j);
                        unicode_point_type subCode2 =
                            this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, k);

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


        JSONStringEncoder::range_array_type JSONStringEncoder::findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
            {
                if(stringLength > 0)
                {
                    this->checkStringForCorrectness(stringStart, stringLength);
                    size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);
                    range_type theRange(0, stringNumberOfCharacters);
                    rangeArray.push_back(theRange);
                }
                return rangeArray;
            }

            // Check both strings for UTF-8 correctness
            checkStringForCorrectness(stringStart, stringLength);
            checkStringForCorrectness(substringStart, substringLength);

            size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);
            size_type substringNumberOfCharacters = this->numberOfCharacters(substringStart, substringLength);

            unicode_point_type subCode = this->unicodeCodePointForCharacterAtIndex(substringStart, substringLength, 0);

            size_type location = 0;
            size_type length = 0;

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                unicode_point_type stringCode = this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, i);

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
                        unicode_point_type stringCode2 =
                            this->unicodeCodePointForCharacterAtIndex(stringStart, stringLength, j);
                        unicode_point_type subCode2 =
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


        ComparisonResult JSONStringEncoder::compareStrings(const char_type *firstStringStart,
                                                           size_type firstStringLength,
                                                           const char_type *secondStringStart,
                                                           size_type secondStringLength)
        {
            size_type firstStringNumberOfCharacters = this->numberOfCharacters(firstStringStart, firstStringLength);
            size_type secondStringNumberOfCharacters = this->numberOfCharacters(secondStringStart, secondStringLength);

            size_type numberOfCharactersToCompare = firstStringNumberOfCharacters >= secondStringNumberOfCharacters
                                                        ? secondStringNumberOfCharacters
                                                        : firstStringNumberOfCharacters;

            unicode_point_type code1;
            unicode_point_type code2;

            for(size_type i = 0; i < numberOfCharactersToCompare; i++)
            {
                code1 = this->unicodeCodePointForCharacterAtIndex(firstStringStart, firstStringLength, i);
                code2 = this->unicodeCodePointForCharacterAtIndex(secondStringStart, secondStringLength, i);

                if(code1 < code2)
                    return OrderedAscending;
                if(code1 > code2)
                    return OrderedDescending;
            }

            if(firstStringNumberOfCharacters == secondStringNumberOfCharacters)
                return OrderedSame;
            else if(firstStringNumberOfCharacters > secondStringNumberOfCharacters)
                return OrderedAscending;

            return OrderedDescending;
        }


        void JSONStringEncoder::convertStringCharacters(char_type *string, size_type length, StringCase convertToCase)
        {
            size_type stringNumberOfCharacters = numberOfCharacters(string, length);

            for(size_type i = 0; i < stringNumberOfCharacters; i++)
            {
                unicode_point_type code = this->unicodeCodePointForCharacterAtIndex(string, length, i);

                // ASCII values A-Z are exactly 32 values from the counterpar a-z characters.

                size_type index = this->arrayIndexOfCharacterAtCharacterIndex(string, length, i);

                if(convertToCase == StringCase::UpperCase)
                {
                    // ASCII codes 97 - 122 are a - z.
                    if(code >= 97 && code <= 122)
                    {
                        *(string + index) = code - 32;
                    }
                }
                if(convertToCase == StringCase::LowerCase)
                {
                    // ASCII codes 65 - 90 are A - Z
                    if(code >= 65 && code <= 90)
                        *(string + index) = code + 32;
                }
            }
        }


        JSONStringEncoder::size_type JSONStringEncoder::computeArraySizeInBytesForStringByReplacingSubstrings(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength, const char_type *replaceStringStart, size_type replaceStringLength,
            range_array_type &ranges)
        {
            size_type newsize = 0;

            if(substringLength > stringLength)
            {
                if(stringLength > 0)
                {
                    checkStringForCorrectness(stringStart, stringLength);
                    return stringLength;
                }
                return 0;
            }

            checkStringForCorrectness(stringStart, stringLength);
            checkStringForCorrectness(substringStart, substringLength);

            const data_type *initialStringUTF8 = reinterpret_cast<const data_type *>(stringStart);
            const data_type *initialSubstringUTF8 = reinterpret_cast<const data_type *>(substringStart);

            size_type initialStringUTF8Length = stringLength;
            size_type initialSubstringUTF8Length = substringLength;

            byte_order_query_type stringQueryResult = this->hasByteOrderMark(stringStart, stringLength);
            byte_order_query_type substringQueryResult = this->hasByteOrderMark(substringStart, substringLength);

            if(stringQueryResult.first)
            {
                initialStringUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialStringUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            if(substringQueryResult.first)
            {
                initialSubstringUTF8 += this->lengthOfByteOrderMarkInBytes();
                initialSubstringUTF8Length -= this->lengthOfByteOrderMarkInBytes();
            }

            // We should now just be able to compare code by code without worrying.
            for(size_type i = 0; i < initialStringUTF8Length; i++)
            {
                if(*(initialStringUTF8 + i) == *initialSubstringUTF8)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(initialSubstringUTF8Length > (initialStringUTF8Length - i))
                    {
                        return newsize + initialStringUTF8Length - i;
                    }


                    for(; j < (i + initialSubstringUTF8Length); j++, k++)
                    {
                        if(*(initialStringUTF8 + j) != *(initialSubstringUTF8 + k))
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
                        range_type genericRange(i, j - i);
                        ranges.push_back(genericRange);
                        newsize += replaceStringLength;
                        i = j - 1;
                        continue;
                    }
                }

                newsize++;
            }

            return newsize;
        }


        void JSONStringEncoder::replaceOccurancesOfStringWithString(
            const char_type *originalStringStart, size_type originalStringLength, char_type *newStringStart,
            size_type newStringLength, const char_type *replacementStringStart, size_type replacementStringLength,
            range_array_type &substringRanges)
        {
            bool endOfRanges = false;

            if(substringRanges.size() == 0)
                endOfRanges = true;

            char_type *newStringTmp = newStringStart;

            auto rangeIter = substringRanges.begin();

            for(size_type i = 0; i < originalStringLength; i++)
            {
                if(!endOfRanges && i == rangeIter->position)
                {
                    std::memcpy(reinterpret_cast<void *>(newStringTmp),
                                reinterpret_cast<void *>(const_cast<char_type *>(replacementStringStart)),
                                replacementStringLength * sizeof(char_type));
                    newStringTmp += replacementStringLength * sizeof(char_type);
                    i += rangeIter->length - 1;
                    rangeIter++;
                    if(rangeIter == substringRanges.end())
                        endOfRanges = true;
                }
                else
                {
                    *newStringTmp = *(originalStringStart + i);
                    newStringTmp++;
                }
            }
        }


        JSONStringEncoder::parent_type::unicode_point_type
            JSONStringEncoder::correctValueForPlatform(const char_type *string, size_type length, Endian endian)
        {
            Endian thisEndian = this->thisSystemEndianness();
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            theNext = this->nextCodePoint(string, length, thisEndian);

            return theNext.first;
        }


        bool JSONStringEncoder::operator==(const StringEncoder &e)
        {
            return this->getEncoderID() == e.getEncoderID();
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


        JSONStringEncoder::size_type JSONStringEncoder::bytesToExpectInJSONSequence(const data_type *s,
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

            if(value >= 0 && value <= 0x7F)
                return 1;
            if(value >= 0xC2 && value <= 0xDF)
                return 2;
            if(value >= 0xE0 && value <= 0xEF)
                return 3;
            if(value >= 0xF0 && value <= 0xF4)
                return 4;
            throw std::range_error("bytesToExpectInJSONSequence` given value that lies outside the acceptable range"
                                   " for UTF-8 code prefix bytes");
        }


        JSONStringEncoder::parent_type::unicode_point_type
            JSONStringEncoder::convertJSONSequenceToUnicodePoint(const data_type *string, size_type length)
        {
            parent_type::unicode_point_type theCode = 0;

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
                        auto low_code = low_surrogate_rval.second.m_code;
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

            if(length == 1)
            {
                theCode = static_cast<parent_type::unicode_point_type>(*string);
            }
            else if(length == 2)
            {
                // Mask off the low 5 bits of the first byte.
                data_type byte1 = *(string)&0x1F;
                // Mask off the low 6 bits of the second byte.
                data_type byte2 = *(string + 1) & 0x3F;
                theCode = static_cast<parent_type::unicode_point_type>((byte1 << 6) | byte2);
            }
            else if(length == 3)
            {
                // Mask off the low 4 bits of the first byte
                data_type byte1 = *(string)&0xF;
                // Mask off the low 6 bits of the second byte
                data_type byte2 = *(string + 1) & 0x3F;
                // Mask off the low 6 bits of the third byte
                data_type byte3 = *(string + 2) & 0x3F;
                theCode = static_cast<parent_type::unicode_point_type>((byte1 << 12) | (byte2 << 6) | byte3);
            }
            else
            {
                // Mask off the low 3 bytes of the first byte
                data_type byte1 = *(string)&0x7;
                // Mask off the low 6 bytes of the second byte
                data_type byte2 = *(string + 1) & 0x3F;
                // Mask off the low 6 bytes of the third byte
                data_type byte3 = *(string + 2) & 0x3F;
                // Mask off the low 6 bytes of the fourth byte
                data_type byte4 = *(string + 3) & 0x3F;
                theCode =
                    static_cast<parent_type::unicode_point_type>((byte1 << 18) | (byte2 << 12) | (byte3 << 6) | byte4);
            }

            return theCode;
        }


        JSONStringEncoder::size_type
            JSONStringEncoder::bytesNeededForUTF8RepresenationOfUnicodePoint(parent_type::unicode_point_type point)
        {
            /* codes between 0 and 0x7F are represented with one byte */
            if(point <= 0x7F)
            {
                return 1;
            }

            /* codes between 0x80 and 0x7FF are represented by two bytes */
            else if(point >= 0x80 && point <= 0x7FF)
            {
                return 2;
            }

            /* codes between 0x800 and 0xFFFF are represented by three bytes */
            else if(point >= 0x800 && point <= 0xFFFF)
            {
                return 3;
            }

            /* All remaining codes are represented by four bytes */
            return 4;
        }

        std::string JSONStringEncoder::getEncoderID() const
        {
            return std::string("AC579979-0E2F-4F6B-A6DE-5B1AB203697E");
        }

    }    // namespace Foundation

}    // namespace TF
