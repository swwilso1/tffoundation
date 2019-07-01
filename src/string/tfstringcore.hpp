/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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

#ifndef TFSTRINGCORE_HPP
#define TFSTRINGCORE_HPP

#define NEEDS_OSTREAM
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

    namespace Foundation
    {

        class StringCore : public AllocatorInterface
        {
        public:
            /** @brief size type */
            using size_type = size_t;

            /** @brief unicode point type */
            using unicode_point_type = unsigned int;

            /** @brief character type */
            using char_type = unsigned char;

            /** @brief default constructor */
            StringCore();

            /** @brief copy constructor */
            StringCore(const StringCore &c);

            /** @brief rvalue constructor */
            StringCore(StringCore &&c) noexcept;

            /** @brief size constructor */
            explicit StringCore(size_type i);

            /** @brief array constructor */
            StringCore(const char_type *t, size_type i);

            /** @brief destructor */
            ~StringCore() override;


            /** @brief assignment operator */
            StringCore &operator=(const StringCore &c);

            /** @brief rvalue assignment operator */
            StringCore &operator=(StringCore &&c) noexcept;

            /** @brief equality operator */
            bool operator==(const StringCore &c) const;

            /** @brief inequality operator */
            bool operator!=(const StringCore &c) const;

            /** @brief length method */
            size_type length() const;

            /** @brief getter for byte array */
            char_type *data();

            /** @brief ostream method */
            std::ostream &description(std::ostream &o) const;


        private:
            char_type *theBytes;

            size_type numberOfBytes;
        };

        std::ostream &operator<<(std::ostream &o, const StringCore &c);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFSTRINGCORE_HPP