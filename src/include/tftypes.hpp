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

#ifndef TFTYPES_HPP
#define TFTYPES_HPP 1

#define NEEDS_TYPE_TRAITS
#define NEEDS_CSTDDEF
#define NEEDS_LIMITS_H
#include "tfheaders.hpp"

namespace TF
{

    namespace Foundation
    {
        /** @brief type used to track intervals in time */
        using TimeInterval = double;

        /** @brief unsigned integer type */
        using UnsignedInteger = unsigned int;

        /** @brief size_t type */
        using Size_t = std::size_t;

        /** @brief float type */
        using Float = float;

        /** @brief 2 dimensional size (width, height) */
        template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
        struct Size
        {
            T width{};
            T height{};

            /** @brief lvalue constructor */
            Size(const T & w, const T & h) : width(w), height(h) {}

            bool operator==(const Size & s) const
            {
                return width == s.width && height == s.height;
            }

            bool operator!=(const Size & s) const
            {
                return ! (*this == s);
            }
        };

        /** @brief Cartesian point (x,y) */
        template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
        struct Point
        {
            T x{};
            T y{};

            /** @brief lvalue constructor */
            Point(const T & a, const T & b) : x(a), y(b) {}

            bool operator==(const Point & p) const
            {
                return x == p.x && y == p.y;
            }

            bool operator!=(const Point & p) const
            {
                return ! (*this == p);
            }
        };

        /** @brief basic rectangle */
        template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
        struct Rectangle
        {
            Point<T> origin{};
            Size<T> size{};

            /** @brief width/height, x/y constructor */
            Rectangle(const T & w, const T & h, const T & x, const T & y) : origin{w, h}, size{x, y} {}

            bool operator==(const Rectangle & r) const
            {
                return origin == r.origin && size == r.size;
            }

            bool operator!=(const Rectangle & r) const
            {
                return ! (*this == r);
            }
        };

        /** @brief range object */
        struct Range
        {
            Size_t position{};
            Size_t length{};

            Range() = default;

            /** @brief lvalue constructor */
            Range(const Size_t & p, const Size_t & l);

            auto operator==(const Range & r) const -> bool;
            bool operator!=(const Range & r) const;
            auto is_empty() const -> bool;
        };

        /**
         * @class Simple templated class that allows returning a result
         * value and a success/failure value from a function.
         * @tparam T the type of value.
         */
        template<typename T>
        struct Result
        {
            T value;
            bool succeeded;

            Result() : value{}, succeeded{false} {}
        };

    } // namespace Foundation

} // namespace TF

#endif /* TFTYPES_HPP */
