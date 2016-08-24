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

#define NEEDS_SSTREAM
#include "tfheaders.hpp"
#include "tfclassformatter.hpp"
#include "tftab.hpp"

namespace TF
{

	namespace Foundation
	{

		ClassFormatter::size_type ClassFormatter::columnWidth = 80;

		ClassFormatter::size_type ClassFormatter::currentColumn = 0;

		ClassFormatter::size_type ClassFormatter::tabWidth = 4;

		ClassFormatter::size_type ClassFormatter::indentLevel = 0;

		ClassFormatter::~ClassFormatter()
		{
			for(auto member : classMemberList)
				delete member;
		}


		void ClassFormatter::setColumnWidth(size_type w)
		{
			columnWidth = w;
		}


		ClassFormatter::size_type ClassFormatter::getColumnWidth(void)
		{
			return columnWidth;
		}


		void ClassFormatter::setTabWidth(size_type w)
		{
			tabWidth = w;
		}


		ClassFormatter::size_type ClassFormatter::getTabWidth(void)
		{
			return tabWidth;
		}


		void ClassFormatter::setClassName(const string_type &name)
		{
			className = name;
		}


		void ClassFormatter::addClassMember(const string_type &type, const string_type &name,
				const string_type &value)
		{
			TemplateClassMemberWithType<string_type> *newMember =
				new TemplateClassMemberWithType<string_type>(type, name, value);
			classMemberList.push_back(newMember);
		}


		 void ClassFormatter::addClassMember(size_type i, const string_type &type,
		 	const string_type &value)
		 {
		 	std::stringstream converter;
		 	converter << i;
		 	TemplateClassMemberWithType<string_type> *newMember =
		 		new TemplateClassMemberWithType<string_type>(type, converter.str(), value);
             classMemberList.push_back(newMember);
		 }


		void ClassFormatter::addClassTemplateType(const string_type &t)
		{
			classTemplateList.push_back(t);
		}


		std::ostream& ClassFormatter::writeToStream(std::ostream &o) const
		{
			indentLevel++;
			if(classTemplateList.size() > 0)
			{
				o << Tab(indentLevel,tabWidth) << className << " template options<";
				size_type i = 0;
				size_type max = classTemplateList.size();
				for(auto templateValue : classTemplateList)
				{
					o << templateValue;
					if(i++ < (max - 1))
						o << ",";
				}
				o << ">" << std::endl;
			}
			else
				o << Tab(indentLevel,tabWidth) << className << std::endl;
			for(auto member : classMemberList)
			{
				o << std::endl << Tab(indentLevel, tabWidth);
				o << member->type() << " " << member->name() << " ";
				o << member->value() << std::endl;
			}
			indentLevel--;
			return o;
		}


		std::ostream& operator<<(std::ostream &o, const ClassFormatter &f)
		{
			return f.writeToStream(o);
		}

	} // Foundation

} // TF

