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

#define NEEDS_SYS_TYPES_H
#define NEEDS_SYS_SOCKET_H
#define NEEDS_NETDB_H
#define NEEDS_ARRAY
#define NEEDS_CASSERT
#include "tfheaders.hpp"
#include "tfipaddress.hpp"
#include "tfnetworkerror.hpp"

namespace TF::Foundation
{
    IPAddress::IPAddress() : m_address_family(PF_INET)
    {
        std::memset(&m_address, 0, sizeof(address_type));
    }

    IPAddress::IPAddress(const struct in_addr & ipv4_address) : m_address_family(PF_INET)
    {
        std::memcpy(&m_address.ipv4_address, &ipv4_address, sizeof(struct in_addr));
    }

    IPAddress::IPAddress(const struct in6_addr & ipv6_address) : m_address_family(PF_INET6)
    {
        std::memcpy(&m_address.ipv6_address, &ipv6_address, sizeof(struct in6_addr));
    }

    IPAddress::IPAddress(const IPAddress & a)
    {
        std::memcpy(&m_address, &a.m_address, sizeof(address_type));
        m_address_family = a.m_address_family;
        m_presentation_name = a.m_presentation_name;
        m_dns_name = a.m_dns_name;
    }

    auto IPAddress::address_from_string(const string_type & value) -> IPAddress
    {
        IPAddress new_address{};
        struct addrinfo address_hints
        {};
        struct addrinfo * address_results{nullptr};

        if (value.length() == 0)
        {
            throw std::runtime_error{"Unable to generate address from empty string"};
        }

        auto value_cstr = value.cStr();

        // Be sure to zero address_hints
        std::memset(&address_hints, 0, sizeof(struct addrinfo));
        address_hints.ai_family = PF_UNSPEC;
        address_hints.ai_socktype = SOCK_STREAM;

        auto api_result =
            getaddrinfo(value_cstr.get(), nullptr /* no service name */, &address_hints, &address_results);
        if (api_result != 0)
        {
            freeaddrinfo(address_results);
            runtime_error_from_api_error(api_result);
        }

        bool found_address{false};
        while (address_results != nullptr)
        {
            // Take the first binary address (It is possible to have more than one).
            if (address_results->ai_family == PF_INET)
            {
                // The address is an IPV4 address, recover the sockaddr_in struct to get to
                // the struct in_addr address.
                auto ipv4_struct = reinterpret_cast<sockaddr_in *>(address_results->ai_addr);
                std::memcpy(&new_address.m_address.ipv4_address, &ipv4_struct->sin_addr, sizeof(struct in_addr));
                new_address.m_address_family = PF_INET;
                found_address = true;
                break;
            }
            else if (address_results->ai_family == PF_INET6)
            {
                // The address is an IPV6 address, recover the sockaddr_in6 struct to get to
                // the struct in6_addr address.
                auto ipv6_struct = reinterpret_cast<sockaddr_in6 *>(address_results->ai_addr);
                std::memcpy(&new_address.m_address.ipv6_address, &ipv6_struct->sin6_addr, sizeof(struct in6_addr));
                new_address.m_address_family = PF_INET6;
                found_address = true;
                break;
            }

            address_results = address_results->ai_next;
        }

        freeaddrinfo(address_results);

        if (! found_address)
        {
            throw std::runtime_error{"Unable to find IP address for string"};
        }
        return new_address;
    }

    auto IPAddress::operator=(const IPAddress & a) -> IPAddress &
    {
        if (this == &a)
        {
            return *this;
        }
        std::memcpy(&m_address, &a.m_address, sizeof(address_type));
        m_address_family = a.m_address_family;
        m_presentation_name = a.m_presentation_name;
        m_dns_name = a.m_dns_name;
        return *this;
    }

    auto IPAddress::operator=(const struct in_addr & ipv4_address) -> IPAddress &
    {
        std::memcpy(&m_address.ipv4_address, &ipv4_address, sizeof(struct in_addr));
        m_address_family = PF_INET;
        m_presentation_name = nullptr;
        m_dns_name = nullptr;
        return *this;
    }

    auto IPAddress::operator=(const struct in6_addr & ipv6_address) -> IPAddress &
    {
        std::memcpy(&m_address.ipv6_address, &ipv6_address, sizeof(struct in6_addr));
        m_address_family = PF_INET6;
        m_presentation_name = nullptr;
        m_dns_name = nullptr;
        return *this;
    }

