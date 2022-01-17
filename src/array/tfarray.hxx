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

#ifndef TFARRAY_HXX
#define TFARRAY_HXX

#define NEEDS_OSTREAM
#define NEEDS_MEMORY

#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdata.hpp"

namespace TF
{

    namespace Foundation
    {

        template<class T>
        class Array;


        /**
         * @brief Simple class to iterate through members of class @e Array.
         * @tparam T
         */
        template<class T>
        class ArrayIterator : public AllocatorInterface
        {
        public:
            /** @brief type of data contained in the array iterator */
            using value_type = T;


            /** @brief size_type */
            using size_type = Size_t;


            /**
             * @brief Default constructor
             */
            ArrayIterator();


            /**
             * @brief Array constructor
             * @param i the location of the iterator index.
             */
            ArrayIterator(Array<value_type> &a, size_type i);


            /**
             * @brief copy constructor
             */
            ArrayIterator(const ArrayIterator &i);


            /**
             * @brief rvalue constructor
             */
            ArrayIterator(ArrayIterator &&i);


            /**
             * @brief Destructor
             */
            ~ArrayIterator();


            /**
             * @brief assignment operator
             */
            ArrayIterator &operator=(const ArrayIterator &i);


            /**
             * @brief rvalue assignment operator
             */
            ArrayIterator &operator=(ArrayIterator &&i);


            /**
             * @brief equality operator
             */
            bool operator==(const ArrayIterator &i) const;


            /**
             * @brief inequality operator
             */
            bool operator!=(const ArrayIterator &i) const;


            /**
             * @brief less-than operator
             */
            bool operator<(const ArrayIterator &i) const;


            /**
             * @brief less-than-equal operator
             */
            bool operator<=(const ArrayIterator &i) const;


            /**
             * @brief greater-than-equal operator
             */
            bool operator>=(const ArrayIterator &i) const;


            /**
             * @brief greater-than operator
             */
            bool operator>(const ArrayIterator &i) const;


            /**
             * @brief helper method to set the iterator to the end.
             */
            void setEnd()
            {
                theKind = iterator_kind::End;
            }

            /**
             * @brief prefix increment operator
             */
            ArrayIterator &operator++();


            /**
             * @brief postfix increment operator
             */
            ArrayIterator operator++(int);


            /**
             * @brief prefix decrement operator
             */
            ArrayIterator &operator--();


            /**
             * @brief postfix decrement operator
             */
            ArrayIterator operator--(int);

            /**
             * @brief addition operator
             */
            ArrayIterator operator+(int i);


            /**
             * @brief subtraction operator
             */
            ArrayIterator operator-(int i);


            /**
             * @brief operator +=
             */
            ArrayIterator &operator+=(int i);


            /**
             * @brief operator-=
             */
            ArrayIterator &operator-=(int i);


            /**
             * @brief dereference operator
             */
            value_type operator*();


            /**
             * @brief pointer access operator
             */
            value_type *operator->();


            /**
             * @brief helper function for writing an iterator to a stream
             * @param o the stream object
             * @return @e o the stream object
             */
            std::ostream &description(std::ostream &o) const;


        private:
            enum class ArrayIteratorKind
            {
                Normal,
                End
            };

            using iterator_kind = ArrayIteratorKind;

            Array<value_type> theArray;
            size_type theLocation;
            iterator_kind theKind;
        };


        /**
         * @brief overloaded operator<< for iterator objects.
         * @tparam T
         * @param o the stream object
         * @param i the iterator object
         * @return @e o the stream object
         */
        template<class T>
        std::ostream &operator<<(std::ostream &o, const ArrayIterator<T> &i);


        /**
         * @brief Simple class that matches most of std::array semantics except that it does
         * not require the size of the array at array type declaration time.
         */
        template<class T>
        class Array : public AllocatorInterface
        {
            friend class ArrayIterator<T>;

        public:
            /** @brief type of data contained in the array */
            using value_type = T;

            /** @brief size type for length parameters */
            using size_type = std::size_t;

            /** @brief difference type for subtraction operations */
            using difference_type = std::ptrdiff_t;

            /** @brief data reference */
            using reference = value_type &;

            /** @brief const data reference */
            using const_reference = const value_type &;

            /** @brief pointer */
            using pointer = value_type *;

            /** @brief const pointer */
            using const_pointer = const value_type *;

            /** @brief iterator */
            using iterator = ArrayIterator<T>;

            /** @brief const iterator */
            using const_iterator = const ArrayIterator<T>;

            /**
             * @brief default constructor
             *
             * Creates an array of zero length.
             */
            Array();


            /**
             * @brief array size constructor
             * @param l the length of the array
             *
             * Creates an array of length @e l
             */
            Array(const size_type &l);


