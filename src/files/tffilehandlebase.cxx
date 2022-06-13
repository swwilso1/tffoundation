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

#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleForReadingAtPath(
            const string_type & path, bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = path;
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleForWritingAtPath(
            const string_type & path, bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = path;
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleForReadingAndWritingAtPath(
            const string_type & path, bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = path;
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleForAppendingAtPath(
            const string_type & path, bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = path;
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleWithStandardInput(
            bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = "stdin";
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleWithStandardOutput(
            bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = "stdout";
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> FileHandleBase<Handle, Descriptor>::fileHandleWithStandardError(
            bool auto_close)
        {
            FileHandleBase h;
            h.m_fileName = "stderr";
            h.m_autoClose = auto_close;
            return h;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor>::FileHandleBase(bool auto_close) :
            m_handle{}, m_autoClose{auto_close}, m_fileName{}
        {}

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor>::FileHandleBase(const FileHandleBase & fh)
        {
            (void)fh;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor>::FileHandleBase(FileHandleBase && fh)
        {
            (void)fh;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor>::~FileHandleBase()
        {}

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> & FileHandleBase<Handle, Descriptor>::operator=(const FileHandleBase & fh)
        {
            (void)fh;
            return *this;
        }

        template<class Handle, class Descriptor>
        FileHandleBase<Handle, Descriptor> & FileHandleBase<Handle, Descriptor>::operator=(FileHandleBase && fh)
        {
            (void)fh;
            return *this;
        }

        template<class Handle, class Descriptor>
        typename FileHandleBase<Handle, Descriptor>::descriptor_type
        FileHandleBase<Handle, Descriptor>::fileDescriptor()
        {
            descriptor_type d;
            return d;
        }

        template<class Handle, class Descriptor>
        typename FileHandleBase<Handle, Descriptor>::data_type FileHandleBase<Handle, Descriptor>::readAvailableData()
        {
            data_type d;
            return d;
        }

        template<class Handle, class Descriptor>
        typename FileHandleBase<Handle, Descriptor>::data_type FileHandleBase<Handle, Descriptor>::readToEndOfFile()
        {
            data_type d;
            return d;
        }

        template<class Handle, class Descriptor>
        typename FileHandleBase<Handle, Descriptor>::data_type FileHandleBase<Handle, Descriptor>::readDataOfLength(
            size_type length)
        {
            (void)length;
            data_type d;
            return d;
        }

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::writeData(const data_type & d)
        {
            (void)d;
        }

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::flush()
        {
            return;
        }

        template<class Handle, class Descriptor>
        typename FileHandleBase<Handle, Descriptor>::size_type FileHandleBase<Handle, Descriptor>::offsetInFile()
        {
            return 0;
        }

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::seekToEndOfFile()
        {}

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::seekToFileOffset(size_type offset)
        {
            (void)offset;
        }

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::closeFile()
        {}

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::truncateFileAtOffset(size_type offset)
        {
            (void)offset;
        }

        template<class Handle, class Descriptor>
        void FileHandleBase<Handle, Descriptor>::setAutoClose(bool autoClose)
        {
            m_autoClose = autoClose;
        }

        template<class Handle, class Descriptor>
        std::ostream & FileHandleBase<Handle, Descriptor>::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("FileHandleBase");
                formatter->addClassTemplateType<Handle>();
                formatter->addClassTemplateType<Descriptor>();
                formatter->addClassMember<bool>("m_autoClose", m_autoClose);
                formatter->addClassMember<string_type>("m_fileName", m_fileName);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        template<class Handle, class Description>
        std::ostream & operator<<(std::ostream & o, const FileHandleBase<Handle, Description> & h)
        {
            return h.description(o);
        }

    } // namespace Foundation

} // namespace TF
