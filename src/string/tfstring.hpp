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
#define NEEDS_FUNCTIONAL
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
            String(const String & s);

            /** RValue constructor */
            String(String && s) noexcept;

            /**
                @brief Constructor with UTF-8 C-style string argument (NULL terminated).
                @param str a pointer to a UTF-8 C-style string.
                @return a new string object
            */
            String(const char * str);

            /**
                @brief UTF-8 C-style string constructor with length parameter.
                @param str a pointer to a UTF-8 C-style string
                @param length the length of @e str array.
                @return a new string object.
            */
            String(const char * str, size_type length);

            /**
                @brief STL string constructor
                @param str a STL string
                @return a new string object.
            */
            String(const std::string & s);

            /**
                @brief UTF-8 string constructor
                @param str an array of UTF-8 code sequences
                @param length the length of the @e str array
                @return a new string object
            */
            String(const unsigned char * str, size_type length);

            /**
                @brief UTF-16 string constructor
                @param str a string of UTF-16 character codes
                @param length the length of the @e str array
                @return a new string object.
            */
            String(const unsigned short * str, size_type length);

            /**
                @brief UTF-32 string constructor
                @param str an array of UTF-32 codes
                @param length the length of the @e str array
                @return a new string object.
            */
            String(const unsigned int * str, size_type length);

            /**
             *  @brief UTF-32 single character constructor
             *  @param c the single UTF-32 character in the platform encoding.
             *  @return a new string object of length 1.
             */
            String(unsigned int c);

            /**
             * @brief constructor from data object.
             * @param data the data object.
             */
            explicit String(const data_type & data);

            /** @brief destructor */
            ~String() override = default;

#pragma mark - Copy methods

            String deepCopy() const;

#pragma mark - Initialization routines

            /**
                @brief static method to create a string object using a specified format
                @param format a C-style string containing the format of the string contents
                @return a new string object

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
                and a pointer to a String respectively.

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
            static String initWithFormat(const char * format, ...);

            /**
             * @brief Initializes a String object using ASCII encoded strings.
             * @param str the ASCII encoded string
             * @return the initialized String
             *
             * The init string can contain standard ASCII as well as escaped unicode characters
             * with the following form: \:000001 -> u0001
             *
             * @code
             * String s = String::initWithASCIIEncodedUnicode("Hello World of \\:003049 \\:003050 Unicode");
             * @endcode
             */
            static String initWithASCIIEncodedUnicode(const char * str);

            /**
             * @brief Initializes a String object using JSON encoded strings.
             * @param str the JSON encoded string
             * @return the initialized String;
             *
             * The init string can contain UTF-8 as well as escaped unicode characters
             * with the following form: \uXXXX
             *
             * @code
             * String s = String::initWithJSONEncodedUnicode("Hello World of \\u3049 \\u3050 Unicode");
             * @endcode
             */
            static String initWithJSONEncodedUnicode(const char * str);

            /**
             * @brief Initializes a String object using JSON encoded strings.
             * @param str the JSON encoded string
             * @param length the number of bytes in @e str
             * @return the initialized String;
             *
             * The init string can contain UTF-8 as well as escaped unicode characters
             * with the following form: \uXXXX
             *
             * @code
             * String s = String::initWithJSONEncodedUnicode("Hello World of \\u3049 \\u3050 Unicode", 36);
             * @endcode
             */
            static String initWithJSONEncodedUnicode(const char * str, size_type length);

#pragma mark - Iterator methods

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

#pragma mark - Equality operators

            /**
                @brief Compares two strings for equality.
                @param s the string to compare.
                @return true if the two strings contain identical content, false otherwise.
            */
            bool operator==(const String & s) const;

            /**
                @brief Compares two strings character by character to see if they do not match.
                @param s the string to compare.
                @return true if the two strings contain differing characters, false otherwise.
            */
            bool operator!=(const String & s) const;

#pragma mark - Assignment operators

            /**
                @brief Assignment operator
                @param s the string to assign.
                @return a reference to the current string updated with the contents of s.
            */
            String & operator=(const String & s);

            /** @brief overloaded rvalue assignment operator */
            String & operator=(String && s) noexcept;

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
            size_type numberOfBytes() const;

