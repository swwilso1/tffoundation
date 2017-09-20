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

#ifndef TFTHREAD_HPP
#define TFTHREAD_HPP

#define NEEDS_OSTREAM
#define NEEDS_TYPE_TRAITS
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

// Very unix centric at the moment.
#include <pthread.h>

namespace TF
{

	namespace Foundation
	{
	
		class ThreadID;

		extern "C"
		{
			typedef void * (*thread_function)(void *);
		}

		class Thread : public AllocatorInterface
		{
		public:

			using id = ThreadID;

			using native_handle_type = pthread_t;

			using thread_function_type = thread_function;

			Thread();

			Thread(Thread &&t);

			Thread(thread_function_type f, void *arg);

			Thread(const Thread &t) = delete;

			~Thread();

			Thread& operator=(Thread &&t);

			bool joinable() const;

			id get_id() const;

			native_handle_type native_handle();

			static unsigned hardware_concurrency();

			void join();

			void detach();

			void swap(Thread &t);

			std::ostream& description(std::ostream &o) const;


		private:

			native_handle_type nativeHandle;
			bool handleValid;

		};


		std::ostream& operator<<(std::ostream &o, const Thread &t);

	} // Foundation
	

} // TF


#endif /* TFTHREAD_HPP */
