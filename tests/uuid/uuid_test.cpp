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

#include <unordered_map>
#include "gtest/gtest.h"
#include "TFFoundation.hpp"

using namespace TF::Foundation;

TEST(UUIDTest, BasicConstructorTest)
{
    UUID theID;
    String idAsString = theID.toString();
    EXPECT_EQ(idAsString.length(), UUID::uuidStringLength);
}

TEST(UUIDTest, CopyConstructorTest)
{
    UUID theID;
    UUID theNextID(theID);
    EXPECT_EQ(theID, theNextID);
}

UUID testHelper()
{
    return UUID();
}

TEST(UUIDTest, RValueConstructorTest)
{
    UUID theID(testHelper());
    String idAsString = theID.toString();
    EXPECT_EQ(idAsString.length(), UUID::uuidStringLength);
}

TEST(UUIDTest, AssignmentOperatorTest)
{
    UUID theUUID;
    UUID theOtherUUID;
    theOtherUUID = theUUID;
    EXPECT_EQ(theUUID, theOtherUUID);
}

TEST(UUIDTest, RValueAssignementOperatorTest)
{
    UUID theUUID = testHelper();
    EXPECT_EQ(theUUID.toString().length(), UUID::uuidStringLength);
}

TEST(UUIDTest, StringAssigmentOperatorTest)
{
    UUID theUUID{};
    theUUID = UUID::string_type{"9c017f6f-6dae-4bad-a18f-51a512bb83ef"};
    EXPECT_EQ(theUUID.toString(), "9C017F6F-6DAE-4BAD-A18F-51A512BB83EF");
}

TEST(UUIDTest, OperatorEqualTest)
{
    auto theUUID = UUID::fromString("7EB28421-0C05-4E4B-8086-3AC98133F016");
    auto theOtherUUID = UUID::fromString("7EB28421-0C05-4E4B-8086-3AC98133F016");

    EXPECT_TRUE(theUUID);
    EXPECT_TRUE(theOtherUUID);
    EXPECT_EQ(theUUID.value(), theOtherUUID.value());
}

TEST(UUIDTest, StringOperatorEqualTest)
{
    auto theUUID = UUID::fromString("4AF5744D-A5F3-4F10-8DDC-1563F6A58796");
    EXPECT_TRUE(theUUID);
    EXPECT_TRUE(theUUID.value() == "4AF5744D-A5F3-4F10-8DDC-1563F6A58796");
    EXPECT_FALSE(theUUID.value() == "12345");
}

TEST(UUIDTest, OperatorNotEqualTest)
{
    UUID theUUID;
    UUID theOtherUUID;
    EXPECT_TRUE(theUUID != theOtherUUID);
    EXPECT_FALSE(theUUID != theUUID);
}

TEST(UUIDTest, StringOperatorNotEqualTest)
{
    auto theUUID = UUID::fromString("87CA2887-48BD-4310-99B2-46618BE65FFF");
    EXPECT_TRUE(theUUID);
    EXPECT_TRUE(theUUID.value() != "87CA2887-48BD-4310-99B2-46618BE65FFE");
    EXPECT_FALSE(theUUID.value() != "87CA2887-48BD-4310-99B2-46618BE65FFF");
}

TEST(UUIDTest, ToStringTest)
{
    auto theUUID = UUID::fromString("BB6F37AD-77A3-45ED-A321-115F59CF7856");
    EXPECT_TRUE(theUUID);
    EXPECT_EQ(theUUID.value().toString(), "BB6F37AD-77A3-45ED-A321-115F59CF7856");
}

TEST(UUIDTest, FromStringTest)
{
    auto bad_uuid = UUID::fromString("foo");
    EXPECT_FALSE(bad_uuid);

    auto good_uuid = UUID::fromString("943CD73D-6A1D-4752-8901-0E34A38C824A");
    EXPECT_TRUE(good_uuid);
    EXPECT_EQ(good_uuid, "943CD73D-6A1D-4752-8901-0E34A38C824A");
}

TEST(UUIDTest, MapKeyTest)
{
    std::unordered_map<UUID, int> the_map{};
    auto uuid_worked = UUID::fromString("943CD73D-6A1D-4752-8901-0E34A38C824A");
    EXPECT_TRUE(uuid_worked);
    the_map.insert(std::make_pair(uuid_worked.value(), 10));

    auto other_uuid_worked = UUID::fromString("9A0D4B3A-4740-4CB6-ADCC-290EEA4A5197");
    EXPECT_TRUE(other_uuid_worked);
    the_map.insert(std::make_pair(other_uuid_worked.value(), 2));

    EXPECT_EQ(the_map.at(uuid_worked.value()), 10);
    EXPECT_EQ(the_map.at(other_uuid_worked.value()), 2);
}
