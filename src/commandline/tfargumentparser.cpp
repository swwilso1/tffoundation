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
SOFTWARE.


******************************************************************************/

#include "tfformatter.hpp"
#include "tfargumentparser.hpp"

#define NEEDS_CSTDLIB
#define NEEDS_IOMANIP
#include "tfheaders.hpp"
#include "tfconversion.hpp"

namespace TF
{

    namespace Foundation
    {

        ArgumentParser::ArgumentParser(std::ostream &o) : m_outStream(o)
        {
            m_prefixCharacter = "-";
            addHelpArgument();
            m_exitAfterHelp = false;
        }

        ArgumentParser::~ArgumentParser()
        {
            for(auto &entry : m_subParserMap)
            {
                delete entry.second;
            }
        }

        void ArgumentParser::setName(const string_type &name)
        {
            m_name = name;
        }

        void ArgumentParser::setDescription(const string_type &description)
        {
            m_description = description;
        }

        void ArgumentParser::setEpilog(const string_type &epilog)
        {
            m_epilog = epilog;
        }

        void ArgumentParser::setPrefixCharacter(const string_type &character)
        {
            m_prefixCharacter = character;
            addHelpArgument();
        }

        void ArgumentParser::setExitOnHelp(bool exit)
        {
            m_exitAfterHelp = exit;
        }


        void ArgumentParser::addArgument(const argument_type &a)
        {
            m_arguments.emplace_back(a);
        }

        void ArgumentParser::addArgument(const string_list_type &names, const argument_type::action_type action,
                                         size_type nArgs, const string_type &dfault, const string_type &constant,
                                         argument_type::argument_type type, const string_type &destination,
                                         const string_list_type &choices, bool required, const string_type &help)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(action);
            arg.setNumberOfArgs(nArgs);
            arg.setDefault(dfault);
            arg.setConstant(constant);
            arg.setArgumentType(type);
            arg.setDestination(destination);
            arg.setChoices(choices);
            arg.setRequired(required);
            arg.setHelp(help);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addStoreArgument(const string_list_type &names, size_type nArgs, const string_type &dfault,
                                              argument_type::argument_type type, const string_type &destination,
                                              const string_list_type &choices, const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::Store);
            arg.setNumberOfArgs(nArgs);
            arg.setDefault(dfault);
            arg.setArgumentType(type);
            arg.setDestination(destination);
            arg.setChoices(choices);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addStoreConstArgument(const string_list_type &names, const string_type &constant,
                                                   argument_type::argument_type type, const string_type &destination,
                                                   const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::StoreConst);
            arg.setNumberOfArgs(0);
            arg.setConstant(constant);
            arg.setArgumentType(type);
            arg.setDestination(destination);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addStoreTrueArgument(const string_list_type &names, const string_type &destination,
                                                  const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::StoreTrue);
            arg.setNumberOfArgs(0);
            arg.setDestination(destination);
            arg.setArgumentType(ArgumentType::Bool);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addStoreFalseArgument(const string_list_type &names, const string_type &destination,
                                                   const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::StoreFalse);
            arg.setNumberOfArgs(0);
            arg.setDestination(destination);
            arg.setArgumentType(ArgumentType::Bool);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addAppendArgument(const string_list_type &names, size_type nArgs,
                                               argument_type::argument_type type, const string_type &destination,
                                               const string_list_type &choices, const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::Append);
            arg.setNumberOfArgs(nArgs);
            arg.setArgumentType(type);
            arg.setDestination(destination);
            arg.setChoices(choices);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addAppendConstArgument(const string_list_type &names, const string_type &constant,
                                                    argument_type::argument_type type, const string_type &destination,
                                                    const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::AppendConst);
            arg.setConstant(constant);
            arg.setNumberOfArgs(0);
            arg.setArgumentType(type);
            arg.setDestination(destination);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }

        void ArgumentParser::addCountArgument(const string_list_type &names, const string_type &destination,
                                              const string_type &help, bool required)
        {
            argument_type arg;
            arg.setNames(names);
            arg.setAction(ArgumentAction::Count);
            arg.setDestination(destination);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }


