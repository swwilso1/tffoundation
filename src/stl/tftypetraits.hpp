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
SOFTWARE.


******************************************************************************/

#ifndef TFTYPETRAITS_HPP
#define TFTYPETRAITS_HPP

#include <type_traits>

namespace TF
{

    namespace Foundation
    {

        namespace Traits
        {

            struct NO
            {};
            template<typename T, typename Arg>
            NO operator==(const T &, const Arg &);

            template<typename T, typename Arg = T>
            struct has_operator_equal
            {
                enum
                {
                    value = ! std::is_same<decltype(std::declval<T>() == std::declval<Arg>()), NO>::value
                };
            };

        } // namespace Traits

    } // namespace Foundation

} // namespace TF

#endif // TFTYPETRAITS_HPP
