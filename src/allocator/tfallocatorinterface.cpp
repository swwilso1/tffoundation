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

#define NEEDS_STDLIB_H
#define NEEDS_CSTDDEF
#include "tfheaders.hpp"
#include "tfallocatorinterface.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         *  The AllocatorInterface class allocates blocks of memory that have an extra pointer pre-pended
         *  to the block.  The extra pointer points to the deallocator function that was in use when the
         *  block was allocated.  Caching the reference to the deallocator allows users of the library to
         *  swap out a deallocator or allocator/deallocator pair at any time.   For example, when the library
         *  starts up, it will by default make use of malloc/free.  But, sometime later, the user of the
         *  library might install a custom memory allocator/deallocator later when the library needs to free
         *  some of that early allocated memory the newer custom deallocator will not correctly handle the
         *  memory release causing a crash (most likely).  By caching the deallocator, we can have the block
         *  freed by the deallocator that should free it.
         */

        struct MemoryBlock
        {
            DeallocatorType deallocator;
            char * theBuffer;
        };

        extern "C"
        {
            /**
             *  @brief Memory allocator function that allocates a MemoryBlock and caches
             *  the deallocator function
             */
            void * MemoryBlockAllocator(Size_t n);
            void MemoryBlockDeallocator(void * p);
        }

        void * MemoryBlockAllocator(Size_t n)
        {
            auto allocator = AllocatorInterface::getAllocator();
            if (allocator != nullptr)
            {
                MemoryBlock * newBlock = reinterpret_cast<MemoryBlock *>(allocator(n + sizeof(MemoryBlock)));
                if (newBlock != nullptr)
                {
                    newBlock->deallocator = AllocatorInterface::getDeallocator();
                    return reinterpret_cast<void *>(&newBlock->theBuffer);
                }
            }

            return nullptr;
        }

        void MemoryBlockDeallocator(void * p)
        {
            if (p != nullptr)
            {
                // Recover the MemoryBlock.
                MemoryBlock * theBlock =
                    reinterpret_cast<MemoryBlock *>(reinterpret_cast<char *>(p) - offsetof(MemoryBlock, theBuffer));
                if (theBlock != nullptr)
                {
                    if (theBlock->deallocator != nullptr)
                        theBlock->deallocator(theBlock);
                }
            }
        }

        AllocatorInterface::allocator_type AllocatorInterface::theAllocator = malloc;

        AllocatorInterface::deallocator_type AllocatorInterface::theDeallocator = free;

        AllocatorInterface::mutex_type AllocatorInterface::theMutex;

        void AllocatorInterface::setAllocator(allocator_type a)
        {
            lock_type theLock(theMutex);
            theAllocator = a;
        }

        void AllocatorInterface::setDeallocator(deallocator_type d)
        {
            lock_type theLock(theMutex);
            theDeallocator = d;
        }

        void AllocatorInterface::setAllocatorAndDeallocator(allocator_type a, deallocator_type d)
        {
            lock_type theLock(theMutex);
            theAllocator = a;
            theDeallocator = d;
        }

        AllocatorInterface::allocator_type AllocatorInterface::getAllocator(void)
        {
            lock_type theLock(theMutex);
            return theAllocator;
        }

        AllocatorInterface::deallocator_type AllocatorInterface::getDeallocator(void)
        {
            lock_type theLock(theMutex);
            return theDeallocator;
        }

        AllocatorInterface::pair_type AllocatorInterface::getAllocatorAndDeallocator(void)
        {
            lock_type theLock(theMutex);
            return std::make_pair(theAllocator, theDeallocator);
        }

        void * AllocatorInterface::operator new(size_type s)
        {
            return MemoryBlockAllocator(s);
        }

        void * AllocatorInterface::operator new[](size_type s)
        {
            return MemoryBlockAllocator(s);
        }

        void * AllocatorInterface::operator new(size_type s, void * p)
        {
            return p;
        }

        void * AllocatorInterface::operator new[](size_type s, void * p)
        {
            return p;
        }

        void AllocatorInterface::operator delete(void * p)
        {
            MemoryBlockDeallocator(p);
        }

        void AllocatorInterface::operator delete[](void * p)
        {
            MemoryBlockDeallocator(p);
        }

        void AllocatorInterface::operator delete(void * p, void * q) {}

        void AllocatorInterface::operator delete[](void * p, void * q) {}

        AllocatorInterface::allocator_type AllocatorInterface::getBlockAllocator(void)
        {
            return MemoryBlockAllocator;
        }

        AllocatorInterface::deallocator_type AllocatorInterface::getBlockDeallocator(void)
        {
            return MemoryBlockDeallocator;
        }

    } // namespace Foundation

} // namespace TF
