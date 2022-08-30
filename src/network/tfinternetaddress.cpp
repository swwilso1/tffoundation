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

#include "tfinternetaddress.hpp"

namespace TF::Foundation
{

    InternetAddress::InternetAddress() : NetworkAddress() {}

    InternetAddress::InternetAddress(const IPAddress & addr, in_port_t port) : NetworkAddress()
    {
        init(addr, port);
    }

    InternetAddress::InternetAddress(const string_type & addr, in_port_t port) : NetworkAddress()
    {
        auto address = IPAddress::address_from_string(addr);
        init(address, port);
    }

    auto InternetAddress::address_length() const -> size_type
    {
        if (m_ipv4_address)
        {
            return sizeof(struct sockaddr_in);
        }
        return sizeof(struct sockaddr_in6);
    }

    auto InternetAddress::get_ip_address() const -> IPAddress
    {
        if (m_ipv4_address)
        {
            auto sockaddr_ptr = reinterpret_cast<const struct sockaddr_in *>(&m_address);
            IPAddress address{sockaddr_ptr->sin_addr};
            return address;
        }
        auto sockaddr_ptr = reinterpret_cast<const struct sockaddr_in6 *>(&m_address);
        IPAddress address{sockaddr_ptr->sin6_addr};
        return address;
    }

    auto InternetAddress::get_port() const -> int
    {
        if (m_ipv4_address)
        {
            auto sockaddr_ptr = reinterpret_cast<const struct sockaddr_in *>(&m_address);
            return ntohs(sockaddr_ptr->sin_port);
        }
        auto sockaddr_ptr = reinterpret_cast<const struct sockaddr_in6 *>(&m_address);
        return ntohs(sockaddr_ptr->sin6_port);
    }

    void InternetAddress::init(const IPAddress & addr, in_port_t port)
    {
        auto sockaddr_ptr = addr.get_as_sockaddr();
        auto sockaddr_length = addr.get_sockaddr_length();

        if (sockaddr_length == 0)
        {
            throw std::runtime_error{"IP address contains no actual address"};
        }

        if (addr.is_ipv4_address())
        {
            auto sockaddr_in_ptr = reinterpret_cast<struct sockaddr_in *>(sockaddr_ptr.get());
            sockaddr_in_ptr->sin_port = htons(port);
            sockaddr_in_ptr->sin_family = PF_INET;
            m_ipv4_address = true;
        }
        else if (addr.is_ipv6_address())
        {
            auto sockaddr_in6_ptr = reinterpret_cast<struct sockaddr_in6 *>(sockaddr_ptr.get());
            sockaddr_in6_ptr->sin6_port = htons(port);
            sockaddr_in6_ptr->sin6_family = PF_INET6;
        }
        else
        {
            throw std::runtime_error{"unrecognized IP address type"};
        }

        clear();
        std::memcpy(&m_address, sockaddr_ptr.get(), sockaddr_length);
    }
} // namespace TF::Foundation
