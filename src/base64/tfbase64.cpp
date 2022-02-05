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

#include "tfbase64.hpp"

namespace TF
{

    namespace Foundation
    {

        Base64::encoding_map_type Base64::sEncodingMap = loadEncodingMap();

        Base64::decoding_map_type Base64::sDecodingMap = loadDecodingMap();

        Base64::encoding_map_type Base64::loadEncodingMap()
        {
            encoding_map_type theMap;
            char theChar = 'A';

            // Load the capital letters first.
            for (int i = 0; i < 26; i++)
                theMap.insert(std::make_pair(i, theChar + i));

            // Now load the lowercase letters.
            theChar = 'a';
            for (int i = 26; i < 52; i++)
                theMap.insert(std::make_pair(i, theChar + i - 26));

            // Now load the digits 0-9
            theChar = '0';
            for (int i = 52; i < 62; i++)
                theMap.insert(std::make_pair(i, theChar + i - 52));

            // Insert the last two chars.
            theMap.insert(std::make_pair(62, '+'));
            theMap.insert(std::make_pair(63, '/'));

            return theMap;
        }

        Base64::decoding_map_type Base64::loadDecodingMap()
        {
            decoding_map_type theMap;
            char theChar = 'A';

            // Insert the capital letters.
            for (int i = 0; i < 26; i++)
                theMap.insert(std::make_pair(theChar + i, i));

            // Insert the lowercase letters.
            theChar = 'a';
            for (int i = 26; i < 52; i++)
                theMap.insert(std::make_pair(theChar + i - 26, i));

            // Insert the digits 0-9
            theChar = '0';
            for (int i = 52; i < 62; i++)
                theMap.insert(std::make_pair(theChar + i - 52, i));

            // Insert the last two chars.
            theMap.insert(std::make_pair('+', 62));
            theMap.insert(std::make_pair('/', 63));

            return theMap;
        }

        Base64::string_type Base64::encode(const data_type & data)
        {
            string_type s;
            const char * bytes = nullptr;
            unsigned int holder = 0;
            data_type dataCopy = data;

            data_type::size_type padBytesToAdd = sBaseNumberOfBytes - (dataCopy.length() % sBaseNumberOfBytes);

            padBytesToAdd = padBytesToAdd == sBaseNumberOfBytes ? 0 : padBytesToAdd;

            for (data_type::size_type i = 0; i < padBytesToAdd; i++)
            {
                char byte = 0;
                dataCopy.append(&byte, 1);
            }

            bytes = dataCopy.bytes();

            for (data_type::size_type i = 0; i < dataCopy.length(); i++)
            {
                holder = static_cast<decltype(holder)>(bytes[i++]);
                holder <<= 8;

                holder |= static_cast<decltype(holder)>(bytes[i++]);
                holder <<= 8;

                holder |= static_cast<decltype(holder)>(bytes[i]);

                auto value = (holder >> 18) & sBitMask;
                auto encodedValue = sEncodingMap[static_cast<int>(value)];

                s += encodedValue;

                value = (holder >> 12) & sBitMask;
                encodedValue = sEncodingMap[static_cast<int>(value)];

                s += encodedValue;

                value = (holder >> 6) & sBitMask;
                encodedValue = sEncodingMap[static_cast<int>(value)];

                s += encodedValue;

                value = holder & sBitMask;
                encodedValue = sEncodingMap[static_cast<int>(value)];

                s += encodedValue;
            }

            if (padBytesToAdd > 0)
            {
                s = s.substringWithRange(Range(0, s.length() - padBytesToAdd));
                for (data_type::size_type i = 0; i < padBytesToAdd; i++)
                    s += sPadChar;
            }

            return s;
        }

        Base64::data_type Base64::decode(const string_type & s)
        {
            data_type d;
            unsigned int decodeHelper = 0;
            size_type padCharsFound = 0;

            if (s.length() == 0)
                return d;

            // The string only has ASCII codes in it so we can go character by character
            // and convert back to the byte array.
            for (string_type::size_type i = 0; i < s.length(); i++)
            {
                auto code = s[i++];
                padCharsFound += code == sPadChar ? 1 : 0;
                auto value = code == sPadChar ? 0 : sDecodingMap[static_cast<char>(code)];
                decodeHelper = static_cast<decltype(decodeHelper)>(value);

                code = s[i++];
                padCharsFound += code == sPadChar ? 1 : 0;
                value = code == sPadChar ? 0 : sDecodingMap[static_cast<char>(code)];
                decodeHelper = (decodeHelper << 6) | static_cast<decltype(decodeHelper)>(value);

                code = s[i++];
                padCharsFound += code == sPadChar ? 1 : 0;
                value = code == sPadChar ? 0 : sDecodingMap[static_cast<char>(code)];
                decodeHelper = (decodeHelper << 6) | static_cast<decltype(decodeHelper)>(value);

                code = s[i];
                padCharsFound += code == sPadChar ? 1 : 0;
                value = code == sPadChar ? 0 : sDecodingMap[static_cast<char>(code)];
                decodeHelper = (decodeHelper << 6) | static_cast<decltype(decodeHelper)>(value);

                char byte = static_cast<char>((decodeHelper >> 16) & sByteMask);
                d.append(&byte, 1);

                byte = static_cast<char>((decodeHelper >> 8) & sByteMask);
                d.append(&byte, 1);

                byte = static_cast<char>(decodeHelper & sByteMask);
                d.append(&byte, 1);
            }

            d = d.subdataWithRange(Range(0, d.length() - padCharsFound));

            return d;
        }

    } // namespace Foundation

} // namespace TF
