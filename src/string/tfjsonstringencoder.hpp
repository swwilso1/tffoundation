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

#ifndef TFJSONSTRINGENCODER_HPP
#define TFJSONSTRINGENCODER_HPP

#define NEEDS_OSTREAM
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstringencoder.hpp"
#include "tfutf8stringencoder.hpp"

namespace TF
{

    namespace Foundation
    {

        class JSONStringEncoder : public UTF8StringEncoder
        {
        public:
            using parent_type = UTF8StringEncoder;

            using size_type = parent_type::size_type;

            using range_type = parent_type::range_type;

            using range_array_type = parent_type::range_array_type;

            using byte_order_query_type = parent_type::byte_order_query_type;

            using data_type = unsigned char;

            ~JSONStringEncoder() {}

            StringEncoder * clone() override;

            size_type numberOfBytesRequiredForLargestCharacterValue() override;

            std::ostream & description(std::ostream & o) const override;

            std::string getEncoderID() const override;

        protected:
            size_type bytesToExpectInUTF8Sequence(const data_type * s, size_type length) override;

            unicode_point_type convertUTF8SequenceToUnicodePoint(const data_type * start, size_type length) override;

        private:
            struct EscapedCodeStatus
            {
                bool m_escaped_unicode;
                unicode_point_type m_code;

                EscapedCodeStatus() : m_escaped_unicode{false}, m_code{0} {};
            };

            std::pair<bool, unicode_point_type> calculateTheUPoint(const char_type * s, size_type length);

            std::pair<bool, EscapedCodeStatus> calculateTheEscapedCode(const char_type * s, size_type length);

            static const size_type byteOrderMarkLength;
        };

    } // namespace Foundation

} // namespace TF

#endif /* TFJSONSTRINGENCODER_HPP */