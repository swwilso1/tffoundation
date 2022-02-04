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

TEST(ArgumentParserTest, OneArgTest)
{
    ArgumentParser parser;
    ArgumentParser::string_type name;
    parser.addStoreArgument({"-n", "--name"}, 1, "", ArgumentType::String, "", {}, "Name of option");
    parser.addStoreArgument({"-i", "--integer"}, 1, "", ArgumentType::Int, "", {}, "Help text");

    EXPECT_TRUE(parser.parseArgs("-n Earl"));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl");

    int i = 0;
    EXPECT_TRUE(parser.parseArgs("-i 10"));
    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_EQ(i, 10);
}

TEST(ArgumentParserTest, OneArgQuotesTest)
{
    ArgumentParser parser;
    ArgumentParser::string_type name;
    parser.addStoreArgument({"-n", "--name"}, 1, "", ArgumentType::String, "", {}, "Name of option");

    EXPECT_TRUE(parser.parseArgs("-n \"Earl Long John\""));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl Long John");

    EXPECT_TRUE(parser.parseArgs("-n 'Earl Long John'"));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl Long John");

    EXPECT_TRUE(parser.parseArgs("-n 'Earl '"));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl ");
}

TEST(ArgumentParserTest, OneArgMixedQuotesTest)
{
    ArgumentParser parser;
    ArgumentParser::string_type name;
    parser.addStoreArgument({"-n", "--name"}, 1, "", ArgumentType::String, "", {}, "Name of option");

    EXPECT_TRUE(parser.parseArgs("-n \"Earl 'Long' John\""));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl 'Long' John");

    EXPECT_TRUE(parser.parseArgs("-n 'Earl \"Long John\"'"));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl \"Long John\"");

    EXPECT_TRUE(parser.parseArgs("-n 'Earl \"Long John'"));
    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_type>("name", name));
    EXPECT_EQ(name, "Earl \"Long John");
}

TEST(ArgumentParserTest, WorkingTest)
{
    ArgumentParser parser;
    Argument foo, bar, bat, bit;
    foo.setNames({"-n", "--nooby"});
    foo.setHelp("A bunch of new cheese");
    foo.setRequired(true);
    foo.setArgumentType(ArgumentType::Int);
    foo.setNumberOfArgs(1);
    bar.setNames({"acid"});
    bar.setLongName("rubber ducky");
    bar.setHelp("hydro ceto-celi-cyclic acid value");
    bat.setNames({"-c", "--crab"});
    bat.setHelp("two yummy in my tummy types of crab");
    bat.setNumberOfArgs(2);
    bat.setChoices({"king", "dungeness"});
    bat.setArgumentType(ArgumentType::String);
    bit.setNames({"-z", "--zebra"});
    bit.setNumberOfArgs(1);
    bit.setRequired(true);
    parser.addStoreArgument({"-f", "--foo"}, 1, "", ArgumentType::String, "", {}, "A funny arg with a funny name");
    parser.addArgument(foo);
    parser.addArgument(bar);
    parser.addArgument(bat);
    parser.addArgument(bit);
    parser.setName("bigglesworth");
    parser.setDescription("Here is some big amount of text for us to put in as the description");
    parser.setEpilog("A string of text to write out after the argument options.");
    parser.setExitOnHelp(true);
    parser.parseArgs("-c king dungeness foo -y -n 32");
    int i = 0;
    parser.getValueForArgument<int>("nooby", i);
    EXPECT_EQ(i, 32);
    ArgumentParser::string_list_type stringList;
    parser.getValueForArgument<ArgumentParser::string_list_type>("crab", stringList);
    ASSERT_EQ(stringList.size(), 2);
    EXPECT_EQ(stringList[0], "king");
    EXPECT_EQ(stringList[1], "dungeness");
}

