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

#ifndef TFPLATFORMID_HPP
#define TFPLATFORMID_HPP

#define NEEDS_OSTREAM
#define NEEDS_MUTEX
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfversion.hpp"
#include "tfcompilerid.hpp"

namespace TF::Foundation
{

    class PlatformId : public AllocatorInterface
    {
    public:
        using string_type = String;
        using version_type = Version;
        using compilerid = CompilerId;

        PlatformId();

#pragma mark - methods to get platform details

        [[nodiscard]] auto get_name() const -> string_type;

        [[nodiscard]] auto get_vendor() const -> string_type;

        [[nodiscard]] auto get_version() const -> version_type;

        [[nodiscard]] auto get_compiler_id() const -> compilerid;

        [[nodiscard]] auto get_number_of_processors() const -> uint32_t;

    private:
        using mutex_type = std::mutex;
        using lock_type = std::lock_guard<mutex_type>;

        static string_type s_name;
        static string_type s_vendor;
        static version_type s_version;
        static compilerid s_compiler_id;
        static uint32_t s_number_of_processors;
        static bool s_initialized;
        static mutex_type s_mutex;

        static void initialize();
    };

} // namespace TF::Foundation

#endif // TFPLATFORMID_HPP
