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

#define NEEDS_SYS_SOCKET_H
#define NEEDS_FCNTL_H
#define NEEDS_UNISTD_H
#define NEEDS_SYSTEM_ERROR
#define NEEDS_ARRAY
#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tfposixsocket.hpp"

#define USE_POLL_STRATEGY
#include "tfpoll.hpp"

namespace TF::Foundation
{

    template<>
    SocketBase<int>::SocketBase(int domain, int type, int protocol)
    {
        m_socket = socket(domain, type, protocol);
        if (m_socket < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to create socket"};
        }
    }

    template<>
    auto SocketBase<int>::write(const void * p, size_type length) -> size_type
    {
        auto write_result = ::write(m_socket, p, length);
        if (write_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to write data to socket"};
        }
        return static_cast<size_type>(write_result);
    }

    template<>
    void SocketBase<int>::write_all(const void * p, size_type length)
    {
        auto tmp = reinterpret_cast<const char *>(p);
        auto bytes_left_to_write = length;
        while (bytes_left_to_write > 0)
        {
            auto bytes_wrote = write(tmp, bytes_left_to_write);
            bytes_left_to_write -= bytes_wrote;
            tmp += bytes_wrote;
        }
    }

    template<>
    void SocketBase<int>::write_data(const data_type & data)
    {
        auto tmp = data.bytes();
        auto bytes_left_to_write = data.length();
        write_all(tmp, bytes_left_to_write);
    }

#if defined(TFLINUX)
    template<>
    void SocketBase<int>::write_message(const struct msghdr & message, int flags)
    {
        auto api_result = sendmsg(m_socket, &message, flags);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Sendmsg error"};
        }
    }
#endif

    template<>
    auto SocketBase<int>::write_to(const address_type & address, const void * p, size_type length) -> size_type
    {
        auto bytes_wrote =
            sendto(m_socket, p, length, 0, address.get_as_sockaddr(), static_cast<socklen_t>(address.address_length()));
        if (bytes_wrote < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to write with sendto"};
        }

        return static_cast<size_type>(bytes_wrote);
    }

    template<>
    void SocketBase<int>::write_all_to(const address_type & address, const void * p, size_type length)
    {
        auto tmp = reinterpret_cast<const char *>(p);
        auto bytes_left_to_write = length;
        while (bytes_left_to_write > 0)
        {
            auto bytes_wrote = write_to(address, tmp, bytes_left_to_write);
            bytes_left_to_write -= bytes_wrote;
            tmp += bytes_wrote;
        }
    }

    template<>
    void SocketBase<int>::write_all_data_to(const address_type & address, const data_type & data)
    {
        auto tmp = data.bytes();
        auto bytes_left_to_write = data.length();
        write_all_to(address, tmp, bytes_left_to_write);
    }

    template<>
    auto SocketBase<int>::read(void * p, size_type length) -> size_type
    {
        auto bytes_read = ::read(m_socket, p, length);
        if (bytes_read < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to read from socket"};
        }
        return static_cast<size_type>(bytes_read);
    }

    template<>
    void SocketBase<int>::read_all(void * p, size_type length)
    {
        auto tmp = reinterpret_cast<char *>(p);
        auto bytes_to_read = length;
        while (bytes_to_read > 0)
        {
            auto bytes_read = read(reinterpret_cast<void *>(tmp), bytes_to_read);
            bytes_to_read -= bytes_read;
            tmp += bytes_read;
        }
    }

    template<>
    auto SocketBase<int>::read_as_data(size_type length) -> data_type
    {
        bool read_what_is_present = length == 0;
        constexpr size_type basic_length{1024};

        if (read_what_is_present)
        {
            std::array<char, basic_length> buffer{};
            length = basic_length;
            auto bytes_read = read(buffer.data(), length);
            return data_type{buffer.data(), bytes_read};
        }
        else
        {
            std::array<char, basic_length> buffer{};
            auto iterations = length / basic_length;
            auto last_extra_to_read = length % basic_length;
            data_type data{};

            while (iterations-- > 0)
            {
                read_all(buffer.data(), basic_length);
                data.append(buffer.data(), basic_length);
            }

            if (last_extra_to_read > 0)
            {
                read_all(buffer.data(), last_extra_to_read);
                data.append(buffer.data(), last_extra_to_read);
            }
            return data;
        }
    }

