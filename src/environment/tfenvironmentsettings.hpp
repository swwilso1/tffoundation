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

#ifndef TFENVIRONMENTSETTINGS_HPP
#define TFENVIRONMENTSETTINGS_HPP

#define NEEDS_OSTREAM
#define NEEDS_VECTOR
#define NEEDS_MAP
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfmap.hxx"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        class EnvironmentSettings : public AllocatorInterface
        {
        public:
            using string_type = String;

            using size_type = size_t;

            using string_array_type = std::vector<string_type>;

            EnvironmentSettings();

            ~EnvironmentSettings();

            string_array_type environmentVariables() const;

            string_type getValueForVariable(const string_type & variable) const;

            bool hasVariable(const string_type & variable) const;

            void setValueForVariable(const string_type & variable, const string_type & value);

            void addToValueForVariable(const string_type & variable, const string_type & value);

            void removeVariable(const string_type & variable);

            size_type numberOfVariables() const;

            std::ostream & description(std::ostream & o) const;

        private:
            using map_type = std::map<string_type, string_type>;

            map_type variableMap;
        };

        std::ostream & operator<<(std::ostream & o, const EnvironmentSettings & s);

    } // namespace Foundation

} // namespace TF

#endif // TFENVIRONMENTSETTINGS_HPP