TEST(ArgumentParserTest, StoreActionOneArgTest)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    parser.addArgument({"-i", "--int"}, ArgumentAction::Store, 1, "", "", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Store, 1, "", "", ArgumentType::Real, "", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Store, 1, "", "", ArgumentType::String, "", {}, false,
                       "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Store, 1, "", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");
    parser.parseArgs("-i 10 -d 3.14 -s foo -b false");

    EXPECT_TRUE(parser.getValueForArgument<int>("int", i));
    EXPECT_EQ(i, 10);

    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 3.14);

    EXPECT_TRUE(parser.getValueForArgument<String>("string", s));
    EXPECT_EQ(s, "foo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, StoreActionMultiArgTest)
{
    ArgumentParser parser;
    ArgumentParser::int_list_type intList;
    ArgumentParser::real_list_type realList;
    ArgumentParser::string_list_type stringList;
    ArgumentParser::bool_list_type boolList;

    parser.addArgument({"-i", "--int"}, ArgumentAction::Store, 2, "", "", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Store, 2, "", "", ArgumentType::Real, "", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Store, 2, "", "", ArgumentType::String, "", {}, false,
                       "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Store, 2, "", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-i 10 25 -d 3.14 3.16 -s foo bar -b false true");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("int", intList));
    EXPECT_EQ(intList[0], 10);
    EXPECT_EQ(intList[1], 25);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("double", realList));
    EXPECT_EQ(realList[0], 3.14);
    EXPECT_EQ(realList[1], 3.16);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("string", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "bar");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("bool", boolList));
    EXPECT_EQ(boolList[0], false);
    EXPECT_EQ(boolList[1], true);
}

TEST(ArgumentParserTest, StoreActionDefaultTest)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    parser.addArgument({"-i", "--int"}, ArgumentAction::Store, 1, "100", "", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Store, 1, "6.55", "", ArgumentType::Real, "", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Store, 1, "zoo", "", ArgumentType::String, "", {}, false,
                       "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Store, 1, "true", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-z 3 -v -v -Verbose");

    EXPECT_TRUE(parser.getValueForArgument<int>("int", i));
    EXPECT_EQ(i, 100);

    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 6.55);

    EXPECT_TRUE(parser.getValueForArgument<String>("string", s));
    EXPECT_EQ(s, "zoo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, true);
}

TEST(ArgumentParserTest, StoreActionChoicesTest)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    // When using choices, a should a default value match the choices, and constant should be empty.
    parser.addArgument({"-i", "--int"}, ArgumentAction::Store, 1, "", "", ArgumentType::Int, "", {"10", "11", "12"},
                       false, "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Store, 1, "", "", ArgumentType::Real, "",
                       {"4.556", "9292.233", "238832.1"}, false, "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Store, 1, "", "", ArgumentType::String, "",
                       {"foo", "bar", "bat"}, false, "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Store, 1, "", "", ArgumentType::Bool, "", {"true"}, false,
                       "Bool help text");

    EXPECT_FALSE(parser.parseArgs("-i 23"));
    EXPECT_TRUE(parser.parseArgs("--int 12"));
    EXPECT_TRUE(parser.getValueForArgument<int>("int", i));
    EXPECT_EQ(i, 12);

    EXPECT_FALSE(parser.parseArgs("-d 3.14159"));
    EXPECT_TRUE(parser.parseArgs("--double 238832.1"));
    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 238832.1);

    EXPECT_FALSE(parser.parseArgs("-s bazinga"));
    EXPECT_TRUE(parser.parseArgs("--string foo"));
    EXPECT_TRUE(parser.getValueForArgument<String>("string", s));
    EXPECT_EQ(s, "foo");

    EXPECT_FALSE(parser.parseArgs("-b false"));
    EXPECT_TRUE(parser.parseArgs("--bool true"));
    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, true);
}

