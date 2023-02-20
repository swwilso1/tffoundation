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
SOFTWARE.


******************************************************************************/

#include <cstring>
#include <string>
#include <string_view>
#include <iostream>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(StringTest, DefaultConstructorTest)
{
    String s1;
    EXPECT_TRUE(s1.empty());
}

#define TEST_STRING "test string"

TEST(StringTest, UTF8CStyleStringConstructorBasicTest)
{
    String s1(TEST_STRING);
    auto ptr = s1.cStr();
    EXPECT_TRUE(strncmp(TEST_STRING, ptr.get(), s1.length()) == 0);
}

#define TEST2_STRING "test 2 string with a bunch more characters in the string"

TEST(StringTest, UTF8CStyleStringConstructorBasic2Test)
{
    String s(TEST2_STRING);
    auto ptr = s.cStr();
    EXPECT_TRUE(strncmp(TEST2_STRING, ptr.get(), s.length()) == 0);
}

TEST(StringTest, UTF8CStyleStringConstructorWithUnicodeTest)
{
    String s("ЀЁЂЃЄЅфЉШ");
    EXPECT_TRUE(s.length() == 9);
    EXPECT_TRUE(s[0] == 0x400);
    EXPECT_TRUE(s[1] == 0x401);
    EXPECT_TRUE(s[2] == 0x402);
    EXPECT_TRUE(s[3] == 0x403);
    EXPECT_TRUE(s[4] == 0x404);
    EXPECT_TRUE(s[5] == 0x405);
    EXPECT_TRUE(s[6] == 0x444);
    EXPECT_TRUE(s[7] == 0x409);
    EXPECT_TRUE(s[8] == 0x428);
    EXPECT_TRUE(s == "ЀЁЂЃЄЅфЉШ");
}

TEST(StringTest, ASCIIEncodedUnicodeInitializerTest)
{
    String s = String::initWithASCIIEncodedUnicode("abc\\:002387def\\:002231");
    EXPECT_EQ(s.length(), 8);
    EXPECT_TRUE(s == "abc⎇def∱");
}

#define HELLO_WORLD "hello world"

TEST(StringTest, CStringConstructorWithLengthTest)
{
    auto tmp = std::unique_ptr<char[]>(new char[40]);
    std::memcpy(reinterpret_cast<void *>(tmp.get()), reinterpret_cast<void *>(const_cast<char *>(HELLO_WORLD)),
                strlen(HELLO_WORLD));
    String s(tmp.get(), strlen(HELLO_WORLD));
    auto ptr = s.cStr();
    EXPECT_TRUE(strncmp(HELLO_WORLD, ptr.get(), s.length()) == 0);
}

TEST(StringTest, CStringConstuctorWithLength2Test)
{
    String s("㆚㆛ぁあぃいぅうぇ", 27);
    EXPECT_EQ(s, "㆚㆛ぁあぃいぅうぇ");
}

TEST(StringTest, CStringConstructorAssignmentTest)
{
    String s = "Hello World";
    EXPECT_EQ(s, "Hello World");
}

TEST(StringTest, CXXStyleStringConstructorTest)
{
    std::string ss(HELLO_WORLD);
    String s(ss);
    auto ptr = s.cStr();
    EXPECT_TRUE(strncmp(HELLO_WORLD, ptr.get(), s.length()) == 0);
}

TEST(StringTest, StringViewConstructorTest)
{
    std::string_view sv(HELLO_WORLD);
    auto s = String::initWithStringView(sv);
    auto ptr = s.cStr();
    EXPECT_TRUE(strncmp(HELLO_WORLD, ptr.get(), s.length()) == 0);
}

TEST(StringTest, U8StringConstructorTest)
{
    String s{u8"㆚㆛ぁあぃいぅうぇ"};
    EXPECT_TRUE(s.length() == 9);
}

TEST(StringTest, UTF8StringConstructorTest)
{
    unsigned char tmp[10];

    tmp[0] = 0xE2;
    tmp[1] = 0x98;
    tmp[2] = 0x83;
    tmp[3] = 0xE2;
    tmp[4] = 0x98;
    tmp[5] = 0x84;
    tmp[6] = 0xF0;
    tmp[7] = 0x9D;
    tmp[8] = 0x8C;
    tmp[9] = 0x86;

    String s(tmp, 10);

    EXPECT_TRUE(s.length() == 3);
    EXPECT_EQ(s.getCharactersInRange(Range(0, 2)), "☃☄");
}