    auto IPAddress::operator==(const IPAddress & a) const -> bool
    {
        if (this == &a)
        {
            return true;
        }

        if (m_address_family != a.m_address_family)
        {
            return false;
        }

        if (a.m_address_family == PF_INET)
        {
            return std::memcmp(&m_address.ipv4_address, &a.m_address.ipv4_address, sizeof(struct in_addr)) == 0;
        }
        return std::memcmp(&m_address.ipv6_address, &a.m_address.ipv6_address, sizeof(struct in6_addr)) == 0;
    }

    auto IPAddress::operator!=(const IPAddress & a) const -> bool
    {
        return ! (*this == a);
    }

    auto IPAddress::operator<(const IPAddress & a) const -> bool
    {
        if (m_address_family == PF_INET6 && a.m_address_family == PF_INET)
        {
            return false;
        }
        if (m_address_family == PF_INET && a.m_address_family == PF_INET6)
        {
            return true;
        }

        if (*this == a)
        {
            return false;
        }

        if (m_address_family == PF_INET)
        {
            return m_address.ipv4_address.s_addr < a.m_address.ipv4_address.s_addr;
        }

        assert(m_address_family == PF_INET6);
        for (size_t i = 0; i < sizeof(struct in6_addr); i++)
        {
            const char * my_byte = reinterpret_cast<const char *>(&m_address.ipv6_address) + i;
            const char * other_byte = reinterpret_cast<const char *>(&a.m_address.ipv6_address) + i;
            if (*my_byte >= *other_byte)
            {
                return false;
            }
        }
        return true;
    }

    auto IPAddress::get_family() const -> sa_family_t
    {
        return m_address_family;
    }

    auto IPAddress::get_as_sockaddr() const -> std::unique_ptr<struct sockaddr>
    {
        std::unique_ptr<struct sockaddr> return_value{};
        if (m_address_family == PF_INET)
        {
            auto address_in_pointer = new sockaddr_in{};
            address_in_pointer->sin_family = PF_INET;
            memcpy(&address_in_pointer->sin_addr, &m_address.ipv4_address, sizeof(struct in_addr));
            return_value = std::unique_ptr<struct sockaddr>(reinterpret_cast<struct sockaddr *>(address_in_pointer));
        }
        else if (m_address_family == PF_INET6)
        {
            auto address_in_pointer = new sockaddr_in6{};
            address_in_pointer->sin6_family = PF_INET6;
            memcpy(&address_in_pointer->sin6_addr, &m_address.ipv4_address, sizeof(struct in6_addr));
            return_value = std::unique_ptr<struct sockaddr>(reinterpret_cast<struct sockaddr *>(address_in_pointer));
        }
        return return_value;
    }

    auto IPAddress::is_ipv4_address() const -> bool
    {
        return m_address_family == PF_INET;
    }

    auto IPAddress::is_ipv6_address() const -> bool
    {
        return m_address_family == PF_INET6;
    }

    auto IPAddress::is_private_address() const -> bool
    {
        if (m_address_family == PF_INET)
        {
            // The value should always be stored in network byte order in the struct.
            auto value = ntohl(static_cast<uint32_t>(m_address.ipv4_address.s_addr));

            // 2886729728 -> 172.16.0.0
            // 2887778303 -> 172.31.255.255
            // 2130706432 -> 127.0.0.0
            // 2147483647 -> 127.255.255.255
            // 2851995648 -> 169.254.0.0
            // 2852061183 -> 169.254.255.255
            // 167772160 -> 10.0.0.0
            // 184549375 -> 10.255.255.255
            // 3232235520 -> 192.168.0.0
            // 3232301055 -> 192.168.255.255
            if ((value >= 2886729728UL && value <= 2887778303UL) || (value >= 2130706432UL && value <= 2147483647UL) ||
                (value >= 2851995648UL && value <= 2852061183UL) || (value >= 167772160UL && value <= 184549375UL) ||
                (value >= 3232235520UL && value <= 3232301055UL))
            {
                return true;
            }
        }
        else if (m_address_family == PF_INET6)
        {
            if (IN6_IS_ADDR_UNSPECIFIED(&m_address.ipv6_address) || IN6_IS_ADDR_LINKLOCAL(&m_address.ipv6_address) ||
                IN6_IS_ADDR_SITELOCAL(&m_address.ipv6_address))
            {
                return true;
            }
        }
        return false;
    }

