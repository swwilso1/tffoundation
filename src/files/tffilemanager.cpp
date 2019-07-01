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

#include "tffilemanager.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

#include "tfenvironmentsettings.hpp"
#include "tfstring.hpp"

#define NEEDS_FSTREAM
#include "tfheaders.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {


        FileManager::string_array_type FileManager::subpathsOfDirectoryAtPath(const string_type &path) const
        {
            string_array_type subPaths;

            auto contentsArray = contentsOfDirectoryAtPath(path);

            for(auto &entry : contentsArray)
            {
                auto newPath = path + pathSeparator + entry;

                subPaths.push_back(newPath);

                if(directoryExistsAtPath(newPath))
                {
                    auto theseSubPaths = subpathsOfDirectoryAtPath(newPath);
                    for(auto &subEntry : theseSubPaths)
                        subPaths.push_back(subEntry);
                }
            }

            return subPaths;
        }


        FileManager::string_array_type FileManager::subpathsAtPath(const string_type &path) const
        {
            string_array_type subPaths;

            auto contentsArray = contentsOfDirectoryAtPath(path);

            for(auto &entry : contentsArray)
            {
                auto newPath = path + pathSeparator + entry;

                subPaths.push_back(newPath);
            }

            return subPaths;
        }


        void FileManager::createDirectoriesAtPath(const string_type &path) const
        {
            String fullPath(path);
            auto components = fullPath.split(pathSeparator);
            String subPath;

            for(auto &entry : components)
            {
                subPath += String(pathSeparator) + entry;
                if(!directoryExistsAtPath(subPath))
                    createDirectoryAtPath(subPath);
            }
        }


        void FileManager::copyItemAtPathToPath(const string_type &sourcePath, const string_type &destPath) const
        {
            auto sourceStr = sourcePath.cStr();
            auto destStr = destPath.cStr();
            std::ifstream inFile(sourceStr.get());
            std::ofstream outFile(destStr.get(), std::ios::out | std::ios::trunc);
            bool wroteBytes = false;

            if(!inFile)
                throw std::runtime_error("Unable to read from input file");

            if(!outFile)
                throw std::runtime_error("Unable to write to output file");

            char c;

            while(inFile.get(c))
            {
                outFile.put(c);
                if(!wroteBytes)
                    wroteBytes = true;
            }

            if(!wroteBytes)
            {
                // The source file was empty so we have to create an empty output file.
                createFileAtPath(destPath);
            }
        }


        FileManager::file_permissions_type FileManager::permissionsForItemAtPath(const string_type &path) const
        {
            auto properties = propertiesForItemAtPath(path);
            return properties.permission;
        }


        bool FileManager::isDeletableAtPath(const string_type &path) const
        {
            auto components = path.split(pathSeparator);

            string_type theDirPath;

            for(size_type i = 0; i < (components.size() - 1); i++)
            {
                theDirPath += pathSeparator + components[i];
            }

            if(isWritableAtPath(theDirPath) || isExecutableAtPath(theDirPath))
                return true;

            return false;
        }


        bool FileManager::contentsEqualAtPathAndPath(const string_type &pathA, const string_type &pathB) const
        {
            if(sizeofFileAtPath(pathA) != sizeofFileAtPath(pathB))
                return false;

            auto pathAStr = pathA.cStr();
            auto pathBStr = pathB.cStr();

            std::ifstream streamA(pathAStr.get());
            std::ifstream streamB(pathBStr.get());

            if(!streamA)
                throw std::runtime_error("Unable to read from file 1.");

            if(!streamB)
                throw std::runtime_error("Unable to read from file 2.");

            char a, b;

            while(streamA.get(a) && streamB.get(b))
            {
                if(a != b)
                    return false;
            }

            return true;
        }


        std::ostream &FileManager::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("FileManager");
                o << *formatter;
                delete formatter;
            }
            return o;
        }


        std::ostream &operator<<(std::ostream &o, const FileManager &m)
        {
            return m.description(o);
        }

    }    // namespace Foundation

}    // namespace TF
