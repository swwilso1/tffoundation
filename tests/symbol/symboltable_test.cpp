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
SOFTWARE.


******************************************************************************/

#include <set>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

struct SimpleType
{
    int a;
};

template<typename T>
class ComplicatedType
{
public:
    using value_type = T;

    ComplicatedType()
    {
    }

    ComplicatedType(const value_type &v) : value(v)
    {
    }

    ComplicatedType &operator=(const ComplicatedType &t)
    {
        if(this != &t)
        {
            value = t.value;
        }
        return *this;
    }

    bool operator==(const ComplicatedType &t)
    {
        return value == t.value;
    }

private:
    value_type value;
};

TEST(SymbolTableTest, AddAndGetTest)
{
    SymbolTable<String> theTable;
    theTable.setValueForKey<int>("key", 10);
    int i = 0;
    theTable.getValueForKey<int>("key", i);
    EXPECT_EQ(i, 10);

    theTable.setValueForKey<double>("double", 10.2);
    double d = 0.0;
    theTable.getValueForKey<double>("double", d);
    EXPECT_EQ(d, 10.2);

    theTable.setValueForKey<String>("string", "value");
    String s;
    theTable.getValueForKey<String>("string", s);
    EXPECT_EQ(s, "value");

    SimpleType a, b;
    a.a = 100;
    theTable.setValueForKey<SimpleType>("type", a);
    theTable.getValueForKey<SimpleType>("type", b);
    EXPECT_EQ(a.a, b.a);

    ComplicatedType<float> c(3.2), e;
    theTable.setValueForKey<ComplicatedType<float>>("ctype", c);
    theTable.getValueForKey("ctype", e);
    EXPECT_TRUE(c == e);
}

TEST(SymbolTableTest, BadRetrievalTest)
{
    SymbolTable<int> theTable;

    theTable.setValueForKey<int>(1, 10);
    double d = 10.0;
    auto result = theTable.getValueForKey<double>(1, d);
    EXPECT_EQ(d, 10);
    EXPECT_FALSE(result);
}

TEST(SymbolTableTest, RemoveValueTest)
{
    SymbolTable<double> theTable;
    double d = 0;

    while(d < 100.0)
    {
        theTable.setValueForKey<int>(d, 1);
        theTable.removeValueForKey(d);
        EXPECT_FALSE(theTable.hasValueForKey(d));
        d += 1;
    }

    EXPECT_EQ(theTable.size(), 0);
}

TEST(SymbolTableTest, ClearTest)
{
    SymbolTable<int> theTable;
    int i = 0;

    while(i < 100)
    {
        theTable.setValueForKey<int>(i, 1);
        i++;
    }

    theTable.clear();
    EXPECT_EQ(theTable.size(), 0);
}

TEST(SymbolTableTest, SizeTest)
{
    SymbolTable<int> theTable;
    int i = 0;

    while(i < 15000)
    {
        theTable.setValueForKey<int>(i, 1);
        EXPECT_EQ(theTable.size(), i + 1);
        i++;
    }

    theTable.clear();
    EXPECT_EQ(theTable.size(), 0);
}

TEST(SymbolTableTest, KeyListTest)
{
    std::set<String> keySet;
    SymbolTable<String> theTable;

    theTable.setValueForKey<int>("key1", 10);
    theTable.setValueForKey<int>("key2", 11);
    theTable.setValueForKey<int>("key3", 12);
    theTable.setValueForKey<int>("key4", 13);
    theTable.setValueForKey<int>("key5", 14);

    keySet.insert("key1");
    keySet.insert("key2");
    keySet.insert("key3");
    keySet.insert("key4");
    keySet.insert("key5");

    SymbolTable<String>::key_list_type keyList;

    keyList = theTable.keys();

    for(auto &key : keyList)
    {
        EXPECT_TRUE(keySet.count(key) > 0);
    }
}