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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#define NEEDS_IOSTREAM
#define NEEDS_UNISTD_H
#define NEEDS_SYS_TYPES_H
#include "tfheaders.hpp"
#include "tfrootusercheck.hpp"

namespace TF::Foundation
{

    bool check_for_root_user(int argc, const char ** argv, const String & tool_name)
    {
        auto id = getuid();
        auto effective_id = geteuid();

        if (id != 0 && effective_id != 0)
        {
            std::cout << tool_name << " must be run as root." << std::endl;
            std::cout << "Use 'sudo ";
            for (int i = 0; i < argc; i++)
            {
                std::cout << *(argv + i);
                if (i < (argc - 1))
                {
                    std::cout << " ";
                }
            }
            std::cout << "'" << std::endl;
            return false;
        }
        return true;
    };

} // namespace TF::Foundation
