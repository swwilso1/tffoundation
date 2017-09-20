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

#ifndef TFALLOCATORINTERFACE_HPP
#define TFALLOCATORINTERFACE_HPP 1

#define NEEDS_MUTEX
#define NEEDS_NEW
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"


namespace TF
{

    namespace Foundation
    {

        extern "C"
        {
            /** @brief C-style allocator function type */
            typedef void * (*AllocatorType)(Size_t n);

            /** @brief C-style deallocator function type */
            typedef void (*DeallocatorType)(void *p);
        }


        /**
         * @brief Memory allocator interface class
         *
         * This class provides an interface to the memory allocator system for the library.
         */
        class AllocatorInterface
        {
            // Allow the new/delete operators to have access to this interface.
            friend void * ::operator new(TF::Foundation::Size_t s) noexcept(false);
            friend void * ::operator new[](TF::Foundation::Size_t s) noexcept(false);
            friend void ::operator delete(void *) noexcept;
            friend void ::operator delete[](void *) noexcept;

        public:

            /** @brief type used to calculate and store memory buffer lengths */
            using size_type = Size_t;

            /** @brief type used to reference function that allocates memory */
            using allocator_type = AllocatorType;

            /** @brief type used to reference function that deallocates memory */
            using deallocator_type = DeallocatorType;

            /** @brief type used to reference an allocator/deallocator function pair */
            using pair_type = std::pair<allocator_type, deallocator_type>;

            /** @brief virtual destructor. This class anchors a whole chain of classes
             * that need access to the allocators */
            virtual ~AllocatorInterface() {}

            /**
             *  @brief static method for setting the root allocator function
             *  @param a the new allocator function.
             */
            static void setAllocator(allocator_type a);

            /**
             *  @brief static method for setting the root deallocator function
             *  @param d the new deallocator function
             */
            static void setDeallocator(deallocator_type d);


            /**
             *  @brief static convenience function for setting the allocator and deallocator
             *  simultaneously.
             *  @param a the new allocator function.
             *  @param d the new deallocator function.
             */
            static void setAllocatorAndDeallocator(allocator_type a, deallocator_type d);


            /**
             *  @brief static method for retrieving the currently used allocator function
             *  @return a pointer to the current allocator function.
             */
            static allocator_type getAllocator(void);


            /**
             *  @brief static method for retrieving the currently used deallocator function
             *  @return a pointer to the current deallocator function.
             */
            static deallocator_type getDeallocator(void);

            /**
             *  @brief static convenience function for getting access to the current allocator and
             *  deallocator functions simultaneously.
             *  @return a pair_type that contains the allocator in the first member, and the deallocator
             *  in the second member.
             */
            static pair_type getAllocatorAndDeallocator(void);


            /**
             *  @brief class definition for single object new operator
             *  @param s the number of bytes to allocate for the new object.
             *  @return a pointer to an object of the new type.
             */
            void * operator new(size_type s);


            /**
             *  @brief class definition for array new allocator.
             *  @param s the number of bytes to allocate for the new array.
             *  @return a pointer to an array of new objects
             */
            void * operator new[](size_type s);


            /**
             *  @brief class definition for the single object placement new operator.
             *  @param s the number of bytes to allocate for the new object.
             *  @param p the byte array to use as the basis for the new object.
             *  @return the pointer to p.
             */
            void * operator new(size_type s, void *p);


            /**
             *  @brief class definition for the array placement new operator.
             *  @param s the number of bytes to allocate for the new array of objects.
             *  @param p the byte byte array to use as the memory for the new array.
             *  @return the pointer to p.
             */
            void * operator new[](size_type s, void *p);


            /**
             *  @brief class definition for the single object delete operator
             *  @param p the pointer to the allocated object.
             */
            void operator delete(void *p);


            /**
             *  @brief class definition for the array object delete operator
             *  @param p the pointer to the allocated array of objects.
             */
            void operator delete[](void *p);


            /**
             *  @brief class definition for the single object placement delete operator.
             */
            void operator delete(void *p, void *q);


            /**
             *  @brief class definition for the array object placement delete operator.
             */
            void operator delete[](void *p, void *q);

        private:

            /** @brief mutex type */
            using mutex_type = std::mutex;

            /** @brief lock type */
            using lock_type = std::unique_lock<mutex_type>;

            static allocator_type theAllocator;
            static deallocator_type theDeallocator;
            static mutex_type theMutex;

            static allocator_type getBlockAllocator(void);
            static deallocator_type getBlockDeallocator(void);
        };

    } // Foundation

} // TF

#endif // TFALLOCATORINTERFACE_HPP

