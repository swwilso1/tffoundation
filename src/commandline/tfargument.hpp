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

#ifndef TFARGUMENT_HPP
#define TFARGUMENT_HPP

#define NEEDS_OSTREAM
#define NEEDS_VECTOR
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfargumentaction.hpp"
#include "tfargumenttype.hpp"
#include "tfvector.hxx"

namespace TF
{

    namespace Foundation
    {

        /**
         * @brief The class type of the objects used by @e ArgumentParser to store
         * command-line arguments.
         */
        class Argument : public AllocatorInterface
        {
        public:
            /** @brief basic string type */
            using string_type = String;

            /** @brief list of strings type */
            using string_list_type = std::vector<string_type>;

            /** @brief the action for the command-line argument */
            using action_type = ArgumentAction;

            /** @brief the type of the command-line argument */
            using argument_type = ArgumentType;

            /** @brief basic size type */
            using size_type = Size_t;

            /**
             * @brief basic constructor
             */
            Argument();

            /**
             * @brief method to set argument short name.
             * @param name the short name.
             *
             * Short names are usually a one character value
             * such as -n or -h.
             */
            void setShortName(const string_type &name);

            /**
             * @brief method to return the argument short name.
             * @return the short name
             */
            string_type getShortName(void) const;

            /**
             * @brief method to get the short name without the prefix character(s).
             * @param prefix the prefix character to strip off the string.
             * @return the short name without the prefix character.
             */
            string_type getShortNameWithoutPrefix(const string_type &prefix) const;

            /**
             * @brief method to set the long name of the argument
             * @param name the long name.
             *
             * Long names are a longer readable form of the argument
             * name.   Example: -help, -foo
             */
            void setLongName(const string_type &name);

            /**
             * @brief method to the get the long name of the argument.
             * @return the long name.
             */
            string_type getLongName(void) const;

            /**
             * @brief method to get the long name of the argument without any prefix
             * characters.
             * @param prefix the character to remove from the long name.
             * @return the long name without any prefix characters.
             */
            string_type getLongNameWithoutPrefix(const string_type &prefix) const;

            /**
             * @brief a method to set both the short name and the long name for the
             * argument simultaneously.
             * @param names a string list with two entries.  The first entry has the
             * short name of the argument and the second has the long name of the
             * argument.
             */
            void setNames(const string_list_type &names);

            /**
             * @brief method to return a string list that contains both the short
             * argument name and the long argument name.
             * @return a string list with the first element set to the short
             * argument name and second element set to the long element name
             */
            string_list_type getNames() const;

            /**
             * @brief method to set the argument action.
             * @param action the action.
             */
            void setAction(const action_type &action);

            /**
             * @brief getter method to get the argument action.
             * @return the action.
             */
            action_type getAction() const;

            /**
             * @brief setter method to set the number of values
             * consumed by the argument
             * @param n the number of values to consume.
             */
            void setNumberOfArgs(const size_type &n);

            /**
             * @brief getter method to get the number of values
             * consumed by the argument
             * @return the number of args.
             */
            size_type getNumberOfArgs() const;

            /**
             * @brief setter method to set the default value to use
             * for the argument if the argument is not present in
             * the command line values.
             *
             * The @e value should be a string form of the argument type.
             * If the argument's type is an Int then the string should contain
             * an integer value, eg: "455", or if the type is a Real then the
             * string should contain a floating point number eg: "4.442"
             * @param value the default value
             */
            void setDefault(const string_type &value);

            /**
             * @brief getter method for the argument default value.
             * @return the default value.
             */
            string_type getDefault() const;

            /**
             * @brief method to set the constant value for StoreConst and
             * AppendConst actions.
             *
             * The @e constant value should match the form of the argument type.
             * If the argument's type is an Int then the string constant should
             * contain an integer value eg: "684", or if the type is Bool then
             * the string should contain a boolean value eg: "true"
             * @param constant the value for the constant.
             */
            void setConstant(const string_type &constant);

            /**
             * @brief getter method for the constant value for argument.
             * @return the constant value.
             */
            string_type getConstant() const;

            /**
             * @brief setter method for the argument type.
             * @param type the arg type.
             */
            void setArgumentType(const argument_type &type);

            /**
             * @brief getter function for the argument type
             * @return the argument's type.
             */
            argument_type getArgumentType() const;

            /**
             * @brief setter method to set the destination name.
             *
             * An argument can have a different value for the name
             * used to retrieve the value in the argument parser. By
             * default the argument parser will use the long name of
             * the argument for argument value retrieval.   Setting a
             * destination name will cause the argument parser to use
             * the destination name for retrieving the argument value.
             * @param destination the destination string.
             */
            void setDestination(const string_type &destination);

