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

#include "tfdata.hpp"

#define NEEDS_CSTRING
#include "tfheaders.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        Data::Chunk::Chunk(const char * c, size_type length) : m_length{length}
        {
            m_buffer = pointer_type(new char[length], std::default_delete<char[]>());
            std::memcpy(m_buffer.get(), c, length * sizeof(char));
        }

        Data::Chunk::Chunk(const Chunk & c) : m_length{c.m_length}
        {
            if (m_length > 0)
            {
                m_buffer = pointer_type(new char[m_length], std::default_delete<char[]>());
                std::memcpy(m_buffer.get(), c.m_buffer.get(), m_length * sizeof(char));
            }
        }

        Data::Chunk::Chunk(Chunk && c)
        {
            m_length = c.m_length;
            m_buffer = std::move(c.m_buffer);
            c.m_length = 0;
        }

        Data::Chunk & Data::Chunk::operator=(const Chunk & c)
        {
            if (this != &c)
            {
                m_length = c.m_length;
                if (m_length > 0)
                {
                    m_buffer = pointer_type(new char[m_length], std::default_delete<char[]>());
                    std::memcpy(m_buffer.get(), c.m_buffer.get(), m_length * sizeof(char));
                }
                else
                {
                    m_buffer = nullptr;
                }
            }

            return *this;
        }

        Data::Chunk & Data::Chunk::operator=(Chunk && c)
        {
            if (this != &c)
            {
                m_length = c.m_length;
                m_buffer = std::move(c.m_buffer);
                c.m_length = 0;
            }

            return *this;
        }

        bool Data::Chunk::operator==(const Chunk & c) const
        {
            if (this != &c)
            {
                if (m_length != c.m_length)
                {
                    return false;
                }

                auto my_pointer = m_buffer.get();
                auto that_pointer = c.m_buffer.get();

                for (size_type i = 0; i < m_length; i++)
                {
                    if (*(my_pointer + i) != *(that_pointer + i))
                        return false;
                }
            }

            return true;
        }

        bool Data::Chunk::operator!=(const Chunk & c) const
        {
            if (*this == c)
                return false;
            return true;
        }

        std::ostream & Data::Chunk::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("Chunk");
                formatter->addClassMember("char *", "m_buffer", m_buffer.get(), m_length);
                formatter->addClassMember("Size_t", "m_length", m_length);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        Data::Data(const char * bytes, size_type length) : m_length{length}
        {
            m_chunk_list = std::make_unique<chunk_list_type>();
            m_chunk_list->emplace_back(chunk_type(bytes, length));
        }

        Data::Data(const Data & d)
        {
            auto d_length = d.length();
            if (d_length != 0)
            {
                m_chunk_list = std::make_unique<chunk_list_type>();
                m_chunk_list->emplace_back(chunk_type(d.bytes(), d_length));
                m_length = d_length;
            }
        }

        Data::Data(Data && d)
        {
            m_length = d.m_length;
            m_chunk_list = std::move(d.m_chunk_list);
            d.m_length = 0;
        }

        Data & Data::operator=(const Data & d)
        {
            if (this != &d)
            {
                auto d_length = d.length();
                if (d_length != 0)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                    m_chunk_list->emplace_back(d.bytes(), d_length);
                }
                else
                {
                    m_chunk_list = nullptr;
                }
                m_length = d_length;
            }

            return *this;
        }

        Data & Data::operator=(Data && d)
        {
            if (this != &d)
            {
                m_length = d.m_length;
                m_chunk_list = std::move(d.m_chunk_list);
                d.m_length = 0;
            }

            return *this;
        }

        bool Data::operator==(const Data & d) const
        {
            if (this != &d)
            {
                if (m_length != d.length())
                    return false;

                {
                    auto myBytes = this->bytes();
                    auto hisBytes = d.bytes();

                    for (size_type i = 0; i < m_length; i++)
                    {
                        if (*(myBytes + i) != *(hisBytes + i))
                            return false;
                    }
                }
            }

            return true;
        }

        bool Data::operator!=(const Data & d) const
        {
            if (*this == d)
                return false;
            return true;
        }

        Data::byte_type Data::operator[](size_type i) const
        {
            if (i > m_length)
            {
                throw std::invalid_argument("request exceeds number of elements.");
            }
            const char * theBytes = this->bytes();
            return static_cast<byte_type>(theBytes[i]);
        }

        Data::size_type Data::length() const
        {
            return m_length;
        }

        const char * Data::bytes() const
        {
            if (m_length == 0 || m_chunk_list->size() == 0)
            {
                return nullptr;
            }

            if (m_chunk_list->size() == 1)
            {
                auto & chunk = m_chunk_list->front();
                return chunk.bytes();
            }

            // We have more than one chunk in the list, so coalesce the list
            // into one big chunk.

            // TODO: Make the function non-const
            auto buffer = std::unique_ptr<char[]>(new char[m_length]);
            auto tmp = buffer.get();

            Data & other = const_cast<Data &>(*this);

            for (auto & chunk : *m_chunk_list)
            {
                std::memcpy(reinterpret_cast<void *>(tmp), reinterpret_cast<void *>(const_cast<char *>(chunk.bytes())),
                            chunk.length() * sizeof(char));
                tmp += chunk.length() * sizeof(char);
            }

            other.m_chunk_list->clear();

            other.m_chunk_list->emplace_back(chunk_type(buffer.get(), m_length));

            auto & b = other.m_chunk_list->back();

            return b.bytes();
        }

        void Data::append(const char * bytes, size_type length)
        {
            if (bytes != nullptr && length != 0)
            {
                if (m_chunk_list == nullptr)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                }
                m_chunk_list->emplace_back(chunk_type(bytes, length));
                m_length += length;
            }
        }

        void Data::append(const Data & d)
        {
            if (d.length() > 0)
            {
                if (m_chunk_list == nullptr)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                }
                m_chunk_list->emplace_back(chunk_type(d.bytes(), d.length()));
                m_length += d.length();
            }
        }

        void Data::append(Data && d)
        {
            if (d.length() > 0)
            {
                if (m_chunk_list == nullptr)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                }

                for (auto & chunk : *d.m_chunk_list)
                {
                    m_chunk_list->emplace_back(chunk);
                }
                m_length += d.length();
                d.m_length = 0;
            }
        }

        void Data::prepend(const char * bytes, size_type length)
        {
            if (bytes != nullptr && length > 0)
            {
                if (m_chunk_list == nullptr)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                }
                m_chunk_list->emplace_front(chunk_type(bytes, length));
                m_length += length;
            }
        }

        void Data::prepend(const Data & d)
        {
            if (d.length() > 0)
            {
                if (m_chunk_list == nullptr)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                }
                m_chunk_list->emplace_front(d.bytes(), d.length());
                m_length += d.length();
            }
        }

        void Data::prepend(Data && d)
        {
            if (d.length() > 0)
            {
                if (m_chunk_list == nullptr)
                {
                    m_chunk_list = std::make_unique<chunk_list_type>();
                }

                for (auto riter = d.m_chunk_list->rbegin(); riter != d.m_chunk_list->rend(); riter++)
                {
                    m_chunk_list->emplace_front(*riter);
                    m_length += riter->length();
                }

                d.m_chunk_list->clear();
                d.m_chunk_list = nullptr;
                d.m_length = 0;
            }
        }

        Data Data::subdataWithRange(const range_type & r) const
        {
            // TODO: The current implementation of this method is not ideal.   It involves a copy
            // of the data to get the bytes all into one contiguous array.   That step is
            // expensive.   A better way would be to copy the bytes into the subData object
            // by walking the chunks.
            Data subData;
            Data copyOfData;

            if (r.position >= m_length || (r.position + (r.length - 1)) >= m_length)
                throw std::range_error("subdataWithRange given range of data that lies outside of the data");

            copyOfData = *this;
            auto bytes = copyOfData.bytes();

            subData.append(bytes + r.position, r.length);
            return subData;
        }

        std::ostream & Data::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("Data");
                formatter->addClassMember("chunk_list_type", "m_chunk_list", m_chunk_list.get());
                formatter->addClassMember<Size_t>("m_length", m_length);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        std::ostream & operator<<(std::ostream & o, const Data::Chunk & c)
        {
            return c.description(o);
        }

        std::ostream & operator<<(std::ostream & o, const Data & d)
        {
            return d.description(o);
        }

    } // namespace Foundation

} // namespace TF
