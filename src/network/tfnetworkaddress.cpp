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
SOFTWARE.


******************************************************************************/

#define NEEDS_CSTRING
#include "tfheaders.hpp"
#include "tfnetworkaddress.hpp"

namespace TF::Foundation
{

    NetworkAddress::NetworkAddress()
    {
        clear();
    }

    NetworkAddress::NetworkAddress(const NetworkAddress & a)
    {
        clear();
        std::memcpy(&m_address, &a.m_address, a.address_length());
    }

    NetworkAddress::NetworkAddress(const struct sockaddr * sa, size_type length)
    {
        clear();
        if (sa != nullptr && length > 0 && length <= sizeof(struct sockaddr_storage))
        {
            std::memcpy(&m_address, sa, length * sizeof(char));
        }
        else
        {
            throw std::runtime_error{"Network address given a null pointer or zero length address"};
        }
    }

    NetworkAddress::~NetworkAddress() {}

    NetworkAddress & NetworkAddress::operator=(const NetworkAddress & a)
    {
        if (this == &a)
        {
            return *this;
        }

        clear();
        std::memcpy(&m_address, &a.m_address, sizeof(struct sockaddr_storage));

        return *this;
    }

    bool NetworkAddress::operator==(const NetworkAddress & a) const
    {
        if (this == &a)
        {
            return true;
        }

        return std::memcmp(&m_address, &a.m_address, a.address_length()) == 0;
    }

    bool NetworkAddress::operator!=(const NetworkAddress & a) const
    {
        return ! (*this == a);
    }

    bool NetworkAddress::operator<(const NetworkAddress & a) const
    {
        if (this == &a)
        {
            return false;
        }

        auto me = reinterpret_cast<const char *>(&m_address);
        auto thee = reinterpret_cast<const char *>(&a.m_address);

        return *me < *thee;
    }

    const struct sockaddr * NetworkAddress::get_as_sockaddr() const
    {
        return reinterpret_cast<const struct sockaddr *>(&m_address);
    }

    NetworkAddress::size_type NetworkAddress::address_length() const
    {
        return sizeof(struct sockaddr_storage);
    }

    void NetworkAddress::clear()
    {
        std::memset(&m_address, 0, sizeof(struct sockaddr_storage));
    }

} // namespace TF::Foundation
