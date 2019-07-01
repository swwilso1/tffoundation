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

#define NEEDS_SSTREAM
#define NEEDS_TYPEINFO
#include "tfheaders.hpp"

namespace TF
{

    namespace Foundation
    {

        template<typename T>
        void ClassFormatter::addClassMember(const string_type &name, const T &value)
        {
            TemplateClassMember<T> *newMember = new TemplateClassMember<T>(name, value);
            classMemberList.push_back(newMember);
        }


        template<typename T>
        void ClassFormatter::addClassMember(size_type i, const T &value)
        {
            std::stringstream converter;
            converter << i;
            TemplateClassMember<T> *newMember = new TemplateClassMember<T>(converter.str(), value);
            classMemberList.push_back(newMember);
        }


        template<typename T>
        void ClassFormatter::addClassMember(const string_type &type, const string_type &name, const T &value)
        {
            TemplateClassMemberWithType<T> *newMember = new TemplateClassMemberWithType<T>(type, name, value);
            classMemberList.push_back(newMember);
        }


        template<typename T>
        void ClassFormatter::addClassMember(const string_type &name, const T *value, const size_type &length)
        {
            TemplateArrayClassMember<T> *newMember = new TemplateArrayClassMember<T>(name, value, length);
            classMemberList.push_back(newMember);
        }


        template<typename T>
        void ClassFormatter::addClassMember(const string_type &type, const string_type &name, const T *value,
                                            const size_type &length)
        {
            TemplateArrayClassMemberWithType<T> *newMember =
                new TemplateArrayClassMemberWithType<T>(type, name, value, length);
            classMemberList.push_back(newMember);
        }


        template<typename T>
        void ClassFormatter::addClassTemplateType()
        {
            T theValue;
            string_type theType = typeid(theValue).name();
            classTemplateList.push_back(theType);
        }


    }    // namespace Foundation

}    // namespace TF