#pragma mark - Method for inspecting aspects of a string

            /**
             * @brief method to check if a string is empty
             * @return true if the string has no characters
             *
             * Use empty instead of length() == 0 to check for an empty string.
             */
            auto empty() const -> bool;

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
            unicode_point_type characterAtIndex(size_type i) const;

            /**
             * @brief method to get the first character in the string.
             * @return the first character.
             *
             * This method will throw an exception if the string contains
             * no characters.
             */
            [[nodiscard]] auto first() const -> unicode_point_type;

            /**
             * @brief method to get the last character in the string.
             * @return the last character.
             *
             * This method will throw an exception if the string contains
             * no characters.
             */
            [[nodiscard]] auto last() const -> unicode_point_type;

            /**
                @brief Returns a new string object with the characters that lie in @e range.
                @param range the range of characters
                @return a new string object containing the requested range of characters.
            */
            String getCharactersInRange(const range_type & range) const;

#pragma mark - Methods for getting C strings

            /**
                @brief return a C string version (if possible) of the string object.
                @return a smart pointer to a C-style string.
            */
            std::unique_ptr<char[]> cStr() const;

            /**
                @brief return a STL string version (if possible) of the string object.
                @return a STL string.
            */
            std::string stlString() const;

#pragma mark - Methods for combining strings

            /**
                @brief create a new string object by appending contents as specified in the format
                @param format a C-Style string containing the format of the string to append
                @return a new string object containing the appended results

                stringByAppendingFormat is a variable argument method used to append arbitrary
                contents to a string.  The format behavior behaves in the same manner as the
                initWithFormat() function.
            */
            String stringByAppendingFormat(const char * format, ...) const;

            /**
                @brief create a new string object by appending the contents of the argument string
                @param str the string to use for appending
                @return a new string object containing the appended results
            */
            String stringByAppendingString(const String & str) const;

            /**
                @brief create a new string by concatenating two strings.
                @param s1 the first string
                @param s2 the second string
                @return a new string created by joining two strings.
            */
            static String concatenateStrings(const String & s1, const String & s2);

            /**
             * @brief method to create a new string by 'adding/appending' the contents of
             * another string.
             * @brief s the other string
             * @return a new string with the original string's contents augemented with @s.
             */
            String operator+(const String & s) const;

            /**
             * @brief method to create a new string by 'adding/appending' a character.
             * @param c the other character
             * @return a new string with the original string's contents with @e c appended.
             */
            String operator+(const char c) const;

            /**
             * @brief method to create a new string by appending a unicode character.
             * @param c the other character
             * @return a new string with the origin string's contents with @e c appended.
             */
            String operator+(const unicode_point_type c) const;

            /**
             * @brief method to append a string to the current string.
             * @param s the other string
             * @return this string with the contents of s appended.
             */
            String & operator+=(const String & s);

            /**
             * @brief method to append a string to the current string.
             * @param c the other character
             * @return this string with the character @e c appended.
             */
            String & operator+=(const char c);

            /**
             * @brief method to append a unicode character value to the current string
             * @param c the character
             * @return this wtring with the character &e c appended.
             */
            String & operator+=(const unicode_point_type c);

#pragma mark - Methods for dividing strings

            /**
                @brief return a new string object representing the characters in the original string starting
                from the index to the end of the string
                @param i an integer variable containing the starting index of the sub-string.
                @return a new string object that contains the requested sub-string
            */
            String substringFromIndex(size_type i) const;

            /**
                @brief return a substring of the original string represented by the range parameter.
                @param range a variable containing the location and length of the requested sub-string.
                @return a new string object that contains the requested sub-string
            */
            String substringWithRange(const range_type & range) const;

            /**
                @brief return an array of substrings not contained in the range.
                @param range the range of substring not to match.
                @return an array of substrings from the string that are not in the specified @e range.
            */
            string_array_type substringsNotInRange(const range_type & range) const;

            /**
                @brief returns a new string containing the characters of the receiver up to,
                but not including, the one at a given index.
                @param i the index of the last desired character in the sub-string
                @return a new string object that contains the requested sub-string.
            */
            String substringToIndex(size_type i) const;

            /**
                @brief return an array of substrings of all the content in the string that does not match the substring
                argument.
                @param str the substring to match.
                @return an array of substrings from the string that do not match @e str.
            */
            string_array_type substringsThatDoNotMatchString(const String & str) const;

            /**
                @brief return an array of substrings created by splitting the string into chunks separated by @e
               splitString.
                @param splitString the string to use for matching
                @return an array of strings containing the substrings
            */
            string_array_type split(const String & splitString) const;

