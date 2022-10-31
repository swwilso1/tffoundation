/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#define NEEDS_RANDOM
#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tfrandomstringgenerator.hpp"

namespace TF::Foundation
{

    auto RandomStringGenerator::string_with_length(int length) -> string_type
    {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937_64 generator(static_cast<std::mt19937_64::result_type>(seed));
        std::uniform_int_distribution<> distribution(97, 122);
        string_type random_name_portion;
        for (decltype(length) i = 0; i < length; i++)
        {
            char value = static_cast<char>(distribution(generator));
            random_name_portion += value;
        }
        return random_name_portion;
    }

} // namespace TF::Foundation
