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

#ifndef TFDURATIONTYPES_HPP
#define TFDURATIONTYPES_HPP

#define NEEDS_CHRONO
#include "tfheaders.hpp"

namespace TF::Foundation
{
    /** @brief duration type representing decades */
    using decades = std::chrono::duration<uint64_t, std::ratio<315569520, 1>>;

    /** @brief duration type representing centuries */
    using centuries = std::chrono::duration<uint64_t, std::ratio<3155695200, 1>>;

    /** @brief duration type representing millenia */
    using millenia = std::chrono::duration<uint64_t, std::ratio<31556952000, 1>>;

    using microsecondsd = std::chrono::duration<double, std::micro>;

    using millisecondsd = std::chrono::duration<double, std::milli>;

    using secondsd = std::chrono::duration<double, std::ratio<1, 1>>;

    using minutesd = std::chrono::duration<double, std::ratio<60, 1>>;

    using hoursd = std::chrono::duration<double, std::ratio<3600, 1>>;

    using daysd = std::chrono::duration<double, std::ratio<86400, 1>>;

    using weeksd = std::chrono::duration<double, std::ratio<604800, 1>>;

    using monthsd = std::chrono::duration<double, std::ratio<2629746, 1>>;

    using yearsd = std::chrono::duration<double, std::ratio<31556952, 1>>;

    using decadesd = std::chrono::duration<double, std::ratio<315569520, 1>>;

    using centuriesd = std::chrono::duration<double, std::ratio<3155695200, 1>>;

    using milleniad = std::chrono::duration<double, std::ratio<31556952000, 1>>;
} // namespace TF::Foundation

#endif // TFDURATIONTYPES_HPP
