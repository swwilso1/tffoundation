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

#ifndef TFARGUMENTPARSER_HPP
#define TFARGUMENTPARSER_HPP

#define NEEDS_OSTREAM
#define NEEDS_IOSTREAM
#define NEEDS_VECTOR
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfargument.hpp"
#include "tfsymboltable.hxx"

namespace TF
{

    namespace Foundation
    {
        /**
         * @page ArgumentParsing %Argument Parsing
         * @tableofcontents
         * The ArgumentParser class provides a simple interface for writing
         * user-friendly command-line interfaces.  With the ArgumentParser class you can write a parser
         * to automatically parser the command-line arguments given to a program.  The class also has
         * the ability to generate help/usage messages and inform the user of errors when the user
         * gives invalid arguments or values to the program.
         *
         * @section example Example
         * The following example program takes a list of integers and sums the values.
         * @code{.cpp}
         * #include <iostream>
         * #include "TFFoundation/TFFoundation.hpp"
         * using namespace TF::Foundation;
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     ArgumentParser::int_list_type intList;
         *     int sum = 0;
         *     parser.addStoreArgument({"-l", "--list"}, 5, "", ArgumentType::Int, "", {}, "List of integers");
         *     parser.setExitOnHelp(true);
         *     auto result = parser.parseArgs(argc, argv);
         *     if(! result)
         *     {
         *         std::cout << "Argument parsing failed" << std::endl;
         *         return -1;
         *     }
         *     intList = parser.getValueForArgument<ArgumentParser::int_list_type>("list");
         *     for(auto &value : intList)
         *     {
         *         sum += value;
         *     }
         *     std::cout << "The sum of the arguments: " << sum << std::endl;
         *     return 0;
         * }
         * @endcode
         *
         * Specifying -help on the command line will generate the following help message:
         *
         * @verbatim
           usage: [-h] [-l L1 L2 L3 L4 L5]

           Optional arguments:
           -h, --help  Show this help message and exit
           -l, --list  List of integers
           @endverbatim
         *
         * Running the program from with the command line parameters gives:
         *
         * @verbatim
           $> program -l 1 2 3 4 5
           The sum of the arguments: 15
           @endverbatim
         *
         * @subsection creation Making a parser
         * To use the command-line argument parser, first create an ArgumentParser object.
         * @code{.cpp}
         * ArgumentParser parser;
         * @endcode
         *
         * The ArgumentParser object holds all the command-line args and algorithms to process
         * the command-line.
         *
         * @subsection adding Adding arguments
         * To add argument information to the parser, use one of the addArgument methods of
         * the parser object.  These methods provide the information on how to convert the
         * command-line strings into usable C++ objects.  The parser stores the information
         * and uses it during the call to the parser's parseArgs method.
         * @code{.cpp}
         * parser.addStoreArgument({"-w","--widgets"}, 1, "", ArgumentType::Int, "", {}, "Number of widgets");
         * parser.addStoreArgument({"-b","--bugs"}, 1, "", ArgumentType::String, "", {}, "Kind of bug");
         * @endcode
         *
         * After calling parseArgs, the parser object may have two objects retrievable with the @c widgets and
         * @c bugs strings.  The @c widgets value will have an integer object and the @c bugs value will refer
         * to a String object.
         *
         * @subsection parsingargs Parsing Arguments
         * The ArgumentParser object parses the command-line values with the parseArgs method.  The method
         * matches command-line values with the information for the arguments stored in the parser object
         * and converts the supplied values to C++ typed objects.
         *
         * When used in a program, parseArgs typicall parses the command line argument passed from the @c main
         * function:
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ....
         *     parser.parseArgs(argc, argv);
         *     ...
         * }
         * @endcode
         *
         * @section configparser Configuring ArgumentParser objects
         * To configure the parser object use the following methods to set different properties
         * of the parser.
         * - [setName](@ref ArgumentParser.setName) The name of the program
         * - [setDescription](@ref ArgumentParser.setDescription) A short description of what the program does.
         * - [setEpilog](@ref ArgumentParser.setEpilog) Text to display after the help message.
         * - [setPrefixCharacter](@ref ArgumentParser.setPrefixCharacter) The character that prefixes optional
         arguments.
         * - [setExitOnHelp](@ref ArgumentParser.setExitOnHelp) The parser exits the program after processing the
         help/usage message.
         *
         * @subsection progname Program Name
         * By default the argument parser will use the value of @c argv[0] to determine the name of the program for
         * the help/usage message. This behavior has the beneficial effect of always providing the correct name for
         * the program regardless of where the program gets invoked.   However, in the event that you need to set
         * the name to a particular value, use the [setName](@ref ArgumentParser.setName) method.
         * @code{.cpp}
         * ArgumentParser parser;
         * parser.setName("myprogram");
         * @endcode
         * gives the following help message:
         * @verbatim
           usage: myprogram [-h]

           Optional arguments:
           -h, --help  Show this help message and exit
           @endverbatim
         *
         * @subsection setdescription Program Description
         * When using the ArgumentParser object, you will usually set a brief description of the program and how it
         * works.  In the help message this description goes between the command-line usage values and the help messages
         * for the different arguments.  Use [setDescription](@ref ArgumentParser.setDescription) to provide this
         * help message.  In this version, the parser does not line-wrap the description message.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *      ArgumentParser parser;
         *      parser.setDescription("The really special program that makes widgets out of blodgets");
         *      parser.parseArgs(argc, argv);
         *      return 0;
         * }
         * @endcode
         *
         * generates the following:
         *
         * @verbatim
           usage: a.out [-h]

           The really special program that makes widgets out of blodgets

           Optional arguments:
           -h, --help   Show this help message and exit
           @endverbatim
         *
         * @subsection setepilog Program Epilog
         * Some programs need to specify more information in the help message after the arguments and their
         * individual help messages. To add more help text, use the [setEpilog](@ref ArgumentParser.setEpilog)
         * method. In this version, the parser does not line wrap the epilog message.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.setDescription("The really special program that makes widgets out of blodgets");
         *     parser.setEpilog("Such is the way to derive a widget from a blodget");
         *     parser.parseArgs(argc, argv);
         *     return 0;
         * }
         * @endcode
         *
         * produces the following output:
         * @verbatim
           usage: a.out [-h]

           The really special program that makes widgets out of blodgets

           Optional arguments:
           -h, --help   Show this help message and exit

           Such is the way to derive a widget from a blodget
           @endverbatim
         *
         * @subsection setprefix Argument Prefix Character
         * Often command-line options will use the '-' character to prefix the options.  However, sometimes it
         * makes sense to use a different prefix character. For example, Windows programs may want to resemble
         * a built-in Windows application by using the '/' character for an option such as @c /foo.  Specify
         * the different prefix character by using the [setPrefixCharacter](@ref ArgumentParser.setPrefixCharacter)
         * method.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.setCharacterPrefix("/");
         *     parser.parseArgs(argc, argv);
         *     return 0;
         * }
         * @endcode
         *
         * produces the following:
         *
         * @verbatim
           usage: a.out [/h]

           Optional arguments:
           /h, /help  Show this help message and exit
           @endverbatim

           Notably the parser handles the '/' character slightly differently in that it does not expect double slash
           characters for long name values.   Eg, for the help message you would use @c /help not @c //help.

         * @subsection setexit Program Exit and Help
         * Most of the time programs will exit after printing out the program help message. To enable this functionality
         * use the [setExitOnHelp](@ref ArgumentParser.setExitOnHelp) method.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.setExitOnHelp(true);
         *     return 0;
         * }
         * @endcode
         *
         * @section addargument Adding Arguments
         * To add a command-line argument to the parser use one of the @c addArgument methods.  Each of these
         * functions take a set of arguments that allow the caller to configure details about how the parser
         * should parse the values from the command-line strings.  To describe each of these arguments, first
         * consider the [addArgument](@ref ArgumentParser.addArgument) method.
         *
         * <em>void addArgument(const string_list_type &names, argument_type::action_type action, size_type nArgs, const
         string_type &default, const string_type &constant, argument_type::argument_type type, const string_type
         &destination, const string_list_type &choices, bool required, const string_type &help)</em>
         *
         * The parameters represent the following:
         * - [names](@ref namearg) - A list of strings containing the short and long argument names with the prefix
         character,
         * eg @c -f and @c --foo.
         * - [action](@ref argaction) - The kind of behavior to follow when encountering the argument on the
         command-line.
         * - [nArgs](@ref argnarg) - The number of command-line values this argument should consume.
         * - [default](@ref argdefault) - The value of the argument if the argument does not occur on the command-line.
         * - [constant](@ref argconstant) - The constant value required by some action types.
         * - [type](@ref argtype) - The parser will convert the command-line string to the named type.
         * - [destination](@ref argdest) - The name to use to retrieve the object.
         * - [choices](@ref argchoices) - a list of strings of values that the argument can have.
         * - [required](@ref argrequired) - whether the parser requires a value for the argument.
         * - [help](@ref arghelp) - a description of what the argument does.
         *
         * @subsection namearg Names
         * The parser uses the names argument to determine if the argument is an optional argument or a positional
         * argument.  Optional arguments start with the prefix character.  Positional arguments do not use the
         * prefix character.  The names argument should have two slots, one for the short name of the argument and
         * one for the long name of the argument, eg @c -f and @c --foo.  When making the optional argument pass
         * the names as a string list <tt>{"-f", "--foo"}</tt>.  When making a positional argument, pass the argument
         * name in the second slot of the string list, <tt>{"", "bar"}</tt>.  For example, an optional argument:
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.addArgument({"-f", "--foo"}, ...);
         * }
         * @endcode
         *
         * A positional argument:
         *
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.addArgument({"", "bar"}, ...);
         * }
         * @endcode
         *
         * @subsection argaction Action
         * ArgumentParser objects match command-line arguments with argument actions.  The actions specify how the
         * parser should handle the command-line argument.
         *
         * - @c ArgumentAction::Store - This action stores the argument's value in the parser.
         * - @c ArgumentAction::StoreConst - This action stores the value specified by the constant argument.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     int i = 0;
         *     parser.addArgument({"-c", "--const"}, ArgumentAction::StoreConst, 0, "", "56", ArgumentType::Int, "", {},
         false, "Help for const");
         *     parser.parseArgs(argc, argv);
         *     i = parser.getValueForArgument<int>("const");
         *     std::cout << i << std::endl;
         *     return 0;
         * }
         * @endcode
         * Produces the following output:
         * @verbatim
           $> ./program -i
           56
           @endverbatim
         * - @c ArgumentAction::StoreTrue - This action stores a true value for the argument.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     bool b;
         *     parser.addArgument("-b","--bool"}, ArgumentAction::StoreTrue, 0, "", "", ArgumentType::Bool, "", {},
         false, "Bool help text");
         *     parser.parseArgs(argc, argv);
         *     b = parser.getValueForArgument<bool>("bool");
         *     std::cout << b << std::endl;
         *     return 0;
         * }
         * @endcode
         * Produces the following output:
         * @verbatim
           $> ./program -b
           1
           @endverbatim
         *
         * - @c ArgumentAction::StoreFalse - This action stores a false value for the argument.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     bool b;
         *     parser.addArgument("-b","--bool"}, ArgumentAction::StoreFalse, 0, "", "", ArgumentType::Bool, "", {},
         false, "Bool help text");
         *     parser.parseArgs(argc, argv);
         *     b = parser.getValueForArgument<bool>("bool");
         *     std::cout << b << std::endl;
         *     return 0;
         * }
         * @endcode
         * Produces the following output:
         * @verbatim
           $> ./program -b
           0
           @endverbatim
         * - @c ArgumentAction::Append - This action stores a list and appends each value to the list.  Use this
         * action to specify an argument multiple times.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     ArgumentParser::string_list_type stringList;
         *     parser.addArgument({"-s", "--string"}, ArgumentAction::Append, 1, "", "", ArgumentType::String, "", {},
         false, "String help text");
         *     parser.parseArgs(argc, argv);
         *     stringList = parser.getValueForArgument<ArgumentParser::string_list_type>("string");
         *     for(auto &s : stringList)
         *     {
         *         std::cout << s << std::endl;
         *     }
         *     return 0
         * }
         * @endcode
         * produces the following output
         * @verbatim
           $> ./program -s house -s car -s dog -s cat
           house
           car
           dog
           cat
           @endverbatim
         * - @c ArgumentAction::AppendConst - This action stores a list and appends the value specified by the
         * constant argument.  The parser can use this action to have multiple arguments store constant values to
         * the same list. Eg:
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     ArgumentParser::int_list_type intList;
         *     parser.addArgument({"-f","--foo"},ArgumentAction::AppendConst, 0, "", "42", ArgumentType::Int,
         "integers", {}, false, "Foo help message");
         *     parser.addArgument({"-b","--bar"},ArgumentAction::AppendConst, 0, "", "26", ArgumentType::Int,
         "integers", {}, false, "Bar help text");
         *     parser.parseArgs(argc, argv);
         *     intList = parser.getValueForArgument<ArgumentParser::int_list_type>("integers");
         *     for(auto &v : intList)
         *     {
         *         std::cout << v << std::endl;
         *     }
         *     return 0;
         * }
         * @endcode
         * Produces the following output:
         * @verbatim
           $> ./program -f -b -f
           42
           26
           42
           @endverbatim
         * - @c ArgumentAction::Count - For this action the parser counts the number of times a keyword argument
         * occurs.  Consider this example of increasing verbosity levels:
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     int verbosityLevel = 0;
         *     parser.addArgument({"-v", "--verbose"}, ArgumentAction::Count, 0, "", "", ArgumentType::Int, "", {},
         false, "Verbose help text");
         *     parser.parseArgs(argc, argv);
         *     verbosityLevel = parser.getValueForArgument<int>("verbose");
         *     std::cout << verbosityLevel << std::endl;
         *     return 0;
         * }
         * @endcode
         * Produces the following output:
         * @verbatim
           $> ./program -v -v -v
           3
           @endverbatim
         *
         * @subsection argnarg Number of Arguments
         * Usually when parsing command-line arguments, you have the parser parse one command-line value for each
         * command-line argument.  However, sometimes you need to have one command-line parser consume more than one
         * argument.  The @c nArgs (number of arguments) lets you assign a different number of command-line values with
         * an argument.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     ArgumentParser::real_list_type realList;
         *     parser.addArgument({"-s","--sum"}, ArgumentAction::Store, 2, "", "", ArgumentType::Real, "", {}, false,
         "Sum the value of two argument values");
         *     parser.parseArgs(argc, argv);
         *     realList = parser.getValueForArgument<ArgumentParser::real_list_type>("sum");
         *     std::cout << realList[0] + realList[1] << std::endl;
         *     return 0;
         * }
         * @endcode
         * Provides the following output:
         * @verbatim
           $> ./program --sum 89.0 3.14
           92.14
           @endverbatim
         *
         * @subsection argdefault Default
         * Users can choose not to provide a value for optional arguments.  Sometimes, the programmer will want to
         * provide a default value that the parser should use if the user does not provide a command line value.  The
         * @c default argument provides this facility.  While the type of the command line argument can vary between
         * an integer, real number, string, or boolean value, the programmer should always use a string to specify the
         * default value.  Consider the following:
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *      ArgumentParser parser;
         *      parser.addArgument({"-d", "--default"}, ArgumentAction::Store, 1, "42", "", ArgumentType::Int, "", {},
         false, "Default help text.");
         *      parser.parserArgs(argc, argv);
         *      auto i = parser.getValueFromArgument<int>("default");
         *      std::cout << i << std::endl;
         *      return 0;
         * }
         * @endcode
         * Produces the following output:
         * @verbatim
           $> ./program
           42
           @endverbatim
         *
         * @subsection argconstant Constant
         * The parser needs the @c constant value when handling argument actions StoreConst and AppendConst.  The
         * programmer should always pass the @c constant value as a string, encoding the value for the constant in
         * the string.  Eg:
         * - ArgumentType::Int - "482"
         * - ArgumentType::Real - "3.14159"
         * - ArgumentType::String - "foo"
         * - ArgumentType::Bool - "true"
         *
         * @subsection argtype Type
         * The parser needs the @c type value in order to convert the string command-line value to the correct type.
         * The type value correspond to C++ as follows:
         * - ArgumentType::Int - @c int
         * - ArgumentType::Real - @c double
         * - ArgumentType::String - @c String
         * - ArgumentType::Bool - @c bool
         *
         * @subsection argdest Destination
         * The parser manages command-line arguments and the values assigned to each and stores each argument/value in
         * the parser under an assigned name.   For positional arguments, that name comes from the @c names argument,
         * the second value <tt>{"", "name"}</tt>.   The parser infers the name for optional arguments from the second
         * value of the @c names argument. In some cases, the programmer may want to use a different name than the
         * names derived from the @c names argument.  Use the @c destination argument to specify that alternative
         * name.
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.addArgument({"-f", "--foo"}, ArgumentAction::Store, 1, "", "", ArgumentType::Int, "bar", {},
         false, "Foo help text");
         *     parser.parserArgs(argc, argv);
         *     auto i = parser.getValueForArgument<int>("bar");
         *     ...
         *     return 0;
         * }
         * @endcode
         *
         * @subsection argchoices Choices
         * Sometimes the programmer may find it useful to restrict the values allowed for a command-line argument.  To
         * restrict the values, pass a string list that contains the allowed values to the addArgument method.
         *
         * @subsection argrequired Required
         * The parser assumes that arguments with the prefix character indicate optional arguments.  To make these
         * arguments required, set the @c required argument to @c true.  If the parser contains a required argument
         * and the command-line does not contain that argument, the parser will generate an error about the command-line
         * not containing the error.
         *
         * @subsection arghelp Help
         * The help argument contains a string that has a brief description of the argument.  When a user uses @c -h or
         * @c --help the parser will display these help descriptions with each argument.
         *
         * @section actionargs Action Specific Arguments
         * The programmer can use the addArgument method to specify any supported command-line argument type.   However,
         * as you can see from the examples, not all the arguments to addArgument get used for each command-line
         argument
         * action type.   The parser supports specific methods for each action type that only allow arguments relevant
         * to the action type.
         * - [addStoreArgument](@ref ArgumentParser::addStoreArgument) - Only allows arguments relevant for the Store
         argument action.
         * - [addStoreConstArgument](@ref ArgumentParser::addStoreConstArgument) - Only allows arguments relevant for
         the StoreConst argument action.
         * - [addStoreTrueArgument](@ref ArgumentParser::addStoreTrueArgument) - Only allows arguments relevant for the
         StoreTue argument action.
         * - [addStoreFalseArgument](@ref ArgumentParser::addStoreFalseArgument) - Only allows arguments relevant for
         the StoreFalse argument action.
         * - [addAppendArgument](@ref ArgumentParser::addAppendArgument) - Only allows arguments relevant for the Append
         argument action.
         * - [addAppendConstArgument](@ref ArgumentParser::addAppendConstArgument) - Only allows arguments relevant for
         the AppendConst argument action.
         * - [addCountArgument](@ref ArgumentParser::addCountArgument) - Only allows arguments relevant for the Count
         argument action.
         * - [addPositionalArgument](@ref ArgumentParser::addPositionalArgument) - Only allows arguments relevant for
         positional arguments.
         *
         * @section parseargs Parsing Arguments
         * The ArgumentParser class supports two parserArgs methods; one for parsing argc/argv from a program's main
         * method and one for parsing a string representing command-line values.  Both methods return a bool value where
         * a @c true value indicates that the parser successfully parsed the command-line and a @c false value indicates
         that
         * the parser failed to parse the command-line.  Usually, when writing a full command-line parser, the
         programmer
         * will want to check the return value of the parseArgs method before checking individual for individual
         * command line values.
         * - Parsing argc/argv
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     parser.addStoreArgument(...);
         *     parser.parseArgs(argc, argv);
         *     ...
         *     return 0;
         * }
         * @endcode
         * - Parsing a string of command-line values
         * @code{.cpp}
         * int main(int argc, const char **argv)
         * {
         *      ArgumentParser parser;
         *      parser.addStoreArgument(...);
         *      parser.parseArgs("-f 10 --how 32 bob")
         *      ...
         *      return 0;
         * }
         * @endcode
         *
         * @section retvalues Retrieving Values
         * The parser supports two methods named [getValueForArgument](@ref ArgumentParser::getValueForArgument) for
         * retrieving values for a command-line argument from the parser.  Both functions take a template argument to
         * to get the value as the correct type.
         *
         * @subsection getvalueforarg1 getValueForArgument 1
         * <em>bool getValueForArgument<T>(const string_type &s, T &value);</em>
         *
         * This version of getValueForArgument checks to see if the parser contains a value of type @c T assigned
         * under the name @c s.  The method takes a reference to a @c value argument of type @c T.  The method returns
         * true if the parser contains the named value of the named type and false otherwise.  If the parser does
         contain
         * the value, then the value is stored in the @c value argument.
         *
         * @subsection getvalueforarg2 getValueForArgument 2
         * <em>T getValueForArgument<T>(const string_type &s);</em>
         *
         * This version of getValueForArgument checks to see if the parser contains a value of type @c T assigned under
         * the name @c s.  If the parser contains the named value of the named type it returns that value.  If the
         * parser does not contain the named value (or does not contain a value with the correct type) this method
         * will throw std::runtime_error.
         *
         * @section subcommands Subcommands
         * Some programs divide their functionality into some number of sub-commands.  Splitting up functionality in
         * this manner makes sense when the program performs a number of different tasks which require different kinds
         * of command-line arguments.  The ArgumentParser class supports the creation of sub-commands with the
         * [addSubparser](@ref ArgumentParser::addSubparser) method.  The programmer calls the addSubparser method with
         * the name of the command and a help message and addSubparser will return an ArgumentParser object.  You can
         * then use that ArgumentParser object to describe command-line arguments for the sub-command.
         * Consider the following example:
         * @code{.cpp}
         * #include "TFFoundation/TFFoundation.hpp"
         * using namespace TF::Foundation;
         * int main(int argc, const char **argv)
         * {
         *     ArgumentParser parser;
         *     ArgumentParser &listParser = parser.addSubparser("list", "List help text");
         *     listParser.addAppendAction({"-k", "--kids"}, 1, ArgumentType::String, "", {}, "Kids help text");
         *     ArgumentParser &showParser = parser.addSubparser("show", "Show help text");
         *     showParser.addStoreAction({"-v","--value"}, 1, "", ArgumentType::Int, "", {}, "Value help text");
         *     parser.parseArgs(argc, argv);
         *     if(parser.hasValueForSubcommand("list"))
         *     {
         *          if(parser.hasValueForArgument("kids"))
         *          {
         *               auto kidsList = parser.getValueForArgument<ArgumentParser::string_list_type>("kids");
         *               for(auto &kid : kidsList)
         *               {
         *                   std::cout << kid << std::endl;
         *               }
         *          }
         *     }
         *     else if(parser.hasValueForSubcommand("show"))
         *     {
         *          if(parser.hasValueForArgument("value"))
         *          {
         *               auto value = parser.getValueForArgument<int>("value");
         *               std::cout << value << std::endl;
         *          }
         *     }
         *     return 0;
         * }
         * @endcode
         * Produces the following outputs:
         * @verbatim
           $> ./program -h
           usage: ./program {show, list} [-h]

           Subcommand:
           show          Show help text
           list          List help text

           Optional arguments:
           -h, --help    Show this help message and exit
           @endverbatim
         *
         * @verbatim
           $> ./program list -h
           usage: ./program [-h] [-k K1]

           Optional arguments:
           -h, --help    Show this help message and exit
           -k, --kids    Kids help text
           @endverbatim

           @verbatim
           $> ./program show -h
           usage: ./program [-h] [-v V1]

           Optional arguments:
           -h, --help    Show this help message and exit
           -v, --value   Value help text
           @endverbatim
         */


