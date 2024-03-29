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

TEST(StringTest, SubstringFromIndexTest)
{
    String s("A house high in the hills overlooks those high in oxygination");
    String t = s.substringFromIndex(0);
    EXPECT_EQ(t, s);
}

TEST(StringTest, SubstringFromIndexTest2)
{
    const char * a[16];

    a[0] = "A big red house";
    a[1] = " big red house";
    a[2] = "big red house";
    a[3] = "ig red house";
    a[4] = "g red house";
    a[5] = " red house";
    a[6] = "red house";
    a[7] = "ed house";
    a[8] = "d house";
    a[9] = " house";
    a[10] = "house";
    a[11] = "ouse";
    a[12] = "use";
    a[13] = "se";
    a[14] = "e";
    a[15] = "";

    String s("A big red house");

    for (size_t i = 0; i < s.length(); i++)
    {
        EXPECT_EQ(s.substringFromIndex(i), a[i]);
    }
}

TEST(StringTest, SubstringFromIndexTest3)
{
    String s = String::initWithASCIIEncodedUnicode("abc\\:103333\\:00003445");

    const char * a[8];

    a[0] = "abc\\:103333\\:00003445";
    a[1] = "bc\\:103333\\:00003445";
    a[2] = "c\\:103333\\:00003445";
    a[3] = "\\:103333\\:00003445";
    a[4] = "\\:00003445";
    a[5] = "45";
    a[6] = "5";
    a[7] = "";

    for (size_t i = 0; i < s.length(); i++)
    {
        EXPECT_EQ(s.substringFromIndex(i), String::initWithASCIIEncodedUnicode(a[i]));
    }
}

TEST(StringTest, SubstringFromIndexTest4)
{
    String s("abc");
    EXPECT_ANY_THROW(auto v = s.substringFromIndex(3));
}

TEST(StringTest, SubstringWithRangeTest)
{
    String s("abc");
    EXPECT_ANY_THROW(auto v = s.substringWithRange(Range(0, 4)));
}

TEST(StringTest, SubstringWithRangeTest2)
{
    String s("Would you like to play a game?");
    auto v = s.substringWithRange(Range(5, 5));
    EXPECT_EQ(v, " you ");
}

TEST(StringTest, SubstringWithRangeTest3)
{
    String s = String::initWithASCIIEncodedUnicode("Would you \\:004432 like to play \\:000034 a game?");
    auto v = s.substringWithRange(Range(10, 18));
    EXPECT_EQ(v, "䐲 like to play 4 a");
}

TEST(StringTest, SubstringsNotInRangeTest)
{
    String s("The quick brown fox jumped over the log.");
    auto substrings = s.substringsNotInRange(Range(16, 3));
    EXPECT_EQ(substrings.size(), 2);
    EXPECT_EQ(substrings[0], "The quick brown ");
    EXPECT_EQ(substrings[1], " jumped over the log.");
}

TEST(StringTest, SubstringsNotInRangeTest2)
{
    String s("The quick brown fox jumped over the log.");
    auto substrings = s.substringsNotInRange(Range(0, 5));
    EXPECT_EQ(substrings.size(), 1);
    EXPECT_EQ(substrings[0], "uick brown fox jumped over the log.");
}

TEST(StringTest, SubstringsNotInRangeTest3)
{
    String s("The quick brown fox jumped over the log.");
    auto substrings = s.substringsNotInRange(Range(15, 25));
    EXPECT_EQ(substrings.size(), 1);
    EXPECT_EQ(substrings[0], "The quick brown");
}

TEST(StringTest, SubstringsNotInRangeTest4)
{
    String s = String::initWithASCIIEncodedUnicode("Hello \\:002233, W\\:000035orldish");
    auto substrings = s.substringsNotInRange(Range(7, 2));
    EXPECT_EQ(substrings.size(), 2);
    EXPECT_EQ(substrings[0], String::initWithASCIIEncodedUnicode("Hello \\:002233"));
    EXPECT_EQ(substrings[1], "W5orldish");
}

TEST(StringTest, SubstringsNotInRangeTest5)
{
    String s("Hello World");
    auto substrings = s.substringsNotInRange(Range(0, 40));
    EXPECT_EQ(substrings.size(), 0);
}

