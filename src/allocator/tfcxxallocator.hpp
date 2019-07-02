/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

#ifndef TFCXXALLOCATOR_HPP
#define TFCXXALLOCATOR_HPP 1

#define NEEDS_NEW
#include "tfheaders.hpp"
#include "tftypes.hpp"

/**
 *  @brief single object operator new function that ties in to the library allocator interface.
 *  @param s the number of bytes to allocate.
 *  @return the allocated memory
 */
extern void *operator new(TF::Foundation::Size_t s) noexcept(false);

/**
 *  @brief array object operator new function that ties in to the library allocator interface
 *  @param s the number of bytes to allocate.
 *  @return the allocated memory
 */
extern void *operator new(TF::Foundation::Size_t s) noexcept(false);

/**
 *  @brief single object operator delete function that ties in to the library allocator interface.
 *  @param p the bytes to free.
 */
extern void operator delete(void *p) noexcept;

/**
 *  @brief array object operator delete function that ties in to the library allocator interface.
 *  @param p the bytes to free.
 */
extern void operator delete[](void *p) noexcept;

#endif    // TFCXXALLOCATOR_HPP