        /**
         * @brief A command-line argument parser.
         *
         * This class provides an API to parse and interpret command-line arguments to a program.
         * To use the class, you instantiate an instance of ArgumentParser, add optional and positional
         * command line arguments with details of how the parser should handle each argument and then
         * parse the arguments.   Once the parser has parsed the arguments, you can use the API to retrieve
         * the command-line argument values from the parser.  The author modeled the parser off the Python
         * <a href="https://docs.python.org/3/library/argparse.html#module-argparse">argparse</a> module.
         */
        class ArgumentParser : public AllocatorInterface
        {
        public:
            /** @brief basic string type */
            using string_type = String;

            /** @brief class that stores argument details */
            using argument_type = Argument;

            /** @brief list of integers */
            using int_list_type = std::vector<int>;

            /** @brief list of real numbers (doubles) */
            using real_list_type = std::vector<double>;

            /** @brief list of strings */
            using string_list_type = std::vector<string_type>;

            /** @brief list of booleans */
            using bool_list_type = std::vector<bool>;

            /**
             * @brief basic constructor
             *
             * Initializes the parser object with a stream output object
             *
             * By default the parser will write to the stdout stream.
             * @param o the stream output object
             */
            explicit ArgumentParser(std::ostream &o = std::cout);

            /**
             * @brief basic destructor
             */
            ~ArgumentParser() override;

