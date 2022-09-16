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

#include "tfplatformid.hpp"

namespace TF::Foundation
{

    PlatformId::string_type PlatformId::s_name{};
    PlatformId::string_type PlatformId::s_vendor{};
    PlatformId::version_type PlatformId::s_version{};
    PlatformId::compilerid PlatformId::s_compiler_id{};
    uint32_t PlatformId::s_number_of_processors{};
    bool PlatformId::s_initialized{};
    PlatformId::mutex_type PlatformId::s_mutex{};

    PlatformId::PlatformId()
    {
        lock_type lock(s_mutex);
        if (! s_initialized)
        {
            initialize();
            s_initialized = true;
        }
    }

    auto PlatformId::get_name() const -> string_type
    {
        return s_name;
    }

    auto PlatformId::get_vendor() const -> string_type
    {
        return s_vendor;
    }

    auto PlatformId::get_version() const -> version_type
    {
        return s_version;
    }

    auto PlatformId::get_compiler_id() const -> compilerid
    {
        return s_compiler_id;
    }

    auto PlatformId::get_number_of_processors() const -> uint32_t
    {
        return s_number_of_processors;
    }

} // namespace TF::Foundation
