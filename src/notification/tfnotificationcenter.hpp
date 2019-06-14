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
SOFTWARE.


******************************************************************************/

#ifndef TFNOTIFICATIONCENTER_HPP
#define TFNOTIFICATIONCENTER_HPP

#define NEEDS_OSTREAM
#define NEEDS_FUNCTIONAL
#define NEEDS_MEMORY
#define NEEDS_MAP
#define NEEDS_LIST
#define NEEDS_MUTEX
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfnotification.hpp"
#include "tfthread.hpp"
#include "tfthreadsafequeue.hxx"
#include "tfthreadcontroller.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * @brief Notification Center class that acts as a central notification
         * hub for a process.  Objects can register as observers of other objects
         * and receive notifications when the observed object posts a notification
         * to the notification center.
         *
         * The observers can register for all notifications of a particular type or
         * they can register for notifications of a type from a particular object.
         *
         * In this incarnation of a notification center, the notification center doesn't
         * really notify individual observer objects.   Instead, the interface supports
         * passing in a lambda function to execute when a notification occurs.  That lambda
         * object allows more general things to occur when a notification is postend than
         * just updating a particular object.
         *
         * The NotificationCenter class implements the singleton pattern where only Notification
         * Center is allowed per process.
         */
        class NotificationCenter : public AllocatorInterface
        {
        public:

            /** @brief notification label type */
            using label_type = String;

            /** @brief notification type */
            using notification_type = Notification;

            /** @brief lambda callback type */
            using callback_type = std::function<void(const notification_type &)>;

            /** brief sender type */
            using sender_type = Notification::object_type;

            /** @brief notification center pointer type */
            using pointer = std::shared_ptr<NotificationCenter>;


            /** @brief destructor */
            ~NotificationCenter();


            /**
             * @brief get the singleton notification center.
             * @return a pointer to the singleton center.
             */
            static pointer DefaultCenter();


            /**
             * @brief Register a callback for a specific notification from a specific object.
             * @param label the notification label
             * @param sender the sender of the notification
             * @param cblabel the label of the callback
             * @param callback the callback handler to call to handle the notification
             *
             * The sender may be a nullptr referring to any arbitrary object that sends the @e label notification.
             * The cblabel parameter is used to identify the specific callback handler in case it needs
             * removed at some later point.
             */
            void registerForNotificationFromSender(const label_type &label, sender_type sender,
                                                  const label_type &cblabel, callback_type callback);


            /**
             * @brief Register a callback handler for any object that posts a notification
             * @param label the notification label
             * @param cblabel the handler label
             * @param callback the callback handler to call to handle the notification.
             */
            void registerForNotification(const label_type &label, const label_type &cblabel,
                                         callback_type callback);



            void removeRegistrationForNotification(const label_type &label, const label_type &cblabel);


            /**
             * @brief post a notification to the center
             * @param n
             */
            void postNotification(const notification_type &n);


            /**
             * @brief method to help write the contents of the notification center to a stream.
             * @param o the stream object
             * @return the stream object @e o
             */
            std::ostream& description(std::ostream &o) const;

        private:

            friend void * processNotifications(void *arg);


            /** @brief list type for list of senders */
            using sender_list_type = std::list<sender_type>;


            /**
             * @brief simple object to hold a callback handler
             * and the senders it watches.
             */
            struct HandlerEntry : public AllocatorInterface
            {
                callback_type handler;
                sender_list_type senders;

                HandlerEntry() {}

                HandlerEntry(callback_type h, sender_type s)
                {
                    handler = h;
                    if(s != nullptr)
                        senders.push_back(s);
                }
            };

            /** @brief handler entry type */
            using handler_entry_type = HandlerEntry;

            /** @brief map of handler labels to handler entries. */
            using handler_label_map = std::map<label_type, handler_entry_type>;

            /** @brief map of message labels to handler maps */
            using message_label_map = std::map<label_type, handler_label_map>;

            /** @brief mutex type for protecting access to the singleton notification center
             * from multiple threads.
             */
            using mutex_type = std::mutex;


            /** @brief lock type for locking the mutex */
            using lock_type = std::unique_lock<mutex_type>;

            /** @brief queue type for sending notifications to the callback handler thread. */
            using queue_type = ThreadSafeQueue<notification_type>;

            /** @brief thread object type for running the callback handler thread */
            using thread_type = Thread;

            /** @brief thread controller object for handling events with callback handler thread */
            using thread_controller_type = ThreadController;

            /** @brief primary mapping data structure of message labels to handlers */
            message_label_map handlerMap;

            /** @brief queue for passing notifications to the processing thread. */
            queue_type notificationQueue;


            /** @brief thread handle for the callback handler thread */
            thread_type threadHandle;

            /** @brief thread controller object for the callback handler thread */
            thread_controller_type threadController;


            /** @brief shared pointer to the singleton instance of the notification center */
            static pointer singleCenter;

            /** @brief shared mutex for accessing the singleton notification center */
            static mutex_type singleMutex;

            /** @brief mutex for access thing pointer to the notification center singleton */
            static mutex_type singletonMutex;

            /**
             * @brief private default constructor so that users must access the notification center
             * through the DefaultCenter method.
             */
            NotificationCenter();

            /**
             * @brief method to iterate through the current queue of notifications and dispatch them.
             */
            void sendNotifications();


            /**
             * @brief method to send an individual notification
             * @param n the notification
             */
            void sendNotification(const notification_type &n);


            /**
             * @brief helper function to see if a sender exists in a sender list
             * @param l the sender list
             * @param sender the sender to find
             * @return true if the list has the sender and false otherwise.
             */
            bool listHasSender(const sender_list_type &l, sender_type sender);
        };


        /**
         * @brief overloaded << operator for NotificationCenter objects
         * @param o the stream object
         * @param c the NotificationCenter object
         * @return the stream object @e o
         */
        std::ostream & operator<<(std::ostream &o, const NotificationCenter &c);

    } // Foundation

} // TF

#endif // TFNOTIFICATIONCENTER_HPP