            /**
             * @brief method to set the parser name.
             *
             * The parser name is used when generating the usage message (for --help
             * arguments).  For example:
             * @code{.cpp}
             * ArgumentParser parser;
             * parser.setName("myprogram");
             * @endcode
             *
             * will generate a usage message with the following form:
             *
             * @verbatim
               usage: myprogram [-h H]
               @endverbatim
             *
             * @param name the name of the program.
             */
            void setName(const string_type &name);

            /**
             * @brief method to set the parser's description text.
             *
             * The description text gets displayed in the usage message after the usage line
             * and before any subcommands, positional or optional arguments.  For example:
             * @code{.cpp}
             * ArgumentParser parser;
             * parser.setName("myprogram");
             * parser.setDescription("Helpful documentation for my program's usage message.");
             * @endcode
             *
             * will generate a usage message with the following form:
             *
             * @verbatim
               usage: myprogram [-h H]

               Helpful documentation for my program's usage message.
               @endverbatim
             * @param description the text of the description message.
             */
            void setDescription(const string_type &description);

            /**
             * @brief method to set the parser's usage message epilog text.
             *
             * The epilog text gets displayed in the usage message after subcommands, positional
             * and optional arguments.  For example:
             * @code{.cpp}
             * ArgumentParser parser;
             * parser.setName("myprogram");
             * parser.setEpilog("Helpful follow up text for my program's usage message.");
             * @endcode
             *
             * will generate a usage message with the following form:
             *
             * @verbatim
               usage: myprogram [-h H]

               Optional arguments:
               -h, --help              Show this help message and exit

               Helpful follow up text for my program's usage message.
               @endverbatim
             * @param epilog the text of the epilog message.
             */
            void setEpilog(const string_type &epilog);

