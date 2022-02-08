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
#include <unordered_map>
#include <vector>
#include "TFFoundation.hpp"

using namespace TF::Foundation;

template<typename T>
void test_integer(const T & t)
{
    MultiValue i1{t};
    EXPECT_TRUE(i1.is_integer());
    EXPECT_TRUE(i1.is_type<T>());
    EXPECT_FALSE(i1.is_real());
    EXPECT_TRUE(i1.is_numeric());
    EXPECT_FALSE(i1.is_string());
    EXPECT_FALSE(i1.is_bool());
    auto i1_get_worked = i1.get<T>();
    EXPECT_TRUE(i1_get_worked);
    EXPECT_EQ(i1_get_worked.value(), t);
}

template<typename T>
void test_real(const T & t)
{
    MultiValue r{t};
    EXPECT_TRUE(r.is_real());
    EXPECT_TRUE(r.is_type<T>());
    EXPECT_FALSE(r.is_integer());
    EXPECT_FALSE(r.is_bool());
    EXPECT_FALSE(r.is_string());
    auto get_worked = r.get<T>();
    EXPECT_TRUE(get_worked);
    EXPECT_EQ(get_worked.value(), t);
}

TEST(MultiValue, BasicTest)
{
    MultiValue v{true};
    EXPECT_TRUE(v.is_type<bool>());
    EXPECT_TRUE(v.is_bool());
    EXPECT_FALSE(v.is_integer());
    EXPECT_FALSE(v.is_real());
    auto get_worked = v.get<bool>();
    EXPECT_TRUE(get_worked);
    EXPECT_TRUE(get_worked.value());

    // Test the integers
    int8_t ai1{INT8_MAX};
    test_integer(ai1);

    uint8_t ai2{UINT8_MAX};
    test_integer(ai2);

    int16_t ai3{INT16_MAX};
    test_integer(ai3);

    uint16_t ai4{UINT16_MAX};
    test_integer(ai4);

    int32_t ai5{INT32_MAX};
    test_integer(ai5);

    uint32_t ai6{UINT32_MAX};
    test_integer(ai6);

    int64_t ai7{INT64_MAX};
    test_integer(ai7);

    uint64_t ai8{UINT64_MAX};
    test_integer(ai8);

    float f{234.234f};
    test_real(f);

    double d{2223.232323};
    test_real(d);

    // Test a String
    String name{"Steven Wilson"};
    MultiValue s1{name};
    EXPECT_TRUE(s1.is_type<String>());
    EXPECT_FALSE(s1.is_numeric());
    EXPECT_TRUE(s1.is_string());
    auto s1_get_worked = s1.get<String>();
    EXPECT_TRUE(s1_get_worked);
    EXPECT_EQ(s1_get_worked.value(), name);
}

TEST(MultiValue, MapTests)
{
    std::unordered_map<String, MultiValue> map;
    map.insert(std::make_pair("one", 1));
    map.insert(std::make_pair("two", 2.0));
    map.insert(std::make_pair("three", 3L));
    map.insert(std::make_pair("pi", 3.1415f));
    map.insert(std::make_pair("false", false));
    map.insert(std::make_pair("string", "string"));

    EXPECT_EQ(map["one"], 1);
    EXPECT_EQ(map["two"], 2.0);
    EXPECT_EQ(map["three"], 3L);
    EXPECT_EQ(map["pi"], 3.1415f);
    EXPECT_EQ(map["false"], false);
    EXPECT_EQ(map["string"], "string");

    std::unordered_map<int, double> test_map;
    map.insert(std::make_pair("map", test_map));

    EXPECT_EQ(map["map"], test_map);

    std::unordered_map<int, MultiValue> multi_value_test_map;
    multi_value_test_map.insert(std::make_pair(1, "one"));
    multi_value_test_map.insert(std::make_pair(2, 2));
    map.insert(std::make_pair("multi-value-map", multi_value_test_map));

    EXPECT_EQ(map["multi-value-map"], multi_value_test_map);
}

TEST(MultiValue, ListTests)
{
    std::vector<MultiValue> multi_list;

    multi_list.push_back(1);
    multi_list.emplace_back(2.0f);
    multi_list.emplace_back(3.14159);
    multi_list.emplace_back(false);
    multi_list.emplace_back("string");
    multi_list.emplace_back(String{"Hello World"});

    EXPECT_EQ(multi_list[0], 1);
    EXPECT_EQ(multi_list[1], 2.0f);
    EXPECT_EQ(multi_list[2], 3.14159);
    EXPECT_EQ(multi_list[3], false);
    EXPECT_EQ(multi_list[4], "string");
    EXPECT_EQ(multi_list[5], String{"Hello World"});
}

struct Simple
{
    int a;
    double d;

    Simple() : a{}, d{} {}

    Simple(int ival, double dval) : a{ival}, d{dval} {}

    bool operator==(const Simple & s) const
    {
        return a == s.a && d == s.d;
    }
};

TEST(MultiValue, CustomTypes)
{
    Simple s{};
    MultiValue v{s};
    EXPECT_EQ(v, s);
}

TEST(MultiValue, AssignmentOperatorTests)
{
    MultiValue v{1};
    EXPECT_TRUE(v.is_integer());
    v = 10.023;
    EXPECT_TRUE(v.is_real());
    v = String{"Hello World"};
    EXPECT_TRUE(v.is_string());
    v = std::unordered_map<int, float>{};
    auto result = v.is_type<std::unordered_map<int, float>>();
    EXPECT_TRUE(result);
    v = std::vector<MultiValue>{};
    result = v.is_type<std::vector<MultiValue>>();
    EXPECT_TRUE(result);

    MultiValue q{10};
    int i = q;
    EXPECT_EQ(i, 10);
    double d{10.1};
    EXPECT_ANY_THROW(d = q);
    EXPECT_EQ(d, 10.1);
}

TEST(MultiValue, DereferenceTests)
{
    MultiValue v{};
    v = Simple{1, 2.3};
    EXPECT_EQ(v.pointer<Simple>()->a, 1);
}
