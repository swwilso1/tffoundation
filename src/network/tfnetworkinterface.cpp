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

#define NEEDS_ALGORITHM
#include "tfheaders.hpp"
#include "tfnetworkinterface.hpp"

namespace TF::Foundation
{

    NetworkInterface::NetworkInterface(const string_type & name) : m_name{name} {}

    auto NetworkInterface::get_name() const -> string_type
    {
        return m_name;
    }

    void NetworkInterface::set_name(const string_type & name)
    {
        m_name = name;
    }

    auto NetworkInterface::get_index() const -> int
    {
        return m_index;
    }

    void NetworkInterface::set_index(int index)
    {
        m_index = index;
    }

    auto NetworkInterface::get_number_of_address() const -> size_type
    {
        return m_addresses.size();
    }

    void NetworkInterface::add_address(const address_type & address)
    {
        m_addresses.emplace_back(address);
    }

    auto NetworkInterface::get_addresses() const -> const address_list_type &
    {
        return m_addresses;
    }

    auto NetworkInterface::get_ipv4_addresses() const -> address_list_type
    {
        return get_addresses_by_criteria([](const address_type & address) -> bool {
            return address.is_ipv4_address();
        });
    }

    auto NetworkInterface::get_ipv6_addresses() const -> address_list_type
    {
        return get_addresses_by_criteria([](const address_type & address) -> bool {
            return address.is_ipv6_address();
        });
    }

    auto NetworkInterface::get_ipv4_address() const -> address_type
    {
        return get_address_by_criteria(
            [](const address_type & address) {
                return address.is_ipv4_address();
            },
            "no ipv4 address assigned to interface");
    }

    auto NetworkInterface::get_ipv6_address() const -> address_type
    {
        return get_address_by_criteria(
            [](const address_type & address) {
                return address.is_ipv6_address();
            },
            "no ipv6 address assigned to interface");
    }

    auto NetworkInterface::get_global_address() const -> address_type
    {
        return get_address_by_criteria(
            [](const address_type & address) {
                return address.is_global_address();
            },
            "no global address assigned to interface");
    }

    auto NetworkInterface::get_global_ipv4_address() const -> address_type
    {
        return get_address_by_criteria(
            [](const address_type & address) {
                return address.is_global_address() && address.is_ipv4_address();
            },
            "no global ipv4 address assigned to interface");
    }

    auto NetworkInterface::get_global_ipv6_address() const -> address_type
    {
        return get_address_by_criteria(
            [](const address_type & address) {
                return address.is_global_address() && address.is_ipv6_address();
            },
            "no global ipv6 address assigned to interface");
    }

    auto NetworkInterface::get_presentation_names() -> string_list_type
    {
        return get_names_by_criteria([](address_type & address) -> string_type {
            auto name_val = address.get_presentation_name();
            return *name_val;
        });
    }

    auto NetworkInterface::get_ipv4_presentation_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_type & address) -> bool {
                return address.is_ipv4_address();
            },
            [](address_type & address) -> string_type {
                auto name_val = address.get_presentation_name();
                return *name_val;
            });
    }

    auto NetworkInterface::get_ipv6_presentation_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_type & address) -> bool {
                return address.is_ipv6_address();
            },
            [](address_type & address) -> string_type {
                auto name_val = address.get_presentation_name();
                return *name_val;
            });
    }

    auto NetworkInterface::get_dns_names() -> string_list_type
    {
        return get_names_by_criteria([](address_type & address) -> string_type {
            auto name_val = address.get_dns_name();
            return *name_val;
        });
    }

    auto NetworkInterface::get_ipv4_dns_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_type & address) -> bool {
                return address.is_ipv4_address();
            },
            [](address_type & address) -> string_type {
                auto name_val = address.get_dns_name();
                return *name_val;
            });
    }

    auto NetworkInterface::get_ipv6_dns_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_type & address) -> bool {
                return address.is_ipv6_address();
            },
            [](address_type & address) -> string_type {
                auto name_val = address.get_dns_name();
                return *name_val;
            });
    }

    auto NetworkInterface::is_loopback_interface() const -> bool
    {
        return has_address_by_criteria([](const address_type & address) -> bool {
            return address.is_loopback_address();
        });
    }

    auto NetworkInterface::has_ipv4_address() const -> bool
    {
        return has_address_by_criteria([](const address_type & address) -> bool {
            return address.is_ipv4_address();
        });
    }

    auto NetworkInterface::has_ipv6_address() const -> bool
    {
        return has_address_by_criteria([](const address_type & address) -> bool {
            return address.is_ipv6_address();
        });
    }

    auto NetworkInterface::has_global_address() const -> bool
    {
        return has_address_by_criteria([](const address_type & address) -> bool {
            return address.is_global_address();
        });
    }

    auto NetworkInterface::has_global_ipv4_address() const -> bool
    {
        return has_address_by_criteria([](const address_type & address) -> bool {
            return address.is_global_address() && address.is_ipv4_address();
        });
    }

    auto NetworkInterface::has_global_ipv6_address() const -> bool
    {
        return has_address_by_criteria([](const address_type & address) -> bool {
            return address.is_global_address() && address.is_ipv6_address();
        });
    }

    auto NetworkInterface::get_address_by_criteria(const std::function<bool(const address_type & address)> & checker,
                                                   const string_type & not_found_message) const -> address_type
    {
        for (auto & address : m_addresses)
        {
            if (checker(address))
            {
                return address;
            }
        }

        auto message_as_cstr = not_found_message.cStr();

        throw std::runtime_error{message_as_cstr.get()};
    }

    auto NetworkInterface::get_addresses_by_criteria(
        const std::function<bool(const address_type & address)> & checker) const -> address_list_type
    {
        address_list_type collected_addresses{};
        std::for_each(m_addresses.cbegin(), m_addresses.cend(),
                      [&collected_addresses, checker](const address_type & address) -> void {
                          if (checker(address))
                          {
                              collected_addresses.emplace_back(address);
                          }
                      });
        return collected_addresses;
    }

    auto NetworkInterface::get_names_by_criteria(const std::function<string_type(address_type & address)> & criteria)
        -> string_list_type
    {
        string_list_type collected_names{};
        std::for_each(m_addresses.begin(), m_addresses.end(),
                      [&collected_names, criteria](address_type & address) -> void {
                          collected_names.emplace_back(criteria(address));
                      });
        return collected_names;
    }

    auto NetworkInterface::get_address_by_criteria_and_name_by_criteria(
        const std::function<bool(address_type & address)> & address_checker,
        const std::function<string_type(address_type & address)> & name_criteria) -> string_list_type
    {
        string_list_type collected_names{};
        std::for_each(m_addresses.begin(), m_addresses.end(),
                      [&collected_names, address_checker, name_criteria](address_type & address) -> void {
                          if (address_checker(address))
                          {
                              collected_names.emplace_back(name_criteria(address));
                          }
                      });
        return collected_names;
    }

    auto NetworkInterface::has_address_by_criteria(
        const std::function<bool(const address_type & address)> & checker) const -> bool
    {
        // Replace this code with return std::ranges::any_of(m_addresses, checker);
        for (auto & address : m_addresses)
        {
            if (checker(address))
            {
                return true;
            }
        }
        return false;
    }

} // namespace TF::Foundation