#if defined(TFLINUX)
    template<>
    auto SocketBase<int>::read_message(struct msghdr & message, int flags) -> size_type
    {
        auto api_result = recvmsg(m_socket, &message, flags);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Recvmsg returned an error"};
        }
        return static_cast<size_type>(api_result);
    }
#endif

    template<>
    auto SocketBase<int>::read_from(void * p, size_type length) -> std::tuple<address_type, size_type>
    {
        struct sockaddr address
        {};
        socklen_t address_length{};
        auto bytes_read = recvfrom(m_socket, p, length, 0, &address, &address_length);
        if (bytes_read < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to use recvfrom"};
        }
        address_type addr{&address, address_length};
        return std::tuple<address_type, size_type>{addr, static_cast<size_type>(bytes_read)};
    }

    template<>
    auto SocketBase<int>::read_all_from(void * p, size_type length) -> address_type
    {
        auto tmp = reinterpret_cast<char *>(p);
        auto bytes_left_to_read = length;
        address_type address;
        while (bytes_left_to_read > 0)
        {
            auto read_result = read_from(tmp, bytes_left_to_read);
            bytes_left_to_read -= std::get<1>(read_result);
            tmp += std::get<1>(read_result);
            address = std::get<0>(read_result);
        }
        return address;
    }

    template<>
    auto SocketBase<int>::read_as_data_from(size_type length) -> std::tuple<address_type, data_type>
    {
        bool read_what_is_present = length == 0;
        constexpr size_type basic_length{1024};

        if (read_what_is_present)
        {
            std::array<char, basic_length> buffer{};
            length = basic_length;
            auto read_result = read_from(buffer.data(), length);
            return std::tuple<address_type, data_type>{std::get<0>(read_result),
                                                       data_type{buffer.data(), std::get<1>(read_result)}};
        }
        else
        {
            std::array<char, basic_length> buffer{};
            auto iterations = length / basic_length;
            auto last_extra_to_read = length % basic_length;
            data_type data{};
            address_type address;

            while (iterations-- > 0)
            {
                address = read_all_from(buffer.data(), basic_length);
                data.append(buffer.data(), basic_length);
            }

            if (last_extra_to_read > 0)
            {
                address = read_all_from(buffer.data(), last_extra_to_read);
                data.append(buffer.data(), last_extra_to_read);
            }
            return std::tuple<address_type, data_type>{address, data};
        }
    }

    template<>
    auto SocketBase<int>::get_close_on_exec() -> bool
    {
        auto api_result = fcntl(m_socket, F_GETFD, 0);
        return api_result & FD_CLOEXEC;
    }

    template<>
    void SocketBase<int>::set_close_on_exec(bool close)
    {
        auto api_result = fcntl(m_socket, F_GETFD, 0);
        if (close)
        {
            api_result |= FD_CLOEXEC;
        }
        else
        {
            api_result |= ~FD_CLOEXEC;
        }
        auto set_result = fcntl(m_socket, F_SETFD, api_result);
        if (set_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Fcntl failed to set close-on-exec"};
        }
    }

    template<>
    auto SocketBase<int>::get_linger() -> bool
    {
        return false;
    }

    template<>
    void SocketBase<int>::set_linger(bool linger)
    {
        (void)linger;
    }

    template<>
    auto SocketBase<int>::get_non_blocking() -> bool
    {
        auto api_result = fcntl(m_socket, F_GETFL, 0);
        return api_result & O_NONBLOCK;
    }

    template<>
    void SocketBase<int>::set_non_blocking(bool non_block)
    {
        auto api_result = fcntl(m_socket, F_GETFL, 0);
        if (non_block)
        {
            api_result |= O_NONBLOCK;
        }
        else
        {
            api_result |= ~O_NONBLOCK;
        }
        auto set_result = fcntl(m_socket, F_SETFL, api_result);
        if (set_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Fcntl failed to set non-blocking"};
        }
    }

    template<>
    auto SocketBase<int>::get_receive_signals() -> bool
    {
        auto api_result = fcntl(m_socket, F_GETFL, 0);
        return api_result & FASYNC;
    }

    template<>
    void SocketBase<int>::set_receive_signals(bool receive)
    {
        auto api_result = fcntl(m_socket, F_GETFL, 0);
        if (receive)
        {
            // Make sure this process is the process set to get signals for the socket.
            auto own_result = fcntl(m_socket, F_SETOWN, getpid());
            if (own_result < 0)
            {
                throw std::system_error{errno, std::system_category(), "Fcntl error while setting socket signal owner"};
            }

            api_result |= FASYNC;
        }
        else
        {
            api_result |= ~FASYNC;
        }
        auto set_result = fcntl(m_socket, F_SETFL, api_result);
        if (set_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Fcntl error while setting FASYNC for socket"};
        }
    }

    template<>
    void SocketBase<int>::get_option(int option, int option_level, void * p, size_type * length)
    {
        socklen_t option_length;
        auto api_result = getsockopt(m_socket, option_level, option, p, &option_length);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Getsockopt failed"};
        }
        if (length != nullptr)
        {
            *length = static_cast<size_type>(option_length);
        }
    }

    template<>
    void SocketBase<int>::set_option(int option, int option_level, void * p, size_type length)
    {
        auto option_length = static_cast<socklen_t>(length);
        auto api_result = setsockopt(m_socket, option_level, option, p, option_length);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Setsockopt failed"};
        }
    }

    template<>
    auto SocketBase<int>::duplicate() -> SocketBase
    {
        auto new_socket_fd = dup(m_socket);
        if (new_socket_fd < 0)
        {
            throw std::system_error{errno, std::system_category(), "Error duplicating socket descriptor"};
        }
        return SocketBase{new_socket_fd};
    }

    template<>
    auto SocketBase<int>::accept() -> std::tuple<address_type, SocketBase>
    {
        struct sockaddr address
        {};
        socklen_t address_length{};

        address_length = sizeof(address);
        auto new_socket = ::accept(m_socket, &address, &address_length);
        if (new_socket < 0)
        {
            throw std::system_error{errno, std::system_category(), "Error accepting connection"};
        }
        address_type addr{&address, address_length};
        return std::tuple<address_type, SocketBase>{addr, SocketBase{new_socket}};
    }

    template<>
    template<typename Rep, typename Period>
    auto SocketBase<int>::accept_for(const std::chrono::duration<Rep, Period> & wait_duration)
        -> std::optional<std::tuple<address_type, SocketBase>>
    {
        Poller poll_thing{};
        poll_thing.add_handle(m_socket, PollEvent::Read, nullptr);
        auto descriptor_ready = poll_thing.wait_for(wait_duration);
        if (! descriptor_ready)
        {
            return {};
        }

        auto new_connection = accept();
        return {new_connection};
    }

    template<>
    template<typename Clock, typename Duration>
    auto SocketBase<int>::accept_until(const std::chrono::time_point<Clock, Duration> & abs_time)
        -> std::optional<std::tuple<address_type, SocketBase>>
    {
        Poller poll_thing{};
        poll_thing.add_handle(m_socket, PollEvent::Read, nullptr);
        auto descriptor_ready = poll_thing.wait_until(abs_time);
        if (! descriptor_ready)
        {
            return {};
        }

        auto new_connection = accept();
        return {new_connection};
    }

    template<>
    void SocketBase<int>::bind(const address_type & address)
    {
        auto api_result = ::bind(m_socket, address.get_as_sockaddr(), static_cast<socklen_t>(address.address_length()));
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to bind to address"};
        }
    }

    template<>
    void SocketBase<int>::listen(int listen_queue_size)
    {
        auto api_result = ::listen(m_socket, listen_queue_size);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to set socket to listen for connection"};
        }
    }

    template<>
    void SocketBase<int>::connect(const address_type & address)
    {
        auto api_result =
            ::connect(m_socket, address.get_as_sockaddr(), static_cast<socklen_t>(address.address_length()));
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Unable to connect"};
        }
    }

    template<>
    template<typename Rep, typename Period>
    auto SocketBase<int>::connect_for(const address_type & address,
                                      const std::chrono::duration<Rep, Period> & wait_duration) -> bool
    {
        set_non_blocking(true);
        auto api_result =
            ::connect(m_socket, address.get_as_sockaddr(), static_cast<socklen_t>(address.address_length()));
        if (api_result < 0)
        {
            if (errno == EINPROGRESS)
            {
                // Connect is happening in the background.
                Poller poll_thing{};
                int event{0};
                event = set_event_for(event, PollEvent::Read);
                event = set_event_for(event, PollEvent::Write);
                event = set_event_for(event, PollEvent::Except);
                poll_thing.add_handle(m_socket, event, nullptr);
                auto descriptor_ready = poll_thing.wait_for(wait_duration);
                if (! descriptor_ready)
                {
                    return false;
                }

                if (poll_thing.handle_set_for_event(m_socket, PollEvent::Read) ||
                    poll_thing.handle_set_for_event(m_socket, PollEvent::Write))
                {
                    // check for a socket error.
                    socklen_t error_var_length{sizeof(int)};
                    int error_var{0};
                    auto sockopt_result = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error_var, &error_var_length);
                    if (sockopt_result < 0)
                    {
                        throw std::system_error{errno, std::system_category(), "getsockopt call failed"};
                    }
                    if (error_var != 0)
                    {
                        throw std::runtime_error{"Error while connecting socket"};
                    }
                }
            }
            else
            {
                throw std::system_error{errno, std::system_category(), "Error calling connect"};
            }
        }
        return true;
    }

    template<>
    template<typename Clock, typename Duration>
    auto SocketBase<int>::connect_until(const address_type & address,
                                        const std::chrono::time_point<Clock, Duration> & abs_time) -> bool
    {
        set_non_blocking(true);
        auto api_result =
            ::connect(m_socket, address.get_as_sockaddr(), static_cast<socklen_t>(address.address_length()));
        if (api_result < 0)
        {
            if (errno == EINPROGRESS)
            {
                // Connect is happening in the background.
                Poller poll_thing{};
                int event{0};
                event = set_event_for(event, PollEvent::Read);
                event = set_event_for(event, PollEvent::Write);
                event = set_event_for(event, PollEvent::Except);
                poll_thing.add_handle(m_socket, event, nullptr);
                auto descriptor_ready = poll_thing.wait_until(abs_time);
                if (! descriptor_ready)
                {
                    return false;
                }

                if (poll_thing.handle_set_for_event(m_socket, PollEvent::Read) ||
                    poll_thing.handle_set_for_event(m_socket, PollEvent::Write))
                {
                    // check for a socket error.
                    socklen_t error_var_length{sizeof(int)};
                    int error_var{0};
                    auto sockopt_result = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error_var, &error_var_length);
                    if (sockopt_result < 0)
                    {
                        throw std::system_error{errno, std::system_category(), "getsockopt call failed"};
                    }
                    if (error_var != 0)
                    {
                        throw std::runtime_error{"Error while connecting socket"};
                    }
                }
            }
            else
            {
                throw std::system_error{errno, std::system_category(), "Error calling connect"};
            }
        }
        return true;
    }

    template<>
    void SocketBase<int>::close()
    {
        if (! m_closed)
        {
            ::close(m_socket);
            m_closed = true;
        }
    }

    template<>
    void SocketBase<int>::shutdown(bool read_side)
    {
        int flag = read_side ? SHUT_RD : SHUT_WR;
        auto api_result = ::shutdown(m_socket, flag);
        if (api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "Error calling shutdown"};
        }
    }

    template<>
    auto SocketBase<int>::description(std::ostream & o) const -> std::ostream &
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("Socket");
            formatter->addClassMember<int>("m_socket", m_socket);
            formatter->addClassMember<bool>("m_closed", m_closed);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

} // namespace TF::Foundation
