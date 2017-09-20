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

#ifndef TFCLASSFORMATTER_HPP
#define TFCLASSFORMATTER_HPP 1

#define NEEDS_OSTREAM
#define NEEDS_STRING
#define NEEDS_VECTOR
#include "tfheaders.hpp"
#include "tfallocator.hpp"
#include "tftypes.hpp"
#include "tfclassmember.hxx"

namespace TF
{

	namespace Foundation
	{

		class ClassFormatter : public AllocatorInterface
		{
		public:

			/** @brief basic simple string type */
			using string_type = std::string;

			/** @brief basic member type */
			using member_type = ClassMember *;

			/** @brief basic member list */
			using member_list_type = std::vector<member_type>;

			/** @brief template list type */
			using template_list_type = std::vector<string_type>;

			/** @brief basic size type */
			using size_type = Size_t;


			/**
			 *  @brief virtual destructor to allow other classes to provide
			 *  a different formatter.
			 */
			virtual ~ClassFormatter();


			/**
			 *  @brief simple static function to set output column width.
			 *  @param w the new column width.
			 */
			static void setColumnWidth(size_type w);

			/**
			 *  @brief simple static function to get the output column width.
			 *  @return the column width.
			 */
			static size_type getColumnWidth(void);

			/**
			 *  @brief simple static function for setting the tab width.
			 *  @param w the new tab width in spaces.
			 */
			static void setTabWidth(size_type w);

			/**
			 *  @brief simple static function to get the tab width.
			 *  @return the tab width in spaces.
			 */
			static size_type getTabWidth(void);

			/**
			 *  @brief function to set the name of the class to format.
			 *  @param name the name of the class.
			 */
			void setClassName(const string_type &name);

			/**
			 *  @brief function for adding a new class member to the list
			 *  all in string form.
			 *  @param type the type of the member.
			 *  @param name the name of the member.
			 *  @param value the value of the member.
			 */
			void addClassMember(const string_type &type, const string_type &name,
					const string_type &value);

			/**
			 *  @brief function for adding an indexed member (such as an array member)
			 *  to the class.
			 *  @param i the index of the member
			 *  @param type the type of the member
			 *  @param value the value of the parameter
			 */
			 void addClassMember(size_type i, const string_type &type, const string_type &value);


			/**
			 * @brief function for adding a void * class member
			 * @param n the name of the member
			 * @param value the void * value
			 */
			void addClassMember(const string_type &n, void *value);


			 /**
			  *  @brief function for adding an indexed member (such as an array member)
			  *  to the class.
			  *  @tparam T the type of the class member
			  *  @param i the index of the parameter
			  *  @param value the value of the parameter
			  */
			 template<typename T>
			 void addClassMember(size_type i, const T &value);

			/**
			 * @brief function for adding a new class member to the member list.
			 * @tparam T the type of the class member.
			 * @param name the member name
			 * @param value the member value.
			 */
			template<typename T>
			void addClassMember(const string_type &name, const T &value);

			/**
			 *  @brief function for adding a new class member to the member list.
			 *  This version explicitly allows the caller to set the type value.
			 *  @tparam T the type of the class member.
			 *  @param type the type of the member.
			 *  @param name the name of the member.
			 *  @param value the member value.
			 */
			template<typename T>
			void addClassMember(const string_type &type, const string_type &name,
					const T &value);

			/**
			 *  @brief function for adding an array to the member list.
			 *  @tparam T the type of the class member.
			 *  @param name the member name.
			 *  @param value the pointer to the start of the array.
			 *  @param length the number of elements in the array.
			 */
			template<typename T>
			void addClassMember(const string_type &name, const T *value,
					const size_type &length);

			/**
			 *  @brief function for adding an array to the member list.
			 *  @tparam T the type of the class member.
			 *  @param type the type of the member.
			 *  @param name the member name.
			 *  @param value the pointer to the start of the array.
			 *  @param length the number of elements in the array.
			 */
			template<typename T>
			void addClassMember(const string_type &type, const string_type &name,
					const T *value, const size_type &length);

			/**
			 *  @brief function for adding a class template type to the formatter.
			 *  @tparam the template type.
			 */
			template<typename T>
			void addClassTemplateType();


			/**
			 *  @brief function for adding a class template type manually to the formatter.
			 *  @param t the template type.
			 */
			void addClassTemplateType(const string_type &t);


			/**
			 *  @brief function to write the formatter contents (the formatted
			 *  representation of the class) to an output stream.
			 *  @param o the output stream.
			 *  @return @e o the output stream.
			 */
			virtual std::ostream& writeToStream(std::ostream &o) const;


		protected:

			/** @brief the name of the class to format */
			string_type className;

			/** @brief the list of class members */
			member_list_type classMemberList;

			/** @brief the list of class template parameters */
			template_list_type classTemplateList;

			/** @brief the output page column width */
			static size_type columnWidth;

			/** @brief the current output column */
			static size_type currentColumn;

			/** @brief the current tab width */
			static size_type tabWidth;

			/** @brief the current indent level */
			static size_type indentLevel;

		};


		/** @brief overloaded operator<< to allow easy writing of formatter objects to
		 *  streams.
		 *  @param o the output stream.
		 *  @param f the formatter.
		 *  @return @e o the output stream.
		 */
		std::ostream& operator<<(std::ostream &o, const ClassFormatter &f);

	} // Foundation

} // TF

#include "tfclassformatter.cxx"

#endif // TFCLASSFORMATTER_HPP
