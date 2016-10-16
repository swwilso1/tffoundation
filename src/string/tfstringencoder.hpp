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

#ifndef TFSTRINGENCODER_HPP
#define TFSTRINGENCODER_HPP

#define NEEDS_OSTREAM
#define NEEDS_VECTOR
#define NEEDS_UTILITY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstringtypes.hpp"
#include "tfcomparison.hpp"
#include "tfstringcase.hpp"

namespace TF
{

	namespace Foundation
	{

		class StringEncoder : public AllocatorInterface
		{
		public:

			/** @brief standard length type */
			using size_type = Size_t;

			/** @brief range type for characters and array elements */
			using range_type = Range;

			/** @brief type for arrays of ranges */
			using range_array_type = std::vector<range_type>;

			/**
				@brief Query result type.
				bool indicates if the string has a byte order mark
				Endian indicates the platform endianness of the string
			*/
			using byte_order_query_type = std::pair<bool, Endian>;


			/** @brief destructor */
			virtual ~StringEncoder();

			/** @brief clone method for creating a new encoder */
			virtual StringEncoder * clone(void) = 0;

			/**
				@brief length in bytes of the basic code used by the encoder's encoding scheme.
				@return the number of bytes used for the basic code.

				For example, UTF-8 encoding uses 1 byte codes, UTF-16 uses 2-byte codes, and
				UTF-32 uses 4-byte codes.
			*/
			virtual size_type basicCodeLengthInBytes(void) = 0;

			/**
				@brief Does the encoder use a fixed size code length per character.
				@brief return true if the encoder has a fixed-length code, or false otherwise.

				For example, UTF-8 uses 1 byte codes, but multi-byte (1,2,3, or 4) characters.
				UTF-16 uses 2-byte codes (but 1 or 2 code characters).  UTF-32 uses 4-byte codes,
				and fixed-length (4 byte) characters.
			*/
			virtual bool hasFixedCodeLength(void) = 0;

			/**
				@brief The number of bytes to successfully represent the largest character value.
				@return the number of bytes required for the largest character value.
			*/
			virtual size_type numberOfBytesRequiredForLargestCharacterValue(void) = 0;

			/**
				@brief Can the encoder use a byte order mark?
				@return true if the encoding scheme can use a byte order mark, false otherwise.

				For example, UTF-8, UTF-16, and UTF-32 can all use byte order marks.  ASCII encoding
				cannot.
			*/
			virtual bool canUseByteOrderMark(void) = 0;

			/**
				@brief Does this version of the encoder expect or insert a byte order mark.
				@return true if the encoding scheme will insert a byte order mark, false otherwise.
			*/
			virtual bool usesByteOrderMark(void) = 0;

			/**
				@brief How many bytes will this encoder use to insert/process the byte order mark
				@return the number of bytes the encoder will write for a byte order mark.
			*/
			virtual size_type lengthOfByteOrderMarkInBytes(void) = 0;

			/**
				@brief given a byte array of adequate length, encode the byte order mark at the start of the
				array.
				@param start the pointer to the beginning of the byte array.
				@param length the number of bytes in the byte array.
			*/
			virtual void writeByteOrderMark(char_type *start, size_type length) = 0;

			/**
				@brief given a byte array, check if the array has a byte order mark and record the endian
				value revealed by the byte order mark.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the bytes array.
				@return the byte_order_query (std::pair<bool,Endian>).
			*/
			virtual byte_order_query_type hasByteOrderMark(const char_type *start, size_type length) = 0;

			/**
				@brief given a byte array, return the number of characters encoded in the array.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@return the number of characters encoded in the byte array.
			*/
			virtual size_type numberOfCharacters(const char_type *start, size_type length) = 0;

			/**
				@brief given a byte array, check the array to make sure it contains correctly encoded
				characters.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@return true if the bytes correctly encode characters, false otherwise.
			*/
			virtual bool checkStringForCorrectness(const char_type *start, size_type length) = 0;


