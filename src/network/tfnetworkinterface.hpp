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

#ifndef TFNETWORKINTERFACE_HPP
#define TFNETWORKINTERFACE_HPP

#define NEEDS_OSTREAM
#define NEEDS_VECTOR
#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfipaddress.hpp"
#include "tfipaddressandnetmask.hpp"

namespace TF::Foundation
{

    /**
     * NetworkInterface provides a representation of a network interface on the system.
     */
    class NetworkInterface : public AllocatorInterface
    {
    public:
#pragma mark - public types
        using address_type = IPAddress;
        using address_and_netmask_type = IPAddressAndNetmask;
        using string_type = String;
        using size_type = Size_t;
        using address_list_type = std::vector<address_type>;
        using address_and_netmask_list_type = std::vector<address_and_netmask_type>;
        using string_list_type = std::vector<string_type>;

#pragma mark - constructors

        NetworkInterface() = default;

        explicit NetworkInterface(const string_type & name);

#pragma mark - destructor

        /**
         * @brief virtual destructor in case of inheritance.
         */
        virtual ~NetworkInterface();

#pragma mark - basic getters/setters

        /**
         * @brief method to get the interface name.
         * @return the interface name.
         */
        [[nodiscard]] auto get_name() const -> string_type;

        /**
         * @brief method to set the interface name.
         * @param name the new interface name.
         */
        void set_name(const string_type & name);

        /**
         * @brief method to get the interface index.
         * @return the index.
         */
        [[nodiscard]] auto get_index() const -> int;

        /**
         * @brief method to set the interface index.
         * @param index the new index.
         */
        void set_index(int index);

        /**
         * @brief method to get the number of addresses used by the interface.
         * @return the number of addresses.
         */
        [[nodiscard]] auto get_number_of_addresses() const -> size_type;

#pragma mark - add/get addresses and netmasks

        void add_address_and_netmask(const address_and_netmask_type & addr_mask);

        [[nodiscard]] auto get_addresses_and_netmasks() const -> address_and_netmask_list_type;

        [[nodiscard]] auto get_ipv4_addresses_and_netmasks() const -> address_and_netmask_list_type;

        [[nodiscard]] auto get_ipv6_addresses_and_netmasks() const -> address_and_netmask_list_type;

        [[nodiscard]] auto get_ipv4_address_and_netmask() const -> address_and_netmask_type;

        [[nodiscard]] auto get_ipv6_address_and_netmask() const -> address_and_netmask_type;

        [[nodiscard]] auto get_global_address_and_netmask() const -> address_and_netmask_type;

        [[nodiscard]] auto get_global_ipv4_address_and_netmask() const -> address_and_netmask_type;

        [[nodiscard]] auto get_global_ipv6_address_and_netmask() const -> address_and_netmask_type;

#pragma mark - add/get addresses

        /**
         * @brief method to add an IP address to the interface.
         * @param address the address.
         */
        void add_address(const address_type & address);

        /**
         * @brief method to get a list of IP addresses used by the interface.
         * @return the list of addresses.
         *
         * The list will contain both IPv4 and IPv6 addresses if they exist.
         */
        [[nodiscard]] auto get_addresses() const -> address_list_type;

        /**
         * @brief method to get a list of IPv4 addresses used by the interface.
         * @return the list of IPv4 addresses.
         */
        [[nodiscard]] auto get_ipv4_addresses() const -> address_list_type;

        /**
         * @brief method to get a list of IPv6 addresses used by the interface.
         * @return the list of IPv6 addresses.
         */
        [[nodiscard]] auto get_ipv6_addresses() const -> address_list_type;

        /**
         * @brief method to get the first IPv4 address used by the interface.
         * @return the first IPv4 address used by the interface.
         */
        [[nodiscard]] auto get_ipv4_address() const -> address_type;

        /**
         * @brief method to get the first IPv6 address used by the interface.
         * @return the first IPv6 address used by the interface.
         */
        [[nodiscard]] auto get_ipv6_address() const -> address_type;

        /**
         * @brief method to get a global address (globally unique) used by the interface.
         * @return the first global address used by the interface.
         *
         * The address may be either IPv4 or IPv6.
         */
        [[nodiscard]] auto get_global_address() const -> address_type;

        /**
         * @brief method to get the first global IPv4 address used by the interface.
         * @return the first global IPv4 address.
         */
        [[nodiscard]] auto get_global_ipv4_address() const -> address_type;

        /**
         * @brief method to get the first global IPv6 address used by the interface.
         * @return the first global IPv6 address.
         */
        [[nodiscard]] auto get_global_ipv6_address() const -> address_type;

#pragma mark - get address names

        /**
         * @brief method to get a list of IP address presentation names used by the interface.
         * @return the list of IP address presentation names.
         */
        [[nodiscard]] auto get_presentation_names() -> string_list_type;

        /**
         * @brief method to get a list of IPv4 address presentation names used by the interface.
         * @return the list of IPv4 address presentation names.
         */
        [[nodiscard]] auto get_ipv4_presentation_names() -> string_list_type;

        /**
         * @brief method to get a list of IPv6 address presentation names used by the interface.
         * @return the list of IPv6 address presentation names.
         */
        [[nodiscard]] auto get_ipv6_presentation_names() -> string_list_type;

