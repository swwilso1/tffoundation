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

#ifndef TFPIPEBASE_HXX
#define TFPIPEBASE_HXX

#define NEEDS_OSTREAM
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdata.hpp"
#include "tffilehandle.hpp"

namespace TF::Foundation
{

    template<class Handle>
    class PipeBase : public AllocatorInterface
    {
    public:
        using handle_type = Handle;

        using data_type = Data;

        using file_handle_type = FileHandle;

        PipeBase() {}

        ~PipeBase(){};

        file_handle_type file_handle_for_reading() const
        {
            return file_handle_type{};
        }

        file_handle_type file_handle_for_writing() const
        {
            return file_handle_type{};
        }

        void close_for_reading() {}

        void close_for_writing() {}

        handle_type read_handle() const
        {
            return m_handles[0];
        }

        handle_type write_handle() const
        {
            return m_handles[1];
        }

        std::ostream & description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("Pipe");
                formatter->addClassMember<handle_type>("m_handles", m_handles, 2);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

    private:
        handle_type m_handles[2]{};
    };

    template<class Handle>
    std::ostream & operator<<(std::ostream & o, const PipeBase<Handle> & p)
    {
        return p.description(o);
    }

} // namespace TF::Foundation

#endif // TFPIPEBASE_HXX
