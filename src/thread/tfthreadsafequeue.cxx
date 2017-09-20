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

#include "tftypes.hpp"
#include "tfformatter.hpp"

namespace TF
{

	namespace Foundation
	{

		template<class T, class Container>
		ThreadSafeQueue<T,Container>::ThreadSafeQueue(
			const container_type &container) : theQueue(container)
		{
		}


		template<class T, class Container>
		ThreadSafeQueue<T,Container>::ThreadSafeQueue(
			const ThreadSafeQueue &other)
		{
			ThreadSafeQueue &modOther =
				const_cast<ThreadSafeQueue &>(other);
			lock_guard_type guard(modOther.theMutex);
			theQueue = other.theQueue;
		}
			

		template<class T, class Container>
		ThreadSafeQueue<T,Container>::ThreadSafeQueue(
			ThreadSafeQueue &&other)
		{
			lock_guard_type guard(other.theMutex);
			theQueue = other.theQueue;
			theMutex = other.theMutex;
		}
			

		template<class T, class Container>
		ThreadSafeQueue<T,Container>::~ThreadSafeQueue()
		{
		}


		template<class T, class Container>
		ThreadSafeQueue<T,Container>&
			ThreadSafeQueue<T,Container>::operator=(
				const ThreadSafeQueue &other)
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				theQueue = other.theQueue;
			}
			
			return *this;
		}
			

		template<class T, class Container>
		ThreadSafeQueue<T,Container>&
			ThreadSafeQueue<T,Container>::operator=(
				ThreadSafeQueue &&other)
		{
			if(this != &other)
			{
				lock_guard_type otherGuard(other.theMutex);
				theMutex.lock();
				theQueue = other.theQueue;
				theMutex.unlock();
				theMutex = other.theMutex;
			}
			
			return *this;
		}


		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::operator==(
			const ThreadSafeQueue &other) const
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				return theQueue == other.theQueue;
			}
			
			return true;
		}

			
		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::operator!=(
			const ThreadSafeQueue &other) const
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				return theQueue != other.theQueue;
			};
			
			return false;
		}


		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::operator<(
			const ThreadSafeQueue &other) const
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				return theQueue < other.theQueue;
			};
			
			return false;
		}
			

		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::operator<=(
			const ThreadSafeQueue &other) const
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				return theQueue <= other.theQueue;
			};
			
			return true;
		}
			

		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::operator>=(
			const ThreadSafeQueue &other) const
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				return theQueue >= other.theQueue;
			};
			
			return true;
		}


		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::operator>(
			const ThreadSafeQueue &other) const
		{
			if(this != &other)
			{
				ThreadSafeQueue &modOther =
					const_cast<ThreadSafeQueue &>(other);
				lock_guard_type otherGuard(modOther.theMutex);
				lock_guard_type myGuard(theMutex);
				return theQueue > other.theQueue;
			};
			
			return false;
		}
			

		template<class T, class Container>
		typename ThreadSafeQueue<T,Container>::reference 
			ThreadSafeQueue<T,Container>::front()
		{
			lock_guard_type guard(theMutex);
			return theQueue.front();
		}
			

		template<class T, class Container>
		typename ThreadSafeQueue<T,Container>::const_reference
			ThreadSafeQueue<T,Container>::front() const
		{
			ThreadSafeQueue &modSelf =
				const_cast<ThreadSafeQueue &>(*this);
			lock_guard_type guard(modSelf.theMutex);
			return theQueue.front();
		}

			
		template<class T, class Container>
		typename ThreadSafeQueue<T,Container>::reference
			ThreadSafeQueue<T,Container>::back()
		{
			lock_guard_type guard(theMutex);
			return theQueue.back();
		}
			

		template<class T, class Container>
		typename ThreadSafeQueue<T,Container>::const_reference
			ThreadSafeQueue<T,Container>::back() const
		{
			ThreadSafeQueue &modSelf =
				const_cast<ThreadSafeQueue &>(*this);
			lock_guard_type(modSelf.theMutex);
			return theQueue.back();
		}


		template<class T, class Container>
		bool ThreadSafeQueue<T,Container>::empty() const
		{
			ThreadSafeQueue &modSelf =
				const_cast<ThreadSafeQueue &>(*this);
			lock_guard_type guard(modSelf.theMutex);
			return theQueue.empty();
		}
			

		template<class T, class Container>
		typename ThreadSafeQueue<T,Container>::size_type
			ThreadSafeQueue<T,Container>::size() const
		{
			ThreadSafeQueue &modSelf =
				const_cast<ThreadSafeQueue &>(*this);
			lock_guard_type guard(modSelf.theMutex);
			return theQueue.size();
		}
			

		template<class T, class Container>
		void ThreadSafeQueue<T,Container>::push(const value_type &v)
		{
			lock_guard_type guard(theMutex);
			theQueue.push(v);
		}
			

		template<class T, class Container>
		void ThreadSafeQueue<T,Container>::push(value_type &&v)
		{
			lock_guard_type guard(theMutex);
			theQueue.push(v);
		}


		template<class T, class Container>
		void ThreadSafeQueue<T,Container>::pop()
		{
			lock_guard_type guard(theMutex);
			theQueue.pop();
		}
			

		template<class T, class Container>
		void ThreadSafeQueue<T,Container>::swap(ThreadSafeQueue &other)
		{
			lock_guard_type otherGuard(other.theMutex);
			lock_guard_type myGuard(theMutex);
			theQueue.swap(other.theQueue);
		}
			

		template<class T, class Container>
		std::ostream& ThreadSafeQueue<T,Container>::description(
			std::ostream &o) const
		{
			ClassFormatter *formatter = FormatterFactory::getFormatter();
			if(formatter != nullptr)
			{
				formatter->setClassName("ThreadSafeQueue");
				formatter->addClassMember<queue_type>("theQueue",
					theQueue);
				o << *formatter;
				delete formatter;
			}
		}


		template<class T, class Container>
		std::ostream& operator<<(std::ostream &o,
			const ThreadSafeQueue<T, Container> &q)
		{
			return q.description(o);
		}

	} // Foundation

} // TF

