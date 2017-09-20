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


#ifndef TFSTRING_HPP
#define TFSTRING_HPP

#define NEEDS_OSTREAM
#define NEEDS_MEMORY
#define NEEDS_UTILITY
#define NEEDS_STRING
#define NEEDS_VECTOR
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdata.hpp"
#include "tfcomparison.hpp"
#include "tfstringcore.hpp"
#include "tfstringiterator.hpp"
#include "tfstringencoder.hpp"

namespace TF
{

    namespace Foundation
    {

        class String : public AllocatorInterface
        {
        public:

            static bool useObjectFormattingOutput;

            /** @brief array type */
            using string_array_type = std::vector<String>;

            /** @brief data type */
            using data_type = Data;

            /** @brief range type */
            using range_type = Range;

            /** @brief range array type */
            using range_array_type = std::vector<Range>;

            /** @brief STL string type */
            using stl_string_type = std::string;

            /** @brief basic char type */
            using unicode_point_type = unsigned int;

            /** @brief iterator type */
            using iterator = StringIterator;

            /** @brief character type */
            using char_type = unsigned char;

#pragma mark - Constructors

            /** Default constructor */
            String();

            /** Copy constructor */
            String(const String& s);

            /** RValue constructor */
            String(String&& s) noexcept;

            /**
                @brief C-style string constructor.
                @param str a pointer to a C-style string
                @return a new string object
            */
            explicit String(const char *str);

            /**
                @brief C-style string constructor with length parameter.
                @param str a pointer to a C-style string
                @param length the length of @e str array.
                @return a new string object.
            */
            String(const char *str, size_type length);

            /**
                @brief STL string constructor
                @param str a STL string
                @return a new string object.
            */
            explicit String(const std::string& s);

            /**
                @brief UTF-8 string constructor
                @param str an array of UTF-8 code sequences
                @param length the length of the @e str array
                @return a new string object
            */
            String(const unsigned char *str, size_type length);

            /**
                @brief UTF-16 string constructor
                @param str a string of UTF-16 character codes
                @param length the length of the @e str array
                @return a new string object.
            */
            String(const unsigned short *str, size_type length);

            /**
                @brief UTF-32 string constructor
                @param str an array of UTF-32 codes
                @param length the length of the @e str array
                @return a new string object.
            */
            String(const unsigned int *str, size_type length);


            /** @brief destructor */
            ~String() override = default;

#pragma mark - Copy methods

            String deepCopy();

#pragma mark - Initialization routines


            /**
                @brief static method to create a string object using a specified format
                @param format a C-style string containing the format of the string contents
                @return a smart pointer to a string object

                The format string is composed of zero or more directives: ordinary characters
                (not @c %), which are copied unchanged to the output stream; and conversion
                specifications, each of which results in fetching zero or more subsequent
                arguments.  Each conversion specification is introduced by the @c % character.
                The arguments must correspond properly (after type promotion) with the
                conversion specifier.

                Conversion specifiers:

                @c d Output a base 10 (decimal) number @n
                @c o Output an octal number @n
                @c x Output a hexidecimal number in lower-case @n
                @c X Output a hexidecimal number in upper-case @n
                @c u Output an unsigned int @n
                @c U Output an unsigned long int @n
                @c c Output a single character @n
                @c s Output a C-style string @n
                @c S Output a STL string @n
                @c f Output a real number with 6 digits of precision @n
                @c g Output a real number with up-to 6 digits of precision @n
                @c e Output a real number with up-to 6 digits of precision in @n
                     scientific notation. @n
                @c @ Output a String object @n

                For the STL string and the String conversion specifiers, the
                value passed to initWithFormat must be a pointer to a std::string
                and a pointer to a StringPointer respectively.

                Each conversion specifier may have an optional modifier as described
                below:

                @c 0 Activates padding with zeros or modifies the precision or field width @n
                @c 1-9 Modifies the requested precision or field width @n
                @c . Activates precision settings for real numbers @n
                @c l Adds long to integer values @n
                @c ll Adds long long to integer values @n
                @c h Makes @c d and @c i modifiers use type short, and @c o, @c u, @c x, @c X an unsigned short.
                @c hh Makes @c d and @c i modifiers user type signed char, and @c o, @c u, @c x, @c X an unsigned char.
                @c - Left aligns the formatted value.
            */
            static String initWithFormat(const char *format,...);

#pragma mark - Iterator methods


#if 1 // Disabled for now
            /**
                @brief returns an iterator pointing to the initial character of the string.
            */
            iterator begin();

