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

#ifndef TFNETWORKADDRESS_HPP
#define TFNETWORKADDRESS_HPP

#define NEEDS_SYS_SOCKET_H
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfnetworkinitialization.hpp"

namespace TF::Foundation
{

    /**
     * NetworkAddress provides a base class for a hierarchy of address types centered around
     * struct sockaddr (struct sockaddr_in, struct sockaddr_in6, struct sockadder_netlink, ...).
     */
    class NetworkAddress : public AllocatorInterface, NetworkInitializer
    {
    public:
        using size_type = Size_t;

        /**
         * @brief default constructor
         */
        NetworkAddress();

        /**
         * @brief copy constructor
         * @param a the address to copy
         */
        NetworkAddress(const NetworkAddress & a);

        /**
         * @brief constructor to initialize from struct sockaddr and length
         * @param sa the pointer to struct sockaddr
         * @param length the length of struct sockaddr (ie the length of the actual struct).
         */
        NetworkAddress(const struct sockaddr * sa, size_type length);

        /**
         * @brief constructor to initialize from a byte array with length.
         * @param p the byte array.
         * @param length the length of the byte array.  The length must be <=
         * sizeof(struct sockaddr_storage).
         */
        NetworkAddress(const void * p, size_type length);

        /**
         * @brief destructor
         */
        virtual ~NetworkAddress();

        /**
         * @brief copy assignment
         * @param a the address to copy
         * @return the updated contents of the object.
         */
        auto operator=(const NetworkAddress & a) -> NetworkAddress &;

        /**
         * @brief equality operator
         * @param a the other address
         * @return true if the two addresses have the same underlying contents
         */
        auto operator==(const NetworkAddress & a) const -> bool;

        /**
         * @brief in-equality operator
         * @param a the other address
         * @return true if the two addresses do not have the same underlying contents.
         */
        auto operator!=(const NetworkAddress & a) const -> bool;

        /**
         * @brief less-than operator
         * @param a the other address
         * @return true if the this address is numerically less than @e a.
         */
        auto operator<(const NetworkAddress & a) const -> bool;

        /**
         * @brief method to get the underlying structure as a sockaddr pointer.
         * @return the pointer to the sockaddr.
         */
        [[nodiscard]] virtual auto get_as_sockaddr() const -> const struct sockaddr *;

        /**
         * @brief method to return the length of the underlying structure.
         * @return the length
         *
         * Override this method when inheriting from this class to specialie
         * interaction with one of the sub-versions of struct sockaddr.
         */
        [[nodiscard]] virtual auto address_length() const -> size_type;

        /**
         * @brief method to initialize the contents of the address to zero.
         */
        void clear();

    protected:
        struct sockaddr_storage m_address
        {};
    };
} // namespace TF::Foundation

#endif // TFNETWORKADDRESS_HPP
