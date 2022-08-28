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

#define NEEDS_SYS_TYPES_H
#define NEEDS_SYS_SOCKET_H
#define NEEDS_NETDB_H
#include "tfheaders.hpp"
#include "tfnetworkerror.hpp"

namespace TF::Foundation
{

    NetworkErrorTable::table_type NetworkErrorTable::s_network_errors{};

    bool NetworkErrorTable::s_initialized_table{false};

    std::mutex NetworkErrorTable::s_init_mutex{};

    void NetworkErrorTable::s_initialize_table()
    {
#if defined(EAI_AGAIN)
        s_network_errors.insert(std::make_pair(EAI_AGAIN, gai_strerror(EAI_AGAIN)));
#endif

#if defined(EAI_BADFLAGS)
        s_network_errors.insert(std::make_pair(EAI_BADFLAGS, gai_strerror(EAI_BADFLAGS)));
#endif

#if defined(EAI_BADHINTS)
        s_network_errors.insert(std::make_pair(EAI_BADHINTS, gai_strerror(EAI_BADHINTS)));
#endif

#if defined(EAI_FAIL)
        s_network_errors.insert(std::make_pair(EAI_FAIL, gai_strerror(EAI_FAIL)));
#endif

#if defined(EAI_FAMILY)
        s_network_errors.insert(std::make_pair(EAI_FAMILY, gai_strerror(EAI_FAMILY)));
#endif

#if defined(EAI_MEMORY)
        s_network_errors.insert(std::make_pair(EAI_MEMORY, gai_strerror(EAI_MEMORY)));
#endif

#if defined(EAI_NONAME)
        s_network_errors.insert(std::make_pair(EAI_NONAME, gai_strerror(EAI_NONAME)));
#endif

#if defined(EAI_PROTOCOL)
        s_network_errors.insert(std::make_pair(EAI_PROTOCOL, gai_strerror(EAI_PROTOCOL)));
#endif

#if defined(EAI_SERVICE)
        s_network_errors.insert(std::make_pair(EAI_SERVICE, gai_strerror(EAI_SERVICE)));
#endif

#if defined(EAI_SOCKTYPE)
        s_network_errors.insert(std::make_pair(EAI_SOCKTYPE, gai_strerror(EAI_SOCKTYPE)));
#endif

#if defined(EAI_SYSTEM)
        s_network_errors.insert(std::make_pair(EAI_SYSTEM, gai_strerror(EAI_SYSTEM)));
#endif
    }

} // namespace TF::Foundation
