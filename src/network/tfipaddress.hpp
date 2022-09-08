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

#ifndef TFIPADDRESS_HPP
#define TFIPADDRESS_HPP

#define NEEDS_MEMORY
#define NEEDS_NETINET_IN_H
#define NEEDS_OSTREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfnetworkinitialization.hpp"

namespace TF::Foundation
{

    class IPAddress : public AllocatorInterface, NetworkInitializer
    {
    public:
#pragma mark - class types

        using string_type = String;

        using string_pointer_type = std::shared_ptr<string_type>;

#pragma mark - class constructors

        /**
         * @brief Default constructor.
         * Initializes address to 0.0.0.0.
         */
        IPAddress();

        /**
         * @brief initializer from struct in_addr.
         * @param ipv4_address the address, should be in network byte order.
         */
        explicit IPAddress(const struct in_addr & ipv4_address);

        /**
         * @brief initializer from struct in6_addr.
         * @param ipv6_address the address, should be in network byte order.
         */
        explicit IPAddress(const struct in6_addr & ipv6_address);

        /**
         * @rief constructor from void p and length.
         * @param p the pointer to the address buffer
         * @param length the length of the address buffer.
         *
         * @e p must point to either a struct in_addr or struct in6_addr object.
         */
        IPAddress(const void * p, size_type length);

        /**
         * @brief constructor from a sockaddr pointer.
         * @param sa the struct sockaddr pointer.
         *
         * Will throw an exception if @e sa is nullptr;
         */
        IPAddress(const struct sockaddr * sa);

        /**
         * @brief copy constructor
         * @param a the other address.
         */
        IPAddress(const IPAddress & a);

#pragma mark - static methods

        /**
         * @brief create an IPAddress from a string value.
         * @param value the string value containing a presentation address form of an IP address.
         * @return the IPAddress or throw std::runtime error if the method cannot create an IPAddress.
         */
        static auto address_from_string(const string_type & value) -> IPAddress;

#pragma mark - assignment operators

        /**
         * @brief IPAddress assignment operator.
         * @param a the address
         * @return the current address updated with the contents of @e a.
         */
        auto operator=(const IPAddress & a) -> IPAddress &;

        /**
         * @brief struct in_addr assignment operator.
         * @param ipv4_address the in_addr object.
         * @return the current address object that now contains @e ipv4_address
         */
        auto operator=(const struct in_addr & ipv4_address) -> IPAddress &;

        /**
         * @brief struct in6_addr assignment operator.
         * @param ipv6_address the in6_addr object.
         * @return the current address object that now contains @e ipv6_address
         */
        auto operator=(const struct in6_addr & ipv6_address) -> IPAddress &;

#pragma mark - comparison operators

        /**
         * @brief equality operator
         * @param a the other address object.
         * @return true if the two addresses are equal.
         */
        auto operator==(const IPAddress & a) const -> bool;

        /**
         * @brief in-equality operator
         * @param a the other address object.
         * @return true if the two addresses are equal.
         */
        auto operator!=(const IPAddress & a) const -> bool;

        /**
         * @brief less-than operator
         * @param a the other address object.
         * @return true if the address is less than @e a
         */
        auto operator<(const IPAddress & a) const -> bool;

#pragma mark - component getters

        /**
         * @brief method to get the address family
         * @return either the constant PF_INET or PF_INET6.
         */
        [[nodiscard]] auto get_family() const -> sa_family_t;

        /**
         * @brief method to get the address as part of a struct sockaddr.
         * @return a unique_ptr to the struct sockaddr.
         */
        [[nodiscard]] auto get_as_sockaddr() const -> std::unique_ptr<struct sockaddr>;

        /**
         * @brief method to get the length of the struct sockaddr structure.
         * @return the number of bytes in the struct sockaddr structure.
         */
        [[nodiscard]] auto get_sockaddr_length() const -> size_type;

#pragma mark - string name getters

        /**
         * @brief method to get the string presentation name of the address
         * @param remove_ipv6_scope true if the method should remove any '%dev' scoping
         * in IPv6 addresses.
         * @return a shared pointer pointing to the presentation name string.
         */
        auto get_presentation_name(bool remove_ipv6_scope = false) -> string_pointer_type;

        /**
         * @brief method to get the string dns name of the address
         * @return a shared pointer pointing to the dns string.
         */
        auto get_dns_name() -> string_pointer_type;

#pragma mark - methods for checking the kind of address

