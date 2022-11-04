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

#define NEEDS_DLFCN_H
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfunixdll.hpp"

namespace TF::Foundation
{

    template<>
    auto DLLBase<void *, void *>::load_function(const string_type & name) -> function_handle
    {
        auto name_cstr = name.cStr();
        auto the_handle = dlsym(m_module_handle, name_cstr.get());
        if (the_handle == nullptr)
        {
            auto dl_error_string = dlerror();
            if (dl_error_string)
            {
                throw std::runtime_error{dl_error_string};
            }

            throw std::runtime_error{"dlsym() failed to find requested function"};
        }
        return the_handle;
    }

    template<>
    void DLLBase<void *, void *>::close()
    {
        auto api_result = dlclose(m_module_handle);
        if (api_result != 0)
        {
            auto dl_error_string = dlerror();
            if (dl_error_string)
            {
                throw std::runtime_error{dl_error_string};
            }
            throw std::runtime_error{"dlclose failed to close library"};
        }
    }

} // namespace TF::Foundation
