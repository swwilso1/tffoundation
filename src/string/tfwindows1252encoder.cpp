/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#define NEEDS_CASSERT
#define NEEDS_CSTRING
#define NEEDS_CMATH
#define NEEDS_VECTOR
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfwindows1252encoder.hpp"
#include "tfformatter.hpp"

namespace TF::Foundation
{
    std::unordered_map<Windows1252StringEncoder::data_type, Windows1252StringEncoder::unicode_point_type>
        Windows1252StringEncoder::s_1252_to_unicode_map{};

    std::unordered_map<Windows1252StringEncoder::unicode_point_type, Windows1252StringEncoder::data_type>
        Windows1252StringEncoder::s_unicode_to_1252_map{};

    std::mutex Windows1252StringEncoder::s_init_mutex{};

    bool Windows1252StringEncoder::s_initialized = false;

    void Windows1252StringEncoder::initialize_code_maps()
    {
        for (data_type i = 0; i < 255; i++)
        {
            s_1252_to_unicode_map[i] = i;
        }

        s_1252_to_unicode_map[255] = 255;

        s_1252_to_unicode_map[0x80] = 0x20ac;
        s_1252_to_unicode_map[0x82] = 0x201a;
        s_1252_to_unicode_map[0x83] = 0x0192;
        s_1252_to_unicode_map[0x84] = 0x201e;
        s_1252_to_unicode_map[0x85] = 0x2026;
        s_1252_to_unicode_map[0x86] = 0x2020;
        s_1252_to_unicode_map[0x87] = 0x2021;
        s_1252_to_unicode_map[0x88] = 0x02c6;
        s_1252_to_unicode_map[0x89] = 0x2030;
        s_1252_to_unicode_map[0x8a] = 0x0160;
        s_1252_to_unicode_map[0x8b] = 0x2039;
        s_1252_to_unicode_map[0x8c] = 0x0152;
        s_1252_to_unicode_map[0x8e] = 0x017d;
        s_1252_to_unicode_map[0x91] = 0x2018;
        s_1252_to_unicode_map[0x92] = 0x2019;
        s_1252_to_unicode_map[0x93] = 0x201c;
        s_1252_to_unicode_map[0x94] = 0x201d;
        s_1252_to_unicode_map[0x95] = 0x2022;
        s_1252_to_unicode_map[0x96] = 0x2013;
        s_1252_to_unicode_map[0x97] = 0x2014;
        s_1252_to_unicode_map[0x98] = 0x02dc;
        s_1252_to_unicode_map[0x99] = 0x2122;
        s_1252_to_unicode_map[0x9a] = 0x0161;
        s_1252_to_unicode_map[0x9b] = 0x203a;
        s_1252_to_unicode_map[0x9c] = 0x0153;
        s_1252_to_unicode_map[0x9d] = 0x009d;
        s_1252_to_unicode_map[0x9e] = 0x017e;
        s_1252_to_unicode_map[0x9f] = 0x0178;

        for (data_type i = 0; i < 255; i++)
        {
            auto val = s_1252_to_unicode_map[i];
            s_unicode_to_1252_map[val] = i;
        }

        s_unicode_to_1252_map[255] = 255;
    }

    Windows1252StringEncoder::Windows1252StringEncoder()
    {
        std::lock_guard<std::mutex> lock(s_init_mutex);
        if (! s_initialized)
        {
            initialize_code_maps();
            s_initialized = true;
        }
    }

    auto Windows1252StringEncoder::clone() -> StringEncoder *
    {
        return new Windows1252StringEncoder;
    }

    auto Windows1252StringEncoder::basicCodeLengthInBytes() -> size_type
    {
        return 1;
    }

    auto Windows1252StringEncoder::hasFixedCodeLength() -> bool
    {
        return true;
    }

    auto Windows1252StringEncoder::numberOfBytesRequiredForLargestCharacterValue() -> size_type
    {
        return 1;
    }

    auto Windows1252StringEncoder::canUseByteOrderMark() -> bool
    {
        return false;
    }

    auto Windows1252StringEncoder::usesByteOrderMark() -> bool
    {
        return false;
    }

    auto Windows1252StringEncoder::lengthOfByteOrderMarkInBytes() -> size_type
    {
        return 0;
    }

    void Windows1252StringEncoder::writeByteOrderMark(char_type * start, size_type length)
    {
        (void)start;
        (void)length;
    }

    auto Windows1252StringEncoder::hasByteOrderMark(const char_type * start, size_type length) -> byte_order_query_type
    {
        (void)start;
        (void)length;
        return std::make_pair(false, Endian::LittleEndian);
    }

