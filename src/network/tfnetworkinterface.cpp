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
#include "tfvector.hxx"

namespace TF::Foundation
{

    NetworkInterface::NetworkInterface(const string_type & name) : m_name{name} {}

    NetworkInterface::~NetworkInterface() {}

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

    auto NetworkInterface::get_number_of_addresses() const -> size_type
    {
        return m_address_and_netmasks.size();
    }

    void NetworkInterface::add_address_and_netmask(const address_and_netmask_type & addr_mask)
    {
        m_address_and_netmasks.emplace_back(addr_mask);
    }

    auto NetworkInterface::get_addresses_and_netmasks() const -> address_and_netmask_list_type
    {
        return m_address_and_netmasks;
    }

    auto NetworkInterface::get_ipv4_addresses_and_netmasks() const -> address_and_netmask_list_type
    {
        return get_addresses_by_criteria([](const address_and_netmask_type & addr_map) -> bool {
            return addr_map.address.is_ipv4_address();
        });
    }

    auto NetworkInterface::get_ipv6_addresses_and_netmasks() const -> address_and_netmask_list_type
    {
        return get_addresses_by_criteria([](const address_and_netmask_type & addr_map) -> bool {
            return addr_map.address.is_ipv6_address();
        });
    }

    auto NetworkInterface::get_ipv4_address_and_netmask() const -> address_and_netmask_type
    {
        return get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_ipv4_address();
            },
            "no ipv4 address assigned to interface");
    }

    auto NetworkInterface::get_ipv6_address_and_netmask() const -> address_and_netmask_type
    {
        return get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_ipv6_address();
            },
            "no ipv6 address assigned to interface");
    }

    auto NetworkInterface::get_global_address_and_netmask() const -> address_and_netmask_type
    {
        return get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_global_address();
            },
            "no global address assigned to interface");
    }

    auto NetworkInterface::get_global_ipv4_address_and_netmask() const -> address_and_netmask_type
    {
        return get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_global_address() && addr_mask.address.is_ipv4_address();
            },
            "no global ipv4 address assigned to interface");
    }

    auto NetworkInterface::get_global_ipv6_address_and_netmask() const -> address_and_netmask_type
    {
        return get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_global_address() && addr_mask.address.is_ipv6_address();
            },
            "no global ipv6 address assigned to interface");
    }

    void NetworkInterface::add_address(const address_type & address)
    {
        m_address_and_netmasks.emplace_back(address_and_netmask_type{address});
    }

    auto NetworkInterface::get_addresses() const -> address_list_type
    {
        address_list_type ip_addr_list{};
        std::for_each(m_address_and_netmasks.cbegin(), m_address_and_netmasks.cend(),
                      [&ip_addr_list](const address_and_netmask_type & addr_mask) -> void {
                          ip_addr_list.emplace_back(addr_mask.address);
                      });
        return ip_addr_list;
    }

    auto NetworkInterface::get_ipv4_addresses() const -> address_list_type
    {
        address_list_type ipv4_addresses;
        auto addr_mask_list = get_addresses_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_ipv4_address();
        });
        std::for_each(addr_mask_list.cbegin(), addr_mask_list.cend(),
                      [&ipv4_addresses](const address_and_netmask_type & addr_map) -> void {
                          ipv4_addresses.emplace_back(addr_map.address);
                      });
        return ipv4_addresses;
    }

    auto NetworkInterface::get_ipv6_addresses() const -> address_list_type
    {
        address_list_type ipv6_addresses{};
        auto addr_mask_list = get_addresses_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_ipv6_address();
        });
        std::for_each(addr_mask_list.cbegin(), addr_mask_list.cend(),
                      [&ipv6_addresses](const address_and_netmask_type & addr_map) -> void {
                          ipv6_addresses.emplace_back(addr_map.address);
                      });
        return ipv6_addresses;
    }

    auto NetworkInterface::get_ipv4_address() const -> address_type
    {
        auto addr_mask = get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_ipv4_address();
            },
            "no ipv4 address assigned to interface");
        return addr_mask.address;
    }

    auto NetworkInterface::get_ipv6_address() const -> address_type
    {
        auto addr_mask = get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_ipv6_address();
            },
            "no ipv6 address assigned to interface");
        return addr_mask.address;
    }

    auto NetworkInterface::get_global_address() const -> address_type
    {
        auto addr_mask = get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_global_address();
            },
            "no global address assigned to interface");
        return addr_mask.address;
    }

    auto NetworkInterface::get_global_ipv4_address() const -> address_type
    {
        auto addr_mask = get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_global_address() && addr_mask.address.is_ipv4_address();
            },
            "no global ipv4 address assigned to interface");
        return addr_mask.address;
    }

    auto NetworkInterface::get_global_ipv6_address() const -> address_type
    {
        auto addr_mask = get_address_by_criteria(
            [](const address_and_netmask_type & addr_mask) {
                return addr_mask.address.is_global_address() && addr_mask.address.is_ipv6_address();
            },
            "no global ipv6 address assigned to interface");
        return addr_mask.address;
    }

    auto NetworkInterface::get_presentation_names() -> string_list_type
    {
        return get_names_by_criteria([](address_and_netmask_type & addr_mask) -> string_type {
            auto name_val = addr_mask.address.get_presentation_name();
            return *name_val;
        });
    }

    auto NetworkInterface::get_ipv4_presentation_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_and_netmask_type & addr_mask) -> bool {
                return addr_mask.address.is_ipv4_address();
            },
            [](address_and_netmask_type & addr_mask) -> string_type {
                auto name_val = addr_mask.address.get_presentation_name();
                return *name_val;
            });
    }

    auto NetworkInterface::get_ipv6_presentation_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_and_netmask_type & addr_mask) -> bool {
                return addr_mask.address.is_ipv6_address();
            },
            [](address_and_netmask_type & addr_mask) -> string_type {
                auto name_val = addr_mask.address.get_presentation_name();
                return *name_val;
            });
    }

    auto NetworkInterface::get_dns_names() -> string_list_type
    {
        return get_names_by_criteria([](address_and_netmask_type & addr_mask) -> string_type {
            auto name_val = addr_mask.address.get_dns_name();
            return *name_val;
        });
    }

    auto NetworkInterface::get_ipv4_dns_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_and_netmask_type & addr_mask) -> bool {
                return addr_mask.address.is_ipv4_address();
            },
            [](address_and_netmask_type & addr_mask) -> string_type {
                auto name_val = addr_mask.address.get_dns_name();
                return *name_val;
            });
    }

    auto NetworkInterface::get_ipv6_dns_names() -> string_list_type
    {
        return get_address_by_criteria_and_name_by_criteria(
            [](address_and_netmask_type & addr_mask) -> bool {
                return addr_mask.address.is_ipv6_address();
            },
            [](address_and_netmask_type & addr_mask) -> string_type {
                auto name_val = addr_mask.address.get_dns_name();
                return *name_val;
            });
    }

    auto NetworkInterface::is_loopback_interface() const -> bool
    {
        return has_address_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_loopback_address();
        });
    }

    auto NetworkInterface::has_ipv4_address() const -> bool
    {
        return has_address_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_ipv4_address();
        });
    }

    auto NetworkInterface::has_ipv6_address() const -> bool
    {
        return has_address_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_ipv6_address();
        });
    }

    auto NetworkInterface::has_global_address() const -> bool
    {
        return has_address_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_global_address();
        });
    }

    auto NetworkInterface::has_global_ipv4_address() const -> bool
    {
        return has_address_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_global_address() && addr_mask.address.is_ipv4_address();
        });
    }

    auto NetworkInterface::has_global_ipv6_address() const -> bool
    {
        return has_address_by_criteria([](const address_and_netmask_type & addr_mask) -> bool {
            return addr_mask.address.is_global_address() && addr_mask.address.is_ipv6_address();
        });
    }

    auto NetworkInterface::description(std::ostream & o) const -> std::ostream &
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("NetworkInterface");
            formatter->addClassMember("string_type", "m_name", m_name);
            formatter->addClassMember("m_index", m_index);
            formatter->addClassMember("m_address_and_netmask", m_address_and_netmasks);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    auto NetworkInterface::get_address_by_criteria(
        const std::function<bool(const address_and_netmask_type & addr_mask)> & checker,
        const string_type & not_found_message) const -> address_and_netmask_type
    {
        for (auto & address : m_address_and_netmasks)
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
        const std::function<bool(const address_and_netmask_type & addr_mask)> & checker) const
        -> address_and_netmask_list_type
    {
        address_and_netmask_list_type collected_addresses{};
        std::for_each(m_address_and_netmasks.cbegin(), m_address_and_netmasks.cend(),
                      [&collected_addresses, checker](const address_and_netmask_type & addr_mask) -> void {
                          if (checker(addr_mask))
                          {
                              collected_addresses.emplace_back(addr_mask);
                          }
                      });
        return collected_addresses;
    }

    auto NetworkInterface::get_names_by_criteria(
        const std::function<string_type(address_and_netmask_type & addr_mask)> & criteria) -> string_list_type
    {
        string_list_type collected_names{};
        std::for_each(m_address_and_netmasks.begin(), m_address_and_netmasks.end(),
                      [&collected_names, criteria](address_and_netmask_type & addr_mask) -> void {
                          collected_names.emplace_back(criteria(addr_mask));
                      });
        return collected_names;
    }

    auto NetworkInterface::get_address_by_criteria_and_name_by_criteria(
        const std::function<bool(address_and_netmask_type & addr_mask)> & address_checker,
        const std::function<string_type(address_and_netmask_type & addr_mask)> & name_criteria) -> string_list_type
    {
        string_list_type collected_names{};
        std::for_each(m_address_and_netmasks.begin(), m_address_and_netmasks.end(),
                      [&collected_names, address_checker, name_criteria](address_and_netmask_type & addr_mask) -> void {
                          if (address_checker(addr_mask))
                          {
                              collected_names.emplace_back(name_criteria(addr_mask));
                          }
                      });
        return collected_names;
    }

    auto NetworkInterface::has_address_by_criteria(
        const std::function<bool(const address_and_netmask_type & addr_mask)> & checker) const -> bool
    {
        // Replace this code with return std::ranges::any_of(m_addresses, checker);
        for (auto & addr_mask : m_address_and_netmasks)
        {
            if (checker(addr_mask))
            {
                return true;
            }
        }
        return false;
    }

} // namespace TF::Foundation
