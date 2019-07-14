/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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
#define NEEDS_STRING_H
#include "tfheaders.hpp"
#include "tfunixfilehandle.hpp"
#include "tffilemanager.hpp"

namespace TF
{

    namespace Foundation
    {
        template<>
        FileHandleBase<FILE *, int>::FileHandleBase()
        {
            m_handle = nullptr;
            m_autoClose = false;
        }


        template<>
        FileHandleBase<FILE *, int>::~FileHandleBase()
        {
            if(m_autoClose && m_handle != nullptr)
            {
                fclose(m_handle);
                m_handle = nullptr;
            }
        }


        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingAtPath(const string_type &path)
        {
            FileManager fm;
            FileHandleBase<FILE *, int> fh;
            if(!fm.fileExistsAtPath(path))
            {
                string_type msg("No such file or directory");
                msg += " '" + path + "'";
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            auto pathCStr = path.cStr();
            fh.m_handle = fopen(pathCStr.get(), "r");
            if(fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "': " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;

            return fh;
        }


        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForWritingAtPath(const string_type &path)
        {
            FileHandleBase fh;
            auto pathCStr = path.cStr();

            fh.m_handle = fopen(pathCStr.get(), "w");
            if(fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "' for writing: " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;

            return fh;
        }


        template<>
        FileHandleBase<FILE *, int>
            FileHandleBase<FILE *, int>::fileHandleForReadingAndWritingAtPath(const string_type &path)
        {
            FileHandleBase fh;
            auto pathCStr = path.cStr();

            fh.m_handle = fopen(pathCStr.get(), "r+");
            if(fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "' for reading and writing: " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;

            return fh;
        }


        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForAppendingAtPath(const string_type &path)
        {
            FileHandleBase fh;
            auto pathCstr = path.cStr();

            fh.m_handle = fopen(pathCstr.get(), "a");
            if(fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "' for reading and writing: " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;
            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardInput()
        {
            FileHandleBase fh;
            fh.m_handle = stdin;
            fh.m_fileName = "stdin";
            return fh;
        }


        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardOutput()
        {
            FileHandleBase fh;
            fh.m_handle = stdout;
            fh.m_fileName = "stdout";
            return fh;
        }


        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardError()
        {
            FileHandleBase fh;
            fh.m_handle = stderr;
            fh.m_fileName = "stderr";
            return fh;
        }


        template<>
        FileHandleBase<FILE *, int>::descriptor_type FileHandleBase<FILE *, int>::fileDescriptor()
        {
            if(m_handle != nullptr)
                return fileno(m_handle);
            return -1;
        }


#define kReadBufferSize 1024

        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readAvailableData()
        {
            data_type d;
            char buffer[kReadBufferSize];

            if(m_handle == nullptr)
                return d;

            size_t amountRead = fread(buffer, sizeof(char), kReadBufferSize, m_handle);
            if(amountRead != kReadBufferSize)
            {
                if(ferror(m_handle))
                {
                    string_type msg("Error reading stream from ");
                    msg += m_fileName;
                    auto msgCStr = msg.cStr();
                    throw std::runtime_error(msgCStr.get());
                }
            }
            d.append(buffer, kReadBufferSize);

            return d;
        }


        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readToEndOfFile()
        {
            data_type d;
            char buffer[kReadBufferSize];

            if(m_handle == nullptr)
                return d;

            while(!feof(m_handle))
            {
                size_t amountRead = fread(buffer, sizeof(char), kReadBufferSize, m_handle);

                if(ferror(m_handle))
                {
                    string_type msg("Error reading stream from ");
                    msg += m_fileName;
                    auto msgCStr = msg.cStr();
                    throw std::runtime_error(msgCStr.get());
                }
                d.append(buffer, amountRead);
            }

            return d;
        }


        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readDataOfLength(size_type length)
        {
            data_type d;
            char *buffer = new char[length];
            char *tmp = buffer;
            size_t bytesToRead = length;
            size_t bytesRead = 0;

            if(m_handle == nullptr)
                return d;

            while(bytesToRead > 0)
            {
                bytesRead = fread(tmp, sizeof(char), bytesToRead, m_handle);
                if(ferror(m_handle))
                {
                    string_type msg("Error reading stream from ");
                    msg += m_fileName;
                    auto msgCStr = msg.cStr();
                    throw std::runtime_error(msgCStr.get());
                }

                d.append(tmp, bytesRead);
                tmp += bytesRead;
                bytesToRead -= bytesRead;

                if(feof(m_handle))
                    return d;
            }

            return d;
        }


        template<>
        void FileHandleBase<FILE *, int>::writeData(const data_type &d)
        {
            const char *bytes = d.bytes();
            auto bytesToWrite = d.length();
            const char *tmp = bytes;
            size_t bytesWrote = 0;

            if(m_handle == nullptr)
                return;

            while(bytesToWrite > 0)
            {
                bytesWrote = fwrite(tmp, sizeof(char), bytesToWrite, m_handle);
                if(ferror(m_handle))
                {
                    string_type msg("Error writing to stream for file ");
                    msg += m_fileName;
                    auto msgCStr = msg.cStr();
                    throw std::runtime_error(msgCStr.get());
                }

                tmp += bytesWrote;
                bytesToWrite -= bytesWrote;
            }
        }


        template<>
        FileHandleBase<FILE *, int>::size_type FileHandleBase<FILE *, int>::offsetInFile()
        {
            if(m_handle == nullptr)
                return 0;
            return static_cast<size_type>(ftell(m_handle));
        }


        template<>
        void FileHandleBase<FILE *, int>::seekToEndOfFile()
        {
            if(m_handle == nullptr)
                return;
            fseek(m_handle, 0, SEEK_END);
        }


        template<>
        void FileHandleBase<FILE *, int>::seekToFileOffset(size_type offset)
        {
            if(m_handle == nullptr)
                return;
            fseek(m_handle, static_cast<long>(offset), SEEK_CUR);
        }


        template<>
        void FileHandleBase<FILE *, int>::closeFile()
        {
            if(m_handle != nullptr)
            {
                fclose(m_handle);
                m_handle = nullptr;
            }
        }


        template<>
        void FileHandleBase<FILE *, int>::truncateFileAtOffset(size_type offset)
        {
            if(m_handle == nullptr)
                return;

            auto descriptor = fileDescriptor();
            auto apiResult = ftruncate(descriptor, static_cast<off_t>(offset));
            if(apiResult != 0)
            {
                string_type msg("Unable to truncate file ");
                msg += m_fileName + ": ";
                msg += strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }
        }


        template<>
        std::ostream &FileHandleBase<FILE *, int>::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("FileHandle");
                formatter->addClassTemplateType<FILE *>();
                formatter->addClassTemplateType<int>();
                formatter->addClassMember<FILE *>("m_handle", m_handle);
                formatter->addClassMember<bool>("m_autoClose", m_autoClose);
                formatter->addClassMember<string_type>("m_fileName", m_fileName);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

    }    // namespace Foundation

}    // namespace TF
