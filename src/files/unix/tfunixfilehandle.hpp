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

#ifndef TFUNIXFILEHANDLE_HPP
#define TFUNIXFILEHANDLE_HPP

#define NEEDS_STDIO_H
#include "tfheaders.hpp"
#include "tffilehandlebase.hxx"

namespace TF
{

    namespace Foundation
    {
        /**
         * The Unix platforms make use of the FileHandleBase class with a FILE * handle and int descriptor type.
         */
        using FileHandle = FileHandleBase<FILE *, int>;

        /**
         * Declare template specializations for all the methods of the FileHandleBase template type.
         */

        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(bool auto_close);

        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(const FileHandleBase & fh);

        template<>
        FileHandleBase<FILE *, int>::FileHandleBase(FileHandleBase && fh);

        template<>
        FileHandleBase<FILE *, int>::~FileHandleBase();

        template<>
        FileHandleBase<FILE *, int> & FileHandleBase<FILE *, int>::operator=(const FileHandleBase & fh);

        template<>
        FileHandleBase<FILE *, int> & FileHandleBase<FILE *, int>::operator=(FileHandleBase && fh);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingAtPath(const string_type & path,
                                                                                            bool auto_close);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForWritingAtPath(const string_type & path,
                                                                                            bool auto_close);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForReadingAndWritingAtPath(
            const string_type & path, bool auto_close);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleForAppendingAtPath(const string_type & path,
                                                                                              bool auto_close);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardInput(bool auto_close);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardOutput(bool auto_close);

        template<>
        FileHandleBase<FILE *, int> FileHandleBase<FILE *, int>::fileHandleWithStandardError(bool auto_close);

        template<>
        FileHandleBase<FILE *, int>::descriptor_type FileHandleBase<FILE *, int>::fileDescriptor();

        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readAvailableData();

        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readToEndOfFile();

        template<>
        FileHandleBase<FILE *, int>::data_type FileHandleBase<FILE *, int>::readDataOfLength(size_type length);

        template<>
        void FileHandleBase<FILE *, int>::writeData(const data_type & d);

        template<>
        void FileHandleBase<FILE *, int>::flush();

        template<>
        FileHandleBase<FILE *, int>::size_type FileHandleBase<FILE *, int>::offsetInFile();

        template<>
        void FileHandleBase<FILE *, int>::seekToEndOfFile();

        template<>
        void FileHandleBase<FILE *, int>::seekToFileOffset(size_type offset);

        template<>
        void FileHandleBase<FILE *, int>::closeFile();

        template<>
        void FileHandleBase<FILE *, int>::truncateFileAtOffset(size_type offset);

        template<>
        std::ostream & FileHandleBase<FILE *, int>::description(std::ostream & o) const;

    } // namespace Foundation

} // namespace TF

#endif // TFUNIXFILEHANDLE_HPP