        void ArgumentParser::addPositionalArgument(const string_type &name, argument_type::argument_type type,
                                                   const string_type &help, bool required)
        {
            argument_type arg;
            arg.setLongName(name);
            arg.setArgumentType(type);
            arg.setHelp(help);
            arg.setRequired(required);
            m_arguments.emplace_back(arg);
        }


        bool ArgumentParser::parseArgs(int argc, const char **argv)
        {
            string_list_type argList;
            for(int i = 0; i < argc; i++)
            {
                argList.emplace_back(argv[i]);
            }

            return parseArgs(argList);
        }

        bool ArgumentParser::parseArgs(const string_type &args)
        {
            string_type::unicode_point_type c;
            string_type::unicode_point_type quoteChar = '"';
            bool inQuotes = false;
            string_type::size_type length = args.length();
            string_type arg;
            string_list_type argList;

            for(string_type::size_type i = 0; i < length; i++)
            {
                c = args.characterAtIndex(i);
                if(c == '"' || c == '\'')
                {
                    if(!inQuotes)
                    {
                        quoteChar = c;
                        inQuotes = true;
                        continue;
                    }
                    else
                    {
                        if(c == quoteChar)
                        {
                            argList.push_back(arg);
                            arg = "";
                            inQuotes = false;
                            continue;
                        }
                    }
                }
                else if(c == ' ' && !inQuotes)
                {
                    if(arg.length() > 0)
                        argList.push_back(arg);
                    arg = "";
                    continue;
                }

                String s(c);
                arg = arg + s;
            }

            if(arg.length() > 0)
                argList.push_back(arg);

            return parseArgs(argList);
        }

