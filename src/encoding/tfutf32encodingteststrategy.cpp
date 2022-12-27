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

#include "tfutf32encodingteststrategy.hpp"
#include "tfutf32stringencoder.hpp"
#include "tfendian.hpp"

namespace TF::Foundation
{

    auto UTF32EncodingTestStrategy::has_encoding(const unsigned char * str, size_type length)
        -> std::optional<encoding_type>
    {
        if (str == nullptr || length == 0)
        {
            return {};
        }

        if ((length % 4) != 0)
        {
            return {};
        }

        auto codes = reinterpret_cast<const unsigned int *>(str);
        auto code_length = length / 4;

        bool found_byte_order{false};
        Endian endian{};
        UTF32StringEncoder encoder{};

        if (*codes == 0xFEFF)
        {
            if (encoder.checkStringForCorrectness(str, length))
            {
                return {Encoding::UTF32};
            }
            return {};
        }
        else if (*codes == 0xFFFE0000)
        {
            if (encoder.checkStringForCorrectness(str, length))
            {
                return {Encoding::UTF32};
            }
            return {};
        }
        else
        {
            // We have no byte order mark, so we need to try and detect if the byte array
            // is encoded in either little-endian or big-endian encoding.  If the encoding
            // is in little-endian we will have XX XX (00-10) 00 in a repeating pattern.
            // If the encoding is in big-endian the encoding will be 00 (0-10) XX XX.  We
            // try and detect either of these patterns from the first four bytes.

            auto byte1 = *str;
            auto byte2 = *(str + 1);
            auto byte3 = *(str + 2);
            auto byte4 = *(str + 3);

            if (byte1 == 0 && (byte2 >= 0 && byte2 <= 10))
            {
                endian = Endian::BigEndian;
                found_byte_order = true;
            }
            else if ((byte3 >= 0 && byte3 <= 10) && byte4 == 0)
            {
                endian = Endian::LittleEndian;
                found_byte_order = true;
            }
        }

        if (! found_byte_order)
        {
            return {};
        }

        // Check to make sure we do not have just a byte order mark and nothing else.
        if ((code_length - 1) == 0)
        {
            return {};
        }

        if (endian == Endian::BigEndian)
        {
            // We need to run through the bytes and look for the pattern
            // 00 (00 - 10) XX XX for big-endian UTF32.

            // Since we have a BOM, start at the beginning + 4 bytes.
            auto start = str + 4;
            auto start_length = length - 4;

            for (size_type i = 0; i < start_length; i += 4)
            {
                auto byte1 = *(start + i);
                auto byte2 = *(start + i + 1);

                if (byte1 != 0)
                {
                    return {};
                }

                if (! (byte2 >= 0 && byte2 <= 10))
                {
                    return {};
                }
            }

            return {Encoding::UTF32};
        }
        else
        {
            // We need to run through the bytes and look for the pattern
            // XX XX (00 - 10) 00 for little-endian UTF32.

            // Since we have a BOM, start at the beginning + 4 bytes.
            auto start = str + 4;
            auto start_length = length - 4;

            for (size_type i = 0; i < start_length; i += 4)
            {
                auto byte1 = *(start + i + 2);
                auto byte2 = *(start + i + 3);

                if (! (byte1 >= 0 && byte1 <= 10))
                {
                    return {};
                }

                if (byte2 != 0)
                {
                    return {};
                }
            }

            return {Encoding::UTF32};
        }
    }

} // namespace TF::Foundation
