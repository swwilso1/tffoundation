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

#include "tffilemanager.hpp"

namespace TF
{

    namespace Foundation
    {
        FileManager::string_type FileManager::homeDirectoryForUser(const string_type &user) const
        {
            String passwdFileContents;

            std::ifstream passwdFile("/etc/passwd");
            if(! passwdFile)
            {
                throw std::runtime_error("Unable to open /etc/passwd");
            }

            char c;
            while(passwdFile.get(c))
            {
                passwdFileContents = passwdFileContents.stringByAppendingFormat("%c", c);
            }

            auto linesFromFile = passwdFileContents.split("\n");

            for(auto &line : linesFromFile)
            {
                auto elementsFromLine = line.split(":");

                if(elementsFromLine.size() == 0)
                    continue;

                if(elementsFromLine[0] == user)
                    return elementsFromLine[5].stlString();
            }

            throw std::runtime_error("User not found on system");
        }

    }

}


