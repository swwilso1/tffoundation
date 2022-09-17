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
#define NEEDS_UNISTD_H
#define NEEDS_ASSERT_H
#define NEEDS_PTHREAD_H
#define NEEDS_CSTRING
#include "tfheaders.hpp"
#include "tfnetlink.hpp"

namespace TF::Foundation
{

    NetlinkAddress::NetlinkAddress() : NetworkAddress() {}

    NetlinkAddress::NetlinkAddress(struct sockaddr_nl * address) : NetworkAddress()
    {
        if (address == nullptr)
        {
            throw std::invalid_argument{"NetlinkAddress constructor passed a null pointer"};
        }
        auto address_ptr = reinterpret_cast<struct nlmsghdr *>(&m_address);
        std::memcpy(address_ptr, address, sizeof(struct nlmsghdr));
    }

    NetlinkAddress::NetlinkAddress(uint32_t process_id, uint32_t groups) : NetworkAddress()
    {
        auto address_ptr = reinterpret_cast<struct sockaddr_nl *>(&m_address);
        address_ptr->nl_family = AF_NETLINK;
        address_ptr->nl_pid = process_id;
        address_ptr->nl_groups = groups;
    }

    NetlinkAddress::NetlinkAddress(const void * p, size_type length) : NetworkAddress(p, length)
    {
        assert(length == sizeof(struct sockaddr_nl));
    }

    auto NetlinkAddress::get_process_id() const -> uint32_t
    {
        auto address_ptr = reinterpret_cast<const struct sockaddr_nl *>(&m_address);
        return address_ptr->nl_pid;
    }

    auto NetlinkAddress::address_length() const -> size_type
    {
        return sizeof(struct sockaddr_nl);
    }

    NetlinkMessage::NetlinkMessage(header_type * header)
    {
        if (header == nullptr)
        {
            throw std::invalid_argument{"NetlinkMessage constructor given null pointer"};
        }

        struct rtattr * attr_table[IFLA_MAX + 1];

        if (header->nlmsg_type == RTM_NEWLINK)
        {
            if (header->nlmsg_len < NLMSG_LENGTH(sizeof(struct ifinfomsg)))
            {
                throw std::runtime_error{"message length does not equal the length of an ifinfomsg"};
            }

            // First get the index.
            auto info_message = reinterpret_cast<struct ifinfomsg *>(NLMSG_DATA(header));
            m_index = info_message->ifi_index;

            // Next try and get the interface name
            extract_rtattr(attr_table, IFLA_MAX + 1, IFLA_RTA(info_message), IFLA_PAYLOAD(header));
            if (attr_table[IFLA_IFNAME] != nullptr)
            {
                m_name = string_type{reinterpret_cast<const char *>(RTA_DATA(attr_table[IFLA_IFNAME]))};
            }
        }
        else
        {
            // Should be an address.
            if (header->nlmsg_len < NLMSG_LENGTH(sizeof(struct ifaddrmsg)))
            {
                throw std::runtime_error{"message length does not equal length of ifaddrmsg"};
            }

            // Get the index of the interface for this address
            auto if_address = reinterpret_cast<struct ifaddrmsg *>(NLMSG_DATA(header));
            m_index = static_cast<decltype(m_index)>(if_address->ifa_index);

            // Now try to get address details
            extract_rtattr(attr_table, IFLA_MAX + 1, IFA_RTA(if_address),
                           header->nlmsg_len - NLMSG_LENGTH(sizeof(struct ifaddrmsg)));
            if (! attr_table[IFA_LOCAL])
            {
                attr_table[IFA_LOCAL] = attr_table[IFA_ADDRESS];
            }

            if (! attr_table[IFA_ADDRESS])
            {
                attr_table[IFA_ADDRESS] = attr_table[IFA_LOCAL];
            }

            if (attr_table[IFA_LOCAL])
            {
                auto address_length = RTA_PAYLOAD(attr_table[IFA_LOCAL]);
                m_address = address_type{static_cast<const void *>(RTA_DATA(attr_table[IFA_LOCAL])), address_length};
            }
        }
    }

    void NetlinkMessage::extract_rtattr(struct rtattr ** table, size_type table_length, struct rtattr * attr_to_process,
                                        size_type attr_length)
    {
        if (table == nullptr || attr_to_process == nullptr || table_length == 0 || attr_length == 0)
        {
            throw std::invalid_argument{"Extract_rtattr given invalid arguments"};
        }
        std::memset(table, 0, sizeof(struct rtattr *) * table_length);
        while (RTA_OK(attr_to_process, attr_length))
        {
            if (attr_to_process->rta_type < table_length)
            {
                table[attr_to_process->rta_type] = attr_to_process;
            }
            attr_to_process = RTA_NEXT(attr_to_process, attr_length);
        }
    }

    NetlinkKernelConnection::NetlinkKernelConnection()
    {
        m_my_id = static_cast<decltype(m_my_id)>(static_cast<uint64_t>(getpid()) | (pthread_self() << 16));
        m_kernel_address = address_type{m_my_id, RTMGRP_LINK};

        int buffer_size{static_cast<int>(s_response_buffer_size)};
        m_kernel_socket.set_option(SO_SNDBUF, SOL_SOCKET, &buffer_size, sizeof(int));
        m_kernel_socket.set_option(SO_RCVBUF, SOL_SOCKET, &buffer_size, sizeof(int));
        m_kernel_socket.bind(m_kernel_address);
    }

