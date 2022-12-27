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

#include "tfwindows1252encodingteststrategy.hpp"

namespace TF::Foundation
{

    auto Windows1252EncodingTestStrategy::has_encoding(const unsigned char * str, size_type length)
        -> std::optional<encoding_type>
    {
        if (str == nullptr || length == 0)
        {
            return {};
        }

        // Windows 1252 (CP 1252) is a single byte encoding with encoded values for every byte value between 0-255.
        // We have no way to test for this encoding at the moment, so if a recognizer calls this strategy, we assume
        // that we in fact, have, Windows 1252.
        return {Encoding::WINDOWS1252};
    }

} // namespace TF::Foundation
