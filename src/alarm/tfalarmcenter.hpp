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

#ifndef TFALARMCENTER_HPP
#define TFALARMCENTER_HPP

#define NEEDS_OSTREAM
#define NEEDS_FUNCTIONAL
#define NEEDS_STRING
#define NEEDS_MAP
#define NEEDS_THREAD
#define NEEDS_MEMORY
#define NEEDS_CHRONO
#define NEEDS_MUTEX
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfmap.hxx"
#include "tfdateclocks.hpp"
#include "tfthreadcontroller.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * The AlarmCenter class provides a facility for setting a notifying alarm for an arbitrary time.
         * The AlarmCenter has two relevant methods, setAlarm and cancelAlarm.   The setAlarm method takes
         * an alarm label, a callback function and a expiration time.   When the alarm expires, the alarm will
         * call the callback function.   Use the cancelAlarm method to cancel an alarm before it expires.
         *
         * A process has only one AlarmCenter.   Use the defaultCenter() method to get access to the shared
         * AlarmCenter.
         */
        class AlarmCenter : public AllocatorInterface
        {
        public:

            /** @brief basic string type */
            using string_type = std::string;

            /**
             * @brief basic function callback type.   The callback can be a function pointer or a lambda.
             */
            using receiver_type = std::function<void(const string_type &label)>;;

            /**
             * @brief pointer type used to refer to an AlarmCenter instance.
             */
            using pointer = std::shared_ptr<AlarmCenter>;

            /**
             * @brief use SystemDates for the time aspect.
             */
            using date_type = SystemDate;

            ~AlarmCenter() = default;

            /**
             * @brief method to retrieve the shared AlarmCenter instance.
             * @return the AlarmCenter instance.
             */
            static pointer defaultCenter();

            /**
             * @brief method to schedule an alarm.
             * @param l the alarm label.
             * @param r the callback function
             * @param d the time in the future that the alarm should expire.
             */
            void setAlarm(const string_type &l, receiver_type r, date_type d);

            /**
             * @brief method to cancel an existing alarm that has not expired.
             * @param l the alarm label to cancel.
             */
            void cancelAlarm(const string_type &l);

            /**
             * @brief method for writing an AlarmCenter object to a stream
             * @param o the stream object
             * @return the stream object @e o.
             */
            std::ostream& description(std::ostream& o) const;

        private:

            /**
             * The AlarmEntry class provides handles to the thread controller and the thread
             * that runs the alarm.
             */
            struct AlarmEntry : public AllocatorInterface
            {
                /** @brief thread controller used to manage the alarm thread. */
                using thread_controller_type = ThreadController;

                /** @brief shared reference to the thread controller so the controller can be copied */
                std::shared_ptr<thread_controller_type> controller;

                /** @brief shared reference to the thread handle so we can effectively copy the handle */
                std::shared_ptr<std::thread> thread;

                /**
                 * @brief default constructor.
                 */
                AlarmEntry();

                /**
                 * @brief copy constructor
                 * @param e the other AlarmEntry.
                 */
                AlarmEntry(const AlarmEntry &e);

                /**
                 * @brief copy assignment operator
                 * @param e the other AlarmEntry
                 * @return this AlarmEntry updated with the contents of @e e.
                 */
                AlarmEntry& operator=(const AlarmEntry &e);

                /**
                 * @brief method to write an AlarmEntry object to a stream.
                 * @param o the stream object.
                 * @return the stream object @e o.
                 */
                std::ostream& description(std::ostream &o) const;
            };


            /**
             * declaration of friendship with operator<< for AlarmEntry class.  Needed
             * because AlarmEntry is a private internal class.
             */
            friend std::ostream& operator<<(std::ostream &o, const AlarmEntry &e);

            /** @brief entry type for the alarm map */
            using entry_type = AlarmEntry;

            /** @brief map type that maps labels to alarm entries. */
            using map_type = Map<string_type, entry_type>;

            /** @brief basic mutex type */
            using mutex_type = std::mutex;

            /** @brief basic lock type */
            using lock_type = std::unique_lock<mutex_type>;

            /**
             * @brief default constructor
             *
             * Private default constructor to prevent type declarations or allocations of the
             * AlarmCenter object.   Instead we share the AlarmCenter instance and must access it
             * throught the defaultCenter() method.
             */
            AlarmCenter() = default;

            /**
             * @brief the map of currently set alarms.
             *
             * Keep a map of currently set alarms so that we can cancel an alarm if necessary.
             */
            map_type alarmMap;


            /** @brief use a mutex to protect access to @e alarmMap from multiple threads */
            mutex_type mapMutex;


            /**
             * @brief the pointer to the shared AlarmCenter.
             */
            static pointer sharedCenter;

            /**
             * @brief indicator of whether we have created the shared AlarmCenter.
             */
            static bool initializedCenter;
        };


        /**
         * Overloaded operator<< for AlarmEntry objects.
         * @param o the stream object.
         * @param e the AlarmEntry object.
         * @return the stream object
         *
         * Writes the AlarmEntry contents to the stream and returns the stream object.
         */
        std::ostream& operator<<(std::ostream &o, const AlarmCenter::AlarmEntry &e);


        /**
         * Overloaded operator<< for AlarmCenter objects.
         * @param o the stream object
         * @param c the AlarmCenter object.
         * @return the stream object.
         */
        std::ostream& operator<<(std::ostream &o, const AlarmCenter &c);

    } // Foundation

} // TF


#endif //TFALARMCENTER_HPP
