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
#include "tfheaders.hpp"
#include "tfutf32stringencoder.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        const UTF32StringEncoder::size_type UTF32StringEncoder::byteOrderMarkLength = sizeof(data_type);

        const UTF32StringEncoder::size_type UTF32StringEncoder::largestCodeLength = sizeof(data_type);


        StringEncoder *UTF32StringEncoder::clone(void)
        {
            return new UTF32StringEncoder;
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::basicCodeLengthInBytes(void)
        {
            return sizeof(data_type);
        }


        bool UTF32StringEncoder::hasFixedCodeLength(void)
        {
            return true;
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::numberOfBytesRequiredForLargestCharacterValue(void)
        {
            return sizeof(data_type);
        }


        bool UTF32StringEncoder::canUseByteOrderMark(void)
        {
            return true;
        }


        bool UTF32StringEncoder::usesByteOrderMark(void)
        {
            return true;
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::lengthOfByteOrderMarkInBytes(void)
        {
            return byteOrderMarkLength;
        }


        void UTF32StringEncoder::writeByteOrderMark(char_type *start, size_type length)
        {
            if(length < byteOrderMarkLength)
                throw std::runtime_error("writeByteOrderMark unable to write mark, destination array too small");

            data_type *stringStart = reinterpret_cast<data_type *>(start);
            *stringStart = 0xFEFF;
        }


        UTF32StringEncoder::byte_order_query_type UTF32StringEncoder::hasByteOrderMark(const char_type *start,
                                                                                       size_type length)
        {
            endian theStringEndianness = Endian::BigEndian;
            bool doesHaveBOM = false;

            if(start == NULL || length == 0)
                return std::make_pair(false, this->thisSystemEndianness());

            const unsigned char *bom = reinterpret_cast<const unsigned char *>(start);
            if(*(bom) == 0 && *(bom + 1) == 0 && *(bom + 2) == 0xFE && *(bom + 3) == 0xFF)
            {
                theStringEndianness = Endian::BigEndian;
                doesHaveBOM = true;
            }
            else if(*(bom) == 0xFF && *(bom + 1) == 0xFE && *(bom + 2) == 0 && *(bom + 3) == 0)
            {
                theStringEndianness = Endian::LittleEndian;
                doesHaveBOM = true;
            }

            return std::make_pair(doesHaveBOM, theStringEndianness);
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::numberOfCharacters(const char_type *start, size_type length)
        {
            size_type characterLength = length / sizeof(data_type);
            byte_order_query_type queryResults = hasByteOrderMark(start, length);

            if(queryResults.first)
            {
                characterLength--;
            }

            return characterLength;
        }


        bool UTF32StringEncoder::checkStringForCorrectness(const char_type *start, size_type length)
        {
            const char_type *stringTmp = start;
            size_type stringLength = length;
            std::pair<parent_type::unicode_point_type, size_type> theNext;
            byte_order_query_type stringQueryResult = this->hasByteOrderMark(start, length);

            if(stringQueryResult.first)
            {
                stringTmp += this->lengthOfByteOrderMarkInBytes();
                stringLength -= this->lengthOfByteOrderMarkInBytes();
            }

            while(stringLength > 0)
            {
                theNext = this->nextCodePoint(stringTmp, stringLength, stringQueryResult.second);
                if(theNext.first > 0x10FFFF)
                    return false;
                stringTmp += theNext.second;
                stringLength -= theNext.second;
            }

            return true;
        }


        std::pair<UTF32StringEncoder::parent_type::unicode_point_type, UTF32StringEncoder::size_type>
            UTF32StringEncoder::nextCodePoint(const char_type *start, size_type length, endian endian)
        {
            if(length < sizeof(data_type))
                throw std::runtime_error("nextCodePoint cannot calculate next code point, data array length less"
                                         " than size of data type");

            parent_type::unicode_point_type theCode = this->correctValueForPlatform(start, length, endian);
            return std::make_pair(theCode, sizeof(data_type));
        }


        std::pair<UTF32StringEncoder::parent_type::unicode_point_type, UTF32StringEncoder::size_type>
            UTF32StringEncoder::nextCode(const char_type *string, size_type length, endian endian)
        {
            if(length < sizeof(data_type))
                throw std::runtime_error("nextCode cannot calculate next code point, data array length less "
                                         " than size of data type");
            const data_type *theCodes = reinterpret_cast<const data_type *>(string);
            data_type theCode = *theCodes;
            parent_type::unicode_point_type finalCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&theCode), sizeof(data_type), endian);
            return std::make_pair(finalCode, sizeof(data_type));
        }

        UTF32StringEncoder::parent_type::unicode_point_type
            UTF32StringEncoder::unicodeCodePointForCharacterAtIndex(const char_type *start, size_type length,
                                                                    size_type index)
        {
            const data_type *characters = reinterpret_cast<const data_type *>(start);
            size_type characterLength = length / sizeof(data_type);
            byte_order_query_type stringQueryResults = this->hasByteOrderMark(start, length);

            if(stringQueryResults.first)
            {
                characters++;
                characterLength--;
            }

            if(index > characterLength)
                throw std::range_error("unicodeCodePointForCharacterAtIndex given an a character index that lies"
                                       " outside of the possible character range");

            characters += index;

            parent_type::unicode_point_type theCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(const_cast<data_type *>(characters)),
                                              sizeof(data_type), stringQueryResults.second);

            return theCode;
        }


        UTF32StringEncoder::size_type
            UTF32StringEncoder::bytesNeededForRepresentationOfCode(parent_type::unicode_point_type code)
        {
            return largestCodeLength;
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::encodeCodePoint(char_type *start, size_type length,
                                                                          parent_type::unicode_point_type code,
                                                                          endian endian)
        {
            if(length < sizeof(data_type))
                throw std::runtime_error("encodeCodePoint not given enough space to encode unicode point");

            data_type *theArray = reinterpret_cast<data_type *>(start);
            *theArray = this->correctValueForPlatform(reinterpret_cast<char_type *>(&code),
                                                      sizeof(parent_type::unicode_point_type), endian);

            return sizeof(data_type);
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::arrayIndexOfCharacterAtCharacterIndex(const char_type *start,
                                                                                                size_type length,
                                                                                                size_type index)
        {
            const data_type *characterStart = reinterpret_cast<const data_type *>(start);
            size_type characterLength = length / sizeof(data_type);
            byte_order_query_type queryResults = hasByteOrderMark(start, length);

            if(queryResults.first)
            {
                characterStart++;
                characterLength--;
            }

            if(index > characterLength)
                throw std::range_error("arrayIndexOfCharacterAtCharacterIndex given string and index where the index"
                                       " value lies outside the range of possible characters");

            const data_type *character = characterStart + index;

            return reinterpret_cast<const char_type *>(character) - start;
        }


        UTF32StringEncoder::size_type
            UTF32StringEncoder::numberOfBytesToCaptureCharactersInRange(const char_type *start, size_type length,
                                                                        const range_type &range)
        {
            if(!doesRangeOfCharactersLieInString(start, length, range))
                throw std::range_error("numberOfBytesToCaptureCharactersInRange given an range and a string where"
                                       " the range of characters lies outside the characters in the string");

            return range.length * sizeof(data_type);
        }


        bool UTF32StringEncoder::containsCharacterWithZeroValue(const char_type *start, size_type length)
        {
            const data_type *characterStart = reinterpret_cast<const data_type *>(start);
            size_type characterLength = length / sizeof(data_type);
            byte_order_query_type queryResults = hasByteOrderMark(start, length);
            parent_type::unicode_point_type theCode;

            if(queryResults.first)
            {
                characterStart++;
                characterLength--;
            }

            for(size_type i = 0; i < characterLength; i++)
            {
                theCode = *(characterStart + i);
                theCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&theCode),
                                                        sizeof(parent_type::unicode_point_type), queryResults.second);
                if(theCode == 0)
                    return true;
            }

            return false;
        }


        bool UTF32StringEncoder::containsCharacterNotInASCIIRange(const char_type *start, size_type length)
        {
            const data_type *characterStart = reinterpret_cast<const data_type *>(start);
            size_type characterLength = length / sizeof(data_type);
            byte_order_query_type queryResults = hasByteOrderMark(start, length);
            parent_type::unicode_point_type theCode;

            if(queryResults.first)
            {
                characterStart++;
                characterLength--;
            }

            for(size_type i = 0; i < characterLength; i++)
            {
                theCode = *(characterStart + i);
                theCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&theCode),
                                                        sizeof(parent_type::unicode_point_type), queryResults.second);
                if(theCode > 127)
                    return true;
            }

            return false;
        }


        UTF32StringEncoder::range_type
            UTF32StringEncoder::findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                 const char_type *substringStart,
                                                                 size_type substringLength)
        {
            range_type result;

            parent_type::unicode_point_type mainStringCode;
            parent_type::unicode_point_type substringCode;
            parent_type::unicode_point_type substringStartCode;

            const data_type *mainStringStart = reinterpret_cast<const data_type *>(stringStart);
            const data_type *mainSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            byte_order_query_type mainStringQueryResults = hasByteOrderMark(stringStart, stringLength);

            size_type mainStringLength = stringLength / sizeof(data_type);
            size_type mainSubstringLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResults = hasByteOrderMark(substringStart, substringLength);

            if(mainStringQueryResults.first)
            {
                mainStringStart++;
                mainStringLength--;
            }


            if(substringQueryResults.first)
            {
                mainSubstringStart++;
                mainSubstringLength--;
            }

            if(mainSubstringLength > mainStringLength)
                return result;

            substringStartCode = *mainSubstringStart;
            substringStartCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                              sizeof(parent_type::unicode_point_type), substringQueryResults.second);

            for(size_type i = 0; i < mainStringLength; i++)
            {
                mainStringCode = *(mainStringStart + i);
                mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                               sizeof(parent_type::unicode_point_type),
                                                               mainStringQueryResults.second);

                if(mainStringCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(mainSubstringLength > (mainStringLength - i))
                        return result;

                    for(; j < (i + mainSubstringLength); j++, k++)
                    {
                        mainStringCode = *(mainStringStart + j);
                        mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                                       sizeof(parent_type::unicode_point_type),
                                                                       mainStringQueryResults.second);

                        substringCode = *(mainSubstringStart + k);
                        substringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                                                      sizeof(parent_type::unicode_point_type),
                                                                      substringQueryResults.second);

                        if(mainStringCode != substringCode)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                        continue;
                    else
                    {
                        // Corresponds to the character result.postion = i;
                        result.position = arrayIndexOfCharacterAtCharacterIndex(stringStart, stringLength, i);
                        // Coresponds to the character result.length = j - i;
                        result.length = (j - i) * sizeof(data_type);
                        return result;
                    }
                }
            }

            return result;
        }


        UTF32StringEncoder::range_array_type
            UTF32StringEncoder::findByteRangesOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                  const char_type *substringStart,
                                                                  size_type substringLength)
        {
            range_array_type rangeArray;

            parent_type::unicode_point_type mainStringCode;
            parent_type::unicode_point_type substringCode;
            parent_type::unicode_point_type substringStartCode;

            const data_type *mainStringStart = reinterpret_cast<const data_type *>(stringStart);
            const data_type *mainSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            byte_order_query_type mainStringQueryResults = hasByteOrderMark(stringStart, stringLength);

            size_type mainStringLength = stringLength / sizeof(data_type);
            size_type mainSubstringLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResults = hasByteOrderMark(substringStart, substringLength);

            if(mainStringQueryResults.first)
            {
                mainStringStart++;
                mainStringLength--;
            }

            if(substringQueryResults.first)
            {
                mainSubstringStart++;
                mainSubstringLength--;
            }

            if(mainSubstringLength > mainStringLength)
                return rangeArray;

            substringStartCode = *mainSubstringStart;
            substringStartCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                              sizeof(parent_type::unicode_point_type), substringQueryResults.second);

            for(size_type i = 0; i < mainStringLength; i++)
            {
                mainStringCode = *(mainStringStart + i);
                mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                               sizeof(parent_type::unicode_point_type),
                                                               mainStringQueryResults.second);

                if(mainStringCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(mainSubstringLength > (mainStringLength - i))
                        return rangeArray;

                    for(; j < (i + mainSubstringLength); j++, k++)
                    {
                        mainStringCode = *(mainStringStart + j);
                        mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                                       sizeof(parent_type::unicode_point_type),
                                                                       mainStringQueryResults.second);

                        substringCode = *(mainSubstringStart + k);
                        substringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                                                      sizeof(parent_type::unicode_point_type),
                                                                      substringQueryResults.second);
                        if(mainStringCode != substringCode)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                        continue;
                    else
                    {
                        range_type genericRange(arrayIndexOfCharacterAtCharacterIndex(stringStart, stringLength, i),
                                                (j - i) * sizeof(data_type));
                        rangeArray.push_back(genericRange);
                        i = j - 1;
                        continue;
                    }
                }
            }

            return rangeArray;
        }


        UTF32StringEncoder::range_type UTF32StringEncoder::findCharacterRangeForSubstringInString(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_type result;

            parent_type::unicode_point_type mainStringCode;
            parent_type::unicode_point_type substringCode;
            parent_type::unicode_point_type substringStartCode;

            const data_type *mainStringStart = reinterpret_cast<const data_type *>(stringStart);
            const data_type *mainSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            byte_order_query_type mainStringQueryResults = hasByteOrderMark(stringStart, stringLength);

            size_type mainStringLength = stringLength / sizeof(data_type);
            size_type mainSubstringLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResults = hasByteOrderMark(substringStart, substringLength);

            if(mainStringQueryResults.first)
            {
                mainStringStart++;
                mainStringLength--;
            }


            if(substringQueryResults.first)
            {
                mainSubstringStart++;
                mainSubstringLength--;
            }

            if(mainSubstringLength > mainStringLength)
                return result;

            substringStartCode = *mainSubstringStart;
            substringStartCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                              sizeof(parent_type::unicode_point_type), substringQueryResults.second);

            for(size_type i = 0; i < mainStringLength; i++)
            {
                mainStringCode = *(mainStringStart + i);
                mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                               sizeof(parent_type::unicode_point_type),
                                                               mainStringQueryResults.second);

                if(mainStringCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(mainSubstringLength > (mainStringLength - i))
                        return result;

                    for(; j < (i + mainSubstringLength); j++, k++)
                    {
                        mainStringCode = *(mainStringStart + j);
                        mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                                       sizeof(parent_type::unicode_point_type),
                                                                       mainStringQueryResults.second);

                        substringCode = *(mainSubstringStart + k);
                        substringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                                                      sizeof(parent_type::unicode_point_type),
                                                                      substringQueryResults.second);

                        if(mainStringCode != substringCode)
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
                        result.length = j - i;
                        return result;
                    }
                }
            }

            return result;
        }


        UTF32StringEncoder::range_array_type UTF32StringEncoder::findCharacterRangesForSubstringInString(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            parent_type::unicode_point_type mainStringCode;
            parent_type::unicode_point_type substringCode;
            parent_type::unicode_point_type substringStartCode;

            const data_type *mainStringStart = reinterpret_cast<const data_type *>(stringStart);
            const data_type *mainSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            byte_order_query_type mainStringQueryResults = hasByteOrderMark(stringStart, stringLength);

            size_type mainStringLength = stringLength / sizeof(data_type);
            size_type mainSubstringLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResults = hasByteOrderMark(substringStart, substringLength);

            if(mainStringQueryResults.first)
            {
                mainStringStart++;
                mainStringLength--;
            }

            if(substringQueryResults.first)
            {
                mainSubstringStart++;
                mainSubstringLength--;
            }

            if(mainSubstringLength > mainStringLength)
                return rangeArray;

            substringStartCode = *mainSubstringStart;
            substringStartCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                              sizeof(parent_type::unicode_point_type), substringQueryResults.second);

            for(size_type i = 0; i < mainStringLength; i++)
            {
                mainStringCode = *(mainStringStart + i);
                mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                               sizeof(parent_type::unicode_point_type),
                                                               mainStringQueryResults.second);

                if(mainStringCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(mainSubstringLength > (mainStringLength - i))
                        return rangeArray;

                    for(; j < (i + mainSubstringLength); j++, k++)
                    {
                        mainStringCode = *(mainStringStart + j);
                        mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                                       sizeof(parent_type::unicode_point_type),
                                                                       mainStringQueryResults.second);

                        substringCode = *(mainSubstringStart + k);
                        substringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                                                      sizeof(parent_type::unicode_point_type),
                                                                      substringQueryResults.second);

                        if(mainStringCode != substringCode)
                        {
                            aborted = true;
                            break;
                        }
                    }

                    if(aborted)
                        continue;
                    else
                    {
                        range_type genericRange(i, j - i);
                        rangeArray.push_back(genericRange);
                        i = j - 1;
                        continue;
                    }
                }
            }

            return rangeArray;
        }


        UTF32StringEncoder::range_array_type UTF32StringEncoder::findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength)
        {
            range_array_type rangeArray;

            parent_type::unicode_point_type mainStringCode;
            parent_type::unicode_point_type substringCode;
            parent_type::unicode_point_type substringStartCode;

            const data_type *mainStringStart = reinterpret_cast<const data_type *>(stringStart);
            const data_type *mainSubstringStart = reinterpret_cast<const data_type *>(substringStart);
            byte_order_query_type mainStringQueryResults = hasByteOrderMark(stringStart, stringLength);

            size_type mainStringLength = stringLength / sizeof(data_type);
            size_type mainSubstringLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResults = hasByteOrderMark(substringStart, substringLength);

            if(mainStringQueryResults.first)
            {
                mainStringStart++;
                mainStringLength--;
            }

            if(substringQueryResults.first)
            {
                mainSubstringStart++;
                mainSubstringLength--;
            }

            if(mainSubstringLength > mainStringLength)
            {
                if(mainStringLength > 0)
                {
                    range_type genericRange(0, mainStringLength);
                    rangeArray.push_back(genericRange);
                }

                return rangeArray;
            }

            substringStartCode = *mainSubstringStart;
            substringStartCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                              sizeof(parent_type::unicode_point_type), substringQueryResults.second);

            size_type location = 0;
            size_type length = 0;

            for(size_type i = 0; i < mainStringLength; i++)
            {
                mainStringCode = *(mainStringStart + i);
                mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                               sizeof(parent_type::unicode_point_type),
                                                               mainStringQueryResults.second);

                if(mainStringCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(mainSubstringLength > (mainStringLength - i))
                    {
                        length += mainStringLength - i;
                        range_type genericRange(location, length);
                        rangeArray.push_back(genericRange);
                        return rangeArray;
                    }

                    for(; j < (i + mainSubstringLength); j++, k++)
                    {
                        mainStringCode = *(mainStringStart + j);
                        mainStringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainStringCode),
                                                                       sizeof(parent_type::unicode_point_type),
                                                                       mainStringQueryResults.second);

                        substringCode = *(mainSubstringStart + k);
                        substringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                                                      sizeof(parent_type::unicode_point_type),
                                                                      substringQueryResults.second);

                        if(mainStringCode != substringCode)
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
                            range_type genericRange(location, length);
                            rangeArray.push_back(genericRange);
                            length = 0;
                        }
                        location = i + mainSubstringLength;
                        i = j - 1;
                        continue;
                    }
                }
                else
                    length++;
            }

            if(length > 0)
            {
                range_type genericRange(location, length);
                rangeArray.push_back(genericRange);
            }

            return rangeArray;
        }


        ComparisonResult UTF32StringEncoder::compareStrings(const char_type *firstStringStart,
                                                            size_type firstStringLength,
                                                            const char_type *secondStringStart,
                                                            size_type secondStringLength)
        {
            const data_type *firstStart = reinterpret_cast<const data_type *>(firstStringStart);
            size_type firstLength = firstStringLength / sizeof(data_type);
            byte_order_query_type firstQueryResponse = hasByteOrderMark(firstStringStart, firstStringLength);

            const data_type *secondStart = reinterpret_cast<const data_type *>(secondStringStart);
            size_type secondLength = secondStringLength / sizeof(data_type);
            byte_order_query_type secondQueryResponse = hasByteOrderMark(secondStringStart, secondStringLength);

            if(firstQueryResponse.first)
            {
                firstStart++;
                firstLength--;
            }

            if(secondQueryResponse.first)
            {
                secondStart++;
                secondLength--;
            }

            size_type charactersToExamine = firstLength >= secondLength ? secondLength : firstLength;

            parent_type::unicode_point_type code1;
            parent_type::unicode_point_type code2;

            for(size_type i = 0; i < charactersToExamine; i++)
            {
                code1 = *(firstStart + i);
                code1 =
                    this->correctValueForPlatform(reinterpret_cast<char_type *>(&code1),
                                                  sizeof(parent_type::unicode_point_type), firstQueryResponse.second);

                code2 = *(secondStart + i);
                code2 =
                    this->correctValueForPlatform(reinterpret_cast<char_type *>(&code2),
                                                  sizeof(parent_type::unicode_point_type), secondQueryResponse.second);

                if(code1 < code2)
                    return OrderedAscending;
                if(code1 > code2)
                    return OrderedDescending;
            }

            if(firstLength == secondLength)
                return OrderedSame;
            else if(firstLength > secondLength)
                return OrderedAscending;

            return OrderedDescending;
        }


        void UTF32StringEncoder::convertStringCharacters(char_type *start, size_type length, StringCase convertToCase)
        {
            data_type *characterStart = reinterpret_cast<data_type *>(start);
            size_type characterLength = length / sizeof(data_type);
            byte_order_query_type queryResponse = this->hasByteOrderMark(start, length);

            if(queryResponse.first)
            {
                characterStart++;
                characterLength--;
            }

            parent_type::unicode_point_type theCode;

            for(size_type i = 0; i < characterLength; i++)
            {
                parent_type::unicode_point_type value = *(characterStart + i);
                theCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&value),
                                                        sizeof(parent_type::unicode_point_type), queryResponse.second);

                if(convertToCase == StringCase::UpperCase)
                {
                    if(theCode >= 97 && theCode <= 122)
                    {
                        theCode -= 32;
                    }
                }
                if(convertToCase == StringCase::LowerCase)
                {
                    if(theCode >= 65 && theCode <= 90)
                    {
                        theCode += 32;
                    }
                }

                value = this->correctValueForPlatform(reinterpret_cast<char_type *>(&theCode),
                                                      sizeof(parent_type::unicode_point_type), queryResponse.second);
                *(characterStart + i) = value;
            }
        }


        UTF32StringEncoder::size_type UTF32StringEncoder::computeArraySizeInBytesForStringByReplacingSubstrings(
            const char_type *stringStart, size_type stringLength, const char_type *substringStart,
            size_type substringLength, const char_type *replaceStringStart, size_type replaceStringLength,
            range_array_type &ranges)
        {
            size_type newsize = 0;

            const data_type *mainCharacterStart = reinterpret_cast<const data_type *>(stringStart);
            size_type mainCharacterLength = stringLength / sizeof(data_type);
            byte_order_query_type mainQueryResponse = hasByteOrderMark(stringStart, stringLength);

            const data_type *substringCharacterStart = reinterpret_cast<const data_type *>(substringStart);
            size_type substringCharacterLength = substringLength / sizeof(data_type);
            byte_order_query_type substringQueryResponse = hasByteOrderMark(substringStart, substringLength);

            const data_type *replaceStringCharacterStart = reinterpret_cast<const data_type *>(replaceStringStart);
            size_type replaceStringCharacterLength = replaceStringLength / sizeof(data_type);
            byte_order_query_type replaceStringQueryResponse =
                hasByteOrderMark(replaceStringStart, replaceStringLength);

            if(substringCharacterLength > mainCharacterLength)
            {
                if(mainCharacterLength > 0)
                    return stringLength;
                return 0;
            }

            if(mainQueryResponse.first)
            {
                mainCharacterStart += 1;
                mainCharacterLength -= 1;
            }

            if(substringQueryResponse.first)
            {
                substringCharacterStart += 1;
                substringCharacterLength -= 1;
            }

            if(replaceStringQueryResponse.first)
            {
                replaceStringCharacterStart += 1;
                replaceStringCharacterLength -= 1;
            }

            parent_type::unicode_point_type mainCode;
            parent_type::unicode_point_type substringCode;
            parent_type::unicode_point_type substringStartCode;

            substringStartCode = *substringCharacterStart;
            substringStartCode =
                this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringStartCode),
                                              sizeof(parent_type::unicode_point_type), substringQueryResponse.second);

            for(size_type i = 0; i < mainCharacterLength; i++)
            {
                mainCode = *(mainCharacterStart + i);
                mainCode =
                    this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainCode),
                                                  sizeof(parent_type::unicode_point_type), mainQueryResponse.second);

                if(mainCode == substringStartCode)
                {
                    size_type j = i + 1, k = 1;
                    bool aborted = false;

                    if(substringCharacterLength > (mainCharacterLength - i))
                        return ((newsize + mainCharacterLength - i) * sizeof(data_type)) +
                               this->lengthOfByteOrderMarkInBytes();

                    for(; j < (i + substringCharacterLength); j++, k++)
                    {
                        mainCode = *(mainCharacterStart + j);
                        mainCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&mainCode),
                                                                 sizeof(parent_type::unicode_point_type),
                                                                 mainQueryResponse.second);

                        substringCode = *(substringCharacterStart + k);
                        substringCode = this->correctValueForPlatform(reinterpret_cast<char_type *>(&substringCode),
                                                                      sizeof(parent_type::unicode_point_type),
                                                                      substringQueryResponse.second);

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
                        newsize += replaceStringCharacterLength;
                        i = j - 1;
                        continue;
                    }
                }

                newsize++;
            }

            return (newsize * sizeof(data_type)) + this->lengthOfByteOrderMarkInBytes();
        }


        void UTF32StringEncoder::replaceOccurancesOfStringWithString(
            const char_type *originalStringStart, size_type originalStringLength, char_type *newStringStart,
            size_type newStringLength, const char_type *replacementStringStart, size_type replacementStringLength,
            range_array_type &substringRanges)
        {
            const data_type *originalDataTypeString = reinterpret_cast<const data_type *>(originalStringStart);
            size_type originalDataTypeStringLength = originalStringLength / sizeof(data_type);
            byte_order_query_type originalStringQuery =
                this->hasByteOrderMark(originalStringStart, originalStringLength);

            data_type *newDataTypeString = reinterpret_cast<data_type *>(newStringStart);
            size_type newDataTypeStringLength = newStringLength / sizeof(data_type);

            const data_type *replacementDataTypeString = reinterpret_cast<const data_type *>(replacementStringStart);
            size_type replacementDataTypeStringLength = replacementStringLength / sizeof(data_type);
            byte_order_query_type replacementStringQuery =
                this->hasByteOrderMark(replacementStringStart, replacementStringLength);

            bool endOfRanges = false;

            if(substringRanges.size() == 0)
                endOfRanges = true;

            auto rangeIter = substringRanges.begin();

            if(originalStringQuery.first)
            {
                originalDataTypeString++;
                originalDataTypeStringLength--;
            }

            if(replacementStringQuery.first)
            {
                replacementDataTypeString++;
                replacementDataTypeStringLength--;
            }

            *newDataTypeString = 0xFEFF;
            newDataTypeString++;
            newDataTypeStringLength--;

            parent_type::unicode_point_type theCode;

            for(size_type i = 0; i < originalDataTypeStringLength; i++)
            {
                if(!endOfRanges && i == rangeIter->position)
                {

                    for(size_type j = 0; j < replacementDataTypeStringLength; j++)
                    {
                        theCode = *(replacementDataTypeString + j);

                        *newDataTypeString = this->correctValueForPlatform(reinterpret_cast<char_type *>(&theCode),
                                                                           sizeof(parent_type::unicode_point_type),
                                                                           replacementStringQuery.second);
                        newDataTypeString++;
                    }

                    i += rangeIter->length - 1;    // Subtract one to get the correct value on the iteration.
                    rangeIter++;
                    if(rangeIter == substringRanges.end())
                        endOfRanges = true;
                }
                else
                {
                    theCode = *(originalDataTypeString + i);
                    *newDataTypeString = this->correctValueForPlatform(reinterpret_cast<char_type *>(&theCode),
                                                                       sizeof(parent_type::unicode_point_type),
                                                                       originalStringQuery.second);
                    newDataTypeString++;
                }
            }
        }


        UTF32StringEncoder::parent_type::unicode_point_type
            UTF32StringEncoder::correctValueForPlatform(const char_type *start, size_type length, endian endian)
        {
            const data_type *theArrayCode = reinterpret_cast<const data_type *>(start);

            if(length < sizeof(data_type))
                throw std::runtime_error("correctValueForPlatform not given enough space to change endianness of code");

            if(endian != this->thisSystemEndianness())
            {
                unsigned char tmp;
                unsigned char *byte = reinterpret_cast<unsigned char *>(const_cast<data_type *>(theArrayCode));
                tmp = *(byte + 0);
                *(byte + 0) = *(byte + 3);
                *(byte + 3) = tmp;
                tmp = *(byte + 1);
                *(byte + 1) = *(byte + 2);
                *(byte + 2) = tmp;
            }

            return static_cast<parent_type::unicode_point_type>(*theArrayCode);
        }


        bool UTF32StringEncoder::operator==(const StringEncoder &e)
        {
            try
            {
                const UTF32StringEncoder &theEncoder = dynamic_cast<const UTF32StringEncoder &>(e);
                return true;
            }
            catch(std::bad_cast e)
            {
                ;
            }

            return false;
        }


        std::ostream &UTF32StringEncoder::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("UTF32StringEncoder");
                o << *formatter;
                delete formatter;
            }
            return o;
        }

    }    // namespace Foundation

}    // namespace TF