            /**
             * @brief array value constructor
             * @param a the array of raw values
             * @param l the length of the array
             */
            Array(const value_type *a, size_type l);


            /**
             * @brief copy constructor
             * @param a the other array
             */
            Array(const Array &a);


            /**
             * @brief rvalue constructor
             * @param a the other array
             */
            Array(Array &&a);


            /**
             * @brief Destructor
             */
            ~Array();


            /**
             * @brief assignment operator
             * @param a the other array.
             * @return this array object changed to hold the same contents
             * as @e a.
             *
             * Overwrites each element with the elements from the @e a array.
             */
            Array &operator=(const Array &a);


            /**
             * @brief rvalue assignment operator
             * @param a the other array.
             * @return this array object with the stolen contents of @e a.
             */
            Array &operator=(Array &&a);


            /**
             * @brief equality operator
             * @param a the other array
             * @return true if the size and contents of the arrays match, and false otherwise.
             */
            bool operator==(const Array &a) const;


            /**
             * @brief inequaltiy operator
             * @param a the other array
             * @return true if th size or contents of the arrays do not match, and false otherwise.
             */
            bool operator!=(const Array &a) const;


            /**
             * @brief return an iterator to the first element of the array
             * @return iterator pointing to the first element of the array
             */
            iterator begin();


            /**
             * @brief return a const iterator to the first element of the array
             * @return const iterator pointing to the first element of the array.
             */
            const_iterator cbegin();


            /**
             * @brief return an iterator pointing the one element past the end of the array
             * @return the iterator pointing past the end of the array.
             */
            iterator end();


            /**
             * @brief return a const iterator pointing to one element past the end of the array
             * @return the const iterator pointing past the end of the array.
             */
            const_iterator cend();

            /**
             * @brief return a reference to the element at the specified index with bounds
             * checking
             * @param i the index
             * @return the reference to the element at i
             *
             * If i >= size() of the array, the array throws std::out_of_range
             */
            reference at(size_type i);


            /**
             * @brief return a constant reference to the element at the specified index with bounds
             * checking
             * @param i the index
             * @return a constant reference.
             *
             * If i >= size() of the array, the array throws std::out_of_range
             */
            const_reference at(size_type i) const;


            /**
             * @brief overloaded [] to access the element at the index without bounds checking
             * @param i the index
             * @return a reference to the element of i
             */
            reference operator[](size_type i);


            /**
             * @brief overloaded[] to access the element at the index without bounds checking
             * @param i the index
             * @return a constant reference to the element.
             */
            const_reference operator[](size_type i) const;


            /**
             * @brief return a reference to the first element in the array.
             * @return a reference to first element.
             *
             * Calling front() on an empty Array is undefined
             */
            reference front();


            /**
             * @brief return a const reference to the first element in the array.
             * @return the const reference to the element.
             *
             * Calling front() on an empty Array is undefined
             */
            const_reference front() const;


            /**
             * @brief return a reference to the last element in the array.
             * @return the reference to the last element.
             *
             * Calling back() on an empty Array is undefined.
             */
            reference back();


            /**
             * @brief return a constant reference to the last element in the array.
             * @return the constant reference
             *
             * Calling back() on an empty Array is undefined.
             */
            const_reference back() const;


            /**
             * @brief get a pointer to the underlying data array
             * @return a pointer to the array.
             */
            pointer data();


            /**
             * @brief get a constant pointer to the underlying data array.
             * @return the constant pointer to the array.
             */
            const_pointer data() const;


            /**
             * @brief check if the Array has no elements.
             * @return true if the Array has no elements and false otherwise.
             */
            constexpr bool empty() const;


            /**
             * @brief return the number of elements in the Array
             * @return the number of elements
             */
            constexpr size_type size() const;


            /**
             * @brief assign the given value to all elements of the array
             * @param value the value
             */
            void fill(const_reference value);


            /**
             * @brief exchange the contents of the array with another array
             * @param other the other array
             */
            void swap(Array &makeother);


            /**
             * @brief method to get the array contents as a Data object
             * @return the bytes of the array as a Data object
             */
            operator Data() const;


            /**
             * @brief method to write an Array contents to a stream.
             * @param o the stream object
             * @return @e o the stream object
             */
            std::ostream &description(std::ostream &o) const;


        private:
            std::shared_ptr<value_type> theArray;
            size_type theLength;
        };


        /**
         * @brief overloaded operator<< for Array objects
         * @tparam T template parameter
         * @param o the stream object
         * @param a the Array object
         * @return @e o the stream object
         */
        template<class T>
        std::ostream &operator<<(std::ostream &o, const Array<T> &a);


    }    // namespace Foundation

}    // namespace TF

#include "tfarray.cxx"

#endif    // TFARRAY_HXX