TEST(StringTest, UTF8StringConstructorTest2)
{
    const unsigned char * tmp = reinterpret_cast<const unsigned char *>("㆚㆛ぁあぃいぅうぇ");

    String s(tmp, 27);

    EXPECT_TRUE(s.length() == 9);
}

TEST(StringTest, U16ConstructorTest)
{
    String s{u"㆚㆛ぁあぃいぅうぇ"};
    EXPECT_TRUE(s.length() == 9);
}

TEST(StringTest, UTF16StringConstructorTest)
{
    unsigned short tmp[7];

    tmp[0] = 0xFEFF;
    tmp[1] = 0x3E6;
    tmp[2] = 0x419;
    tmp[3] = 0x46A;
    tmp[4] = 0x5D0;
    tmp[5] = 0xD800;
    tmp[6] = 0xDF88;

    String s(tmp, 7);

    EXPECT_TRUE(s.length() == 5);
}

TEST(StringTest, U32ConstructorTest)
{
    String s{U"㆚㆛ぁあぃいぅうぇ"};
    EXPECT_TRUE(s.length() == 9);
}

TEST(StringTest, UTF32StringConstructorTest)
{
    unsigned int tmp[7];

    tmp[0] = 0xFEFF;
    tmp[1] = 0x10388;
    tmp[2] = 0x5D0;
    tmp[3] = 0xD800;
    tmp[4] = 0x435;
    tmp[5] = 0x2939;
    tmp[6] = 0x10302;

    String s(tmp, 7);

    EXPECT_TRUE(s.length() == 6);
}

TEST(StringTest, CopyConstructorTest)
{
    String s("foo bar bat");
    String t(s);

    EXPECT_TRUE(s == t);
}

String RValueHelper(String a)
{
    return a;
}

TEST(StringTest, RValueConstructorTest)
{
    String s = RValueHelper(String::initWithFormat("foo bar"));
    auto ptr = s.cStr();
    EXPECT_TRUE(strncmp("foo bar", ptr.get(), s.length()) == 0);
}

TEST(StringTest, UnicodePointConstructorTest)
{
    String s(10022);
    EXPECT_EQ(s.length(), 1);
    EXPECT_EQ(s[0], 10022);
}

TEST(StringTest, JSONStringConstructorTest)
{
    // Basic simple ASCII content string.
    String s = String::initWithJSONEncodedUnicode("Hello World");
    EXPECT_EQ(s, "Hello World");

    // init with one escaped unicode char.
    s = String::initWithJSONEncodedUnicode("Hello World\\u2231");
    EXPECT_TRUE(s == "Hello World∱");

    // init with two escaped unicode chars.
    s = String::initWithJSONEncodedUnicode("abc\\u2387def\\u2231");
    EXPECT_TRUE(s == "abc⎇def∱");

    // init with broken unicode char:
    try
    {
        s = String::initWithJSONEncodedUnicode("abc\\u238zdef\\u2231");
    }
    catch (std::runtime_error & e)
    {}

    // init with an unfinished unicode char
    try
    {
        s = String::initWithJSONEncodedUnicode("abc\\u238");
    }
    catch (std::runtime_error & e)
    {}

    // init with a unicode surrogate pair
    s = String::initWithJSONEncodedUnicode("abc\\ud83d\\ude00");
    unsigned char tmp[7];
    tmp[0] = 'a';
    tmp[1] = 'b';
    tmp[2] = 'c';
    tmp[3] = 0xF0;
    tmp[4] = 0x9F;
    tmp[5] = 0x98;
    tmp[6] = 0x80;
    String tmp_string(tmp, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a bad unicode surrogate pair
    try
    {
        s = String::initWithJSONEncodedUnicode("abc\\ud83d\\u2231def");
    }
    catch (std::runtime_error & e)
    {}

    // init with a missing high surrogate
    try
    {
        s = String::initWithJSONEncodedUnicode("abc\\u2331\\ude00def");
    }
    catch (std::runtime_error & e)
    {}

    // init with a \b
    s = String::initWithJSONEncodedUnicode("abc\\bdef");
    unsigned char tmp2[7];
    tmp2[0] = 'a';
    tmp2[1] = 'b';
    tmp2[2] = 'c';
    tmp2[3] = '\b';
    tmp2[4] = 'd';
    tmp2[5] = 'e';
    tmp2[6] = 'f';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a '\'
    s = String::initWithJSONEncodedUnicode("abc\\\\def");
    tmp2[3] = '\\';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a '/'
    s = String::initWithJSONEncodedUnicode("abc\\/def");
    tmp2[3] = '/';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a \f
    s = String::initWithJSONEncodedUnicode("abc\\fdef");
    tmp2[3] = '\f';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a \n
    s = String::initWithJSONEncodedUnicode("abc\\ndef");
    tmp2[3] = '\n';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a \r
    s = String::initWithJSONEncodedUnicode("abc\\rdef");
    tmp2[3] = '\r';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);

    // init with a \t
    s = String::initWithJSONEncodedUnicode("abc\\tdef");
    tmp2[3] = '\t';
    tmp_string = String(tmp2, 7);
    EXPECT_EQ(s, tmp_string);
}

