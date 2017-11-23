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

#include "tfalarmcenter.hpp"

namespace TF
{

    namespace Foundation
    {

        AlarmCenter::AlarmEntry::AlarmEntry()
        {
            // The controller type has elements that do not have copy constructors or
            // assignment operators.   So, in order to copy around a controller, we just
            // make one instances per thread and copy a pointer to it.
            controller = std::make_shared<thread_controller_type >(false);
        }

        AlarmCenter::AlarmEntry::AlarmEntry(const AlarmEntry &e)
        {
            controller = e.controller;
            thread = e.thread;
        }


        AlarmCenter::AlarmEntry& AlarmCenter::AlarmEntry::operator=(const AlarmEntry &e)
        {
            if(this != &e)
            {
                controller = e.controller;
                thread = e.thread;
            }

            return *this;
        }

        std::ostream& AlarmCenter::AlarmEntry::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("AlarmEntry");
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream& operator<<(std::ostream &o, const AlarmCenter::AlarmEntry &e)
        {
            return e.description(o);
        }


        AlarmCenter::pointer AlarmCenter::sharedCenter;

        bool AlarmCenter::initializedCenter = false;


        AlarmCenter::pointer AlarmCenter::defaultCenter()
        {
            if(! initializedCenter)
            {
                // Use new here (rather than std::make_shared),because AlarmCenter
                // constructor is a private class member.
                sharedCenter = std::shared_ptr<AlarmCenter>(new AlarmCenter());
            }

            return sharedCenter;
        }


        void AlarmCenter::setAlarm(const string_type &l, receiver_type r, date_type d)
        {
            lock_type lock(mapMutex);
            entry_type entry;

            // Create a pointer to a thread object so that we can copy the thread reference
            // around (std::thread doesn't support copy construction or assignment operations).
            // Use a lambda function to run the alarm thread.   Make sure to copy entry d, r, and
            // l in the lambda rather than use a reference.
            entry.thread = std::shared_ptr<std::thread>(new std::thread([this,entry,d,r,l](){

                // Assume the provided date is some time in the future.  So we calculate how long to
                // wait till we reach that time from now.
                auto timeToWait = d - date_type();

                // Use the controller wait_for method so that we can interrupt the wait if we need
                // to in the cancelAlarm method.
                auto status = entry.controller->wait_for(timeToWait);

                // The controller wait_for method will return Signaled when cancelAlarm tries to cancel
                // the method.
                if(status == WaitStatus::Signaled)
                    return;

                // The controller wait_for method will return Expired with the wait reaches the timeout.
                if(status == WaitStatus::Expired)
                {
                    this->alarmMap.erase(l);

                    // Call the callback function.
                    r(l);
                }
            }));

            // We must detach the alarm thread because the thread destructor checks the threads joinable
            // state otherwise and if it has not finished throws an exception.
            entry.thread->detach();

            alarmMap.insert(std::make_pair(l,entry));
        }


        void AlarmCenter::cancelAlarm(const string_type &l)
        {
            lock_type lock(mapMutex);
            if(alarmMap.count(l) > 0)
            {
                auto entry = alarmMap[l];

                // Signal the alarm thread to terminate early.
                entry.controller->signal();

                alarmMap.erase(l);
            }
        }


        std::ostream& AlarmCenter::description(std::ostream& o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("AlarmCenter");
                formatter->addClassMember<map_type>("alarmMap",alarmMap);
                formatter->addClassMember<bool>("initializedCenter", initializedCenter);
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream& operator<<(std::ostream &o, const AlarmCenter &c)
        {
            return c.description(o);
        }

    } // Foundation

} // TF

