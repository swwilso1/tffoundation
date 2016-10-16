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
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstringencoder.hpp"
#include "tfstringtypes.hpp"

namespace TF
{

	namespace Foundation
	{

		class StringIterator : public AllocatorInterface
		{
		public:

			using size_type = size_t;

			StringIterator();

			StringIterator(const StringIterator& i);

			StringIterator(const char_type *bytes, size_type byteLength,
				StringEncoder *encoder, Endian endian);

			virtual ~StringIterator();


			StringIterator& operator=(const StringIterator& i);

			/**
				@brief pre-increment operator
			*/
			StringIterator& operator++();

			/**
				@brief post-increment operator
			*/
			StringIterator operator++(int);

			/**
				@brief dereference operator.

				This method returns the character code for the character currently
				pointed to by the iterator.
			*/
			unicode_point_type operator*(void);

			/**
				@brief equality operator
				@param i the iterator to compare
				@return true if the iterators reference the same string and the same character in that string,
				false otherwise.
			*/
			bool operator==(const StringIterator& i);


			/**
				@brief in-equality operator
				@param i the iterator to compare
				@return true if the iterators reference different strings or if the iterators reference the same
				strings, but different characters, otherwise false.
			*/
			bool operator!=(const StringIterator& i);


			/** @brief std::ostream interation method */
			std::ostream& description(std::ostream& o) const;

		private:

			char_type *theBytes;

			size_type theBytesLength;

			StringEncoder *theEncoder;

			Endian theEndian;
		};

		std::ostream& operator<<(std::ostream& o, const StringIterator& i);

	}  // Foundation

} // TF

#endif /* TFSTRINGITERATOR_HPP */

