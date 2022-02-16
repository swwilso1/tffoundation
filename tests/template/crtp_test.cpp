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

#include "gtest/gtest.h"
#include "TFFoundation.hpp"

using namespace TF::Foundation;

template<typename T>
struct Square : CRTP<T, Square>
{
    void square()
    {
        this->underlying().setValue(this->underlying().getValue() * this->underlying().getValue());
    }
};

template<typename T>
struct Scale : CRTP<T, Scale>
{
    void scale(double multiplier)
    {
        this->underlying().setValue(this->underlying().getValue() * multiplier);
    }
};

class Thing : public Scale<Thing>, public Square<Thing>
{
public:
    Thing() : m_value(0.0) {}

    double getValue() const
    {
        return m_value;
    }
    void setValue(double value)
    {
        m_value = value;
    }

private:
    double m_value;
};

TEST(CRTP, compiles)
{
    Thing thing;
    thing.setValue(10);
    thing.square();
    EXPECT_EQ(thing.getValue(), 100.0);
    thing.scale(0.5);
    EXPECT_EQ(thing.getValue(), 50.0);
}