			/**
				@brief given a pointer to location in a byte array, calculate the Unicode code point encoded
				at that position.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param endian the byte order of the characters encoded in the byte array.
				@return a std::pair containing the value of the code point and the number of bytes required to
				encode that code point.

				Use this method as the basis of an iterator for iterating through a byte array for characters.
			*/
			virtual std::pair<unicode_point_type, size_type> nextCodePoint(const char_type *start, size_type length, Endian endian) = 0;

			/**
				@brief given a pointer to a location in a byte array, calculate the next code point value at that position.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param endian the byte order of the characters encoded in the byte array.
				@return a std::pair containing the value of the code and the number of bytes required to encode
				that code.

				The return code is not a Unicode value, but rather the value of the next code in the array.
			*/
			virtual std::pair<unicode_point_type, size_type> nextCode(const char_type *start, size_type length, Endian endian) = 0;

			/**
				@brief given a pointer to a byte array and a character index into that array, return the Unicode code
				point encoded at that position.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param index the character index of the code point to retrieve.
				@return the Unicode code point value of the character located at @e index.
			*/
			virtual unicode_point_type unicodeCodePointForCharacterAtIndex(const char_type *start, size_type length, size_type index) = 0;

			/**
				@brief given a Unicode code point value, calculate the number of bytes required to represent this
				code point in the given encoding scheme.
				@param code the Unicode code point.
				@return the number of bytes necessary to encode the value in the encoding scheme.
			*/
			virtual size_type bytesNeededForRepresentationOfCode(unicode_point_type code) = 0;

			/**
				@brief given a pointer to a byte array and Unicode code point, encode that code point at the start of
				the byte array.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param code the Unicode code point to encode.
				@param endian the byte order of the string.
				@return the number of bytes used to encode @e code.
			*/
			virtual size_type encodeCodePoint(char_type *start, size_type length, unicode_point_type code, Endian endian) = 0;

			/**
				@brief given a pointer to a byte array and a character index, return the array index (byte offset) of
				the character at the character index.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param index the character index.
				@return the offset in bytes in the byte array of the location of the character at @e index.
			*/
			virtual size_type arrayIndexOfCharacterAtCharacterIndex(const char_type *start, size_type length, size_type index) = 0;

			/**
				@brief given a pointer to a byte array and range object, caculate whether or not that range of bytes
				lies in the given byte array.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param range the byte range.
				@return true if the byte array contains the byte range, false otherwise.
			*/
			virtual bool doesRangeOfBytesLieInString(const char_type *start, size_type length, range_type& range);

			/**
				@brief given a pointer to a byte array and range object, caculate whether or not that range of characters
				lies in the given byte array.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param range the character range.
				@return true if the byte array contains the character range, false otherwise.
			*/
			virtual bool doesRangeOfCharactersLieInString(const char_type *start, size_type length, range_type& range);


			/**
				@brief given a byte array and a range of characters, caculate the number of bytes from the byte
				array required to represent that range of characters.
				@param start the pointer to the array of bytes.
				@param lengt hthe number of bytes in the byte array.
				@param range the character range
				@return the number of bytes used to encode that character range in the byte array.

				This method does *not* include the bytes for the byte order mark.
			*/
			virtual size_type numberOfBytesToCaptureCharactersInRange(const char_type *start, size_type length,
				range_type& range) = 0;

			/**
				@brief given a byte array, calculate whether or not the array has a zero value character.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@return true if the byte array contains a character with the Unicode point value 0, false
				otherwise.
			*/
			virtual bool containsCharacterWithZeroValue(const char_type *start, size_type length) = 0;

			/**
				@brief given a byte array, caclulate whether or not the array has a character value that
				lies outside the range of ASCII characters (1 - 127).
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@return true if the byte array contains a character that lies outside the range of ASCII characters,
				false otherwise.
			*/
			virtual bool containsCharacterNotInASCIIRange(const char_type *start, size_type length) = 0;

