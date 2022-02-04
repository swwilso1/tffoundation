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

#ifndef TFDATETYPES_HPP
#define TFDATETYPES_HPP

#define NEEDS_CTIME
#define NEEDS_OSTREAM
#include "tfheaders.hpp"

namespace TF
{

    namespace Foundation
    {

        enum class DayOfWeek
        {
            Sunday,
            Monday,
            Tuesday,
            Wednesday,
            Thursday,
            Friday,
            Saturday
        };

        std::ostream & operator<<(std::ostream & o, const DayOfWeek & d);

        enum class MonthOfYear
        {
            January,
            February,
            March,
            April,
            May,
            June,
            July,
            August,
            September,
            October,
            November,
            December
        };

        std::ostream & operator<<(std::ostream & o, const MonthOfYear & m);

        enum class HalfOfDay
        {
            AM,
            PM
        };

        std::ostream & operator<<(std::ostream & o, const HalfOfDay & h);

        std::ostream & operator<<(std::ostream & o, const struct tm & t);

    } // namespace Foundation

} // namespace TF

#endif // TFDATETYPES_HPP
