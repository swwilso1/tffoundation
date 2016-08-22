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

#ifndef TFTHREADSAFEQUEUE_HXX
#define TFTHREADSAFEQUEUE_HXX

#define NEEDS_OSTREAM
#define NEEDS_MUTEX
#define NEEDS_DEQUE
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfqueue.hxx"
#include "tfmutex.hpp"

namespace TF
{
	
	namespace Foundation
	{

		/**
		 * @brief The ThreadSafeQueue class provides a container
		 * that gives queue semantics while also providing thread
		 * safety.  This means that multiple threads can access the
		 * queue simultaneously without causing data corruption
		 */
		template<class T, class Container = std::deque<T>>
		class ThreadSafeQueue : public AllocatorInterface
		{
		public:

			using container_type = Container;
			
			using value_type = typename Container::value_type;
			
			using size_type = typename Container::size_type;
			
			using reference = typename Container::reference;
			
			using const_reference = typename Container::const_reference;
			
			ThreadSafeQueue() {}

			explicit ThreadSafeQueue(const container_type &container);

			ThreadSafeQueue(const ThreadSafeQueue &other);
			
			ThreadSafeQueue(ThreadSafeQueue &&other);
			
			~ThreadSafeQueue();
			
			ThreadSafeQueue& operator=(const ThreadSafeQueue &other);
			
			ThreadSafeQueue& operator=(ThreadSafeQueue &&other);
			
			bool operator==(const ThreadSafeQueue &other) const;
			
			bool operator!=(const ThreadSafeQueue &other) const;
			
			bool operator<(const ThreadSafeQueue &other) const;
			
			bool operator<=(const ThreadSafeQueue &other) const;
			
			bool operator>=(const ThreadSafeQueue &other) const;
			
			bool operator>(const ThreadSafeQueue &other) const;
			
			reference front();
			
			const_reference front() const;
			
			reference back();
			
			const_reference back() const;
			
			bool empty() const;
			
			size_type size() const;
			
			void push(const value_type &v);
			
			void push(value_type &&v);
			
			template<typename... Args>
			void emplace(Args&&... args)
			{
				lock_guard_type guard(theMutex);
				theQueue.emplace(args...);
			}

			void pop();
			
			void swap(ThreadSafeQueue &other);
			
			std::ostream& description(std::ostream &o) const;
			
		private:
		
			using queue_type = Queue<T, Container>;
			
			using mutex_type = Mutex;
			
			using lock_guard_type = std::lock_guard<mutex_type>;
			
			queue_type theQueue;

			mutex_type theMutex;

		};
		
		
		template<class T, class Container = std::deque<T>>
		std::ostream& operator<<(std::ostream &o,
			const ThreadSafeQueue<T, Container> &q);



	} // Foundation

} // TF

#include "tfthreadsafequeue.cxx"

#endif // TFTHREADSAFEQUEUE_HXX

