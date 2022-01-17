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

#ifndef TFCLASSMEMBER_HXX
#define TFCLASSMEMBER_HXX

#define NEEDS_SSTREAM
#define NEEDS_STRING
#define NEEDS_OSTREAM
#define NEEDS_TYPEINFO
#include "tfheaders.hpp"
#include "tfallocator.hpp"
#include "tftypes.hpp"

// On macOS, using clang, the Clock::time_point and duration types
// must have an operator<< defined prior to their use in a template
// object.  That means for the Date formatter code used in the
// Date::description method to function correctly, the operator<<
// functions need declared prior to their use in the template.  This
// must happen at template declaration time, not template use time.
#include "tfchronostream.hxx"
// Included for operator<< for struct tm.
#include "tfdatetypes.hpp"

namespace TF
{

    namespace Foundation
    {

        struct ClassMember : public AllocatorInterface
        {
            /** @brief simple string type */
            using string_type = std::string;

            /** @brief simple size type */
            using size_type = Size_t;

            ClassMember() : AllocatorInterface()
            {
            }

            ClassMember(const string_type &t, const string_type &n) : theType(t), theName(n)
            {
            }

            virtual ~ClassMember()
            {
            }

            virtual string_type type()
            {
                return theType;
            }

            virtual string_type name()
            {
                return theName;
            }

            virtual string_type value()
            {
                return "";
            }

            virtual size_type length()
            {
                return 0;
            }

            virtual std::ostream &writeToStream(std::ostream &o) const;

        protected:
            string_type theType;
            string_type theName;
        };


        std::ostream &operator<<(std::ostream &o, const ClassMember &m);


        class VoidClassMember : public ClassMember
        {
        public:
            VoidClassMember() : ClassMember()
            {
                theValue = nullptr;
            }

            VoidClassMember(const string_type &n, void *value) : ClassMember("void *", n)
            {
                theValue = value;
            }

            string_type value() override
            {
                std::stringstream theConverter;
                theConverter << theValue;
                return theConverter.str();
            }


            std::ostream &writeToStream(std::ostream &o) const override;

        private:
            void *theValue;
        };


        template<typename T>
        struct TemplateClassMember : public ClassMember
        {
            /** @brief basic member value type */
            using value_type = T;

            TemplateClassMember()
            {
                value_type v;
                theType = typeid(v).name();
            }

            TemplateClassMember(const string_type &n, const value_type &v)
            {
                theType = typeid(v).name();
                theName = n;
                theValue = v;
            }


            string_type value() override
            {
                std::stringstream collector;
                collector << theValue;
                return collector.str();
            }


            std::ostream &writeToStream(std::ostream &o) const override
            {
                o << theType << " " << theName << " " << theValue;
                return o;
            }

        protected:
            value_type theValue;
        };


        template<typename T>
        struct TemplateClassMemberWithType : public ClassMember
        {
            /** @brief basic member value type */
            using value_type = T;

            TemplateClassMemberWithType() : ClassMember()
            {
                value_type v;
            }


            TemplateClassMemberWithType(const string_type &t, const string_type &n, const value_type &v)
                : ClassMember(t, n)
            {
                theValue = v;
            }


            string_type value() override
            {
                std::stringstream collector;
                collector << theValue;
                return collector.str();
            }


            std::ostream &writeToStream(std::ostream &o) const override
            {
                o << theType << " " << theName << " " << theValue;
                return o;
            }

        protected:
            value_type theValue;
        };


        template<typename T>
        struct TemplateArrayClassMember : public ClassMember
        {
            /** @brief basic member value type */
            using value_type = T;

            /** @brief simple size type */
            using size_type = Size_t;

            TemplateArrayClassMember()
            {
                value_type v;
                theType = typeid(v).name();
                theLength = 0;
                theValue = nullptr;
            }


            TemplateArrayClassMember(const string_type &n, const value_type *v, const size_type &l)
            {
                value_type vt;
                theType = typeid(vt).name();
                theName = n;
                theValue = new value_type[l];
                for(size_type i = 0; i < l; i++)
                {
                    theValue[i] = *(v + i);
                }

                theLength = l;
            }


            ~TemplateArrayClassMember()
            {
                if(theLength > 0 && theValue != nullptr)
                    delete[] theValue;
            }


            string_type value() override
            {
                std::stringstream collector;
                if(theLength > 0)
                {
                    collector << "{";
                    for(size_type i = 0; i < theLength; i++)
                    {
                        collector << theValue[i];
                        if(i < (theLength - 1))
                            collector << ",";
                    }
                    collector << "}";
                }
                return collector.str();
            }


            size_type length() override
            {
                return theLength;
            }


            std::ostream &writeToStream(std::ostream &o) const override
            {
                o << theType << " " << theName << "[" << theLength << "]";
                if(theLength > 0)
                {
                    o << " {";
                    for(size_type i = 0; i < theLength; i++)
                    {
                        o << theValue[i];
                        if(i < (theLength - 1))
                            o << ",";
                    }
                    o << "}";
                }
                return o;
            }

        protected:
            value_type *theValue;
            size_type theLength;
        };


        template<>
        TemplateArrayClassMember<unsigned char>::string_type TemplateArrayClassMember<unsigned char>::value();

        template<>
        std::ostream &TemplateArrayClassMember<unsigned char>::writeToStream(std::ostream &o) const;

        template<typename T>
        struct TemplateArrayClassMemberWithType : public ClassMember
        {
            /** @brief basic member value type */
            using value_type = T;

            /** @brief simple size type */
            using size_type = Size_t;

            TemplateArrayClassMemberWithType() : ClassMember()
            {
                value_type v;
                theLength = 0;
                theValue = nullptr;
            }


            TemplateArrayClassMemberWithType(const string_type &t, const string_type &n, const value_type *v,
                                             const size_type &l)
                : ClassMember(t, n)
            {
                theValue = new value_type[l];
                for(size_type i = 0; i < l; i++)
                {
                    theValue[i] = *(v + i);
                }

                theLength = l;
            }


            ~TemplateArrayClassMemberWithType()
            {
                if(theLength > 0 && theValue != nullptr)
                    delete[] theValue;
            }


            string_type value() override
            {
                std::stringstream collector;
                if(theLength > 0)
                {
                    collector << "{";
                    for(size_type i = 0; i < theLength; i++)
                    {
                        collector << theValue[i];
                        if(i < (theLength - 1))
                            collector << ",";
                    }
                    collector << "}";
                }
                return collector.str();
            }


            size_type length() override
            {
                return theLength;
            }

            std::ostream &writeToStream(std::ostream &o) const override
            {
                o << theType << " " << theName << "[" << theLength << "]";
                if(theLength > 0)
                {
                    o << " {";
                    for(size_type i = 0; i < theLength; i++)
                    {
                        o << theValue[i];
                        if(i < (theLength - 1))
                            o << ",";
                    }
                    o << "}";
                }
                return o;
            }

        protected:
            value_type *theValue;
            size_type theLength;
        };

        template<>
        TemplateArrayClassMemberWithType<unsigned char>::string_type
            TemplateArrayClassMemberWithType<unsigned char>::value();

        template<>
        std::ostream &TemplateArrayClassMemberWithType<unsigned char>::writeToStream(std::ostream &o) const;

    }    // namespace Foundation

}    // namespace TF

#endif    // TFCLASSMEMBER_HXX
