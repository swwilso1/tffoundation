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

#include "tffilemanager.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

#include "tfenvironmentsettings.hpp"

namespace TF
{

    namespace Foundation
    {

        FileManager::string_type FileManager::pathSeparator = "/";

        FileManager::string_type FileManager::temporaryDirectory() const
        {
            EnvironmentSettings es;

            if(es.hasVariable("TEMP"))
                return es.getValueForVariable("TEMP");
            else if(es.hasVariable("TMP"))
                return es.getValueForVariable("TMP");
            else if(es.hasVariable("TEMP_DIR"))
                return es.getValueForVariable("TEMP_DIR");
            else if(es.hasVariable("TMP_DIR"))
                return es.getValueForVariable("TMP_DIR");

            return "/tmp";
        }


        FileManager::string_type FileManager::homeDirectoryForCurrentUser() const
        {
            EnvironmentSettings es;
            return es.getValueForVariable("HOME");
        }


        FileManager::string_array_type FileManager::contentsOfDirectoryAtPath(const string_type &path) const
        {
            string_array_type contentsArray;

            struct dirent *dirEntry;
            DIR *opaqueDirHandle;

            auto pathStr = path.cStr();

            opaqueDirHandle = opendir(pathStr.get());
            if(opaqueDirHandle == nullptr)
                return contentsArray;

            while((dirEntry = readdir(opaqueDirHandle)) != nullptr)
            {
                string_type entryName(dirEntry->d_name);
                if(entryName == "." || entryName == "..")
                    continue;
                contentsArray.push_back(entryName);
            }

            closedir(opaqueDirHandle);

            return contentsArray;
        }