TEST(ArgumentParserTest, StoreActionDestinationTest)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    parser.addArgument({"-i", "--int"}, ArgumentAction::Store, 1, "", "", ArgumentType::Int, "integer", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Store, 1, "", "", ArgumentType::Real, "real", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Store, 1, "", "", ArgumentType::String, "str", {}, false,
                       "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Store, 1, "", "", ArgumentType::Bool, "boolean", {}, false,
                       "Bool help text");

    parser.parseArgs("-i 10 -d 3.145 -s foo -b false");

    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_EQ(i, 10);

    EXPECT_TRUE(parser.getValueForArgument<double>("real", d));
    EXPECT_EQ(d, 3.145);

    EXPECT_TRUE(parser.getValueForArgument<String>("str", s));
    EXPECT_EQ(s, "foo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("boolean", b));
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, StoreConstTest)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    // Hmm, StoreConst implies nargs should be zero, how do I enforce that requirement?
    parser.addArgument({"-i", "--int"}, ArgumentAction::StoreConst, 0, "", "10", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::StoreConst, 0, "", "3.14", ArgumentType::Real, "", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::StoreConst, 0, "", "foo", ArgumentType::String, "", {},
                       false, "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::StoreConst, 0, "", "false", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-i -d -s -b");

    EXPECT_TRUE(parser.getValueForArgument<int>("int", i));
    EXPECT_EQ(i, 10);

    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 3.14);

    EXPECT_TRUE(parser.getValueForArgument<String>("string", s));
    EXPECT_EQ(s, "foo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, StoreTrueTest)
{
    ArgumentParser parser;
    bool b;

    // StoreTrue implies that default is empty, constant is blank, choices is empty and nArgs is 0.
    parser.addArgument({"-b", "--bool"}, ArgumentAction::StoreTrue, 0, "", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-b");

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, true);
}

TEST(ArgumentParserTest, StoreFalseTest)
{
    ArgumentParser parser;
    bool b;

    // StoreTrue implies that constant is blank, choices is empty and nArgs is 0, and ArgumentType is Bool.
    parser.addArgument({"-b", "--bool"}, ArgumentAction::StoreFalse, 0, "", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-b");

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, AppendTest)
{
    ArgumentParser parser;
    ArgumentParser::int_list_type intList;
    ArgumentParser::real_list_type realList;
    ArgumentParser::string_list_type stringList;
    ArgumentParser::bool_list_type boolList;

    parser.addArgument({"-i", "--int"}, ArgumentAction::Append, 1, "", "", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Append, 1, "", "", ArgumentType::Real, "", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Append, 1, "", "", ArgumentType::String, "", {}, false,
                       "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Append, 1, "", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-i 10 -i 11 -d 3.14 -d 3.16 -s foo -s bar -b true -b false");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("int", intList));
    EXPECT_EQ(intList[0], 10);
    EXPECT_EQ(intList[1], 11);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("double", realList));
    EXPECT_EQ(realList[0], 3.14);
    EXPECT_EQ(realList[1], 3.16);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("string", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "bar");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("bool", boolList));
    EXPECT_EQ(boolList[0], true);
    EXPECT_EQ(boolList[1], false);
}

TEST(ArgumentParserTest, AppendConstTest)
{
    ArgumentParser parser;
    ArgumentParser::int_list_type intList;
    ArgumentParser::real_list_type realList;
    ArgumentParser::string_list_type stringList;
    ArgumentParser::bool_list_type boolList;

    // AppendConst implies that nargs is 0, default and choices are not relevant.
    parser.addArgument({"-i", "--int"}, ArgumentAction::AppendConst, 0, "", "55", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::AppendConst, 0, "", "2.21", ArgumentType::Real, "", {},
                       false, "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::AppendConst, 0, "", "foo", ArgumentType::String, "", {},
                       false, "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::AppendConst, 0, "", "false", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-i -d -s -b -i -d -s -b -i -d -s -b");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("int", intList));
    EXPECT_EQ(intList[0], 55);
    EXPECT_EQ(intList[1], 55);
    EXPECT_EQ(intList[2], 55);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("double", realList));
    EXPECT_EQ(realList[0], 2.21);
    EXPECT_EQ(realList[1], 2.21);
    EXPECT_EQ(realList[2], 2.21);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("string", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "foo");
    EXPECT_EQ(stringList[2], "foo");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("bool", boolList));
    EXPECT_EQ(boolList[0], false);
    EXPECT_EQ(boolList[1], false);
    EXPECT_EQ(boolList[2], false);
}

TEST(ArgumentParserTest, CountTest)
{
    ArgumentParser parser;
    ArgumentParser::size_type count;

    // Count implies nArgs is 0, and that ArgumentType doesn't matter, default/constant/choices are all not considered.
    parser.addArgument({"-i", "--int"}, ArgumentAction::Count, 0, "", "", ArgumentType::Int, "", {}, false,
                       "Int help text");
    parser.addArgument({"-d", "--double"}, ArgumentAction::Count, 0, "", "", ArgumentType::Real, "", {}, false,
                       "Double help text");
    parser.addArgument({"-s", "--string"}, ArgumentAction::Count, 0, "", "", ArgumentType::String, "", {}, false,
                       "String help text");
    parser.addArgument({"-b", "--bool"}, ArgumentAction::Count, 0, "", "", ArgumentType::Bool, "", {}, false,
                       "Bool help text");

    parser.parseArgs("-i -i -i -d -d -s -s -s -s -b -b -b -b -b");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("int", count));
    EXPECT_EQ(count, 3);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("double", count));
    EXPECT_EQ(count, 2);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("string", count));
    EXPECT_EQ(count, 4);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("bool", count));
    EXPECT_EQ(count, 5);
}