            /**
                @brief returns an iterator pointing to the initial character of the string.
            */
            iterator begin() const;


            /**
                @brief returns an iterator point to the one element past the last character of the string.
            */
            iterator end();


            /**
                @brief returns an iterator point to the one element past the last character of the string.
            */
            iterator end() const;

#endif // Disabled iterator methods

    #pragma mark - Equality operators

            /**
                @brief compares a string with a C string for equality.
                @param s the C-style string.
                @return true if the contents of the strings match, false otherwise.
            */
            bool operator==(const char *s) const;

            /**
                @brief compares a string with a STL string for equality.
                @param s the STL string.
                @return true if the contents of the strings match, false otherwise.
            */
            bool operator==(const std::string& s) const;

            /**
                @brief Compares two strings for equality.
                @param s the string to compare.
                @return true if the two strings contain identical content, false otherwise.
            */
            bool operator==(const String& s) const;


            /**
                @brief Compares two strings character by character to see if they do not match.
                @param s the string to compare.
                @return true if the two strings contain differing characters, false otherwise.
            */
            bool operator!=(const String& s) const;

#pragma mark - Overloaded operators

            /**
                @brief Assignment operator
                @param s the string to assign.
                @return a reference to the current string updated with the contents of s.
            */
            String& operator=(const String& s);


            /** @brief overloaded rvalue assignment operator */
            String& operator=(String&& s) noexcept;

#pragma mark - Length and size methods

            /**
                @brief returns the number of characters in the string
                @return the number of characters in the string
            */
            size_type length() const;

            /**
                @brief returns the number of bytes used to encode the string
                @return the number of bytes
            */
            size_type numberOfBytes();

#pragma mark - Methods for getting characters

            /**
                @brief returns the character at index i
                @param i the index
                @return the character at the specified index
            */
            unicode_point_type operator[](size_type i) const;


            /**
                @brief returns the character at index i
                @param i the index
                @return the character at the specified index
            */
            unicode_point_type characterAtIndex(size_type i);


            /**
                @brief Returns a new string object with the characters that lie in @e range.
                @param range the range of characters
                @return a pointer to a new string object containing the requested range of characters.
            */
            String getCharactersInRange(range_type& range);

#pragma mark - Methods for getting C strings

            /**
                @brief return a C string version (if possible) of the string object.
                @return a pointer to a C-style string.
            */
            std::unique_ptr<const char> c_str();

            /**
                @brief return a STL string version (if possible) of the string object.
                @return a STL string.
            */
            std::string stlString();

#pragma mark - Methods for combining strings

            /**
                @brief create a new string object by appending contents as specified in the format
                @param format a C-Style string containing the format of the string to append
                @return a smart pointer to the new string object containing the appended results

                stringByAppendingFormat is a variable argument method used to append arbitrary
                contents to a string.  The format behavior behaves in the same manner as the
                printf() family of functions.
            */
            String stringByAppendingFormat(const char *format, ...);


            /**
                @brief create a new string object by appending the contents of the argument string
                @param str a smart pointer to the string to use for appending
                @return a smart pointer to the new objected containing the appended results
            */
            String stringByAppendingString(const String& str);


            /**
                @brief create a new string by concatenating two strings.
                @param s1 the first string
                @param s2 the second string
                @return a smart pointer to new string created by joining two strings.
            */
            static String concatenateStrings(const String& s1, const String& s2);


#pragma mark - Methods for dividing strings