        void FileManager::createDirectoryAtPath(const string_type &path) const
        {
            auto pathStr = path.cStr();
            auto result = mkdir(pathStr.get(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
            if(result < 0)
                throw std::runtime_error("Unable to create directory");
        }


        void FileManager::createFileAtPath(const string_type &path) const
        {
            auto pathStr = path.cStr();
            if(!fileExistsAtPath(path) && !directoryExistsAtPath(path))
            {
                auto result = open(pathStr.get(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                if(result < 0)
                    throw std::runtime_error("Unable to create file");
                close(result);
            }
        }


        void FileManager::removeItemAtPath(const string_type &path) const
        {
            auto pathStr = path.cStr();
            if(directoryExistsAtPath(path))
            {
                auto result = rmdir(pathStr.get());
                if(result < 0)
                    throw std::runtime_error("Unable to remove directory");
            }
            else if(fileExistsAtPath(path))
            {
                auto result = unlink(pathStr.get());
                if(result < 0)
                    throw std::runtime_error("Unable to remove file");
            }
        }

        void FileManager::moveItemAtPathToPath(const string_type &sourcePath, const string_type &destPath) const
        {
            auto sourceStr = sourcePath.cStr();
            auto destStr = destPath.cStr();
            auto result = rename(sourceStr.get(), destStr.get());
            if(result < 0)
                throw std::runtime_error("Unable to move entry to new location");
        }


        void FileManager::createSymbolicLinkAtPathWithDestinationPath(const string_type &linkPath,
                                                                      const string_type &destPath) const
        {
            auto linkStr = linkPath.cStr();
            auto destStr = destPath.cStr();
            auto result = symlink(destStr.get(), linkStr.get());
            if(result < 0)
                throw std::runtime_error("Unable to create symbolic link");
        }


        void FileManager::createHardLinkAtPathWithDestinationPath(const string_type &linkPath,
                                                                  const string_type &destPath) const
        {
            auto linkStr = linkPath.cStr();
            auto destStr = destPath.cStr();
            auto result = link(destStr.get(), linkStr.get());
            if(result < 0)
                throw std::runtime_error("Unable to create hard link");
        }


        bool FileManager::itemAtPathIsLink(const string_type &path) const
        {
            struct stat item_stats;

            auto lstat_api_result = lstat(path.stlString().c_str(), &item_stats);
            if(lstat_api_result == 0)
            {
                if((item_stats.st_mode & S_IFMT) == S_IFLNK)
                {
                    return true;
                }
            }

            return false;
        }


        bool FileManager::fileExistsAtPath(const string_type &path) const
        {
            struct stat pathData;

            auto pathStr = path.cStr();

            auto result = lstat(pathStr.get(), &pathData);
            if(result < 0)
                return false;

            if(S_ISREG(pathData.st_mode) || S_ISCHR(pathData.st_mode) || S_ISBLK(pathData.st_mode) ||
               S_ISFIFO(pathData.st_mode) || S_ISLNK(pathData.st_mode) || S_ISSOCK(pathData.st_mode))
                return true;

            return false;
        }


        bool FileManager::directoryExistsAtPath(const string_type &path) const
        {
            struct stat pathData;

            auto pathStr = path.cStr();

            auto result = lstat(pathStr.get(), &pathData);

            if(result < 0)
                return false;    // probably not a standard file or directory.

            if(S_ISDIR(pathData.st_mode))
                return true;

            if(S_ISLNK(pathData.st_mode))
            {
                struct stat linkData;
                auto statResult = stat(pathStr.get(), &linkData);

                if(statResult < 0)
                    return false;

                if(S_ISDIR(linkData.st_mode))
                    return true;
            }

            return false;
        }


        FileManager::file_properties_type FileManager::propertiesForItemAtPath(const string_type &path) const
        {
            file_properties_type theProperties;
            struct stat pathData;

            auto pathStr = path.cStr();

            auto result = lstat(pathStr.get(), &pathData);
            if(result < 0)
                throw std::runtime_error("Unable to query file system object");

            theProperties.size = pathData.st_size;

            if(S_ISREG(pathData.st_mode))
                theProperties.type = FileType::Regular;
            else if(S_ISDIR(pathData.st_mode))
                theProperties.type = FileType::Directory;
            else if(S_ISCHR(pathData.st_mode))
                theProperties.type = FileType::CharacterSpecial;
            else if(S_ISBLK(pathData.st_mode))
                theProperties.type = FileType::Block;
            else if(S_ISFIFO(pathData.st_mode))
                theProperties.type = FileType::Pipe;
            else if(S_ISLNK(pathData.st_mode))
                theProperties.type = FileType::Link;
            else if(S_ISSOCK(pathData.st_mode))
                theProperties.type = FileType::Socket;
            else if(S_TYPEISMQ(&pathData))
                theProperties.type = FileType::MessageQueue;
            else if(S_TYPEISSEM(&pathData))
                theProperties.type = FileType::Semaphore;
            else if(S_TYPEISSEM(&pathData))
                theProperties.type = FileType::SharedMemory;

            if((pathData.st_mode & S_IRUSR) != 0)
                theProperties.permission.setUserReadPermission(true);
            if((pathData.st_mode & S_IWUSR) != 0)
                theProperties.permission.setUserWritePermission(true);
            if((pathData.st_mode & S_IXUSR) != 0)
                theProperties.permission.setUserExecutePermission(true);
            if((pathData.st_mode & S_IRGRP) != 0)
                theProperties.permission.setGroupReadPermission(true);
            if((pathData.st_mode & S_IWGRP) != 0)
                theProperties.permission.setGroupWritePermission(true);
            if((pathData.st_mode & S_IXGRP) != 0)
                theProperties.permission.setGroupExecutePermission(true);
            if((pathData.st_mode & S_IROTH) != 0)
                theProperties.permission.setOtherReadPermission(true);
            if((pathData.st_mode & S_IWOTH) != 0)
                theProperties.permission.setOtherWritePermission(true);
            if((pathData.st_mode & S_IXOTH) != 0)
                theProperties.permission.setOtherExecutePermission(true);
            if((pathData.st_mode & S_ISVTX) != 0)
                theProperties.permission.setStickyBit(true);
            if((pathData.st_mode & S_ISUID) != 0)
                theProperties.permission.setSetUserID(true);
            if((pathData.st_mode & S_ISGID) != 0)
                theProperties.permission.setSetGroupID(true);

            theProperties.userID = static_cast<int>(pathData.st_uid);
            theProperties.groupID = static_cast<int>(pathData.st_gid);


            if(theProperties.type == FileType::Link && theProperties.size > 0)
            {
                // We want to read the target of the link.
                char buffer[theProperties.size + 1];

                auto result = readlink(pathStr.get(), buffer, theProperties.size);

                if(result < 0)
                    throw std::runtime_error("Unable to read link target");

                buffer[theProperties.size] = '\0';

                theProperties.linkTarget = buffer;
            }


            return theProperties;
        }


        void FileManager::setPermissionsForItemAtPath(const string_type &path, const file_permissions_type &p) const
        {
            mode_t newMode = 0;

            if(p.hasUserReadPermission())
                newMode |= S_IRUSR;
            if(p.hasUserWritePermission())
                newMode |= S_IWUSR;
            if(p.hasSetUserID())
                newMode |= S_ISUID;
            else if(p.hasUserExecutePermission())
                newMode |= S_IXUSR;
            if(p.hasGroupReadPermission())
                newMode |= S_IRGRP;
            if(p.hasGroupWritePermission())
                newMode |= S_IWGRP;
            if(p.hasSetGroupID())
                newMode |= S_ISGID;
            else if(p.hasGroupExecutePermission())
                newMode |= S_IXGRP;
            if(p.hasOtherReadPermission())
                newMode |= S_IROTH;
            if(p.hasOtherWritePermission())
                newMode |= S_IWOTH;
            if(p.hasOtherExecutePermission())
                newMode |= S_IXOTH;

            // Disable the umask so we can get an exact permissions change.
            auto origUmask = umask(0);

            auto pathStr = path.cStr();

            auto result = chmod(pathStr.get(), newMode);
            if(result < 0)
                throw std::runtime_error("Unable to change permissions on file system object");

            // Re-enable the umask.
            umask(origUmask);
        }


        bool FileManager::isReadableAtPath(const string_type &path) const
        {
            struct stat pathData;
            auto pathStr = path.cStr();
            auto result = lstat(pathStr.get(), &pathData);
            if(result < 0)
                throw std::runtime_error("Unable to query file");

            auto effectiveUserID = geteuid();
            auto effectiveGroupID = getegid();

            if(pathData.st_uid == effectiveUserID)
            {
                if((pathData.st_mode & S_IRUSR) != 0)
                    return true;
                return false;
            }
            else if(pathData.st_gid == effectiveGroupID)
            {
                if((pathData.st_mode & S_IRGRP) != 0)
                    return true;
                return false;
            }

            // Check the OTH permission bits.
            if((pathData.st_mode & S_IROTH) != 0)
                return true;

            return false;
        }


        bool FileManager::isWritableAtPath(const string_type &path) const
        {
            struct stat pathData;
            auto pathStr = path.cStr();
            auto result = lstat(pathStr.get(), &pathData);
            if(result < 0)
                throw std::runtime_error("Unable to query file");

            auto effectiveUserID = geteuid();
            auto effectiveGroupID = getegid();

            if(pathData.st_uid == effectiveUserID)
            {
                if((pathData.st_mode & S_IWUSR) != 0)
                    return true;
                return false;
            }
            else if(pathData.st_gid == effectiveGroupID)
            {
                if((pathData.st_mode & S_IWGRP) != 0)
                    return true;
                return false;
            }

            // Check the OTH permission bits.
            if((pathData.st_mode & S_IWOTH) != 0)
                return true;

            return false;
        }


        bool FileManager::isExecutableAtPath(const string_type &path) const
        {
            struct stat pathData;
            auto pathStr = path.cStr();
            auto result = lstat(pathStr.get(), &pathData);
            if(result < 0)
                throw std::runtime_error("Unable to query file");

            auto effectiveUserID = geteuid();
            auto effectiveGroupID = getegid();

            if(pathData.st_uid == effectiveUserID)
            {
                if((pathData.st_mode & S_IXUSR) != 0)
                    return true;
                return false;
            }
            else if(pathData.st_gid == effectiveGroupID)
            {
                if((pathData.st_mode & S_IXGRP) != 0)
                    return true;
                return false;
            }

            // Check the OTH permission bits.
            if((pathData.st_mode & S_IXOTH) != 0)
                return true;

            return false;
        }


        void FileManager::changeCurrentWorkingDirectoryToPath(const string_type &path)
        {
            auto pathStr = path.cStr();
            auto result = chdir(pathStr.get());
            if(result < 0)
                throw std::runtime_error("Unable to change current working directory");
        }


        FileManager::string_type FileManager::currentWorkingDirectoryPath() const
        {
            size_type i = 1024;

            for(int j = 0; j < 10; j++)
            {
                auto tmp = new char[i];
                auto buffer = getcwd(tmp, i);
                if(buffer == nullptr)
                {
                    delete[] tmp;
                    i *= 2;
                }
                else
                {
                    string_type directory(buffer);
                    delete[] tmp;
                    return directory;
                }
            }

            throw std::runtime_error("Unable to retrieve current working directory");
        }


        FileManager::size_type FileManager::sizeofFileAtPath(const string_type &path) const
        {
            struct stat info;

            auto pathStr = path.cStr();

            auto result = stat(pathStr.get(), &info);

            if(result < 0)
                throw std::runtime_error("Unable to get file size");

            return info.st_size;
        }
    }    // namespace Foundation
}    // namespace TF
