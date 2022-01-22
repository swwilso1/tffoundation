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
SOFTWARE.


******************************************************************************/

#include "TFFoundation.hpp"
#include "gtest/gtest.h"
#include <iostream>

using namespace TF::Foundation;

TEST(PollTest, BasicTest)
{
    Poller poller;
    FileManager manager;
    std::chrono::seconds secs {5};
    bool triggered {false};

    auto file_name = manager.temporaryDirectory() + FileManager::pathSeparator + "polltest.log";

    auto handle = FileHandle::fileHandleForWritingAtPath(file_name);
    auto descriptor = handle.fileDescriptor();
    poller.add_handle(descriptor, PollEvent::Write, [&triggered](int h) { triggered = true; });
    poller.wait_for(secs);
    EXPECT_TRUE(triggered);
    EXPECT_TRUE(poller.handle_set_for_event(descriptor, PollEvent::Write));

    char byte_array[100];
    memset(byte_array, 0, 100 * sizeof(char));
    Data basic_data {byte_array, 100};
    handle.writeData(basic_data);
    handle.closeFile();

    poller.clear();

    auto handle2 = FileHandle::fileHandleForReadingAtPath(file_name);
    auto descriptor2 = handle2.fileDescriptor();
    triggered = false;
    poller.add_handle(descriptor2, PollEvent::Read, [&triggered](int h) { triggered = true; });
    poller.wait_for(secs);
    EXPECT_TRUE(triggered);
    EXPECT_TRUE(poller.handle_set_for_event(descriptor, PollEvent::Read));

    manager.removeItemAtPath(file_name);
}