            /**
                @brief return a new string object representing the characters in the original string starting
                from the index to the end of the string
                @param i an integer variable containing the starting index of the sub-string.
                @return a smart pointer to a new string object that contains the requested sub-string
            */
            String substringFromIndex(size_type i);


            /**
                @brief return a substring of the original string represented by the range parameter.
                @param range a variable containing the location and length of the requested sub-string.
                @return a smart pointer to a new string object that contains the requested sub-string
            */
            String substringWithRange(range_type& range);


            /**
                @brief return an array of substrings not contained in the range.
                @param range the range of substring not to match.
                @return an array of substrings from the string that are not in the specified @e range.
            */
            string_array_type substringsNotInRange(range_type& range);


            /**
                @brief returns a new string containing the characters of the receiver up to,
                but not including, the one at a given index.
                @param i the index of the last desired character in the sub-string
                @return a smart pointer to a new string object that contains the requested sub-string.
            */
            String substringToIndex(size_type i);


            /**
                @brief return an array of substrings of all the content in the string that does not match the substring
                argument.
                @param str the substring to match.
                @return an array of substrings from the string that do not match @e str.
            */
            string_array_type substringsThatDoNotMatchString(const String& str);


            /**
                @brief return an array of substrings created by splitting the string into chunks separated by @e splitString.
                @param splitString the string to use for matching
                @return an array of string pointers containing the substrings
            */
            string_array_type split(const String& splitString);

#pragma mark - Methods for finding characters and substrings

            /**
                @brief return a TFRange object containing the location of the first instance of the starting character
                and the length of the string given as a parameter
                @param str a smart pointer to a string object indicating the sub-string to find
                @return a TFRange object containing the index of the starting character and the length of the sub-string
            */
            range_type rangeOfString(const String& str);


            /**
                @brief return an array of TFRange objects containing the locations of the substring in the string.
                @param str the substring.
                @return an array of ranges.
            */
            range_array_type rangesOfString(const String& str);


#pragma mark - Methods for replacing substrings

            /**
                @brief return a string created by replacing instances of a sub-string with another string
                @param original the sub-string to replace
                @param replacement the string to insert in place of @e original
                @return a smart pointer to a new string object
            */
            String stringByReplacingOccurencesOfStringWithString(const String& original, const String& replacement);


            /**
                @brief return a string created by replacing the sub-string in the specified range with another string
                @param range the origin and length of the sub-string to replace
                @param str the string to insert in place of the string int @e range
                @return a smart pointer to a new string object
            */
            String stringByReplacingCharactersInRangeWithString(range_type& range, const String& str);

#pragma mark - Methods for comparing strings

            /**
                @brief return a TFComparisonResult indicating the lexical ordering of the string with another string
                @param str the string object to compare
                @return TFOrderedAscending if the string precedes @e str lexically,
                        TFOrderedSame if the string and @e str have an equivalent lexical value and
                    TFOrderedDescending if the string lexically follows @e str
            */
            ComparisonResult compare(const String& str) const;


            /**
                @brief return a TFComparisonResult indicating the lexical ordering of a sub-string
                       specified by a range with respect another string
                @param range the origin and length of the sub-string to use for comparison
                @param str the string to use for comparison with the sub-string
                @return TFOrderedAscending if the sub-string precedes @e str lexically,
                    TFOrderedSame if the sub-string and @e str have an equivalent lexical value, and
                        TFOrderedDescending if the sub-string lexically follows @e str
            */
            ComparisonResult compareRangeWithString(range_type& range, const String& str);


            /**
                @brief returns a boolean value indicating whether the intial characters of string
                       match another string
                @param str a string
                @return @c true if the string's first characters match the values in @e str, @c false if
                        the string's first characters do not match the values in @e str
            */
            bool hasPrefix(const String& str);


