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
#include "tfstring.hpp"

#define NEEDS_FSTREAM
#include "tfheaders.hpp"
#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {
        void FileManager::forcefullyRemoveItemAtPath(const string_type & path) const
        {
            walkItemsAtPathRecursively(path, [this](const string_type & subitem) -> bool {
                this->removeItemAtPath(subitem);
                return true;
            });
            removeItemAtPath(path);
        }

        FileManager::string_array_type FileManager::subpathsOfDirectoryAtPath(const string_type & path) const
        {
            string_array_type subPaths;

            auto contentsArray = contentsOfDirectoryAtPath(path);

            for (auto & entry : contentsArray)
            {
                auto newPath = path + pathSeparator + entry;

                subPaths.push_back(newPath);

                if (directoryExistsAtPath(newPath))
                {
                    auto theseSubPaths = subpathsOfDirectoryAtPath(newPath);
                    for (auto & subEntry : theseSubPaths)
                        subPaths.push_back(subEntry);
                }
            }

            return subPaths;
        }

        FileManager::string_array_type FileManager::subpathsAtPath(const string_type & path) const
        {
            string_array_type subPaths;

            auto contentsArray = contentsOfDirectoryAtPath(path);

            for (auto & entry : contentsArray)
            {
                auto newPath = path + pathSeparator + entry;

                subPaths.push_back(newPath);
            }

            return subPaths;
        }

        void FileManager::createDirectoriesAtPath(const string_type & path) const
        {
            if (path.contains(pathSeparator))
            {
                auto components = path.split(pathSeparator);
                String subPath{};
                if (path.substringToIndex(pathSeparator.length()) == pathSeparator)
                {
                    for (auto & entry : components)
                    {
                        if (entry.empty())
                        {
                            continue;
                        }

                        subPath += pathSeparator + entry;
                        if (! directoryExistsAtPath(subPath))
                        {
                            createDirectoryAtPath(subPath);
                        }
                    }
                }
                else
                {
                    auto components_size = components.size();
                    if (components_size > 0)
                    {
                        subPath = components[0];

                        if (! directoryExistsAtPath(subPath))
                        {
                            createDirectoryAtPath(subPath);
                        }

                        for (decltype(components)::size_type i = 1; i < components_size; i++)
                        {
                            subPath += pathSeparator + components[i];
                            if (! directoryExistsAtPath(subPath))
                            {
                                createDirectoryAtPath(subPath);
                            }
                        }
                    }
                }
            }
            else
            {
                if (! directoryExistsAtPath(path))
                {
                    createDirectoryAtPath(path);
                }
            }
        }

        void FileManager::copyItemAtPathToPath(const string_type & sourcePath, const string_type & destPath) const
        {
            auto sourceStr = sourcePath.cStr();
            auto destStr = destPath.cStr();
            std::ifstream inFile(sourceStr.get());
            std::ofstream outFile(destStr.get(), std::ios::out | std::ios::trunc);
            bool wroteBytes = false;

            if (! inFile)
                throw std::runtime_error("Unable to read from input file");

            if (! outFile)
                throw std::runtime_error("Unable to write to output file");

            char c;

            while (inFile.get(c))
            {
                outFile.put(c);
                if (! wroteBytes)
                    wroteBytes = true;
            }

            if (! wroteBytes)
            {
                // The source file was empty, so we have to create an empty output file.
                createFileAtPath(destPath);
            }
        }

        FileManager::file_permissions_type FileManager::permissionsForItemAtPath(const string_type & path) const
        {
            auto properties = propertiesForItemAtPath(path);
            return properties.permission;
        }

        bool FileManager::isDeletableAtPath(const string_type & path) const
        {
            auto components = path.split(pathSeparator);

            string_type theDirPath;

            for (size_type i = 0; i < (components.size() - 1); i++)
            {
                theDirPath += pathSeparator + components[i];
            }

            if (isWritableAtPath(theDirPath) || isExecutableAtPath(theDirPath))
                return true;

            return false;
        }

        auto FileManager::extensionOfItemAtPath(const string_type & path) const -> string_type
        {
            auto path_separator_ranges = path.rangesOfString(pathSeparator);
            auto dot_character_ranges = path.rangesOfString(".");

            if (dot_character_ranges.empty())
            {
                return {};
            }

            if (path_separator_ranges.empty())
            {
                auto & last_range = dot_character_ranges[dot_character_ranges.size() - 1];
                return path.substringFromIndex(last_range.position + last_range.length);
            }

            auto & last_range = dot_character_ranges[dot_character_ranges.size() - 1];
            auto & last_separator_range = path_separator_ranges[path_separator_ranges.size() - 1];

            if (last_range.position > (last_separator_range.position + last_separator_range.length))
            {
                return path.substringFromIndex(last_range.position + last_range.length);
            }

            return {};
        }

        bool FileManager::contentsEqualAtPathAndPath(const string_type & pathA, const string_type & pathB) const
        {
            if (sizeofFileAtPath(pathA) != sizeofFileAtPath(pathB))
                return false;

            auto pathAStr = pathA.cStr();
            auto pathBStr = pathB.cStr();

            std::ifstream streamA(pathAStr.get());
            std::ifstream streamB(pathBStr.get());

            if (! streamA)
                throw std::runtime_error("Unable to read from file 1.");

            if (! streamB)
                throw std::runtime_error("Unable to read from file 2.");

            char a, b;

            while (streamA.get(a) && streamB.get(b))
            {
                if (a != b)
                    return false;
            }

            return true;
        }

        void FileManager::walkItemsAtPathRecursively(const string_type & path,
                                                     const std::function<bool(const string_type & path)> & callback,
                                                     bool * keep_running) const
        {
            walkItemsAtPath(false, path, callback, keep_running);
        }

        void FileManager::walkItemsAtPath(bool top_down, const string_type & path,
                                          const std::function<bool(const string_type & path)> & callback,
                                          bool * keep_running) const
        {
            bool my_keep_running = true;
            if (keep_running == nullptr)
            {
                keep_running = &my_keep_running;
            }

            if (! itemExistsAtPath(path))
            {
                return;
            }

            if (! directoryExistsAtPath(path))
            {
                *keep_running = callback(path);
                return;
            }

            auto contents = contentsOfDirectoryAtPath(path);

            if (top_down)
            {
                string_array_type directories{};
                for (auto & item : contents)
                {
                    auto full_path = path + pathSeparator + item;
                    *keep_running = callback(full_path);
                    if (! *keep_running)
                    {
                        return;
                    }
                    if (directoryExistsAtPath(full_path))
                    {
                        directories.emplace_back(full_path);
                    }
                }

                for (auto & item : directories)
                {
                    walkItemsAtPath(top_down, item, callback, keep_running);
                    if (! *keep_running)
                    {
                        return;
                    }
                }
            }
            else
            {
                string_array_type files{};
                for (auto & item : contents)
                {
                    auto full_path = path + pathSeparator + item;
                    if (! directoryExistsAtPath(full_path))
                    {
                        files.emplace_back(full_path);
                        continue;
                    }

                    walkItemsAtPath(top_down, full_path, callback, keep_running);
                    if (! *keep_running)
                    {
                        return;
                    }
                    *keep_running = callback(full_path);
                    if (! *keep_running)
                    {
                        return;
                    }
                }

                for (auto & item : files)
                {
                    *keep_running = callback(item);
                    if (! *keep_running)
                    {
                        return;
                    }
                }
            }
        }

        std::ostream & FileManager::description(std::ostream & o) const
        {
            ClassFormatter * formatter = FormatterFactory::getFormatter();
            if (formatter != nullptr)
            {
                formatter->setClassName("FileManager");
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        std::ostream & operator<<(std::ostream & o, const FileManager & m)
        {
            return m.description(o);
        }

    } // namespace Foundation

} // namespace TF
