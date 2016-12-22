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

#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(NotificationTest, DefaultCenterTest)
{
    NotificationCenter::pointer theCenter = NotificationCenter::DefaultCenter();
    EXPECT_TRUE(theCenter.get() != nullptr);
}


TEST(NotificationTest, RegisterCallbackHandlerForSenderTest)
{
    NotificationCenter::pointer theCenter = NotificationCenter::DefaultCenter();
    theCenter->registerForNotificationFromSender("T1", nullptr, "foo_handler1",
        [](const Notification &n){
            std::cout << n << std::endl;
        });
}


TEST(NotificationTest, RegisterCallbackHandlerTest)
{
    NotificationCenter::pointer theCenter = NotificationCenter::DefaultCenter();
    theCenter->registerForNotification("T2", "foo_handler2", [](const Notification &n){
        std::cout << n << std::endl;
    });
}


TEST(NotificationTest, FullNotificationTest)
{
    NotificationCenter::pointer theCenter = NotificationCenter::DefaultCenter();
    bool succeeded = false;
    ThreadController controller;

    theCenter->registerForNotification("T3","foo_handler3",[&succeeded, &controller](const Notification &n){

        if(n.label() == "T3")
        {
            succeeded = true;
            controller.signal();
        }
    });

    theCenter->postNotification(Notification("T3"));

    controller.wait();

    EXPECT_EQ(true, succeeded);

}