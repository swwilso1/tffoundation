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

#include "tfplatform.hpp"

#if defined(TFLINUX)
#    define NEEDS_SYS_SOCKET_H
#endif

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

    /**
     * SocketBase provides a templated API for socket object construction and usage.
     * @tparam SOCKET the type of the underlying raw socket.
     *
     * SocketBase is implemented with the idea that platform specific implementations will
     * provide template instantiations for specific SOCKET types.  Very few of the base
     * template functions actually do anything useful.
     */
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

        /**
         * @brief construct a socket from a network family, a stream type and a protocol selector.
         * @param domain the network family type such as PF_INET, PF_INET6.
         * @param type the stream type such as SOCK_STREAM, SOCK_DGRAM, SOCK_RAW.
         * @param protocol protocol specific selector, usually unused.
         */
        SocketBase(int domain, int type, int protocol = 0)
        {
            (void)domain;
            (void)type;
            (void)protocol;
        }

        /**
         * @brief socket desctructor that calls close during destruction.
         */
        ~SocketBase() override
        {
            close();
        }

#pragma mark - write methods

        /**
         * @brief method to write @e length bytes from the byte array @e p.
         * @param p the pointer to the byte array.
         * @param length the number of bytes in @e p.
         * @return the number of bytes from @e p written to the socket.
         */
        [[nodiscard]] auto write(const void * p, size_type length) -> size_type
        {
            (void)p;
            (void)length;
            return 0;
        }

        /**
         * @brief method to write all the bytes from a buffer to the socket.
         * @param p the pointer to the buffer
         * @param length the number of bytes in @e p.
         */
        void write_all(const void * p, size_type length)
        {
            (void)p;
            (void)length;
        }

        /**
         * @brief method to write all the bytes from a data object to the socket.
         * @param data the data object.
         */
        void write_data(const data_type & data)
        {
            (void)data;
        }

#if defined(TFLINUX)
        /**
         * @brief method to write a msg to a Netlink socket on Linux.
         * @param message the message.
         * @param flags
         */
        void write_message(const struct msghdr & message, int flags)
        {

            (void)message;
            (void)flags;
        }
#endif

        /**
         * @brief method to write bytes from an array of bytes to a destination address.
         * @param address the destination address.
         * @param p the pointer to the byte array.
         * @param length the number of bytes in @e p.
         * @return the number of bytes writtent to the socket to go to address.
         */
        [[nodiscard]] auto write_to(const address_type & address, const void * p, size_type length) -> size_type
        {
            (void)address;
            (void)p;
            (void)length;
            return 0;
        }

        /**
         * @brief method to write all the bytes from an array of bytes to a destination address.
         * @param address the destination address.
         * @param p the pointer to the byte array.
         * @param length the number of bytes in the @e p.
         */
        void write_all_to(const address_type & address, const void * p, size_type length)
        {
            (void)address;
            (void)p;
            (void)length;
        }

        /**
         * @brief method to write all the bytes contained in a data object to an address.
         * @param address the destination address.
         * @param data the data object.
         */
        void write_all_data_to(const address_type & address, const data_type & data)
        {
            (void)address;
            (void)data;
        }

        // TODO: Add asynchronous write API.

#pragma mark - read methods

        /**
         * @brief method to read at most @e length bytes in to the byte array @e p.
         * @param p the byte array.
         * @param length the max number of bytes to read into @e p.
         * @return the actual number of bytes read to @e p.
         */
        [[nodiscard]] auto read(void * p, size_type length) -> size_type
        {
            (void)p;
            (void)length;
            return 0;
        }

        /**
         * @brief method to read @e length bytes to the byte buffer @e p.
         * @param p the pointer to the byte buffer.
         * @param length the number of bytes to read to @e p.
         */
        void read_all(void * p, size_type length)
        {
            (void)p;
            (void)length;
        }

        /**
         * @brief method to read @e length bytes from the socket and return them as a data object.
         * @param length the number of bytes to read.
         * @return the data object.
         *
         * When @e length is zero, read some system pre-defined length of bytes.
         */
        [[nodiscard]] auto read_as_data(size_type length = 0) -> data_type
        {
            (void)length;
            return {};
        };

#if defined(TFLINUX)
        /**
         * @brief read a message from a Linux Kernel Netlink socket.
         * @param message the message used to store the message from the Kernel.
         * @param flags
         * @return the number of bytes read into @e message.
         */
        [[nodiscard]] auto read_message(struct msghdr & message, int flags) -> size_type
        {
            (void)message;
            (void)flags;
            return 0;
        }