        bool ArgumentParser::parseArgs(const string_list_type &l)
        {
            m_symbolTable.clear();

            if(m_name.length() == 0 && l.size() > 0)
            {
                m_name = l[0];
            }

            // First run through the arguments and see if the arguments have
            // h or help.
            for(auto iter = l.begin(); iter != l.end(); iter++)
            {
                auto value = *iter;
                if(valueMatchesArgument(value))
                {
                    auto arg = getArgumentForValue(value);

                    if(arg.getLongNameWithoutPrefix(m_prefixCharacter) == "help")
                    {
                        outputUsageMessage();
                        if(m_exitAfterHelp)
                            exit(0);
                        return true;
                    }
                }
                if(valueMatchesSubCommand(value))
                {
                    auto orig_iter = iter;
                    auto subParser = m_subParserMap[value];

                    for(; iter != l.end(); iter++)
                    {
                        value = *iter;

                        if(subParser->parser->valueMatchesArgument(value))
                        {
                            auto subArg = subParser->parser->getArgumentForValue(value);

                            if(subArg.getLongNameWithoutPrefix(m_prefixCharacter) == "help")
                            {
                                subParser->parser->outputUsageMessage();
                                if(m_exitAfterHelp)
                                    exit(0);
                                return true;
                            }
                        }
                    }

                    iter = orig_iter;
                }
            }

            string_list_type::const_iterator iter;

            // Now process the arguments.
            for(iter = l.cbegin(); iter != l.cend(); iter++)
            {
                auto &value = *iter;

                if(valueMatchesArgument(value))
                {
                    auto arg = getArgumentForValue(value);

                    if(arg.consumesMoreValues())
                    {
                        string_list_type moreValues;

                        iter++;
                        while(arg.consumesMoreValues() && iter != l.cend())
                        {
                            // If the value matches another argument or event looks like an argument
                            // then write an error message and return.
                            if(valueMatchesArgument(*iter) || valueLooksLikeOptionalArgument(*iter))
                            {
                                notEnoughValuesForArgumentMessage(arg);
                                return false;
                            }

                            moreValues.emplace_back(*iter);
                            arg.consumeValue();
                            iter++;
                        }

                        // If we did not get all the values for the argument write an error message and return.
                        if(arg.consumesMoreValues())
                        {
                            notEnoughValuesForArgumentMessage(arg);
                            return false;
                        }

                        // We have the necessary number of values for the argument.

                        // Back up iter so that when we cycle it correctly points to the right
                        // value.
                        iter--;

                        // Check to see if the arg has existing values for choices.   If so we need
                        // to know if the values passed to us match those choices.
                        if(arg.hasChoices() && !arg.valuesMatchChoices(moreValues))
                        {
                            m_outStream << "Value " << arg.valueThatDoesNotMatchChoice(moreValues);
                            m_outStream << " for argument " << arg.getShortName() << "/";
                            m_outStream << arg.getLongName() << " does not match choice list" << std::endl;
                            return false;
                        }

                        auto destinationName = arg.getDestination(m_prefixCharacter);
                        auto type = arg.getArgumentType();
                        auto action = arg.getAction();

                        if(action == ArgumentAction::Store)
                        {
                            if(m_symbolTable.hasValueForKey(destinationName))
                                m_symbolTable.removeValueForKey(destinationName);

                            if(type == ArgumentType::Int)
                            {
                                if(moreValues.size() > 1)
                                {
                                    int_list_type intList;
                                    for(auto &mValue : moreValues)
                                    {
                                        Result<int> res = Conversion::convertStringToInt(mValue);
                                        if(!res.succeeded)
                                        {
                                            errorConvertingStringToIntMessage(mValue);
                                            return false;
                                        }
                                        intList.emplace_back(res.value);
                                    }

                                    m_symbolTable.setValueForKey<int_list_type>(destinationName, intList);
                                }
                                else
                                {
                                    Result<int> res = Conversion::convertStringToInt(moreValues[0]);
                                    if(!res.succeeded)
                                    {
                                        errorConvertingStringToIntMessage(moreValues[0]);
                                        return false;
                                    }
                                    m_symbolTable.setValueForKey<int>(destinationName, res.value);
                                }
                            }
                            else if(type == ArgumentType::Real)
                            {
                                if(moreValues.size() > 1)
                                {
                                    real_list_type realList;
                                    for(auto &mValue : moreValues)
                                    {
                                        Result<double> res = Conversion::convertStringToDouble(mValue);
                                        if(!res.succeeded)
                                        {
                                            errorConvertingStringToDoubleMessage(mValue);
                                            return false;
                                        }
                                        realList.emplace_back(res.value);
                                    }

                                    m_symbolTable.setValueForKey<real_list_type>(destinationName, realList);
                                }
                                else
                                {
                                    Result<double> res = Conversion::convertStringToDouble(moreValues[0]);
                                    if(!res.succeeded)
                                    {
                                        errorConvertingStringToDoubleMessage(moreValues[0]);
                                        return false;
                                    }
                                    m_symbolTable.setValueForKey<double>(destinationName, res.value);
                                }
                            }
                            else if(type == ArgumentType::String)
                            {
                                if(moreValues.size() > 1)
                                {
                                    m_symbolTable.setValueForKey<string_list_type>(destinationName, moreValues);
                                }
                                else
                                {
                                    m_symbolTable.setValueForKey<string_type>(destinationName, moreValues[0]);
                                }
                            }
                            else if(type == ArgumentType::Bool)
                            {
                                if(moreValues.size() > 1)
                                {
                                    bool_list_type boolList;
                                    for(auto &mValue : moreValues)
                                    {
                                        Result<bool> res = Conversion::convertStringToBool(mValue);
                                        if(!res.succeeded)
                                        {
                                            errorConvertingStringToBoolMessage(mValue);
                                            return false;
                                        }
                                        boolList.emplace_back(res.value);
                                    }

                                    m_symbolTable.setValueForKey<bool_list_type>(destinationName, boolList);
                                }
                                else
                                {
                                    Result<bool> res = Conversion::convertStringToBool(moreValues[0]);
                                    if(!res.succeeded)
                                    {
                                        errorConvertingStringToBoolMessage(moreValues[0]);
                                        return false;
                                    }
                                    m_symbolTable.setValueForKey<bool>(destinationName, res.value);
                                }
                            }
                        }
                        else if(action == ArgumentAction::Append)
                        {
                            addListValueForType(destinationName, type);

                            for(auto &mValue : moreValues)
                            {
                                bool result = addValueToListForType(destinationName, type, mValue);
                                if(!result)
                                    return false;
                            }
                        }
                    }
                    else
                    {
                        // The argument does not consume more values.   This means that it functions as a switch
                        // with an action type of StoreConst, StoreTrue, StoreFalse, AppendConst, or Count

                        auto action = arg.getAction();
                        auto destinationName = arg.getDestination(m_prefixCharacter);
                        auto type = arg.getArgumentType();

                        if(action == ArgumentAction::StoreConst)
                        {
                            // The argument should have a const value, check for that first.   If it does not
                            // exist then output an error.
                            auto constant = arg.getConstant();
                            if(constant.length() == 0)
                            {
                                noConstantValueProvidedMessage(arg);
                                return false;
                            }

                            if(m_symbolTable.hasValueForKey(destinationName))
                                m_symbolTable.removeValueForKey(destinationName);

                            if(type == ArgumentType::Int)
                            {
                                Result<int> res = Conversion::convertStringToInt(constant);
                                if(!res.succeeded)
                                {
                                    errorConvertingStringToIntMessage(constant);
                                    return false;
                                }
                                m_symbolTable.setValueForKey<int>(destinationName, res.value);
                            }
                            else if(type == ArgumentType::Real)
                            {
                                Result<double> res = Conversion::convertStringToDouble(constant);
                                if(!res.succeeded)
                                {
                                    errorConvertingStringToDoubleMessage(constant);
                                    return false;
                                }
                                m_symbolTable.setValueForKey<double>(destinationName, res.value);
                            }
                            else if(type == ArgumentType::String)
                            {
                                m_symbolTable.setValueForKey<string_type>(destinationName, constant);
                            }
                            else if(type == ArgumentType::Bool)
                            {
                                Result<bool> res = Conversion::convertStringToBool(constant);
                                if(!res.succeeded)
                                {
                                    errorConvertingStringToBoolMessage(constant);
                                    return false;
                                }
                                m_symbolTable.setValueForKey<bool>(destinationName, res.value);
                            }
                        }
                        else if(action == ArgumentAction::StoreTrue)
                        {
                            if(m_symbolTable.hasValueForKey(destinationName))
                                m_symbolTable.removeValueForKey(destinationName);

                            m_symbolTable.setValueForKey<bool>(destinationName, true);
                        }
                        else if(action == ArgumentAction::StoreFalse)
                        {
                            if(m_symbolTable.hasValueForKey(destinationName))
                                m_symbolTable.removeValueForKey(destinationName);

                            m_symbolTable.setValueForKey<bool>(destinationName, false);
                        }
                        else if(action == ArgumentAction::AppendConst)
                        {
                            auto constant = arg.getConstant();
                            if(constant.length() == 0)
                            {
                                noConstantValueProvidedMessage(arg);
                                return false;
                            }

                            addListValueForType(destinationName, type);
                            bool res = addValueToListForType(destinationName, type, constant);
                            if(!res)
                                return false;
                        }
                        else if(action == ArgumentAction::Count)
                        {
                            if(!m_symbolTable.hasValueForKey(destinationName))
                            {
                                size_type i = 0;
                                m_symbolTable.setValueForKey<size_type>(destinationName, i);
                            }

                            size_type i = 0;
                            m_symbolTable.getValueForKey<size_type>(destinationName, i);
                            i++;
                            m_symbolTable.removeValueForKey(destinationName);
                            m_symbolTable.setValueForKey<size_type>(destinationName, i);
                        }
                    }
                }
                else
                {
                    if(valueMatchesSubCommand(value))
                    {
                        string_list_type remainingArgs;

                        iter++;
                        for(; iter != l.end(); iter++)
                        {
                            remainingArgs.emplace_back(*iter);
                        }

                        SubParser *subParser = m_subParserMap[value];
                        if(subParser != nullptr && subParser->parser != nullptr)
                        {
                            auto result = subParser->parser->parseArgs(remainingArgs);
                            if(result)
                            {
                                m_symbolTable.update(subParser->parser->m_symbolTable);
                            }

                            subParser->foundCommand = true;
                            return result;
                        }
                        else
                        {
                            m_outStream << "Unable to invoke sub-parser for command " << value << std::endl;
                            return false;
                        }
                    }

                    for(auto &arg : m_arguments)
                    {
                        if(!arg.argumentIsOptional(m_prefixCharacter) && !arg.getMatched())
                        {
                            auto destination = arg.getDestination(m_prefixCharacter);
                            auto type = arg.getArgumentType();

                            if(m_symbolTable.hasValueForKey(destination))
                                m_symbolTable.removeValueForKey(destination);

                            if(type == ArgumentType::String)
                            {
                                m_symbolTable.setValueForKey<string_type>(destination, value);
                            }
                            else if(type == ArgumentType::Int)
                            {
                                Result<int> result = Conversion::convertStringToInt(value);
                                if(!result.succeeded)
                                {
                                    errorConvertingStringToIntMessage(value);
                                    return false;
                                }
                                m_symbolTable.setValueForKey<int>(destination, result.value);
                            }
                            else if(type == ArgumentType::Real)
                            {
                                Result<double> result = Conversion::convertStringToDouble(value);
                                if(!result.succeeded)
                                {
                                    errorConvertingStringToDoubleMessage(value);
                                    return false;
                                }
                                m_symbolTable.setValueForKey<double>(destination, result.value);
                            }
                            else if(type == ArgumentType::Bool)
                            {
                                Result<bool> result = Conversion::convertStringToBool(value);
                                if(!result.succeeded)
                                {
                                    errorConvertingStringToBoolMessage(value);
                                    return false;
                                }
                                m_symbolTable.setValueForKey<bool>(destination, result.value);
                            }

                            arg.setMatched(true);
                            break;
                        }
                    }
                }
            }

            // Search through the argument list for default arguments.   If an argument does not have a value and
            // the action is Store, and the arg has a default, then we must add a value to the symbol table.
            for(auto &arg : m_arguments)
            {
                if(arg.getAction() == ArgumentAction::Store)
                {
                    auto destinationName = arg.getDestination(m_prefixCharacter);
                    if(!m_symbolTable.hasValueForKey(destinationName) && arg.getDefault().length() > 0)
                    {
                        auto argType = arg.getArgumentType();
                        auto theDefault = arg.getDefault();
                        if(argType == ArgumentType::Int)
                        {
                            Result<int> res = Conversion::convertStringToInt(theDefault);
                            if(!res.succeeded)
                            {
                                errorConvertingStringToIntMessage(theDefault);
                                return false;
                            }
                            m_symbolTable.setValueForKey<int>(destinationName, res.value);
                        }
                        else if(argType == ArgumentType::Real)
                        {
                            Result<double> res = Conversion::convertStringToDouble(theDefault);
                            if(!res.succeeded)
                            {
                                errorConvertingStringToDoubleMessage(theDefault);
                                return false;
                            }
                            m_symbolTable.setValueForKey<double>(destinationName, res.value);
                        }
                        else if(argType == ArgumentType::String)
                        {
                            m_symbolTable.setValueForKey<string_type>(destinationName, theDefault);
                        }
                        else if(argType == ArgumentType::Bool)
                        {
                            Result<bool> res = Conversion::convertStringToBool(theDefault);
                            if(!res.succeeded)
                            {
                                errorConvertingStringToBoolMessage(theDefault);
                                return false;
                            }
                            m_symbolTable.setValueForKey<bool>(destinationName, res.value);
                        }
                    }
                }
            }

            // Finally search through the argument list for any required arguments.   If a required argument
            // exists and the symbol table does not have a value for it, then output a message and return.
            for(auto &arg : m_arguments)
            {
                if(arg.getRequired())
                {
                    auto destinationName = arg.getDestination(m_prefixCharacter);
                    if(!m_symbolTable.hasValueForKey(destinationName))
                    {
                        m_outStream << "Argument " << arg.getShortName() << "/";
                        m_outStream << arg.getLongName() << " required but not given";
                        m_outStream << std::endl;
                        return false;
                    }
                }
            }

            return true;
        }


