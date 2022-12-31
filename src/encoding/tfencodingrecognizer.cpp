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

#define NEEDS_VECTOR
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tfencodingrecognizer.hpp"
#include "tfencodingteststrategy.hpp"
#include "tfasciiencodingteststrategy.hpp"
#include "tfutf8encodingteststrategy.hpp"
#include "tfutf16encodingteststrategy.hpp"
#include "tfutf32encodingteststrategy.hpp"
#include "tfwindows1252encodingteststrategy.hpp"

namespace TF::Foundation
{

    EncodingRecognizer::EncodingRecognizer()
    {
        auto utf32_test_strategy = std::make_unique<UTF32EncodingTestStrategy>();
        auto utf16_test_strategy = std::make_unique<UTF16EncodingTestStrategy>();
        auto utf8_test_strategy = std::make_unique<UTF8EncodingTestStrategy>();
        auto ascii_test_strategy = std::make_unique<ASCIIEncodingTestStrategy>();
        auto windows1252_test_strategy = std::make_unique<Windows1252EncodingTestStrategy>();

        m_test_strategy_list.emplace_back(std::move(utf32_test_strategy));
        m_test_strategy_list.emplace_back(std::move(utf16_test_strategy));
        m_test_strategy_list.emplace_back(std::move(utf8_test_strategy));
        m_test_strategy_list.emplace_back(std::move(ascii_test_strategy));
        m_test_strategy_list.emplace_back(std::move(windows1252_test_strategy));
    }

    auto EncodingRecognizer::recognize_encoding(const unsigned char * str, size_type length)
        -> std::optional<encoding_type>
    {
        for (auto & strategy : m_test_strategy_list)
        {
            auto result = strategy->has_encoding(str, length);
            if (result)
            {
                return result;
            }
        }

        return {};
    }

} // namespace TF::Foundation