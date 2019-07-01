/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

#ifndef TFBASE64_HPP
#define TFBASE64_HPP

#define NEEDS_MAP
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdata.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        class Base64 : public AllocatorInterface
        {
        public:
            using data_type = Data;

            using string_type = String;

            static string_type encode(const data_type &data);

            static data_type decode(const string_type &s);

        private:
            using encoding_map_type = std::map<int, char>;

            using decoding_map_type = std::map<char, int>;

            static encoding_map_type sEncodingMap;
            static encoding_map_type loadEncodingMap();

            static decoding_map_type sDecodingMap;
            static decoding_map_type loadDecodingMap();

            static const int sBaseNumberOfBytes = 3;

            static const int sBitMask = 0x3f;

            static const int sByteMask = 0xff;

            static const char sPadChar = '=';
        };


    }    // namespace Foundation

}    // namespace TF

#endif    // TFBASE64_HPP