			/**
				@brief given a byte array and another byte array representing the contents of a substring,
				calculate the range (location and length) of bytes of the first occurance of the substring
				in the first byte array.
				@param stringStart the pointer to the array of bytes.
				@param stringLength the number of bytes in the byte array.
				@param substringStart the pointer to the substring's array of bytes.
				@param substringLength the number of bytes in the substring's byte array.
				@return a range object specifying the start location and number of bytes of the first occurance
				of the substring in the string.
			*/
			virtual range_type findByteRangeOfSubstringInString(const char_type *stringStart, size_type stringLength,
				const char_type *substringStart, size_type substringLength) = 0;

			/**
				@brief given a byte array and another byte array representing the contents of a substring,
				calculate all the ranges (location and length) of bytes of the occurances of the substring
				int the first byte array.
				@param stringStart the pointer to the array of bytes.
				@param stringLength the number of bytes in the byte array.
				@param substringStart the pointer to the substring's array of bytes.
				@param substringLength the number of bytes in the substring's byte array.
				@return an array of range objects specifying the location and lengths of bytes that match
				the substring in the string.
			*/
			virtual range_array_type findByteRangesOfSubstringInString(const char_type *stringStart, size_type stringLength,
				const char_type *substringStart, size_type substringLength) = 0;

			/**
				@brief given a byte array and another byte array representing the contents of a substring,
				calculate the range (location and length) of characters of the first occurance of the substring
				in the first byte array.
				@param stringStart the pointer to the array of bytes.
				@param stringLength the number of bytes in the byte array.
				@param substringStart the pointer to the substring's array of bytes.
				@param substringLength the number of bytes in the substring's byte array.
				@return a range object specifying the start location and number of characters of the first occurance
				of the substring in the string.
			*/
			virtual range_type findCharacterRangeForSubstringInString(const char_type *stringStart, size_type stringLength,
				const char_type *substringStart, size_type substringLength) = 0;

			/**
				@brief given a byte array and another byte array representing the contents of a substring,
				calculate all the ranges (location and length) of characters of the occurances of the substring
				int the first byte array.
				@param stringStart the pointer to the array of bytes.
				@param stringLength the number of bytes in the byte array.
				@param substringStart the pointer to the substring's array of bytes.
				@param substringLength the number of bytes in the substring's byte array.
				@return an array of range objects specifying the location and lengths of characters that match
				the substring in the string.
			*/
			virtual range_array_type findCharacterRangesForSubstringInString(const char_type *stringStart, size_type stringLength,
				const char_type *substringStart, size_type substringLength) = 0;

			/**
				@brief given a byte array and another byte array representing the contents of a substring,
				caculate all the ranges (location and lengths) of characters in the first string that do
				not match the substring.
				@param stringStart the pointer to the array of bytes.
				@param stringLength the number of bytes in the byte array.
				@param substringStart the pointer to the substring's array of bytes.
				@param substringLength the number of bytes in the substring's byte array.
				@return an array of range objects specifying the location and lengths of the characters that do
				not match the substring from the string.
			*/
			virtual range_array_type findCharacterRangesOfSubstringsThatDoNotMatchSubstring(const char_type *stringStart, size_type stringLength,
				const char_type *substringStart, size_type substringLength) = 0;

			/**
				@brief given two byte arrays, compare the character values encoded in those arrays.
				@param firstStringStart the pointer to the array of bytes for the first string.
				@param firstStringLength the number of bytes in the first string's byte array.
				@param secondStringStart the pointer to the array of bytes for the second string.
				@param secondStringLength the number of bytes in the second string's byte array.
				@return OrderedAscending if the characters in the first string come lexically before
				the characters in the second string, OrderedDescending if the characters in the first
				string come lexically after the characters in the second string and OrderedSame of the
				contents of the two strings are equal.
			*/
			virtual ComparisonResult compareStrings(const char_type *firstStringStart, size_type firstStringLength,
				const char_type *secondStringStart, size_type secondStringLength);