            /**
                @brief returns a boolean value indicating whether the last characters of the string
                           match another string
                @param str a string
                @return @c true if the string's last characters match the values in @e str, @c false if
                        the string's last characters do not match the values in @e str
            */
            bool hasSuffix(const String& str);


            /**
                @brief returns a boolean value indicating whether the string is equal to another
                       string by doing a literal Unicode-based comparison
                @param str a string
                @return @c true if the strings compare equal, otherwise @c false
            */
            bool isEqualToString(const String& str);

#pragma mark - Methods for changing character case

            /**
                @brief return a capitalized representation of the string
                @return A new string with the first character from each word in the string
                        changed to its corresponding uppercase value, and all remaining characters
                        changed to their corresponding lowercase values
            */
            String capitalizedString();


            /**
                @brief return a lowercase representation of the string
                @return A string with each character of the original replaced by its corresponding
                    lowercase value
            */
            String lowercaseString();


            /**
                @brief return a uppercase representation of the string
                @return A string with each character of the original replaced by its corresponding
                        uppercase value
             */
            String uppercaseString();

            
#pragma mark - Methods for converting a string to a data object

            /**
                @brief return a Data object pointer with the
                contents of the string.
                 @return a new Data object that
                contains the contents of the string.
            */
            data_type getAsData();


            /**
                @brief return a TFData object pointer with the contents of the
                string in ASCII encoding.
                @return a pointer to a new TFData object that contains a copy
                of the string in ASCII encoding.
            */
            data_type getAsDataInASCIIEncoding() const;

            /**
                @brief return a TFData object pointer with the contents of the
                string in UTF-8 encoding.
                @return a pointer to a new TFData object that contains a copy
                of the string in UTF-8 encoding.
            */
            data_type getAsDataInUTF8Encoding() const;

            /**
                @brief return a TFData object pointer with the contents of the
                string in UTF-16 encoding.
                @return a pointer to a new TFData object that contains a copy
                of the string in UTF-16 encoding.
            */
            data_type getAsDataInUTF16Encoding() const;

            /**
                @brief return a TFData object pointer with the contents of the
                string in UTF-32 encoding.
                @return a pointer to a new TFData object that contains a copy
                of the string in UTF-32 encoding.
            */
            data_type getAsDataInUTF32Encoding() const;

#pragma mark - Methods for testing purposes

#ifdef TFTESTS
            char_type * bytes();

            StringEncoder * encoder();
#endif /* TFTESTS */


            /** @brief std::ostream& interaction method */
            std::ostream& description(std::ostream& o) const;

        private:

#pragma mark - Shadow methods for variadic argument functions

            /**
                @brief static method to create a string object using a specified format
                @param format a C-style string containing the format of the string contents
                @param argList a va_list object referencing the other arguments for the format.
                @return a smart pointer to a string object.

                initWithFormat is a variable argument method used to create string object
                according to a format template.   This method behaves in the same manner as
                the standard printf() family of functions.
            */
            static String initWithFormat(const char *format, va_list *argList);

            /**
                @brief create a new string object by appending contents as specified in the format
                @param format a C-Style string containing the format of the string to append
                @param argList a va_list pointer referencing the other arguments for the format.
                @return a smart pointer to the new string object containing the appended results

                stringByAppendingFormat is a variable argument method used to append arbitrary
                contents to a string.  The format behavior behaves in the same manner as the
                printf() family of functions.
            */
            String stringByAppendingFormat(const char *format, va_list *argList);

            using encoder_type = StringEncoder;

            static data_type convertToThisEncoding(const String &s, encoder_type *encoder);

            using code_array_type = std::shared_ptr<unicode_point_type>;

            using core_type = StringCore;

            using core_pointer_type = std::shared_ptr<core_type>;

            core_pointer_type core;
        };


        std::ostream& operator<<(std::ostream& o, const String& s);


        ComparisonResult compareStrings(const String& stringOne, const String& stringTwo, void *context);

    } // Foundation

} // TF

#endif /* TFSTRING_HPP */


