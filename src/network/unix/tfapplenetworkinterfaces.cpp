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

#define NEEDS_ARRAY
#define NEEDS_MEMORY
#define NEEDS_ASSERT_H
#define NEEDS_SYS_TYPES_H
#define NEEDS_SYS_SOCKET_H
#define NEEDS_NETINET_IN_H
#define NEEDS_NET_IF_H
#define NEEDS_UNISTD_H
#define NEEDS_SYS_IOCTL_H
#define NEEDS_NETDB_H
#define NEEDS_ARPA_INET_H
#define NEEDS_FCNTL_H
#define NEEDS_SYS_FILE_H
#define NEEDS_SYS_TIME_H
#define NEEDS_SYS_SYSCTL_H
#define NEEDS_NET_IF_TYPES_H
#define NEEDS_NET_IF_DL_H
#define NEEDS_NET_ROUTE_H
#include "tfheaders.hpp"
#include "tfnetworkinterfaces.hpp"
#include "tflog.hpp"
#include "tfsocket.hpp"
#include "tfposixnetworkutilities.hpp"

namespace TF::Foundation
{

    template<typename T>
    auto length_roundup(T val, T size) -> T
    {
        return val & (size - 1) ? (1 + (val | (size - 1))) : val;
    }

    void NetworkInterfaces::load_interfaces()
    {
        std::array<struct sockaddr *, RTAX_MAX> sockaddr_array{};

        auto get_next_sockaddr = [](struct sockaddr * sa) -> struct sockaddr *
        {
            return reinterpret_cast<struct sockaddr *>(
                reinterpret_cast<caddr_t>(sa) +
                (sa->sa_len ? length_roundup(static_cast<size_t>(sa->sa_len), sizeof(uint32_t)) : sizeof(uint32_t)));
        };

        auto decode_address_into_table = [&sockaddr_array, &get_next_sockaddr](int address_selector,
                                                                               struct sockaddr * sa) -> void {
            for (decltype(sockaddr_array)::size_type i = 0; i < sockaddr_array.size(); i++)
            {
                sockaddr_array[i] = nullptr;
                if (address_selector & (1 << i))
                {
                    sockaddr_array[i] = sa;
                    sa = get_next_sockaddr(sa);
                }
            }
        };

        auto load_interface_and_addresses_for_family = [this, &decode_address_into_table,
                                                        &sockaddr_array](int family) -> void {
            std::array<int, 6> control{};
            string_type name{};
            size_t length{};

            control[0] = CTL_NET;
            control[1] = PF_ROUTE;
            control[2] = 0;
            control[3] = family;
            control[4] = NET_RT_IFLIST;
            control[5] = 0;

            // Request the length of the buffer needed to hold the interface/address data.
            auto api_result = sysctl(control.data(), control.size(), nullptr, &length, nullptr, 0);
            if (api_result < 0)
            {
                throw std::system_error{errno, std::system_category(), "Sysctl error"};
            }

            auto buffer = std::unique_ptr<char[], std::default_delete<char[]>>(new char[length]);

            decltype(length) retrieved_length{length};

            // Now get the interface and address data.
            api_result = sysctl(control.data(), control.size(), buffer.get(), &retrieved_length, nullptr, 0);
            if (api_result < 0)
            {
                throw std::system_error{errno, std::system_category(), "Sysctl error getting interface data"};
            }

            // The filled buffer length should be <= the allocated buffer length.
            assert(retrieved_length <= length);

            auto end_of_buffer = buffer.get() + retrieved_length;
            auto current_part_of_buffer = buffer.get();

            while (current_part_of_buffer < end_of_buffer)
            {
                // The next thing should be a struct if_msghdr object.
                auto message_header = reinterpret_cast<struct if_msghdr *>(current_part_of_buffer);

                // The header should contain an interface object.
                if (message_header->ifm_type == RTM_IFINFO)
                {
                    // The block might contain information on whether the interface is up and the name.
                    if (message_header->ifm_addrs & RTA_IFP)
                    {
                        if ((message_header->ifm_flags & IFF_UP) == 0)
                        {
                            // The interface is not up, so skip ahead to the next message header.
                            current_part_of_buffer += message_header->ifm_msglen;
                            continue;
                        }

                        auto info_structure = reinterpret_cast<struct sockaddr_dl *>(
                            current_part_of_buffer + message_header->ifm_msglen - sizeof(struct sockaddr_dl));

                        auto string_info = link_ntoa(info_structure);
                        if (string_info && std::strlen(string_info) > 0 && info_structure->sdl_nlen > 0)
                        {
                            try
                            {
                                name = string_type{info_structure->sdl_data, info_structure->sdl_nlen};
                            }
                            catch (...)
                            {
                                // Sometimes the information from the Kernel contains a name entry that is invalid.
                                // There is probably an error in the code as it tries to pass through the interface
                                // data structure, but for now, we catch the problem here, note it in the log, and
                                // keep traversing the list of messages.
                                LOG(LogPriority::Finer, "Unable to calculate interface name. Length parameter: %d",
                                    static_cast<int32_t>(info_structure->sdl_nlen));
                                current_part_of_buffer += message_header->ifm_msglen;
                                continue;
                            }
                            if (! m_interface_map.contains(name))
                            {
                                network_interface new_interface{name};

                                auto name_cstring = name.cStr();
                                auto index = if_nametoindex(name_cstring.get());
                                new_interface.set_index(static_cast<int32_t>(index));

                                m_interface_map[name] = new_interface;
                            }
                        }
                    }

                    // Move ahead to the next interface.
                    current_part_of_buffer += message_header->ifm_msglen;

                    // Now try to find address information.
                    while (current_part_of_buffer < end_of_buffer)
                    {
                        auto next_message_header = reinterpret_cast<struct if_msghdr *>(current_part_of_buffer);
                        if (next_message_header->ifm_type == RTM_NEWADDR)
                        {
                            // We found an address entry
                            auto address_header = reinterpret_cast<struct ifa_msghdr *>(current_part_of_buffer);
                            auto sa = reinterpret_cast<struct sockaddr *>(address_header + 1);
                            decode_address_into_table(address_header->ifam_addrs, sa);

                            auto address = sockaddr_array[RTAX_IFA];
                            if (address != nullptr)
                            {
                                IPAddress ip_address{address};
                                auto & interface = m_interface_map[name];

                                // We could cache the netmask value for a speedup on machines with many network
                                // interfaces.
                                auto netmask =
                                    get_netmask_for_interface_with_name(interface.get_name(), ip_address.get_family());
                                interface.add_address_and_netmask(IPAddressAndNetmask{ip_address, netmask});
                            }
                        }
                        else if (next_message_header->ifm_type == RTM_IFINFO)
                        {
                            break;
                        }

                        current_part_of_buffer += next_message_header->ifm_msglen;
                    }
                }
                else
                {
                    current_part_of_buffer += message_header->ifm_msglen;
                }
            }
        };

        load_interface_and_addresses_for_family(PF_INET);
        load_interface_and_addresses_for_family(PF_INET6);
    }

} // namespace TF::Foundation
