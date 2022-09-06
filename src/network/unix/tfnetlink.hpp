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

#ifndef TFNETLINK_HPP
#define TFNETLINK_HPP

#define NEEDS_MEMORY
#define NEEDS_UNORDERED_MAP
#define NEEDS_FUNCTIONAL
#define NEEDS_LINUX_NETLINK_H
#define NEEDS_LINUX_RTNETLINK_H
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfnetworkaddress.hpp"
#include "tfinternetaddress.hpp"
#include "tfipaddress.hpp"
#include "tfsocket.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    /**
     * NetlinkAddress represents an address used to represent the user-land side of the connection to the Kernel
     * Netlink system.
     */
    class NetlinkAddress : public NetworkAddress
    {
    public:
        NetlinkAddress();

        /**
         * @brief constructor to initialize an address from an existing struct sockaddr_nl.
         * @param address
         */
        explicit NetlinkAddress(struct sockaddr_nl * address);

        /**
         * @brief constructor to initialize an address from an existing process and group.
         * @param process_id the process id
         * @param groups the group identifier
         */
        NetlinkAddress(uint32_t process_id, uint32_t groups);

        /**
         * @brief constructor for initializing an address from a raw struct sockaddr_nl byte array.
         * @param p the pointer to the byte array.
         * @param length the length of the byte array.
         */
        NetlinkAddress(const void * p, size_type length);

        /**
         * @brief method to get the process id used in the address.
         * @return the process id.
         */
        [[nodiscard]] auto get_process_id() const -> uint32_t;

        /**
         * @brief method to get the length of the binary address.
         * @return the length.
         */
        [[nodiscard]] auto address_length() const -> size_type override;
    };

    /**
     * NetlinkMessage holds an index value, a name, and/or an IP address sent
     * as a response by the Linux kernel in a struct nlmsghdr object. NetlinkMessage
     * decodes the header and extracts the index, name, and/or IP address.
     */
    class NetlinkMessage : public AllocatorInterface
    {
    public:
        using string_type = String;
        using header_type = struct nlmsghdr;
        using address_type = IPAddress;
        using size_type = Size_t;

        /**
         * @brief constructor to initialize the message from a struct nlmsghdr object.
         * @param header the pointer to the struct nlmsghdr object.
         */
        explicit NetlinkMessage(header_type * header);

        /**
         * @brief method to get the index.
         * @return the index.
         *
         * The index value represents the index value of the network inteface name, or the network interface index
         * to which the network interface belongs.
         */
        [[nodiscard]] auto get_index() const -> int32_t
        {
            return m_index;
        }

        /**
         * @brief method to get the interface name.
         * @return the interface name.
         */
        [[nodiscard]] auto get_name() const -> string_type
        {
            return m_name;
        }

        /**
         * @brief method to get the IP address.
         * @return the IP address.
         */
        [[nodiscard]] auto get_address() const -> address_type
        {
            return m_address;
        }

    private:
        int32_t m_index{0};
        string_type m_name{};
        address_type m_address{};

        /**
         * @brief helper method to decode the internals of the message from the Linux Kernel.
         * @param table the table of struct rtattr objects.
         * @param table_length the size of @e table.
         * @param attr_to_process the struct rtattr to decode.
         * @param attr_length the length of @e attr_to_process.
         */
        static void extract_rtattr(struct rtattr ** table, size_type table_length, struct rtattr * attr_to_process,
                                   size_type attr_length);
    };

    /**
     * NetlinkKernelConnection represents a connection to the Kernel's Netlink system. You can use
     * the object to query the Kernel for existing network interfaces, and existing network addresses
     * (IP addresses).
     */
    class NetlinkKernelConnection : public AllocatorInterface
    {
    public:
        using address_type = NetlinkAddress;
        using message_type = NetlinkMessage;
        using internet_address_type = InternetAddress;
        using ipaddress_type = IPAddress;
        using string_type = String;

        NetlinkKernelConnection();
        ~NetlinkKernelConnection() noexcept override;

        /**
         * @brief method to get a map of the network interface indexes to the network interface names.
         * @return the map of interface indexes to interface names.
         */
        auto get_interface_name_map() -> std::unordered_map<int32_t, string_type>;

        /**
         * @brief method to get a map of network interface indexes to the network addresses (usually IP addresses).
         * @return the map of interface indexes to network addresses.
         */
        auto get_address_map() -> std::unordered_map<int32_t, std::vector<ipaddress_type>>;

    private:
        Socket m_kernel_socket{PF_NETLINK, SOCK_RAW};
        address_type m_kernel_address{};
        uint32_t m_my_id{};
        uint32_t m_message_sequence_number{0};

        static constexpr int s_response_buffer_size = 32768; // 4096 * 8

        /**
         * @brief helper method to request information from the Kernel Netlink system for a given network address
         * family and message type.
         * @param family network address family (such as PF_INET, PF_INET6).
         * @param type type value specific to the Netlink system, usually a selector that chooses either network
         * interfaces or network addresses.
         */
        void request_info_from_kernel_for_address_family_by_type(int family, int type);

        /**
         * @brief helper method to read the results from a Kernel Netlink request.  The responses are decoded
         * into messages which are then handled specifically by the callback function handler.
         * @param handler the callback handler that dispatches the decoded message.
         */
        void read_results_from_kernel_and_handle_message(
            const std::function<void(const message_type & message)> & handler);
    };

} // namespace TF::Foundation

#endif // TFNETLINK_HPP