TEST(ArgumentParserTest, AddStoreArgumentTest)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    // Test a simple one argument version with not default/destination/choices.
    parser.addStoreArgument({"-i", "--int"}, 1, "", ArgumentType::Int, "", {}, "Int test help");
    parser.addStoreArgument({"-d", "--double"}, 1, "", ArgumentType::Real, "", {}, "Double test help");
    parser.addStoreArgument({"-s", "--string"}, 1, "", ArgumentType::String, "", {}, "String test help");
    parser.addStoreArgument({"-b", "--bool"}, 1, "", ArgumentType::Bool, "", {}, "Bool help text");

    EXPECT_TRUE(parser.parseArgs("-b false -s foo -d 6.26 -i 10"));

    EXPECT_TRUE(parser.getValueForArgument<int>("int", i));
    EXPECT_EQ(i, 10);

    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 6.26);

    EXPECT_TRUE(parser.getValueForArgument<String>("string", s));
    EXPECT_EQ(s, "foo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, false);

    parser.reset();

    ArgumentParser::int_list_type intList;
    ArgumentParser::real_list_type realList;
    ArgumentParser::string_list_type stringList;
    ArgumentParser::bool_list_type boolList;

    // Try a multi-argument no default/destination/choices
    parser.addStoreArgument({"-i", "--int"}, 3, "", ArgumentType::Int, "", {}, "Int test help");
    parser.addStoreArgument({"-d", "--double"}, 2, "", ArgumentType::Real, "", {}, "Double test help");
    parser.addStoreArgument({"-s", "--string"}, 2, "", ArgumentType::String, "", {}, "String test help");
    parser.addStoreArgument({"-b", "--bool"}, 3, "", ArgumentType::Bool, "", {}, "Bool help text");

    EXPECT_TRUE(parser.parseArgs("-b true false true -s 'foo bar' bat -d 1.1 1.2 -i 10 11 12"));

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("int", intList));
    EXPECT_EQ(intList[0], 10);
    EXPECT_EQ(intList[1], 11);
    EXPECT_EQ(intList[2], 12);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("double", realList));
    EXPECT_EQ(realList[0], 1.1);
    EXPECT_EQ(realList[1], 1.2);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("string", stringList));
    EXPECT_EQ(stringList[0], "foo bar");
    EXPECT_EQ(stringList[1], "bat");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("bool", boolList));
    EXPECT_EQ(boolList[0], true);
    EXPECT_EQ(boolList[1], false);
    EXPECT_EQ(boolList[2], true);

    parser.reset();

    // Test the default value and the destination
    parser.addStoreArgument({"-i", "--int"}, 1, "31", ArgumentType::Int, "integer", {}, "Int help text.");
    parser.addStoreArgument({"-d", "--double"}, 1, "2343.2344", ArgumentType::Real, "real", {}, "Double help text.");
    parser.addStoreArgument({"-s", "--string"}, 1, "foo", ArgumentType::String, "str", {}, "String help text.");
    parser.addStoreArgument({"-b", "--bool"}, 1, "false", ArgumentType::Bool, "boolean", {}, "Bool help text.");

    EXPECT_TRUE(parser.parseArgs("-j 10 -k 11 -l 12"));

    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_EQ(i, 31);

    EXPECT_TRUE(parser.getValueForArgument<double>("real", d));
    EXPECT_EQ(d, 2343.2344);

    EXPECT_TRUE(parser.getValueForArgument<String>("str", s));
    EXPECT_EQ(s, "foo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("boolean", b));
    EXPECT_EQ(b, false);

    parser.reset();

    // Check choices
    parser.addStoreArgument({"-i", "--int"}, 1, "", ArgumentType::Int, "integer", {"10", "11"}, "Int help text.");
    parser.addStoreArgument({"-d", "--double"}, 1, "", ArgumentType::Real, "real", {"89.3", "122.3"},
                            "Double help text.");
    parser.addStoreArgument({"-s", "--string"}, 1, "", ArgumentType::String, "str", {"foo", "bar"},
                            "String help text.");
    parser.addStoreArgument({"-b", "--bool"}, 1, "", ArgumentType::Bool, "boolean", {"false"}, "Bool help text.");

    EXPECT_FALSE(parser.parseArgs("-i 13"));
    EXPECT_FALSE(parser.parseArgs("-d 3.14"));
    EXPECT_FALSE(parser.parseArgs("-s bat"));
    EXPECT_FALSE(parser.parseArgs("-b true"));

    EXPECT_TRUE(parser.parseArgs("-i 11 -d 122.3 -s foo -b false"));

    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_EQ(i, 11);

    EXPECT_TRUE(parser.getValueForArgument<double>("real", d));
    EXPECT_EQ(d, 122.3);

    EXPECT_TRUE(parser.getValueForArgument<String>("str", s));
    EXPECT_EQ(s, "foo");

    EXPECT_TRUE(parser.getValueForArgument<bool>("boolean", b));
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, AddStoreConstArgument)
{
    ArgumentParser parser;
    int i;
    double d;
    String s;
    bool b;

    parser.addStoreConstArgument({"-i", "--int"}, "50", ArgumentType::Int, "integer", "Int help text");
    parser.addStoreConstArgument({"-d", "--double"}, "6.02", ArgumentType::Real, "real", "Double help text");
    parser.addStoreConstArgument({"-s", "--string"}, "foo", ArgumentType::String, "str", "String help text");
    parser.addStoreConstArgument({"-b", "--bool"}, "false", ArgumentType::Bool, "boolean", "Bool help text");

    EXPECT_TRUE(parser.parseArgs("--int --double --string --bool"));

    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_TRUE(parser.getValueForArgument<double>("real", d));
    EXPECT_TRUE(parser.getValueForArgument<String>("str", s));
    EXPECT_TRUE(parser.getValueForArgument<bool>("boolean", b));

    EXPECT_EQ(i, 50);
    EXPECT_EQ(d, 6.02);
    EXPECT_EQ(s, "foo");
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, AddStoreTrueArgumentTest)
{
    ArgumentParser parser;
    bool b = false;

    parser.addStoreTrueArgument({"-b", "--bool"}, "boolean", "Bool help text");

    EXPECT_TRUE(parser.parseArgs("--bool"));
    EXPECT_TRUE(parser.getValueForArgument<bool>("boolean", b));
    EXPECT_EQ(b, true);
}

