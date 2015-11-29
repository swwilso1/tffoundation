/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 Tectiform Inc.

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

#ifndef TFTAB_HPP
#define TFTAB_HPP 1

#define NEEDS_OSTREAM
#define NEEDS_SSTREAM
#define NEEDS_STRING
#include "tfheaders.hpp"
#include "tfallocator.hpp"
#include "tftypes.hpp"

namespace TF
{

	namespace Foundation
	{

		class Tab : public AllocatorInterface
		{
		public:

			/** @brief simple string type */
			using string_type = std::string;

			/** @brief simple size type */
			using size_type = Size_t;

			/** @brief default constructor */
			Tab() { tabs = 1; tabWidth = 4; }

			/**
			 *  @brief constructor with number of tabs.
			 *  @param t the number of tabs.
			 */
			Tab(const size_type &t) { tabs = t; }

			/**
			 *  @brief constructor with number of tabs and tab width.
			 *  @param t the number of tabs.
			 *  @param w the width of each tab in spaces.
			 */
			Tab(const size_type &t, const size_type &w)
			{ tabs = t; tabWidth = w; }

			string_type str() const
			{
				std::stringstream collector;
				for (size_type i = 0; i < (tabs * tabWidth); i++) collector << " ";
				return collector.str();
			}

			size_type size(void) const { return tabs * tabWidth; }

			std::ostream& writeToStream(std::ostream &o) const
			{
				o << str();
				return o;
			}

		private:

			size_type tabs;
			size_type tabWidth;

		};


		/**
		 *  @brief overloaded operator<< for writing tabs to streams
		 *  @param o the stream.
		 *  @param t the tab.
		 *  @return @e o the stream.
		 */
		std::ostream& operator<<(std::ostream &o, const Tab &t);

	} // Foundation

} // TF

#endif // TFTAB_HPP

