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

    /**
     * Templated base class for representing a dynamic shared library object.
     * @tparam MODULE_HANDLE the type representing the handle to the shared library.
     * @tparam FUNCTION_HANDLE the type represent a handle to a function in the shared library.
     */
    template<typename MODULE_HANDLE, typename FUNCTION_HANDLE>
    class DLLBase : public AllocatorInterface
    {
    public:
        using string_type = String;
        using module_handle = MODULE_HANDLE;
        using function_handle = FUNCTION_HANDLE;

        /**
         * @brief default constructor
         * @param auto_close true if the object should automatically try to close the handle to the library
         * in the destructor.
         */
        explicit DLLBase(bool auto_close = false) : m_auto_close{auto_close} {}

        /**
         * @brief module handle constructor
         * @param handle the handle to the shared library
         * @param auto_close true if the object should automatically try to close the handle to the library
         * in the destructor.
         */
        DLLBase(module_handle handle, bool auto_close = true) : m_module_handle{handle}, m_auto_close{auto_close} {}

        // Deleted copy/move constructors. Wrap DLL in std::shared_ptr if you need to copy the DLL around.
        DLLBase(const DLLBase & dll) = delete;

        DLLBase(DLLBase && dll) = delete;

        /**
         * @brief destructor
         *
         * Attempts to automatically close the handle to the library.
         */
        ~DLLBase()
        {
            if (m_auto_close)
            {
                close();
            }
        }

        // Deleted assigment/move assignment operators. Use std::shared_ptr if needed.

        DLLBase & operator=(const DLLBase & dll) = delete;

        DLLBase & operator=(DLLBase && dll) = delete;

        /**
         * @brief method to load a handle to the named function from the shared library
         * @param name the name of the function
         * @return a handle to the function on success, will throw a std::runtime_error
         * exception on error.
         */
        [[nodiscard]] auto load_function(const string_type & name) -> function_handle
        {
            (void)name;
            return {};
        }

        /**
         * @brief method to close the handle to the library.
         */
        void close()
        {
            m_auto_close = false;
        }

    private:
        module_handle m_module_handle{};
        bool m_auto_close{};
    };

} // namespace TF::Foundation

#endif // TFDLLBASE_HXX
