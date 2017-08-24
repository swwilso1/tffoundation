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

#include "tfstringencoder.hpp"

namespace TF
{
	namespace Foundation
	{
	
		bool StringEncoder::doesRangeOfBytesLieInString(const char_type *start, size_type length, range_type& range)
		{
			if(range.position > length)
				return false;
			
			if(range.length > (length - range.position))
			   return false;
			   
			return true;
		}


		bool StringEncoder::doesRangeOfCharactersLieInString(const char_type *start, size_type length, range_type& range)
		{
			size_type theNumberOfCharacters = this->numberOfCharacters(start, length);

			if(range.position > theNumberOfCharacters)
				return false;

			if(range.length > (theNumberOfCharacters - range.position))
				return false;

			return true;
		}


		ComparisonResult StringEncoder::compareStrings(const char_type *firstStringStart, size_type firstStringLength, 
			const char_type *secondStringStart, size_type secondStringLength)
		{
			size_type firstStringNumberOfCharacters = this->numberOfCharacters(firstStringStart, firstStringLength);
			size_type secondStringNumberOfCharacters = this->numberOfCharacters(secondStringStart, secondStringLength);
			
			size_type numberOfCharactersToCompare = firstStringNumberOfCharacters >= secondStringNumberOfCharacters ?
				secondStringNumberOfCharacters : firstStringNumberOfCharacters;
			
			unicode_point_type code1;
			unicode_point_type code2;
			
			for(size_type i = 0; i < numberOfCharactersToCompare; i++)
			{
				code1 = this->unicodeCodePointForCharacterAtIndex(firstStringStart, firstStringLength, i);
				code2 = this->unicodeCodePointForCharacterAtIndex(secondStringStart, secondStringLength, i);
				
				if(code1 < code2)
					return OrderedAscending;
				if(code1 > code2)
					return OrderedDescending;
			}
			
			if(firstStringNumberOfCharacters == secondStringNumberOfCharacters)
				return OrderedSame;
            if(firstStringNumberOfCharacters > secondStringNumberOfCharacters)
				return OrderedAscending;
			
			return OrderedDescending;
		}


		bool StringEncoder::operator!=(const StringEncoder& e)
		{
			if(*this == e)
				return false;
			return true;
		}


		Endian StringEncoder::thisSystemEndianness()
		{
			unsigned short value = 0xFEFF;
			unsigned char *bytes = reinterpret_cast<unsigned char *>(&value);
			if(*bytes == 0xFE)
				return Endian::BigEndian;
			return Endian::LittleEndian;
		}


		const Endian StringEncoder::myEndianValue = thisSystemEndianness();

	
	} // Foundation
	
}