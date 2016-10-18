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

#define NEEDS_CSTRING
#define NEEDS_CSTDARG
#define NEEDS_IOSTREAM
#define NEEDS_SSTREAM
#define NEEDS_STRING
#define NEEDS_STACK
#define NEEDS_STDEXCEPT
#include "tfheaders.hpp"
#include "tfstring.hpp"
#include "tfstringencoder.hpp"
#include "tfasciistringencoder.hpp"
#include "tfutf8stringencoder.hpp"
#include "tfutf16stringencoder.hpp"
#include "tfutf32stringencoder.hpp"
#include "tfstringiterator.hpp"
#include "tfinternalexceptions.hpp"

namespace TF
{

	namespace Foundation
	{

#pragma mark - Constructor methods

		// Default constructor
		String::String(StringEncoderType type)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;

			if(type == StringEncoderType::ASCIIEncoder)
				theEncoder = new ASCIIStringEncoder;
			else if(type == StringEncoderType::UTF8Encoder)
				theEncoder = new UTF8StringEncoder;
			else if(type == StringEncoderType::UTF16Encoder)
				theEncoder = new UTF16StringEncoder;
			else if(type == StringEncoderType::UTF32Encoder)
				theEncoder = new UTF32StringEncoder;
			else
				theEncoder = new UTF8StringEncoder;
		}


		String::String(const String& s)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;

			if(s.theBytes != nullptr && s.theNumberOfBytes > 0)
			{
				theBytes = new char_type[s.theNumberOfBytes];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(s.theBytes), s.theNumberOfBytes * sizeof(char_type));
				theNumberOfBytes = s.theNumberOfBytes;
			}

			theEncoder = s.theEncoder->clone();
		}


		String::String(String &&s)
		{
			theBytes = s.theBytes;
			theNumberOfBytes = s.theNumberOfBytes;
			theEncoder = s.theEncoder;

			s.theBytes = nullptr;
			s.theNumberOfBytes = 0;
			s.theEncoder = nullptr;
		}


		String::String(const char_type *str, size_type length, encoder_type *e)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;

			if(str && length > 0)
			{
				theBytes = new char_type[length];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<char_type *>(str)), length * sizeof(char));
				theNumberOfBytes = length;
			}

			theEncoder = e->clone();
		}


		String::String(const char *str)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;
			theEncoder = nullptr;

			if(str)
			{
				size_type theLength = std::strlen(str);
				theBytes = new char_type[theLength];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<char *>(str)), theLength * sizeof(char));
				theNumberOfBytes = theLength;
			}

			theEncoder = new ASCIIStringEncoder;
		}


		String::String(const char *str, size_type length)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;
			theEncoder = nullptr;

			if(str && length > 0)
			{
				theBytes = new char_type[length];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<char *>(str)), length * sizeof(char));
				theNumberOfBytes = length;
			}

			theEncoder = new ASCIIStringEncoder;
		}



		String::String(const std::string& s)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;
			theEncoder = nullptr;

			if(s.size() > 0)
			{
				theBytes = new char_type[s.size()];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<char *>(s.c_str())), s.size() * sizeof(char));
				theNumberOfBytes = s.size();
			}

			theEncoder = new ASCIIStringEncoder;
		}


		String::String(const unsigned char *str, size_type length)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;
			theEncoder = nullptr;

			if(str)
			{
				theBytes = new char_type[length];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<unsigned char *>(str)), length * sizeof(unsigned char));
				theNumberOfBytes = length;
			}

			theEncoder = new UTF8StringEncoder;
		}


		String::String(const unsigned short *str, size_type length)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;
			theEncoder = nullptr;

			if(str && length > 0)
			{
				theNumberOfBytes = length * sizeof(unsigned short);
				theBytes = new char_type[theNumberOfBytes];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<unsigned short *>(str)), theNumberOfBytes);
			}

			theEncoder = new UTF16StringEncoder;
		}


		String::String(const unsigned int *str, size_type length)
		{
			theBytes = nullptr;
			theNumberOfBytes = 0;
			theEncoder = nullptr;

			if(str && length > 0)
			{
				theNumberOfBytes = length * sizeof(unsigned int);
				theBytes = new char_type[theNumberOfBytes];
				std::memcpy(reinterpret_cast<void *>(theBytes),
					reinterpret_cast<void *>(const_cast<unsigned int *>(str)), theNumberOfBytes);
			}

			theEncoder = new UTF32StringEncoder;
		}


		String::~String()
		{
			if(theBytes)
			{
				delete[] theBytes;
				theBytes = nullptr;
			}

			if(theEncoder)
			{
				delete theEncoder;
				theEncoder = nullptr;
			}

			theNumberOfBytes = 0;
		}


		String String::initWithFormat(const char *format,...)
		{
			String result;

			// Set up the variable argument list
			va_list argList;
			va_start(argList,format);
			result = initWithFormat(format, &argList);
			va_end(argList);
			return result;
		}


#		define CLEAR_CONTROL_SETTINGS() \
		{ \
			processingFormatCode = false; \
			processingPrecision = false; \
			needsZeroPadding = false; \
			hasLLModifier = false; \
			hasLModifier = false; \
			hasCapitalLModifier = false; \
			hasHModifier = false; \
			hasHHModifier = false; \
			hasFieldWidth = false; \
			hasPrecision = false; \
			needsLeftAdjustment = false; \
			fieldWidth = 0; \
			precision = 0; \
			counter++; \
		}


