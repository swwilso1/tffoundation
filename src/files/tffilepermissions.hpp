/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2017 Tectiform Inc.

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

#ifndef TFFILEPERMISSIONS_HPP
#define TFFILEPERMISSIONS_HPP

#define NEEDS_OSTREAM
#define NEEDS_STRING
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

    namespace Foundation
    {

        enum Permission
        {
            UserRead      = 0x001,
            UserWrite     = 0x002,
            UserExecute   = 0x004,
            GroupRead     = 0x008,
            GroupWrite    = 0x010,
            GroupExecute  = 0x020,
            OtherRead     = 0x040,
            OtherWrite    = 0x080,
            OtherExecute  = 0x100,
            Sticky        = 0x200,
            SetUserID     = 0x400,
            SetGroupID    = 0x800
        };



        class FilePermissions : public AllocatorInterface
        {
        public:

            using permission = Permission;

            using string_type = std::string;

            FilePermissions();

            FilePermissions(const int &p);

            FilePermissions(const FilePermissions &p);

            ~FilePermissions() {}

            FilePermissions& operator=(const FilePermissions &p);

            FilePermissions& operator=(const int &p);

            bool operator==(const FilePermissions &p) const;

            bool operator==(const int &p) const;

            bool operator!=(const FilePermissions &p) const;

            bool operator!=(const int &p) const;

            bool hasUserReadPermission() const;

            bool hasUserWritePermission() const;

            bool hasUserExecutePermission() const;

            bool hasGroupReadPermission() const;

            bool hasGroupWritePermission() const;

            bool hasGroupExecutePermission() const;

            bool hasOtherReadPermission() const;

            bool hasOtherWritePermission() const;

            bool hasOtherExecutePermission() const;

            bool hasStickyBit() const;

            bool hasSetUserID() const;

            bool hasSetGroupID() const;

            void setUserReadPermission(bool value);

            void setUserWritePermission(bool value);

            void setUserExecutePermission(bool value);

            void setGroupReadPermission(bool value);

            void setGroupWritePermission(bool value);

            void setGroupExecutePermission(bool value);

            void setOtherReadPermission(bool value);

            void setOtherWritePermission(bool value);

            void setOtherExecutePermission(bool value);

            void setStickyBit(bool value);

            void setSetUserID(bool value);

            void setSetGroupID(bool value);

            string_type unixForm() const;

            std::ostream& description(std::ostream &o) const;

        private:

            int permissions;
        };


        std::ostream& operator<<(std::ostream &o, const FilePermissions &p);

    } // Foundation

} // TF


#endif //TFFILEPERMISSIONS_HPP
