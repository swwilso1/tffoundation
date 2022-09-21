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

#include "tfipaddressandnetmask.hpp"

namespace TF::Foundation
{

    IPAddressAndNetmask::IPAddressAndNetmask(const IPAddress & addr) : address{addr}
    {
        if (addr.is_ipv6_address())
        {
            // IPAddress{false} indicates an IPv6 address
            netmask = IPAddress{false};
        }
    }

    IPAddressAndNetmask::IPAddressAndNetmask(const string_type & addr, const string_type & netm)
    {
        address = IPAddress::address_from_string(addr);
        netmask = IPAddress::address_from_string(netm);
    }

    struct BitMask
    {
        unsigned char mask;
        unsigned char bits;
    };

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
        return string_type::initWithFormat("%@/%d", address_string_representation.get(), bits_in_mask);
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
