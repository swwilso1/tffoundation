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


#include "TFFoundation.hpp"
#include "gtest/gtest.h"


using namespace TF::Foundation;

TEST(FileManagerTest, TemporaryDirectoryTest)
{
    FileManager fm;
    EXPECT_EQ(fm.temporaryDirectory(), "/tmp");
}


TEST(FileManagerTest, HomeDirectoryForCurrentUserTest)
{
    FileManager fm;
    EnvironmentSettings es;
    auto currentUser = es.getValueForVariable("USER");
    auto userHomeDir = fm.homeDirectoryForUser(currentUser);
    EXPECT_EQ(fm.homeDirectoryForCurrentUser(), userHomeDir);
}


TEST(FileManagerTest, HomeDirectoryForUserTest)
{
    FileManager fm;
#if defined(TFLINUX)
    EXPECT_EQ(fm.homeDirectoryForUser("root"), "/root");
#elif defined(TFMACOS)
    EXPECT_EQ(fm.homeDirectoryForUser("root"), "/var/root");
#endif
}


TEST(FileManagerTest, ContentsOfDirectoryAtPathTest)
{
    FileManager fm;
    auto contents = fm.contentsOfDirectoryAtPath("/tmp");
    EXPECT_NE(contents.size(), 0);
}


TEST(FileManagerTest, SubpathsForDirectoryAtPathTest)
{
    FileManager fm;
    auto subPaths = fm.subpathsOfDirectoryAtPath("/tmp");
    EXPECT_NE(subPaths.size(), 0);
}


TEST(FileManagerTest, SubpathsAtPathTest)
{
    FileManager fm;
    auto subPaths = fm.subpathsAtPath("/tmp");
    EXPECT_NE(subPaths.size(), 0);
}


TEST(FileManagerTest, CreateDirectoryAtPathTest)
{
    FileManager fm;
    fm.createDirectoryAtPath("/tmp/foo");
    EXPECT_TRUE(fm.directoryExistsAtPath("/tmp/foo"));
    fm.removeItemAtPath("/tmp/foo");
}


TEST(FileManagerTest, CreateDirectoriesAtPathTest)
{
    FileManager fm;
    fm.createDirectoriesAtPath("/tmp/foo/bar/bat");
    EXPECT_TRUE(fm.directoryExistsAtPath("/tmp/foo/bar/bat"));
    fm.removeItemAtPath("/tmp/foo/bar/bat");
    fm.removeItemAtPath("/tmp/foo/bar");
    fm.removeItemAtPath("/tmp/foo");
}


TEST(FileManagerTest, CreateFileAtPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/foo");
    EXPECT_TRUE(fm.fileExistsAtPath("/tmp/foo"));
    fm.removeItemAtPath("/tmp/foo");
}


TEST(FileManagerTest, RemoveItemAtPathTest)
{
    FileManager fm;
    fm.createDirectoryAtPath("/tmp/foo");
    fm.removeItemAtPath("/tmp/foo");
    EXPECT_FALSE(fm.directoryExistsAtPath("/tmp/foo"));

    fm.createFileAtPath("/tmp/foo");
    fm.removeItemAtPath("/tmp/foo");
    EXPECT_FALSE(fm.fileExistsAtPath("/tmp/foo"));
}


TEST(FileManagerTest, CopyItemAtPathToPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/fileA");
    fm.copyItemAtPathToPath("/tmp/fileA", "/tmp/fileB");
    EXPECT_TRUE(fm.contentsEqualAtPathAndPath("/tmp/fileA", "/tmp/fileB"));
    fm.removeItemAtPath("/tmp/fileB");
    fm.removeItemAtPath("/tmp/fileA");
}


TEST(FileManagerTest, MoveItemAtPathToPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/mvA");
    fm.moveItemAtPathToPath("/tmp/mvA", "/tmp/mvB");
    EXPECT_FALSE(fm.fileExistsAtPath("/tmp/mvA"));
    EXPECT_TRUE(fm.fileExistsAtPath("/tmp/mvB"));
    fm.removeItemAtPath("/tmp/mvB");
}


TEST(FileManagerTest, CreateSymbolicLinkAtPathWithDestinationPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/destinationPath");
    fm.createSymbolicLinkAtPathWithDestinationPath("/tmp/linkPath", "/tmp/destinationPath");
    EXPECT_TRUE(fm.fileExistsAtPath("/tmp/linkPath"));
    fm.removeItemAtPath("/tmp/linkPath");
    fm.removeItemAtPath("/tmp/destinationPath");
}


TEST(FileManagerTest, CreateHardLinkAtPathWithDestinationPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/destinationPath");
    fm.createHardLinkAtPathWithDestinationPath("/tmp/linkPath", "/tmp/destinationPath");
    EXPECT_TRUE(fm.fileExistsAtPath("/tmp/linkPath"));
    fm.removeItemAtPath("/tmp/linkPath");
    fm.removeItemAtPath("/tmp/destinationPath");
}


TEST(FileManagerTest, IsReadableAtPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/readFile");
    EXPECT_TRUE(fm.isReadableAtPath("/tmp/readFile"));
    fm.removeItemAtPath("/tmp/readFile");
}


TEST(FileManagerTest, IsWriteableAtPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/writeFile");
    EXPECT_TRUE(fm.isWritableAtPath("/tmp/writeFile"));
    fm.removeItemAtPath("/tmp/writeFile");
}


TEST(FileManagerTest, IsDeletableAtPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/deleteFile");
    EXPECT_TRUE(fm.isDeletableAtPath("/tmp/deleteFile"));
    fm.removeItemAtPath("/tmp/deleteFile");
}


TEST(FileManagerTest, IsExecutableAtPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/executeFile");
    EXPECT_FALSE(fm.isExecutableAtPath("/tmp/executeFile"));
    fm.removeItemAtPath("/tmp/executeFile");
}


TEST(FileManagerTest, ChangeCurrentWorkingDirectoryToPathTest)
{
    FileManager fm;
#if defined(TFLINUX)
    fm.changeCurrentWorkingDirectoryToPath("/tmp");
    EXPECT_EQ(fm.currentWorkingDirectoryPath(), "/tmp");
#elif defined(TFMACOS)
    fm.changeCurrentWorkingDirectoryToPath("/private/tmp");
    EXPECT_EQ(fm.currentWorkingDirectoryPath(), "/private/tmp");
#endif
}


TEST(FileManagerTest, SizeofFileAtPathTest)
{
    FileManager fm;
    fm.createFileAtPath("/tmp/sizeFile");
    EXPECT_EQ(fm.sizeofFileAtPath("/tmp/sizeFile"), 0);
    fm.removeItemAtPath("/tmp/sizeFile");
}