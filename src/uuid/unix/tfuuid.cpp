/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

#include "tfuuid.hpp"
#include "tfconfigure.hpp"

namespace TF
{

    namespace Foundation
    {

        const int UUID::uuidStringLength;

        UUID::UUID()
        {
            uuid_generate(m_theUUID);
        }

        UUID::UUID(const UUID &id)
        {
            uuid_copy(m_theUUID, id.m_theUUID);
        }

        UUID::UUID(UUID &&id)
        {
            uuid_copy(m_theUUID, id.m_theUUID);
        }

        UUID::~UUID()
        {
            uuid_clear(m_theUUID);
        }

        UUID &UUID::operator=(const UUID &id)
        {
            uuid_copy(m_theUUID, id.m_theUUID);
            return *this;
        }

        UUID &UUID::operator=(UUID &&id)
        {
            uuid_copy(m_theUUID, id.m_theUUID);
            return *this;
        }

        bool UUID::operator==(const UUID &id) const
        {
            if(uuid_compare(m_theUUID, id.m_theUUID) == 0)
                return true;
            return false;
        }

        bool UUID::operator==(const string_type &s) const
        {
            UUID theUUID;
            bool result = theUUID.fromString(s);
            if(result)
            {
                return *this == theUUID;
            }
            return false;
        }

        bool UUID::operator!=(const UUID &id) const
        {
            if(*this == id)
                return false;
            return true;
        }

        bool UUID::operator!=(const string_type &s) const
        {
            if(*this == s)
                return false;
            return true;
        }

        UUID::string_type UUID::toString() const
        {
#ifdef HAVE_UUID_STRING_T
            uuid_string_t uuidString;
#else
            char uuidString[uuidStringLength + 1];
            uuidString[uuidStringLength] = 0;
#endif
            uuid_unparse(m_theUUID, uuidString);
            string_type s = uuidString;
            return s.uppercaseString();
        }

        bool UUID::fromString(const string_type &s)
        {
            tfuuid_t theUUID;
            auto cStr = s.cStr();
            int result = uuid_parse(cStr.get(), theUUID);
            if(result == 0)
            {
                uuid_copy(m_theUUID, theUUID);
                return true;
            }
            return false;
        }

        std::ostream &UUID::description(std::ostream &o) const
        {
            o << toString();
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const UUID &id)
        {
            return id.description(o);
        }

    }    // namespace Foundation

}    // namespace TF
