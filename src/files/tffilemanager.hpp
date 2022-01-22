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

#ifndef TFFILEMANAGER_HPP
#define TFFILEMANAGER_HPP

#define NEEDS_OSTREAM
#define NEEDS_VECTOR
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tffileproperties.hpp"
#include "tffilepermissions.hpp"
#include "tfstring.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * @brief class for manipulating and inspecting files in a file system.
         */
        class FileManager
        {
        public:
            /** @brief basic string type */
            using string_type = String;

            /** @brief basic size type */
            using size_type = size_t;

            /** @brief basic string list(array) type */
            using string_array_type = std::vector<string_type>;

            /** @brief file properties type */
            using file_properties_type = FileProperties;

            /** @brief file permissions type */
            using file_permissions_type = FilePermissions;


            /**
             * @brief default constructor.
             */
            FileManager() = default;


            /**
             * @brief method to return the name of a usable temporary directory.
             * @return the temporary directory name.
             *
             * This method will respect the following environment variables:
             * TEMP, TMP, TEMP_DIR, and TMP_DIR.   If none of those variables
             * are set, the method returns /tmp.
             */
            string_type temporaryDirectory() const;


            /**
             * @brief method to return the path to the home directory of the current user.
             * @return the user's home directory.
             */
            string_type homeDirectoryForCurrentUser() const;


            /**
             * @brief method to find the home directory for any named system user.
             * @param user the name of the user
             * @return the home directory for user @e user.
             *
             * The caller should pass one of the system user entries present in /etc/passwd.
             */
            string_type homeDirectoryForUser(const string_type &user) const;


            /**
             * @brief method to return a string array of the contents of a directory.
             * @param path the directory location
             * @return an array of strings of the names of the items in @e path.
             */
            string_array_type contentsOfDirectoryAtPath(const string_type &path) const;


            /**
             * @brief method to return a string array of all the subpaths found in @e path.
             * @param path the directory to read
             * @return an array of strings representing the paths of all the items in
             * @e path and all the sub-items below @e path hierarchically.
             */
            string_array_type subpathsOfDirectoryAtPath(const string_type &path) const;


            /**
             * @brief method to return a string array of the all the entries in @e path in
             * absolute path form.
             * @param path the directory to read
             * @return an array of strings representing all the elements in @e path in full
             * absolute path form.
             */
            string_array_type subpathsAtPath(const string_type &path) const;


            /**
             * @brief method to create a directory at the named path
             * @param path
             */
            void createDirectoryAtPath(const string_type &path) const;


            /**
             * @brief method to create a directory at the named path including any intermediate
             * directories.
             * @param path
             */
            void createDirectoriesAtPath(const string_type &path) const;


            /**
             * @brief method to create an empty file at the named path.
             * @param path
             */
            void createFileAtPath(const string_type &path) const;


            /**
             * @brief method to remove a file system item located at path.
             * @param path the file system item
             *
             * When path refers to a directory, the directory must be empty
             * for the method to remove it.
             */
            void removeItemAtPath(const string_type &path) const;


            /**
             * @brief method for copying a file.
             * @param sourcePath the source file
             * @param destPath the path to the destination file.
             */
            void copyItemAtPathToPath(const string_type &sourcePath, const string_type &destPath) const;


            /**
             * @brief method for moving an item from one place to another in the file system.
             * @param sourcePath the location of the item
             * @param destPath the new location for the item.
             */
            void moveItemAtPathToPath(const string_type &sourcePath, const string_type &destPath) const;


            /**
             * @brief method to create a symbolic link to a file
             * @param linkPath the location for the new symbolic link
             * @param destPath the source item referred to by @e linkPath.
             */
            void createSymbolicLinkAtPathWithDestinationPath(const string_type &linkPath,
                                                             const string_type &destPath) const;


            /**
             * @brief method to create a hard link to a file
             * @param linkPath the location for the new link to the file
             * @param destPath the source item to use for the new link.
             */
            void createHardLinkAtPathWithDestinationPath(const string_type &linkPath,
                                                         const string_type &destPath) const;


            /**
             * @brief method to check if a filesystem entry is a link.
             * @param path the location of the item
             * @return true if the item is a link and false otherwise
             */
            bool itemAtPathIsLink(const string_type &path) const;

            /**
             * @brief method to check if a file exists at a path location
             * @param path the location
             * @return true if the item exists and is not a directory and false otherwise.
             */
            bool fileExistsAtPath(const string_type &path) const;


            /**
             * @brief method to check if a directory exists at a path location
             * @param path the location
             * @return true if the item exists and is a directory and false otherwise.
             */
            bool directoryExistsAtPath(const string_type &path) const;


            /**
             * @brief method to see if a file system entry exists at a path location
             * @param path the location
             * @return true if the item exists and false otherwise.
             */
            bool itemExistsAtPath(const string_type &path) const;


            /**
             * @brief method to return the properties of the file system object located at @e path.
             * @param path the location of the file system object
             * @return the properties for the item at @e path
             */
            file_properties_type propertiesForItemAtPath(const string_type &path) const;


            /**
             * @brief method to return the permissions of the file system object located at @e path
             * @param path the location of the file system object.
             * @return the permissions for the item at @e path.
             */
            file_permissions_type permissionsForItemAtPath(const string_type &path) const;


            /**
             * @brief method to se the permissions of the file system object located at @e path
             * @param path the location of the file system object.
             * @param p the new file permissions
             */
            void setPermissionsForItemAtPath(const string_type &path, const file_permissions_type &p) const;


            /**
             * @brief method to check if a file system item is readable
             * @param path
             * @return true if the process can read the item and false otherwise.
             */
            bool isReadableAtPath(const string_type &path) const;


            /**
             * @brief method to check if a file system item is writable.
             * @param path
             * @return true if the process can write to the item and false otherwise.
             */
            bool isWritableAtPath(const string_type &path) const;


            /**
             * @brief method to check if a file system item is executable.
             * @param path
             * @return true if the process can execute the item and false otherwise.
             */
            bool isExecutableAtPath(const string_type &path) const;


            /**
             * @brief method to check if the manager can remove a file system item.
             * @param path
             * @return true if the manager can remove the item and false otherwise.
             */
            bool isDeletableAtPath(const string_type &path) const;


            /**
             * @brief method to compare the contents of two file system items.
             * @param pathA the path to the first item
             * @param pathB the path to the second item
             * @return true if the two items contain the same contents and false otherwise.
             */
            bool contentsEqualAtPathAndPath(const string_type &pathA, const string_type &pathB) const;


            /**
             * @brief method to change the process' current working directory
             * @param path the new working directory
             */
            void changeCurrentWorkingDirectoryToPath(const string_type &path);


            /**
             * @brief method to get the process' current working directory.
             * @return the working directory
             */
            string_type currentWorkingDirectoryPath() const;


            /**
             * Method to get the size (in bytes) of the file item
             * @param path the location of the file
             * @return the size in bytes of the file
             */
            size_type sizeofFileAtPath(const string_type &path) const;


            /**
             * @brief method to get the base name of the item at path
             * @param path the location of the item
             * @return the basename.
             */
            string_type baseNameOfItemAtPath(const string_type &path) const;


            /**
             * @brief method to get the directory name of the item at path
             * @param path the location of the item
             * @return the directory name
             */
            string_type dirNameOfItemAtPath(const string_type &path) const;


            std::ostream &description(std::ostream &o) const;

            static string_type pathSeparator;
        };


        std::ostream &operator<<(std::ostream &o, const FileManager &m);

    }    // namespace Foundation

}    // namespace TF

#endif    // TFFILEMANAGER_HPP
