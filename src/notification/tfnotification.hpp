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

#ifndef TFNOTIFICATION_HPP
#define TFNOTIFICATION_HPP

#define NEEDS_OSTREAM
#define NEEDS_STREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdata.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * @brief notification object that represents a notification message sent in
         * a system.
         *
         * Notifications are designed so that an object sends a notification to a notification
         * center.   The notification contains the notification message and the sender of the
         * notification.
         */
        class Notification : public AllocatorInterface
        {
        public:
            /** @brief basic string type */
            using string_type = String;

            /** @brief notification label type */
            using notification_label = string_type;

            /** @brief notification data type */
            using notification_data = Data;

            /** @brief object type to use for the sender */
            using object_type = void *;


            /**
             * @brief Default constructor
             */
            Notification();


            /**
             * @brief basic message constructor
             * @param l the notification label
             * @param d the notification data
             * @param s the sending object
             */
            Notification(const notification_label &l, const notification_data &d, object_type s);


            /**
             * @brief message consructor without data object.
             * @param l the label.
             * @param s the sender.
             */
            Notification(const notification_label &l, object_type s);


            /**
             * @brief message constructor with label and data, but no sender
             * @param l the label
             * @param d the data
             */
            Notification(const notification_label &l, const notification_data &d);


            /**
             * @brief message constructor without data object or sender
             * @param l the label.
             */
            Notification(const notification_label &l);


            /**
             * @brief copy constructor
             * @param n the other notification
             */
            Notification(const Notification &n);


            /**
             * @brief rvalue constructor
             * @param n the other notification
             */
            Notification(Notification &&n);


            /**
             * @brief destructor
             */
            ~Notification();


            /**
             * @brief assignment operator
             * @param n the other Notification
             * @return this object updated with the contents of @e n
             */
            Notification &operator=(const Notification &n);


            /**
             * @brief rvalue assignment operator
             * @param n the other Notification
             * @return this object with the exact contents of &e n
             */
            Notification &operator=(Notification &&n);


            /**
             * @brief equality operator
             * @param n the other Notification
             * @return true if the two notifications are equal and false otherwise.
             */
            bool operator==(const Notification &n);


            /**
             * @brief inequality operator
             * @param n the other Notification
             * @return true if the two notifications are not equal and false otherwise.
             */
            bool operator!=(const Notification &n);


            /**
             * @brief method to get the notification label
             * @return a copy of the notification label
             */
            notification_label label() const
            {
                return theLabel;
            }


            /**
             * @brief method to get the notification data
             * @return a copy of the notification data
             */
            notification_data data() const
            {
                return theData;
            }


            /**
             * @brief method to get the sender object
             * @return the notification sender
             */
            object_type sender() const
            {
                return theSender;
            }


            /**
             * @brief helper method for writing the method to stream
             * @param o the stream object
             * @return the stream object @e o.
             */
            std::ostream &description(std::ostream &o) const;

        private:
            /** @brief the label */
            notification_label theLabel;

            /** @brief the data */
            notification_data theData;

            /** @brief the sender */
            object_type theSender;
        };


        /**
         * @brief overloaded << operator for Notification objects.
         * @param o the stream object
         * @param n the notification object
         * @return @e o the stream object
         */
        std::ostream &operator<<(std::ostream &o, const Notification &n);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFNOTIFICATION_HPP
