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

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(Base64Test, SimpleEncodeTest)
{
    String s("Man is distinguished, not only by his reason, but by this singular passion from other animals, "
             "which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable "
             "generation of knowledge, exceeds the short vehemence of any carnal pleasure.");
    auto d = s.getAsDataInASCIIEncoding();
    auto out = Base64::encode(d);
    EXPECT_EQ(out, "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
                   "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
                   "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
                   "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
                   "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
}

TEST(Base64Test, PaddingEncodeTests)
{
    String s("any carnal pleasure.");
    String::data_type d = s.getAsDataInASCIIEncoding();
    String out = Base64::encode(d);
    EXPECT_EQ(out, "YW55IGNhcm5hbCBwbGVhc3VyZS4=");

    s = "any carnal pleasure";
    d = s.getAsDataInASCIIEncoding();
    out = Base64::encode(d);
    EXPECT_EQ(out, "YW55IGNhcm5hbCBwbGVhc3VyZQ==");

    s = "any carnal pleasur";
    d = s.getAsDataInASCIIEncoding();
    out = Base64::encode(d);
    EXPECT_EQ(out, "YW55IGNhcm5hbCBwbGVhc3Vy");

    s = "any carnal pleasu";
    d = s.getAsDataInASCIIEncoding();
    out = Base64::encode(d);
    EXPECT_EQ(out, "YW55IGNhcm5hbCBwbGVhc3U=");

    s = "any carnal pleas";
    d = s.getAsDataInASCIIEncoding();
    out = Base64::encode(d);
    EXPECT_EQ(out, "YW55IGNhcm5hbCBwbGVhcw==");

    s = "Hello World, how are you?";
    d = s.getAsDataInASCIIEncoding();
    out = Base64::encode(d);
    EXPECT_EQ(out, "SGVsbG8gV29ybGQsIGhvdyBhcmUgeW91Pw==");
}

TEST(Base64Test, SimpleDecodeTest)
{
    String a("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
             "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
             "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
             "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
             "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
    String b("Man is distinguished, not only by his reason, but by this singular passion from other animals, "
             "which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable "
             "generation of knowledge, exceeds the short vehemence of any carnal pleasure.");
    Data bAsData = b.getAsDataInASCIIEncoding();
    Data out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);
}

TEST(Base64Test, PaddingDecodeTest)
{
    String a("YW55IGNhcm5hbCBwbGVhc3VyZS4=");
    String b("any carnal pleasure.");
    Data bAsData = b.getAsDataInASCIIEncoding();
    Data out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);

    a = "YW55IGNhcm5hbCBwbGVhc3VyZQ==";
    b = "any carnal pleasure";
    bAsData = b.getAsDataInASCIIEncoding();
    out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);

    a = "YW55IGNhcm5hbCBwbGVhc3Vy";
    b = "any carnal pleasur";
    bAsData = b.getAsDataInASCIIEncoding();
    out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);

    a = "YW55IGNhcm5hbCBwbGVhc3U=";
    b = "any carnal pleasu";
    bAsData = b.getAsDataInASCIIEncoding();
    out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);

    a = "YW55IGNhcm5hbCBwbGVhcw==";
    b = "any carnal pleas";
    bAsData = b.getAsDataInASCIIEncoding();
    out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);

    a = "SGVsbG8gV29ybGQsIGhvdyBhcmUgeW91Pw==";
    b = "Hello World, how are you?";
    bAsData = b.getAsDataInASCIIEncoding();
    out = Base64::decode(a);
    EXPECT_EQ(bAsData, out);
}
