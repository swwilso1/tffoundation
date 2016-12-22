/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 Tectiform Inc.

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

#define NEEDS_IOSTREAM
#define NEEDS_IOMANIP
#include "tfheaders.hpp"
#include "tfclassmember.hxx"

namespace TF
{

	namespace Foundation
	{

		template<> TemplateArrayClassMember<unsigned char>::string_type
			TemplateArrayClassMember<unsigned char>::value()
		{
			std::stringstream collector;
			if(theLength > 0)
			{
				collector << "{";
				for(size_type i = 0; i < theLength; i++)
				{
					int value = static_cast<int>(theValue[i]);
					collector << std::hex << std::setw(2) << std::setfill('0') << value << std::dec;
					if(i < (theLength - 1))
						collector << ",";
				}
				collector << "}";
			}
			return collector.str();

		}


		template<> std::ostream& TemplateArrayClassMember<unsigned char>::writeToStream(std::ostream &o) const
		{
			o << theType << " " << theName << "[" << theLength << "]";
			if(theLength > 0)
			{
				o << " {";
				for(size_type i = 0; i < theLength; i++)
				{
					int value = static_cast<int>(theValue[i]);
					o << std::hex << std::setw(2) << std::setfill('0') << value << std::dec;
					if(i < (theLength - 1))
						o << ",";
				}
				o << "}";
			}
			return o;
		}


		template<> TemplateArrayClassMemberWithType<unsigned char>::string_type
			TemplateArrayClassMemberWithType<unsigned char>::value()
		{
			std::stringstream collector;
			if(theLength > 0)
			{
				collector << "{";
				for(size_type i = 0; i < theLength; i++)
				{
					int value = static_cast<int>(theValue[i]);
					collector << std::hex << std::setw(2) << std::setfill('0') << theValue[i] << std::dec;
					if(i < (theLength - 1))
						collector << ",";
				}
				collector << "}";
			}
			return collector.str();
		}


		template<> std::ostream& TemplateArrayClassMemberWithType<unsigned char>::writeToStream(std::ostream &o) const
		{
			o << theType << " " << theName << "[" << theLength << "]";
			if(theLength > 0)
			{
				o << " {";
				for(size_type i = 0; i < theLength; i++)
				{
					int value = static_cast<int>(theValue[i]);
					o << std::hex << std::setw(2) << std::setfill('0') << theValue[i] << std::dec;
					if(i < (theLength - 1))
						o << ",";
				}
				o << "}";
			}
			return o;
		}


		std::ostream& ClassMember::writeToStream(std::ostream &o) const
		{
			o << theType << " " << theName;
			return o;
		}


		std::ostream& VoidClassMember::writeToStream(std::ostream &o) const
		{
			std::stringstream converter;
			converter << theValue;
			o << theType << " " << theName << " " << converter.str();
			return o;
		}


		std::ostream& operator<<(std::ostream &o, const ClassMember &m)
		{
			return m.writeToStream(o);
		}

	} // Foundation

} // TF