#endif

        /**
         * @brief method to read length bytes and return the address that sent the bytes.
         * @param p the pointer to the byte array to receive the bytes.
         * @param length the number of bytes to read.
         * @return the address of the sender and the number of bytes read to @e p.
         */
        [[nodiscard]] auto read_from(void * p, size_type length) -> std::tuple<address_type, size_type>
        {
            (void)p;
            (void)length;
            return {};
        }

        /**
         * @brief method to read exactly @e length bytes and return the sender of the bytes.
         * @param p the byte array to hold the bytes.
         * @param length the number of bytes to read.
         * @return the address of the sender.
         */
        [[nodiscard]] auto read_all_from(void * p, size_type length) -> address_type
        {
            (void)p;
            (void)length;
            return {};
        }

        /**
         * @brief method to read @e length bytes from the socket and return them as a data object along with the
         * address of the sender.
         * @param length the number of bytes to read.
         * @return both the address of the sender and the data object containing the bytes read.
         *
         * If @e length is zero, then the system uses a pre-defined system-specific value.
         */
        [[nodiscard]] auto read_as_data_from(size_type length = 0) -> std::tuple<address_type, data_type>
        {
            (void)length;
            return {};
        }

        // TODO: Add asynchronous read API

#pragma mark - socket option methods

        /**
         * @brief method to get the close-on-exec socket option.
         * @return true if the socket will close on exec().
         */
        [[nodiscard]] auto get_close_on_exec() -> bool
        {
            return false;
        }

        /**
         * @brief method to set the close-on-exec socket option.
         * @param close true if the socket should close on exec().
         */
        void set_close_on_exec(bool close)
        {
            (void)close;
        }

        /**
         * @brief method to get the socket linker socket option.
         * @return true if the socket is set to linker.
         */
        [[nodiscard]] auto get_linger() -> bool
        {
            return false;
        }

        /**
         * @brief method to set the socket linker socket option.
         * @param linger true if the socket should linger.
         */
        void set_linger(bool linger)
        {
            (void)linger;
        }

        /**
         * @brief method to get the blocking socket option.
         * @return true if the socket will not block.
         */
        [[nodiscard]] auto get_non_blocking() -> bool
        {
            return false;
        }

        /**
         * @brief method to set the socket blocking behavior.
         * @param non_block true if the socket should not block.
         */
        void set_non_blocking(bool non_block)
        {
            (void)non_block;
        }

        /**
         * @brief method to get the socket signal receipt option.
         * @return true if the socket should receive signals.
         */
        [[nodiscard]] auto get_receive_signals() -> bool
        {
            return false;
        }

        /**
         * @brief method to set the socket signal receipt option.
         * @param receive true if the socket should receive signals.
         */
        void set_receive_signals(bool receive)
        {
            (void)receive;
        }

        /**
         * @brief method to get a generic socket option
         * @param option the option selector.
         * @param option_level the selector level.
         * @param p the pointer to the byte array that will hold the option results.
         * @param length the pointer to the size object that will the number of bytes in @e p.
         */
        void get_option(int option, int option_level, void * p, size_type * length)
        {
            (void)option;
            (void)option_level;
            (void)p;
            (void)length;
        }

        /**
         * @brief method to set a generic socket option.
         * @param option the socket option.
         * @param option_level the socket option level.
         * @param p the pointer to a byte array that contains the new socket option setting.
         * @param length the number of bytes in the byte array @e p.
         */
        void set_option(int option, int option_level, void * p, size_type length)
        {
            (void)option;
            (void)option_level;
            (void)p;
            (void)length;
        }

#pragma mark - duplication methods

        /**
         * @brief method to duplicate a socket.
         * @return a new socket object with a duplicate of the internal socket.
         */
        [[nodiscard]] auto duplicate() -> SocketBase
        {
            return {};
        }

