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


#include "TFFoundation.hpp"
#include "gtest/gtest.h"


using namespace TF::Foundation;

TEST(EnvironmentTest, BasicEnvironmentSettingsTest)
{
    EnvironmentSettings es;
    EXPECT_NE(es.numberOfVariables(), 0);
    EXPECT_TRUE(es.hasVariable("HOME"));
    EXPECT_TRUE(es.hasVariable("USER"));
}


TEST(EnvironmentTest, GetValueForVariableTest)
{
    EnvironmentSettings es;
    EXPECT_NO_THROW(es.getValueForVariable("HOME"));
    EXPECT_NO_THROW(es.getValueForVariable("USER"));
}


TEST(EnvironmentTest, SetValueForVariableTest)
{
    EnvironmentSettings es;
    es.setValueForVariable("HORSE", "THOR");
    EXPECT_EQ(es.getValueForVariable("HORSE"), "THOR");
}


TEST(EnvironmentTest, AddToValueForVariableTest)
{
    EnvironmentSettings es;
    es.setValueForVariable("HORSE", "THOR");
    es.addToValueForVariable("HORSE", "MJOLNIR");
    EXPECT_EQ(es.getValueForVariable("HORSE"), "THOR:MJOLNIR");
}


TEST(EnvironmentTest, RemoveVariableTest)
{
    EnvironmentSettings es;
    es.setValueForVariable("HORSE", "THOR");
    EXPECT_TRUE(es.hasVariable("HORSE"));
    es.removeVariable("HORSE");
    EXPECT_FALSE(es.hasVariable("HORSE"));
}


TEST(EnvironmentTest, NumberOfVariablesTest)
{
    EnvironmentSettings es;
    auto numVariables = es.numberOfVariables();
    es.setValueForVariable("HORSE", "THOR");
    EXPECT_EQ(es.numberOfVariables(), numVariables + 1);
}


TEST(EnvironmentTest, EnvironmentVariablesTest)
{
    EnvironmentSettings es;
    auto variables = es.environmentVariables();
    EXPECT_EQ(variables.size(), es.numberOfVariables());
}
