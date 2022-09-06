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

TEST(NetworkInterfacesTest, compile_test)
{
    NetworkInterfaces network_interfaces{};
    network_interfaces.load_interfaces();
    auto interface_names = network_interfaces.get_interface_names();
    for (auto & interface_name : interface_names)
    {
        std::cout << interface_name << ":" << std::endl;
        auto interface = network_interfaces.get_interface_by_name(interface_name);
        if (interface)
        {
            auto & addresses = interface.value().get_addresses();
            for (auto address : addresses)
            {
                address.get_presentation_name();
                std::cout << "\t" << address << std::endl;
            }
        }
    }
}

TEST(NetworkInterfacesTest, wireless_interfaces)
{
    NetworkInterfaces network_interfaces{};
    network_interfaces.load_interfaces();
    auto wireless_interfaces = network_interfaces.get_wireless_interfaces();
    for (auto & interface : wireless_interfaces)
    {
        std::cout << interface.get_name() << ":" << std::endl;
        auto addresses = interface.get_addresses();
        for (auto & address : addresses)
        {
            std::cout << "\t" << address << std::endl;
        }
    }
}