TEST(ArgumentParserTest, AddStoreFalseArgumentTest)
{
    ArgumentParser parser;
    bool b = true;

    parser.addStoreFalseArgument({"-b", "--bool"}, "boolean", "Bool help text");

    EXPECT_TRUE(parser.parseArgs("--bool"));
    EXPECT_TRUE(parser.getValueForArgument<bool>("boolean", b));
    EXPECT_EQ(b, false);
}

TEST(ArgumentParserTest, AddAppendArgumentTest)
{
    ArgumentParser parser;

    ArgumentParser::int_list_type intList;
    ArgumentParser::real_list_type realList;
    ArgumentParser::string_list_type stringList;
    ArgumentParser::bool_list_type boolList;

    parser.addAppendArgument({"-i", "--int"}, 1, ArgumentType::Int, "integer", {}, "Int help text.");
    parser.addAppendArgument({"-d", "--double"}, 1, ArgumentType::Real, "real", {}, "Double help text.");
    parser.addAppendArgument({"-s", "--string"}, 1, ArgumentType::String, "str", {}, "String help text.");
    parser.addAppendArgument({"-b", "--bool"}, 1, ArgumentType::Bool, "boolean", {}, "Bool help text.");

    EXPECT_TRUE(parser.parseArgs("-i 10 -i 11 --double 3.23 --double 2.12 --string foo -s bar --bool true -b false"));

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("integer", intList));
    EXPECT_EQ(intList[0], 10);
    EXPECT_EQ(intList[1], 11);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("real", realList));
    EXPECT_EQ(realList[0], 3.23);
    EXPECT_EQ(realList[1], 2.12);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("str", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "bar");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("boolean", boolList));
    EXPECT_EQ(boolList[0], true);
    EXPECT_EQ(boolList[1], false);

    parser.reset();

    parser.addAppendArgument({"-i", "--int"}, 1, ArgumentType::Int, "", {"11", "12"}, "Int help text");
    parser.addAppendArgument({"-d", "--double"}, 1, ArgumentType::Real, "", {"11.1", "12.2"}, "Double help text");
    parser.addAppendArgument({"-s", "--string"}, 1, ArgumentType::String, "", {"foo", "bar"}, "String help text");
    parser.addAppendArgument({"-b", "--bool"}, 1, ArgumentType::Bool, "", {"true"}, "Bool help text");

    EXPECT_FALSE(parser.parseArgs("-i 13"));
    EXPECT_FALSE(parser.parseArgs("-d 13.3"));
    EXPECT_FALSE(parser.parseArgs("-s bat"));
    EXPECT_FALSE(parser.parseArgs("--bool false"));

    EXPECT_TRUE(parser.parseArgs("--int 11 -i 12 -i 12 -d 11.1 -d 12.2 -d 11.1 -s foo -s bar -s foo -b true -b true"));

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("int", intList));
    EXPECT_EQ(intList[0], 11);
    EXPECT_EQ(intList[1], 12);
    EXPECT_EQ(intList[2], 12);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("double", realList));
    EXPECT_EQ(realList[0], 11.1);
    EXPECT_EQ(realList[1], 12.2);
    EXPECT_EQ(realList[2], 11.1);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("string", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "bar");
    EXPECT_EQ(stringList[2], "foo");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("bool", boolList));
    EXPECT_EQ(boolList[0], true);
    EXPECT_EQ(boolList[1], true);
}

