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
#include "tfstringiterator.hpp"
#include "tfformatter.hpp"

namespace TF
{

	namespace Foundation
	{

		StringIterator::StringIterator()
		{
			theBytes = NULL;
			theBytesLength = 0;
			theEncoder = NULL;
			theEndian = NoEndian;
		}


		StringIterator::StringIterator(const StringIterator& i)
		{
			theBytes = i.theBytes;
			theBytesLength = i.theBytesLength;
			theEncoder = i.theEncoder;
			theEndian = i.theEndian;
		}


		StringIterator::StringIterator(const char_type *bytes, size_type byteLength,
			StringEncoder *encoder, Endian endian)
		{
			StringEncoder::byte_order_query_type stringQueryResult = encoder->hasByteOrderMark(bytes, byteLength);
			theBytes = const_cast<char_type *>(bytes);
			theBytesLength = byteLength;

			if(stringQueryResult.first && byteLength > 0)
			{
				theBytes += encoder->lengthOfByteOrderMarkInBytes();
				theBytesLength -= encoder->lengthOfByteOrderMarkInBytes();
			}

			theEncoder = encoder;
			theEndian = endian;
		}


		StringIterator::~StringIterator()
		{
		}


		StringIterator& StringIterator::operator=(const StringIterator& i)
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


		StringIterator& StringIterator::operator++()
		{
			std::pair<unicode_point_type, size_type> theNextCodePoint;
			theNextCodePoint = theEncoder->nextCodePoint(theBytes, theBytesLength, theEndian);
			theBytes += theNextCodePoint.second;
			theBytesLength -= theNextCodePoint.second;

			return *this;
		}


		StringIterator StringIterator::operator++(int)
		{
			StringIterator newIterator(*this);
			std::pair<unicode_point_type, size_type> theNextCodePoint;
			theNextCodePoint = theEncoder->nextCodePoint(theBytes, theBytesLength, theEndian);
			theBytes += theNextCodePoint.second;
			theBytesLength -= theNextCodePoint.second;
			return newIterator;
		}


		unicode_point_type StringIterator::operator*(void)
		{
			if(theBytesLength == 0)
				throw std::runtime_error("End of iteration in operator*");

			std::pair<unicode_point_type, size_type> theNextCodePoint;
			theNextCodePoint = theEncoder->nextCodePoint(theBytes, theBytesLength, theEndian);
			return theNextCodePoint.first;
		}


		bool StringIterator::operator==(const StringIterator& i)
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


		bool StringIterator::operator!=(const StringIterator& i)
		{
			if(*this == i)
				return false;
			return true;
		}


		std::ostream& StringIterator::description(std::ostream& o) const
		{
			ClassFormatter *formatter = FormatterFactory::getFormatter();
			if(formatter != nullptr)
			{
				formatter->setClassName("StringIterator");
				formatter->addClassMember("char_type *", "theBytes",
					theBytes);
				formatter->addClassMember("size_type", "theBytesLength",
					theBytesLength);
				formatter->addClassMember<Endian>("Endian", "theEndian", theEndian);
				o << *formatter;
				delete formatter;
			}
			return o;
		}


		std::ostream& operator<<(std::ostream& o, const StringIterator& i)
		{
			return i.description(o);
		}

	} // Foundation

} // TF