    NetlinkKernelConnection::~NetlinkKernelConnection() noexcept
    {
        m_kernel_socket.close();
    }

    auto NetlinkKernelConnection::get_interface_name_map() -> std::unordered_map<int32_t, string_type>
    {
        std::unordered_map<int32_t, string_type> interface_map{};

        auto get_interface_information_for_family = [this, &interface_map](int family) -> void {
            // Send the request for interface information
            request_info_from_kernel_for_address_family_by_type(family, RTM_GETLINK);

            // Process the results for interface information
            read_results_from_kernel_and_handle_message([&interface_map](const message_type & message) -> void {
                interface_map.insert(std::make_pair(message.get_index(), message.get_name()));
            });
        };

        get_interface_information_for_family(PF_INET);
        get_interface_information_for_family(PF_INET6);

        return interface_map;
    }

    auto NetlinkKernelConnection::get_address_map() -> std::unordered_map<int32_t, std::vector<ipaddress_type>>
    {
        std::unordered_map<int32_t, std::vector<ipaddress_type>> address_map{};

        auto get_address_information_for_family = [this, &address_map](int family) -> void {
            // Send the request for the address information
            request_info_from_kernel_for_address_family_by_type(family, RTM_GETADDR);

            read_results_from_kernel_and_handle_message([&address_map](const message_type & message) -> void {
                auto ipaddress = message.get_address();
                if (! address_map.contains(message.get_index()))
                {
                    std::vector<ipaddress_type> address_list{};
                    address_list.emplace_back(ipaddress);
                    address_map.insert(std::make_pair(message.get_index(), address_list));
                }
                else
                {
                    auto & address_list = address_map[message.get_index()];
                    address_list.emplace_back(ipaddress);
                }
            });
        };

        get_address_information_for_family(PF_INET);
        get_address_information_for_family(PF_INET6);

        return address_map;
    }

    void NetlinkKernelConnection::request_info_from_kernel_for_address_family_by_type(int family, int type)
    {
        struct
        {
            struct nlmsghdr header;
            struct rtgenmsg genmsg;
        } request{};

        // Use static_cast to disambiguate constructor
        address_type destination_address{static_cast<uint32_t>(0), 0};

        std::memset(&request, 0, sizeof(request));
        request.header.nlmsg_len = sizeof(request);
        request.header.nlmsg_type = static_cast<decltype(request.header.nlmsg_type)>(type);
        request.header.nlmsg_flags = NLM_F_ROOT | NLM_F_REQUEST | NLM_F_MATCH;
        request.header.nlmsg_pid = 0;
        request.header.nlmsg_seq = ++m_message_sequence_number;
        request.genmsg.rtgen_family = static_cast<decltype(request.genmsg.rtgen_family)>(family);

        m_kernel_socket.write_all_to(destination_address, &request, sizeof(request));
    }

    void NetlinkKernelConnection::read_results_from_kernel_and_handle_message(
        const std::function<void(const message_type & message)> & handler)
    {
        address_type netlink_address{};
        struct iovec iovec_thing
        {};
        struct msghdr message_header
        {};
        std::array<char, s_response_buffer_size> response_buffer{};

        std::memset(&message_header, 0, sizeof(message_header));
        message_header.msg_name =
            reinterpret_cast<void *>(const_cast<struct sockaddr *>(netlink_address.get_as_sockaddr()));
        message_header.msg_namelen =
            static_cast<decltype(message_header.msg_namelen)>(netlink_address.address_length());
        message_header.msg_iov = &iovec_thing;
        message_header.msg_iovlen = 1;

        iovec_thing.iov_base = response_buffer.data();
        while (true)
        {
            iovec_thing.iov_len = response_buffer.size();
            auto bytes_read = m_kernel_socket.read_message(message_header, 0);
            if (bytes_read == 0)
            {
                throw std::runtime_error{"EOF while reading response from Kernel."};
            }
            auto header_pointer = reinterpret_cast<struct nlmsghdr *>(response_buffer.data());
            while (NLMSG_OK(header_pointer, bytes_read))
            {
                if (netlink_address.get_process_id() != 0 || header_pointer->nlmsg_pid != m_my_id ||
                    header_pointer->nlmsg_seq != m_message_sequence_number)
                {
                    header_pointer = NLMSG_NEXT(header_pointer, bytes_read);
                    continue;
                }

                if (header_pointer->nlmsg_type == NLMSG_DONE)
                {
                    return;
                }

                if (header_pointer->nlmsg_type == NLMSG_ERROR)
                {
                    auto error_pointer = reinterpret_cast<struct nlmsgerr *>(NLMSG_DATA(header_pointer));
                    if (header_pointer->nlmsg_len >= NLMSG_LENGTH(sizeof(struct nlmsgerr)))
                    {
                        // Hmm: Why do we negate error here?
                        errno = -error_pointer->error;
                    }
                    throw std::system_error{errno, std::system_category(), "Error retrieving Kernel response"};
                }

                message_type kernel_message{header_pointer};
                handler(kernel_message);

                header_pointer = NLMSG_NEXT(header_pointer, bytes_read);
            }
            if (message_header.msg_flags & MSG_TRUNC)
            {
                continue;
            }
            if (bytes_read > 0)
            {
                throw std::runtime_error{"Message from Kernel has extra bytes"};
            }
        }
    }

} // namespace TF::Foundation
