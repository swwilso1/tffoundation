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

namespace TF::Foundation
{

    class NetworkInterface : public AllocatorInterface
    {
    public:
#pragma mark - public types
        using address_type = IPAddress;
        using string_type = String;
        using size_type = Size_t;
        using address_list_type = std::vector<address_type>;
        using string_list_type = std::vector<string_type>;

#pragma mark - constructors

        NetworkInterface() = default;

        explicit NetworkInterface(const string_type & name);

#pragma mark - basic getters/setters

        [[nodiscard]] auto get_name() const -> string_type;

        void set_name(const string_type & name);

        [[nodiscard]] auto get_index() const -> int;

        void set_index(int index);

        [[nodiscard]] auto get_number_of_address() const -> size_type;

#pragma mark - add/get addresses

        void add_address(const address_type & address);

        [[nodiscard]] auto get_ipv4_addresses() const -> address_list_type;

        [[nodiscard]] auto get_ipv6_addresses() const -> address_list_type;

        [[nodiscard]] auto get_ipv4_address() const -> address_type;

        [[nodiscard]] auto get_ipv6_address() const -> address_type;

#pragma mark - get address names

        [[nodiscard]] auto get_presentation_names() -> string_list_type;

        [[nodiscard]] auto get_ipv4_presentation_names() -> string_list_type;

        [[nodiscard]] auto get_ipv6_presentation_names() -> string_list_type;

        [[nodiscard]] auto get_dns_names() -> string_list_type;

        [[nodiscard]] auto get_ipv4_dns_names() -> string_list_type;

        [[nodiscard]] auto get_ipv6_dns_names() -> string_list_type;

#pragma mark - feature test methods

        [[nodiscard]] auto is_loopback_interface() const -> bool;

        [[nodiscard]] auto has_ipv4_address() const -> bool;

        [[nodiscard]] auto has_ipv6_address() const -> bool;

#pragma mark - stream related methods

        std::ostream & description(std::ostream & o) const;

    private:
        string_type m_name{};
        address_list_type m_addresses{};
        int m_index{};

        [[nodiscard]] auto get_address_by_criteria(const std::function<bool(const address_type & address)> & checker,
                                                   const string_type & not_found_message) const -> address_type;

        [[nodiscard]] auto get_addresses_by_criteria(
            const std::function<bool(const address_type & address)> & checker) const -> address_list_type;

        [[nodiscard]] auto get_names_by_criteria(const std::function<string_type(address_type & address)> & criteria)
            -> string_list_type;

        [[nodiscard]] auto get_address_by_criteria_and_name_by_criteria(
            const std::function<bool(address_type & address)> & address_checker,
            const std::function<string_type(address_type & address)> & name_criteria) -> string_list_type;

        [[nodiscard]] auto has_address_by_criteria(
            const std::function<bool(const address_type & address)> & checker) const -> bool;
    };

    std::ostream & operator<<(std::ostream & o, const NetworkInterface & i);

} // namespace TF::Foundation

#endif // TFNETWORKINTERFACE_HPP
