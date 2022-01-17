/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

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

// This implementation is currently very Unix centric.

#define NEEDS_SYSTEM_ERROR
#include "tfheaders.hpp"
#include "tfthread.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        class ThreadID : public AllocatorInterface
        {
        public:
            ThreadID() noexcept
            {
            }
        };


        Thread::Thread() : nativeHandle(), handleValid(false)
        {
        }


        Thread::Thread(Thread &&t) : nativeHandle(), handleValid(false)
        {
            nativeHandle = t.nativeHandle;
            handleValid = true;
            t.handleValid = false;
        }


        Thread::Thread(thread_function_type f, void *arg) : nativeHandle(), handleValid(false)
        {
            int apiResult = pthread_create(&nativeHandle, nullptr, f, arg);
            if(apiResult != 0)
            {
                throw std::system_error(apiResult, std::system_category(), "thread constructor error");
            }

            handleValid = true;
        }


        Thread::~Thread()
        {
        }


        Thread &Thread::operator=(Thread &&t)
        {
            if(this != &t)
            {
                nativeHandle = t.nativeHandle;
                handleValid = true;
                t.handleValid = false;
            }

            return *this;
        }


        bool Thread::joinable() const
        {
            return handleValid;
        }


        Thread::id Thread::get_id() const
        {
            return id();
        }


        Thread::native_handle_type Thread::native_handle()
        {
            return nativeHandle;
        }


        unsigned Thread::hardware_concurrency()
        {
            // TODO: implement system inspection of the
            // number of processors on a machine.
            return 1;
        }


        void Thread::join()
        {
            if(handleValid)
            {
                int apiResult = pthread_join(nativeHandle, nullptr);
                if(apiResult == 0)
                    handleValid = false;
            }
        }


        void Thread::detach()
        {
            if(handleValid)
            {
                int apiResult = pthread_detach(nativeHandle);

                if(apiResult == 0)
                    handleValid = 0;
            }
        }


        void Thread::swap(Thread &t)
        {
            native_handle_type tmpHandle = t.nativeHandle;
            bool tmpValid = t.handleValid;

            t.nativeHandle = nativeHandle;
            t.handleValid = handleValid;

            nativeHandle = tmpHandle;
            handleValid = tmpValid;
        }


        std::ostream &Thread::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("Thread");
                formatter->addClassMember<bool>("handleValid", handleValid);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const Thread &t)
        {
            return t.description(o);
        }

    }    // namespace Foundation


}    // namespace TF
