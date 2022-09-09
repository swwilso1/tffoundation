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

// This code uses very macOS specific CoreFoundation and SystemConfiguration frameworks.
#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

#define NEEDS_CSTRING
#define NEEDS_ARRAY
#include "tfheaders.hpp"
#include "tfnetworkinterface.hpp"

namespace TF::Foundation
{
    auto NetworkInterface::is_wireless_interface() const -> bool
    {
        std::array<char, 1024> interface_name_buffer{};
        CFArrayRef interfaces{};
        SCNetworkInterfaceRef interface {};
        CFIndex number_of_interfaces{};
        CFStringRef interface_type{};
        bool method_result{false};

        std::memset(interface_name_buffer.data(), 0, interface_name_buffer.size());

        interfaces = SCNetworkInterfaceCopyAll();
        number_of_interfaces = CFArrayGetCount(interfaces);
        for (decltype(number_of_interfaces) i = 0; i < number_of_interfaces; i++)
        {
            interface = reinterpret_cast<SCNetworkInterfaceRef>(CFArrayGetValueAtIndex(interfaces, i));
            interface_type = SCNetworkInterfaceGetInterfaceType(interface);

            auto bsd_interface_name = SCNetworkInterfaceGetBSDName(interface);
            auto c_bsd_interface_name = CFStringGetCStringPtr(bsd_interface_name, kCFStringEncodingUTF8);
            if (c_bsd_interface_name == nullptr)
            {
                auto api_result = CFStringGetCString(bsd_interface_name, interface_name_buffer.data(),
                                                     interface_name_buffer.size(), kCFStringEncodingUTF8);
                if (api_result)
                {
                    c_bsd_interface_name =
                        reinterpret_cast<decltype(c_bsd_interface_name)>(interface_name_buffer.data());
                }
            }

            if (c_bsd_interface_name)
            {
                string_type interface_name{c_bsd_interface_name};
                if (interface_name == m_name)
                {
                    if (CFStringCompare(interface_type, kSCNetworkInterfaceTypeIEEE80211, 0) == 0)
                    {
                        method_result = true;
                    }
                    break;
                }
            }
        }

        return method_result;
    }
} // namespace TF::Foundation