TEST(ArgumentParserTest, AddAppendConstArgumentTest)
{
    ArgumentParser parser;

    ArgumentParser::int_list_type intList;
    ArgumentParser::real_list_type realList;
    ArgumentParser::string_list_type stringList;
    ArgumentParser::bool_list_type boolList;

    parser.addAppendConstArgument({"-i", "--int"}, "45", ArgumentType::Int, "", "Int help text");
    parser.addAppendConstArgument({"-d", "--double"}, "45.5", ArgumentType::Real, "", "Double help text");
    parser.addAppendConstArgument({"-s", "--string"}, "foo", ArgumentType::String, "", "String help text");
    parser.addAppendConstArgument({"-b", "--bool"}, "true", ArgumentType::Bool, "", "Bool help text");

    EXPECT_TRUE(parser.parseArgs("-i -i -d -d -d -s -s -s -s -b -b"));

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("int", intList));
    EXPECT_EQ(intList[0], 45);
    EXPECT_EQ(intList[1], 45);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("double", realList));
    EXPECT_EQ(realList[0], 45.5);
    EXPECT_EQ(realList[1], 45.5);
    EXPECT_EQ(realList[2], 45.5);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("string", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "foo");
    EXPECT_EQ(stringList[2], "foo");
    EXPECT_EQ(stringList[3], "foo");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("bool", boolList));
    EXPECT_EQ(boolList[0], true);
    EXPECT_EQ(boolList[1], true);

    parser.reset();

    parser.addAppendConstArgument({"-i", "--int"}, "45", ArgumentType::Int, "integer", "Int help text");
    parser.addAppendConstArgument({"-d", "--double"}, "45.5", ArgumentType::Real, "real", "Double help text");
    parser.addAppendConstArgument({"-s", "--string"}, "foo", ArgumentType::String, "str", "String help text");
    parser.addAppendConstArgument({"-b", "--bool"}, "true", ArgumentType::Bool, "boolean", "Bool help text");

    EXPECT_TRUE(parser.parseArgs("-i -i -d -d -d -s -s -s -s -b -b"));

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("integer", intList));
    EXPECT_EQ(intList[0], 45);
    EXPECT_EQ(intList[1], 45);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::real_list_type>("real", realList));
    EXPECT_EQ(realList[0], 45.5);
    EXPECT_EQ(realList[1], 45.5);
    EXPECT_EQ(realList[2], 45.5);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::string_list_type>("str", stringList));
    EXPECT_EQ(stringList[0], "foo");
    EXPECT_EQ(stringList[1], "foo");
    EXPECT_EQ(stringList[2], "foo");
    EXPECT_EQ(stringList[3], "foo");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::bool_list_type>("boolean", boolList));
    EXPECT_EQ(boolList[0], true);
    EXPECT_EQ(boolList[1], true);
}

TEST(ArgumentParserTest, AddCountArgumentTest)
{
    ArgumentParser parser;
    ArgumentParser::size_type count;

    parser.addCountArgument({"-i", "--int"}, "", "Int help text");
    parser.addCountArgument({"-d", "--double"}, "", "Double help text");
    parser.addCountArgument({"-s", "--string"}, "", "String help text");
    parser.addCountArgument({"-b", "--bool"}, "", "Bool help text");

    parser.parseArgs("-i -i -i -d -d -s -s -s -s -b -b -b -b -b");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("int", count));
    EXPECT_EQ(count, 3);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("double", count));
    EXPECT_EQ(count, 2);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("string", count));
    EXPECT_EQ(count, 4);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("bool", count));
    EXPECT_EQ(count, 5);

    parser.reset();

    parser.addCountArgument({"-i", "--int"}, "integer", "Int help text");
    parser.addCountArgument({"-d", "--double"}, "real", "Double help text");
    parser.addCountArgument({"-s", "--string"}, "str", "String help text");
    parser.addCountArgument({"-b", "--bool"}, "boolean", "Bool help text");

    parser.parseArgs("-i -i -i -d -d -s -s -s -s -b -b -b -b -b");

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("integer", count));
    EXPECT_EQ(count, 3);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("real", count));
    EXPECT_EQ(count, 2);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("str", count));
    EXPECT_EQ(count, 4);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::size_type>("boolean", count));
    EXPECT_EQ(count, 5);
}