#		define ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(value) \
		if(processingPrecision) \
		{ \
			precision = (precision * 10) + value; \
			if(! hasPrecision) \
				hasPrecision = true; \
		} \
		else \
		{ \
			fieldWidth = (fieldWidth * 10) + value; \
			if(! hasFieldWidth) \
				hasFieldWidth = true; \
		}

		String String::initWithFormat(const char *format, va_list *argList)
		{
			bool processingFormatCode = false;
			bool processingPrecision = false;
			bool needsZeroPadding = false;
			bool hasLLModifier = false;
			bool hasLModifier = false;
			bool hasCapitalLModifier = false;
			bool hasHModifier = false;
			bool hasHHModifier = false;
			bool hasFieldWidth = false;
			bool hasPrecision = false;
			bool needsLeftAdjustment = false;

			int fieldWidth = 0;
			int precision = 0;

			int counter = 0;

			std::stringstream accumulator;

			// Now parse the format string.   The format string must be
			// a C-Style (ie ends with '\0' (null-terminated) string.
			// Standard printf style arguments are accepted and one
			// extra form.  %@ now indicates a String argument.

			const char *fmt = format;

			while(*fmt != '\0')
			{

				// The easiest thing to do here will be to use stl strings
				// to collect the format valid characters and the conversion values.
				// At the end we can drop it all into a UTF-16 string.
				switch(*fmt)
				{
					case '%': // Activate code processing mode
						if(processingFormatCode)
						{
							accumulator << *fmt;
							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							processingFormatCode = true;
						}
						break;
					case 'i': // Same as 'd' modifier
					case 'd': // Output a base 10 (decimal) number
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
							}


							if(hasLLModifier)
							{
								long long arg;
								arg = va_arg(*argList, long long);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}
							else if(hasLModifier)
							{
								long arg;
								arg = va_arg(*argList, long);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}
							else if(hasHHModifier)
							{
								int arg;
								arg = va_arg(*argList, int);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}
							else if(hasHModifier)
							{
								int arg;
								arg = va_arg(*argList, int);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}
							else
							{
								int arg;
								arg = va_arg(*argList, int);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'o': // Output an octal number
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
							}

							if(hasLLModifier)
							{
								unsigned long long arg;
								arg = va_arg(*argList, unsigned long long);
								if(needsLeftAdjustment)
									value << std::left << std::oct << arg;
								else
									value << std::oct << arg;
							}
							else if(hasLModifier)
							{
								unsigned long arg;
								arg = va_arg(*argList, unsigned long);
								if(needsLeftAdjustment)
									value << std::left << std::oct << arg;
								else
									value << std::oct << arg;
							}
							else if(hasHHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::oct << arg;
								else
									value << std::oct << arg;
							}
							else if(hasHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::oct << arg;
								else
									value << std::oct << arg;
							}
							else
							{
								int arg;
								arg = va_arg(*argList, int);
								if(needsLeftAdjustment)
									value << std::left << std::oct << arg;
								else
									value << std::oct << arg;
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'x': // Output a hexidecimal number in lower-case
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
							}

							if(hasLLModifier)
							{
								unsigned long long arg;
								arg = va_arg(*argList, unsigned long long);
								if(needsLeftAdjustment)
									value << std::left << std::nouppercase << std::hex << arg;
								else
									value << std::nouppercase << std::hex << arg;
							}
							else if(hasLModifier)
							{
								unsigned long arg;
								arg = va_arg(*argList, unsigned long);
								if(needsLeftAdjustment)
									value << std::left << std::nouppercase << std::hex << arg;
								else
									value << std::nouppercase << std::hex << arg;
							}
							else if(hasHHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::nouppercase << std::hex << arg;
								else
									value << std::nouppercase << std::hex << arg;
							}
							else if(hasHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::nouppercase << std::hex << arg;
								else
									value << std::nouppercase << std::hex << arg;
							}
							else
							{
								int arg;
								arg = va_arg(*argList, int);
								if(needsLeftAdjustment)
									value << std::left << std::nouppercase << std::hex << arg;
								else
									value << std::nouppercase << std::hex << arg;
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'X': // Output a hexidecimal number in upper-case
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
							}

							if(hasLLModifier)
							{
								unsigned long long arg;
								arg = va_arg(*argList, unsigned long long);
								if(needsLeftAdjustment)
									value << std::left << std::uppercase << std::hex << arg;
								else
									value << std::uppercase << std::hex << arg;
							}
							else if(hasLModifier)
							{
								unsigned long arg;
								arg = va_arg(*argList, unsigned long);
								if(needsLeftAdjustment)
									value << std::left << std::uppercase << std::hex << arg;
								else
									value << std::uppercase << std::hex << arg;
							}
							else if(hasHHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::uppercase << std::hex << arg;
								else
									value << std::uppercase << std::hex << arg;
							}
							else if(hasHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::uppercase << std::hex << arg;
								else
									value << std::uppercase << std::hex << arg;
							}
							else
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << std::uppercase << std::hex << arg;
								else
									value << std::uppercase << std::hex << arg;
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'u': // Output an unsigned int.
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
							}

							if(hasLLModifier)
							{
								unsigned long long arg;
								arg = va_arg(*argList, unsigned long long);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							else if(hasLModifier)
							{
								unsigned long arg;
								arg = va_arg(*argList, unsigned long);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							else if(hasHHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							else if(hasHModifier)
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							else
							{
								unsigned int arg;
								arg = va_arg(*argList, unsigned int);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'U': // Output an unsigned long int.
						if(processingFormatCode)
						{
							unsigned long arg;
							std::stringstream value;

							arg = va_arg(*argList, unsigned long);

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							else
							{
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'c': // Output a single character.
						if(processingFormatCode)
						{
							int arg = va_arg(*argList, int);
							char c = static_cast<char>(arg);
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
								if(needsLeftAdjustment)
									value << std::left << c;
								else
									value << c;
							}
							else
							{
								if(needsLeftAdjustment)
									value << std::left << c;
								else
									value << c;
							}
							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 's': // Output a C-Style string
						if(processingFormatCode)
						{
							char *s = va_arg(*argList, char *);
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
							}

							if(needsLeftAdjustment)
								value << std::left << s;
							else
								value << s;
							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'S': // Output a std::string
						if(processingFormatCode)
						{
							std::string *s = va_arg(*argList,std::string *);
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding)
									value.fill('0');
							}
							if(s)
							{
								if(needsLeftAdjustment)
									value << std::left << *s;
								else
									value << *s;
							}
							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'f': // Output a real number with 6 digits of precision.
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
								if(hasPrecision) value.precision(precision);
								else value.precision(6);
							}
							else
							{
								if(hasPrecision) value.precision(precision);
								else value.precision(6);
							}

							if(hasCapitalLModifier)
							{
								long double arg = va_arg(*argList, long double);
								if(needsLeftAdjustment)
									value << std::left << std::fixed << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << std::fixed << arg;
									else
										value << std::fixed << arg;
								}
							}
							else if(hasLModifier)
							{
								double arg = va_arg(*argList,double);
								if(needsLeftAdjustment)
									value << std::left << std::fixed << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << std::fixed << arg;
									else
										value << std::fixed << arg;
								}
							}
							else
							{
								double arg = va_arg(*argList,double);
								if(needsLeftAdjustment)
									value << std::left << std::fixed << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << std::fixed << arg;
									else
										value << std::fixed << arg;
								}
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'g': // Output a real number with up-to 6 digits of precision.
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
								if(hasPrecision) value.precision(precision);
								else value.precision(6);
							}
							else
							{
								if(hasPrecision) value.precision(precision);
								else value.precision(6);
							}

							if(hasCapitalLModifier)
							{
								long double arg = va_arg(*argList, long double);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}
							else if(hasLModifier)
							{
								double arg = va_arg(*argList,double);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}
							else
							{
								double arg = va_arg(*argList,double);
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << arg;
									else
										value << arg;
								}
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'e': // Output a real number with up-to 6 digits of precision in scientific notation.
						if(processingFormatCode)
						{
							std::stringstream value;

							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
								if(hasPrecision) value.precision(precision);
								else value.precision(6);
							}
							else
							{
								if(hasPrecision) value.precision(precision);
								else value.precision(6);
							}


							if(hasCapitalLModifier)
							{
								long double arg = va_arg(*argList, long double);
								if(needsLeftAdjustment)
									value << std::left << std::scientific << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << std::scientific << arg;
									else
										value << std::scientific << arg;
								}
							}
							else if(hasLModifier)
							{
								double arg = va_arg(*argList,double);
								if(needsLeftAdjustment)
									value << std::left << std::scientific << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << std::scientific << arg;
									else
										value << std::scientific << arg;
								}
							}
							else
							{
								double arg = va_arg(*argList,double);
								if(needsLeftAdjustment)
									value << std::left << std::scientific << arg;
								else
								{
									if(arg < 0 && needsZeroPadding)
										value << std::internal << std::scientific << arg;
									else
										value << std::scientific << arg;
								}
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'p': // A void pointer.
						if(processingFormatCode)
						{
							void *arg = reinterpret_cast<void *>(va_arg(*argList, void *));
							std::stringstream value;
							if(hasFieldWidth)
							{
								value.width(fieldWidth);
								if(needsZeroPadding) value.fill('0');
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							else
							{
								if(needsLeftAdjustment)
									value << std::left << arg;
								else
									value << arg;
							}
							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '@': // Output a String object.
						if(processingFormatCode)
						{

							String *arg = va_arg(*argList, String *);
							std::stringstream value;


							if(needsLeftAdjustment)
								value << std::left << *arg;
							else
								value << *arg;

							if(hasFieldWidth)
							{
								if(fieldWidth > value.str().size())
								{
									accumulator.width(fieldWidth);
									if(needsZeroPadding) accumulator.fill('0');
								}
							}

							accumulator << value.str();

							CLEAR_CONTROL_SETTINGS();
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '0': // Check to see if we need to turn on needsZeroPadding.
						if(processingFormatCode)
						{
							if(! hasFieldWidth && ! processingPrecision)
							{
								needsZeroPadding = true;
							}
							else
							{
								ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(0);
							}
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '1': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(1);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '2': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(2);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '3': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(3);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '4': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(4);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '5': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(5);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '6': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(6);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '7': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(7);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '8': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(8);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '9': // Check to see if we need augment fieldWidth.
						if(processingFormatCode)
						{
							ADD_VALUE_TO_PRECISION_OR_FIELDWIDTH(9);
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '.': // Check to see if we have a precision setting.
						if(processingFormatCode)
						{
							processingPrecision = true;
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'l': // Check to see if we need to turn on hasLModifier or hasLLModifier.
						if(processingFormatCode)
						{
							if(hasLModifier)
								hasLLModifier = true;
							else
								hasLModifier = true;
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'L': // Check to see if we need to turn on hasCapitalLModifier.
						if(processingFormatCode)
						{
							hasCapitalLModifier = true;
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case 'h': // Check to see if we need to turn on hasHModifier or hasHHModifier.
						if(processingFormatCode)
						{
							if(hasHModifier)
								hasHHModifier = true;
							else
								hasHModifier = true;
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					case '-': // Turn on left justification.
						if(processingFormatCode)
						{
							needsLeftAdjustment = true;
						}
						else
						{
							accumulator << *fmt;
						}
						break;
					default:
						accumulator << *fmt;
				}

				fmt++;
			}

			return String(accumulator.str().c_str());
		}


		String String::convertToThisEncoding(const String& str,
			encoder_type *theEncoder)
		{
			size_type theNumberOfBytesToEncodeTheSubstringInThisEncoding = 0;
			StringIterator theIterator;
			unicode_point_type theCode;

			for(theIterator = str.begin(); theIterator != str.end(); theIterator++)
			{
				theCode = *theIterator;
				theNumberOfBytesToEncodeTheSubstringInThisEncoding +=
					theEncoder->bytesNeededForRepresentationOfCode(theCode);
			}

			if(theEncoder->usesByteOrderMark())
			{
				theNumberOfBytesToEncodeTheSubstringInThisEncoding +=
					theEncoder->lengthOfByteOrderMarkInBytes();
			}

			char_type *newSubstringBytes = new char_type[theNumberOfBytesToEncodeTheSubstringInThisEncoding];
			char_type *tmp = newSubstringBytes;
			size_type bytesLeftInTheNewString = theNumberOfBytesToEncodeTheSubstringInThisEncoding;

			if(theEncoder->usesByteOrderMark())
			{
				theEncoder->writeByteOrderMark(tmp, bytesLeftInTheNewString);
				tmp += theEncoder->lengthOfByteOrderMarkInBytes();
				bytesLeftInTheNewString -= theEncoder->lengthOfByteOrderMarkInBytes();
			}

			size_type bytesUsedToEncodeTheCode;
			Endian thisEndian = theEncoder->thisSystemEndianness();

			for(theIterator = str.begin(); theIterator != str.end(); theIterator++)
			{
				theCode = *theIterator;
				bytesUsedToEncodeTheCode = theEncoder->encodeCodePoint(tmp, bytesLeftInTheNewString, theCode, thisEndian);
				tmp += bytesUsedToEncodeTheCode;
				bytesLeftInTheNewString -= bytesUsedToEncodeTheCode;
			}

			String newString(newSubstringBytes,
				theNumberOfBytesToEncodeTheSubstringInThisEncoding, theEncoder);

			delete[] newSubstringBytes;

			return newString;
		}


		String::iterator String::begin(void)
		{
			iterator newIterator(theBytes, theNumberOfBytes, theEncoder, theEncoder->thisSystemEndianness());
			return newIterator;
		}


		String::iterator String::begin(void) const
		{
			iterator newIterator(theBytes, theNumberOfBytes, theEncoder, theEncoder->thisSystemEndianness());
			return newIterator;
		}


		String::iterator String::end(void)
		{
			iterator endIterator(theBytes + theNumberOfBytes, 0, theEncoder, theEncoder->thisSystemEndianness());
			return endIterator;
		}


		String::iterator String::end(void) const
		{
			iterator endIterator(theBytes + theNumberOfBytes, 0, theEncoder, theEncoder->thisSystemEndianness());
			return endIterator;
		}


		String::code_iterator String::begin_code(void)
		{
			code_iterator newIterator(theBytes, theNumberOfBytes, theEncoder, theEncoder->thisSystemEndianness());
			return newIterator;
		}


		String::code_iterator String::begin_code(void) const
		{
			code_iterator newIterator(theBytes, theNumberOfBytes, theEncoder, theEncoder->thisSystemEndianness());
			return newIterator;
		}


		String::code_iterator String::end_code(void)
		{
			code_iterator endIterator(theBytes + theNumberOfBytes, 0, theEncoder, theEncoder->thisSystemEndianness());
			return endIterator;
		}


		String::code_iterator String::end_code(void) const
		{
			code_iterator endIterator(theBytes + theNumberOfBytes, 0, theEncoder, theEncoder->thisSystemEndianness());
			return endIterator;
		}


		bool String::operator==(const char *s) const
		{
			const String theString(s);
			return *this == theString;
		}


		bool String::operator==(const std::string& s) const
		{
			const String theString(s);
			return *this == theString;
		}


		bool String::operator==(const String& s) const
		{
			if(this != &s)
			{

				if(this->length() != s.length())
					return false;

				iterator thisIterator;
				iterator sIterator;

				for(thisIterator = this->begin(), sIterator = s.begin(); thisIterator != this->end();
					thisIterator++, sIterator++)
				{
					if(*thisIterator != *sIterator)
						return false;
				}
			}

			return true;
		}


		bool String::operator!=(const String& s) const
		{
			if(*this == s)
				return false;
			return true;
		}


		String& String::operator=(const String& s)
		{
			if(this != &s)
			{
				if(s.theBytes && s.theNumberOfBytes > 0)
				{
					char_type *newBytes = new char_type[s.theNumberOfBytes];
					std::memcpy(reinterpret_cast<void *>(newBytes),
						reinterpret_cast<void *>(s.theBytes), s.theNumberOfBytes * sizeof(char_type));
					if(theBytes)
					{
						delete[] theBytes;
						theBytes = nullptr;
					}

					theBytes = newBytes;
					theNumberOfBytes = s.theNumberOfBytes;
				}

				StringEncoder *newEncoder = s.theEncoder->clone();
				if(theEncoder)
					delete theEncoder;
				theEncoder = newEncoder;
			}

			return *this;
		}


		String& String::operator=(String &&s)
		{
			if(theBytes && theNumberOfBytes > 0)
			{
				delete[] theBytes;
				theBytes = nullptr;
				theNumberOfBytes = 0;
			}

			if(theEncoder)
			{
				delete theEncoder;
				theEncoder = nullptr;
			}

			theBytes = s.theBytes;
			theNumberOfBytes = s.theNumberOfBytes;
			theEncoder = s.theEncoder;

			s.theBytes = nullptr;
			s.theNumberOfBytes = 0;
			s.theEncoder = nullptr;

			return *this;
		}


		String::size_type String::length(void) const
		{
			if(theBytes == nullptr)
				return 0;

			if(theEncoder == nullptr)
				throw std::runtime_error("length method does not have a string encoder");

			return theEncoder->numberOfCharacters(theBytes, theNumberOfBytes);
		}


		String::size_type String::numberOfBytes(void)
		{
			return theNumberOfBytes;
		}


		unicode_point_type String::operator[](size_type i) const
		{
			if(theEncoder == nullptr)
				throw std::runtime_error("operator[] method does not have a string encoder.");

			return theEncoder->unicodeCodePointForCharacterAtIndex(theBytes, theNumberOfBytes, i);
		}


		unicode_point_type String::characterAtIndex(size_type i)
		{
			if(theEncoder == nullptr)
				throw std::runtime_error("characterAtIndex method does not have a string encoder.");

			return theEncoder->unicodeCodePointForCharacterAtIndex(theBytes, theNumberOfBytes, i);
		}


		String String::getCharactersInRange(range_type& range)
		{
			if(! theEncoder->doesRangeOfCharactersLieInString(theBytes, theNumberOfBytes, range))
				throw std::out_of_range("range argument lies outside range of string.");

			size_type bytesToHoldCharacters = theEncoder->numberOfBytesToCaptureCharactersInRange(
				theBytes, theNumberOfBytes, range);
			size_type totalNewBytes = bytesToHoldCharacters;
			size_type characterStartIndex = theEncoder->arrayIndexOfCharacterAtCharacterIndex(theBytes,
				theNumberOfBytes, range.position);

			if(theEncoder->usesByteOrderMark())
			{
				totalNewBytes += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			char_type *newBytes = new char_type[totalNewBytes];
			char_type *tmp = newBytes;

			if(theEncoder->usesByteOrderMark())
			{
				theEncoder->writeByteOrderMark(newBytes, totalNewBytes);
				tmp += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			std::memcpy(reinterpret_cast<void *>(tmp), reinterpret_cast<void *>(theBytes +
				characterStartIndex), bytesToHoldCharacters * sizeof(char_type));

			String newPointer(newBytes, totalNewBytes, theEncoder);

			delete[] newBytes;

			return newPointer;
		}


		const char * String::c_str(void)
		{
			std::stringstream accumulator;

			iterator theIterator;

			for(theIterator = this->begin(); theIterator != this->end(); theIterator++)
			{
				unicode_point_type theCode = *theIterator;

				if(theCode <= 127)
				{
					char theCharacter = static_cast<char>(theCode);
					accumulator << theCharacter;
				}
				else
				{
					std::vector<char> theStack;
					size_type numberOfZeros = 0;
					unicode_point_type tmpCode = theCode;

					while(tmpCode > 0)
					{
						unicode_point_type value = tmpCode % 16;
						if(value <= 9)
						{
							theStack.push_back(static_cast<char>(48 + value));
						}
						else
						{
							theStack.push_back(static_cast<char>(55 + value));
						}

						tmpCode /= 16;
					}

					numberOfZeros = 6 - theStack.size();

					accumulator << "\\:";

					for(size_type i = 0; i < numberOfZeros; i++)
						accumulator << "0";

					std::vector<char>::reverse_iterator iter;
					for(iter = theStack.rbegin(); iter != theStack.rend(); ++iter)
						accumulator << *iter;
				}
			}

			cString = accumulator.str();
			return cString.c_str();
		}


		std::string String::stlString(void)
		{
			const char *theCString = this->c_str();
			return std::string(theCString);
		}


		String String::stringByAppendingFormat(const char *format, ...)
		{
			String result;
			va_list argList;
			va_start(argList, format);
			result = stringByAppendingFormat(format, &argList);
			va_end(argList);
			return result;
		}


		String String::stringByAppendingFormat(const char *format, va_list *argList)
		{
			String formatString;
			String newString;

			formatString = initWithFormat(format, argList);
			newString = concatenateStrings(*this, formatString);

			return newString;
		}


		String String::stringByAppendingString(const String& str)
		{
			return concatenateStrings(*this, str);
		}


		String String::concatenateStrings(const String& s1, const String& s2)
		{
			size_type s1Length = s1.length();
			size_type s2Length = s2.length();
			size_type newLength = s1Length + s2Length;

			// We add one extra code for the byte order mark.
			unicode_point_type *newArray = new unicode_point_type[newLength + 1];
			size_type i;

			newArray[0] = 0xFEFF;

			StringIterator theIterator;

			i = 1;
			for(theIterator = s1.begin(); theIterator != s1.end(); theIterator++, i++)
			{
				*(newArray + i) = *theIterator;
			}

			for(theIterator = s2.begin(); theIterator != s2.end(); theIterator++, i++)
			{
				*(newArray + i) = *theIterator;
			}

			String intermediateResult(newArray, newLength + 1);

			delete[] newArray;

			return convertToThisEncoding(intermediateResult, s1.theEncoder);
		}


		String String::substringFromIndex(size_type i)
		{
			size_type arrayIndex = theEncoder->arrayIndexOfCharacterAtCharacterIndex(theBytes, theNumberOfBytes,
				i);

			size_type numberOfBytesToAllocate = theNumberOfBytes - 	arrayIndex;
			size_type theBytesToTakeFromTheArray = numberOfBytesToAllocate;

			if(theEncoder->usesByteOrderMark())
			{
				numberOfBytesToAllocate += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			char_type *newBytes = new char_type[numberOfBytesToAllocate];
			char_type *tmp = newBytes;

			if(theEncoder->usesByteOrderMark())
			{
				theEncoder->writeByteOrderMark(tmp, numberOfBytesToAllocate);
				tmp += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			std::memcpy(reinterpret_cast<void *>(tmp),
				reinterpret_cast<void *>(theBytes + arrayIndex), theBytesToTakeFromTheArray * sizeof(char_type));

			String newString(newBytes, numberOfBytesToAllocate, theEncoder);

			delete[] newBytes;

			return newString;
		}


		String String::substringWithRange(range_type& range)
		{
			if(! theEncoder->doesRangeOfCharactersLieInString(theBytes, theNumberOfBytes, range))
				throw std::out_of_range("range lies outside range of string.");

			size_type arrayIndex = theEncoder->arrayIndexOfCharacterAtCharacterIndex(theBytes,
				theNumberOfBytes, range.position);
			size_type theNumberOfBytesInTheRange = theEncoder->numberOfBytesToCaptureCharactersInRange(
				theBytes, theNumberOfBytes, range);

			size_type theNumberOfBytesToAllocate = theNumberOfBytesInTheRange;

			if(theEncoder->usesByteOrderMark())
			{
				theNumberOfBytesToAllocate += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			char_type *newBytes = new char_type[theNumberOfBytesToAllocate];
			char_type *tmp = newBytes;

			if(theEncoder->usesByteOrderMark())
			{
				theEncoder->writeByteOrderMark(newBytes, theNumberOfBytesToAllocate);
				tmp += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			std::memcpy(reinterpret_cast<void *>(tmp),
				reinterpret_cast<void *>(theBytes + arrayIndex), theNumberOfBytesInTheRange);

			String newString(newBytes, theNumberOfBytesToAllocate, theEncoder);

			delete[] newBytes;

			return newString;
		}


		String::string_array_type String::substringsNotInRange(range_type& range)
		{
			string_array_type substringArray;

			if(range.position > 0)
			{
				range_type theRange(0, range.position);
				String tmpString = this->substringWithRange(theRange);
				substringArray.push_back(tmpString);
			}

			if(range.position + range.length < this->length())
			{
				size_type theLength = this->length() - (range.position + range.length);
				range_type theRange(range.position + range.length, theLength);
				String tmpString = this->substringWithRange(theRange);
				substringArray.push_back(tmpString);
			}

			return substringArray;
		}


		String String::substringToIndex(size_type i)
		{
			size_type terminusIndex = theEncoder->arrayIndexOfCharacterAtCharacterIndex(theBytes, theNumberOfBytes, i);
			return String(theBytes, terminusIndex, theEncoder);
		}


		String::string_array_type String::substringsThatDoNotMatchString(const String& str)
		{
			String strInThisEncoding = convertToThisEncoding(str, theEncoder);

			range_array_type rangesOfSubStrings;
			string_array_type theSubStrings;

			try
			{
				rangesOfSubStrings = theEncoder->findCharacterRangesOfSubstringsThatDoNotMatchSubstring(
					theBytes, theNumberOfBytes, strInThisEncoding.theBytes,
					strInThisEncoding.theNumberOfBytes);
			}
			catch(BadUnicodeException e)
			{
				throw std::runtime_error("String object or substring argument contains a bad Unicode sequence.");
			}

			for(auto &theRange : rangesOfSubStrings)
			{
				theSubStrings.push_back(this->substringWithRange(theRange));
			}

			return theSubStrings;
		}


		String::string_array_type String::split(const String& splitString)
		{
			return this->substringsThatDoNotMatchString(splitString);
		}


		String::range_type String::rangeOfString(const String& str)
		{
			range_type theRange;
			String strInThisEncoding = convertToThisEncoding(str, theEncoder);

			try
			{
				theRange = theEncoder->findCharacterRangeForSubstringInString(theBytes,
					theNumberOfBytes, strInThisEncoding.theBytes,
					strInThisEncoding.theNumberOfBytes);
			}
			catch(BadUnicodeException e)
			{
				throw std::runtime_error("String object or substring argument contains a bad unicode sequence");
			}

			return theRange;
		}


		String::range_array_type String::rangesOfString(const String& str)
		{
			range_array_type theRanges;
			String strInThisEncoding = convertToThisEncoding(str, theEncoder);

			try
			{
				theRanges = theEncoder->findCharacterRangesForSubstringInString(theBytes,
					theNumberOfBytes, strInThisEncoding.theBytes,
					strInThisEncoding.theNumberOfBytes);
			}
			catch(BadUnicodeException e)
			{
				throw std::runtime_error("String object or substring argument contains a bad unicode sequence");
			}

			return theRanges;
		}


		String String::stringByReplacingOccurencesOfStringWithString(
			const String& original, const String& replacement)
		{
			range_array_type originalStringRanges;
			size_type bytesNeededForNewString;
			String originalStringInThisEncoding = convertToThisEncoding(original, theEncoder);
			String replacementStringInthisEncoding = convertToThisEncoding(
				replacement, theEncoder);

			bytesNeededForNewString = theEncoder->computeArraySizeInBytesForStringByReplacingSubstrings(
				theBytes, theNumberOfBytes, originalStringInThisEncoding.theBytes,
				originalStringInThisEncoding.theNumberOfBytes,
				replacementStringInthisEncoding.theBytes,
				replacementStringInthisEncoding.theNumberOfBytes, originalStringRanges);

			char_type *newString = new char_type[bytesNeededForNewString];

			theEncoder->replaceOccurancesOfStringWithString(theBytes, theNumberOfBytes, newString,
				bytesNeededForNewString, replacementStringInthisEncoding.theBytes,
				replacementStringInthisEncoding.theNumberOfBytes, originalStringRanges);

			String theResultString(newString, bytesNeededForNewString, theEncoder);

			delete[] newString;

			return theResultString;
		}


		String String::stringByReplacingCharactersInRangeWithString(range_type& range, const String& str)
		{
			if(! theEncoder->doesRangeOfCharactersLieInString(theBytes, theNumberOfBytes, range))
				throw std::out_of_range("Range argument lies outside range of string.");

			String theReplacementStringInThisEncoding = convertToThisEncoding(str, theEncoder);

			String theNewString(nullptr, 0, theEncoder);

			size_type totalLength = this->length();

			if(range.position == 0)
			{
				// The string represented by range is at the beginning of this string.
				// Calculate the range of the remaining string.
				range_type newRange(range.length,
					totalLength - range.length);
				String theRemainingString = this->substringWithRange(newRange);
				theNewString = theNewString.stringByAppendingString(theReplacementStringInThisEncoding);
				theNewString = theNewString.stringByAppendingString(theRemainingString);
			}
			else if(range.position == (totalLength - range.length - 1))
			{
				// The string represented by range is at the end of this string.
				// Calculate the range of the preceding string.
				range_type newRange(0, range.position);
				String thePrecedingString = this->substringWithRange(newRange);
				theNewString = theNewString.stringByAppendingString(thePrecedingString);
				theNewString = theNewString.stringByAppendingString(theReplacementStringInThisEncoding);
			}
			else
			{
				// The string represented by range is somewhere in the middle of this string, ie not at
				// either edge.  Calculate the preceding and remaining stubstrings.
				range_type precedingRange(0, range.position);
				size_type remainingLocation = range.position + range.length;
				range_type remainingRange(remainingLocation,
					totalLength - remainingLocation);
				String thePrecedingString = this->substringWithRange(precedingRange);
				String theRemainingString = this->substringWithRange(remainingRange);
				theNewString = theNewString.stringByAppendingString(thePrecedingString);
				theNewString = theNewString.stringByAppendingString(theReplacementStringInThisEncoding);
				theNewString = theNewString.stringByAppendingString(theRemainingString);
			}

			return theNewString;
		}


		ComparisonResult String::compare(const String& str) const
		{
			StringIterator thisIterator;
			StringIterator thatIterator;

			for(thisIterator = this->begin(), thatIterator = str.begin(); thisIterator != this->end() &&
				thatIterator != str.end(); thisIterator++, thatIterator++)
			{
				if(*thisIterator < *thatIterator)
					return OrderedAscending;
				if(*thisIterator > *thatIterator)
					return OrderedDescending;
			}

			if(this->length() == str.length())
				return OrderedSame;
			else if(this->length() > str.length())
				return OrderedAscending;

			return OrderedDescending;
		}


		ComparisonResult String::compareRangeWithString(range_type& range, const String& str)
		{
			String rangeOfThisString = this->substringWithRange(range);
			return rangeOfThisString.compare(str);
		}


		bool String::hasPrefix(const String& str)
		{
			if(this->length() < str.length())
				return false;

			range_type initialRangeOfThisString(0, str.length());
			String thePrefixOfThisString = this->substringWithRange(initialRangeOfThisString);

			if(thePrefixOfThisString.compare(str) == OrderedSame)
				return true;

			return false;
		}


		bool String::hasSuffix(const String& str)
		{
			if(this->length() < str.length())
				return false;

			range_type endRangeOfThisString(this->length() - str.length(),
				str.length());
			String theSuffixOfThisString = this->substringWithRange(endRangeOfThisString);

			if(theSuffixOfThisString.compare(str) == OrderedSame)
				return true;

			return false;
		}


		bool String::isEqualToString(const String& str)
		{
			if(this->compare(str) == OrderedSame)
				return true;
			return false;
		}


		String String::capitalizedString(void)
		{
			bool capitalizeTheNextCharacter = true;
			String theNewString(theBytes, theNumberOfBytes, theEncoder);

			char_type *tmp = theNewString.theBytes;
			size_type bytesLeft = theNewString.theNumberOfBytes;

			StringEncoder::byte_order_query_type theQueryResult = theEncoder->hasByteOrderMark(
				theNewString.theBytes, theNewString.theNumberOfBytes);

			if(theQueryResult.first)
			{
				tmp += theEncoder->lengthOfByteOrderMarkInBytes();
			}

			while(bytesLeft > 0)
			{
				std::pair<unicode_point_type, size_type> thePair = theEncoder->nextCodePoint(tmp, bytesLeft,
					theQueryResult.second);

				if(capitalizeTheNextCharacter)
				{
					if(thePair.first >= 97 && thePair.first <= 122)
					{
						theEncoder->encodeCodePoint(tmp, bytesLeft, thePair.first - 32, theQueryResult.second);
					}
					capitalizeTheNextCharacter = false;
				}
				else
				{
					if(thePair.first == 32)
						capitalizeTheNextCharacter = true;
					else
					{
						if(thePair.first >= 65 && thePair.first <= 90)
						{
							theEncoder->encodeCodePoint(tmp, bytesLeft, thePair.first + 32, theQueryResult.second);
						}
					}
				}

				tmp += thePair.second;
				bytesLeft -= thePair.second;
			}

			return theNewString;
		}


		String String::lowercaseString(void)
		{
			String theNewString(theBytes, theNumberOfBytes, theEncoder);
			theEncoder->convertStringCharacters(theNewString.theBytes, theNewString.theNumberOfBytes,
				StringCase::LowerCase);
			return theNewString;
		}


		String String::uppercaseString(void)
		{
			String theNewString(theBytes, theNumberOfBytes, theEncoder);
			theEncoder->convertStringCharacters(theNewString.theBytes, theNewString.theNumberOfBytes,
				StringCase::UpperCase);
			return theNewString;
		}


		String::data_type String::getAsData(void)
		{
			data_type theData(
				reinterpret_cast<char *>(theBytes), theNumberOfBytes * sizeof(char_type));
			return theData;
		}


		String::data_type String::getAsDataInASCIIEncoding(void)
		{
			StringEncoder *newEncoder = new ASCIIStringEncoder;
			String thisString(theBytes, theNumberOfBytes, theEncoder);
			String stringInNewEncoding = this->convertToThisEncoding(thisString, newEncoder);
			delete newEncoder;
			return stringInNewEncoding.getAsData();
		}


		String::data_type String::getAsDataInUTF8Encoding(void)
		{
			StringEncoder *newEncoder = new UTF8StringEncoder;
			String thisString(theBytes, theNumberOfBytes, theEncoder);
			String stringInNewEncoding = convertToThisEncoding(thisString, newEncoder);
			delete newEncoder;
			return stringInNewEncoding.getAsData();
		}


		String::data_type String::getAsDataInUTF16Encoding(void)
		{
			StringEncoder *newEncoder = new UTF16StringEncoder;
			String thisString(theBytes, theNumberOfBytes, theEncoder);
			String stringInNewEncoding = convertToThisEncoding(thisString, newEncoder);
			delete newEncoder;
			return stringInNewEncoding.getAsData();
		}


		String::data_type String::getAsDataInUTF32Encoding(void)
		{
			StringEncoder *newEncoder = new UTF32StringEncoder;
			String thisString(theBytes, theNumberOfBytes, theEncoder);
			String stringInNewEncoding = convertToThisEncoding(thisString, newEncoder);
			delete newEncoder;
			return stringInNewEncoding.getAsData();
		}


#ifdef TFTESTS
			char_type * String::bytes(void)
			{
				return theBytes;
			}


			StringEncoder * String::encoder(void)
			{
				return theEncoder;
			}
#endif /* TFTESTS */


		std::ostream& String::description(std::ostream& o) const
		{
			ClassFormatter *formatter = FormatterFactory::getFormatter();
			if(formatter != nullptr)
			{
				formatter->setClassName("String");
				formatter->addClassMember("char_type", "theBytes", theBytes);
				formatter->addClassMember("size_type", "theNumberOfBytes", theNumberOfBytes);
				formatter->addClassMember<encoder_type *>("encoder_type *", "theEncoder",
					theEncoder);
				o << *formatter;
				delete formatter;
			}
			return o;
		}


		std::ostream& operator<<(std::ostream& o, const String& s)
		{
			return s.description(o);
		}


		ComparisonResult compareStrings(const String& stringOne, const String& stringTwo, void *context)
		{
			return stringOne.compare(stringTwo);
		}

	} // Foundation

} // TF

