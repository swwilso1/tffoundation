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

#include <unistd.h>
#define NEEDS_STRING_H
#define NEEDS_SYSTEM_ERROR
#include "tfheaders.hpp"
#include "tfunixfilehandle.hpp"
#include "tffilemanager.hpp"
#include "tfstringencoder.hpp"

namespace TF
{

    namespace Foundation
    {
        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(bool auto_close) :
            m_handle{nullptr}, m_autoClose{auto_close}, m_fileName{}
        {}

        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(handle_type h, bool auto_close) :
            m_handle{h}, m_autoClose{auto_close}, m_fileName{}
        {}

        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(const FileHandleBase & fh) :
            m_handle{fh.m_handle}, m_autoClose{fh.m_autoClose}, m_fileName{fh.m_fileName}
        {}

        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(FileHandleBase && fh) :
            m_handle{fh.m_handle}, m_autoClose{fh.m_autoClose}, m_fileName{fh.m_fileName}
        {
            fh.m_handle = nullptr;
            fh.m_autoClose = false;
            fh.m_fileName = "";
        }

        template<>
        FileHandleBase<FILE *, int>::~FileHandleBase()
        {
            if (m_autoClose && m_handle != nullptr)
            {
                fclose(m_handle);
                m_handle = nullptr;
            }
        }

        template<>
        FileHandleBase<FILE *, int> & FileHandleBase<FILE *, int>::operator=(const FileHandleBase & fh)
        {
            if (this == &fh)
            {
                return *this;
            }

            if (m_autoClose && m_handle != nullptr)
            {
                fclose(m_handle);
                m_handle = nullptr;
            }

            m_handle = fh.m_handle;
            m_fileName = fh.m_fileName;
            m_autoClose = fh.m_autoClose;

            return *this;
        }

