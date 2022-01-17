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
#include "tfheaders.hpp"
#include "tfasciistringencoder.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        ASCIIStringEncoder::~ASCIIStringEncoder()
        {
        }

        StringEncoder *ASCIIStringEncoder::clone(void)
        {
            return new ASCIIStringEncoder;
        }

        ASCIIStringEncoder::size_type ASCIIStringEncoder::basicCodeLengthInBytes(void)
        {
            return 1;
        }

        bool ASCIIStringEncoder::hasFixedCodeLength(void)
        {
            return false;
        }

        ASCIIStringEncoder::size_type ASCIIStringEncoder::numberOfBytesRequiredForLargestCharacterValue(void)
        {
            return 8;    // \:10FFFF
        }


        bool ASCIIStringEncoder::canUseByteOrderMark(void)
        {
            return false;
        }


        bool ASCIIStringEncoder::usesByteOrderMark(void)
        {
            return false;
        }


        ASCIIStringEncoder::size_type ASCIIStringEncoder::lengthOfByteOrderMarkInBytes(void)
        {
            return 0;
        }


        void ASCIIStringEncoder::writeByteOrderMark(char_type *string, size_type length)
        {
            return;
        }


        ASCIIStringEncoder::byte_order_query_type ASCIIStringEncoder::hasByteOrderMark(const char_type *string,
                                                                                       size_type length)
        {
            return std::make_pair(false, this->thisSystemEndianness());
        }


        ASCIIStringEncoder::size_type ASCIIStringEncoder::numberOfCharacters(const char_type *string, size_type length)
        {
            size_type characterCount = 0;

            for(size_type i = 0; i < length; i++)
            {
                if(*(string + i) == '\\')
                {
                    characterCount++;
                    if(i == (length - 1))
                        continue;

                    if(*(string + i + 1) == ':')
                    {
                        if((i + 7) < length)
                        {
                            size_type j, k;
                            bool aborted = false;
                            for(j = i + 2, k = 5; j < (i + 7); j++, k--)
                            {
                                if(*(string + j) >= 48 && *(string + j) <= 57)
                                    continue;
                                else if(*(string + j) >= 65 && *(string + j) <= 70)
                                    continue;
                                else
                                {
                                    // Found a non-allowed character, just move on to
                                    // the ':' character.
                                    aborted = true;
                                    break;
                                }
                            }
                            if(aborted)
                                continue;
                            i += 7;
                            continue;
                        }
                    }

                    continue;
                }
                characterCount++;
            }

            return characterCount;
        }


        bool ASCIIStringEncoder::checkStringForCorrectness(const char_type *string, size_type length)
        {
            for(size_type i = 0; i < length; i++)
            {
                if(*(string + i) == '\\')
                {
                    if(i == (length - 1))
                        continue;

                    if(*(string + i + 1) == ':')
                    {
                        if((i + 7) < length)
                        {
                            size_type j, k;
                            parent_type::unicode_point_type theCode = 0;
                            bool aborted = false;
                            for(j = i + 2, k = 5; j < (i + 7); j++, k--)
                            {
                                double base = std::pow(static_cast<double>(16.0), static_cast<double>(k));
                                size_type modifier;
                                if(*(string + j) >= 48 && *(string + j) <= 57)
                                    // Gives integer value 0-9
                                    modifier = static_cast<size_type>(*(string + j) - 48);
                                else if(*(string + j) >= 65 && *(string + j) <= 70)
                                    // Gives integer value 10-15 (A-F)
                                    modifier = static_cast<size_type>(*(string + j) - 55);
                                else
                                {
                                    // Found a non-allowed character, just move on to
                                    // the ':' character.
                                    aborted = true;
                                    break;
                                }

                                theCode += base * modifier;
                                if(theCode >= 0x10FFFF)
                                    return false;
                            }
                            if(aborted)
                                continue;
                            i += 7;
                            continue;
                        }
                    }
                }
            }

            return true;
        }


        std::pair<ASCIIStringEncoder::parent_type::unicode_point_type, ASCIIStringEncoder::size_type>
            ASCIIStringEncoder::nextCodePoint(const char_type *string, size_type length, Endian endian)
        {
            parent_type::unicode_point_type theCode = 0;

            if(*string == '\\')
            {
                if(length == 1)
                    return std::make_pair(static_cast<parent_type::unicode_point_type>(*string), 1);

                if(*(string + 1) == ':')
                {
                    if(7 < length)
                    {
                        size_type j, k;
                        for(j = 2, k = 5; j <= 7; j++, k--)
                        {
                            double base = std::pow(static_cast<double>(16.0), static_cast<double>(k));
                            size_type modifier;
                            if(*(string + j) >= 48 && *(string + j) <= 57)
                                // Gives integer value 0-9
                                modifier = static_cast<size_type>(*(string + j) - 48);
                            else if(*(string + j) >= 65 && *(string + j) <= 70)
                                // Gives integer value 10-15 (A-F)
                                modifier = static_cast<size_type>(*(string + j) - 55);
                            else
                            {
                                // We found a non-allowed character so we rewind back to the '\' character...
                                theCode = static_cast<parent_type::unicode_point_type>(*string);
                                return std::make_pair(theCode, 1);
                            }

                            theCode += base * modifier;
                        }

                        return std::make_pair(theCode, 8);
                    }
                }
            }

            theCode = static_cast<parent_type::unicode_point_type>(*string);
            return std::make_pair(theCode, 1);
        }


        std::pair<ASCIIStringEncoder::parent_type::unicode_point_type, ASCIIStringEncoder::size_type>
            ASCIIStringEncoder::nextCode(const char_type *string, size_type length, Endian endian)
        {
            if(length == 0)
                throw std::runtime_error("nextCode unable to calculate next code point from zero length array");
            return std::make_pair(static_cast<parent_type::unicode_point_type>(*string), 1);
        }


        ASCIIStringEncoder::parent_type::unicode_point_type
            ASCIIStringEncoder::unicodeCodePointForCharacterAtIndex(const char_type *string, size_type length,
                                                                    size_type index)
        {
            const char_type *tmp = string;
            size_type tmpLength = length;
            size_type indexCounter = 0;
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            Endian thisEndian = thisSystemEndianness();

            while(indexCounter <= index)
            {
                theNext = this->nextCodePoint(tmp, tmpLength, thisEndian);
                tmp += theNext.second;
                tmpLength -= theNext.second;
                indexCounter++;
            }

            return theNext.first;
        }


        ASCIIStringEncoder::size_type
            ASCIIStringEncoder::bytesNeededForRepresentationOfCode(parent_type::unicode_point_type code)
        {
            if(code > 0 && code < 128)
                return 1;
            return this->numberOfBytesRequiredForLargestCharacterValue();
        }


        ASCIIStringEncoder::size_type ASCIIStringEncoder::encodeCodePoint(char_type *string, size_type length,
                                                                          parent_type::unicode_point_type code,
                                                                          Endian endian)
        {
            if(this->bytesNeededForRepresentationOfCode(code) > length)
                throw std::runtime_error("encodeCodePoint not given enough space to encode character");

            if(code > 0 && code < 128)
            {
                *string = static_cast<char_type>(code);
                return 1;
            }

            if(code == 0)
            {
                *string = '\\';
                *(string + 1) = ':';
                *(string + 2) = '0';
                *(string + 3) = '0';
                *(string + 4) = '0';
                *(string + 5) = '0';
                *(string + 6) = '0';
                *(string + 7) = '0';
                return 8;
            }

            *string = '\\';
            *(string + 1) = ':';

            char_type *tmp = string + 2;
            parent_type::unicode_point_type tmpCode = code;
            size_type i = 5, initialI = 5;

            while(tmpCode > 0)
            {
                parent_type::unicode_point_type theValue = tmpCode % 16;
                if(theValue < 10)
                {
                    *(tmp + i) = static_cast<char_type>(48 + theValue);
                }
                else
                {
                    *(tmp + i) = static_cast<char_type>(65 + theValue - 10);
                }
                tmpCode /= 16;
                i--;
            }

            while(i < initialI)
            {
                *(tmp + i) = '0';
                i--;
            }

            return this->numberOfBytesRequiredForLargestCharacterValue();
        }


        ASCIIStringEncoder::size_type ASCIIStringEncoder::arrayIndexOfCharacterAtCharacterIndex(const char_type *string,
                                                                                                size_type length,
                                                                                                size_type index)
        {
            const char_type *tmp = string;
            size_type tmpLength = length;
            size_type indexCounter = 0;
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            Endian thisEndian = thisSystemEndianness();

            theNext = this->nextCodePoint(tmp, tmpLength, thisEndian);

            while(indexCounter < index)
            {
                tmp += theNext.second;
                tmpLength -= theNext.second;
                indexCounter++;
                if(indexCounter < index)
                    theNext = this->nextCodePoint(tmp, tmpLength, thisEndian);
            }

            return tmp - string;
        }


        ASCIIStringEncoder::size_type
            ASCIIStringEncoder::numberOfBytesToCaptureCharactersInRange(const char_type *string, size_type length,
                                                                        const range_type &range)
        {
            if(!this->doesRangeOfCharactersLieInString(string, length, range))
                throw std::range_error("numberOfBytesToCaptureCharactersInRange asked to capture characters from "
                                       "range that lies outside the length of the string");

            size_type firstCharacterIndex;
            const char_type *tmp = string;
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            size_type theLength;
            size_type i = 0, byteAccumulator = 0;
            Endian theEndian = this->thisSystemEndianness();

            firstCharacterIndex = this->arrayIndexOfCharacterAtCharacterIndex(string, length, range.position);
            tmp += firstCharacterIndex;
            theLength = length - firstCharacterIndex;

            theNext = nextCodePoint(tmp, theLength, theEndian);

            while(i < range.length)
            {
                tmp += theNext.second;
                theLength -= theNext.second;
                byteAccumulator += theNext.second;
                i++;
                if(i < range.length)
                    theNext = nextCodePoint(tmp, theLength, theEndian);
            }

            return byteAccumulator;
        }


        bool ASCIIStringEncoder::containsCharacterWithZeroValue(const char_type *string, size_type length)
        {
            const char_type *tmp = string;
            size_type theLength = length;
            Endian theEndian = this->thisSystemEndianness();
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            while(theLength > 0)
            {
                theNext = this->nextCodePoint(tmp, theLength, theEndian);
                if(theNext.first == 0)
                    return true;
                tmp += theNext.second;
                theLength -= theNext.second;
            }

            return false;
        }


        bool ASCIIStringEncoder::containsCharacterNotInASCIIRange(const char_type *string, size_type length)
        {
            const char_type *tmp = string;
            size_type theLength = length;
            Endian theEndian = this->thisSystemEndianness();
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            while(theLength > 0)
            {
                theNext = this->nextCodePoint(tmp, theLength, theEndian);
                if(theNext.first == 0 || theNext.first > 127)
                    return true;
                tmp += theNext.second;
                theLength -= theNext.second;
            }

            return false;
        }


        ASCIIStringEncoder::range_type
            ASCIIStringEncoder::findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                 const char_type *substringStart,
                                                                 size_type substringLength)
        {
            range_type result;

            if(substringLength > stringLength)
                return result;

            for(size_type i = 0; i < stringLength; i++)
            {
                if(*(stringStart + i) == *substringStart)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringLength > (stringLength - i))
                        return result;

                    for(; j < (i + substringLength); j++, k++)
                    {
                        if(*(stringStart + j) != *(substringStart + k))
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


        ASCIIStringEncoder::range_array_type
            ASCIIStringEncoder::findByteRangesOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                  const char_type *substringStart,
                                                                  size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
                return rangeArray;

            for(size_type i = 0; i < stringLength; i++)
            {
                if(*(stringStart + i) == *substringStart)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringLength > (stringLength - i))
                        return rangeArray;

                    for(; j < (i + substringLength); j++, k++)
                    {
                        if(*(stringStart + j) != *(substringStart + k))
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                        continue;
                    else
                    {
                        range_type genericRange(i, substringLength);
                        rangeArray.push_back(genericRange);
                        i = j - 1;
                        continue;
                    }
                }
            }

            return rangeArray;
        }


        ASCIIStringEncoder::range_type ASCIIStringEncoder::findCharacterRangeForSubstringInString(
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
                        return range_type();

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


        ASCIIStringEncoder::range_array_type ASCIIStringEncoder::findCharacterRangesForSubstringInString(
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


        ASCIIStringEncoder::range_array_type ASCIIStringEncoder::findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            if(substringLength > stringLength)
            {
                if(stringLength > 0)
                {
                    size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);
                    range_type theRange(0, stringNumberOfCharacters);
                    rangeArray.push_back(theRange);
                }
                return rangeArray;
            }

            size_type stringNumberOfCharacters = this->numberOfCharacters(stringStart, stringLength);
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


        void ASCIIStringEncoder::convertStringCharacters(char_type *string, size_type length, StringCase convertToCase)
        {
            char_type *tmp = string;
            size_type theLength = length;
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            size_type theNumberOfCharacters = this->numberOfCharacters(string, length);
            Endian thisEndian = this->thisSystemEndianness();

            for(size_type i = 0; i < theNumberOfCharacters; i++)
            {
                theNext = this->nextCodePoint(tmp, theLength, thisEndian);
                if(theNext.second == 1)
                {
                    if(convertToCase == StringCase::UpperCase)
                    {
                        // ASCII codes 97 - 122 are a - z.
                        if(*tmp >= 97 && *tmp <= 122)
                        {
                            *tmp -= 32;
                        }
                    }
                    if(convertToCase == StringCase::LowerCase)
                    {
                        // ASCII codes 65 - 90 are A - Z
                        if(*tmp >= 65 && *tmp <= 90)
                            *tmp += 32;
                    }
                }

                tmp += theNext.second;
                theLength -= theNext.second;
            }
        }


        ASCIIStringEncoder::size_type ASCIIStringEncoder::computeArraySizeInBytesForStringByReplacingSubstrings(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength, const char_type *replaceStringStart, size_type replaceStringLength,
            range_array_type &ranges)
        {
            size_type newsize = 0;

            if(substringLength > stringLength)
            {
                if(stringLength > 0)
                    return stringLength;
                else
                    return 0;
            }

            for(size_type i = 0; i < stringLength; i++)
            {
                if(*(stringStart + i) == *substringStart)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringLength > (stringLength - i))
                        return newsize + stringLength - i;

                    for(; j < (i + substringLength); j++, k++)
                    {
                        if(*(stringStart + j) != *(substringStart + k))
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
                        newsize += replaceStringLength;
                        i = j - 1;
                        continue;
                    }
                }

                newsize++;
            }

            return newsize;
        }


        void ASCIIStringEncoder::replaceOccurancesOfStringWithString(
            const char_type *originalStringStart, size_type originalStringLength, char_type *newStringStart,
            size_type newStringLength, const char_type *replacementStringStart, size_type replacementStringLength,
            range_array_type &substringRanges)
        {
            bool endOfRanges = false;

            char_type *newStringTmp = newStringStart;

            if(substringRanges.size() == 0)
                endOfRanges = true;


            auto rangeIter = substringRanges.begin();

            for(size_type i = 0; i < originalStringLength; i++)
            {
                if(!endOfRanges && i == (*rangeIter).position)
                {
                    std::memcpy(reinterpret_cast<void *>(newStringTmp),
                                reinterpret_cast<void *>(const_cast<char_type *>(replacementStringStart)),
                                replacementStringLength * sizeof(char_type));
                    newStringTmp += replacementStringLength * sizeof(char_type);
                    i += (*rangeIter).length - 1;
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


        ASCIIStringEncoder::parent_type::unicode_point_type
            ASCIIStringEncoder::correctValueForPlatform(const char_type *string, size_type length, Endian endian)
        {
            Endian thisEndian = this->thisSystemEndianness();
            std::pair<parent_type::unicode_point_type, size_type> theNext;

            theNext = this->nextCodePoint(string, length, thisEndian);

            return theNext.first;
        }


        bool ASCIIStringEncoder::operator==(const StringEncoder &e)
        {
            return this->getEncoderID() == e.getEncoderID();
        }


        std::ostream &ASCIIStringEncoder::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("ASCIIStringEncoder");
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        std::string ASCIIStringEncoder::getEncoderID() const
        {
            return std::string("58C183D5-08DE-4B9F-A3FE-FB75017A9C87");
        }


    }    // namespace Foundation


}    // namespace TF
