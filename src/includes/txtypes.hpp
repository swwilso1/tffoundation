/* begin copyright *******************************************************

                    Tectiform Inc. source file

        Copyright 2011 through 2015 by Tectiform Inc.

This material contains trade secrets and may be registered with the
U.S. Copyright Office as an unpublished work, pursuant to Title 17,
U.S. Code, Section 408.  Unauthorized copying, adaptation, distribution
or display is prohibited.

********************************************************* end copyright */

#ifndef TXTYPES_HPP
#define TXTYPES_HPP 1

namespace TX
{

	/** @brief type used to track intervals in time */
	using TimeInterval = double;

	/** @brief unsigned integer type */
	using UnsignedInteger = unsigned int;

	/** @brief float type */
	using Float = float;

	/** @brief 2 dimensional size (width, height) */
	struct Size
	{
		Float width;
		Float height;

		/** @brief default constructor */
		Size() : width(0.0), height(0.0) {}

		/** @brief lvalue constructor */
		Size(const Float &w, const Float &h) : width(w), height(h) {}

		/** @brief rvalue constructor */
		Size(const Float &&w, const Float &&h) : width(w), height(h) {}
	};


	/** @brief Cartesian point (x,y) */
	struct Point
	{
		Float x;
		Float y;

		/** @brief default constructor */
		Point() : x(0.0), y(0.0) {}

		/** @brief lvalue constructor */
		Point(const Float &a, const Float &b) : x(a), y(b) {}

		/** @brief rvalue constructor */
		Point(const Float &&a, const Float &&b) : x(a), y(b) {}
	};


	/** @brief basic rectangle */
	struct Rectangle
	{
		Point origin;
		Size size;

		/** @brief point/size constructor */
		Rectangle(const Point &p, const Size &s) : origin(p), size(s) {}

		/** @brief width/height, x/y constructor */
		Rectangle(const Float &w, const Float &h, const Float &x, const Float &y) :
			origin(w,h), size(x,y) {}
	};


	/** @brief range object */
	struct Range
	{
		UnsignedInteger position;
		UnsignedInteger length;

		/** @brief default constructor */
		Range() : position(0), length(0) {}

		/** @brief lvalue constructor */
		Range(const UnsignedInteger &p, const UnsignedInteger &l) : position(p),
				length(l) {}
	};

} // TX

#endif /* TXTYPES_HPP */
