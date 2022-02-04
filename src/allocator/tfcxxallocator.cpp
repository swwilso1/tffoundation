/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

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

#include "tfallocatorinterface.hpp"
#include "tfcxxallocator.hpp"

void * operator new(TF::Foundation::Size_t s) noexcept(false)
{
    auto allocator = TF::Foundation::AllocatorInterface::getBlockAllocator();
    if (allocator != nullptr)
    {
        void * memory = allocator(s);
        if (memory == nullptr)
            throw std::bad_alloc();
        return memory;
    }
    throw std::bad_alloc();
}

void * operator new[](TF::Foundation::Size_t s) noexcept(false)
{
    auto allocator = TF::Foundation::AllocatorInterface::getBlockAllocator();
    if (allocator != nullptr)
    {
        void * memory = allocator(s);
        if (memory == nullptr)
            throw std::bad_alloc();
        return memory;
    }
    throw std::bad_alloc();
}

void operator delete(void * p) noexcept
{
    if (p == nullptr)
        return;

    auto deallocator = TF::Foundation::AllocatorInterface::getBlockDeallocator();
    if (deallocator != nullptr)
    {
        deallocator(p);
    }
}

void operator delete[](void * p) noexcept
{
    if (p == nullptr)
        return;

    auto deallocator = TF::Foundation::AllocatorInterface::getBlockDeallocator();
    if (deallocator != nullptr)
    {
        deallocator(p);
    }
}
