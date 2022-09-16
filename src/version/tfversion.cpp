/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#include "tfversion.hpp"
#include "tfconversion.hpp"

namespace TF::Foundation
{

    Version::Version(uint32_t maj, uint32_t min, uint32_t rel, uint32_t pat) :
        major{maj}, minor{min}, release{rel}, patch{pat}
    {}

    auto Version::description(std::ostream & o) const -> std::ostream &
    {
        o << major << "." << minor << "." << release;
        if (patch > 0)
        {
            o << "." << patch;
        }
        return o;
    }

    std::ostream & operator<<(std::ostream & o, const Version & v)
    {
        return v.description(o);
    }

    auto Version::version_from_string(const string_type & s) -> Version
    {
        Version version{};

        if (s.length() == 0)
        {
            throw std::invalid_argument{"Version string is empty"};
        }

        auto version_numbers = s.split(".");
        if (version_numbers.size() == 0)
        {
            throw std::invalid_argument{"Version string did not contain a '.' character"};
        }

        auto major = Conversion::convertStringToInt(version_numbers[0]);
        if (! major)
        {
            throw std::invalid_argument{"Version string did not contain a major version number"};
        }

        version.major = static_cast<decltype(version.major)>(major.value());

        if (version_numbers.size() > 1)
        {
            auto minor = Conversion::convertStringToInt(version_numbers[1]);
            if (minor)
            {
                version.minor = static_cast<decltype(version.minor)>(minor.value());

                if (version_numbers.size() > 2)
                {
                    auto release = Conversion::convertStringToInt(version_numbers[2]);
                    if (release)
                    {
                        version.release = static_cast<decltype(version.release)>(release.value());

                        if (version_numbers.size() > 3)
                        {
                            auto patch = Conversion::convertStringToInt(version_numbers[3]);
                            if (patch)
                            {
                                version.patch = static_cast<decltype(version.patch)>(patch.value());
                            }
                        }
                    }
                }
            }
        }

        return version;
    }

} // namespace TF::Foundation