            /**
             * @brief method to set the parser prefix character.
             *
             * The prefix character represents the character used to
             * identify optional arguments from positional arguments.
             *
             * Eg. the '-' in --foo -h or the '/' in /foo /h
             *
             * @param character the character to use for prefix.
             */
            void setPrefixCharacter(const string_type &character);

            /**
             * @brief method to set the the behavior of the parser
             * to exit the program after writing out the usage message.
             * @param exit true if the parser should exit after writing the
             * usage message and false otherwise.
             */
            void setExitOnHelp(bool exit);

            /**
             * @brief method to add an argument object to the parser.
             * @param a the argument object.
             */
            void addArgument(const argument_type &a);

            /**
             * @brief method to add an argument to the parser by argument details
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param action the argument action (Store,StoreConst,Append,AppendConst,Count,etc...)
             * @param nArgs the number of values the argument will consume from the command line.
             * @param dfault the value of the argument if the command line does not contain this argument.
             * @param constant the constant value used when the action is StoreConst or AppendConst.
             * @param type the argument type (Int,Real,%String,Bool)
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param choices a string list representing allowed values for the command-line argument values.
             * If present the command-line argument value must match one of the choices.  The choices must also
             * match the @e type argument.  For example if @e type is Int, the choices must be integer values
             * encoded as strings: "1", "493", etc...
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             * @param help the help text for the argument's usage message.
             */
            void addArgument(const string_list_type &names, argument_type::action_type action, size_type nArgs,
                             const string_type &dfault, const string_type &constant, argument_type::argument_type type,
                             const string_type &destination, const string_list_type &choices, bool required,
                             const string_type &help);

