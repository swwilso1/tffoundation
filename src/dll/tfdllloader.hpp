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

#ifndef TFDLLLOADER_HPP
#define TFDLLLOADER_HPP

#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfdll.hpp"

namespace TF::Foundation
{

    /**
     * Class used to load a dynamic shared library.
     */
    class DLLLoader : public AllocatorInterface
    {
    public:
        using string_type = String;
        using dll_type = std::shared_ptr<DLL>;

        /**
         * @brief method to load a named shared library.
         * @param name usually the full filesystem path to the library.
         * @return a std::shared_ptr to the DLL object.
         */
        static auto load_library(const string_type & name) -> dll_type;

        /**
         * @brief method to load a reference to the currently running shared library.
         * @return a std::shared_ptr to the currently running DLL object.
         */
        static auto load_library() -> dll_type;

    private:
        /**
         * @brief helper function that does the work of loading a named shared library.
         * @param name the name of the shared library.
         * @return a std::shared_ptr to the shared library.
         */
        static auto load_library_helper(const char * name) -> dll_type;
    };

} // namespace TF::Foundation

#endif // TFDLLLOADER_HPP