    auto Windows1252StringEncoder::numberOfCharacters(const char_type * start, size_type length) -> size_type
    {
        (void)start;
        return length;
    }

    auto Windows1252StringEncoder::checkStringForCorrectness(const char_type * start, size_type length) -> bool
    {
        (void)start;
        (void)length;
        return true;
    }

    auto Windows1252StringEncoder::nextCodePoint(const char_type * start, size_type length, Endian endian)
        -> std::pair<unicode_point_type, size_type>
    {
        (void)length;
        (void)endian;
        return std::make_pair(convertCodePointToUnicodeCodePoint(*start), 1);
    }

    auto Windows1252StringEncoder::nextCode(const char_type * start, size_type length, Endian endian)
        -> std::pair<unicode_point_type, size_type>
    {
        // This encoding is not a multibyte encoding, so we can piggy-back on nextCodePoint().
        return nextCodePoint(start, length, endian);
    }

    auto Windows1252StringEncoder::unicodeCodePointForCharacterAtIndex(const char_type * start, size_type length,
                                                                       size_type index) -> unicode_point_type
    {
        (void)length;
        return convertCodePointToUnicodeCodePoint(*(start + index));
    }

    auto Windows1252StringEncoder::bytesNeededForRepresentationOfCode(unicode_point_type code) -> size_type
    {
        (void)code;
        return 1;
    }

    auto Windows1252StringEncoder::encodeCodePoint(char_type * start, size_type length, unicode_point_type code,
                                                   Endian endian) -> size_type
    {
        (void)endian;
        (void)length;
        if (s_unicode_to_1252_map.contains(code))
        {
            *(reinterpret_cast<data_type *>(start)) = s_unicode_to_1252_map[code];
            return 1;
        }

        return 0;
    }

    auto Windows1252StringEncoder::arrayIndexOfCharacterAtCharacterIndex(const char_type * start, size_type length,
                                                                         size_type index) -> size_type
    {
        (void)start;
        (void)length;
        return index;
    }

    auto Windows1252StringEncoder::numberOfBytesToCaptureCharactersInRange(const char_type * start, size_type length,
                                                                           const range_type & range) -> size_type
    {
        (void)start;
        (void)length;
        return range.length;
    }

    auto Windows1252StringEncoder::containsCharacterWithZeroValue(const char_type * start, size_type length) -> bool
    {
        for (size_type i = 0; i < length; i++)
        {
            if (*(reinterpret_cast<const data_type *>(start) + i) == '\0')
            {
                return true;
            }
        }
        return false;
    }

    auto Windows1252StringEncoder::containsCharacterNotInASCIIRange(const char_type * start, size_type length) -> bool
    {
        for (size_type i = 0; i < length; i++)
        {
            auto val = s_1252_to_unicode_map[*(reinterpret_cast<const data_type *>(start) + i)];
            if (val > 255)
            {
                return true;
            }
        }
        return false;
    }

    auto Windows1252StringEncoder::findByteRangeOfSubstringInString(const char_type * stringStart,
                                                                    size_type stringLength,
                                                                    const char_type * substringStart,
                                                                    size_type substringLength) -> range_type
    {
        range_type result_range{};

        if (substringLength > stringLength)
        {
            return result_range;
        }

        for (size_type i = 0; i < stringLength; i++)
        {
            if (*(stringStart + i) == *substringStart)
            {
                size_type j = i + 1, k = 1;
                bool aborted = false;

                if (substringLength > (stringLength - i))
                {
                    return result_range;
                }

                for (; j < (i + substringLength); j++, k++)
                {
                    if (*(stringStart + j) != *(substringStart + k))
                    {
                        aborted = true;
                        break;
                    }
                }

                if (aborted)
                {
                    continue;
                }
                else
                {
                    result_range.position = i;
                    result_range.length = j - i;
                    return result_range;
                }
            }
        }

        return result_range;
    }

    auto Windows1252StringEncoder::findByteRangesOfSubstringInString(const char_type * stringStart,
                                                                     size_type stringLength,
                                                                     const char_type * substringStart,
                                                                     size_type substringLength) -> range_array_type
    {
        range_array_type result_range{};

        if (substringLength > stringLength)
        {
            return result_range;
        }

        for (size_type i = 0; i < stringLength; i++)
        {
            if (*(stringStart + i) == *substringStart)
            {
                size_type j = i + 1, k = 1;
                bool aborted = false;

                if (substringLength > (stringLength - i))
                {
                    return result_range;
                }

                for (; j < (i + substringLength); j++, k++)
                {
                    if (*(stringStart + j) != *(substringStart + k))
                    {
                        aborted = true;
                        break;
                    }
                }

                if (aborted)
                {
                    continue;
                }
                else
                {
                    range_type r(i, j - i);
                    result_range.push_back(r);
                    i = j - 1;
                    continue;
                }
            }
        }

        return result_range;
    }

