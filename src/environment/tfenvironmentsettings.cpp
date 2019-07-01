/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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

#include "tfenvironmentsettings.hpp"

#define NEEDS_CSTDLIB
#include "tfheaders.hpp"
#include "tfformatter.hpp"

extern char **environ;

namespace TF
{

    namespace Foundation
    {
        EnvironmentSettings::EnvironmentSettings()
        {
            auto tmp = environ;

            while(tmp != nullptr && *tmp != nullptr)
            {
                string_type entry = *tmp;
                auto equalsLocation = entry.rangeOfString("=");
                string_type key, value;

                key = entry.substringToIndex(equalsLocation.position);
                value = entry.substringFromIndex(equalsLocation.position + equalsLocation.length);

                variableMap.insert(std::make_pair(key, value));
                tmp++;
            }
        }


        EnvironmentSettings::~EnvironmentSettings()
        {
        }


        EnvironmentSettings::string_array_type EnvironmentSettings::environmentVariables() const
        {
            string_array_type variables;

            for(auto &mapEntry : variableMap)
            {
                variables.push_back(mapEntry.first);
            }

            return variables;
        }


        EnvironmentSettings::string_type EnvironmentSettings::getValueForVariable(const string_type &variable) const
        {
            if(variableMap.count(variable) > 0)
                return variableMap.at(variable);

            // Try using getenv to find the variable.
            auto value = std::getenv(variable.cStr().get());
            if(value != nullptr)
            {
                return string_type(value);
            }

            throw std::runtime_error("Unable to locate variable in system environment");
        }


        bool EnvironmentSettings::hasVariable(const string_type &variable) const
        {
            if(variableMap.count(variable) > 0)
                return true;

            auto value = std::getenv(variable.cStr().get());
            if(value != nullptr)
                return true;

            return false;
        }


        void EnvironmentSettings::setValueForVariable(const string_type &variable, const string_type &value)
        {
            bool overwrite = false;
            if(variableMap.count(variable) > 0)
            {
                variableMap[variable] = value;
                overwrite = true;
            }
            else
            {
                variableMap.insert(std::make_pair(variable, value));
            }

            setenv(variable.cStr().get(), value.cStr().get(), overwrite ? 1 : 0);
        }


        void EnvironmentSettings::addToValueForVariable(const string_type &variable, const string_type &value)
        {
            bool overwrite = false;

            if(variableMap.count(variable) > 0)
            {
                auto theValue = variableMap.at(variable);
                theValue += ":" + value;
                variableMap[variable] = theValue;
                overwrite = true;
            }
            else
                variableMap.insert(std::make_pair(variable, value));

            setenv(variable.cStr().get(), value.cStr().get(), overwrite ? 1 : 0);
        }


        void EnvironmentSettings::removeVariable(const string_type &variable)
        {
            variableMap.erase(variable);
            unsetenv(variable.cStr().get());
        }


        EnvironmentSettings::size_type EnvironmentSettings::numberOfVariables() const
        {
            return variableMap.size();
        }


        std::ostream &EnvironmentSettings::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("EnvironmentSettings");
                formatter->addClassMember<map_type>("variableMap", variableMap);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const EnvironmentSettings &s)
        {
            return s.description(o);
        }

    }    // namespace Foundation

}    // namespace TF
