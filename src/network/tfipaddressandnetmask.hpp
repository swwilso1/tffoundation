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

#ifndef TFIPADDRESSANDNETMASK_HPP
#define TFIPADDRESSANDNETMASK_HPP

#define NEEDS_OSTREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfipaddress.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    /**
     * Class that contains both an IPAddress and a related netmask.
     */
    struct IPAddressAndNetmask : public AllocatorInterface
    {
        using string_type = String;

        IPAddress address{};
        IPAddress netmask{};

        IPAddressAndNetmask() = default;

        /**
         * @brief constructor that initializes from a an address. Initializes the netmask to 0 for
         * the IPv4/IPv6 type of the address.
         * @param addr the address.
         */
        explicit IPAddressAndNetmask(const IPAddress & addr);

        /**
         * @brief constructo that initializes from a string with address and CIDR notation.
         * @param addr_with_cidr the string form of the address in CIDR notation.
         *
         * Example: 192.168.1.223/24
         *
         * If @e addr_with_cidr does not have the '/xx'  CIDR notation, the constructor just
         * initializes the address with the address value and the netmask to 0.
         */
        explicit IPAddressAndNetmask(const string_type & addr_with_cidr);

        /**
         * @brief constructor initialize the object with the address and netmask values
         * @param addr the address in string form
         * @param netm the netmask in string form.
         */
        IPAddressAndNetmask(const string_type & addr, const string_type & netm);

        /**
         * @brief constructor initializing the object with an address and netmask.
         * @param addr the address.
         * @param nmask the netmask.
         */
        IPAddressAndNetmask(const IPAddress & addr, const IPAddress & nmask) : address{addr}, netmask{nmask} {}

        /**
         * @brief method to get the address and netmask as a string value in CIDR notation.
         * @return the string address and netmask in CIDR notation.
         *
         * Example: 192.168.1.223, 255.255.255.0 converts to 192.168.1.223/24
         */
        [[nodiscard]] auto get_as_cidr_notation() -> string_type;

        /**
         * @brief method for writing the object to a stream.
         * @param o the stream object
         * @return @e o after writing the address and netmask to the stream.
         */
        [[nodiscard]] auto description(std::ostream & o) const -> std::ostream &;
    };

    /**
     * @brief overloaded operator<< for IPAddressAndNetmask objects.
     * @param o the stream object
     * @param addr_mask the IPAddressAndNetmask object.
     * @return @e o after writing the contents of @e addr_mask to the stream.
     */
    std::ostream & operator<<(std::ostream & o, const IPAddressAndNetmask & addr_mask);

} // namespace TF::Foundation

#endif // TFIPADDRESSANDNETMASK_HPP