#pragma mark - Methods for finding characters and substrings

            /**
                @brief return a Range object containing the location of the first instance of the starting character
                and the length of the string given as a parameter
                @param str a string object indicating the sub-string to find
                @return a Range object containing the index of the starting character and the length of the sub-string
            */
            range_type rangeOfString(const String & str) const;

            /**
                @brief return an array of TFRange objects containing the locations of the substring in the string.
                @param str the substring.
                @return an array of ranges.
            */
            range_array_type rangesOfString(const String & str) const;

            /**
             * @brief method to check if a string contains a substring.
             * @param str the substring
             * @return true if the string contains @e str.
             */
            auto contains(const String & str) const -> bool;

            /**
             * @brief method to remove space characters at the beginning of a string and at
             * the end of a string.
             */
            void trim();

#pragma mark - Methods for replacing substrings

            /**
                @brief return a string created by replacing instances of a sub-string with another string
                @param original the sub-string to replace
                @param replacement the string to insert in place of @e original
                @return a new string object
            */
            String stringByReplacingOccurencesOfStringWithString(const String & original,
                                                                 const String & replacement) const;

            /**
                @brief return a string created by replacing the sub-string in the specified range with another string
                @param range the origin and length of the sub-string to replace
                @param str the string to insert in place of the string int @e range
                @return a new string object
            */
            String stringByReplacingCharactersInRangeWithString(const range_type & range, const String & str) const;

#pragma mark - Methods for comparing strings

            /**
                @brief return a ComparisonResult indicating the lexical ordering of the string with another string
                @param str the string object to compare
                @return OrderedAscending if the string precedes @e str lexically,
                        OrderedSame if the string and @e str have an equivalent lexical value and
                    OrderedDescending if the string lexically follows @e str
            */
            ComparisonResult compare(const String & str) const;

            /**
                @brief return a ComparisonResult indicating the lexical ordering of a sub-string
                       specified by a range with respect to another string
                @param range the origin and length of the sub-string to use for comparison
                @param str the string to use for comparison with the sub-string
                @return OrderedAscending if the sub-string precedes @e str lexically,
                    OrderedSame if the sub-string and @e str have an equivalent lexical value, and
                        OrderedDescending if the sub-string lexically follows @e str
            */
            ComparisonResult compareRangeWithString(const range_type & range, const String & str) const;

            /**
                @brief returns a boolean value indicating whether the initial characters of the string
                       match another string
                @param str a string
                @return @c true if the string's first characters match the values in @e str, @c false if
                        the string's first characters do not match the values in @e str
            */
            bool hasPrefix(const String & str) const;

            /**
                @brief returns a boolean value indicating whether the last characters of the string
                           match another string
                @param str a string
                @return @c true if the string's last characters match the values in @e str, @c false if
                        the string's last characters do not match the values in @e str
            */
            bool hasSuffix(const String & str) const;

            /**
                @brief returns a boolean value indicating whether the string is equal to another
                       string by doing a literal Unicode-based comparison
                @param str a string
                @return @c true if the strings compare equal, otherwise @c false
            */
            bool isEqualToString(const String & str) const;

#pragma mark - Methods for changing character case

            /**
                @brief return a capitalized representation of the string
                @return A new string with the first character from each word in the string
                        changed to its corresponding uppercase value, and all remaining characters
                        changed to their corresponding lowercase values
            */
            String capitalizedString() const;

            /**
                @brief return a lowercase representation of the string
                @return A string with each character of the original replaced by its corresponding
                    lowercase value
            */
            String lowercaseString() const;

            /**
                @brief return a uppercase representation of the string
                @return A string with each character of the original replaced by its corresponding
                        uppercase value
             */
            String uppercaseString() const;

#pragma mark - Methods for converting to ASCII encoded string

            std::string getAsASCIIEncodedSTLString(void) const;

#pragma mark - Methods for converting a string to a data object

            /**
                @brief return a Data object with the
                contents of the string.
                 @return a new Data object that
                contains the contents of the string.
            */
            data_type getAsData() const;

            /**
                @brief return a Data object with the contents of the
                string in ASCII encoding.
                @return a new Data object that contains a copy
                of the string in ASCII encoding.
            */
            data_type getAsDataInASCIIEncoding() const;

            /**
             * @brief return a Data object with the contents of the
             * string in JSON encoding.
             * @return a new Data object that contains a copy
             * of the string in JSON encoding.
             */
            data_type getAsDataInJSONEncoding() const;

            /**
                @brief return a Data object with the contents of the
                string in UTF-8 encoding.
                @return a new Data object that contains a copy
                of the string in UTF-8 encoding.
            */
            data_type getAsDataInUTF8Encoding() const;

            /**
                @brief return a Data object with the contents of the
                string in UTF-16 encoding.
                @return a new Data object that contains a copy
                of the string in UTF-16 encoding.
            */
            data_type getAsDataInUTF16Encoding() const;

            /**
                @brief return a Data object with the contents of the
                string in UTF-32 encoding.
                @return a new Data object that contains a copy
                of the string in UTF-32 encoding.
            */
            data_type getAsDataInUTF32Encoding() const;