TEST(ArgumentParserTest, BreakItTest)
{
    ArgumentParser parser;

    parser.addStoreArgument({"-i", "--integer"}, 1, "", ArgumentType::Int, "", {}, "Int help text");
    parser.addStoreArgument({"-d", "--double"}, 1, "", ArgumentType::Real, "", {}, "Double help text");
    parser.addStoreArgument({"-b", "--bool"}, 1, "", ArgumentType::Bool, "", {}, "Bool help text");

    EXPECT_FALSE(parser.parseArgs("-i three"));
    EXPECT_TRUE(parser.parseArgs("-i 0"));
    EXPECT_FALSE(parser.parseArgs("-i s0"));
    EXPECT_FALSE(parser.parseArgs("-i 0.0"));

    EXPECT_FALSE(parser.parseArgs("-d four"));
    EXPECT_TRUE(parser.parseArgs("-d 0.0"));
    EXPECT_TRUE(parser.parseArgs("-d 0"));
    EXPECT_FALSE(parser.parseArgs("-d 0.0.s"));
    EXPECT_FALSE(parser.parseArgs("-d 3.14s"));
    EXPECT_FALSE(parser.parseArgs("-d s3.14"));

    EXPECT_FALSE(parser.parseArgs("-b eighty"));
    EXPECT_TRUE(parser.parseArgs("-b true"));
    EXPECT_TRUE(parser.parseArgs("--bool True"));
    EXPECT_TRUE(parser.parseArgs("--bool TrUe"));
    EXPECT_TRUE(parser.parseArgs("--bool TRUE"));
    EXPECT_TRUE(parser.parseArgs("--bool 1"));
    EXPECT_TRUE(parser.parseArgs("-b False"));
    EXPECT_TRUE(parser.parseArgs("-b FaLsE"));
    EXPECT_TRUE(parser.parseArgs("-b FALSE"));
    EXPECT_TRUE(parser.parseArgs("-b 0"));

    parser.reset();

    parser.addStoreArgument({"-i", "--integer"}, 2, "", ArgumentType::Int, "", {}, "Int help text");
    parser.addStoreArgument({"-d", "--double"}, 1, "", ArgumentType::Real, "", {}, "Double help text");

    EXPECT_FALSE(parser.parseArgs("-i 3 -d 4.5"));
}

TEST(ArgumentParserTest, PositionalArgumentTest)
{
    ArgumentParser parser;
    int i, k;
    String foo;
    double d;
    bool b;
    ArgumentParser::int_list_type intList;

    parser.addPositionalArgument("foo", ArgumentType::String, "Foo help text");
    parser.addPositionalArgument("integer", ArgumentType::Int, "Int help text");
    parser.addPositionalArgument("double", ArgumentType::Real, "Real help text");
    parser.addPositionalArgument("bool", ArgumentType::Bool, "Bool help text");
    parser.addStoreArgument({"-k", "--koolaid"}, 1, "", ArgumentType::Int, "", {}, "Koolaid help text");
    parser.addStoreArgument({"-a", "--args"}, 2, "", ArgumentType::Int, "", {"10", "11"}, "Args help text");

    EXPECT_TRUE(parser.parseArgs("-k 10 bar -a 10 11 23 3.15 true"));

    EXPECT_TRUE(parser.getValueForArgument<int>("koolaid", k));
    EXPECT_EQ(k, 10);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("args", intList));
    EXPECT_EQ(intList[0], 10);
    EXPECT_EQ(intList[1], 11);

    EXPECT_TRUE(parser.getValueForArgument<String>("foo", foo));
    EXPECT_EQ(foo, "bar");

    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_EQ(i, 23);

    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 3.15);

    EXPECT_TRUE(parser.getValueForArgument<bool>("bool", b));
    EXPECT_EQ(b, true);
}

TEST(ArgumentParserTest, HowIWantItToWorkTest)
{
    ArgumentParser parser;

    parser.addStoreArgument({"-i", "--igloo"}, 1, "", ArgumentType::Int, "", {}, "Some text");

    parser.parseArgs("-i 10 -n foo -b");

    auto i = parser.getValueForArgument<int>("igloo");
    EXPECT_EQ(i, 10);
    EXPECT_THROW(parser.getValueForArgument<String>("j"), std::runtime_error);
    EXPECT_THROW(parser.getValueForArgument<bool>("b"), std::runtime_error);
}

