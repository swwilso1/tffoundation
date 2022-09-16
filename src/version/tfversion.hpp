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

#ifndef TFVERSION_HPP
#define TFVERSION_HPP

#define NEEDS_OSTREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    struct Version : public AllocatorInterface
    {
        using string_type = String;

        uint32_t major{};
        uint32_t minor{};
        uint32_t release{};
        uint32_t patch{};

        Version() = default;

        Version(uint32_t maj, uint32_t min, uint32_t rel, uint32_t pat = 0);

        [[nodiscard]] auto description(std::ostream & o) const -> std::ostream &;

        static auto version_from_string(const string_type & s) -> Version;
    };

    std::ostream & operator<<(std::ostream & o, const Version & v);

} // namespace TF::Foundation

#endif // TFVERSION_HPP
