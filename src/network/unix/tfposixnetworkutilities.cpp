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
#define NEEDS_NETINET_IN_H
#define NEEDS_NET_IF_H
#define NEEDS_UNISTD_H
#define NEEDS_SYS_IOCTL_H
#include "tfheaders.hpp"
#include "tfposixnetworkutilities.hpp"
#include "tfsocket.hpp"

namespace TF::Foundation
{

    IPAddress get_netmask_for_interface_with_name(const String & name, int family_type)
    {
        struct ifreq ifr
        {};
        Socket s{family_type, SOCK_DGRAM};

        std::memset(&ifr, 0, sizeof(struct ifreq));
        ifr.ifr_addr.sa_family = static_cast<sa_family_t>(family_type);

        auto iface_name_cstr = name.cStr();
        auto length = name.length() > (IFNAMSIZ - 1) ? (IFNAMSIZ - 1) : name.length();
        std::memcpy(ifr.ifr_name, iface_name_cstr.get(), length);
        auto api_result = ioctl(s.get_raw_socket(), SIOCGIFNETMASK, &ifr);
        if (api_result >= 0)
        {
            IPAddress netmask{&ifr.ifr_addr};
            return {&ifr.ifr_addr};
        }

        return IPAddress{family_type == PF_INET};
    }

} // namespace TF::Foundation
