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

#ifndef TFUUID_HPP
#define TFUUID_HPP

#define NEEDS_OPTIONAL
#define NEEDS_OSTREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfuuidtype.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        class UUID : public AllocatorInterface
        {
        public:
            using string_type = String;

            UUID();

            UUID(const UUID &id);

            UUID(UUID &&id);

            ~UUID();

            UUID &operator=(const UUID &id);

            UUID &operator=(UUID &&id);

            bool operator==(const UUID &id) const;

            bool operator==(const string_type &s) const;

            bool operator!=(const UUID &id) const;

            bool operator!=(const string_type &s) const;

            string_type toString() const;

            static std::optional<UUID> fromString(const string_type &s);

            std::ostream &description(std::ostream &o) const;

            static const int uuidStringLength = 36;

        private:
            UUID(tfuuid_t uid);

            tfuuid_t m_theUUID;
        };

        std::ostream &operator<<(std::ostream &o, const UUID &id);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFUUID_HPP
