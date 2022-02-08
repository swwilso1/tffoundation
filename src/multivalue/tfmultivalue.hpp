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

#ifndef TFMULTIVALUE_HPP
#define TFMULTIVALUE_HPP

#define NEEDS_MEMORY
#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tftypetraits.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        namespace values
        {
            class IVal
            {
            public:
                virtual ~IVal() {}
                virtual IVal * copy() const
                {
                    return nullptr;
                }
                virtual bool operator==(const IVal & v) const
                {
                    (void)v;
                    return false;
                }
            };

            template<typename T>
            class AVal : public IVal
            {
                static_assert(Traits::has_operator_equal<T>::value, "Type T must support comparison");

            public:
                using value_type = T;

                AVal() : value{} {}

                IVal * copy() const override
                {
                    AVal * new_val = new AVal;
                    new_val->value = value;
                    return new_val;
                }

                bool operator==(const IVal & v) const override
                {
                    const AVal * other_val = dynamic_cast<const AVal *>(&v);
                    if (other_val)
                    {
                        return value == other_val->value;
                    }
                    return false;
                }

                value_type value;
            };
        } // namespace values

        class MultiValue
        {
        public:
            MultiValue() : m_ival{} {}

            template<typename T>
            MultiValue(const T & t) : m_ival{}
            {
                auto thing = std::make_unique<values::AVal<T>>();
                thing->value = t;
                m_ival = std::move(thing);
            }

            template<typename T>
            MultiValue(T t[]) : m_ival{}
            {
                auto thing = std::make_unique<values::AVal<T *>>();
                thing->value = t;
                m_ival = std::move(thing);
            }

            MultiValue(const MultiValue & v)
            {
                m_ival = ival_ptr(v.m_ival->copy());
            }

            MultiValue(MultiValue && v)
            {
                m_ival = std::move(v.m_ival);
            }

            MultiValue & operator=(const MultiValue & v)
            {
                if (this == &v)
                {
                    return *this;
                }

                m_ival = ival_ptr(v.m_ival->copy());
                return *this;
            }

            MultiValue & operator=(MultiValue && v)
            {
                if (this == &v)
                {
                    return *this;
                }

                m_ival = std::move(v.m_ival);
                return *this;
            }

            template<typename T>
            MultiValue & operator=(const T & t)
            {
                auto thing = std::make_unique<values::AVal<T>>();
                thing->value = t;
                m_ival = std::move(thing);
                return *this;
            }

            bool operator==(const MultiValue & v) const
            {
                if (this == &v)
                {
                    return true;
                }

                if (! m_ival || ! v.m_ival)
                {
                    return false;
                }

                return *m_ival == *v.m_ival;
            }

            bool operator!=(const MultiValue & v) const
            {
                return ! (*this == v);
            }

            template<typename T>
            bool is_type() const
            {
                values::AVal<T> * thing = dynamic_cast<values::AVal<T> *>(m_ival.get());
                return thing != nullptr;
            }

            bool is_integer() const
            {
                return is_type<int8_t>() || is_type<uint8_t>() || is_type<int16_t>() || is_type<uint16_t>() ||
                       is_type<int32_t>() || is_type<uint32_t>() || is_type<int64_t>() || is_type<uint64_t>();
            }

            bool is_real() const
            {
                return is_type<float>() || is_type<double>();
            }

            bool is_numeric() const
            {
                return is_real() || is_integer();
            }

            bool is_bool() const
            {
                return is_type<bool>();
            }

            bool is_string() const
            {
                return is_type<String>();
            }

            template<typename T>
            std::optional<T> get() const
            {
                values::AVal<T> * thing = dynamic_cast<values::AVal<T> *>(m_ival.get());
                if (thing)
                {
                    return std::optional<T>{thing->value};
                }
                return std::optional<T>{};
            }

        public:
            using ival_ptr = std::unique_ptr<values::IVal>;

            ival_ptr m_ival;
        };

    } // namespace Foundation

} // namespace TF

#endif // TFMULTIVALUE_HPP
