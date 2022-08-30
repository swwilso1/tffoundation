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

#ifndef TFPOSIXSOCKET_HPP
#define TFPOSIXSOCKET_HPP

#include "tfsocketbase.hxx"

namespace TF::Foundation
{

    using Socket = SocketBase<int>;

    template<>
    SocketBase<int>::SocketBase(int domain, int type, int protocol);

    template<>
    auto SocketBase<int>::write(const void * p, size_type length) -> size_type;

    template<>
    void SocketBase<int>::write_all(const void * p, size_type length);

    template<>
    void SocketBase<int>::write_data(const data_type & data);

    template<>
    auto SocketBase<int>::write_to(const address_type & address, const void * p, size_type length) -> size_type;

    template<>
    void SocketBase<int>::write_all_to(const address_type & address, const void * p, size_type length);

    template<>
    void SocketBase<int>::write_all_data_to(const address_type & address, const data_type & data);

    template<>
    auto SocketBase<int>::read(void * p, size_type length) -> size_type;

    template<>
    void SocketBase<int>::read_all(void * p, size_type length);

    template<>
    auto SocketBase<int>::read_as_data(size_type length) -> data_type;

    template<>
    auto SocketBase<int>::read_from(void * p, size_type length) -> std::tuple<address_type, size_type>;

    template<>
    auto SocketBase<int>::read_all_from(void * p, size_type length) -> address_type;

    template<>
    auto SocketBase<int>::read_as_data_from(size_type length) -> std::tuple<address_type, data_type>;

    template<>
    auto SocketBase<int>::get_close_on_exec() -> bool;

    template<>
    void SocketBase<int>::set_close_on_exec(bool close);

    template<>
    auto SocketBase<int>::get_linger() -> bool;

    template<>
    void SocketBase<int>::set_linger(bool linger);

    template<>
    auto SocketBase<int>::get_non_blocking() -> bool;

    template<>
    void SocketBase<int>::set_non_blocking(bool non_block);

    template<>
    auto SocketBase<int>::get_receive_signals() -> bool;

    template<>
    void SocketBase<int>::set_receive_signals(bool receive);

    template<>
    template<typename T, typename B>
    auto SocketBase<int>::get_option(int option, int option_level) -> T;

    template<>
    template<typename T, typename B>
    void SocketBase<int>::set_option(int option, int option_level, const T & thing);

    template<>
    auto SocketBase<int>::duplicate() -> SocketBase;

    template<>
    auto SocketBase<int>::accept() -> std::tuple<address_type, SocketBase>;

    template<>
    template<typename Rep, typename Period>
    auto SocketBase<int>::accept_for(const std::chrono::duration<Rep, Period> & wait_duration)
        -> std::optional<std::tuple<address_type, SocketBase>>;

    template<>
    template<typename Clock, typename Duration>
    auto SocketBase<int>::accept_until(const std::chrono::time_point<Clock, Duration> & abs_time)
        -> std::optional<std::tuple<address_type, SocketBase>>;

    template<>
    void SocketBase<int>::bind(const address_type & address);

    template<>
    void SocketBase<int>::listen(int listen_queue_size);

    template<>
    void SocketBase<int>::connect(const address_type & address);

    template<>
    template<typename Rep, typename Period>
    auto SocketBase<int>::connect_for(const address_type & address,
                                      const std::chrono::duration<Rep, Period> & wait_duration) -> bool;

    template<>
    template<typename Clock, typename Duration>
    auto SocketBase<int>::connect_until(const address_type & address,
                                        const std::chrono::time_point<Clock, Duration> & abs_time) -> bool;

    template<>
    void SocketBase<int>::close();

    template<>
    void SocketBase<int>::shutdown(bool read_side);

    template<>
    auto SocketBase<int>::description(std::ostream & o) const -> std::ostream &;

} // namespace TF::Foundation

#endif // TFPOSIXSOCKET_HPP
