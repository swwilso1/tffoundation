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

#ifndef TFSOCKETBASE_HXX
#define TFSOCKETBASE_HXX

#define NEEDS_CHRONO
#define NEEDS_OSTREAM
#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfnetworkaddress.hpp"
#include "tfnetworkinitialization.hpp"
#include "tfdata.hpp"

namespace TF::Foundation
{

    template<typename SOCKET>
    class SocketBase : public AllocatorInterface, NetworkInitializer
    {
    public:
#pragma mark - class types
        using raw_socket_type = SOCKET;
        using address_type = NetworkAddress;
        using size_type = Size_t;
        using data_type = Data;

#pragma mark - constructors

        SocketBase(int domain, int type, int protocol = 0)
        {
            (void)domain;
            (void)type;
            (void)protocol;
        }

        ~SocketBase()
        {
            close();
        }

#pragma mark - write methods

        [[nodiscard]] auto write(const void * p, size_type length) -> size_type
        {
            (void)p;
            (void)length;
            return 0;
        }

        void write_all(const void * p, size_type length)
        {
            (void)p;
            (void)length;
        }

        void write_data(const data_type & data)
        {
            (void)data;
        }

        [[nodiscard]] auto write_to(const address_type & address, const void * p, size_type length) -> size_type
        {
            (void)address;
            (void)p;
            (void)length;
            return 0;
        }

        void write_all_to(const address_type & address, const void * p, size_type length)
        {
            (void)address;
            (void)p;
            (void)length;
        }

        void write_all_data_to(const address_type & address, const data_type & data)
        {
            (void)address;
            (void)data;
        }

        // TODO: Add asynchronous write API.

#pragma mark - read methods

        [[nodiscard]] auto read(void * p, size_type length) -> size_type
        {
            (void)p;
            (void)length;
            return 0;
        }

        void read_all(void * p, size_type length)
        {
            (void)p;
            (void)length;
        }

        [[nodiscard]] auto read_as_data(size_type length = 0) -> data_type
        {
            (void)length;
            return {};
        };

        [[nodiscard]] auto read_from(void * p, size_type length) -> std::tuple<address_type, size_type>
        {
            (void)p;
            (void)length;
            return {};
        }

        [[nodiscard]] auto read_all_from(void * p, size_type length) -> address_type
        {
            (void)p;
            (void)length;
            return {};
        }

        [[nodiscard]] auto read_as_data_from(size_type length = 0) -> std::tuple<address_type, data_type>
        {
            (void)length;
            return {};
        }

        // TODO: Add asynchronous read API

#pragma mark - socket option methods

        [[nodiscard]] auto get_close_on_exec() -> bool
        {
            return false;
        }

        void set_close_on_exec(bool close)
        {
            (void)close;
        }

        [[nodiscard]] auto get_linger() -> bool
        {
            return false;
        }

        void set_linger(bool linger)
        {
            (void)linger;
        }

        [[nodiscard]] auto get_non_blocking() -> bool
        {
            return false;
        }

        void set_non_blocking(bool non_block)
        {
            (void)non_block;
        }

        [[nodiscard]] auto get_receive_signals() -> bool
        {
            return false;
        }

        void set_receive_signals(bool receive)
        {
            (void)receive;
        }

#pragma mark - duplication methods

        [[nodiscard]] auto duplicate() -> SocketBase
        {
            return {};
        }

#pragma mark - socket connection methods

        [[nodiscard]] auto accept() -> std::tuple<address_type, SocketBase>
        {
            return {};
        }

        template<typename Rep, typename Period>
        [[nodiscard]] auto accept_for(const std::chrono::duration<Rep, Period> & wait_duration)
            -> std::optional<std::tuple<address_type, SocketBase>>
        {
            (void)wait_duration;
            return {};
        }

        template<typename Clock, typename Duration>
        [[nodiscard]] auto accept_until(const std::chrono::time_point<Clock, Duration> & abs_time)
            -> std::optional<std::tuple<address_type, SocketBase>>
        {
            (void)abs_time;
            return {};
        }

        void bind(const address_type & address)
        {
            (void)address;
        }

        void listen(int listen_queue_size = 0)
        {
            (void)listen_queue_size;
        }

        void connect(const address_type & address)
        {
            (void)address;
        }

        template<typename Rep, typename Period>
        [[nodiscard]] auto connect_for(const address_type & address,
                                       const std::chrono::duration<Rep, Period> & wait_duration) -> bool
        {
            (void)address;
            (void)wait_duration;
            return false;
        }

        template<typename Clock, typename Duration>
        [[nodiscard]] auto connect_until(const address_type & address,
                                         const std::chrono::time_point<Clock, Duration> & abs_time) -> bool
        {
            (void)address;
            (void)abs_time;
            return false;
        }
        // TODO: Need asynchronous API for connection

#pragma mark - socket shutdown related methods

        void close() {}

        void shutdown(bool read_side)
        {
            (void)read_side;
        }

        [[nodiscard]] auto is_closed() const -> bool
        {
            return m_closed;
        }

#pragma mark - method to get the raw socket

        [[nodiscard]] auto get_raw_socket() const -> raw_socket_type
        {
            return m_socket;
        }

#pragma mark - method to interface with streams

        auto description(std::ostream & o) const -> std::ostream &
        {
            return o;
        }

    private:
        raw_socket_type m_socket{};
        bool m_closed{false};

#pragma mark - private constructors

        explicit SocketBase(raw_socket_type raw_socket) : m_socket{raw_socket} {}
    };

    template<typename SOCKET>
    auto operator<<(std::ostream & o, const SocketBase<SOCKET> & s) -> std::ostream &
    {
        return s.description(o);
    }

} // namespace TF::Foundation

#endif // TFSOCKETBASE_HXX