			/**
				@brief given a byte array and case conversion specificer, convert the characters in the byte
				array to the case indicated.
				@param start the pointer to the array of bytes.
				@param length the number of bytes in the byte array.
				@param convertToCase the new case value.  TFUpperCase indicates that all lower case values
				should be converted to upper case.  TFLowerCase indicates that all upper case values shoul
				be converted to lower case.
			*/
			virtual void convertStringCharacters(char_type *start, size_type length, StringCase convertToCase) = 0;

			/**
				@brief calculate the number of bytes needed to create a new string from an old string with a substring
				of the old string replaced with a replacement string value.
				@param stringStart the pointer to the array of bytes.
				@param stringLength the number of bytes in the array of bytes.
				@param substringStart the pointer to the array of bytes representing the substring to replace.
				@param substringLength the number of bytes in the substring byte array.
				@param replaceStringLength the number of bytes in the replacement byte array.
				@param ranges a reference to an array of range objects used to record the locations of the substring
				in the string.
				@return the number of bytes for the new array with the substring replaced by the replacement string.

				This method must include the number of bytes needed for any byte order mark used by the encoder.
			*/
			virtual size_type computeArraySizeInBytesForStringByReplacingSubstrings(const char_type *stringStart, size_type stringLength,
				const char_type *substringStart, size_type substringLength,
				const char_type *replaceStringStart, size_type replaceStringLength, range_array_type& ranges) = 0;

			/**
				@brief give a byte array and new byte array and a replacement byte array and an array of ranges of
				the characters to replace, create the new array by inserting the replacement byte array values in
				place of the ranges specified.
				@param originalStringStart the pointer to the byte array of the original string.
				@param originalStringLength the number of bytes in the original string array.
				@param newStringStart the pointer to the bytes array for the new string.
				@param newStringLength the number of bytes in the new string array.
				@param replacementStringStart the pointer to the array of bytes for the replacement string.
				@param replacementStringLength the number of bytes in the replacment string array.
				@param substringRanges a reference to an array of ranges that contain the location and
				lengths of the substrings to replace in originalStringStart.

				This method will copy the values from the original string to the new string that are not contained
				in the ranges list, and will insert the replacement string in the new string in place of the values
				in the ranges list.

				This method must write the byte order mark to the string if the encoder uses a byte order mark.
				In addition, the replacementString and the originalString will both contain byte order marks if
				the encoder uses byte order marks.
			*/
			virtual void replaceOccurancesOfStringWithString(const char_type *originalStringStart, size_type originalStringLength,
				char_type *newStringStart, size_type newStringLength, const char_type *replacementStringStart,
				size_type replacementStringLength, range_array_type& substringRanges) = 0;

			/**
				@brief given a byte array and a byte order direction, retrieve the Unicode value for the character
				stored at the first byte of the array.
				@param start the pointer to the bytes array.
				@param length the number of bytes in the byte array.
				@param endian the byte order of the characters in the array.
				@return the Unicode code point value for the character at the beginning of the array.
			*/
			virtual unicode_point_type correctValueForPlatform(const char_type *start, size_type length, Endian endian) = 0;

			/**
				@brief test whether a given string encoder is the same class as this string encoder.
				@param e the other string encoder.
				@return true if @e e is of the same class as this encoder, false otherwise.
			*/
			virtual bool operator==(const StringEncoder& e) = 0;

			/**
				@brief test whether a given string encoder is not the same class as this string encoder.
				@param e the other string encoder.
				@return true if @e is not from the same class as this encoder, false otherwise.
			*/
			virtual bool operator!=(const StringEncoder& e);

			/** @brief caculate the byte order used by the currently running system. */
			static Endian thisSystemEndianness(void);

			/**
				@brief ostream interaction method.
			*/
			virtual std::ostream& description(std::ostream& o) const = 0;

		protected:

			/** @brief a cached value of the current running system's byte order. */
			static const Endian myEndianValue;


		};

	} // Foundation


} // TF


#endif /* TFSTRINGENCODER_HPP */
