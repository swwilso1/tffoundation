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

#ifndef TFUTF16STRINGENCODER_HPP
#define TFUTF16STRINGENCODER_HPP

#define NEEDS_OSTREAM
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstringencoder.hpp"

namespace TF
{

    namespace Foundation
    {

        class UTF16StringEncoder : public StringEncoder
        {
        public:
            using parent_type = StringEncoder;

            using size_type = parent_type::size_type;

            using range_type = parent_type::range_type;

            using range_array_type = parent_type::range_array_type;

            using byte_order_query_type = parent_type::byte_order_query_type;

            using data_type = unsigned short;

            ~UTF16StringEncoder() {}

            StringEncoder * clone(void) override;

            size_type basicCodeLengthInBytes(void) override;

            bool hasFixedCodeLength(void) override;

            size_type numberOfBytesRequiredForLargestCharacterValue(void) override;

            bool canUseByteOrderMark(void) override;

            bool usesByteOrderMark(void) override;

            size_type lengthOfByteOrderMarkInBytes(void) override;

            void writeByteOrderMark(char_type * start, size_type length) override;

            byte_order_query_type hasByteOrderMark(const char_type * start, size_type length) override;

            size_type numberOfCharacters(const char_type * start, size_type length) override;

            bool checkStringForCorrectness(const char_type * start, size_type length) override;

            std::pair<unicode_point_type, size_type> nextCodePoint(const char_type * start, size_type length,
                                                                   Endian endian) override;

            std::pair<unicode_point_type, size_type> nextCode(const char_type * start, size_type length,
                                                              Endian endian) override;

            unicode_point_type unicodeCodePointForCharacterAtIndex(const char_type * start, size_type length,
                                                                   size_type index) override;

            size_type bytesNeededForRepresentationOfCode(unicode_point_type code) override;

            size_type encodeCodePoint(char_type * start, size_type length, unicode_point_type code,
                                      Endian endian) override;

            size_type arrayIndexOfCharacterAtCharacterIndex(const char_type * start, size_type length,
                                                            size_type index) override;

            size_type numberOfBytesToCaptureCharactersInRange(const char_type * start, size_type length,
                                                              const range_type & range) override;

            bool containsCharacterWithZeroValue(const char_type * start, size_type length) override;

            bool containsCharacterNotInASCIIRange(const char_type * start, size_type length) override;

            range_type findByteRangeOfSubstringInString(const char_type * stringStart, size_type stringLength,
                                                        const char_type * substringStart,
                                                        size_type substringLength) override;

            range_array_type findByteRangesOfSubstringInString(const char_type * stringStart, size_type stringLength,
                                                               const char_type * substringStart,
                                                               size_type substringLength) override;

            range_type findCharacterRangeForSubstringInString(const char_type * stringStart, size_type stringLength,
                                                              const char_type * substringStart,
                                                              size_type substringLength) override;

            range_array_type findCharacterRangesForSubstringInString(const char_type * stringStart,
                                                                     size_type stringLength,
                                                                     const char_type * substringStart,
                                                                     size_type substringLength) override;

            range_array_type findCharacterRangesOfSubstringsThatDoNotMatchSubstring(const char_type * stringStart,
                                                                                    size_type stringLength,
                                                                                    const char_type * substringStart,
                                                                                    size_type substringLength) override;

            void convertStringCharacters(char_type * start, size_type length, StringCase convertToCase) override;

            size_type computeArraySizeInBytesForStringByReplacingSubstrings(
                const char_type * stringStart, size_type stringLength, const char_type * substringStart,
                size_type substringLength, const char_type * replaceStringStart, size_type replaceStringLength,
                range_array_type & ranges) override;

            void replaceOccurancesOfStringWithString(const char_type * originalStringStart,
                                                     size_type originalStringLength, char_type * newStringStart,
                                                     size_type newStringLength,
                                                     const char_type * replacementStringStart,
                                                     size_type replacementStringLength,
                                                     range_array_type & substringRanges) override;

            unicode_point_type correctValueForPlatform(const char_type * start, size_type length,
                                                       Endian endian) override;

            bool operator==(const StringEncoder & e) override;

            std::ostream & description(std::ostream & o) const override;

            std::string getEncoderID() const override;

            unicode_point_type convertSurrogatePairToCodePoint(data_type highSurrogate, data_type lowSurrogate);

        private:
            static const size_type byteOrderMarkLength;

            data_type correctUTF16CodeForPlatform(data_type theCode, Endian endian);
        };

    } // namespace Foundation

} // namespace TF

#endif /* TFUTF16STRINGENCODER_HPP */