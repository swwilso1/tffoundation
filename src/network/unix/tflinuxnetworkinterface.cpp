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
#define NEEDS_SYS_IOCTL_H
#define NEEDS_LINUX_WIRELESS_H
#include "tfheaders.hpp"
#include "tfnetworkinterface.hpp"

namespace TF::Foundation
{
    auto NetworkInterface::is_wireless_interface() const -> bool
    {
        struct iwreq req_struct
        {};
        std::memset(&req_struct, 0, sizeof(struct iwreq));

        auto name_cstring = m_name.cStr();

        // We cannot use m_name.length() because it m_name may contain non-ascii characters.
        auto name_strlen = std::strlen(name_cstring.get());
        std::memcpy(req_struct.ifr_name, name_cstring.get(), name_strlen > IFNAMSIZ ? IFNAMSIZ : name_strlen);

        auto s = socket(PF_INET, SOCK_STREAM, 0);
        if (s < 0)
        {
            throw std::system_error{errno, std::system_category(), "unable to create socket for ioctl use"};
        }

        auto api_result = ioctl(s, SIOCGIWNAME, &req_struct);
        close(s);
        return api_result < 0 ? false : true;
    }

} // namespace TF::Foundation