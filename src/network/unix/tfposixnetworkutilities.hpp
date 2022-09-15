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

#ifndef TFPOSIXNETWORKUTILITIES_HPP
#define TFPOSIXNETWORKUTILITIES_HPP

#include "tfstring.hpp"
#include "tfipaddress.hpp"

namespace TF::Foundation
{

    /**
     * @brief function to load the netmask associated with an interface with name @e name of the
     * network family @e family
     * @param name the name of the network interface
     * @param family PF_INET, PF_INET6, etc...
     * @return the address found to use as a netmask for the interface.
     */
    IPAddress get_netmask_for_interface_with_name(const String & name, int family);

} // namespace TF::Foundation

#endif // TFPOSIXNETWORKUTILITIES_HPP
