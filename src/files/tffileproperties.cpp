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

#include "tffileproperties.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        std::ostream & operator<<(std::ostream & o, const FileType & t)
        {
            switch (t)
            {
                case FileType::Regular:
                    o << "Regular";
                    break;
                case FileType::Directory:
                    o << "Directory";
                    break;
                case FileType::CharacterSpecial:
                    o << "CharacterSpecial";
                    break;
                case FileType::Block:
                    o << "Block";
                    break;
                case FileType::Pipe:
                    o << "Pipe";
                    break;
                case FileType::Link:
                    o << "Link";
                    break;
                case FileType::Socket:
                    o << "Socket";
                    break;
                case FileType::MessageQueue:
                    o << "MessageQueue";
                    break;
                case FileType::Semaphore:
                    o << "Semaphore";
                    break;
                case FileType::SharedMemory:
                    o << "SharedMemory";
                    break;
            }

            return o;
        }

        FileProperties::FileProperties() : size(0), type(FileType::Regular), userID(0), groupID(0) {}

        FileProperties::FileProperties(const FileProperties & p)
        {
            size = p.size;
            type = p.type;
            permission = p.permission;
            userID = p.userID;
            groupID = p.groupID;
            linkTarget = p.linkTarget;
        }

        FileProperties & FileProperties::operator=(const FileProperties & p)
        {
            if (this != &p)
            {
                size = p.size;
                type = p.type;
                permission = p.permission;
                userID = p.userID;
                groupID = p.groupID;
                linkTarget = p.linkTarget;
            }

            return *this;
        }

        bool FileProperties::operator==(const FileProperties & p)
        {
            if (this != &p)
            {
                if (size != p.size)
                    return false;
                if (type != p.type)
                    return false;
                if (permission != p.permission)
                    return false;
                if (userID != p.userID)
                    return false;
                if (groupID != p.groupID)
                    return false;
                if (linkTarget != p.linkTarget)
                    return false;
            }
            return true;
        }

        bool FileProperties::operator!=(const FileProperties & p)
        {
            if (*this == p)
                return false;
            return true;
        }

        std::ostream & FileProperties::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("FileProperties");
                formatter->addClassMember<size_type>("size_type", "size", size);
                formatter->addClassMember<file_type>("file_type", "type", type);
                formatter->addClassMember<permissions>("permissions", "permission", permission);
                formatter->addClassMember<int>("int", "userID", userID);
                formatter->addClassMember<int>("int", "groupID", groupID);
                formatter->addClassMember<string_type>("string_type", "linkTarget", linkTarget);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        std::ostream & operator<<(std::ostream & o, const FileProperties & p)
        {
            return p.description(o);
        }

    } // namespace Foundation

} // namespace TF
