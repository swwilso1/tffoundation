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

#ifndef TFTHREADSAFE_HXX
#define TFTHREADSAFE_HXX

#define NEEDS_MUTEX
#define NEEDS_MEMORY
#define NEEDS_FUNCTIONAL
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF::Foundation
{

    template<typename DataStructure>
    class ThreadSafe : public AllocatorInterface
    {
    public:
        using data_structure = DataStructure;
        using data_structure_ptr = std::unique_ptr<data_structure>;

        ThreadSafe()
        {
            m_data_structure = std::make_unique<data_structure>();
        }

        ThreadSafe(ThreadSafe & ts)
        {
            lock_type lock(ts.m_mutex);
            m_data_structure = std::make_unique<data_structure>();
            if (ts.m_data_structure)
            {
                *m_data_structure = *ts.m_data_structure;
            }
        }

        ThreadSafe(ThreadSafe && ts)
        {
            lock_type lock(ts.m_mutex);
            m_data_structure = std::move(ts.m_data_structure);
        }

        virtual ~ThreadSafe() {}

        ThreadSafe & operator=(ThreadSafe & ts)
        {
            if (this == &ts)
            {
                return *this;
            }

            lock_type my_lock(m_mutex);
            lock_type other_lock(ts.m_mutex);

            if (ts.m_data_structure)
            {
                *m_data_structure = *ts.m_data_structure;
            }

            return *this;
        }

        ThreadSafe & operator=(ThreadSafe && ts)
        {
            lock_type my_lock(m_mutex);
            lock_type other_lock(ts.m_mutex);
            m_data_structure = std::move(ts.m_data_structure);
        }

    protected:
        using lock_type = std::lock_guard<std::mutex>;
        std::mutex m_mutex{};
        data_structure_ptr m_data_structure{};
    };

} // namespace TF::Foundation

#endif // TFTHREADSAFE_HXX