#pragma mark - static method to get string encoder

            /**
             * @brief static method to get an encoder object used by the string class.
             * @return the encoder object.
             */
            [[nodiscard]] static auto getEncoder() -> StringEncoder &;

#pragma mark - Methods for writing to an ostream

            /** @brief std::ostream& interaction method */
            std::ostream & description(std::ostream & o) const;

        private:
#pragma mark - Shadow methods for variadic argument functions

            /**
                @brief static method to create a string object using a specified format
                @param format a C-style string containing the format of the string contents
                @param argList a va_list object referencing the other arguments for the format.
                @return a new string object.

                initWithFormat is a variable argument method used to create string object
                according to a format template.   This method behaves in the same manner as
                the standard printf() family of functions.
            */
            static String initWithFormat(const char * format, va_list * argList);

            /**
                @brief create a new string object by appending contents as specified in the format
                @param format a C-Style string containing the format of the string to append
                @param argList a va_list pointer referencing the other arguments for the format.
                @return a new string object containing the appended results

                stringByAppendingFormat is a variable argument method used to append arbitrary
                contents to a string.  The format behavior behaves in the same manner as the
                printf() family of functions.
            */
            String stringByAppendingFormat(const char * format, va_list * argList) const;

            using encoder_type = StringEncoder;

            static data_type convertToThisEncoding(const String & s, encoder_type * encoder);

            using core_type = StringCore;

            using core_pointer_type = std::shared_ptr<core_type>;

            core_pointer_type core;

            friend std::hash<String>;
        };

        std::ostream & operator<<(std::ostream & o, const String & s);

        /**
         * @brief function to compare two strings
         * @param stringOne
         * @param stringTwo
         * @param context a user supplied string context.
         * @return OrderedAscending if @e stringOne is lexically less than @e stringTwo, OrderedDescending if
         *   @e stringOne is lexically greater than @e stringTwo and OrderedSame if @e stringOne equals @e stringTwo.
         */
        ComparisonResult compareStrings(const String & stringOne, const String & stringTwo, void * context);

        /**
         * @brief overloaded == operator to compare a C-style string and a String.
         * @param s the C-style string
         * @param t the String object.
         * @return true if the two strings are lexically equal and false otherwise.
         */
        bool operator==(const char * s, const String & t);

        /**
         * @brief overloaded == operator to compare a STL string and a String
         * @param s the STL string
         * @param t the String object
         * @return true if the two strings are lexiccally equal and false otherwise.
         */
        bool operator==(const std::string & s, const String & t);

        /**
         * @brief overloaded < operator to compare two String objects
         * @param a the first String object
         * @param b the second String object
         * @return true if @e a is lexically less than @e b and false otherwise.
         */
        bool operator<(const String & a, const String & b);

        /**
         * @brief overloaded > operator to compare two String objects
         * @param a the first String object
         * @param b the second String object
         * @return true if @e a is lexically greater than @e b and false otherwise.
         */
        bool operator>(const String & a, const String & b);

        /**
         * @brief overloaded + operator to concatenate a C-style string and a String.
         * @param a the C-style string
         * @param b the String object
         * @return a String object with @e a concatenated with @e b
         */
        String operator+(const char * a, const String & b);

        /**
         * @brief overloaded + operator to concatenate a STL string and a String
         * @param a the STL string
         * @param b the String object
         * @return a String object with @e a concatenated with @e b.
         */
        String operator+(const std::string & a, const String & b);

    } // namespace Foundation

} // namespace TF

namespace std
{
    template<>
    struct hash<TF::Foundation::String>
    {
        typedef TF::Foundation::String argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const & s) const noexcept
        {
            auto bytes = s.core->data();
            auto length = s.core->length();

            // Simple hash function from stack overflow.
            // TODO: Use a better hash function.
            result_type hash = 7;
            for (decltype(length) i = 0; i < length; i++)
            {
                hash = hash * 31 + static_cast<result_type>(*(bytes + i));
            }
            return hash;
        }
    };
} // namespace std

#endif /* TFSTRING_HPP */
