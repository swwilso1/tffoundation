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

#ifndef TFNETWORKINTERFACES_HPP
#define TFNETWORKINTERFACES_HPP

#define NEEDS_UNORDERED_MAP
#define NEEDS_FUNCTIONAL
#define NEEDS_VECTOR
#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfnetworkinterface.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    /**
     * NetworkInterfaces provides a simple API for querying the network subsystem about network
     * interfaces on a machine.
     */
    class NetworkInterfaces
    {
    public:
        using string_type = String;
        using network_interface = NetworkInterface;

        /**
         * @brief method to get a list of interface objects.
         * @return the list of interface objects.
         */
        [[nodiscard]] auto get_interfaces() const -> std::vector<network_interface>;

        /**
         * @brief method to get a vector of the names of the interfaces on a system.
         * @return the vector of interface names.
         */
        [[nodiscard]] auto get_interface_names() const -> std::vector<string_type>;

        /**
         * @brief method to get a vector of the index numbers of the interfaces on a system.
         * @return the vector of interface indexes.
         */
        [[nodiscard]] auto get_interface_indexes() const -> std::vector<int32_t>;

        /**
         * @brief method to get a specific interface by name.
         * @param name the name of the interface.
         * @return std::optional that will contain the network interface on success.
         */
        [[nodiscard]] auto get_interface_by_name(const string_type & name) -> std::optional<network_interface>;

        /**
         * @brief method to get a specific interface by index number.
         * @param index the index number.
         * @return std::optional that will contain the network interface on success.
         */
        [[nodiscard]] auto get_interface_by_index(int32_t index) -> std::optional<network_interface>;

        /**
         * @brief method to get a specific network interface by name without the overhead of
         * checking whether the interface exists.
         * @param name the network interface name.
         * @return a network interface object.  The object may be empty if no interface with the name
         * @e name exists.
         */
        [[nodiscard]] auto operator[](const string_type & name) -> network_interface &;

        /**
         * @brief method to get the system loopback interface.
         * @return a std::optional that will contain the loopback interface on success.
         */
        [[nodiscard]] auto get_loopback_interface() const -> std::optional<network_interface>;

        /**
         * @brief method to get the first network interface with a global IP address.
         * @return a std::optional that will contain the network interface with a global IP address on success.
         */
        [[nodiscard]] auto get_interface_with_global_address() const -> std::optional<network_interface>;

        /**
         * @brief method to get a vector of the wireless network interfaces on the system.
         * @return the vector of wireless interfaces.
         */
        [[nodiscard]] auto get_wireless_interfaces() const -> std::vector<network_interface>;

        /**
         * @brief method to get a vector of the network interfaces with global IP addresses on the system.
         * @return the vector of network interfaces with global IP addresses.
         */
        [[nodiscard]] auto get_interfaces_with_global_addresses() const -> std::vector<network_interface>;

        /**
         * @brief method to get the non-loopback, non-wireless interfaces on the system.
         * @return the vector of non-loopback, non-wireless interfaces.
         */
        [[nodiscard]] auto get_nonloopback_nonwireless_interfaces() const -> std::vector<network_interface>;

        /**
         * @brief method to load the network interfaces from the system.
         */
        void load_interfaces();

    private:
        std::unordered_map<string_type, network_interface> m_interface_map{};

        /**
         * @brief helper method to create a list of network interfaces that match a criteria provided by
         * the criteria_checker callback function.
         * @param criteria_checker callback function called for every interface on the system. The criteria_checker
         * callback should return true to include the interface in the returned vector.
         * @return the vector of selected network interfaces based on criteria_checker.
         */
        auto get_interfaces_that_meet_criteria(
            const std::function<bool(const network_interface & interface)> & criteria_checker) const
            -> std::vector<network_interface>;
    };

} // namespace TF::Foundation

#endif // TFNETWORKINTERFACES_HPP
