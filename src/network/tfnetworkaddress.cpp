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
        if (sa == nullptr || length == 0 || length > sizeof(struct sockaddr_storage))
        {
            throw std::runtime_error{
                "Network address given a null pointer, a zero length address, or an address longer than sockaddr_storage"};
        }
        std::memcpy(&m_address, sa, length * sizeof(char));
    }

    NetworkAddress::NetworkAddress(const void * p, size_type length)
    {
        clear();
        if (p == nullptr || length == 0 || length > sizeof(struct sockaddr_storage))
        {
            throw std::invalid_argument{"NetworkAddress argument byte array exceeds size of struct sockaddr_storage"};
        }
        std::memcpy(&m_address, p, length * sizeof(char));
    }

    NetworkAddress::~NetworkAddress() {}

    auto NetworkAddress::operator=(const NetworkAddress & a) -> NetworkAddress &
    {
        if (this == &a)
        {
            return *this;
        }

        clear();
        std::memcpy(&m_address, &a.m_address, sizeof(struct sockaddr_storage));

        return *this;
    }

    auto NetworkAddress::operator==(const NetworkAddress & a) const -> bool
    {
        if (this == &a)
        {
            return true;
        }

        return std::memcmp(&m_address, &a.m_address, a.address_length()) == 0;
    }

    auto NetworkAddress::operator!=(const NetworkAddress & a) const -> bool
    {
        return ! (*this == a);
    }

    auto NetworkAddress::operator<(const NetworkAddress & a) const -> bool
    {
        if (this == &a)
        {
            return false;
        }

        auto me = reinterpret_cast<const char *>(&m_address);
        auto thee = reinterpret_cast<const char *>(&a.m_address);

        return *me < *thee;
    }

    auto NetworkAddress::get_as_sockaddr() const -> const struct sockaddr *
    {
        return reinterpret_cast<const struct sockaddr *>(&m_address);
    }

    auto NetworkAddress::address_length() const -> NetworkAddress::size_type
    {
        return sizeof(struct sockaddr_storage);
    }

    void NetworkAddress::clear()
    {
        std::memset(&m_address, 0, sizeof(struct sockaddr_storage));
    }

} // namespace TF::Foundation