TEST(StringTest, Windows1252StringConstructionTest)
{
    unsigned char simple_characters[128];
    for (int64_t i = 0; i < 128; i++)
    {
        simple_characters[i] = static_cast<unsigned char>(i);
    }

    char simple_string_chars[128];
    for (int64_t i = 0; i < 128; i++)
    {
        simple_string_chars[i] = static_cast<char>(i);
    }

    auto windows_string1 = String::initWithWindows1252(simple_characters, 128);
    String simple_string1{simple_string_chars, 128};

    EXPECT_EQ(windows_string1, simple_string1);

    unsigned char end_simple_characters[96];
    for (int64_t i = 0; i < 96; i++)
    {
        end_simple_characters[i] = static_cast<unsigned char>(i + 160);
    }

    unsigned int end_simple_unicode_codes[98];
    end_simple_unicode_codes[0] = 0xFEFF;
    for (int64_t i = 1; i < 98; i++)
    {
        end_simple_unicode_codes[i] = static_cast<unsigned int>(i + 159);
    }

    windows_string1 = String::initWithWindows1252(end_simple_characters, 96);
    String simple_string2{end_simple_unicode_codes, 97};

    EXPECT_EQ(windows_string1, simple_string2);

    unsigned char complex_characters[32];
    for (int64_t i = 0; i < 32; i++)
    {
        complex_characters[i] = static_cast<unsigned char>(i) + 0x80;
    }

    auto windows_complicated_string1 = String::initWithWindows1252(complex_characters, 32);

    unsigned int complex_codes[33];
    complex_codes[0] = 0xfeff;
    complex_codes[1] = 0x20ac;
    complex_codes[2] = 0x0081;
    complex_codes[3] = 0x201a;
    complex_codes[4] = 0x0192;
    complex_codes[5] = 0x201e;
    complex_codes[6] = 0x2026;
    complex_codes[7] = 0x2020;
    complex_codes[8] = 0x2021;
    complex_codes[9] = 0x02c6;
    complex_codes[10] = 0x2030;
    complex_codes[11] = 0x0160;
    complex_codes[12] = 0x2039;
    complex_codes[13] = 0x0152;
    complex_codes[14] = 0x008d;
    complex_codes[15] = 0x017d;
    complex_codes[16] = 0x008f;
    complex_codes[17] = 0x0090;
    complex_codes[18] = 0x2018;
    complex_codes[19] = 0x2019;
    complex_codes[20] = 0x201c;
    complex_codes[21] = 0x201d;
    complex_codes[22] = 0x2022;
    complex_codes[23] = 0x2013;
    complex_codes[24] = 0x2014;
    complex_codes[25] = 0x02dc;
    complex_codes[26] = 0x2122;
    complex_codes[27] = 0x0161;
    complex_codes[28] = 0x203a;
    complex_codes[29] = 0x0153;
    complex_codes[30] = 0x009d;
    complex_codes[31] = 0x017e;
    complex_codes[32] = 0x0178;

    String complex_string{complex_codes, 33};

    EXPECT_EQ(windows_complicated_string1, complex_string);
}
