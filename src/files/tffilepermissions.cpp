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

#include <string>
#include "tffilepermissions.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        FilePermissions::FilePermissions() : permissions(0)
        {
        }


        FilePermissions::FilePermissions(const int &p)
        {
            permissions = p;
        }


        FilePermissions::FilePermissions(const FilePermissions &p)
        {
            permissions = p.permissions;
        }


        FilePermissions &FilePermissions::operator=(const FilePermissions &p)
        {
            if(this != &p)
            {
                permissions = p.permissions;
            }
            return *this;
        }


        FilePermissions &FilePermissions::operator=(const int &p)
        {
            permissions = p;
            return *this;
        }


        bool FilePermissions::operator==(const FilePermissions &p) const
        {
            if(this != &p)
            {
                return permissions == p.permissions;
            }

            return true;
        }


        bool FilePermissions::operator==(const int &p) const
        {
            return permissions == p;
        }


        bool FilePermissions::operator!=(const FilePermissions &p) const
        {
            if(*this == p)
                return false;
            return true;
        }


        bool FilePermissions::operator!=(const int &p) const
        {
            return permissions != p;
        }

#define CHECK_PERMISSION(perms, checkPerm) (perms & checkPerm) == checkPerm ? true : false


        bool FilePermissions::hasUserReadPermission() const
        {
            return CHECK_PERMISSION(permissions, UserRead);
        }


        bool FilePermissions::hasUserWritePermission() const
        {
            return CHECK_PERMISSION(permissions, UserWrite);
        }


        bool FilePermissions::hasUserExecutePermission() const
        {
            return CHECK_PERMISSION(permissions, UserExecute);
        }


        bool FilePermissions::hasGroupReadPermission() const
        {
            return CHECK_PERMISSION(permissions, GroupRead);
        }


        bool FilePermissions::hasGroupWritePermission() const
        {
            return CHECK_PERMISSION(permissions, GroupWrite);
        }


        bool FilePermissions::hasGroupExecutePermission() const
        {
            return CHECK_PERMISSION(permissions, GroupExecute);
        }


        bool FilePermissions::hasOtherReadPermission() const
        {
            return CHECK_PERMISSION(permissions, OtherRead);
        }


        bool FilePermissions::hasOtherWritePermission() const
        {
            return CHECK_PERMISSION(permissions, OtherWrite);
        }


        bool FilePermissions::hasOtherExecutePermission() const
        {
            return CHECK_PERMISSION(permissions, OtherExecute);
        }


        bool FilePermissions::hasStickyBit() const
        {
            return CHECK_PERMISSION(permissions, Sticky);
        }


        bool FilePermissions::hasSetUserID() const
        {
            return CHECK_PERMISSION(permissions, SetUserID);
        }


        bool FilePermissions::hasSetGroupID() const
        {
            return CHECK_PERMISSION(permissions, SetGroupID);
        }


#define SET_PERMISSION(value, permission)                                                                              \
    if(value)                                                                                                          \
        permissions = permissions | permission;                                                                        \
    else                                                                                                               \
        permissions = permissions & ~permission


        void FilePermissions::setUserReadPermission(bool value)
        {
            SET_PERMISSION(value, UserRead);
        }


        void FilePermissions::setUserWritePermission(bool value)
        {
            SET_PERMISSION(value, UserWrite);
        }


        void FilePermissions::setUserExecutePermission(bool value)
        {
            SET_PERMISSION(value, UserExecute);
        }


        void FilePermissions::setGroupReadPermission(bool value)
        {
            SET_PERMISSION(value, GroupRead);
        }


        void FilePermissions::setGroupWritePermission(bool value)
        {
            SET_PERMISSION(value, GroupWrite);
        }


        void FilePermissions::setGroupExecutePermission(bool value)
        {
            SET_PERMISSION(value, GroupExecute);
        }


        void FilePermissions::setOtherReadPermission(bool value)
        {
            SET_PERMISSION(value, OtherRead);
        }


        void FilePermissions::setOtherWritePermission(bool value)
        {
            SET_PERMISSION(value, OtherWrite);
        }


        void FilePermissions::setOtherExecutePermission(bool value)
        {
            SET_PERMISSION(value, OtherExecute);
        }


        void FilePermissions::setStickyBit(bool value)
        {
            SET_PERMISSION(value, Sticky);
        }


        void FilePermissions::setSetUserID(bool value)
        {
            SET_PERMISSION(value, SetUserID);
        }


        void FilePermissions::setSetGroupID(bool value)
        {
            SET_PERMISSION(value, SetGroupID);
        }


        FilePermissions::string_type FilePermissions::unixForm() const
        {
            string_type formattedOutput;

            if(hasStickyBit())
                formattedOutput += "s";
            else
                formattedOutput += "-";

            if(hasUserReadPermission())
                formattedOutput += "r";
            else
                formattedOutput += "-";

            if(hasUserWritePermission())
                formattedOutput += "w";
            else
                formattedOutput += "-";

            if(hasUserExecutePermission())
                formattedOutput += "x";
            else if(hasSetUserID())
                formattedOutput += "S";
            else
                formattedOutput += "-";

            if(hasGroupReadPermission())
                formattedOutput += "r";
            else
                formattedOutput += "-";

            if(hasGroupWritePermission())
                formattedOutput += "w";
            else
                formattedOutput += "-";

            if(hasGroupExecutePermission())
                formattedOutput += "x";
            else if(hasSetGroupID())
                formattedOutput += "S";
            else
                formattedOutput += "-";

            if(hasOtherReadPermission())
                formattedOutput += "r";
            else
                formattedOutput += "-";

            if(hasOtherWritePermission())
                formattedOutput += "w";
            else
                formattedOutput += "-";

            if(hasOtherExecutePermission())
                formattedOutput += "x";
            else
                formattedOutput += "-";

            return formattedOutput;
        }


        std::ostream &FilePermissions::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("FilePermissions");
                formatter->addClassMember<string_type>("string_type", "permissions", unixForm());
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const FilePermissions &p)
        {
            return p.description(o);
        }


    }    // namespace Foundation

}    // namespace TF
