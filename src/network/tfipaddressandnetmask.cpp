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

#define NEEDS_ASSERT_H
#include "tfheaders.hpp"
#include "tfipaddressandnetmask.hpp"
#include "tfconversion.hpp"

namespace TF::Foundation
{

    struct BitMask
    {
        unsigned char mask;
        unsigned char bits;
    };

    IPAddressAndNetmask::IPAddressAndNetmask(const IPAddress & addr) : address{addr}
    {
        if (addr.is_ipv6_address())
        {
            // IPAddress{false} indicates an IPv6 address
            netmask = IPAddress{false};
        }
    }

    IPAddressAndNetmask::IPAddressAndNetmask(const string_type & addr_with_cidr)
    {
        // We have either just a plain address, or we have addr/cidr.  Check for the '/'
        // character.
        auto substrings = addr_with_cidr.substringsThatDoNotMatchString("/");
        if (substrings.size() == 1)
        {
            // The address has no CIDR component, so just initialize the address and netmask
            address = IPAddress::address_from_string(addr_with_cidr);
            if (address.is_ipv6_address())
            {
                // IPAddress{false} indicates an IPV6 address.
                netmask = IPAddress{false};
            }
        }
        else
        {
            constexpr BitMask masks[7] = {{0x80, 1}, {0xc0, 2}, {0xe0, 3}, {0xf0, 4}, {0xf8, 5}, {0xfc, 6}, {0xfe, 7}};

            // Hmm, should create a log entry in the shipping version if the assert fails...
            assert(substrings.size() >= 2);
            address = IPAddress::address_from_string(substrings[0]);
            auto cidr_value = Conversion::convertStringToInt(substrings[1]);
            if (! cidr_value)
            {
                throw std::invalid_argument{"string form of address contains an invalid CIDR specification"};
            }

            auto bytes = cidr_value.value() / 8;
            auto bits = cidr_value.value() % 8;

            if (address.is_ipv4_address())
            {
                struct in_addr netmask_value
                {};
                if (static_cast<size_t>(bytes) > sizeof(struct in_addr))
                {
                    throw std::invalid_argument{"CIDR specification contains an invalid range for IPv4 address"};
                }
                auto tmp = reinterpret_cast<unsigned char *>(&netmask_value);
                for (int i = 0; i < bytes; i++)
                {
                    *(tmp + i) = 0xFF;
                }
                tmp += bytes;
                if (bits > 0)
                {
                    *tmp = masks[bits - 1].mask;
                }
                netmask = IPAddress{netmask_value};
            }
            else if (address.is_ipv6_address())
            {
                struct in6_addr netmask_value
                {};
                if (static_cast<size_t>(bytes) > sizeof(struct in6_addr))
                {
                    throw std::invalid_argument{"CIDR specification contains an invalid range for IPv6 address"};
                }
                auto tmp = reinterpret_cast<unsigned char *>(&netmask_value);
                for (int i = 0; i < bytes; i++)
                {
                    *(tmp + i) = 0xFF;
                }
                tmp += bytes;
                if (bits > 0)
                {
                    *tmp = masks[bits - 1].mask;
                }
                netmask = IPAddress{netmask_value};
            }
        }
    }

    IPAddressAndNetmask::IPAddressAndNetmask(const string_type & addr, const string_type & netm)
    {
        address = IPAddress::address_from_string(addr);
        netmask = IPAddress::address_from_string(netm);
    }

    auto IPAddressAndNetmask::get_as_cidr_notation() -> string_type
    {
        constexpr BitMask masks[7] = {{0xfe, 7}, {0xfc, 6}, {0xf8, 5}, {0xf0, 4}, {0xe0, 3}, {0xc0, 2}, {0x80, 1}};
        auto raw_binary_netmask = netmask.get_as_in6_addr();
        auto tmp = reinterpret_cast<unsigned char *>(&raw_binary_netmask);
        int bits_in_mask{0};
        for (size_t i = 0; i < sizeof(decltype(raw_binary_netmask)); i++)
        {
            if (*(tmp + i) == 0xFF)
            {
                bits_in_mask += 8;
                continue;
            }

            if (*(tmp + i) == 0)
            {
                break;
            }

            auto number_of_array_elements = sizeof(masks) / sizeof(BitMask);
            for (decltype(number_of_array_elements) j = 0; j < number_of_array_elements; j++)
            {
                if ((*(tmp + i) & masks[j].mask) == masks[j].mask)
                {
                    bits_in_mask += masks[j].bits;
                    break;
                }
            }
            break;
        }

        auto address_string_representation = address.get_presentation_name();
        string_type result{*address_string_representation};
        if (bits_in_mask > 0)
        {
            result = string_type::initWithFormat("%@/%d", address_string_representation.get(), bits_in_mask);
        }
        return result;
    }

    auto IPAddressAndNetmask::description(std::ostream & o) const -> std::ostream &
    {
        auto formatter = std::unique_ptr<ClassFormatter>(FormatterFactory::getFormatter());
        if (formatter != nullptr)
        {
            formatter->setClassName("IPAddressAndNetmask");
            formatter->addClassMember("IPAddress", "address", address);
            formatter->addClassMember("IPAddress", "netmask", netmask);
            o << *formatter;
        }
        return o;
    }

    std::ostream & operator<<(std::ostream & o, const IPAddressAndNetmask & addr_mask)
    {
        return addr_mask.description(o);
    }

} // namespace TF::Foundation