            /**
             * @brief add a command-line argument with the Store action and only provide arguments
             * relevant to Store action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param nArgs the number of values the argument will consume from the command line.
             * @param dfault the value of the argument if the command line does not contain this argument.
             * @param type the argument type (Int,Real,%String,Bool)
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param choices a string list representing allowed values for the command-line argument values.
             * If present the command-line argument value must match one of the choices.  The choices must also
             * match the @e type argument.  For example if @e type is Int, the choices must be integer values
             * encoded as strings: "1", "493", etc...
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addStoreArgument(const string_list_type &names, size_type nArgs, const string_type &dfault,
                                  argument_type::argument_type type, const string_type &destination,
                                  const string_list_type &choices, const string_type &help, bool required = false);

            /**
             * @brief add a command-line argument with the StoreConst action and only provide arguments
             * relevant to StoreConst action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param constant the constant value to use for the argument.
             * @param type the argument type (Int,Real,%String,Bool)
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addStoreConstArgument(const string_list_type &names, const string_type &constant,
                                       argument_type::argument_type type, const string_type &destination,
                                       const string_type &help, bool required = false);

            /**
             * @brief add a command-line argument with the StoreTrue action and only provide arguments relevant
             * to a StoreTrue action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addStoreTrueArgument(const string_list_type &names, const string_type &destination,
                                      const string_type &help, bool required = false);

            /**
             * @brief add a command-line argument with the StoreFalse action and only provide arguments relevant
             * to a StoreFalse action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addStoreFalseArgument(const string_list_type &names, const string_type &destination,
                                       const string_type &help, bool required = false);

            /**
             * @brief add a command-line argument with the Append action and only provide arguments relevant
             * to an Append action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param nArgs the number of values the argument will consume from the command line.
             * @param type the argument type (Int,Real,%String,Bool)
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param choices a string list representing allowed values for the command-line argument values.
             * If present the command-line argument value must match one of the choices.  The choices must also
             * match the @e type argument.  For example if @e type is Int, the choices must be integer values
             * encoded as strings: "1", "493", etc...
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addAppendArgument(const string_list_type &names, size_type nArgs, argument_type::argument_type type,
                                   const string_type &destination, const string_list_type &choices,
                                   const string_type &help, bool required = false);

            /**
             * @brief add a command-line argument with the AppendConst action and only provide arguments
             * relevant to an AppendStore action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param constant the constant value to use for the argument.
             * @param type the argument type (Int,Real,%String,Bool)
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addAppendConstArgument(const string_list_type &names, const string_type &constant,
                                        argument_type::argument_type type, const string_type &destination,
                                        const string_type &help, bool required = false);

            /**
             * @brief add a command-line argument with the Count action and only provide arguments relevant
             * to the Count action.
             * @param names a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param destination the string to use to retrieve the argument value.  Provides an alternative
             * to the long name.
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addCountArgument(const string_list_type &names, const string_type &destination,
                                  const string_type &help, bool required = false);

            /**
             * @brief add a positional command-line argument to the parser.
             * @param name a string list of short and long names for the argument.  The first
             * element of the list is the short name and the second element of list is the long
             * name.
             * @param type the argument type (Int,Real,%String,Bool)
             * @param help the help text for the argument's usage message.
             * @param required true if the command-line must provide a value for the argument and false otherwise.
             */
            void addPositionalArgument(const string_type &name, argument_type::argument_type type,
                                       const string_type &help, bool required = false);

