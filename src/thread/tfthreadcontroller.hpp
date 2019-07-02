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


#ifndef TFTHREADCONTROLLER_HPP
#define TFTHREADCONTROLLER_HPP

#define NEEDS_OSTREAM
#define NEEDS_MUTEX
#define NEEDS_CONDITION_VARIABLE
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfmutex.hpp"
#include "tfconditionvariable.hpp"

namespace TF
{

    namespace Foundation
    {

        enum class WaitStatus
        {
            Signaled,
            NotSignaled,
            Expired
        };

        class ThreadController : public AllocatorInterface
        {
        private:
            using mutex_type = std::mutex;

            using conditional_variable_type = std::condition_variable;

            using lock_type = std::unique_lock<mutex_type>;

        public:
            using wait_status_type = WaitStatus;

            ThreadController(bool reset = false) : theSignal(false), autoReset(reset), theStop(false)
            {
            }

            wait_status_type wait()
            {
                lock_type theLock(theMutex);
                theConditional.wait(theLock);
                if(theSignal)
                {
                    if(autoReset)
                        theSignal = false;
                    return WaitStatus::Signaled;
                }
                return WaitStatus::NotSignaled;
            }

            template<class Predicate>
            wait_status_type wait(Predicate pred)
            {
                lock_type theLock(theMutex);
                theConditional.wait(theLock, pred);
                if(theSignal)
                {
                    if(autoReset)
                        theSignal = false;
                    return WaitStatus::Signaled;
                }
                return WaitStatus::NotSignaled;
            }


            template<class Rep, class Period>
            wait_status_type wait_for(const std::chrono::duration<Rep, Period> &relTime)
            {
                lock_type theLock(theMutex);
                auto waitResult = theConditional.wait_for(theLock, relTime);

                if(theSignal)
                {
                    if(autoReset)
                        theSignal = false;
                    return WaitStatus::Signaled;
                }

                if(waitResult == std::cv_status::timeout)
                    return WaitStatus::Expired;

                return WaitStatus::NotSignaled;
            }


            template<class Rep, class Period, class Predicate>
            wait_status_type wait_for(const std::chrono::duration<Rep, Period> &relTime, Predicate pred)
            {
                lock_type theLock(theMutex);
                auto waitResult = theConditional.wait_for(theLock, relTime, pred);

                if(theSignal)
                {
                    if(autoReset)
                        theSignal = false;
                    return WaitStatus::Signaled;
                }

                if(waitResult == std::cv_status::timeout)
                    return WaitStatus::Expired;

                return WaitStatus::NotSignaled;
            }


            template<class Clock, class Duration>
            wait_status_type wait_until(const std::chrono::time_point<Clock, Duration> &timeout_time)
            {
                lock_type theLock(theMutex);
                auto waitResult = theConditional.wait_until(theLock, timeout_time);

                if(theSignal)
                {
                    if(autoReset)
                        theSignal = false;
                    return WaitStatus::Signaled;
                }

                if(waitResult == std::cv_status::timeout)
                    return WaitStatus::Expired;

                return WaitStatus::NotSignaled;
            }


            template<class Clock, class Duration, class Predicate>
            wait_status_type wait_until(const std::chrono::time_point<Clock, Duration> &timeout_time, Predicate pred)
            {
                lock_type theLock(theMutex);
                auto waitResult = theConditional.wait_until(theLock, timeout_time, pred);

                if(theSignal)
                {
                    if(autoReset)
                        theSignal = false;
                    return WaitStatus::Signaled;
                }

                if(waitResult == std::cv_status::timeout)
                    return WaitStatus::Expired;

                return WaitStatus::NotSignaled;
            }


            void reset()
            {
                lock_type theLock(theMutex);
                theSignal = false;
            }


            void signal()
            {
                theConditional.notify_all();
            }


            void signalStop()
            {
                lock_type theLock(theMutex);
                theStop = true;
            }


            bool shouldStop()
            {
                lock_type theLock(theMutex);
                return theStop;
            }

            std::ostream &description(std::ostream &o) const;

        private:
            mutex_type theMutex;

            conditional_variable_type theConditional;

            bool theSignal;

            bool autoReset;

            bool theStop;
        };


        std::ostream &operator<<(std::ostream &, const ThreadController &c);

    }    // namespace Foundation

}    // namespace TF


#endif    // THFTHREADCONTROLLER_HPP
