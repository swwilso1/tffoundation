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

#include "gtest/gtest.h"
#include "TFFoundation.hpp"

using namespace TF::Foundation;

void cidr_test(const String & address, const String & netmask, const String & result)
{
    IPAddressAndNetmask addr_mask{address, netmask};
    EXPECT_EQ(addr_mask.get_as_cidr_notation(), result);
}

TEST(IPAddressAndNetmask, ipv4_cidr_tests)
{
    String ipv4_addr_str{"10.0.1.0"};
    cidr_test(ipv4_addr_str, "255.255.255.255", ipv4_addr_str + "/32");
    cidr_test(ipv4_addr_str, "255.255.255.254", ipv4_addr_str + "/31");
    cidr_test(ipv4_addr_str, "255.255.255.252", ipv4_addr_str + "/30");
    cidr_test(ipv4_addr_str, "255.255.255.248", ipv4_addr_str + "/29");
    cidr_test(ipv4_addr_str, "255.255.255.240", ipv4_addr_str + "/28");
    cidr_test(ipv4_addr_str, "255.255.255.224", ipv4_addr_str + "/27");
    cidr_test(ipv4_addr_str, "255.255.255.192", ipv4_addr_str + "/26");
    cidr_test(ipv4_addr_str, "255.255.255.128", ipv4_addr_str + "/25");
    cidr_test(ipv4_addr_str, "255.255.255.0", ipv4_addr_str + "/24");
    cidr_test(ipv4_addr_str, "255.255.254.0", ipv4_addr_str + "/23");
    cidr_test(ipv4_addr_str, "255.255.252.0", ipv4_addr_str + "/22");
    cidr_test(ipv4_addr_str, "255.255.248.0", ipv4_addr_str + "/21");
    cidr_test(ipv4_addr_str, "255.255.240.0", ipv4_addr_str + "/20");
    cidr_test(ipv4_addr_str, "255.255.224.0", ipv4_addr_str + "/19");
    cidr_test(ipv4_addr_str, "255.255.192.0", ipv4_addr_str + "/18");
    cidr_test(ipv4_addr_str, "255.255.128.0", ipv4_addr_str + "/17");
    cidr_test(ipv4_addr_str, "255.255.0.0", ipv4_addr_str + "/16");
    cidr_test(ipv4_addr_str, "255.254.0.0", ipv4_addr_str + "/15");
    cidr_test(ipv4_addr_str, "255.252.0.0", ipv4_addr_str + "/14");
    cidr_test(ipv4_addr_str, "255.248.0.0", ipv4_addr_str + "/13");
    cidr_test(ipv4_addr_str, "255.240.0.0", ipv4_addr_str + "/12");
    cidr_test(ipv4_addr_str, "255.224.0.0", ipv4_addr_str + "/11");
    cidr_test(ipv4_addr_str, "255.192.0.0", ipv4_addr_str + "/10");
    cidr_test(ipv4_addr_str, "255.128.0.0", ipv4_addr_str + "/9");
    cidr_test(ipv4_addr_str, "255.0.0.0", ipv4_addr_str + "/8");
    cidr_test(ipv4_addr_str, "254.0.0.0", ipv4_addr_str + "/7");
    cidr_test(ipv4_addr_str, "252.0.0.0", ipv4_addr_str + "/6");
    cidr_test(ipv4_addr_str, "248.0.0.0", ipv4_addr_str + "/5");
    cidr_test(ipv4_addr_str, "240.0.0.0", ipv4_addr_str + "/4");
    cidr_test(ipv4_addr_str, "224.0.0.0", ipv4_addr_str + "/3");
    cidr_test(ipv4_addr_str, "192.0.0.0", ipv4_addr_str + "/2");
    cidr_test(ipv4_addr_str, "128.0.0.0", ipv4_addr_str + "/1");
    cidr_test(ipv4_addr_str, "0.0.0.0", ipv4_addr_str + "/0");
}

void test_from_addr_and_cidr(const String & addr_and_cidr, const String & address, const String & netmask)
{
    IPAddressAndNetmask addr_mask{addr_and_cidr};
    EXPECT_EQ(addr_mask.address, IPAddress::address_from_string(address));
    EXPECT_EQ(addr_mask.netmask, IPAddress::address_from_string(netmask));
}

TEST(IPAddressAndNetmask, ipv4_from_addr_and_cidr_test)
{
    const String address{"192.168.1.223"};
    test_from_addr_and_cidr(address + "/0", address, "0.0.0.0");
    test_from_addr_and_cidr(address + "/1", address, "128.0.0.0");
    test_from_addr_and_cidr(address + "/2", address, "192.0.0.0");
    test_from_addr_and_cidr(address + "/3", address, "224.0.0.0");
    test_from_addr_and_cidr(address + "/4", address, "240.0.0.0");
    test_from_addr_and_cidr(address + "/5", address, "248.0.0.0");
    test_from_addr_and_cidr(address + "/6", address, "252.0.0.0");
    test_from_addr_and_cidr(address + "/7", address, "254.0.0.0");
    test_from_addr_and_cidr(address + "/8", address, "255.0.0.0");
    test_from_addr_and_cidr(address + "/9", address, "255.128.0.0");
    test_from_addr_and_cidr(address + "/10", address, "255.192.0.0");
    test_from_addr_and_cidr(address + "/11", address, "255.224.0.0");
    test_from_addr_and_cidr(address + "/12", address, "255.240.0.0");
    test_from_addr_and_cidr(address + "/13", address, "255.248.0.0");
    test_from_addr_and_cidr(address + "/14", address, "255.252.0.0");
    test_from_addr_and_cidr(address + "/15", address, "255.254.0.0");
    test_from_addr_and_cidr(address + "/16", address, "255.255.0.0");
    test_from_addr_and_cidr(address + "/17", address, "255.255.128.0");
    test_from_addr_and_cidr(address + "/18", address, "255.255.192.0");
    test_from_addr_and_cidr(address + "/19", address, "255.255.224.0");
    test_from_addr_and_cidr(address + "/20", address, "255.255.240.0");
    test_from_addr_and_cidr(address + "/21", address, "255.255.248.0");
    test_from_addr_and_cidr(address + "/22", address, "255.255.252.0");
    test_from_addr_and_cidr(address + "/23", address, "255.255.254.0");
    test_from_addr_and_cidr(address + "/24", address, "255.255.255.0");
    test_from_addr_and_cidr(address + "/25", address, "255.255.255.128");
    test_from_addr_and_cidr(address + "/26", address, "255.255.255.192");
    test_from_addr_and_cidr(address + "/27", address, "255.255.255.224");
    test_from_addr_and_cidr(address + "/28", address, "255.255.255.240");
    test_from_addr_and_cidr(address + "/29", address, "255.255.255.248");
    test_from_addr_and_cidr(address + "/30", address, "255.255.255.252");
    test_from_addr_and_cidr(address + "/31", address, "255.255.255.254");
    test_from_addr_and_cidr(address + "/32", address, "255.255.255.255");
}
