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

#ifndef TFUNIXPIPE_HPP
#define TFUNIXPIPE_HPP

#include <unistd.h>
#include "tfpipebase.hxx"

namespace TF::Foundation
{
    using Pipe = PipeBase<int>;

    template<>
    PipeBase<int>::PipeBase();

    template<>
    inline PipeBase<int>::~PipeBase()
    {
        close(m_handles[0]);
        close(m_handles[1]);
    }

    template<>
    PipeBase<int>::file_handle_type PipeBase<int>::file_handle_for_reading() const;

    template<>
    PipeBase<int>::file_handle_type PipeBase<int>::file_handle_for_writing() const;

    template<>
    void PipeBase<int>::close_for_reading();

    template<>
    void PipeBase<int>::close_for_writing();

} // namespace TF::Foundation

#endif // TFUNIXPIPE_HPP
