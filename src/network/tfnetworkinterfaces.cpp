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

#include "tfnetworkinterfaces.hpp"

namespace TF::Foundation
{

    auto NetworkInterfaces::get_interfaces() const -> std::vector<network_interface>
    {
        std::vector<network_interface> interfaces{};
        std::for_each(m_interface_map.cbegin(), m_interface_map.cend(),
                      [&interfaces](const std::pair<string_type, network_interface> & pair) -> void {
                          interfaces.emplace_back(pair.second);
                      });
        return interfaces;
    }

    auto NetworkInterfaces::get_interface_names() const -> std::vector<string_type>
    {
        std::vector<string_type> interface_names{};
        std::for_each(m_interface_map.cbegin(), m_interface_map.cend(),
                      [&interface_names](const std::pair<string_type, network_interface> & pair) -> void {
                          interface_names.emplace_back(pair.first);
                      });
        return interface_names;
    }

    auto NetworkInterfaces::get_interface_indexes() const -> std::vector<int32_t>
    {
        std::vector<int32_t> interface_indexes{};
        std::for_each(m_interface_map.cbegin(), m_interface_map.cend(),
                      [&interface_indexes](const std::pair<string_type, network_interface> & pair) -> void {
                          interface_indexes.emplace_back(pair.second.get_index());
                      });
        return interface_indexes;
    }

    auto NetworkInterfaces::get_interface_by_name(const string_type & name) -> std::optional<network_interface>
    {
        if (m_interface_map.contains(name))
        {
            return {m_interface_map[name]};
        }
        return {};
    }

    auto NetworkInterfaces::get_interface_by_index(int32_t index) -> std::optional<network_interface>
    {
        for (auto & pair : m_interface_map)
        {
            if (pair.second.get_index() == index)
            {
                return {pair.second};
            }
        }
        return {};
    }

    auto NetworkInterfaces::operator[](const string_type & name) -> network_interface &
    {
        return m_interface_map[name];
    }

    auto NetworkInterfaces::get_loopback_interface() const -> std::optional<network_interface>
    {
        for (auto & pair : m_interface_map)
        {
            if (pair.second.is_loopback_interface())
            {
                return {pair.second};
            }
        }
        return {};
    }

    auto NetworkInterfaces::get_interface_with_global_address() const -> std::optional<network_interface>
    {
        for (auto & pair : m_interface_map)
        {
            if (pair.second.has_global_address())
            {
                return {pair.second};
            }
        }
        return {};
    }

    auto NetworkInterfaces::get_wireless_interfaces() const -> std::vector<network_interface>
    {
        return get_interfaces_that_meet_criteria([](const network_interface & interface) -> bool {
            return interface.is_wireless_interface();
        });
    }

    auto NetworkInterfaces::get_interfaces_with_global_addresses() const -> std::vector<network_interface>
    {
        return get_interfaces_that_meet_criteria([](const network_interface & interface) -> bool {
            return interface.has_global_address();
        });
    }

    auto NetworkInterfaces::get_nonloopback_nonwireless_interfaces() const -> std::vector<network_interface>
    {
        return get_interfaces_that_meet_criteria([](const network_interface & interface) -> bool {
            return (! interface.is_loopback_interface()) && (! interface.is_wireless_interface());
        });
    }

    auto NetworkInterfaces::get_interfaces_that_meet_criteria(
        const std::function<bool(const network_interface & interface)> & criteria_checker) const
        -> std::vector<network_interface>
    {
        std::vector<network_interface> interfaces{};
        std::for_each(m_interface_map.cbegin(), m_interface_map.cend(),
                      [&criteria_checker, &interfaces](const std::pair<string_type, network_interface> & pair) -> void {
                          if (criteria_checker(pair.second))
                          {
                              interfaces.emplace_back(pair.second);
                          }
                      });
        return interfaces;
    }

} // namespace TF::Foundation
