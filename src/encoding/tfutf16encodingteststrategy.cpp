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

#include "tfutf16encodingteststrategy.hpp"
#include "tfutf16stringencoder.hpp"

namespace TF::Foundation
{

    auto UTF16EncodingTestStrategy::has_encoding(const unsigned char * str, size_type length)
        -> std::optional<encoding_type>
    {
        if (str == nullptr || length == 0)
        {
            return {};
        }

        auto codes = reinterpret_cast<const uint16_t *>(str);
        auto code_length = length / sizeof(uint16_t);
        bool found_byte_order{false};

        if (code_length < 2)
        {
            return {};
        }

        if (*codes == 0xFEFF)
        {
            found_byte_order = true;
        }
        else if (*codes == 0xFFFE)
        {
            found_byte_order = true;
        }

        if (found_byte_order)
        {
            return {Encoding::UTF16};
        }

        // We do not have a UTF-16 byte order mark, so check for any surrogate pairs.
        for (size_type i = 0; i < code_length; i++)
        {
            auto code1 = *(codes + i);
            if (code1 >= 0xD800 && code1 <= 0xDBFF)
            {
                // Now see if we have the second part of a surrogate pair.
                if (i + 1 >= code_length)
                {
                    return {};
                }

                auto code2 = *(codes + i + 1);
                if (code2 >= 0xDC00 && code2 <= 0xDFFF)
                {
                    return {Encoding::UTF16};
                }
            }
        }

        auto byte1 = *str;
        auto byte2 = *(str + 1);
        bool evens{false};

        if (byte1 > 0 && byte2 == 0)
        {
            evens = true;
        }

        size_type zeros{0};

        // We have no surrogate pairs.  Now check every byte and see if every other byte
        // has a 0 value.  If so we can assert UTF16.

        bool odd{true};
        for (size_type i = 0; i < length; i += 2)
        {
            auto byte = *(str + i);

            if (odd && ! evens)
            {
                if (byte == 0)
                {
                    zeros++;
                }
            }
            else if (! odd && evens)
            {
                if (byte == 0)
                {
                    zeros++;
                }
            }

            odd = ! odd;
        }

        double percent = static_cast<double>(zeros) / static_cast<double>(length);

        if (percent >= 0.75)
        {
            return {Encoding::UTF16};
        }

        return {};
    }

} // namespace TF::Foundation
