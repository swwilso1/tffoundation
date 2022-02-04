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

#ifndef TFFILEHANDLEBASE_HXX
#define TFFILEHANDLEBASE_HXX

#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfstring.hpp"
#include "tfdata.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * @brief Base class that defines the interface for interacting with file handle objects.
         *
         * This class is a templated class to allow for platform specific implementation on platforms with
         * different underlying file system semantics and data structures such as Unix vs Win32.  The actual
         * @e FileHandle classes will be versions of this class with platform specific @e Handle and @e Descriptor
         * template arguments.
         *
         * This class does not provide any open() methods.   To get a file handle you need to use one of the static
         * methods designed to open/create a file:
         * - fileHandleForReadingAtPath
         * - fileHandleForWritingAtPath
         * - fileHandleForReadingAndWritingAtPath
         * - fileHandleForAppendingAtPath
         * - fileHandleForStandardInput
         * - fileHandleForStandardOutput
         * - fileHandleForStandardError
         *
         * Users will not interact directly with the FileHandleBase object, but instead one of the platform specific
         * implementations.
         *
         * @tparam Handle the type that represents a handle to a file object.
         * @tparam Descriptor the type that represents a low level reference to the file object.
         */
        template<class Handle, class Descriptor>
        class FileHandleBase : public AllocatorInterface
        {
        public:
#pragma mark - Class types

            /** @brief the file system handle type */
            using handle_type = Handle;

            /** @brief the file system descriptor type */
            using descriptor_type = Descriptor;

            /** @brief the size type used for file sizes and offset values */
            using size_type = Size_t;

            /** @brief the string type used to specify path names */
            using string_type = String;

            /** @brief the type used to store buffers of memory */
            using data_type = Data;

#pragma mark - Constructor/Destructor

            /**
             * @brief Simple default constructor
             * @param auto_close true if the destructor should automatically close the
             * file handle and false otherwise.
             *
             * The Constructor does not open any files.
             */
            explicit FileHandleBase(bool auto_close = false);

            /**
             * @brief copy constructor
             * @param fh the file handle to copy.
             *
             * The copy construction will copy the auto close setting from @e fh.  After construction, you will
             * have two objects potentially with auto-close settings.  The caller must call setAutoClose(false)
             * for the object that should no longer automatically close the file handle.
             */
            FileHandleBase(const FileHandleBase & fh);

            /**
             * @brief move constructor
             * @param fh the file handle to move.
             *
             * After the object is constructed the original @e fh object will no longer contain a valid
             * file handle.
             */
            FileHandleBase(FileHandleBase && fh);

            /**
             * @brief Simple destructor.
             *
             * The destructor might close the underlying handle to the file system object
             * depending if the caller called the setAutoClose(true) method.
             */
            ~FileHandleBase();

#pragma mark - Assignment operators

            /**
             * @brief copy assignment operator
             * @param fh the other file handle
             * @return this file handle now referring to the underlying file system object as @e fh.
             *
             * If the current contents of the file handle are set to auto-close, then this operation closes
             * the handle before copying the handle from @e fh.  The copy operation will copy @e fh's
             * auto close setting which will leave two file handles in an auto close state.  One should
             * be turned off.  The caller must explicitly call setAutoClose(false) on the handle that
             * should no longer auto close the file.  This requirement may change in the future.
             */
            FileHandleBase & operator=(const FileHandleBase & fh);

            /**
             * @brief move assignment operator
             * @param fh the other file handle
             * @return this file handle now referring to the underlying file system object as @e fh.
             *
             * If the current contents of the file handle are set to auto-close, then this operation closes
             * the handle before copying the handle from @e fh.  After the move operation the original file
             * handle object will no longer refer to a valid file handle.
             */
            FileHandleBase & operator=(FileHandleBase && fh);

#pragma mark - Static methods to create FileHandles

            /**
             * @brief static method that returns an object with a handle for reading the object at @e path.
             *
             * The method can open absolute or relative paths.   If using a relative path, the method will open
             * the file relative to the current working directory.   If the argument @e path contains intermediate
             * directories, these directories must already exist.
             *
             * @param path The location of the object to read in the file system.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object initialized to read the contents of the file located at @e path.
             */
            static FileHandleBase fileHandleForReadingAtPath(const string_type & path, bool auto_close = false);

            /**
             * @brief static method that returns an object with a handle for writing to a file system object at @e
             * path.
             *
             * The method can open absolute or relative paths.   If using a relative path, the method will open
             * the file relative to the current working directory.   If the argument @e path contains intermediate
             * directories, these directories must already exist.   This method will truncate and overwrite the contents
             * of any previously existing file system object already located at @e path.
             *
             * @param path the location of the object to write in the file system.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object initialized to write to the object in the file system located at @e path.
             */
            static FileHandleBase fileHandleForWritingAtPath(const string_type & path, bool auto_close = false);

            /**
             * @brief static method that returns an object suitable for reading and writing to the file system object
             * located at @e path.
             *
             * The method can open absolute or relative paths.   If using a relative path, the method will open
             * the file relative to the current working directory.   If the argument @e path contains intermediate
             * directories, these directories must already exist.
             *
             * @param path the location of the object to read/write in the file system.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object initialized to read/write to object in the file system located at @e
             * path.
             */
            static FileHandleBase fileHandleForReadingAndWritingAtPath(const string_type & path,
                                                                       bool auto_close = false);

            /**
             * @brief static method that returns an object suitable for appending data to the file system object located
             * at @e path.
             *
             * The method can open absolute or relative paths.   If using a relative path, the method will open
             * the file relative to the current working directory.   If the argument @e path contains intermediate
             * directories, these directories must already exist.  If the object already exists at @e path, the method
             * will return a file handle that will write data (append data) to the end of the file after the previously
             * existing content.
             *
             * @param path the location of the object to append in the file system.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object initialized to append data to the object in the file system located at
             * @e path.
             */
            static FileHandleBase fileHandleForAppendingAtPath(const string_type & path, bool auto_close = false);

            /**
             * @brief static method that returns a file handle opened for reading from the process' standard input
             * stream.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object opened to read from the process' standard input.
             */
            static FileHandleBase fileHandleWithStandardInput(bool auto_close = false);

            /**
             * @brief static method that returns a file handle opened for writing to a process's standard output
             * stream.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object opened to write to the process's standard output.
             */
            static FileHandleBase fileHandleWithStandardOutput(bool auto_close = false);

            /**
             * @brief a static method that returns a file handle opened for writing to a process's standard error
             * stream.
             * @param auto_close true if the handle should automatically close the handle and false otherwise.
             * @return a FileHandleBase object opened to write to the process' standard error.
             */
            static FileHandleBase fileHandleWithStandardError(bool auto_close = false);

#pragma mark - Method to get descriptor object

            /**
             * @brief method to return the low level descriptor representation of the file handle.
             * @return the descriptor object that represents the file.
             */
            descriptor_type fileDescriptor();

#pragma mark - Methods for reading data
            /**
             * @brief method to read a buffer of data from the stream.
             *
             * The method reads a buffer of data available in the stream and returns that buffer.  The buffer length
             * is unspecified specifically to allow the file handle to read any data present in the stream.
             *
             * @return a data object that contains the buffer of bytes read from the stream.
             */
            data_type readAvailableData();

            /**
             * @brief method to read all the data in the file up to the end of the file.
             * @return a data object containing all the remaining data from the file.
             */
            data_type readToEndOfFile();

            /**
             * @brief method to read a buffer of the specified @e length.
             *
             * The method may return a data object with less than @e length bytes if the method reaches the end of
             * the file.
             *
             * @param length the number of bytes to read.
             * @return a data object with at most @e length bytes of data from the file.
             */
            data_type readDataOfLength(size_type length);

#pragma mark - Methods for writing data

            /**
             * @brief method to write a the contents of a data object to a file.
             *
             * The file handle must be opened for writing.
             *
             * @param d the data object containing the bytes to write to the file.
             */
            void writeData(const data_type & d);

#pragma mark - Methods for controlling file offsets

            /**
             * @brief method that returns the current read location in the file as the number
             * of bytes preceding the current location in the file.
             * @return the number of bytes in the file marking the current read location.
             */
            size_type offsetInFile();

            /**
             * @brief method to move the read location of the file handle to the end of the file.
             */
            void seekToEndOfFile();

            /**
             * @brief method to move the read location of the file handle to the start of the file.
             */
            void seekToStartOfFile();

            /**
             * @brief method to move the read location of the file handle to an arbitrary location in the file.
             *
             * The @e offset parameter must be less than the number of bytes in the file.
             *
             * @param offset the number of bytes in the file to position the read location.
             */
            void seekToFileOffset(size_type offset);

#pragma mark - Method to close file

            /**
             * @brief method to close the underlying handle to the file system object.
             */
            void closeFile();

            /**
             * @brief method to enable or disable automatic file handle close behavior in the object destructor.
             *
             * By default a file handle must be manually closed with the @e closeFile method.  When called with
             * @e autoClose true, the setAutoClose method turns on automatic close behavior in the file handle object
             * so that when the file handle object goes out of scope, the file handle destructor automatically closes
             * the underlying file system object.
             *
             * @param autoClose true if the file handle object should automatically close the handle to the file system
             * object when the file handle goes out of scope and false otherwise.
             */
            void setAutoClose(bool autoClose);

#pragma mark - Method to truncate file size

            /**
             * @brief method to shrink or enlarge the number of bytes used the file.
             *
             * If @e offset is less than the number of bytes in the file, the the file size will be shrunk to @e offset
             * bytes.   If @e offset is larger than the number of bytes in the file, then the file will be grown
             * to contain that many bytes.
             *
             * @param offset the number of bytes for the file to now contain.
             */
            void truncateFileAtOffset(size_type offset);

#pragma mark - Method for writing to a stream

            /**
             * @brief method for writing a file handle object to a stream object.
             * @param o the output stream object
             * @return @e o after writing the contents of the file to the stream.
             */
            std::ostream & description(std::ostream & o) const;

        public:
#pragma mark - Class members

            /** @brief the handle to the underlying file system object */
            handle_type m_handle;

            /** @brief the boolean that holds true when the file handle should automatically close the file handle and
             * false otherwise.
             */
            bool m_autoClose;

            /** @brief the name of the underlying file system object */
            string_type m_fileName;
        };

        /**
         * @brief overloaded << operator for writing file handle objects to an ostream.
         * @tparam Handle the type of the underlying file system object.
         * @tparam Description the type of the low level file descriptor object.
         * @param o the ostream object
         * @param h the file handle object.
         * @return @e o after writing the contents of the handle @e h to stream @e o.
         */
        template<class Handle, class Description>
        std::ostream & operator<<(std::ostream & o, const FileHandleBase<Handle, Description> & h);

    } // namespace Foundation

} // namespace TF

#include "tffilehandlebase.cxx"

#endif // TFFILEHANDLEBASE_HXX