#pragma mark - socket connection methods

        /**
         * @brief method to accept an incoming connection.
         * @return return the address of the incoming connection and a socket representing that connection.
         */
        [[nodiscard]] auto accept() -> std::tuple<address_type, SocketBase>
        {
            return {};
        }

        /**
         * @brief method to wait a period @e wait_duration for a socket connection.
         * @tparam Rep type of the ticks in the duration.
         * @tparam Period type that represents the duration period.
         * @param wait_duration the duration parameter.
         * @return a std::optional that will contain the connecting address and socket representing the connection
         * on success.
         */
        template<typename Rep, typename Period>
        [[nodiscard]] auto accept_for(const std::chrono::duration<Rep, Period> & wait_duration)
            -> std::optional<std::tuple<address_type, SocketBase>>
        {
            (void)wait_duration;
            return {};
        }

        /**
         * @brief method to wait till a certain point in time for a socket connection.
         * @tparam Clock the type that represents the clock of the point in time.
         * @tparam Duration the type that represents the duration of the point in time.
         * @param abs_time the point in time for which to wait for a connection.
         * @return a std::optional that contains the connecting address and a new socket that represents
         * the new connection on success.
         */
        template<typename Clock, typename Duration>
        [[nodiscard]] auto accept_until(const std::chrono::time_point<Clock, Duration> & abs_time)
            -> std::optional<std::tuple<address_type, SocketBase>>
        {
            (void)abs_time;
            return {};
        }

        /**
         * @brief method to bind the socket to an address
         * @param address the address.
         */
        void bind(const address_type & address)
        {
            (void)address;
        }

        /**
         * @brief method to put the socket in listening mode to listen for an incoming connection.
         * @param listen_queue_size the queue size.
         */
        void listen(int listen_queue_size = 0)
        {
            (void)listen_queue_size;
        }

        /**
         * @brief method to connect to a destination address.
         * @param address the destination address.
         */
        void connect(const address_type & address)
        {
            (void)address;
        }

        /**
         * @brief method to attempt to connect to a destination address in a given duration of time.
         * @tparam Rep type of the ticks in the duration.
         * @tparam Period type that represents the duration period.
         * @param address the destination address.
         * @param wait_duration the duration parameter.
         * @return true if the connection succeeds in the duration time.
         */
        template<typename Rep, typename Period>
        [[nodiscard]] auto connect_for(const address_type & address,
                                       const std::chrono::duration<Rep, Period> & wait_duration) -> bool
        {
            (void)address;
            (void)wait_duration;
            return false;
        }

        /**
         * @brief method to attempt to connect to a destination address until reaching a fixed point in time.
         * @tparam Clock the type that represents the clock of the point in time.
         * @tparam Duration the type that represents the duration of the point in time.
         * @param address the destination address.
         * @param abs_time the fixed point in for which to wait.
         * @return true if the connect succeeds before reaching the fixed point in time.
         */
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

        /**
         * @brief method to close the socket.
         */
        void close() {}

        /**
         * @brief method to shutdown one side of the socket.
         * @param read_side true if the the method should close the read side, otherwise the method closes
         * the write side of the socket.
         */
        void shutdown(bool read_side)
        {
            (void)read_side;
        }

        /**
         * @brief method to check if a socket is closed.
         * @return true if the socket is closed.
         */
        [[nodiscard]] auto is_closed() const -> bool
        {
            return m_closed;
        }

#pragma mark - method to get the raw socket

        /**
         * @brief method to get the underlying raw socket object.
         * @return the underlying socket handle.
         */
        [[nodiscard]] auto get_raw_socket() const -> raw_socket_type
        {
            return m_socket;
        }

#pragma mark - method to interface with streams

        /**
         * @brief method for writing a socket to the stream.
         * @param o the stream.
         * @return the stream object @e o after writing the socket contents.
         */
        auto description(std::ostream & o) const -> std::ostream &
        {
            return o;
        }

    private:
        raw_socket_type m_socket{};
        bool m_closed{false};

#pragma mark - private constructors

        /**
         * @brief private constructor for creating a socket from a raw socket handle.
         * @param raw_socket the raw socket handle.
         */
        explicit SocketBase(raw_socket_type raw_socket) : m_socket{raw_socket} {}
    };

    /**
     * @brief overloaded << operator for SocketBase objects.
     * @tparam SOCKET the type of the underlying raw socket handle.
     * @param o the stream object.
     * @param s the SocketBase object.
     * @return @e o after writing @e s to the stream.
     */
    template<typename SOCKET>
    auto operator<<(std::ostream & o, const SocketBase<SOCKET> & s) -> std::ostream &
    {
        return s.description(o);
    }

} // namespace TF::Foundation

#endif // TFSOCKETBASE_HXX
