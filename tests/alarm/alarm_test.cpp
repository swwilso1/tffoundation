/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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


#include <thread>
#include <chrono>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"


using namespace TF::Foundation;

TEST(AlarmTest, SimpleAlarmTest)
{
    bool alarmTriggered = false;
    auto theCenter = AlarmCenter::defaultCenter();
    SystemDate now;
    theCenter->setAlarm(
        "SimpleAlarm", [&alarmTriggered](const AlarmCenter::string_type &s) { alarmTriggered = true; },
        now + duration_cast<SystemDate::duration>(100ms));
    std::this_thread::sleep_for(2s);
    EXPECT_TRUE(alarmTriggered);
}


TEST(AlarmTest, CancelTest)
{
    bool alarmTriggered = false;
    auto theCenter = AlarmCenter::defaultCenter();
    SystemDate now;
    theCenter->setAlarm(
        "ExtendedAlarm", [&alarmTriggered](const AlarmCenter::string_type &s) { alarmTriggered = true; },
        now + duration_cast<SystemDate::duration>(2s));
    theCenter->cancelAlarm("ExtendedAlarm");
    EXPECT_FALSE(alarmTriggered);
}
