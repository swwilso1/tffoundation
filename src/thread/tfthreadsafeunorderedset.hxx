/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#ifndef TFTHREADSAFEUNORDEREDSET_HXX
#define TFTHREADSAFEUNORDEREDSET_HXX

#define NEEDS_UNORDERED_SET
#define NEEDS_MUTEX
#define NEEDS_MEMORY
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfthreadsafe.hxx"

namespace TF::Foundation
{

    template<typename T>
    class ThreadSafeUnorderedSet : public ThreadSafe<std::unordered_set<T>>
    {
        using basic_set_type = std::unordered_set<T>;
        using thread_safe = ThreadSafe<basic_set_type>;

    public:
        using set_type = std::unordered_set<T>;

        using key_type = typename basic_set_type::key_type;
        using value_type = typename basic_set_type::value_type;
        using size_type = typename basic_set_type::size_type;
        using difference_type = typename basic_set_type::difference_type;
        using reference = typename basic_set_type::reference;
        using const_reference = typename basic_set_type::const_reference;
        using pointer = typename basic_set_type::pointer;
        using const_pointer = typename basic_set_type::const_pointer;
        using node_type = typename basic_set_type::node_type;
        using insert_return_type = typename basic_set_type::insert_return_type;

        ThreadSafeUnorderedSet() : ThreadSafe<basic_set_type>() {}

        ThreadSafeUnorderedSet(ThreadSafeUnorderedSet & s) : ThreadSafe<basic_set_type>(s) {}

        ThreadSafeUnorderedSet(ThreadSafeUnorderedSet && s) : ThreadSafe<basic_set_type>(s) {}

        [[nodiscard]] bool empty()
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->empty();
        }

        [[nodiscard]] size_type size()
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->size();
        }

        [[nodiscard]] size_type max_size()
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->max_size();
        }

        void clear()
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            thread_safe::m_data_structure->clear();
        }

        void insert(const value_type & v)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            thread_safe::m_data_structure->insert(v);
        }

        void insert(value_type && v)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            thread_safe::m_data_structure->insert(v);
        }

        size_type erase(const key_type & k)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->erase(k);
        }

        size_type count(const key_type & k)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->count(k);
        }

        template<typename K>
        size_type count(const K & k)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->count(k);
        }

        bool contains(const key_type & k)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->contains(k);
        }

        template<typename K>
        bool contains(const K & k)
        {
            typename thread_safe::lock_type lock(thread_safe::m_mutex);
            return thread_safe::m_data_structure->contains(k);
        }
    };

} // namespace TF::Foundation

#endif // TFTHREADSAFEUNORDEREDSET_HXX
