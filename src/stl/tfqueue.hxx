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

#ifndef TFQUEUE_HXX
#define TFQUEUE_HXX

#define NEEDS_QUEUE
#define NEEDS_DEQUE
#define NEEDS_OSTREAM
#include "tfheaders.hpp"

namespace TF
{
	
	namespace Foundation
	{

		template<class T, class Container = std::deque<T>>
		class Queue : public std::queue<T, Container>
		{
		public:
		
			using parent = std::queue<T, Container>;
			
			std::ostream& description(std::ostream &o) const;
		};
		
		
		template<class T, class Container = std::deque<T>>
		std::ostream& operator<<(std::ostream &o,
			const Queue<T, Container> &q);

	} // Foundation

} // TF

#include "tfqueue.cxx"

#endif // TFQUEUE_HXX

