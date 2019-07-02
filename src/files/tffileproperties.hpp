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

#ifndef TFFILEPROPERTIES_HPP
#define TFFILEPROPERTIES_HPP

#define NEEDS_OSTREAM
#define NEEDS_STRING
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tffilepermissions.hpp"

namespace TF
{

    namespace Foundation
    {

        enum class FileType
        {
            Regular,
            Directory,
            CharacterSpecial,
            Block,
            Pipe,
            Link,
            Socket,
            MessageQueue,
            Semaphore,
            SharedMemory
        };

        std::ostream &operator<<(std::ostream &o, const FileType &t);


        struct FileProperties : public AllocatorInterface
        {
            using permissions = FilePermissions;

            using file_type = FileType;

            using size_type = size_t;

            using string_type = std::string;

            size_type size;

            file_type type;

            permissions permission;

            int userID;

            int groupID;

            string_type linkTarget;

            FileProperties();


            FileProperties(const FileProperties &p);


            FileProperties &operator=(const FileProperties &p);


            bool operator==(const FileProperties &p);


            bool operator!=(const FileProperties &p);


            std::ostream &description(std::ostream &o) const;
        };


        std::ostream &operator<<(std::ostream &o, const FileProperties &p);


    }    // namespace Foundation

}    // namespace TF

#endif    // TFFILEPROPERTIES_HPP