    auto IPAddress::is_global_address() const -> bool
    {
        if (m_address_family == PF_INET)
        {
            // The value should always be stored in network byte order in the struct.
            auto value = ntohl(static_cast<uint32_t>(m_address.ipv4_address.s_addr));

            // 2886729728 -> 172.16.0.0
            // 2887778303 -> 172.31.255.255
            // 2130706432 -> 127.0.0.0
            // 2147483647 -> 127.255.255.255
            // 2851995648 -> 169.254.0.0
            // 2852061183 -> 169.254.255.255
            // 167772160 -> 10.0.0.0
            // 184549375 -> 10.255.255.255
            // 3232235520 -> 192.168.0.0
            // 3232301055 -> 192.168.255.255
            if ((value >= 2886729728UL && value <= 2887778303UL) || (value >= 2130706432UL && value <= 2147483647UL) ||
                (value >= 2851995648UL && value <= 2852061183UL) || (value >= 167772160UL && value <= 184549375UL) ||
                (value >= 3232235520UL && value <= 3232301055UL))
            {
                return false;
            }
        }
        else if (m_address_family == PF_INET6)
        {
            if (IN6_IS_ADDR_V4MAPPED(&m_address.ipv6_address) || IN6_IS_ADDR_LINKLOCAL(&m_address.ipv6_address) ||
                IN6_IS_ADDR_LOOPBACK(&m_address.ipv6_address) || IN6_IS_ADDR_UNSPECIFIED(&m_address.ipv6_address) ||
                IN6_IS_ADDR_MULTICAST(&m_address.ipv6_address) || IN6_IS_ADDR_SITELOCAL(&m_address.ipv6_address))
            {
                return false;
            }
        }
        return true;
    }

    auto IPAddress::is_site_local_address() const -> bool
    {
        if (m_address_family == PF_INET6 && IN6_IS_ADDR_SITELOCAL(&m_address.ipv6_address))
        {
            return true;
        }
        return false;
    }

    auto IPAddress::is_link_local_address() const -> bool
    {
        if (m_address_family == PF_INET)
        {
            auto value = ntohl(static_cast<uint32_t>(m_address.ipv4_address.s_addr));
            // 2851995648 -> 169.254.0.0
            // 2852061183 -> 169.254.255.255
            if (value >= 2851995648UL && value <= 2852061183UL)
            {
                return true;
            }
        }
        else if (m_address_family == PF_INET6)
        {
            return IN6_IS_ADDR_LINKLOCAL(&m_address.ipv6_address);
        }
        return false;
    }

    auto IPAddress::is_loopback_address() const -> bool
    {
        if (m_address_family == PF_INET)
        {
            auto value = ntohl(static_cast<uint32_t>(m_address.ipv4_address.s_addr));
            // 2130706432 -> 127.0.0.0
            // 2147483647 -> 127.255.255.255
            if (value >= 2130706432UL && value <= 2147483647UL)
            {
                return true;
            }
        }
        else if (m_address_family == PF_INET6)
        {
            return IN6_IS_ADDR_LOOPBACK(&m_address.ipv6_address);
        }
        return false;
    }

    auto IPAddress::is_ipv4_mapped_ipv6_address() const -> bool
    {
        if (m_address_family == PF_INET6)
        {
            return IN6_IS_ADDR_V4MAPPED(&m_address.ipv6_address);
        }
        return false;
    }

    auto IPAddress::description(std::ostream & o) const -> std::ostream &
    {
        if (m_presentation_name != nullptr)
        {
            o << *m_presentation_name;
        }
        else
        {
            auto pname = get_address_name();
            o << *pname;
        }
        return o;
    }

    auto IPAddress::get_presentation_name() -> string_pointer_type
    {
        if (m_presentation_name != nullptr)
        {
            return m_presentation_name;
        }
        m_presentation_name = get_address_name();
        return m_presentation_name;
    }

    auto IPAddress::get_dns_name() -> string_pointer_type
    {
        if (m_dns_name != nullptr)
        {
            return m_dns_name;
        }
        m_dns_name = get_address_name(true);
        return m_dns_name;
    }

    void IPAddress::runtime_error_from_api_error(int error)
    {
        NetworkErrorTable table{};
        auto error_message = table.get_explanation_for_error(error);
        if (error_message.length() == 0)
        {
            error_message = "IP address lookup from string content failed";
        }
        auto error_cstr = error_message.cStr();
        throw std::runtime_error{error_cstr.get()};
    }