TEST(StringTest, SubstringToIndexTest)
{
    String s("Butternut wood is best");
    auto t = s.substringToIndex(15);
    EXPECT_EQ(t, "Butternut wood ");
}

TEST(StringTest, SubstringToIndexTest2)
{
    String s("Basswood wood is best");
    EXPECT_ANY_THROW(auto t = s.substringToIndex(30));
}

TEST(StringTest, SubstringToIndexTest3)
{
    String s("Basswood wood is best");
    EXPECT_ANY_THROW(auto t = s.substringToIndex(21));
}

TEST(StringTest, SubstringsThatDoNotMatchStringTest)
{
    String s("How now brown now cow");
    auto substrings = s.substringsThatDoNotMatchString(String("now"));
    EXPECT_EQ(substrings.size(), 3);
    EXPECT_EQ(substrings[0], "How ");
    EXPECT_EQ(substrings[1], " brown ");
    EXPECT_EQ(substrings[2], " cow");
}

TEST(StringTest, SubstringsThatDoNotMatchStringTest2)
{
    String s("How now brown cow");
    auto substrings = s.substringsThatDoNotMatchString("ow");
    EXPECT_EQ(substrings.size(), 4);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n c");
}

TEST(StringTest, SubstringsThatDoNotMatchStringTest3)
{
    String s("How now brown owow cow");
    auto substrings = s.substringsThatDoNotMatchString("ow");
    EXPECT_EQ(substrings.size(), 6);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n ");
    EXPECT_EQ(substrings[4], String{});
    EXPECT_EQ(substrings[5], " c");
}

TEST(StringTest, SubstringsThatDoNotMatchStringTest4)
{
    String s("How now brown cowow");
    auto substrings = s.substringsThatDoNotMatchString("ow");
    EXPECT_EQ(substrings.size(), 5);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n c");
    EXPECT_EQ(substrings[4], String{});
}

TEST(StringTest, SubstringsThatDoNotMatchStringTest5)
{
    String s("How now brown owow cowow");
    auto substrings = s.substringsThatDoNotMatchString("ow");
    EXPECT_EQ(substrings.size(), 7);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n ");
    EXPECT_EQ(substrings[4], String{});
    EXPECT_EQ(substrings[5], " c");
    EXPECT_EQ(substrings[6], String{});
}

TEST(StringTest, SubstringsThatDoNotMatchStringTest6)
{
    String s = String::initWithASCIIEncodedUnicode(
        "H\\:004455\\:003421 n\\:004455\\:003421 br\\:004455\\:003421n c\\:004455\\:003421");
    auto substrings = s.substringsThatDoNotMatchString("䑕㐡");
    EXPECT_EQ(substrings.size(), 4);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n c");
}

TEST(StringTest, SplitTest)
{
    String s("How now brown now cow");
    auto substrings = s.split(String("now"));
    EXPECT_EQ(substrings.size(), 3);
    EXPECT_EQ(substrings[0], "How ");
    EXPECT_EQ(substrings[1], " brown ");
    EXPECT_EQ(substrings[2], " cow");
}

TEST(StringTest, SplitTest2)
{
    String s("How now brown cow");
    auto substrings = s.split("ow");
    EXPECT_EQ(substrings.size(), 4);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n c");
}

TEST(StringTest, SplitTest3)
{
    String s = String::initWithASCIIEncodedUnicode(
        "H\\:004455\\:003421 n\\:004455\\:003421 br\\:004455\\:003421n c\\:004455\\:003421");
    auto substrings = s.split("䑕㐡");
    EXPECT_EQ(substrings.size(), 4);
    EXPECT_EQ(substrings[0], "H");
    EXPECT_EQ(substrings[1], " n");
    EXPECT_EQ(substrings[2], " br");
    EXPECT_EQ(substrings[3], "n c");
}

