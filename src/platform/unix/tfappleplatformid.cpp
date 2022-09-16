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
#define NEEDS_SYS_SYSCTL_H
#include "tfheaders.hpp"
#include "tfplatformid.hpp"
#include "tfposixplatformutilities.hpp"

namespace TF::Foundation
{

    void PlatformId::initialize()
    {
        s_name = "macOS";
        s_vendor = "Apple";
        s_version = Version::version_from_string(run_command("/usr/bin/sw_vers -productVersion"));

        // Calculate the number of processors on the machine.
        int mib[4];
        auto length = sizeof(s_number_of_processors);

        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU;

        sysctl(mib, 2, &s_number_of_processors, &length, NULL, 0);

        if (s_number_of_processors < 1)
        {
            mib[1] = HW_NCPU;
            sysctl(mib, 2, &s_number_of_processors, &length, NULL, 0);
            if (s_number_of_processors < 1)
            {
                s_number_of_processors = 1;
            }
        }

        s_compiler_id.name = "Apple Clang";
        s_compiler_id.version = Version(__clang_major__, __clang_minor__, __clang_patchlevel__);
    }

} // namespace TF::Foundation
