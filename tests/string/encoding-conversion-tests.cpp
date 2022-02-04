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
#include <iostream>
#include <utility>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(StringTest, GetAsDataInASCIIEncodingTest)
{
    unsigned int array[5];

    array[0] = 0xFEFF;
    array[1] = 0x3893;
    array[2] = 0x103432;
    array[3] = 0x3832;
    array[4] = 0x0080;

    String s(array, 5);
    auto theData = s.getAsDataInASCIIEncoding();
    EXPECT_EQ(theData.length(), 32);
    EXPECT_EQ(theData[0], '\\');
    EXPECT_EQ(theData[1], ':');
    EXPECT_EQ(theData[2], '0');
    EXPECT_EQ(theData[3], '0');
    EXPECT_EQ(theData[4], '3');
    EXPECT_EQ(theData[5], '8');
    EXPECT_EQ(theData[6], '9');
    EXPECT_EQ(theData[7], '3');

    EXPECT_EQ(theData[8], '\\');
    EXPECT_EQ(theData[9], ':');
    EXPECT_EQ(theData[10], '1');
    EXPECT_EQ(theData[11], '0');
    EXPECT_EQ(theData[12], '3');
    EXPECT_EQ(theData[13], '4');
    EXPECT_EQ(theData[14], '3');
    EXPECT_EQ(theData[15], '2');

    EXPECT_EQ(theData[16], '\\');
    EXPECT_EQ(theData[17], ':');
    EXPECT_EQ(theData[18], '0');
    EXPECT_EQ(theData[19], '0');
    EXPECT_EQ(theData[20], '3');
    EXPECT_EQ(theData[21], '8');
    EXPECT_EQ(theData[22], '3');
    EXPECT_EQ(theData[23], '2');

    EXPECT_EQ(theData[24], '\\');
    EXPECT_EQ(theData[25], ':');
    EXPECT_EQ(theData[26], '0');
    EXPECT_EQ(theData[27], '0');
    EXPECT_EQ(theData[28], '0');
    EXPECT_EQ(theData[29], '0');
    EXPECT_EQ(theData[30], '8');
    EXPECT_EQ(theData[31], '0');
}

TEST(StringTest, GetAsDataInUTF8Encoding)
{
    unsigned int array[5];

    array[0] = 0xFEFF;
    array[1] = 0x3893;
    array[2] = 0x103432;
    array[3] = 0x3832;
    array[4] = 0x0080;

    String s(array, 5);
    auto theData = s.getAsDataInUTF8Encoding();

    EXPECT_EQ(theData.length(), 12);

    EXPECT_EQ(theData[0], 0xE3);
    EXPECT_EQ(theData[1], 0xA2);
    EXPECT_EQ(theData[2], 0x93);

    EXPECT_EQ(theData[3], 0xF4);
    EXPECT_EQ(theData[4], 0x83);
    EXPECT_EQ(theData[5], 0x90);
    EXPECT_EQ(theData[6], 0xB2);

    EXPECT_EQ(theData[7], 0xE3);
    EXPECT_EQ(theData[8], 0xA0);
    EXPECT_EQ(theData[9], 0xB2);

    EXPECT_EQ(theData[10], 0xC2);
    EXPECT_EQ(theData[11], 0x80);
}

TEST(StringTest, GetAsDataInUTF16Encoding)
{
    unsigned int array[5];

    array[0] = 0xFEFF;
    array[1] = 0x3893;
    array[2] = 0x103432;
    array[3] = 0x3832;
    array[4] = 0x0080;

    String s(array, 5);
    auto theData = s.getAsDataInUTF16Encoding();

    EXPECT_EQ(theData.length(), 12);

    if (theData[0] == 0xFF)
    {
        // Little Ending encoding.

        // Byte order mark
        EXPECT_EQ(theData[0], 0xFF);
        EXPECT_EQ(theData[1], 0xFE);

        EXPECT_EQ(theData[2], 0x93);
        EXPECT_EQ(theData[3], 0x38);

        EXPECT_EQ(theData[4], 0xCD);
        EXPECT_EQ(theData[5], 0xDB);
        EXPECT_EQ(theData[6], 0x32);
        EXPECT_EQ(theData[7], 0xDC);

        EXPECT_EQ(theData[8], 0x32);
        EXPECT_EQ(theData[9], 0x38);

        EXPECT_EQ(theData[10], 0x80);
        EXPECT_EQ(theData[11], 0);
    }
    else
    {
        // Big Endian Encoding
        EXPECT_EQ(theData[0], 0xFE);
        EXPECT_EQ(theData[1], 0xFF);

        EXPECT_EQ(theData[2], 0x38);
        EXPECT_EQ(theData[3], 0x93);

        EXPECT_EQ(theData[4], 0xDB);
        EXPECT_EQ(theData[5], 0xCD);
        EXPECT_EQ(theData[6], 0xDC);
        EXPECT_EQ(theData[7], 0x32);

        EXPECT_EQ(theData[8], 0x38);
        EXPECT_EQ(theData[9], 0x32);

        EXPECT_EQ(theData[10], 0);
        EXPECT_EQ(theData[11], 0x80);
    }
}

