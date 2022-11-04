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

#ifndef TFDEFER_HPP
#define TFDEFER_HPP

#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * Defer provides a mechanism to execute arbitrary code when the
         * defer object goes out of scope.
         */
        class Defer : public AllocatorInterface
        {
        public:
            using deferred_function = std::function<void(void)>;

            /**
             * @brief constructor that takes a defer function.
             * @param f the defer function
             */
            explicit Defer(deferred_function f);

            /**
             * @brief destructor that executes the defer function.
             */
            ~Defer();

        private:
            deferred_function m_deferred_function{};
        };

    } // namespace Foundation

} // namespace TF

#endif // TFDEFER_HPP