        template<>
        FileHandleBase<FILE *, int> & FileHandleBase<FILE *, int>::operator=(FileHandleBase && fh)
        {
            if (this == &fh)
            {
                return *this;
            }

            if (m_autoClose && m_handle != nullptr)
            {
                fclose(m_handle);
                m_handle = nullptr;
            }

            m_handle = fh.m_handle;
            m_fileName = fh.m_fileName;
            m_autoClose = fh.m_autoClose;

            fh.m_handle = nullptr;
            fh.m_fileName = "";
            fh.m_autoClose = false;

            return *this;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingAtPath(const string_type & path,
                                                                                            bool auto_close)
        {
            FileManager fm;
            FileHandleBase<FILE *, int> fh;
            if (! fm.fileExistsAtPath(path))
            {
                string_type msg("No such file or directory");
                msg += " '" + path + "'";
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            auto pathCStr = path.cStr();
            fh.m_handle = fopen(pathCStr.get(), "r");
            if (fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "': " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;
            fh.setAutoClose(auto_close);

            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForWritingAtPath(const string_type & path,
                                                                                            bool auto_close)
        {
            FileHandleBase fh;
            auto pathCStr = path.cStr();

            fh.m_handle = fopen(pathCStr.get(), "w");
            if (fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "' for writing: " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;
            fh.setAutoClose(auto_close);

            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingAndWritingAtPath(
            const string_type & path, bool auto_close)
        {
            FileHandleBase fh;
            auto pathCStr = path.cStr();

            fh.m_handle = fopen(pathCStr.get(), "r+");
            if (fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "' for reading and writing: " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;
            fh.setAutoClose(auto_close);

            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForAppendingAtPath(const string_type & path,
                                                                                              bool auto_close)
        {
            FileHandleBase fh;
            auto pathCstr = path.cStr();

            fh.m_handle = fopen(pathCstr.get(), "a");
            if (fh.m_handle == nullptr)
            {
                string_type msg("Unable to open ");
                msg += "'" + path + "' for reading and writing: " + strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }

            fh.m_fileName = path;
            fh.setAutoClose(auto_close);

            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleFromHandle(handle_type h, bool auto_close)
        {
            FileHandleBase fh{h, auto_close};
            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingFromDescriptor(descriptor_type d,
                                                                                                    bool auto_close)
        {
            if (d < 0)
            {
                throw std::invalid_argument("descriptor cannot be negative");
            }

            FileHandleBase h{};
            h.m_handle = fdopen(d, "r");
            if (h.m_handle == nullptr)
            {
                throw std::invalid_argument("descriptor not suitable for reading");
            }
            h.m_autoClose = auto_close;
            return h;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForWritingFromDescriptor(descriptor_type d,
                                                                                                    bool auto_close)
        {
            if (d < 0)
            {
                throw std::invalid_argument("descriptor cannot be negative");
            }

            FileHandleBase h{};
            h.m_handle = fdopen(d, "w");
            if (h.m_handle == nullptr)
            {
                throw std::invalid_argument("descriptor not suitable for writing");
            }
            h.m_autoClose = auto_close;
            return h;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingAndWritingFromDescriptor(
            descriptor_type d, bool auto_close)
        {
            if (d < 0)
            {
                throw std::invalid_argument("descriptor cannot be negative");
            }

            FileHandleBase h{};
            h.m_handle = fdopen(d, "r+");
            if (h.m_handle == nullptr)
            {
                throw std::invalid_argument("descriptor not suitable for reading and writing");
            }
            h.m_autoClose = auto_close;
            return h;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForAppendingFromDescriptor(descriptor_type d,
                                                                                                      bool auto_close)
        {
            if (d < 0)
            {
                throw std::invalid_argument("descriptor cannot be negative");
            }

            FileHandleBase h{};
            h.m_handle = fdopen(d, "a");
            if (h.m_handle == nullptr)
            {
                throw std::invalid_argument("descriptor not suitable for appending");
            }
            h.m_autoClose = auto_close;
            return h;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardInput(bool auto_close)
        {
            FileHandleBase fh;
            fh.m_handle = stdin;
            fh.m_fileName = "stdin";
            fh.setAutoClose(auto_close);
            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardOutput(bool auto_close)
        {
            FileHandleBase fh;
            fh.m_handle = stdout;
            fh.m_fileName = "stdout";
            fh.setAutoClose(auto_close);
            return fh;
        }

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardError(bool auto_close)
        {
            FileHandleBase fh;
            fh.m_handle = stderr;
            fh.m_fileName = "stderr";
            fh.setAutoClose(auto_close);
            return fh;
        }

        template<>
        FileHandleBase<FILE *, int>::descriptor_type FileHandleBase<FILE *, int>::fileDescriptor()
        {
            if (m_handle != nullptr)
                return fileno(m_handle);
            return -1;
        }

        template<>
        FileHandleBase<FILE *, int>::handle_type FileHandleBase<FILE *, int>::fileHandle()
        {
            return m_handle;
        }

#define kReadBufferSize 1024

        template<>
        FileHandleBase<FILE *, int>::size_type FileHandleBase<FILE *, int>::readAvailableDataToBuffer(char * buffer,
                                                                                                      size_type length)
        {
            if (m_handle == nullptr)
            {
                return 0;
            }

            // We absolutely have to make use of read() here because the C stream library
            // will wait to fill a buffer with fread/fgets/fgetc if no data is present.

            auto descriptor = fileDescriptor();

            auto amountRead = read(descriptor, buffer, length);
            if (amountRead < 0)
            {
                throw std::system_error{errno, std::system_category(), "Error reading from stream"};
            }
            else if (amountRead == 0)
            {
                return 0;
            }

            return static_cast<size_type>(amountRead);
        }

        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readAvailableData(size_type length)
        {
            data_type d;
            auto buffer = std::unique_ptr<char, std::default_delete<char[]>>(new char[length]);
            char * tmp = buffer.get();

            auto amountRead = readAvailableDataToBuffer(tmp, length);

            if (amountRead == 0)
            {
                return d;
            }

            d.append(tmp, static_cast<Data::size_type>(amountRead));

            return d;
        }

        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readToEndOfFile()
        {
            data_type d;
            char buffer[kReadBufferSize];

            if (m_handle == nullptr)
                return d;

            while (! feof(m_handle))
            {
                size_t amountRead = fread(buffer, sizeof(char), kReadBufferSize, m_handle);

                if (ferror(m_handle))
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
            auto buffer = std::unique_ptr<char, std::default_delete<char[]>>(new char[length]);
            char * tmp = buffer.get();
            size_t bytesToRead = length;
            size_t bytesRead = 0;

            if (m_handle == nullptr)
                return d;

            while (bytesToRead > 0)
            {
                bytesRead = fread(tmp, sizeof(char), bytesToRead, m_handle);
                if (ferror(m_handle))
                {
                    string_type msg("Error reading stream from ");
                    msg += m_fileName;
                    auto msgCStr = msg.cStr();
                    throw std::runtime_error(msgCStr.get());
                }

                d.append(tmp, bytesRead);
                tmp += bytesRead;
                bytesToRead -= bytesRead;

                if (feof(m_handle))
                    return d;
            }

            return d;
        }

        template<>
        auto FileHandleBase<FILE *, int>::readLine() -> string_type
        {
            // The file handle API reads bytes. Those bytes will be in a system encoding.
            // For now, we assume that the system encoding is the same encoding used by the
            // string class. We will construct characters byte by byte and then join them together
            // into a string.

            auto & encoder = string_type::getEncoder();

            StringEncoder::char_type byte{};
            bool has_read_a_line{false};

            auto buffer = std::unique_ptr<decltype(byte)[], std::default_delete<decltype(byte)[]>>(
                new decltype(byte)[encoder.numberOfBytesRequiredForLargestCharacterValue()]);

            std::vector<char> byte_array{};

            while (! has_read_a_line)
            {
                auto bytes_read = fread(&byte, sizeof(byte), 1, m_handle);
                if (bytes_read == 0)
                {
                    // 0 indicates end-of-file, so break out of the loop.
                    has_read_a_line = true;
                    continue;
                }

                if (byte == '\n')
                {
                    byte_array.emplace_back(byte);
                    has_read_a_line = true;
                    continue;
                }

                auto bytes_to_expect =
                    encoder.bytesToExpectForCharacterInByteSequence(&byte, sizeof(byte), encoder.getMyEndianness());

                if (bytes_to_expect == 1)
                {
                    byte_array.emplace_back(byte);
                    continue;
                }

                if (bytes_to_expect > 1)
                {
                    *(buffer.get()) = byte;
                    auto bytes_still_to_read = bytes_to_expect - 1;
                    auto tmp = buffer.get() + 1;
                    while (bytes_still_to_read)
                    {
                        auto next_bytes_read = fread(tmp, sizeof(byte), bytes_still_to_read, m_handle);
                        if (next_bytes_read == 0)
                        {
                            // End-of-file.
                            has_read_a_line = true;
                            break;
                        }

                        bytes_still_to_read -= static_cast<decltype(bytes_still_to_read)>(next_bytes_read);
                        tmp += next_bytes_read;
                    }

                    if (has_read_a_line)
                    {
                        continue;
                    }

                    // buffer should now contain a character of bytes.
                    for (decltype(bytes_to_expect) i = 0; i < bytes_to_expect; i++)
                    {
                        byte_array.emplace_back(*(buffer.get() + i));
                    }
                }
            }

            return string_type{byte_array.data(), byte_array.size()};
        }

        template<>
        void FileHandleBase<FILE *, int>::writeString(const string_type & s)
        {
            // Someday we need to add an Encoding object to the FileHandle, so that
            // a file handle represents a file in a particular encoding.  For now,
            // try to write the file in UTF-8 encoding.

            // I'm not happy about this step because it will copy the data in 's',
            // however, I would rather keep the internals of 's' agnostic to a particular
            // encoding, so I will allow it for now.
            auto s_in_utf8 = s.getAsDataInUTF8Encoding();
            writeData(s_in_utf8);
        }

        template<>
        void FileHandleBase<FILE *, int>::writeData(const data_type & d)
        {
            write(d.bytes(), d.length());
        }

        template<>
        void FileHandleBase<FILE *, int>::write(const void * vp, size_type length)
        {
            const char * bytes = reinterpret_cast<const char *>(vp);
            auto bytesToWrite = length;
            const char * tmp = bytes;
            size_t bytesWrote = 0;
            int errorCounter = 0;

            if (m_handle == nullptr)
                return;

            while (bytesToWrite > 0)
            {
                bytesWrote = fwrite(tmp, sizeof(char), bytesToWrite, m_handle);
                if (ferror(m_handle))
                {
                    if (++errorCounter > 10)
                    {
                        string_type msg("Error writing to stream for file ");
                        msg += m_fileName;
                        auto msgCStr = msg.cStr();
                        throw std::runtime_error(msgCStr.get());
                    }
                    clearerr(m_handle);
                }

                tmp += bytesWrote;
                bytesToWrite -= bytesWrote;
            }
        }

        template<>
        void FileHandleBase<FILE *, int>::flush()
        {
            fflush(m_handle);
        }

        template<>
        FileHandleBase<FILE *, int>::size_type FileHandleBase<FILE *, int>::offsetInFile()
        {
            if (m_handle == nullptr)
                return 0;
            return static_cast<size_type>(ftello(m_handle));
        }

        template<>
        void FileHandleBase<FILE *, int>::seekToEndOfFile()
        {
            if (m_handle == nullptr)
                return;
            fseeko(m_handle, 0, SEEK_END);
        }

        template<>
        void FileHandleBase<FILE *, int>::seekToStartOfFile()
        {
            if (m_handle == nullptr)
            {
                return;
            }
            fseeko(m_handle, 0, SEEK_SET);
        }

        template<>
        void FileHandleBase<FILE *, int>::seekToFileOffset(size_type offset)
        {
            if (m_handle == nullptr)
                return;
            fseeko(m_handle, static_cast<off_t>(offset), SEEK_CUR);
        }

        template<>
        auto FileHandleBase<FILE *, int>::atEndOfFile() const -> bool
        {
            if (m_handle == nullptr)
            {
                return true;
            }
            return feof(m_handle) != 0;
        }

        template<>
        void FileHandleBase<FILE *, int>::closeFile()
        {
            if (m_handle != nullptr)
            {
                fclose(m_handle);
                m_handle = nullptr;
            }
        }

        template<>
        void FileHandleBase<FILE *, int>::truncateFileAtOffset(size_type offset)
        {
            if (m_handle == nullptr)
                return;

            auto descriptor = fileDescriptor();
            auto apiResult = ftruncate(descriptor, static_cast<off_t>(offset));
            if (apiResult != 0)
            {
                string_type msg("Unable to truncate file ");
                msg += m_fileName + ": ";
                msg += strerror(errno);
                auto msgCStr = msg.cStr();
                throw std::runtime_error(msgCStr.get());
            }
        }

        template<>
        std::ostream & FileHandleBase<FILE *, int>::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
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

    } // namespace Foundation

} // namespace TF
