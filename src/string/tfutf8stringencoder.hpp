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

#ifndef TFUTF8STRINGENCODER_HPP
#define TFUTF8STRINGENCODER_HPP

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

        class UTF8StringEncoder : public StringEncoder
        {
        public:
            using parent_type = StringEncoder;

            using size_type = parent_type::size_type;

            using range_type = parent_type::range_type;

            using range_array_type = parent_type::range_array_type;

            using byte_order_query_type = parent_type::byte_order_query_type;

            using data_type = unsigned char;

            virtual ~UTF8StringEncoder();

            virtual StringEncoder *clone() override;

            virtual size_type basicCodeLengthInBytes() override;

            virtual bool hasFixedCodeLength() override;

            virtual size_type numberOfBytesRequiredForLargestCharacterValue() override;

            virtual bool canUseByteOrderMark() override;

            virtual bool usesByteOrderMark() override;

            virtual size_type lengthOfByteOrderMarkInBytes() override;

            virtual void writeByteOrderMark(char_type *start, size_type length) override;

            virtual byte_order_query_type hasByteOrderMark(const char_type *start, size_type length) override;

            virtual size_type numberOfCharacters(const char_type *start, size_type length) override;

            virtual bool checkStringForCorrectness(const char_type *start, size_type length) override;

            virtual std::pair<unicode_point_type, size_type> nextCodePoint(const char_type *start, size_type length,
                                                                           Endian endian) override;

            virtual std::pair<unicode_point_type, size_type> nextCode(const char_type *start, size_type length,
                                                                      Endian endian) override;

            virtual unicode_point_type unicodeCodePointForCharacterAtIndex(const char_type *start, size_type length,
                                                                           size_type index) override;

            virtual size_type bytesNeededForRepresentationOfCode(unicode_point_type code) override;

            virtual size_type encodeCodePoint(char_type *start, size_type length, unicode_point_type code,
                                              Endian endian) override;

            virtual size_type arrayIndexOfCharacterAtCharacterIndex(const char_type *start, size_type length,
                                                                    size_type index) override;

            virtual size_type numberOfBytesToCaptureCharactersInRange(const char_type *start, size_type length,
                                                                      const range_type &range) override;

            virtual bool containsCharacterWithZeroValue(const char_type *start, size_type length) override;

            virtual bool containsCharacterNotInASCIIRange(const char_type *start, size_type length) override;

            virtual range_type findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
                                                                const char_type *substringStart,
                                                                size_type substringLength) override;

            virtual range_array_type findByteRangesOfSubstringInString(const char_type *stringStart,
                                                                       size_type stringLength,
                                                                       const char_type *substringStart,
                                                                       size_type substringLength) override;

            virtual range_type findCharacterRangeForSubstringInString(const char_type *stringStart,
                                                                      size_type stringLength,
                                                                      const char_type *substringStart,
                                                                      size_type substringLength) override;

            virtual range_array_type findCharacterRangesForSubstringInString(const char_type *stringStart,
                                                                             size_type stringLength,
                                                                             const char_type *substringStart,
                                                                             size_type substringLength) override;

            virtual range_array_type findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
                const char_type *stringStart, size_type stringLength, const char_type *substringStart,
                size_type substringLength) override;

            virtual ComparisonResult compareStrings(const char_type *firstStringStart, size_type firstStringLength,
                                                    const char_type *secondStringStart,
                                                    size_type secondStringLength) override;

            virtual void convertStringCharacters(char_type *start, size_type length, StringCase convertToCase) override;

            virtual size_type computeArraySizeInBytesForStringByReplacingSubstrings(
                const char_type *stringStart, size_type stringLength, const char_type *substringStart,
                size_type substringLength, const char_type *replaceStringStart, size_type replaceStringLength,
                range_array_type &ranges) override;

            virtual void replaceOccurancesOfStringWithString(const char_type *originalStringStart,
                                                             size_type originalStringLength, char_type *newStringStart,
                                                             size_type newStringLength,
                                                             const char_type *replacementStringStart,
                                                             size_type replacementStringLength,
                                                             range_array_type &substringRanges) override;

            virtual unicode_point_type correctValueForPlatform(const char_type *start, size_type length,
                                                               Endian endian) override;

            virtual bool operator==(const StringEncoder &e) override;

            virtual std::ostream &description(std::ostream &o) const override;

            virtual std::string getEncoderID() const override;

        protected:
            virtual size_type bytesToExpectInUTF8Sequence(const data_type *s, size_type length);

            virtual unicode_point_type convertUTF8SequenceToUnicodePoint(const data_type *start, size_type length);

            virtual size_type bytesNeededForUTF8RepresenationOfUnicodePoint(unicode_point_type point);

        private:
            static const size_type byteOrderMarkLength;
        };

    }    // namespace Foundation

}    // namespace TF

#endif /* TFUTF8STRINGENCODER_HPP */