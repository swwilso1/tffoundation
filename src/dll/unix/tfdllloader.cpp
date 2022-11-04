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

#define NEEDS_STDEXCEPT
#define NEEDS_DLFCN_H
#include "tfheaders.hpp"
#include "tfdllloader.hpp"

namespace TF::Foundation
{

    auto DLLLoader::load_library(const string_type & name) -> dll_type
    {
        auto name_cstr = name.cStr();
        return load_library_helper(name_cstr.get());
    }

    auto DLLLoader::load_library() -> dll_type
    {
        return load_library_helper(nullptr);
    }

    auto DLLLoader::load_library_helper(const char * name) -> dll_type
    {
        auto module_handle = dlopen(name, RTLD_LAZY);
        if (module_handle == nullptr)
        {
            auto dll_error = dlerror();
            if (dll_error)
            {
                throw std::runtime_error{dll_error};
            }

            throw std::runtime_error{"dlopen failed to open the named library"};
        }
        return std::make_shared<DLL>(module_handle);
    }

} // namespace TF::Foundation
