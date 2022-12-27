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

#ifndef TFWINDOWS1252STRINGENCODER_HPP
#define TFWINDOWS1252STRINGENCODER_HPP

#define NEEDS_UNORDERED_MAP
#define NEEDS_OSTREAM
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstringencoder.hpp"

namespace TF::Foundation
{

    class Windows1252StringEncoder : public StringEncoder
    {
    public:
        using parent_type = StringEncoder;

        using size_type = parent_type::size_type;

        using range_type = parent_type::range_type;

        using range_array_type = parent_type::range_array_type;

        using byte_order_query_type = parent_type::byte_order_query_type;

        using data_type = unsigned char;

        Windows1252StringEncoder();

        virtual ~Windows1252StringEncoder() = default;

        virtual auto clone() -> StringEncoder * override;

        virtual auto basicCodeLengthInBytes() -> size_type override;

        virtual auto hasFixedCodeLength() -> bool override;

        virtual auto numberOfBytesRequiredForLargestCharacterValue() -> size_type override;

        virtual auto canUseByteOrderMark() -> bool override;

        virtual auto usesByteOrderMark() -> bool override;

        virtual auto lengthOfByteOrderMarkInBytes() -> size_type override;

        virtual void writeByteOrderMark(char_type * start, size_type length) override;

        virtual auto hasByteOrderMark(const char_type * start, size_type length) -> byte_order_query_type override;

        virtual auto numberOfCharacters(const char_type * start, size_type length) -> size_type override;

        virtual auto checkStringForCorrectness(const char_type * start, size_type length) -> bool override;

        virtual auto nextCodePoint(const char_type * start, size_type length, Endian endian)
            -> std::pair<unicode_point_type, size_type> override;

        virtual auto nextCode(const char_type * start, size_type length, Endian endian)
            -> std::pair<unicode_point_type, size_type> override;

        virtual auto unicodeCodePointForCharacterAtIndex(const char_type * start, size_type length, size_type index)
            -> unicode_point_type override;

        virtual auto bytesNeededForRepresentationOfCode(unicode_point_type code) -> size_type override;

        virtual auto encodeCodePoint(char_type * start, size_type length, unicode_point_type code, Endian endian)
            -> size_type override;

        virtual auto arrayIndexOfCharacterAtCharacterIndex(const char_type * start, size_type length, size_type index)
            -> size_type override;

        virtual auto numberOfBytesToCaptureCharactersInRange(const char_type * start, size_type length,
                                                             const range_type & range) -> size_type override;

        virtual auto containsCharacterWithZeroValue(const char_type * start, size_type length) -> bool override;

        virtual auto containsCharacterNotInASCIIRange(const char_type * start, size_type length) -> bool override;

        virtual auto findByteRangeOfSubstringInString(const char_type * stringStart, size_type stringLength,
                                                      const char_type * substringStart, size_type substringLength)
            -> range_type override;

        virtual auto findByteRangesOfSubstringInString(const char_type * stringStart, size_type stringLength,
                                                       const char_type * substringStart, size_type substringLength)
            -> range_array_type override;

        virtual auto findCharacterRangeForSubstringInString(const char_type * stringStart, size_type stringLength,
                                                            const char_type * substringStart, size_type substringLength)
            -> range_type override;

        virtual auto findCharacterRangesForSubstringInString(const char_type * stringStart, size_type stringLength,
                                                             const char_type * substringStart,
                                                             size_type substringLength) -> range_array_type override;

        virtual auto findCharacterRangesOfSubstringsThatDoNotMatchSubstring(const char_type * stringStart,
                                                                            size_type stringLength,
                                                                            const char_type * substringStart,
                                                                            size_type substringLength)
            -> range_array_type override;

        virtual auto compareStrings(const char_type * firstStringStart, size_type firstStringLength,
                                    const char_type * secondStringStart, size_type secondStringLength)
            -> ComparisonResult override;

        virtual void convertStringCharacters(char_type * start, size_type length, StringCase convertToCase) override;

        virtual auto computeArraySizeInBytesForStringByReplacingSubstrings(
            const char_type * stringStart, size_type stringLength, const char_type * substringStart,
            size_type substringLength, const char_type * replaceStringStart, size_type replaceStringLength,
            range_array_type & ranges) -> size_type override;

        virtual void replaceOccurrencesOfStringWithString(const char_type * originalStringStart,
                                                          size_type originalStringLength, char_type * newStringStart,
                                                          size_type newStringLength,
                                                          const char_type * replacementStringStart,
                                                          size_type replacementStringLength,
                                                          range_array_type & substringRanges) override;

        virtual auto correctValueForPlatform(const char_type * start, size_type length, Endian endian)
            -> unicode_point_type override;

        virtual auto operator==(const StringEncoder & e) -> bool override;

        virtual auto description(std::ostream & o) const -> std::ostream & override;

        virtual auto getEncoderID() const -> std::string override;

        auto bytesToExpectForCharacterInByteSequence(const char_type * s, size_type length, Endian endian)
            -> size_type override;

    private:
        static auto convertCodePointToUnicodeCodePoint(data_type code_point) -> unicode_point_type;
        static std::unordered_map<data_type, unicode_point_type> s_1252_to_unicode_map;
        static std::unordered_map<unicode_point_type, data_type> s_unicode_to_1252_map;
        static std::mutex s_init_mutex;
        static bool s_initialized;

        static void initialize_code_maps();
    };

} // namespace TF::Foundation

#endif // TFWINDOWS1252STRINGENCODER_HPP