        bool ArgumentParser::hasValueForSubcommand(const string_type &command)
        {
            if(m_subParserMap.count(command) > 0)
            {
                auto subParser = m_subParserMap[command];
                return subParser->foundCommand;
            }
            return false;
        }


        bool ArgumentParser::hasValueForArgument(const string_type &arg)
        {
            return m_symbolTable.hasValueForKey(arg);
        }

        void ArgumentParser::reset()
        {
            m_symbolTable.clear();
            for(auto &entry : m_subParserMap)
            {
                delete entry.second;
            }
            m_subParserMap.clear();
            m_arguments.clear();
            m_name = "";
            m_description = "";
            m_epilog = "";
            m_prefixCharacter = "-";
            addHelpArgument();
            m_exitAfterHelp = false;
        }


        ArgumentParser &ArgumentParser::addSubparser(const string_type &command, const string_type &help)
        {
            auto *subParser = new SubParser(new ArgumentParser(), help);
            subParser->parser->m_name = m_name;
            subParser->parser->m_subCommand = command;
            m_subParserMap[command] = subParser;
            return *subParser->parser;
        }


        void ArgumentParser::outputUsageMessage() const
        {
            size_type lengthOfNameColumn = 0;
            size_type lengthOfHelpColumn = 0;

            m_outStream << "usage: ";

            if(m_name.length() > 0)
                m_outStream << m_name << " ";

            if(m_subCommand.length() > 0)
                m_outStream << m_subCommand << " ";

            if(!m_subParserMap.empty())
            {
                size_type numberOfSubCommands = m_subParserMap.size();
                size_type i = 0;
                auto iter = m_subParserMap.begin();

                m_outStream << "{";

                for(i = 0; i < numberOfSubCommands; i++)
                {
                    m_outStream << (*iter).first;
                    if(i < (numberOfSubCommands - 1))
                    {
                        m_outStream << ",";
                    }
                    iter++;
                }

                m_outStream << "} ";
            }

            bool hasPositionalArgument = false;
            bool hasOptionalArgument = false;
            size_type i = 0;
            for(auto &arg : m_arguments)
            {
                string_type shortestName = arg.getShortestName();
                string_type upperCaseName = shortestName.uppercaseString();
                string_type argExample =
                    upperCaseName.stringByReplacingOccurencesOfStringWithString(m_prefixCharacter, "");
                if(arg.argumentIsOptional(m_prefixCharacter))
                {
                    auto numberOfArgs = arg.getNumberOfArgs();
                    if(numberOfArgs > 0)
                    {
                        m_outStream << "[" << shortestName << " ";
                        for(size_type i = 1; i <= numberOfArgs; i++)
                        {
                            m_outStream << argExample << i;
                            if(i < numberOfArgs)
                                m_outStream << " ";
                        }
                        m_outStream << "]";
                    }
                    else
                        m_outStream << "[" << shortestName << "]";
                    hasOptionalArgument = true;
                }
                else
                {
                    m_outStream << shortestName;
                    hasPositionalArgument = true;
                }

                if(i < (m_arguments.size() - 1))
                    m_outStream << " ";

                size_type length = arg.lengthOfNamesForHelp();
                if(length > lengthOfNameColumn)
                    lengthOfNameColumn = length;

                length = arg.lengthOfHelp();
                if(length > lengthOfHelpColumn)
                    lengthOfHelpColumn = length;
            }

            m_outStream << std::endl << std::endl;

            if(m_description.length() > 0)
                m_outStream << m_description << std::endl << std::endl;

            if(!m_subParserMap.empty())
            {
                m_outStream << "Subcommands:" << std::endl;
                for(auto &entry : m_subParserMap)
                {
                    m_outStream << std::left << std::setw((int)(lengthOfNameColumn + 1));
                    m_outStream << entry.first << " ";

                    m_outStream << std::left << std::setw((int)(lengthOfHelpColumn + 1));
                    m_outStream << (entry.second)->help;
                    m_outStream << std::endl;
                }

                m_outStream << std::endl;
            }
            if(hasPositionalArgument)
            {
                m_outStream << "Positional arguments:" << std::endl;

                for(auto &arg : m_arguments)
                {
                    if(!arg.argumentIsOptional(m_prefixCharacter))
                    {
                        m_outStream << std::left << std::setw((int)(lengthOfNameColumn + 1));
                        m_outStream << arg.namesForHelp() << " ";

                        m_outStream << std::left << std::setw((int)(lengthOfHelpColumn + 1));
                        m_outStream << arg.getHelp();

                        if(arg.getRequired())
                            m_outStream << "  [required]";

                        m_outStream << std::endl;
                    }
                }

                m_outStream << std::endl;
            }

            if(hasOptionalArgument)
            {
                m_outStream << "Optional arguments:" << std::endl;

                for(auto &arg : m_arguments)
                {
                    if(arg.argumentIsOptional(m_prefixCharacter))
                    {
                        m_outStream << std::left << std::setw((int)(lengthOfNameColumn + 1));
                        m_outStream << arg.namesForHelp() << " ";

                        m_outStream << std::left << std::setw((int)(lengthOfHelpColumn + 1));
                        if(arg.getHelp().length() > 0)
                            m_outStream << arg.getHelp();
                        else
                            m_outStream << " ";

                        if(arg.getRequired())
                            m_outStream << " [required]";

                        m_outStream << std::endl;
                    }
                }

                m_outStream << std::endl;
            }

            if(m_epilog.length() > 0)
            {
                m_outStream << m_epilog << std::endl;
            }

            m_outStream << std::endl;
        }

