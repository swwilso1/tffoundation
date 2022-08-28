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

#include <array>
#include "gtest/gtest.h"
#include "TFFoundation.hpp"

using namespace TF::Foundation;

TEST(IPAddress, compile_test)
{
    IPAddress address{};
    EXPECT_EQ(address.get_family(), PF_INET);
}

void ipv4_constructor_helper(uint32_t value, const String & addr)
{
    struct in_addr ip
    {};
    ip.s_addr = htonl(value);
    IPAddress address{ip};
    EXPECT_EQ(address, addr);
}

void ipv6_constructor_helper(std::array<uint32_t, 4> & array, const String & addr)
{
    struct in6_addr ip
    {};
    std::memcpy(&ip, array.data(), 4 * sizeof(uint32_t));
    IPAddress address{ip};
    EXPECT_EQ(address, addr);
}

TEST(IPAddress, constructor_tests)
{
    ipv4_constructor_helper(2886729728, "172.16.0.0");
    ipv4_constructor_helper(2886729729, "172.16.0.1");
    ipv4_constructor_helper(2887778302, "172.31.255.254");
    ipv4_constructor_helper(2130706432, "127.0.0.0");
    ipv4_constructor_helper(2130706433, "127.0.0.1");
    ipv4_constructor_helper(2147483647, "127.255.255.255");
    ipv4_constructor_helper(2851995648, "169.254.0.0");
    ipv4_constructor_helper(2852061182, "169.254.255.254");
    ipv4_constructor_helper(2852061183, "169.254.255.255");
    ipv4_constructor_helper(167772160, "10.0.0.0");
    ipv4_constructor_helper(167772161, "10.0.0.1");
    ipv4_constructor_helper(184549375, "10.255.255.255");
    ipv4_constructor_helper(184760251, "11.3.55.187");
    ipv4_constructor_helper(3232235520, "192.168.0.0");
    ipv4_constructor_helper(3232301054, "192.168.255.254");
    ipv4_constructor_helper(3232301055, "192.168.255.255");

    {
        std::array<uint32_t, 4> ip_array{};
        *ip_array.data() = 33022;
        *(ip_array.data() + 1) = 0;
        *(ip_array.data() + 2) = 606281752;
        *(ip_array.data() + 3) = 2967686819;
        ipv6_constructor_helper(ip_array, "fe80::1820:2324:a34e:e3b0");
    }

    {
        std::array<uint32_t, 4> ip_array{};
        *ip_array.data() = 2164392230;
        *(ip_array.data() + 1) = 453902528;
        *(ip_array.data() + 2) = 1038497792;
        *(ip_array.data() + 3) = 2978577987;
        ipv6_constructor_helper(ip_array, "2601:281:c000:e1b:38:e63d:437e:89b1");
    }

    {
        std::array<uint32_t, 4> ip_array{};
        *ip_array.data() = 2164392230;
        *(ip_array.data() + 1) = 453902528;
        *(ip_array.data() + 2) = 503621077;
        *(ip_array.data() + 3) = 1712613023;
        ipv6_constructor_helper(ip_array, "2601:281:c000:e1b:d5a5:41e:9f66:1466");
    }
}

void ipv4_address_from_string_helper(uint32_t value, const String & addr)
{
    struct in_addr ip
    {};
    ip.s_addr = htonl(value);
    IPAddress address{ip};
    auto address2 = IPAddress::address_from_string(addr);
    EXPECT_EQ(address, address2);
}

void ipv6_address_from_string_helper(std::array<uint32_t, 4> & array, const String & addr)
{
    struct in6_addr ip
    {};
    std::memcpy(&ip, array.data(), 4 * sizeof(uint32_t));
    IPAddress address{ip};
    auto address2 = IPAddress::address_from_string(addr);
    EXPECT_EQ(address, address2);
}

TEST(IPAddress, address_from_string_tests)
{
    ipv4_address_from_string_helper(2886729728, "172.16.0.0");
    ipv4_address_from_string_helper(2886729729, "172.16.0.1");
    ipv4_address_from_string_helper(2887778302, "172.31.255.254");
    ipv4_address_from_string_helper(2130706432, "127.0.0.0");
    ipv4_address_from_string_helper(2130706433, "127.0.0.1");
    ipv4_address_from_string_helper(2147483647, "127.255.255.255");
    ipv4_address_from_string_helper(2851995648, "169.254.0.0");
    ipv4_address_from_string_helper(2852061182, "169.254.255.254");
    ipv4_address_from_string_helper(2852061183, "169.254.255.255");
    ipv4_address_from_string_helper(167772160, "10.0.0.0");
    ipv4_address_from_string_helper(167772161, "10.0.0.1");
    ipv4_address_from_string_helper(184549375, "10.255.255.255");
    ipv4_address_from_string_helper(184760251, "11.3.55.187");
    ipv4_address_from_string_helper(3232235520, "192.168.0.0");
    ipv4_address_from_string_helper(3232301054, "192.168.255.254");
    ipv4_address_from_string_helper(3232301055, "192.168.255.255");

    {
        std::array<uint32_t, 4> ip_array{};
        *ip_array.data() = 33022;
        *(ip_array.data() + 1) = 0;
        *(ip_array.data() + 2) = 606281752;
        *(ip_array.data() + 3) = 2967686819;
        ipv6_address_from_string_helper(ip_array, "fe80::1820:2324:a34e:e3b0");
    }

    {
        std::array<uint32_t, 4> ip_array{};
        *ip_array.data() = 2164392230;
        *(ip_array.data() + 1) = 453902528;
        *(ip_array.data() + 2) = 1038497792;
        *(ip_array.data() + 3) = 2978577987;
        ipv6_address_from_string_helper(ip_array, "2601:281:c000:e1b:38:e63d:437e:89b1");
    }

    {
        std::array<uint32_t, 4> ip_array{};
        *ip_array.data() = 2164392230;
        *(ip_array.data() + 1) = 453902528;
        *(ip_array.data() + 2) = 503621077;
        *(ip_array.data() + 3) = 1712613023;
        ipv6_address_from_string_helper(ip_array, "2601:281:c000:e1b:d5a5:41e:9f66:1466");
    }
}
