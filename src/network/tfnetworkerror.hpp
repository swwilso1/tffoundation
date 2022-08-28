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

#ifndef TFNETWORKERROR_HPP
#define TFNETWORKERROR_HPP

#define NEEDS_UNORDERED_MAP
#define NEEDS_MUTEX
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    class NetworkErrorTable : public AllocatorInterface
    {
    public:
        using string_type = String;

        NetworkErrorTable();

        string_type get_explanation_for_error(int error);

    private:
        using table_type = std::unordered_map<int, string_type>;

        static table_type s_network_errors;
        static bool s_initialized_table;
        static std::mutex s_init_mutex;
        static void s_initialize_table();
    };

} // namespace TF::Foundation

#endif // TFNETWORKERROR_HPP
