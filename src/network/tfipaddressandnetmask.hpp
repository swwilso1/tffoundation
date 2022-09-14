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

namespace TF::Foundation
{

    struct IPAddressAndNetmask : public AllocatorInterface
    {
        IPAddress address{};
        IPAddress netmask{};

        IPAddressAndNetmask() = default;

        explicit IPAddressAndNetmask(const IPAddress & addr);

        IPAddressAndNetmask(const IPAddress & addr, const IPAddress & nmask) : address{addr}, netmask{nmask} {}

        [[nodiscard]] auto description(std::ostream & o) const -> std::ostream &;
    };

    std::ostream & operator<<(std::ostream & o, const IPAddressAndNetmask & addr_mask);

} // namespace TF::Foundation

#endif // TFIPADDRESSANDNETMASK_HPP
