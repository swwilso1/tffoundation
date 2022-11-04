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

#ifndef TFDLLBASE_HXX
#define TFDLLBASE_HXX

#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"

namespace TF::Foundation
{

    template<typename MODULE_HANDLE, typename FUNCTION_HANDLE>
    class DLLBase : public AllocatorInterface
    {
    public:
        using string_type = String;
        using module_handle = MODULE_HANDLE;
        using function_handle = FUNCTION_HANDLE;

        DLLBase() = default;

        explicit DLLBase(module_handle handle) : m_module_handle{handle} {}

        [[nodiscard]] auto load_function(const string_type & name) -> function_handle
        {
            (void)name;
            return {};
        }

    private:
        module_handle m_module_handle{};
    };

} // namespace TF::Foundation

#endif // TFDLLBASE_HXX
