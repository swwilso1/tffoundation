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

#define NEEDS_CSTRING
#define NEEDS_SYS_IOCTL_H
#define NEEDS_NET_IF_H
#define NEEDS_ARPA_INET_H
#include "tfheaders.hpp"
#include "tfnetworkinterfaces.hpp"
#include "tfnetlink.hpp"
#include "tflog.hpp"
#include "tfsocket.hpp"

namespace TF::Foundation
{

    void NetworkInterfaces::load_interfaces()
    {
        NetlinkKernelConnection netlink_connection{};

        auto network_interface_map = netlink_connection.get_interface_name_map();
        auto address_map = netlink_connection.get_address_map();

        // Load the addresses that correspond with interfaces into the interface map.
        std::for_each(address_map.cbegin(), address_map.cend(),
                      [this, &network_interface_map](const std::pair<int32_t, std::vector<IPAddress>> & pair) -> void {
                          // First look up the interface name from the interface index which is in pair.first.
                          if (! network_interface_map.contains(pair.first))
                          {
                              LOG(LogPriority::Info, "Network index %d does not map to an interface name", pair.first)
                              return;
                          }

                          auto & interface_name = network_interface_map[pair.first];

                          // Now check to see if we have created a NetworkInterface object for this name.
                          if (! m_interface_map.contains(interface_name))
                          {
                              auto interface = network_interface{interface_name};
                              interface.set_index(pair.first);
                              m_interface_map[interface_name] = interface;
                          }

                          auto & interface = m_interface_map[interface_name];

                          // Now for each interface in the list in pair.second, add it to the interface.
                          std::for_each(
                              pair.second.cbegin(), pair.second.cend(),
                              [&interface](const IPAddress & address) -> void {
                                  auto family_type = address.is_ipv4_address() ? PF_INET : PF_INET6;
                                  struct ifreq ifr
                                  {};
                                  Socket s{family_type, SOCK_DGRAM};

                                  std::memset(&ifr, 0, sizeof(struct ifreq));
                                  ifr.ifr_addr.sa_family = static_cast<sa_family_t>(family_type);

                                  auto iface_name = interface.get_name();
                                  auto iface_name_cstr = iface_name.cStr();
                                  auto length =
                                      iface_name.length() > (IFNAMSIZ - 1) ? (IFNAMSIZ - 1) : iface_name.length();
                                  std::memcpy(ifr.ifr_name, iface_name_cstr.get(), length);
                                  auto api_result = ioctl(s.get_raw_socket(), SIOCGIFNETMASK, &ifr);
                                  if (api_result >= 0)
                                  {
                                      IPAddress netmask{&ifr.ifr_addr};
                                      interface.add_address_and_netmask(IPAddressAndNetmask{address, netmask});
                                  }
                                  else
                                  {
                                      interface.add_address(address);
                                  }
                              });
                      });

        // Now see if there are any leftover interfaces that do not have addresses.  If so we want to add
        // the empty interface.
        std::for_each(network_interface_map.cbegin(), network_interface_map.cend(),
                      [this](const std::pair<int32_t, string_type> & pair) -> void {
                          if (! m_interface_map.contains(pair.second))
                          {
                              auto interface = network_interface{pair.second};
                              interface.set_index(pair.first);
                              m_interface_map[pair.second] = interface;
                          }
                      });
    }

} // namespace TF::Foundation