        bool ArgumentParser::valueMatchesArgument(const string_type &s) const
        {
            for(auto &arg : m_arguments)
            {
                if(arg.valueMatchesName(s))
                    return true;
            }

            return false;
        }

        ArgumentParser::argument_type ArgumentParser::getArgumentForValue(const string_type &v) const
        {
            for(auto &arg : m_arguments)
            {
                if(arg.valueMatchesName(v))
                    return arg;
            }

            return argument_type();
        }


        void ArgumentParser::notEnoughValuesForArgumentMessage(const argument_type &arg) const
        {
            m_outStream << "Not enough values for " << arg.getShortName() << "/";
            m_outStream << arg.getLongName() << " argument" << std::endl;
        }

        bool ArgumentParser::valueLooksLikeOptionalArgument(const string_type &v) const
        {
            auto res = v.rangeOfString(m_prefixCharacter);
            return res.position == 0 && res.length > 0;
        }

        void ArgumentParser::noConstantValueProvidedMessage(const argument_type &arg) const
        {
            m_outStream << "Argument " << arg.getShortName() << "/";
            m_outStream << arg.getLongName() << " configured to store a constant value,";
            m_outStream << " but no constant provided" << std::endl;
        }

        void ArgumentParser::errorConvertingStringToIntMessage(const string_type &v) const
        {
            m_outStream << "Cannot convert '" << v << "' to an integer" << std::endl;
        }

