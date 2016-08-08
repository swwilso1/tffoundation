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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/


#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(DataTest, DefaultConstructorTest)
{
	Data d;
	EXPECT_EQ(0,d.length());
	EXPECT_EQ(nullptr,d.bytes());
}


// kDataSize should not be larger than 256 (the largest (-1) integer a byte can hold)
#define kDataSize 256

TEST(DataTest, ByteConstructorTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}


	Data d(data, kDataSize);

	EXPECT_EQ(kDataSize, d.length());

	const char *tmp = d.bytes();

	for(int i = 0; i < d.length(); i++)
	{
		EXPECT_EQ(i, static_cast<unsigned char>(*(tmp + i)));
	}
}


TEST(DataTest, CopyConstructorTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	Data d(data, kDataSize);

	Data copyOfD(d);

	EXPECT_EQ(d.length(), copyOfD.length());

	EXPECT_EQ(kDataSize, copyOfD.length());

	for(int i = 0; i < kDataSize; i++)
	{
		EXPECT_EQ(d[i], copyOfD[i]);
	}
}


TEST(DataTest, RValueConstructorTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	Data d(Data(data, kDataSize));	

	EXPECT_EQ(kDataSize, d.length());

	const char *tmp = d.bytes();

	for(int i = 0; i < d.length(); i++)
	{
		EXPECT_EQ(i, static_cast<unsigned char>(*(tmp + i)));
	}
}


TEST(DataTest, AssignmentOperatorTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	Data d(data, kDataSize);

	Data assignedD = d;

	EXPECT_EQ(kDataSize, assignedD.length());

	const char *tmp = assignedD.bytes();

	for(int i = 0; i < assignedD.length(); i++)
	{
		EXPECT_EQ(i, static_cast<unsigned char>(*(tmp + i)));
	}
}


TEST(DataTest, RValueAssignmentOperatorTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	Data d(Data(data, kDataSize));

	EXPECT_EQ(kDataSize, d.length());

	const char *tmp = d.bytes();

	for(int i = 0; i < d.length(); i++)
	{
		EXPECT_EQ(i, static_cast<unsigned char>(*(tmp + i)));
	}
}


TEST(DataTest, OperatorEqualTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
		data2[i] = i;
	}


	Data d(data, kDataSize);
	Data d2(data2, kDataSize);
	Data d3;

	EXPECT_EQ(d, d2);
	EXPECT_NE(d, d3);
	EXPECT_NE(d2, d3);

}


TEST(DataTest, OperatorNotEqualTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	Data d(data, kDataSize);
	Data other;

	EXPECT_NE(d, other);
}


TEST(DataTest, OperatorBracketTest)
{
	char data[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	Data d(data, kDataSize);

	for(int i = 0; i < kDataSize; i++)
	{
		EXPECT_EQ(d[i],i);
	}
}


TEST(DataTest, LengthMethodTest)
{
	char data[kDataSize];

	Data d(data, kDataSize);
	EXPECT_EQ(d.length(), kDataSize);
}



TEST(DataTest, BytesMethodTest)
{
	char data[kDataSize];

	Data d(data, kDataSize);

	const char *bytes = d.bytes();

	for(int i = 0; i < kDataSize; i++)
	{
		EXPECT_EQ(data[i], *(bytes + i));
	}
}


TEST(DataTest, AppendByteArrayTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = data2[i] = i;
	}

	Data d(data,kDataSize);

	d.append(data2, kDataSize);

	for(int i = 0; i < kDataSize; i++)
	{
		ASSERT_EQ(d[i],i);
	}

	for(int i = kDataSize; i < (2 * kDataSize); i++)
	{
		ASSERT_EQ(d[i], i - kDataSize);
	}
}


TEST(DataTest, AppendDataObjectTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = data2[i] = i;
	}

	Data d(data, kDataSize);
	Data d2(data, kDataSize);

	d.append(d2);

	for(int i = 0; i < kDataSize; i++)
	{
		ASSERT_EQ(d[i],i);
	}

	for(int i = kDataSize; i < (2 * kDataSize); i++)
	{
		ASSERT_EQ(d[i], i - kDataSize);
	}
}


TEST(DataTest, AppendDataMoveTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = data2[i] = i;
	}

	Data d(data, kDataSize);

	d.append(Data(data2, kDataSize));

	for(int i = 0; i < kDataSize; i++)
	{
		ASSERT_EQ(d[i],i);
	}

	for(int i = kDataSize; i < (2 * kDataSize); i++)
	{
		ASSERT_EQ(d[i], i - kDataSize);
	}	
}


TEST(DataTest, PrependByteArrayTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	for(int i = 0, j = kDataSize - 1; i < kDataSize; i++, j--)
	{
		data2[i] = j;
	}

	Data d(data, kDataSize);

	d.prepend(data2, kDataSize);

	for(int i = 0; i < kDataSize; i++)
	{
		ASSERT_EQ(d[i], kDataSize - i - 1);
	}

	for(int i = kDataSize, j = 0; i < (2 * kDataSize); i++, j++)
	{
		ASSERT_EQ(d[i], j);
	}
}



TEST(DataTest, PrependDataObjectTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	for(int i = 0, j = kDataSize - 1; i < kDataSize; i++, j--)
	{
		data2[i] = j;
	}

	Data d(data, kDataSize);
	Data d2(data2, kDataSize);

	d.prepend(d2);

	for(int i = 0; i < kDataSize; i++)
	{
		ASSERT_EQ(d[i], kDataSize - i - 1);
	}

	for(int i = kDataSize, j = 0; i < (2 * kDataSize); i++, j++)
	{
		ASSERT_EQ(d[i], j);
	}
}


TEST(DataTest, PrependDataObjectMoveTest)
{
	char data[kDataSize];
	char data2[kDataSize];

	for(int i = 0; i < kDataSize; i++)
	{
		data[i] = i;
	}

	for(int i = 0, j = kDataSize - 1; i < kDataSize; i++, j--)
	{
		data2[i] = j;
	}

	Data d(data, kDataSize);
	d.prepend(Data(data2, kDataSize));

	for(int i = 0; i < kDataSize; i++)
	{
		ASSERT_EQ(d[i], kDataSize - i - 1);
	}

	for(int i = kDataSize, j = 0; i < (2 * kDataSize); i++, j++)
	{
		ASSERT_EQ(d[i], j);
	}
}