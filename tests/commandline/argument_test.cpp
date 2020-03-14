/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2020 Tectiform Inc.

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

TEST(ArgumentTest, NamesTest)
{
    Argument arg1;
    arg1.setShortName("-f");
    EXPECT_EQ(arg1.getShortName(), "-f");
    arg1.setLongName("--foo");
    EXPECT_EQ(arg1.getLongName(), "--foo");

    auto names = arg1.getNames();
    EXPECT_EQ(names[0], "-f");
    EXPECT_EQ(names[1], "--foo");

    arg1.setNames({"-p", "--pow"});
    EXPECT_EQ(arg1.getShortName(), "-p");
    EXPECT_EQ(arg1.getLongName(), "--pow");
    EXPECT_EQ(arg1.getShortestName(), "-p");
    EXPECT_EQ(arg1.getShortNameWithoutPrefix("-"), "p");
    EXPECT_EQ(arg1.getLongNameWithoutPrefix("-"), "pow");
}

TEST(ArgumentTest, SetterGetterTest)
{
    Argument arg;

    arg.setAction(ArgumentAction::Store);
    EXPECT_EQ(arg.getAction(), ArgumentAction::Store);

    arg.setNumberOfArgs(5);
    EXPECT_EQ(arg.getNumberOfArgs(), 5);

    arg.setDefault("5");
    EXPECT_EQ(arg.getDefault(), "5");

    arg.setConstant("3.14159");
    EXPECT_EQ(arg.getConstant(), "3.14159");

    arg.setArgumentType(ArgumentType::Bool);
    EXPECT_EQ(arg.getArgumentType(), ArgumentType::Bool);

    arg.setDestination("foo");
    arg.setLongName("--bar");
    EXPECT_EQ(arg.getDestination("-"), "foo");

    arg.setChoices({"foo", "bar", "bat"});
    EXPECT_EQ(arg.getChoices()[0], "foo");
    EXPECT_EQ(arg.getChoices()[1], "bar");
    EXPECT_EQ(arg.getChoices()[2], "bat");

    arg.setRequired(true);
    EXPECT_EQ(arg.getRequired(), true);

    Argument arg2;
    arg2.setHelp("A string of text");
    EXPECT_EQ(arg2.getHelp(), "A string of text");
}

TEST(ArgumentTest, ValuesMatchTest)
{
    Argument arg;

    arg.setNames({"-foo", "--foobar"});
    EXPECT_TRUE(arg.valueMatchesName("-foo"));
    EXPECT_TRUE(arg.valueMatchesName("--foobar"));
    EXPECT_FALSE(arg.valueMatchesName("-bar"));

    arg.setChoices({"one", "two", "three"});
    EXPECT_TRUE(arg.valueMatchesChoice("one"));
    EXPECT_TRUE(arg.valueMatchesChoice("two"));
    EXPECT_TRUE(arg.valueMatchesChoice("three"));
    EXPECT_FALSE(arg.valueMatchesChoice("four"));
    EXPECT_TRUE(arg.valuesMatchChoices({"one", "two"}));
    EXPECT_EQ(arg.valueThatDoesNotMatchChoice({"one", "two", "four"}), "four");
    EXPECT_TRUE(arg.hasChoices());
}

TEST(ArgumentTest, OptionalVsPositionalTest)
{
    Argument arg, arg2;

    arg.setNames({"-n", "--new"});
    EXPECT_TRUE(arg.argumentIsOptional("-"));

    arg2.setLongName("cow");
    EXPECT_FALSE(arg2.argumentIsOptional("-"));
}

TEST(ArgumentTest, ShortestNameTest)
{
    Argument arg;

    arg.setShortName("-f");
    arg.setLongName("--foo");
    EXPECT_EQ(arg.getShortestName(), "-f");

    arg.setShortName("--foo");
    arg.setLongName("-f");
    EXPECT_EQ(arg.getShortestName(), "-f");
}

TEST(ArgumentTest, HelpNames)
{
    Argument arg;
    String names("-f, -foobar");

    arg.setNames({"-f", "-foobar"});
    EXPECT_EQ(arg.namesForHelp(), names);
    EXPECT_EQ(arg.lengthOfNamesForHelp(), names.length());
}

TEST(ArgumentTest, ConsumationTest)
{
    Argument arg;

    arg.setNumberOfArgs(3);
    EXPECT_EQ(arg.valuesConsumed(), 0);
    arg.consumeValue();
    EXPECT_EQ(arg.valuesConsumed(), 1);
    EXPECT_TRUE(arg.consumesMoreValues());
    arg.consumeValue();
    EXPECT_EQ(arg.valuesConsumed(), 2);
    EXPECT_TRUE(arg.consumesMoreValues());
    arg.consumeValue();
    EXPECT_EQ(arg.valuesConsumed(), 3);
    EXPECT_FALSE(arg.consumesMoreValues());
}

TEST(ArgumentTest, MatchedTest)
{
    Argument arg;

    arg.setMatched(true);
    EXPECT_TRUE(arg.getMatched());
}
