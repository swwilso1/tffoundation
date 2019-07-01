/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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
        using Size_t = size_t;

        /** @brief float type */
        using Float = float;

        /** @brief 2 dimensional size (width, height) */
        struct Size
        {
            Float width;
            Float height;

            /** @brief default constructor */
            Size() : width(0.0), height(0.0)
            {
            }

            /** @brief lvalue constructor */
            Size(const Float &w, const Float &h) : width(w), height(h)
            {
            }

            /** @brief rvalue constructor */
            Size(const Float &&w, const Float &&h) : width(w), height(h)
            {
            }

            bool operator==(const Size &s) const
            {
                return width == s.width && height == s.height;
            }


            bool operator!=(const Size &s) const
            {
                if(*this == s)
                    return false;
                return true;
            }
        };


        /** @brief Cartesian point (x,y) */
        struct Point
        {
            Float x;
            Float y;

            /** @brief default constructor */
            Point() : x(0.0), y(0.0)
            {
            }

            /** @brief lvalue constructor */
            Point(const Float &a, const Float &b) : x(a), y(b)
            {
            }

            /** @brief rvalue constructor */
            Point(const Float &&a, const Float &&b) : x(a), y(b)
            {
            }


            bool operator==(const Point &p) const
            {
                return x == p.x && y == p.y;
            }


            bool operator!=(const Point &p) const
            {
                if(*this == p)
                    return false;
                return true;
            }
        };


        /** @brief basic rectangle */
        struct Rectangle
        {
            Point origin;
            Size size;

            /** @brief point/size constructor */
            Rectangle(const Point &p, const Size &s) : origin(p), size(s)
            {
            }

            /** @brief width/height, x/y constructor */
            Rectangle(const Float &w, const Float &h, const Float &x, const Float &y) : origin(w, h), size(x, y)
            {
            }


            bool operator==(const Rectangle &r) const
            {
                return origin == r.origin && size == r.size;
            }


            bool operator!=(const Rectangle &r) const
            {
                if(*this == r)
                    return false;
                return true;
            }
        };


        /** @brief range object */
        struct Range
        {
            Size_t position;
            Size_t length;

            /** @brief default constructor */
            Range() : position(0), length(0)
            {
            }

            /** @brief lvalue constructor */
            Range(const Size_t &p, const Size_t &l) : position(p), length(l)
            {
            }


            bool operator==(const Range &r) const
            {
                return position == r.position && length == r.length;
            }


            bool operator!=(const Range &r) const
            {
                if(*this == r)
                    return false;
                return true;
            }
        };

    }    // namespace Foundation

}    // namespace TF

#endif /* TFTYPES_HPP */
