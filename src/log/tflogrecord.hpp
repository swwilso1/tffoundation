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

#ifndef TFLOGRECORD_HPP
#define TFLOGRECORD_HPP

#define NEEDS_OPTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfunordered_map.hxx"
#include "tfstring.hpp"
#include "tfmultivalue.hpp"

namespace TF
{

    namespace Foundation
    {

        class LogRecord
        {
        public:
            using string_type = String;
            using key_type = string_type;
            using value_type = MultiValue;

            template<typename T>
            void addValueForKey(const key_type & k, const T & t)
            {
                m_map.insert(std::make_pair(k, t));
            }

            template<typename T>
            std::optional<T> getValueForKey(const key_type & key)
            {
                if (m_map.count(key) == 0)
                {
                    return std::optional<T>{};
                }

                auto value = m_map[key];
                return value.get<T>();
            }

            template<typename T>
            bool value_is_type_for_key(const key_type & key)
            {
                auto value = m_map[key];
                return value.is_type<T>();
            }

            bool contains(const key_type & key)
            {
                return m_map.count(key) > 0;
            }

        private:
            using map_type = std::unordered_map<key_type, value_type>;

            map_type m_map;
        };

    } // namespace Foundation

} // namespace TF

#endif // TFLOGRECORD_HPP
