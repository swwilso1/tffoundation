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

#include <unistd.h>
#include "tfunixpipe.hpp"

namespace TF::Foundation
{

    template<>
    PipeBase<int>::PipeBase()
    {
        auto presult = pipe(m_handles);
        if (presult < 0)
        {
            throw std::system_error{errno, std::system_category(), "pipe open failed"};
        }
    }

    template<>
    PipeBase<int>::file_handle_type & PipeBase<int>::file_handle_for_reading()
    {
        if (m_read_handle.fileDescriptor() != m_handles[0])
        {
            // Do not auto-close the file handle.  The descriptors will get closed in the
            // destructor.
            m_read_handle = file_handle_type::fileHandleForReadingFromDescriptor(m_handles[0]);
        }
        return m_read_handle;
    }

    template<>
    PipeBase<int>::file_handle_type & PipeBase<int>::file_handle_for_writing()
    {
        if (m_write_handle.fileDescriptor() != m_handles[1])
        {
            // Do not auto-close the file handle.  The descriptors will get closed in the
            // destructor.
            m_write_handle = file_handle_type::fileHandleForWritingFromDescriptor(m_handles[1]);
        }
        return m_write_handle;
    }

    template<>
    void PipeBase<int>::close_for_reading()
    {
        close(m_handles[0]);
    }

    template<>
    void PipeBase<int>::close_for_writing()
    {
        close(m_handles[1]);
    }

} // namespace TF::Foundation