        void ArgumentParser::errorConvertingStringToDoubleMessage(const string_type &v) const
        {
            m_outStream << "Cannot convert '" << v << "' to a double" << std::endl;
        }

        void ArgumentParser::errorConvertingStringToBoolMessage(const string_type &v) const
        {
            m_outStream << "Cannot convert '" << v << "' to a bool" << std::endl;
        }

        void ArgumentParser::addListValueForType(const string_type &destinationName, ArgumentType type)
        {
            if(!m_symbolTable.hasValueForKey(destinationName))
            {
                if(type == ArgumentType::Int)
                {
                    int_list_type intList;
                    m_symbolTable.setValueForKey<int_list_type>(destinationName, intList);
                }
                else if(type == ArgumentType::Real)
                {
                    real_list_type realList;
                    m_symbolTable.setValueForKey<real_list_type>(destinationName, realList);
                }
                else if(type == ArgumentType::String)
                {
                    string_list_type stringList;
                    m_symbolTable.setValueForKey<string_list_type>(destinationName, stringList);
                }
                else if(type == ArgumentType::Bool)
                {
                    bool_list_type boolList;
                    m_symbolTable.setValueForKey<bool_list_type>(destinationName, boolList);
                }
            }
        }

        bool ArgumentParser::addValueToListForType(const string_type &destinationName, ArgumentType type,
                                                   const string_type &value)
        {
            if(type == ArgumentType::Int)
            {
                int_list_type intList;
                Result<int> res = Conversion::convertStringToInt(value);
                if(!res.succeeded)
                {
                    errorConvertingStringToIntMessage(value);
                    return false;
                }
                m_symbolTable.getValueForKey<int_list_type>(destinationName, intList);
                intList.emplace_back(res.value);
                m_symbolTable.removeValueForKey(destinationName);
                m_symbolTable.setValueForKey<int_list_type>(destinationName, intList);
            }
            else if(type == ArgumentType::Real)
            {
                real_list_type realList;
                Result<double> res = Conversion::convertStringToDouble(value);
                if(!res.succeeded)
                {
                    errorConvertingStringToDoubleMessage(value);
                    return false;
                }
                m_symbolTable.getValueForKey<real_list_type>(destinationName, realList);
                realList.emplace_back(res.value);
                m_symbolTable.removeValueForKey(destinationName);
                m_symbolTable.setValueForKey<real_list_type>(destinationName, realList);
            }
            else if(type == ArgumentType::String)
            {
                string_list_type stringList;
                m_symbolTable.getValueForKey<string_list_type>(destinationName, stringList);
                stringList.emplace_back(value);
                m_symbolTable.removeValueForKey(destinationName);
                m_symbolTable.setValueForKey<string_list_type>(destinationName, stringList);
            }
            else if(type == ArgumentType::Bool)
            {
                bool_list_type boolList;
                Result<bool> res = Conversion::convertStringToBool(value);
                if(!res.succeeded)
                {
                    errorConvertingStringToBoolMessage(value);
                    return false;
                }
                m_symbolTable.getValueForKey<bool_list_type>(destinationName, boolList);
                boolList.emplace_back(res.value);
                m_symbolTable.removeValueForKey(destinationName);
                m_symbolTable.setValueForKey<bool_list_type>(destinationName, boolList);
            }

            return true;
        }