            /**
             * @brief method to parse the command line arguments.  This version of parseArgs operates
             * on an int and char ** array as passed to a program in the main function.
             * @param argc the number of command line values in @e argv.
             * @param argv the array of command line values.
             * @return true if the parser successfully parses the command-line arguments and false
             * otherwise.
             */
            bool parseArgs(int argc, const char **argv);

            /**
             * @brief method to parse the command line arguments.  This version of parseArgs operates
             * on a string representing the command-line arguments.
             * @param args the command-line arguments as string values separated by spaces.
             * @return true if the parser successfully parses the command-line arguments and false
             * otherwise.
             */
            bool parseArgs(const string_type &args);

            /**
             * @brief method to see if the command line parser parsed values for the subcommand named in @e command.
             * @param command the command to check
             * @return true if the parser parsed arguments for the subcommand and false otherwise.
             */
            bool hasValueForSubcommand(const string_type &command);

            /**
             * @brief method to see if the parser parsed a value for the given argument name.
             * @param arg the argument name.
             * @return true if the parser parsed a value for the subcommand and false otherwise.
             */
            bool hasValueForArgument(const string_type &arg);

            /**
             * @brief method to get a command-line argument value from the parser after the parser
             * has parsed the arguments.
             * @tparam T the type of the argument to get from the parser.
             * @param argName the name of the command-line argument to get.
             * @param value a reference to an object of type @e T that will contain the value.
             * @return true if the method successfully retrieved a value of type @e T with
             * name @e argName.
             */
            template<typename T>
            bool getValueForArgument(const string_type &argName, T &value)
            {
                if(!m_symbolTable.hasValueForKey(argName))
                    return false;

                m_symbolTable.getValueForKey<T>(argName, value);
                return true;
            }

