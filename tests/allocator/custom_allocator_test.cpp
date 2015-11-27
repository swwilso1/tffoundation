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

#include <cstddef>
#include <stdlib.h>
#include "TFFoundation/TFFoundation.hpp"

using foo_type = double;

struct MemoryBlock
{
	foo_type foo;
	unsigned int bar;
	char *mem;
};

extern "C"
{
	void * allocator(TF::Foundation::Size_t n);
	void deallocator(void *p);
}


void * allocator(TF::Foundation::Size_t n)
{
	MemoryBlock *newBlock = reinterpret_cast<MemoryBlock *>(malloc(n * sizeof(MemoryBlock)));
	if(newBlock != nullptr)
	{
		return reinterpret_cast<void *>(&newBlock->mem);
	}

	return nullptr;
}


void deallocator(void *p)
{
	if(p == nullptr)
		return;

	MemoryBlock *theBlock = reinterpret_cast<MemoryBlock *>(reinterpret_cast<char *>(p) -
			offsetof(MemoryBlock, mem));
	free(reinterpret_cast<void *>(theBlock));
}


int main()
{
	// Allocate several blocks of memory with the original allocator.
	char *foo = new char[100];
	int *bar = new int;
	double *bat = new double[500];

	// Swap in the alternate allocator
	TF::Foundation::AllocatorInterface::setAllocatorAndDeallocator(allocator,deallocator);

	// Use the alternate allocator to allocate several more blocks of memory.
	float *f = new float[256];
	delete[] f;
	unsigned long *ul = new unsigned long[20];
	delete[] ul;

	// Now delete the original blocks of memory.  These should delete using the
	// original deallocator.   If they use the new allocator, they will crash
	// the program.
	delete[] bat;
	delete[] bar;
	delete[] foo;

	// Test successfull.
	return 0;
}


