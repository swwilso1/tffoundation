/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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
#include "tfutf8stringencoder.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        const UTF8StringEncoder::size_type UTF8StringEncoder::byteOrderMarkLength = 3;


        UTF8StringEncoder::~UTF8StringEncoder()
        {
        }


        StringEncoder *UTF8StringEncoder::clone()
        {
            return new UTF8StringEncoder;
        }


        UTF8StringEncoder::size_type UTF8StringEncoder::basicCodeLengthInBytes()
        {
            return sizeof(data_type);
        }


        bool UTF8StringEncoder::hasFixedCodeLength()
        {
            return false;
        }


        UTF8StringEncoder::size_type UTF8StringEncoder::numberOfBytesRequiredForLargestCharacterValue()
        {
            return 4 * sizeof(data_type);
        }


        bool UTF8StringEncoder::canUseByteOrderMark()
        {
            return true;
        }


        bool UTF8StringEncoder::usesByteOrderMark()
        {
            return false;
        }


        UTF8StringEncoder::size_type UTF8StringEncoder::lengthOfByteOrderMarkInBytes()
        {
            return byteOrderMarkLength;
        }


        void UTF8StringEncoder::writeByteOrderMark(char_type *string, size_type length)
        {
            if(length < byteOrderMarkLength)
                throw std::runtime_error("writeByteOrderMark cannot write byte order mark, destination array too"
                                         " small");

            data_type *tmp = reinterpret_cast<data_type *>(string);

            *(tmp) = 0xEF;
            *(tmp + 1) = 0xBB;
            *(tmp + 2) = 0xBF;
        }


        UTF8StringEncoder::byte_order_query_type UTF8StringEncoder::hasByteOrderMark(const char_type *string,
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


        UTF8StringEncoder::size_type UTF8StringEncoder::numberOfCharacters(const char_type *string, size_type length)
        {
            const data_type *initialUTF8 = reinterpret_cast<const data_type *>(string);
            size_type initialUTF8Length = length;
            size_type i, counter = 0, incrementValue;
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

            i = 0;

            while(!loopFinished)
            {
                incrementValue = bytesToExpectInUTF8Sequence(*(initialUTF8 + i));
                initialUTF8 += incrementValue;
                counter++;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return counter;
        }


        bool UTF8StringEncoder::checkStringForCorrectness(const char_type *string, size_type length)
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
                incrementValue = bytesToExpectInUTF8Sequence(*(initialUTF8 + i));
                theCode = convertUTF8SequenceToUnicodePoint(initialUTF8 + i, incrementValue);
                if(theCode > 0x10FFFF)
                    return false;
                i += incrementValue;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return true;
        }


        std::pair<UTF8StringEncoder::parent_type::unicode_point_type, UTF8StringEncoder::size_type>
            UTF8StringEncoder::nextCodePoint(const char_type *string, size_type length, Endian endian)
        {
            size_type incrementValue = bytesToExpectInUTF8Sequence(*string);
            parent_type::unicode_point_type theCode =
                convertUTF8SequenceToUnicodePoint(reinterpret_cast<const data_type *>(string), incrementValue);
            return std::make_pair(theCode, incrementValue);
        }


        std::pair<UTF8StringEncoder::parent_type::unicode_point_type, UTF8StringEncoder::size_type>
            UTF8StringEncoder::nextCode(const char_type *string, size_type length, Endian endian)
        {
            if(length == 0)
                throw std::runtime_error("nextCode cannot calculate next code point, length of data array is zero.");
            return std::make_pair(static_cast<parent_type::unicode_point_type>(*string), 1);
        }


        UTF8StringEncoder::parent_type::unicode_point_type
            UTF8StringEncoder::unicodeCodePointForCharacterAtIndex(const char_type *string, size_type length,
                                                                   size_type index)
        {
            size_type theIndex = arrayIndexOfCharacterAtCharacterIndex(string, length, index);
            size_type interval = bytesToExpectInUTF8Sequence(static_cast<const data_type>(*(string + theIndex)));
            parent_type::unicode_point_type theCode =
                convertUTF8SequenceToUnicodePoint(reinterpret_cast<const data_type *>(string + theIndex), interval);
            return theCode;
        }


        UTF8StringEncoder::size_type UTF8StringEncoder::bytesNeededForRepresentationOfCode(unicode_point_type code)
        {
            return bytesNeededForUTF8RepresenationOfUnicodePoint(code);
        }


        UTF8StringEncoder::size_type UTF8StringEncoder::encodeCodePoint(char_type *string, size_type length,
                                                                        unicode_point_type code, Endian endian)
        {
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


        UTF8StringEncoder::size_type UTF8StringEncoder::arrayIndexOfCharacterAtCharacterIndex(const char_type *string,
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
                incrementValue = bytesToExpectInUTF8Sequence(*(initialUTF8 + i));
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


        UTF8StringEncoder::size_type UTF8StringEncoder::numberOfBytesToCaptureCharactersInRange(const char_type *string,
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


        bool UTF8StringEncoder::containsCharacterWithZeroValue(const char_type *string, size_type length)
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
                incrementValue = bytesToExpectInUTF8Sequence(*(initialUTF8 + i));
                theCode = convertUTF8SequenceToUnicodePoint(initialUTF8 + i, incrementValue);
                if(theCode == 0)
                    return true;
                i += incrementValue;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return false;
        }


        bool UTF8StringEncoder::containsCharacterNotInASCIIRange(const char_type *string, size_type length)
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
                incrementValue = bytesToExpectInUTF8Sequence(*(initialUTF8 + i));
                theCode = convertUTF8SequenceToUnicodePoint(initialUTF8 + i, incrementValue);
                if(theCode > 127 || theCode == 0)
                    return true;
                i += incrementValue;
                if(incrementValue >= initialUTF8Length)
                    loopFinished = true;
                initialUTF8Length -= incrementValue;
            }

            return false;
        }


        UTF8StringEncoder::range_type
            UTF8StringEncoder::findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
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


        UTF8StringEncoder::range_array_type
            UTF8StringEncoder::findByteRangesOfSubstringInString(const char_type *stringStart, size_type stringLength,
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


        UTF8StringEncoder::range_type UTF8StringEncoder::findCharacterRangeForSubstringInString(
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


        UTF8StringEncoder::range_array_type UTF8StringEncoder::findCharacterRangesForSubstringInString(
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


        UTF8StringEncoder::range_array_type UTF8StringEncoder::findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
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


        ComparisonResult UTF8StringEncoder::compareStrings(const char_type *firstStringStart,
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


        void UTF8StringEncoder::convertStringCharacters(char_type *string, size_type length, StringCase convertToCase)
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


        UTF8StringEncoder::size_type UTF8StringEncoder::computeArraySizeInBytesForStringByReplacingSubstrings(
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


        void UTF8StringEncoder::replaceOccurancesOfStringWithString(
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


        UTF8StringEncoder::parent_type::unicode_point_type
            UTF8StringEncoder::correctValueForPlatform(const char_type *string, size_type length, Endian endian)
        {
            Endian thisEndian = this->thisSystemEndianness();
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            theNext = this->nextCodePoint(string, length, thisEndian);

            return theNext.first;
        }


        bool UTF8StringEncoder::operator==(const StringEncoder &e)
        {
            try
            {
                const UTF8StringEncoder &theEncoder = dynamic_cast<const UTF8StringEncoder &>(e);
                return true;
            }
            catch(std::bad_cast e)
            {
                ;
            }

            return false;
        }


        std::ostream &UTF8StringEncoder::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("UTF8StringEncoder");
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        UTF8StringEncoder::size_type UTF8StringEncoder::bytesToExpectInUTF8Sequence(const data_type value)
        {
            if(value >= 0 && value <= 0x7F)
                return 1;
            if(value >= 0xC2 && value <= 0xDF)
                return 2;
            if(value >= 0xE0 && value <= 0xEF)
                return 3;
            if(value >= 0xF0 && value <= 0xF4)
                return 4;
            throw std::range_error("bytesToExpectInUTF8Sequence given value that lies outside the acceptable range"
                                   " for UTF-8 code prefix bytes");
        }


        UTF8StringEncoder::parent_type::unicode_point_type
            UTF8StringEncoder::convertUTF8SequenceToUnicodePoint(const data_type *string, size_type length)
        {
            parent_type::unicode_point_type theCode = 0;

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


        UTF8StringEncoder::size_type
            UTF8StringEncoder::bytesNeededForUTF8RepresenationOfUnicodePoint(parent_type::unicode_point_type point)
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

    }    // namespace Foundation

}    // namespace TF