            /**
             * @brief method to get a value for a command-line argument after the parser
             * has parsed the command-line argument.
             *
             * This method will throw a std::runtime_error if it cannot
             * retrieve a value of type @e T with the name @e arg.
             * @tparam T the type of value.
             * @param arg the name of the command-line argument.
             * @return the value of type @e T for name @e arg.
             */
            template<typename T>
            T getValueForArgument(const string_type arg)
            {
                T value;

                if(!m_symbolTable.hasValueForKey(arg))
                {
                    string_type message("No value for key ");
                    message += arg;
                    auto messageCString = message.cStr();
                    throw std::runtime_error(messageCString.get());
                }

                m_symbolTable.getValueForKey<T>(arg, value);

                return value;
            }

            /**
             * @brief method to reset the internals of the argument parser.
             *
             * After a reset, you can add new arguments to the parser and
             * parse a new set of argument values.
             */
            void reset();

            /**
             * @brief method to add a sub-parser.
             *
             * To support sub-commands that have their own command-line arguments, the addSubparser
             * method allows the caller to specify the command and help message and returns a full
             * ArgumentParser.   Use the argument parser to specify command-line arguments for the
             * sub-command.  Parsing is still done by the root ArgumentParser object.  Do not call
             * the parseArgs method on the sub ArgumentParser object.
             *
             * @code{.cpp}
             * ArgumentParser parser;
             * auto &subparser = parser.addSubparser("foo", "Help text for foo");
             * subparser.addStoreArgument(...);
             * auto result = parser.parseArgs(...);
             * @endcode
             *
             * @param command the name of the sub-command.
             * @param help the help message for the argument.
             * @return a reference to an ArgumentParser object to use for specifying command specific
             * command-line arguments.
             */
            ArgumentParser &addSubparser(const string_type &command, const string_type &help);

            /**
             * @brief method for writing an argument parser object to an ostream.
             * @param o the ostream object.
             * @return the ostream object @e o after writing the parser contents
             * to the stream.
             */
            std::ostream &description(std::ostream &o) const;

        private:
            /** @brief list of argument objects */
            using argument_list_type = std::vector<argument_type>;

            /** @brief symbol table with strings for keys */
            using symbol_table_type = SymbolTable<string_type>;

