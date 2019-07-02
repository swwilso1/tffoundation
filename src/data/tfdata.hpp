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

#ifndef TFDATA_HPP
#define TFDATA_HPP

#define NEEDS_OSTREAM
#define NEEDS_LIST
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tflist.hxx"

namespace TF
{

    namespace Foundation
    {

        /**
         *  @brief A class design to pass arrays of bytes around in a program.  The
         *  class holds arbitrary bytes of arbitrary length.
         **/
        class Data : public AllocatorInterface
        {

        private:
            /**
                    @brief The Chunk class encapsulates a buffer and a length.
            */
            class Chunk : public AllocatorInterface
            {

            public:
                /** @brief standard length type used in a chunk */
                using size_type = Size_t;

                /** @brief default constructor, initializes chunk to the empty state */
                Chunk() : theBuffer(nullptr), theLength(0)
                {
                }

                /**
                 *  @brief byte constructor.   Used to copy bytes into the buffer.
                 *  @param c the pointer to the bytes
                 *  @param length the number of bytes to copy
                 **/
                Chunk(const char *c, size_type length);

                /**
                 *  @brief copy constructor.
                 *  @param c the chunk to copy
                 **/
                Chunk(const Chunk &c);


                /**
                 *  @brief rvalue copy constructor
                 *  @brief c the chunk to copy using
                 *  rvalue semantics.
                 **/
                Chunk(Chunk &&c);

                /** @brief destructor */
                ~Chunk()
                {
                    if(theBuffer != nullptr && theLength != 0)
                        delete[] theBuffer;
                }

                /**
                 *  @brief standard assignment operattor
                 *  @param c the chunk to assign from
                 *  @return this chunk updated with a copy of the contents of
                 *  @e c
                 **/
                Chunk &operator=(const Chunk &c);

                /**
                 *  @brief rvalue semantics assignment operator.
                 *  used to assign a chunk rvalue to this object.
                 *  @param c the chunk to assign from.
                 *  @return this chunk updated with the contents of @e c
                 **/
                Chunk &operator=(Chunk &&c);

                /**
                 *  @brief equality operator to compare if two chunks are equal.
                 *  @param c the other chunk
                 *  @return true if the two chunks have identical content, and false
                 *  otherwise.
                 */
                bool operator==(const Chunk &c) const;

                /**
                 *  @brief equality operator to compare if two chunks do not have
                 *  the same content
                 *  @param c the other chunk
                 *  @return true if the two chunks do not have the same content, and
                 *  false otherwise.
                 */
                bool operator!=(const Chunk &c) const;


                /** @brief return the number of bytes in the chunk */
                size_type length() const
                {
                    return theLength;
                }

                /** @brief return a pointer to the first byte in the byte array */
                const char *bytes() const
                {
                    return theBuffer;
                }

                /**
                 *  @brief writes a formatted copy of the chunk contents to the
                 *  stream using the current formatter.
                 *  @param o the stream object.
                 *  @preturn o the stream object.
                 **/
                std::ostream &description(std::ostream &o) const;

            private:
                /** @brief the pointer to the first byte in the byte array */
                char *theBuffer;

                /** @brief the number of bytes in the byte array */
                size_type theLength;
            };


            /** @brief Friend with the operator<< overload so that we can write chunks
             *  easily to streams
             **/
            friend std::ostream &operator<<(std::ostream &o, const Chunk &c);

        public:
            /** @brief type used to hold the number of bytes in the data object */
            using size_type = Size_t;


            /** @brief range type */
            using range_type = Range;


            /** @brief type to used represent an individucal byte value from the data object */
            using byte_type = unsigned char;


            /** @brief default constructor */
            Data() : theLength(0)
            {
                chunkList = nullptr;
            }

            /**
             *  @brief byte array constructor
             *  @param bytes the pointer to the array of bytes
             *  @param length the number of bytes in the array
             **/
            Data(const char *bytes, size_type length);

            /**
             *  @brief copy constructor, copies the contents of another
             *  Data object
             *  @param d the other Data object
             **/
            Data(const Data &d);

            /**
             *  @brief rvalue copy constructor, moves the contents of another
             *  Data object into the new object.
             *  @param d the other object.
             **/
            Data(Data &&d);

            /** @brief destructor */
            ~Data();


            /**
             *  @brief copies the contents of another Data object
             *  to this object.
             *  @param d the other data object
             *  @return this object with a copy of the contents of
             *  @e d.
             **/
            Data &operator=(const Data &d);

