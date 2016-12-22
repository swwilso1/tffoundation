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
SOFTWARE.


******************************************************************************/

#include "tfnotificationcenter.hpp"

namespace TF
{

    namespace Foundation
    {
        void * processNotifications(void *arg);

        NotificationCenter::pointer NotificationCenter::singleCenter = nullptr;

        NotificationCenter::mutex_type NotificationCenter::singleMutex;

        NotificationCenter::NotificationCenter()
        {
            threadHandle = thread_type(processNotifications, reinterpret_cast<void *>(this));
        }


        NotificationCenter::~NotificationCenter()
        {
            threadController.signalStop();
            threadController.signal();
            threadHandle.join();
        }


        NotificationCenter::pointer NotificationCenter::DefaultCenter()
        {
            lock_type theLock(singleMutex);
            if(singleCenter == nullptr)
            {
                singleCenter = pointer(new NotificationCenter());
            }
            return singleCenter;
        }


        void NotificationCenter::registerForNotificationFromSender(const label_type &label, sender_type sender,
                                               const label_type &cblabel, callback_type callback)
        {
            // We lock the mutex here so that the notification sending thread can't access the table at the same
            // time we are updating it.
            lock_type theLock(singleMutex);

            if(handlerMap.count(label) > 0)
            {
                // We have callback map for this message, now we need to check to see if we already have an
                // entry for this callback label.
                auto &callbackMap = handlerMap[label];

                if(callbackMap.count(cblabel) > 0)
                {
                    // We have an entry so now we probably need to add the sender object to the list of senders
                    // that we pay attention to for this callback.  Of course the sender might be nullptr so in
                    // that case we do nothing.
                    auto &handlerEntry = callbackMap[cblabel];

                    if(sender != nullptr)
                    {
                        // Check to make sure the sender is not already in the list.
                        if(listHasSender(handlerEntry.senders, sender))
                            handlerEntry.senders.push_back(sender);
                    }
                }
                else
                {
                    // Our callback map doesn't have this labeled callback yet, so we need to create a new
                    // entry in the map for this callback.
                    callbackMap.insert(std::make_pair(cblabel, handler_entry_type(callback, sender)));
                }
            }
            else
            {
                // We don't have a callback map yet for this notfication label, so set up the whole kit-n-kaboodle
                handler_label_map callbackMap;

                callbackMap.insert(std::make_pair(cblabel, handler_entry_type(callback, sender)));

                handlerMap.insert(std::make_pair(label, callbackMap));
            }
        }


        void NotificationCenter::registerForNotification(const label_type &label, const label_type &cblabel,
                                     callback_type callback)
        {
            registerForNotificationFromSender(label, nullptr, cblabel, callback);
        }



        void NotificationCenter::removeRegistrationForNotification(const label_type &label, const label_type &cblabel)
        {
            // Lock access to the data structures so that this operation doesn't conflict with the notification sending
            // thread.
            lock_type theLock(singleMutex);

            // Check to see if we event have an entry for this notification label.
            if(handlerMap.count(label) > 0)
            {
                // Now check to see if we have an entry in the callback map for this notfication label.
                auto &callbackMap = handlerMap[label];

                if(callbackMap.count(cblabel) > 0)
                    callbackMap.erase(cblabel);
            }
        }


        void NotificationCenter::postNotification(const notification_type &n)
        {
            notificationQueue.push(n);
            threadController.signal();
        }

        std::ostream& NotificationCenter::description(std::ostream &o) const
        {
            return o;
        }


        void NotificationCenter::sendNotifications()
        {
            while(! notificationQueue.empty())
            {
                auto &notification = notificationQueue.front();
                sendNotification(notification);
                notificationQueue.pop();
            }
        }


        void NotificationCenter::sendNotification(const notification_type &n)
        {
            // Lock access to the handler data structures so that it doesn't
            // change while we pass through it here.
            lock_type theLock(singleMutex);

            if(handlerMap.count(n.label()) > 0)
            {
                // We have a handler callback map for this notification label.
                auto &callbackMap = handlerMap[n.label()];

                // Now iterate through the map and try to send the actual notifications.
                for(auto &entry : callbackMap)
                {
                    if((n.sender() != nullptr && listHasSender(entry.second.senders, n.sender()))
                        || entry.second.senders.size() == 0)
                        entry.second.handler(n);
                }
            }
        }


        bool NotificationCenter::listHasSender(const sender_list_type &l, sender_type sender)
        {
            for(auto s : l)
            {
                if (s == sender)
                    return true;
            }

            return false;
        }


        std::ostream& operator<<(std::ostream &o, const NotificationCenter &c)
        {
            return c.description(o);
        }


        void * processNotifications(void *arg)
        {
            NotificationCenter *theCenter = reinterpret_cast<NotificationCenter *>(arg);

            if(theCenter == nullptr)
                return nullptr;

            while(! theCenter->threadController.shouldStop())
            {
                theCenter->sendNotifications();
                theCenter->threadController.wait();
            }

            return nullptr;
        }


    } // Foundation

} // TF