        ArgumentParser::parser_key_list_type ArgumentParser::getKeysForSubParserMap() const
        {
            parser_key_list_type keyList;

            for(auto &entry : m_subParserMap)
            {
                keyList.emplace_back(entry.first);
            }

            return keyList;
        }


        bool ArgumentParser::valueMatchesSubCommand(const string_type &value) const
        {
            parser_key_list_type keyList = getKeysForSubParserMap();

            for(auto &key : keyList)
            {
                if(value == key)
                    return true;
            }

            return false;
        }


        void ArgumentParser::addHelpArgument()
        {
            argument_type help;

            string_type shortName;
            string_type longName;

            shortName = m_prefixCharacter + "h";
            if(m_prefixCharacter == "/")
            {
                longName = m_prefixCharacter + "help";
            }
            else
            {
                longName = m_prefixCharacter + m_prefixCharacter + "help";
            }

            help.setNames({shortName, longName});
            help.setHelp("Show this help message and exit");

            if(!m_arguments.empty())
                m_arguments[0] = help;
            else
                m_arguments.emplace_back(help);
        }


        std::ostream &ArgumentParser::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("ArgumentParser");
                formatter->addClassMember<symbol_table_type>("m_symbolTable", m_symbolTable);
                formatter->addClassMember<argument_list_type>("m_arguments", m_arguments);
                formatter->addClassMember<string_type>("m_name", m_name);
                formatter->addClassMember<string_type>("m_description", m_description);
                formatter->addClassMember<string_type>("m_epilog", m_epilog);
                formatter->addClassMember<string_type>("m_prefixCharacters", m_prefixCharacter);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        std::ostream &operator<<(std::ostream &o, const ArgumentParser &p)
        {
            return p.description(o);
        }

    }    // namespace Foundation

}    // namespace TF