        /**
         * @brief method to get a list of reverse DNS names for the IP addresses used by the interface.
         * @return the list of reverse DNS names.
         *
         * The DNS names may represent IPv4 and IPv6 addresses.
         */
        [[nodiscard]] auto get_dns_names() -> string_list_type;

        /**
         * @brief method to get a list of reverse DNS names for the IPv4 addresses used by the interface.
         * @return the list of reverse DNS names.
         */
        [[nodiscard]] auto get_ipv4_dns_names() -> string_list_type;

        /**
         * @brief method to get a list of reverse DNS names for the IPv6 addresses used by the interface.
         * @return the list of reverse DNS names.
         */
        [[nodiscard]] auto get_ipv6_dns_names() -> string_list_type;

#pragma mark - feature test methods

        /**
         * @brief method to check if the interface is a loopback interface.
         * @return true if the interface is a loopback interface.
         */
        [[nodiscard]] auto is_loopback_interface() const -> bool;

        /**
         * @brief method to check if the interface has valid IPv4 addresses.
         * @return true if the interface has IPv4 addresses.
         */
        [[nodiscard]] auto has_ipv4_address() const -> bool;

        /**
         * @brief method to check if the interface has valid IPv6 addresses.
         * @return true if the interface has IPv6 addresses.
         */
        [[nodiscard]] auto has_ipv6_address() const -> bool;

        /**
         * @brief method to check if the interface has a globally unique IP address.
         * @return true if the interface has a globally unique address.
         *
         * The address may be an IPv4 or IPv6 address.
         */
        [[nodiscard]] auto has_global_address() const -> bool;

        /**
         * @brief method to check if the interface has a globally unique IPv4 address.
         * @return true if the interface has a globally unique address.
         */
        [[nodiscard]] auto has_global_ipv4_address() const -> bool;

        /**
         * @brief method to check if the interface has a globally unique IPv6 address.
         * @return true if the interface has a globally unique address.
         */
        [[nodiscard]] auto has_global_ipv6_address() const -> bool;

#pragma mark - methods to check interface type

        /**
         * @brief method to check if the interface is a wireless interface.
         * @return true if the interface is wireless.
         */
        [[nodiscard]] auto is_wireless_interface() const -> bool;

#pragma mark - clear method

        /**
         * @brief method to reset an interface contents
         */
        void clear();

#pragma mark - stream related methods

        /**
         * @brief method for writing a network interface to a stream.
         * @param o the stream.
         * @return @e o after writing the interface details to the stream.
         */
        [[nodiscard]] auto description(std::ostream & o) const -> std::ostream &;

    private:
        string_type m_name{};
        address_and_netmask_list_type m_address_and_netmasks{};
        int m_index{};

        /**
         * @brief helper function to locate the first address that meets the criteria represented by the callback
         * function checker.
         * @param checker the callback function that checks each address and returns true if the address/netmask meets
         * some criteria.
         * @param not_found_message message to generate in an exception when the address is not found.
         * @return the address found that matches the criteria (from @e checker).
         */
        [[nodiscard]] auto get_address_by_criteria(
            const std::function<bool(const address_and_netmask_type & addr_mask)> & checker,
            const string_type & not_found_message) const -> address_and_netmask_type;

        /**
         * @brief helper function locate all addresses in the interface that meet the criteria represented by the
         * callback function @e checker.
         * @param checker the callback function that checks each address/netmask and returns true if the address/mask
         * meets some criteria.
         * @return a list of the addresses/masks that match the criteria (from @e checker).
         */
        [[nodiscard]] auto get_addresses_by_criteria(
            const std::function<bool(const address_and_netmask_type & addr_mask)> & checker) const
            -> address_and_netmask_list_type;

        /**
         * @brief method to get a list of names generated by the criteria function.
         * @param criteria the function that generates a string from the address/netmask argument.
         * @return the list of strings generated by calling @e criteria for each address.
         */
        [[nodiscard]] auto get_names_by_criteria(
            const std::function<string_type(address_and_netmask_type & addr_mask)> & criteria) -> string_list_type;

        /**
         * @brief helper method to return a list of string values for a list of addresses specified by address_criteria
         * and the string values for those addresses generated by name_criteria.
         * @param address_checker the callback function that returns true when an address/netmask matches the selection
         * criteria.
         * @param name_criteria the callback function that converts the address/netmask into a string value to include
         * in the result list for the selected address.
         * @return the name values generated from the selected list of addresses.
         */
        [[nodiscard]] auto get_address_by_criteria_and_name_by_criteria(
            const std::function<bool(address_and_netmask_type & addr_mask)> & address_checker,
            const std::function<string_type(address_and_netmask_type & addr_mask)> & name_criteria) -> string_list_type;

        /**
         * @brief helper method to check if an interface has an address that meets some criteria encapsulated in
         * checker.
         * @param checker the callback function that returns true if the address/netmask meets its internal selection
         * criteria.
         * @return true if the interface has at least one address that meets the criteria.
         */
        [[nodiscard]] auto has_address_by_criteria(
            const std::function<bool(const address_and_netmask_type & addr_mask)> & checker) const -> bool;
    };

    /**
     * @brief overloaded << operator for network interface objects.
     * @param o the stream object.
     * @param i the network interface object.
     * @return @e o after writing @e i to the stream.
     */
    std::ostream & operator<<(std::ostream & o, const NetworkInterface & i);

} // namespace TF::Foundation

#endif // TFNETWORKINTERFACE_HPP
