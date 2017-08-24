/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2016 Tectiform Inc.

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

#ifndef TFSTRINGITERATOR_HPP
#define TFSTRINGITERATOR_HPP

#define NEEDS_OSTREAM
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstringcore.hpp"

namespace TF
{

    namespace Foundation
    {

        class StringIterator : public AllocatorInterface
        {
        public:

            /** @brief unicode point */
            using unicode_point_type = unsigned int;

            /** @brief size type */
            using size_type = size_t;

            /** @brief core type */
            using core_type = StringCore;

            /** @brief core pointer type */
            using core_pointer_type = std::shared_ptr<core_type>;

            /** @brief default constructor */
            StringIterator();

            /** @brief copy constructor */
            StringIterator(const StringIterator &i);

            /** @brief rvalue constructor */
            StringIterator(StringIterator &&i) noexcept;

            /** @brief string blob constructor */
            StringIterator(const core_pointer_type &c, size_type index = 0);

            /** @brief default destructor */
            ~StringIterator() = default;

            /** @brief assignment operator */
            StringIterator& operator=(const StringIterator &i);

            /** @brief rvalue assignment operator */
            StringIterator& operator=(StringIterator &&i) noexcept;

            /** @brief pre-increment operator */
            StringIterator& operator++();

            /** @brief post-increment opewrator */
            StringIterator operator++(int);

            /** @brief pre-decrement operator */
            StringIterator& operator--();

            /** @brief post-decrement operator */
            StringIterator operator--(int);

            /** @brief dereference operator */
            unicode_point_type operator*();

            /** @brief equality operator */
            bool operator==(const StringIterator &i) const;

            /** @brief in-equality operator */
            bool operator!=(const StringIterator &i) const;

            /** @brief std::ostream method */
            std::ostream& description(std::ostream &o) const;

        private:

            core_pointer_type core;

            size_type currentIndex;
        };


        std::ostream& operator<<(std::ostream &o, const StringIterator &i);

    } // Foundation

} // TF


#endif //TFSTRINGITERATOR_HPP
