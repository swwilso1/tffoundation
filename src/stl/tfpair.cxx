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

#include "tfformatter.hpp"

namespace TF
{
	
	namespace Foundation
	{

		template<class A, class B>
		Pair<A,B>::Pair() : parent()
		{
		}


		template<class A, class B>
		Pair<A,B>::Pair(const A &a, const B &b) : parent(a, b)
		{
		}


		template<class A, class B>
		std::ostream& Pair<A,B>::description(std::ostream &o) const
		{
			ClassFormatter *formatter =
				FormatterFactory::getFormatter();
			if(formatter != nullptr)
			{
				formatter->setClassName("Pair");
				formatter->addClassMember<A>(
					"first", this->first);
				formatter->addClassMember<B>(
					"second", this->second);
				o << *formatter;
				delete formatter;
			}
			return o;
		}
		
		
		template<class A, class B>
		std::ostream& operator<<(std::ostream &o, const Pair<A,B> &p)
		{
			return p.description(o);
		}

	} // Foundation

} // TF