            /**
             * @brief Internal class used to track parsers for sub-commands.
             */
            struct SubParser
            {
                ArgumentParser *parser;
                string_type help;
                bool foundCommand;

                /**
                 * @brief basic constructor
                 */
                SubParser()
                {
                    parser = nullptr;
                    foundCommand = false;
                }

                /**
                 * @brief constructor for initializing object contents
                 * @param p the pointer to a new ArgumentParser object.
                 * @param h the help string for the sub-command.
                 *
                 * the SubParser object owns the memory for the ArgumentParser and
                 * will free it.
                 */
                SubParser(ArgumentParser *p, string_type h) : parser(p), help(std::move(h))
                {
                    foundCommand = false;
                }

                /**
                 * @brief destructor.
                 */
                ~SubParser()
                {
                    delete parser;
                }
            };

            /** @brief map type for mapping subcommands to the SubParser object that holds
             * the sub ArgumentParser object.
             */
            using parser_map_type = std::unordered_map<string_type, SubParser *>;

            /**
             * @brief list type for keys of the subparser map.
             */
            using parser_key_list_type = std::vector<string_type>;

            /**
             * @brief method to parse the arguments contained in a string list.
             *
             * This method underlies the other parseArgs methods.
             * @param l the list of command-line values
             * @return true if the parser correctly parsers the values and false
             * otherwise
             */
            bool parseArgs(const string_list_type &l);

            /**
             * @brief method to write the usage message to the output stream.
             */
            void outputUsageMessage() const;

            /**
             * @brief method to check to see if a value matches a short name or a
             * long name in the list of arguments.
             * @param s the value
             * @return true if the value @e s matches one of the arguments and false
             * otherwise.
             */
            bool valueMatchesArgument(const string_type &s) const;

            /**
             * @brief method to get the argument object that matches the value @e v.
             * @param v the value.
             * @return the argument that matches the value @e v.
             */
            argument_type getArgumentForValue(const string_type &v) const;

            /**
             * @brief method to write a message to the output stream that the parser
             * found an argument that does not have enough values.
             * @param arg the argument object representing the argument that did not
             * have enough values.
             */
            void notEnoughValuesForArgumentMessage(const argument_type &arg) const;

            /**
             * @brief method to test whether the value looks like an optional argument.
             *
             * Checks the value against the prefix character.
             * @param v the value object.
             * @return true if the value resembles an optional argument and false otherwise.
             */
            bool valueLooksLikeOptionalArgument(const string_type &v) const;

            /**
             * @brief method to write a message to the output stream that the parser
             * encountered a StoreConst or AppendConst argument that did not have a
             * corresponding constant value.
             * @param arg the argument object of the argument that does not have
             * a constant value.
             */
            void noConstantValueProvidedMessage(const argument_type &arg) const;

            /**
             * @brief method to write a message about a conversion error converting
             * a string to an int.
             * @param v the string that the parser could not convert to an int.
             */
            void errorConvertingStringToIntMessage(const string_type &v) const;

            /**
             * @brief method to write a message about a conversion error converting
             * a string to a double.
             * @param v the string that the parser could not convert to a double.
             */
            void errorConvertingStringToDoubleMessage(const string_type &v) const;

            /**
             * @brief method to write a message about a conversion error converting
             * a string to a bool.
             * @param v the string that the parser could not convert to a bool.
             */
            void errorConvertingStringToBoolMessage(const string_type &v) const;

            /**
             * @brief method to add a list value to the symbol table for the given
             * @e destinationName.
             * @param destinationName the name associated with the new list.
             * @param type the type of the list to create.
             */
            void addListValueForType(const string_type &destinationName, ArgumentType type);

            /**
             * @brief method to add a value of the named type @e type to the list associated
             * with @e destinationName.
             * @param destinationName the name in the symbol table associated with the list.
             * @param type the type of the value to add to the list.
             * @param value the value, in string form, to add to the list.
             * @return true if the method adds the value to the list and false otherwise.
             */
            bool addValueToListForType(const string_type &destinationName, ArgumentType type, const string_type &value);

            /**
             * @brief method to generate the keys for the subparser map.
             * @return the keys in a list object.
             */
            parser_key_list_type getKeysForSubParserMap() const;

            /**
             * @brief method that checks whether a value matches a sub-command.
             * @param value the value to match agains the sub-commands.
             * @return true if the value matches and false otherwise.
             */
            bool valueMatchesSubCommand(const string_type &value) const;

            /**
             * @brief method to add/re-add the help argument to the parser.
             */
            void addHelpArgument();

            symbol_table_type m_symbolTable;

            parser_map_type m_subParserMap;

            argument_list_type m_arguments;

            string_type m_name;

            string_type m_subCommand;

            string_type m_description;

            string_type m_epilog;

            string_type m_prefixCharacter;

            bool m_exitAfterHelp;

            std::ostream &m_outStream;
        };

        /**
         * @brief function to write an ArgumentParser to an ostream.
         * @param o the ostream object
         * @param p the parser
         * @return the ostream object @e o after writing the parser internals
         * to the stream.
         */
        std::ostream &operator<<(std::ostream &o, const ArgumentParser &p);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFARGUMENTPARSER_HPP
