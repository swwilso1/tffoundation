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

#ifndef TFTHREADSAFEUNORDEREDMAP_HXX
#define TFTHREADSAFEUNORDEREDMAP_HXX

#define NEEDS_OSTREAM
#define NEEDS_MUTEX
#define NEEDS_UNORDERED_MAP
#define NEEDS_MEMORY
#define NEEDS_UTILITY
#define NEEDS_FUNCTIONAL
#define NEEDS_VECTOR
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfunordered_map.hxx"
#include "tfmutex.hpp"

namespace TF::Foundation
{

    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>,
             typename Allocator = std::allocator<std::pair<const Key, T>>>
    class ThreadSafeUnorderedMap : public AllocatorInterface
    {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = Size_t;
        using difference_type = std::ptrdiff_t;
        using hasher = Hash;
        using key_equal = KeyEqual;
        using allocator_type = Allocator;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using key_list_type = std::vector<key_type>;

        ThreadSafeUnorderedMap() : m_map{}, m_mutex{}
        {
            m_map = map_pointer_type(new map_type{});
        }

        ThreadSafeUnorderedMap(ThreadSafeUnorderedMap & m) : m_map{}, m_mutex{}
        {
            lock_guard_type guard{m.m_mutex};
            m_map = map_pointer_type(new map_type{});
            if (m.m_map)
            {
                *m_map = *m.m_map;
            }
        }

        ThreadSafeUnorderedMap(ThreadSafeUnorderedMap && m) : m_map{}, m_mutex{}
        {
            lock_guard_type guard{m.m_mutex};
            m_map = std::move(m.m_map);
        }

        ThreadSafeUnorderedMap & operator=(ThreadSafeUnorderedMap & m)
        {
            if (this == &m)
            {
                return *this;
            }

            lock_guard_type my_guard{m_mutex};
            lock_guard_type other_guard{m.m_mutex};

            if (m.m_map)
            {
                *m_map = *m.m_map;
            }

            return *this;
        }

        ThreadSafeUnorderedMap & operator=(ThreadSafeUnorderedMap && m)
        {
            if (this == &m)
            {
                return this;
            }

            lock_guard_type my_guard{m_mutex};
            lock_guard_type other_guard{m.m_mutex};
            m_map = std::move(m.m_map);
        }

        [[nodiscard]] bool empty() noexcept
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->empty();
        }

        [[nodiscard]] size_type size() noexcept
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->size();
        }

        [[nodiscard]] size_type max_size() noexcept
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->max_size();
        }

        void clear() noexcept
        {
            lock_guard_type guard{m_mutex};
            m_map->clear();
        }

        void insert(const value_type & value)
        {
            lock_guard_type guard{m_mutex};
            m_map->insert(value);
        }

        void insert(value_type && value)
        {
            lock_guard_type guard{m_mutex};
            m_map->insert(value);
        }

        [[nodiscard]] size_type erase(const key_type & key)
        {
            lock_guard_type guard{m_mutex};
            return m_map->erase(key);
        }

        void swap(ThreadSafeUnorderedMap & m)
        {
            if (this == &m)
            {
                return;
            }

            lock_guard_type my_guard{m_mutex};
            lock_guard_type other_guard{m.m_mutex};
            m_map->swap(*m.m_map);
        }

        [[nodiscard]] mapped_type & at(const key_type & key)
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->at(key);
        }

        [[nodiscard]] mapped_type & operator[](const key_type & key)
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->operator[](key);
        }

        [[nodiscard]] mapped_type & operator[](key_type && key)
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->operator[](key);
        }

        [[nodiscard]] size_type count(const key_type & key)
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->count(key);
        }

        [[nodiscard]] float load_factor()
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->load_factor();
        }

        [[nodiscard]] float max_load_factor()
        {
            lock_guard_type my_guard{m_mutex};
            return m_map->max_load_factor();
        }

        void max_float_factor(float ml)
        {
            lock_guard_type my_guard{m_mutex};
            m_map->max_float_factor(ml);
        }

        void rehash(size_type count)
        {
            lock_guard_type my_guard{m_mutex};
            m_map->rehash(count);
        }

        void reserve(size_type count)
        {
            lock_guard_type my_guard{m_mutex};
            m_map->reserve(count);
        }

        [[nodiscard]] hasher hash_function()
        {
            lock_guard_type my_guard{m_mutex};
            m_map->hash_function();
        }

        [[nodiscard]] key_equal key_eq()
        {
            lock_guard_type my_guard{m_mutex};
            m_map->key_eq();
        }

        [[nodiscard]] bool contains(const key_type & key)
        {
            lock_guard_type my_guard{m_mutex};
            // TODO: With C++20, change this to m_map.contains().
            return m_map->count(key) > 0;
        }

        [[nodiscard]] key_list_type keys()
        {
            lock_guard_type my_guard{m_mutex};
            key_list_type key_list;
            for (auto & entry : *m_map)
            {
                key_list.emplace_back(entry.first);
            }
            return key_list;
        }

    private:
        using map_type = std::unordered_map<key_type, mapped_type, hasher, key_equal, allocator_type>;
        using map_pointer_type = std::unique_ptr<map_type>;
        using mutex_type = std::mutex;
        using lock_guard_type = std::lock_guard<mutex_type>;

        map_pointer_type m_map;
        mutex_type m_mutex;
    };

} // namespace TF::Foundation

#endif // TFTHREADSAFEUNORDEREDMAP_HXX
