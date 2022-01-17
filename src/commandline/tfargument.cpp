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

#include "tfformatter.hpp"
#include "tfargument.hpp"

namespace TF
{

    namespace Foundation
    {
        Argument::Argument()
        {
            m_required = false;
            m_valuesConsumed = 0;
            m_nArgs = 0;
            m_action = ArgumentAction::Store;
            m_argumentType = ArgumentType::String;
            m_matched = false;
        }

        void Argument::setShortName(const string_type &name)
        {
            m_shortName = name;
        }

        Argument::string_type Argument::getShortName() const
        {
            return m_shortName;
        }

        Argument::string_type Argument::getShortNameWithoutPrefix(const string_type &prefix) const
        {
            return m_shortName.stringByReplacingOccurencesOfStringWithString(prefix, "");
        }

        void Argument::setLongName(const string_type &name)
        {
            m_longName = name;
        }

        Argument::string_type Argument::getLongName() const
        {
            return m_longName;
        }

        Argument::string_type Argument::getLongNameWithoutPrefix(const string_type &prefix) const
        {
            return m_longName.stringByReplacingOccurencesOfStringWithString(prefix, "");
        }

        void Argument::setNames(const string_list_type &names)
        {
            if(names.size() >= 1)
                m_shortName = names[0];

            if(names.size() >= 2)
                m_longName = names[1];
        }

        Argument::string_list_type Argument::getNames() const
        {
            string_list_type names;
            names.emplace_back(m_shortName);
            names.emplace_back(m_longName);
            return names;
        }

        void Argument::setAction(const action_type &action)
        {
            m_action = action;
        }

        Argument::action_type Argument::getAction() const
        {
            return m_action;
        }

        void Argument::setNumberOfArgs(const size_type &n)
        {
            m_nArgs = n;
        }

        Argument::size_type Argument::getNumberOfArgs() const
        {
            return m_nArgs;
        }

        void Argument::setDefault(const string_type &value)
        {
            m_default = value;
        }

        Argument::string_type Argument::getDefault() const
        {
            return m_default;
        }

        void Argument::setConstant(const string_type &constant)
        {
            m_constant = constant;
        }

        Argument::string_type Argument::getConstant() const
        {
            return m_constant;
        }

        void Argument::setArgumentType(const argument_type &type)
        {
            m_argumentType = type;
        }

        Argument::argument_type Argument::getArgumentType() const
        {
            return m_argumentType;
        }

        void Argument::setDestination(const string_type &destination)
        {
            m_destination = destination;
        }

        Argument::string_type Argument::getDestination(const string_type &prefix) const
        {
            if(m_destination.length() > 0)
                return m_destination;

            return m_longName.stringByReplacingOccurencesOfStringWithString(prefix, "");
        }

        void Argument::setChoices(const string_list_type &choices)
        {
            m_choices = choices;
        }

        Argument::string_list_type Argument::getChoices() const
        {
            return m_choices;
        }

        void Argument::setRequired(bool required)
        {
            m_required = required;
        }

        bool Argument::getRequired() const
        {
            return m_required;
        }

        void Argument::setHelp(const string_type &help)
        {
            m_help = help;
        }

        Argument::string_type Argument::getHelp() const
        {
            string_type helpString = m_help;
            if(m_choices.size() > 0)
            {
                helpString += " {";
                string_list_type::size_type i = 0;
                for(auto &choice : m_choices)
                {
                    helpString += choice;
                    if(i++ < (m_choices.size() - 1))
                        helpString += ", ";
                }
                helpString += "}";
            }

            if(m_default.length() > 0)
            {
                helpString += " (Default: " + m_default + ")";
            }
            return helpString;
        }

        bool Argument::valueMatchesName(const string_type &value) const
        {
            return value == m_shortName || value == m_longName;
        }

        bool Argument::valueMatchesChoice(const string_type &value) const
        {
            for(auto &choice : m_choices)
            {
                if(choice == value)
                    return true;
            }
            return false;
        }

        bool Argument::argumentIsOptional(const string_type &prefix) const
        {
            auto result = m_shortName.rangeOfString(prefix);
            if(result.position == 0 && result.length > 0)
                return true;

            result = m_longName.rangeOfString(prefix);
            if(result.position == 0 && result.length > 0)
                return true;

            return false;
        }

        Argument::string_type Argument::getShortestName() const
        {
            string_type shortest = m_shortName.length() <= m_longName.length() ? m_shortName : m_longName;
            return shortest;
        }

        Argument::string_type Argument::getLongestName() const
        {
            string_type longest = m_longName.length() >= m_shortName.length() ? m_longName : m_shortName;
            return longest;
        }

        Argument::size_type Argument::lengthOfNamesForHelp() const
        {
            return namesForHelp().length();
        }

        Argument::size_type Argument::lengthOfHelp() const
        {
            return getHelp().length();
        }

        Argument::string_type Argument::namesForHelp() const
        {
            string_type nameJoin;

            size_type shortLength = m_shortName.length();
            size_type longLength = m_longName.length();

            if(shortLength > 0)
                nameJoin = m_shortName;

            if(shortLength > 0 && longLength > 0)
                nameJoin += ", ";

            if(longLength > 0)
                nameJoin += m_longName;

            return nameJoin;
        }

        void Argument::consumeValue(void)
        {
            m_valuesConsumed++;
        }

        Argument::size_type Argument::valuesConsumed(void) const
        {
            return m_valuesConsumed;
        }

        bool Argument::consumesMoreValues() const
        {
            return m_valuesConsumed < m_nArgs;
        }

        bool Argument::valuesMatchChoices(const string_list_type &values) const
        {
            for(auto &value : values)
            {
                if(!valueMatchesChoice(value))
                    return false;
            }
            return true;
        }

        bool Argument::hasChoices() const
        {
            return m_choices.size() > 0;
        }

        bool Argument::getMatched() const
        {
            return m_matched;
        }

        void Argument::setMatched(bool m)
        {
            m_matched = m;
        }

        Argument::string_type Argument::valueThatDoesNotMatchChoice(const string_list_type &values) const
        {
            for(auto &value : values)
            {
                if(!valueMatchesChoice(value))
                    return value;
            }
            return string_type();
        }


        std::ostream &Argument::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("Argument");
                formatter->addClassMember<string_type>("m_shortName", m_shortName);
                formatter->addClassMember<string_type>("m_longName", m_longName);
                formatter->addClassMember<action_type>("m_action", m_action);
                formatter->addClassMember<size_type>("m_nArgs", m_nArgs);
                formatter->addClassMember<string_type>("m_default", m_default);
                formatter->addClassMember<string_type>("m_constant", m_constant);
                formatter->addClassMember<argument_type>("m_argumentType", m_argumentType);
                formatter->addClassMember<string_type>("m_destination", m_destination);
                formatter->addClassMember<string_list_type>("m_choices", m_choices);
                formatter->addClassMember<bool>("m_required", m_required);
                formatter->addClassMember<bool>("m_matched", m_matched);
                formatter->addClassMember<string_type>("m_help", m_help);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        std::ostream &operator<<(std::ostream &o, const Argument &a)
        {
            return a.description(o);
        }

    }    // namespace Foundation

}    // namespace TF