            /**
             *  @brief moves the contents of another Data object
             *  to this object.
             *  @param d the other object
             *  @return this object now holding the contents of
             *  @e d.
             **/
            Data &operator=(Data &&d);


            /**
             *  @brief compare two Data objects to see if they contain
             *  the same byte array.
             *  @param d the other Data object.
             *  @return true if the two objects contain the same array
             *  and false otherwise.
             **/
            bool operator==(const Data &d) const;


            /**
             *  @brief compare two Data objects to see if they do not
             *  contain the same byte array.
             *  @param d the other Data object.
             *  @return true if the two objects do not contain the same
             *  array and false otherwise.
             **/
            bool operator!=(const Data &d) const;


            /**
             *  @brief method to return the byte value located at a named
             *  index.
             *  @param i the index.
             *  @return the byte value at index @e i.
             *
             *  This method does not do bounds checking on the index.
             **/
            byte_type operator[](size_type i) const;


            /**
             *  @brief method to calculate the number of bytes held by
             *  the Data object
             *  @return the number of bytes in the Data object.
             **/
            size_type length() const;


            /**
             *  @brief method to return a pointer to the start of the array
             *  of bytes in the Data object.
             *  @return the pointer to the bytes.
             **/
            const char *bytes() const;


            /**
             *  @brief method to append a raw byte array to the end of the
             *  bytes in the Data object.
             *  @param bytes a pointer to the new bytes
             *  @param length the number of bytes that @e bytes holds
             **/
            void append(const char *bytes, size_type length);


            /**
             *  @brief method to copy and append the contents of another Data
             *  object to the end of the current data object's byte array.
             *  @param d the other Data object.
             **/
            void append(const Data &d);


            /**
             *  @brief method to move and append the contents of another Data
             *  object to the end of the current data object's byte array.
             *  @param d the other Data object
             **/
            void append(Data &&d);

            /**
             *  @brief method to prepend a raw byte array to the byte array already
             *  contained in the object.
             *  @param bytes a pointer to the new byte array.
             *  @param length the number of bytes in the new byte array.
             **/
            void prepend(const char *bytes, size_type length);

            /**
             *  @brief method to prepend a copy of the byte array from another Data
             *  object to the byte array in the current object.
             *  @param d the other Data object.
             **/
            void prepend(const Data &d);

            /**
             *  @brief method to prepend a the byte array from another Data object
             *  to the byte array in the current object.
             *  @param d the other Data object.
             **/
            void prepend(Data &&d);


            /**
             * @brief method to return a subset of the data described by a range.
             * @param r the range
             * @return the subset of data described by @e r
             */
            Data subdataWithRange(const range_type &r) const;

            /**
             *  @brief method to write the object contents to a stream object
             *  in a formatted manner.
             *  @param o the output stream object.
             *  @return the output stream object.
             **/
            std::ostream &description(std::ostream &o) const;

        private:
            /**
                    This class uses a strategy of buffer chaining in a linked list to
                    make append/prepend operations very fast.   When a byte array is
                    added to the object, a new buffer is created and then the buffer is
                    stored in a linked list of buffers.   When an append or prepend happens
                    then a new buffer is created and just appended or prepended to the linked
                    list.   The buffers will stay in a linked list until the user of the
                    object calls operator[](), or bytes().   If either of those things
                    happen, the object will create a new monolithic buffer, copy the contents
                    of each individual buffers into the new large buffer, and then
                    destroy each of the smaller buffers.  The new buffer then becomes the only
                    element in the list of buffers.
            */

            /** @brief the buffer type */
            using chunk_type = Chunk;

            /** @brief the type for the list of buffers */
            using chunk_list_type = std::list<chunk_type *>;


            /** @brief the list of buffers */
            chunk_list_type *chunkList;

            /**
                    @brief the counter that holds the overall number of bytes tracked
                    by the data object.  Each time data is added the amount added gets
                    added to theLength.
            */
            size_type theLength;
        };


        /**
         *  @brief overloaded << operator function for C++ style output of
         *  Chunk objects.
         *  @param o the output stream operator.
         *  @param c the Chunk object.
         *  @return the output stream operator.
         **/
        std::ostream &operator<<(std::ostream &o, const Data::Chunk &c);


        /**
         *  @brief overloaded << operator function for C++ style output of
         *  Data objects.
         *  @param o the output stream operator.
         *  @param d the Data object.
         *  @return the output stream operator.
         **/
        std::ostream &operator<<(std::ostream &o, const Data &d);


    }    // namespace Foundation


}    // namespace TF


#endif /* TFDATA_HPP */