TEST(StringTest, SplitTest4)
{
    String lorumIpsum = String::initWithASCIIEncodedUnicode(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut "
        "labore et dolore magna aliqua. Eu mi bibendum neque egestas. Nisi vitae suscipit tellus mauris "
        "a diam maecenas sed enim. Sit amet cursus sit amet. Vehicula ipsum a arcu cursus vitae congue. "
        "Consectetur adipiscing elit duis tristique sollicitudin nibh sit. At tempor commodo ullamcorper"
        " a lacus vestibulum sed. Sapien eget mi proin sed libero enim sed. Lacinia quis vel eros donec "
        "ac odio. Tempor orci dapibus ultrices in iaculis nunc. Felis eget velit aliquet sagittis id.\n"
        "\n"
        "Et magnis dis parturient montes nascetur. Euismod elementum nisi quis eleifend quam adipiscing "
        "vitae proin sagittis. Amet nisl suscipit adipiscing bibendum est. Tristique senectus et netus "
        "et malesuada fames. Cras sed felis eget velit aliquet sagittis. Justo donec enim diam vulputate"
        " ut pharetra sit amet aliquam. Laoreet suspendisse interdum consectetur libero. Nunc mattis "
        "enim ut tellus elementum sagittis vitae et leo. Accumsan in nisl nisi scelerisque eu ultrices "
        "vitae. Montes nascetur ridiculus mus mauris vitae ultricies. Fermentum odio eu feugiat pretium "
        "nibh. Sit amet commodo nulla facilisi nullam vehicula ipsum a. Urna id volutpat lacus laoreet "
        "non. Donec ultrices tincidunt arcu non sodales neque. Viverra mauris in aliquam sem fringilla "
        "ut morbi tincidunt. Mus mauris vitae ultricies leo integer malesuada nunc vel risus. Tellus in "
        "metus vulputate eu scelerisque felis imperdiet.\n"
        "\n"
        "Viverra ipsum nunc aliquet bibendum enim facilisis. Sem viverra aliquet eget sit amet tellus "
        "cras adipiscing enim. Lacus luctus accumsan tortor posuere. Proin sagittis nisl rhoncus mattis "
        "rhoncus urna neque viverra justo. Gravida in fermentum et sollicitudin ac orci. Tortor vitae "
        "purus faucibus ornare suspendisse sed nisi. Augue interdum velit euismod in pellentesque massa."
        " Placerat in egestas erat imperdiet sed. Neque vitae tempus quam pellentesque nec nam aliquam "
        "sem. Commodo sed egestas egestas fringilla phasellus faucibus. Ut ornare lectus sit amet est "
        "placerat in. Ac turpis egestas integer eget aliquet nibh praesent tristique magna. Id volutpat "
        "lacus laoreet non curabitur gravida. Adipiscing enim eu turpis egestas.\n"
        "\n"
        "Etiam erat velit scelerisque in dictum non. Rutrum quisque non tellus orci ac auctor. Dui "
        "accumsan sit amet nulla facilisi morbi tempus iaculis. Morbi tincidunt augue interdum velit "
        "euismod in pellentesque massa. Risus nullam eget felis eget nunc lobortis mattis aliquam "
        "faucibus. Urna duis convallis convallis tellus id interdum velit. Imperdiet sed euismod nisi "
        "porta lorem mollis aliquam ut porttitor. Nullam ac tortor vitae purus faucibus ornare "
        "suspendisse. Leo integer malesuada nunc vel risus. Ac tincidunt vitae semper quis lectus "
        "nulla at volutpat. Accumsan sit amet nulla facilisi.\n"
        "\n"
        "Amet mattis vulputate enim nulla. Aliquam nulla facilisi cras fermentum odio eu. Sed id semper "
        "risus in hendrerit gravida rutrum. Viverra orci sagittis eu volutpat odio. Sit amet mattis "
        "vulputate enim. Enim eu turpis egestas pretium aenean pharetra magna ac placerat. Ac tortor "
        "dignissim convallis aenean et tortor at. Imperdiet sed euismod nisi porta lorem mollis. "
        "Aliquet lectus proin nibh nisl. Varius vel pharetra vel turpis nunc. Nunc sed id semper risus "
        "in hendrerit gravida rutrum. Gravida neque convallis a cras semper auctor neque vitae tempus. "
        "Scelerisque in dictum non consectetur.\n\n");

    auto lorem_ipsum_split_by_newline = lorumIpsum.split("\n");
    EXPECT_EQ(lorem_ipsum_split_by_newline.size(), 10);
}