            /**
             * @brief method to get the destination value for the argument.
             *
             * This method automatically selects the correct value for the
             * destination for the argument from the argument long name and
             * the argument destination value.  If the method picks the long
             * name, the method tries to remove the @e prefix character
             * from the name.
             * @param prefix the characters to remove from the destination.
             * @return a string containing the destination name to use
             * for argument value lookup.
             */
            string_type getDestination(const string_type &prefix) const;

            /**
             * @brief method to set the list of possible values for an argument.
             *
             * The values should represent values of the argument's type.
             * @param choices a string list of possible values that the argument
             * can accept.
             */
            void setChoices(const string_list_type &choices);

            /**
             * @brief method to get the list of possible values supported by the
             * argument.
             *
             * The values represent values of the argument's type.
             * @return a string list of the possible values.
             */
            string_list_type getChoices() const;

            /**
             * @brief method to set the boolean value that indicates if the
             * argument parser will require a value for the argument
             * @param required true if the argument must have a value and
             * false otherwise.
             */
            void setRequired(bool required);

            /**
             * @brief method to get the required property for the argument.
             * @return true if the argument parser must get a value for the
             * argument and false otherwise.
             */
            bool getRequired() const;

            /**
             * @brief method for setting the help message for the argument.
             * @param help the help message.
             */
            void setHelp(const string_type &help);

            /**
             * @brief method for getting the argument's help message.
             * @return the help message.
             */
            string_type getHelp() const;

            /**
             * @brief method that matches an argument value against the short
             * and long names to find a match.
             * @param value the value to match
             * @return true if either the short name or the long name match
             * the @e value and false otherwise.
             */
            bool valueMatchesName(const string_type &value) const;

            /**
             * @brief method that matches a value against the argument choices
             * to find a match.
             * @param value the value to match.
             * @return true if the value matches a choice and false otherwise.
             */
            bool valueMatchesChoice(const string_type &value) const;

            /**
             * @brief method to check if the argument is optional or positional.
             * @param prefix the prefix character to check against the long name and
             * short name.
             * @return true if the argument is optional and false if the argument
             * is positional.
             */
            bool argumentIsOptional(const string_type &prefix) const;

            /**
             * @brief method that returns the shorter of the long name and the
             * short name.
             * @return a string representing the shortest name.
             */
            string_type getShortestName() const;

            /**
             * @brief method to calculate the length of the string that contains
             * the short name and long name formatted for use in a usage message.
             * @return the number of characters in the usage message string.
             */
            size_type lengthOfNamesForHelp() const;

            /**
             * @brief method to return the length of the help string.
             * @return number of characters in the help message.
             */
            size_type lengthOfHelp() const;

            /**
             * @brief calculate the string that contains the short name and long name
             * formatted for use in a usage message.
             * @return the formatted short name and long name.
             */
            string_type namesForHelp() const;

            /**
             * @brief method to increase the number of consumed values tracked by
             * the argument object.
             *
             * As the argument parser checks values for an argument in "consumes" a
             * value for each of number of args supported by the argument.
             */
            void consumeValue(void);

            /**
             * @brief method to return the current number of values consumed by the
             * argument
             * @return the number of consumed values.
             */
            size_type valuesConsumed(void) const;

            /**
             * @brief method that returns a boolean that indicates whether the argument
             * can consume more values.
             * @return true if the argument can consume more arguments and false otherwise.
             */
            bool consumesMoreValues() const;

            /**
             * @brief method that compares a list of values to see if they match the argument
             * choices.
             * @param values the list of values
             * @return true if the values match the argument choices and false otherwise.
             */
            bool valuesMatchChoices(const string_list_type &values) const;

            /**
             * @brief method that compares a list of values with argument choices and returns
             * a value that does not match the choices.
             * @param values the list of values.
             * @return a value from the list of values that does not match the choices.
             */
            string_type valueThatDoesNotMatchChoice(const string_list_type &values) const;

            /**
             * @brief method that indicates if the argument has choices.
             * @return true if the argument has choices and false otherwise.
             */
            bool hasChoices() const;

            /**
             * @brief method that indicates if a positional argument has matched.
             * @return true if the argument has matched and false otherwise.
             */
            bool getMatched() const;

            /**
             * @brief set the matched property for the positional argument.
             * @param m true if the argument matched and false otherwise.
             */
            void setMatched(bool m);

            std::ostream &description(std::ostream &o) const;

        private:
            /**
             * @brief method to return the longest of the short and long names.
             * @return string containing the longest name.
             */
            string_type getLongestName() const;

            string_type m_shortName;

            string_type m_longName;

            action_type m_action;

            size_type m_nArgs;

            size_type m_valuesConsumed;

            string_type m_default;

            string_type m_constant;

            argument_type m_argumentType;

            string_type m_destination;

            string_list_type m_choices;

            bool m_required;

            bool m_matched;

            string_type m_help;
        };

        std::ostream &operator<<(std::ostream &o, const Argument &a);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFARGUMENT_HPP
