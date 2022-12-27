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

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(EncodingRecognizerTest, UTF8Recognizer)
{
    auto text = reinterpret_cast<const unsigned char *>("ЀЁЂЃЄЅфЉШ");
    EncodingRecognizer recognizer{};
    auto result = recognizer.recognize_encoding(text, 19);
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), Encoding::UTF8);
}

TEST(EncodingRecognizerTest, UTF16Recognizer)
{
    EncodingRecognizer recognizer{};
    unsigned short tmp[7];

    tmp[0] = 0xFEFF;
    tmp[1] = 0x3E6;
    tmp[2] = 0x419;
    tmp[3] = 0x46A;
    tmp[4] = 0x5D0;
    tmp[5] = 0xD800;
    tmp[6] = 0xDF88;

    auto result = recognizer.recognize_encoding(reinterpret_cast<unsigned char *>(tmp), sizeof(tmp));
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), Encoding::UTF16);
}

TEST(EncodingRecognizerTest, UTF32Recognizer)
{
    EncodingRecognizer recognizer{};
    auto str = U"㆚㆛ぁあぃいぅうぇ";
    auto result = recognizer.recognize_encoding(reinterpret_cast<const unsigned char *>(str), sizeof(char32_t) * 8);
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), Encoding::UTF32);
}

// Since ASCII is a subset of UTF-8, this test will always return UTF-8.
TEST(EncodingRecognizerTest, ASCIIRecognizer)
{
    EncodingRecognizer recognizer{};
    auto str = "The rain in Spain falls mainly on the plain.";
    auto result = recognizer.recognize_encoding(reinterpret_cast<const unsigned char *>(str), std::strlen(str));
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), Encoding::UTF8);
}

TEST(EncodingRecognizerTest, Windows1252Recognizer)
{
    EncodingRecognizer recognizer{};
    unsigned char complex_characters[32];
    for (int64_t i = 0; i < 32; i++)
    {
        complex_characters[i] = static_cast<unsigned char>(i) + 0x80;
    }
    auto result = recognizer.recognize_encoding(complex_characters, sizeof(complex_characters));
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), Encoding::WINDOWS1252);
}