    auto IPAddress::get_address_name(bool get_dns_name) const -> IPAddress::string_pointer_type
    {
        auto address_pointer = get_as_sockaddr();
        std::array<char, NI_MAXHOST> hostname{};
        std::array<char, NI_MAXSERV> servicename{};

        int flags;

        if (get_dns_name)
        {
            flags = NI_NAMEREQD | NI_NUMERICSERV;
        }
        else
        {
            flags = NI_NUMERICHOST | NI_NUMERICSERV;
        }

        // Not sure if this is needed.
        std::memset(hostname.data(), 0, NI_MAXHOST);
        std::memset(servicename.data(), 0, NI_MAXSERV);

        socklen_t address_length =
            m_address_family == PF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

        auto api_result = getnameinfo(address_pointer.get(), address_length, hostname.data(), NI_MAXHOST,
                                      servicename.data(), NI_MAXSERV, flags);
        if (api_result != 0)
        {
            runtime_error_from_api_error(api_result);
        }

        string_type the_name(hostname.data(), std::strlen(hostname.data()));
        auto range_of_char = the_name.rangeOfString("%");
        if (range_of_char.length > 0)
        {
            Range range_of_name{0, range_of_char.length};
            the_name = the_name.substringWithRange(range_of_name);
        }

        return std::make_shared<string_type>(the_name);
    }

    auto operator<<(std::ostream & o, const IPAddress & a) -> std::ostream &
    {
        return a.description(o);
    }

    auto operator==(const IPAddress & a, const struct in_addr & ipv4_address) -> bool
    {
        IPAddress other{ipv4_address};
        return a == other;
    }

    auto operator==(const IPAddress & a, const struct in6_addr & ipv6_address) -> bool
    {
        IPAddress other{ipv6_address};
        return a == other;
    }

    auto operator==(const IPAddress & a, const String & address) -> bool
    {
        try
        {
            auto other = IPAddress::address_from_string(address);
            return a == other;
        }
        catch (...)
        {
            return false;
        }
    }

    auto operator==(const struct in_addr & ipv4_address, const IPAddress & a) -> bool
    {
        IPAddress other{ipv4_address};
        return other == a;
    }

    auto operator==(const struct in6_addr & ipv6_address, const IPAddress & a) -> bool
    {
        IPAddress other{ipv6_address};
        return other == a;
    }

    auto operator==(const String & address, const IPAddress & a) -> bool
    {
        try
        {
            auto other = IPAddress::address_from_string(address);
            return other == a;
        }
        catch (...)
        {
            return false;
        }
    }

    auto operator!=(const IPAddress & a, const struct in_addr & ipv4_address) -> bool
    {
        IPAddress other{ipv4_address};
        return a != other;
    }

    auto operator!=(const IPAddress & a, const struct in6_addr & ipv6_address) -> bool
    {
        IPAddress other{ipv6_address};
        return a != other;
    }

    auto operator!=(const IPAddress & a, const String & address) -> bool
    {
        try
        {
            auto other = IPAddress::address_from_string(address);
            return a != other;
        }
        catch (...)
        {
            return false;
        }
    }

    auto operator!=(const struct in_addr & ipv4_address, const IPAddress & a) -> bool
    {
        IPAddress other{ipv4_address};
        return other != a;
    }

    auto operator!=(const struct in6_addr & ipv6_address, const IPAddress & a) -> bool
    {
        IPAddress other{ipv6_address};
        return other != a;
    }

    auto operator!=(const String & address, const IPAddress & a) -> bool
    {
        try
        {
            auto other = IPAddress::address_from_string(address);
            return other != a;
        }
        catch (...)
        {
            return false;
        }
    }

    auto operator<(const IPAddress & a, const struct in_addr & ipv4_address) -> bool
    {
        IPAddress other{ipv4_address};
        return a < other;
    }

    auto operator<(const IPAddress & a, const struct in6_addr & ipv6_address) -> bool
    {
        IPAddress other{ipv6_address};
        return a < other;
    }

    auto operator<(const IPAddress & a, const String & address) -> bool
    {
        try
        {
            auto other = IPAddress::address_from_string(address);
            return a < other;
        }
        catch (...)
        {
            return false;
        }
    }

    auto operator<(const struct in_addr & ipv4_address, const IPAddress & a) -> bool
    {
        IPAddress other{ipv4_address};
        return other < a;
    }

    auto operator<(const struct in6_addr & ipv6_address, const IPAddress & a) -> bool
    {
        IPAddress other{ipv6_address};
        return other < a;
    }

    auto operator<(const String & address, const IPAddress & a) -> bool
    {
        try
        {
            auto other = IPAddress::address_from_string(address);
            return other < a;
        }
        catch (...)
        {
            return false;
        }
    }

} // namespace TF::Foundation