TEST(ArgumentParserTest, SubParserTest)
{
    ArgumentParser parser;
    int i;
    double d;
    bool b;

    parser.setName("testprog");
    parser.addStoreArgument({"-i", "--integer"}, 1, "", ArgumentType::Int, "", {}, "Integer help text");
    auto & fooParser = parser.addSubparser("foo", "Foo help text");
    fooParser.addStoreArgument({"-d", "--double"}, 1, "", ArgumentType::Real, "", {}, "Double help text");
    auto & barParser = parser.addSubparser("bar", "Bar's really long help text that you might or might not read");
    barParser.addStoreTrueArgument({"-p", "--pow"}, "", "Pow help text");
    barParser.addPositionalArgument("bubba", ArgumentType::Int, "Bubba help text", true);

    EXPECT_TRUE(parser.parseArgs("-i 10 foo -d 3.13"));
    EXPECT_TRUE(parser.getValueForArgument<int>("integer", i));
    EXPECT_EQ(i, 10);
    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 3.13);

    EXPECT_TRUE(parser.parseArgs("foo -d 3.33"));
    EXPECT_TRUE(parser.getValueForArgument<double>("double", d));
    EXPECT_EQ(d, 3.33);

    EXPECT_TRUE(parser.parseArgs("bar -p 22"));
    EXPECT_TRUE(parser.getValueForArgument<bool>("pow", b));
    EXPECT_TRUE(parser.getValueForArgument<int>("bubba", i));
    EXPECT_EQ(i, 22);

    parser.parseArgs("-h");
}

TEST(ArgumentParserTest, PrefixCharacterTest)
{
    ArgumentParser parser;

    parser.setPrefixCharacter("*");

    parser.addStoreArgument({"*n", "**new"}, 1, "", ArgumentType::Int, "", {}, "New help text");

    EXPECT_TRUE(parser.parseArgs("*n 1"));

    int i;
    EXPECT_TRUE(parser.getValueForArgument<int>("new", i));
    EXPECT_EQ(i, 1);

    parser.reset();

    parser.setPrefixCharacter("/");

    parser.addStoreArgument({"/n", "/new"}, 1, "", ArgumentType::Int, "", {}, "New Help");

    EXPECT_TRUE(parser.parseArgs("/n 1"));
    EXPECT_TRUE(parser.getValueForArgument<int>("new", i));
    EXPECT_EQ(i, 1);
}

TEST(ArgumentParserTest, ArgcArgvTest)
{
    ArgumentParser parser;

    bool b;
    ArgumentParser::int_list_type i;

    parser.addStoreTrueArgument({"-t", "--true"}, "", "Help for true argument");
    parser.addAppendArgument({"-f", "--foo"}, 2, ArgumentType::Int, "", {}, "Help for foo argument");

    const char * argv[] = {"-t", "-f", "5", "6", nullptr};
    int argc = 4;

    parser.parseArgs(argc, argv);

    EXPECT_TRUE(parser.getValueForArgument<bool>("true", b));
    EXPECT_EQ(b, true);

    EXPECT_TRUE(parser.getValueForArgument<ArgumentParser::int_list_type>("foo", i));
    EXPECT_EQ(i[0], 5);
    EXPECT_EQ(i[1], 6);
}

TEST(ArgumentParserTest, HasValueForCommandTest)
{
    ArgumentParser parser;

    ArgumentParser & listParser = parser.addSubparser("list", "List help text");
    listParser.addStoreArgument({"-n", "--name"}, 1, "", ArgumentType::String, "", {}, "Name help text");

    EXPECT_TRUE(parser.parseArgs("list -n foo"));
    EXPECT_TRUE(parser.hasValueForSubcommand("list"));
    EXPECT_FALSE(parser.hasValueForSubcommand("show"));
}

TEST(ArgumentParserTest, HasValueForArgumentTest)
{
    ArgumentParser parser;

    parser.addPositionalArgument("bar", ArgumentType::Int, "Bar help text");
    parser.addStoreArgument({"-p", "--position"}, 1, "", ArgumentType::Real, "", {}, "Position help text");

    EXPECT_TRUE(parser.parseArgs("-p 3.14 34"));
    EXPECT_TRUE(parser.hasValueForArgument("bar"));
    EXPECT_TRUE(parser.hasValueForArgument("position"));
}
