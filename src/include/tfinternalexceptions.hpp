/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2016 Tectiform Inc.

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

#ifndef TFINTERNALEXCEPTIONS_HPP
#define TFINTERNALEXCEPTIONS_HPP

#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

	namespace Foundation
	{
	
		/**
			@brief Indicates that a buffer argument to a method does not contain enough space to finish algorithm 
			
			Functions and methods may throw this object as an exception to indicate that buffer provided as an
			argument to the function or method does not contain enough space to complete the requested operation
		*/
		class NotEnoughBufferSpaceException : public AllocatorInterface
		{
		public:
			NotEnoughBufferSpaceException() {}
			virtual ~NotEnoughBufferSpaceException() {}
		}; // End class NotEnoughBufferSpaceException
		
		
		/**
			@brief Indicates that an index used to request a value lies outside the objects range.
		 
			Functions and methods may throw this object as an exception to indicate that a requested index lies outside the bounds of the queried object.
		*/
		class RangeException : public AllocatorInterface
		{
		public:
			RangeException() {}
			virtual ~RangeException() {}
		};
		
		
		/**
			@brief Indicates that the requested conversion from one form to another has failed.
		 
			Functions and methods may throw this object as an exception to indicate that a requested conversion
			operation has failed.
		*/
		class BadConversionException : public AllocatorInterface
		{
		public:
			BadConversionException() {}
			virtual ~BadConversionException() {}
		};


		/**
			@brief Functions in the unicode utilities module will throw 
			BadUnicodeException objects upon encountering errors in an 
			Unicode strings
		*/
		class BadUnicodeException : public AllocatorInterface
		{
		public:
			/** 
				@brief Location contains the integer location of the byte, 
				word, or code that contains the error 
			*/
			Size_t location;


			/** Default constructor */
			BadUnicodeException() : location(0) {};
			
			/**
				@brief Location constructor
				@param i a Size_t value containing the location in the byte, 
				word, or code string that contains the bad Unicode sequence
			*/
			BadUnicodeException(Size_t i) : location(i) {}
			
			virtual ~BadUnicodeException() {}
		}; // End class BadUnicodeException

	
	} // Foundation
	

} // TF


#endif /* TFINTERNALEXCEPTIONS_HPP */