    auto Windows1252StringEncoder::findCharacterRangeForSubstringInString(const char_type * stringStart,
                                                                          size_type stringLength,
                                                                          const char_type * substringStart,
                                                                          size_type substringLength) -> range_type
    {
        // Since we are dealing with a single byte encoding, this function behaves exactly as
        // findByteRangeOfSubstringInString.
        return findByteRangeOfSubstringInString(stringStart, stringLength, substringStart, substringLength);
    }

    auto Windows1252StringEncoder::findCharacterRangesForSubstringInString(const char_type * stringStart,
                                                                           size_type stringLength,
                                                                           const char_type * substringStart,
                                                                           size_type substringLength)
        -> range_array_type
    {
        // Since we are dealing with a single byte encoding, this function behaves exactly as
        // findByteRangesOfSubstringInString.
        return findByteRangesOfSubstringInString(stringStart, stringLength, substringStart, substringLength);
    }

    auto Windows1252StringEncoder::findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
        const char_type * stringStart, size_type stringLength, const char_type * substringStart,
        size_type substringLength) -> range_array_type
    {
        range_array_type rangeArray{};

        if (substringLength > stringLength)
        {
            if (stringLength > 0)
            {
                range_type r(0, stringLength);
                rangeArray.push_back(r);
            }
            return rangeArray;
        }

        size_type location = 0;
        size_type length = 0;

        for (size_type i = 0; i < stringLength; i++)
        {
            if (*(reinterpret_cast<const data_type *>(stringStart) + i) ==
                *(reinterpret_cast<const data_type *>(substringStart)))
            {
                size_type j = i + 1, k = 1;
                bool aborted = false;

                if (substringLength > (stringLength - i))
                {
                    length += stringLength - 1;
                    range_type r(location, length);
                    rangeArray.push_back(r);
                    return rangeArray;
                }

                for (; j < (i + substringLength); j++, k++)
                {
                    if (*(reinterpret_cast<const data_type *>(stringStart) + j) !=
                        *(reinterpret_cast<const data_type *>(substringStart) + k))
                    {
                        aborted = true;
                        break;
                    }
                }

                if (aborted)
                {
                    length++;
                    continue;
                }
                else
                {
                    // We explicitly allow length to be zero, this case will create a range object for a case like
                    // the following:
                    // 'alasdfkalsdf\n\nasldkflaksdf' when the character to match on is a newline character.
                    // We assert here that we have a zero length string between the \n and \n characters.
                    range_type theRange(location, length);
                    rangeArray.push_back(theRange);
                    length = 0;
                    location = i + substringLength;
                    i = j - 1;
                    continue;
                }
            }
            else
            {
                length++;
            }
        }

        if (length > 0)
        {
            range_type theRange(location, length);
            rangeArray.push_back(theRange);
        }

        return rangeArray;
    }

    auto Windows1252StringEncoder::compareStrings(const char_type * firstStringStart, size_type firstStringLength,
                                                  const char_type * secondStringStart, size_type secondStringLength)
        -> ComparisonResult
    {
        auto initialLengthToCompare = firstStringLength > secondStringLength ? secondStringLength : firstStringLength;

        for (size_type i = 0; i < initialLengthToCompare; i++)
        {
            if (*(reinterpret_cast<const data_type *>(firstStringStart) + i) <
                *(reinterpret_cast<const data_type *>(secondStringStart) + i))
            {
                return OrderedAscending;
            }
            if (*(reinterpret_cast<const data_type *>(firstStringStart) + i) >
                *(reinterpret_cast<const data_type *>(secondStringStart) + i))
            {
                return OrderedDescending;
            }
        }

        if (firstStringLength == secondStringLength)
        {
            return OrderedSame;
        }
        else if (firstStringLength > secondStringLength)
        {
            return OrderedAscending;
        }

        return OrderedDescending;
    }

    void Windows1252StringEncoder::convertStringCharacters(char_type * start, size_type length,
                                                           StringCase convertToCase)
    {
        // ASCII values A-Z are exactly 32 values from the counterpart a-z characters.
        for (size_type i = 0; i < length; i++)
        {
            auto value = *(reinterpret_cast<data_type *>(start) + i);
            if (convertToCase == StringCase::UpperCase)
            {
                // ASCII codes 97 - 122 are a - z.
                if (value >= 97 && value <= 122)
                {
                    *(start + i) = static_cast<char_type>(value - 32);
                }
            }
            if (convertToCase == StringCase::LowerCase)
            {
                // ASCII codes 65 - 90 are A - Z
                if (value >= 65 && value <= 90)
                {
                    *(start + i) = static_cast<char_type>(value + 32);
                }
            }
        }
    }

    auto Windows1252StringEncoder::computeArraySizeInBytesForStringByReplacingSubstrings(
        const char_type * stringStart, size_type stringLength, const char_type * substringStart,
        size_type substringLength, const char_type * replaceStringStart, size_type replaceStringLength,
        range_array_type & ranges) -> size_type
    {
        (void)replaceStringStart;

        size_type newsize{0};

        if (substringLength > stringLength)
        {
            if (stringLength > 0)
            {
                return stringLength;
            }
            return 0;
        }

        for (size_type i = 0; i < stringLength; i++)
        {
            if (*(reinterpret_cast<const data_type *>(stringStart) + i) ==
                *(reinterpret_cast<const data_type *>(substringStart) + i))
            {
                size_type j = i + 1, k = 1;
                bool aborted = false;

                if (substringLength > (stringLength - i))
                {
                    return newsize + stringLength - i;
                }

                for (; j < (i + substringLength); j++, k++)
                {
                    if (*(reinterpret_cast<const data_type *>(stringStart) + j) !=
                        *(reinterpret_cast<const data_type *>(substringStart) + k))
                    {
                        aborted = true;
                        break;
                    }
                }

                if (aborted)
                {
                    newsize++;
                    continue;
                }
                else
                {
                    range_type r(i, j - 1);
                    ranges.push_back(r);
                    newsize += replaceStringLength;
                    i = j - 1;
                    continue;
                }
            }

            newsize++;
        }

        return newsize;
    }

    void Windows1252StringEncoder::replaceOccurrencesOfStringWithString(
        const char_type * originalStringStart, size_type originalStringLength, char_type * newStringStart,
        size_type newStringLength, const char_type * replacementStringStart, size_type replacementStringLength,
        range_array_type & substringRanges)
    {
        (void)newStringLength;

        bool endOfRanges = false;

        if (substringRanges.empty())
            endOfRanges = true;

        char_type * newStringTmp = newStringStart;

        auto rangeIter = substringRanges.begin();

        for (size_type i = 0; i < originalStringLength; i++)
        {
            if (! endOfRanges && i == rangeIter->position)
            {
                std::memcpy(reinterpret_cast<void *>(newStringTmp),
                            reinterpret_cast<void *>(const_cast<char_type *>(replacementStringStart)),
                            replacementStringLength * sizeof(char_type));
                newStringTmp += replacementStringLength * sizeof(char_type);
                i += rangeIter->length - 1;
                rangeIter++;
                if (rangeIter == substringRanges.end())
                    endOfRanges = true;
            }
            else
            {
                *newStringTmp = *(originalStringStart + i);
                newStringTmp++;
            }
        }
    }

    auto Windows1252StringEncoder::correctValueForPlatform(const char_type * start, size_type length, Endian endian)
        -> unicode_point_type
    {
        (void)length;
        (void)endian;
        return s_1252_to_unicode_map[*(reinterpret_cast<const data_type *>(start))];
    }

    auto Windows1252StringEncoder::operator==(const StringEncoder & e) -> bool
    {
        return this->getEncoderID() == e.getEncoderID();
    }

    auto Windows1252StringEncoder::description(std::ostream & o) const -> std::ostream &
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("Windows1252StringEncoder");
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    auto Windows1252StringEncoder::getEncoderID() const -> std::string
    {
        return {"537FE459-6BA5-4F25-9748-E67857B9D6FD"};
    }

    auto Windows1252StringEncoder::bytesToExpectForCharacterInByteSequence(const char_type * s, size_type length,
                                                                           Endian endian) -> size_type
    {
        (void)s;
        (void)length;
        (void)endian;
        return 1;
    }

    auto Windows1252StringEncoder::convertCodePointToUnicodeCodePoint(data_type code_point) -> unicode_point_type
    {
        return s_1252_to_unicode_map[code_point];
    }

} // namespace TF::Foundation