        /**
         * @brief method to check if the address is an IPv4 address.
         * @return true if the address is an IPv4 address.
         */
        [[nodiscard]] auto is_ipv4_address() const -> bool;

        /**
         * @brief method to check if the address is an IPv6 address.
         * @return true if the address is an IPv6 address.
         */
        [[nodiscard]] auto is_ipv6_address() const -> bool;

        /**
         * @brief method to check if the address is a private address.
         * @return true if the address is private.
         */
        [[nodiscard]] auto is_private_address() const -> bool;

        /**
         * @brief method to check if the address is a global address.
         * @return true if the address is global.
         */
        [[nodiscard]] auto is_global_address() const -> bool;

        /**
         * @brief method to check if the address is a site-local address.
         * @return true if the address is site-local.
         */
        [[nodiscard]] auto is_site_local_address() const -> bool;

        /**
         * @brief method to check if the address is a linnk-local address.
         * @return true if the address is link-local.
         */
        [[nodiscard]] auto is_link_local_address() const -> bool;

        /**
         * @brief method to check if the address is a loopback address.
         * @return true if the address is loopback.
         */
        [[nodiscard]] auto is_loopback_address() const -> bool;

        /**
         * @brief method to check if the address is an IPv4 address mapped in an IPv6 address.
         * @return true if the address is IPv4 mapped.
         */
        [[nodiscard]] auto is_ipv4_mapped_ipv6_address() const -> bool;

#pragma mark - ostream methods

        /**
         * @brief method to write the address to a stream
         * @param o the stream object
         * @return the stream object
         */
        auto description(std::ostream & o) const -> std::ostream &;

    private:
        using address_type = union
        {
            struct in_addr ipv4_address;
            struct in6_addr ipv6_address;
        };

        address_type m_address{};
        sa_family_t m_address_family{};
        string_pointer_type m_presentation_name{nullptr};
        string_pointer_type m_dns_name{nullptr};

#pragma mark - class helper methods

        /**
         * @brief helper function to generate a runtime error.
         * @param error the error
         */
        static void runtime_error_from_api_error(int error);

        /**
         * @brief load either the presentation name or the dns name for the address
         * @param get_dns_name true if the method should load the dns name
         * @return a pointer to the string containing either the presentation name
         * or the dns name.
         *
         * When @e get_dns_name is false, only load the presentation name.
         */
        [[nodiscard]] auto get_address_name(bool get_dns_name = false, bool remove_ipv6_scope = false) const
            -> string_pointer_type;
    };

    auto operator<<(std::ostream & o, const IPAddress & a) -> std::ostream &;

#pragma mark - generic comparison operators

    auto operator==(const IPAddress & a, const struct in_addr & ipv4_address) -> bool;
    auto operator==(const IPAddress & a, const struct in6_addr & ipv6_address) -> bool;
    auto operator==(const IPAddress & a, const String & address) -> bool;
    auto operator==(const struct in_addr & ipv4_address, const IPAddress & a) -> bool;
    auto operator==(const struct in6_addr & ipv6_address, const IPAddress & a) -> bool;
    auto operator==(const String & address, const IPAddress & a) -> bool;

    auto operator!=(const IPAddress & a, const struct in_addr & ipv4_address) -> bool;
    auto operator!=(const IPAddress & a, const struct in6_addr & ipv6_address) -> bool;
    auto operator!=(const IPAddress & a, const String & address) -> bool;
    auto operator!=(const struct in_addr & ipv4_address, const IPAddress & a) -> bool;
    auto operator!=(const struct in6_addr & ipv6_address, const IPAddress & a) -> bool;
    auto operator!=(const String & address, const IPAddress & a) -> bool;

    auto operator<(const IPAddress & a, const struct in_addr & ipv4_address) -> bool;
    auto operator<(const IPAddress & a, const struct in6_addr & ipv6_address) -> bool;
    auto operator<(const IPAddress & a, const String & address) -> bool;
    auto operator<(const struct in_addr & ipv4_address, const IPAddress & a) -> bool;
    auto operator<(const struct in6_addr & ipv6_address, const IPAddress & a) -> bool;
    auto operator<(const String & address, const IPAddress & a) -> bool;

} // namespace TF::Foundation

#endif // TFIPADDRESS_HPP