TEST(StringTest, GetAsDataInUTF32Encoding)
{
    unsigned int array[5];

    array[0] = 0xFEFF;
    array[1] = 0x3893;
    array[2] = 0x103432;
    array[3] = 0x3832;
    array[4] = 0x0080;

    String s(array, 5);
    auto theData = s.getAsDataInUTF32Encoding();

    EXPECT_EQ(theData.length(), 20);

    if (theData[0] == 0xFF)
    {
        // Little Endian encoding.
        EXPECT_EQ(theData[0], 0xFF);
        EXPECT_EQ(theData[1], 0xFE);
        EXPECT_EQ(theData[2], 0);
        EXPECT_EQ(theData[3], 0);

        EXPECT_EQ(theData[4], 0x93);
        EXPECT_EQ(theData[5], 0x38);
        EXPECT_EQ(theData[6], 0);
        EXPECT_EQ(theData[7], 0);

        EXPECT_EQ(theData[8], 0x32);
        EXPECT_EQ(theData[9], 0x34);
        EXPECT_EQ(theData[10], 0x10);
        EXPECT_EQ(theData[11], 0);

        EXPECT_EQ(theData[12], 0x32);
        EXPECT_EQ(theData[13], 0x38);
        EXPECT_EQ(theData[14], 0);
        EXPECT_EQ(theData[15], 0);

        EXPECT_EQ(theData[16], 0x80);
        EXPECT_EQ(theData[17], 0);
        EXPECT_EQ(theData[18], 0);
        EXPECT_EQ(theData[19], 0);
    }
    else
    {
        // Big Endian Encoding
        EXPECT_EQ(theData[0], 0);
        EXPECT_EQ(theData[1], 0);
        EXPECT_EQ(theData[2], 0xFE);
        EXPECT_EQ(theData[3], 0xFF);

        EXPECT_EQ(theData[4], 0);
        EXPECT_EQ(theData[5], 0);
        EXPECT_EQ(theData[6], 0x38);
        EXPECT_EQ(theData[7], 0x93);

        EXPECT_EQ(theData[8], 0);
        EXPECT_EQ(theData[9], 0x10);
        EXPECT_EQ(theData[10], 0x34);
        EXPECT_EQ(theData[11], 0x32);

        EXPECT_EQ(theData[12], 0);
        EXPECT_EQ(theData[13], 0);
        EXPECT_EQ(theData[14], 0x38);
        EXPECT_EQ(theData[15], 0x32);

        EXPECT_EQ(theData[16], 0);
        EXPECT_EQ(theData[17], 0);
        EXPECT_EQ(theData[18], 0);
        EXPECT_EQ(theData[19], 0x80);
    }
}

TEST(StringTest, GetAsDataInJSONEncoding)
{
    unsigned int array[5];

    array[0] = 0xFEFF;
    array[1] = 0x3893;
    array[2] = 0x103432;
    array[3] = 0x3832;
    array[4] = 0x0080;

    String s(array, 5);
    auto theData = s.getAsDataInUTF8Encoding();

    EXPECT_EQ(theData.length(), 12);

    EXPECT_EQ(theData[0], 0xE3);
    EXPECT_EQ(theData[1], 0xA2);
    EXPECT_EQ(theData[2], 0x93);

    EXPECT_EQ(theData[3], 0xF4);
    EXPECT_EQ(theData[4], 0x83);
    EXPECT_EQ(theData[5], 0x90);
    EXPECT_EQ(theData[6], 0xB2);

    EXPECT_EQ(theData[7], 0xE3);
    EXPECT_EQ(theData[8], 0xA0);
    EXPECT_EQ(theData[9], 0xB2);

    EXPECT_EQ(theData[10], 0xC2);
    EXPECT_EQ(theData[11], 0x80);
}
