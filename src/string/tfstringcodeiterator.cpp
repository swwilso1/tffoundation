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

#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfstringcodeiterator.hpp"

namespace TF
{

	namespace Foundation
	{

		StringCodeIterator::StringCodeIterator()
		{
			theBytes = NULL;
			theBytesLength = 0;
			theEncoder = NULL;
			theEndian = NoEndian;
		}


		StringCodeIterator::StringCodeIterator(const StringCodeIterator& i)
		{
			theBytes = i.theBytes;
			theBytesLength = i.theBytesLength;
			theEncoder = i.theEncoder;
			theEndian = i.theEndian;
		}


		StringCodeIterator::StringCodeIterator(const char_type *bytes, size_type byteLength,
			StringEncoder *encoder, Endian endian)
		{
			theBytes = const_cast<char_type *>(bytes);
			theBytesLength = byteLength;
			theEncoder = encoder;
			theEndian = endian;
		}


		StringCodeIterator::~StringCodeIterator()
		{
		}


		StringCodeIterator& StringCodeIterator::operator=(const StringCodeIterator& i)
		{
			if(this != &i)
			{
				theBytes = i.theBytes;
				theBytesLength = i.theBytesLength;
				theEncoder = i.theEncoder;
				theEndian = i.theEndian;
			}

			return *this;
		}


		StringCodeIterator& StringCodeIterator::operator++()
		{
			theBytes += theEncoder->basicCodeLengthInBytes();
			theBytesLength -= theEncoder->basicCodeLengthInBytes();
			return *this;
		}


		StringCodeIterator StringCodeIterator::operator++(int)
		{
			StringCodeIterator newIterator(*this);
			theBytes += theEncoder->basicCodeLengthInBytes();
			theBytesLength -= theEncoder->basicCodeLengthInBytes();
			return newIterator;
		}


		unicode_point_type StringCodeIterator::operator*(void)
		{
			if(theBytesLength == 0)
				throw std::runtime_error("End of iteration in operator*");

			std::pair<unicode_point_type, size_type> theNext;
			theNext = theEncoder->nextCode(theBytes, theBytesLength, theEndian);
			return theNext.first;
		}


		bool StringCodeIterator::operator==(const StringCodeIterator& i)
		{
			if(this != &i)
			{
				if(theBytes != i.theBytes)
					return false;

				if(theBytesLength != i.theBytesLength)
					return false;

				if(theEncoder != i.theEncoder)
					return false;

				if(theEndian != i.theEndian)
					return false;
			}

			return true;
		}


		bool StringCodeIterator::operator!=(const StringCodeIterator& i)
		{
			if(*this == i)
				return false;
			return true;
		}

	} // Foundation

} // TF


