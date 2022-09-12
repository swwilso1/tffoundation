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

class FileHandleTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        lorumIpsum = FileHandle::string_type(
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut "
            "labore et dolore magna aliqua. Eu mi bibendum neque egestas. Nisi vitae suscipit tellus mauris "
            "a diam maecenas sed enim. Sit amet cursus sit amet. Vehicula ipsum a arcu cursus vitae congue. "
            "Consectetur adipiscing elit duis tristique sollicitudin nibh sit. At tempor commodo ullamcorper"
            " a lacus vestibulum sed. Sapien eget mi proin sed libero enim sed. Lacinia quis vel eros donec "
            "ac odio. Tempor orci dapibus ultrices in iaculis nunc. Felis eget velit aliquet sagittis id.\n"
            "\n"
            "Et magnis dis parturient montes nascetur. Euismod elementum nisi quis eleifend quam adipiscing "
            "vitae proin sagittis. Amet nisl suscipit adipiscing bibendum est. Tristique senectus et netus "
            "et malesuada fames. Cras sed felis eget velit aliquet sagittis. Justo donec enim diam vulputate"
            " ut pharetra sit amet aliquam. Laoreet suspendisse interdum consectetur libero. Nunc mattis "
            "enim ut tellus elementum sagittis vitae et leo. Accumsan in nisl nisi scelerisque eu ultrices "
            "vitae. Montes nascetur ridiculus mus mauris vitae ultricies. Fermentum odio eu feugiat pretium "
            "nibh. Sit amet commodo nulla facilisi nullam vehicula ipsum a. Urna id volutpat lacus laoreet "
            "non. Donec ultrices tincidunt arcu non sodales neque. Viverra mauris in aliquam sem fringilla "
            "ut morbi tincidunt. Mus mauris vitae ultricies leo integer malesuada nunc vel risus. Tellus in "
            "metus vulputate eu scelerisque felis imperdiet.\n"
            "\n"
            "Viverra ipsum nunc aliquet bibendum enim facilisis. Sem viverra aliquet eget sit amet tellus "
            "cras adipiscing enim. Lacus luctus accumsan tortor posuere. Proin sagittis nisl rhoncus mattis "
            "rhoncus urna neque viverra justo. Gravida in fermentum et sollicitudin ac orci. Tortor vitae "
            "purus faucibus ornare suspendisse sed nisi. Augue interdum velit euismod in pellentesque massa."
            " Placerat in egestas erat imperdiet sed. Neque vitae tempus quam pellentesque nec nam aliquam "
            "sem. Commodo sed egestas egestas fringilla phasellus faucibus. Ut ornare lectus sit amet est "
            "placerat in. Ac turpis egestas integer eget aliquet nibh praesent tristique magna. Id volutpat "
            "lacus laoreet non curabitur gravida. Adipiscing enim eu turpis egestas.\n"
            "\n"
            "Etiam erat velit scelerisque in dictum non. Rutrum quisque non tellus orci ac auctor. Dui "
            "accumsan sit amet nulla facilisi morbi tempus iaculis. Morbi tincidunt augue interdum velit "
            "euismod in pellentesque massa. Risus nullam eget felis eget nunc lobortis mattis aliquam "
            "faucibus. Urna duis convallis convallis tellus id interdum velit. Imperdiet sed euismod nisi "
            "porta lorem mollis aliquam ut porttitor. Nullam ac tortor vitae purus faucibus ornare "
            "suspendisse. Leo integer malesuada nunc vel risus. Ac tincidunt vitae semper quis lectus "
            "nulla at volutpat. Accumsan sit amet nulla facilisi.\n"
            "\n"
            "Amet mattis vulputate enim nulla. Aliquam nulla facilisi cras fermentum odio eu. Sed id semper "
            "risus in hendrerit gravida rutrum. Viverra orci sagittis eu volutpat odio. Sit amet mattis "
            "vulputate enim. Enim eu turpis egestas pretium aenean pharetra magna ac placerat. Ac tortor "
            "dignissim convallis aenean et tortor at. Imperdiet sed euismod nisi porta lorem mollis. "
            "Aliquet lectus proin nibh nisl. Varius vel pharetra vel turpis nunc. Nunc sed id semper risus "
            "in hendrerit gravida rutrum. Gravida neque convallis a cras semper auctor neque vitae tempus. "
            "Scelerisque in dictum non consectetur.");

        temporaryDirectory = fileManager.temporaryDirectory();
    }

    static FileHandle::string_type lorumIpsum;

    static FileManager fileManager;

    static FileHandle::string_type temporaryDirectory;
};

FileHandle::string_type FileHandleTest::lorumIpsum;

FileManager FileHandleTest::fileManager;

FileHandle::string_type FileHandleTest::temporaryDirectory;

TEST_F(FileHandleTest, SimpleConstructorTest)
{
    FileHandle fh{};
}

TEST_F(FileHandleTest, SimpleFileForWritingTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile001.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);

    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    // We haven't used any Unicode characters in this test, so compare the FileManager's size of the file
    // with the length of the string.
    EXPECT_EQ(FileHandleTest::lorumIpsum.length(), FileHandleTest::fileManager.sizeofFileAtPath(fileName));

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, SimpleFileForReadingTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile002.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);

    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForReadingAtPath(fileName);

    FileHandle::data_type data = fh.readToEndOfFile();
    EXPECT_EQ(FileHandleTest::lorumIpsum.getAsData(), data);

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, ReadFileByLines)
{
    auto lorem_ipsum_split_by_newlines = FileHandleTest::lorumIpsum.split("\n");
    auto file_name = FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFileByLines.txt";

    auto write_fh = FileHandle::fileHandleForWritingAtPath(file_name);
    write_fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    write_fh.closeFile();

    auto read_fh = FileHandle::fileHandleForReadingAtPath(file_name);

    for (auto & line : lorem_ipsum_split_by_newlines)
    {
        auto read_line = read_fh.readLine();

        String compare_line{read_line};
        if (read_line.last() == '\n')
        {
            compare_line = read_line.substringToIndex(line.length());
        }

        EXPECT_EQ(line, compare_line);
    }
    read_fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(file_name))
    {
        FileHandleTest::fileManager.removeItemAtPath(file_name);
    }
}

TEST_F(FileHandleTest, ReadFileByLinesTillEnd)
{
    auto file_name = FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFileByLines.txt";

    auto write_fh = FileHandle::fileHandleForWritingAtPath(file_name);
    write_fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    write_fh.closeFile();

    auto read_fh = FileHandle::fileHandleForReadingAtPath(file_name);
    int lines{0};
    while (! read_fh.atEndOfFile())
    {
        auto read_line = read_fh.readLine();
        lines++;
    }
    read_fh.closeFile();

    EXPECT_EQ(lines, 9);

    if (FileHandleTest::fileManager.isDeletableAtPath(file_name))
    {
        FileHandleTest::fileManager.removeItemAtPath(file_name);
    }
}

TEST_F(FileHandleTest, ReadFileThatDoesNotExistTest)
{
    EXPECT_ANY_THROW(FileHandle fh = FileHandle::fileHandleForReadingAtPath("/foo/bar/bat.txt"));
}

TEST_F(FileHandleTest, StdinHandleTest)
{
    FileHandle fh = FileHandle::fileHandleWithStandardInput();
    EXPECT_EQ(fh.fileDescriptor(), 0);
}

TEST_F(FileHandleTest, StdoutHandleTest)
{
    FileHandle fh = FileHandle::fileHandleWithStandardOutput();
    EXPECT_EQ(fh.fileDescriptor(), 1);
}

TEST_F(FileHandleTest, StderrHandleTest)
{
    FileHandle fh = FileHandle::fileHandleWithStandardError();
    EXPECT_EQ(fh.fileDescriptor(), 2);
}

TEST_F(FileHandleTest, OffsetTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile003.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForReadingAtPath(fileName);

    EXPECT_EQ(fh.offsetInFile(), 0);

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, SeekToEndOfFileTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile004.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForReadingAtPath(fileName);

    fh.seekToEndOfFile();

    EXPECT_EQ(fh.offsetInFile(), FileHandleTest::lorumIpsum.length());

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, SeekToStartOfFileTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile009.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForReadingAtPath(fileName);

    fh.seekToStartOfFile();

    EXPECT_EQ(fh.offsetInFile(), 0);

    fh.seekToEndOfFile();

    EXPECT_EQ(fh.offsetInFile(), FileHandleTest::lorumIpsum.length());

    fh.seekToStartOfFile();

    EXPECT_EQ(fh.offsetInFile(), 0);

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
    {
        FileHandleTest::fileManager.removeItemAtPath(fileName);
    }
}

TEST_F(FileHandleTest, SeekToOffsetTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile005.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForReadingAtPath(fileName);

    fh.seekToFileOffset(500);

    FileHandle::data_type d = fh.readToEndOfFile();

    FileHandle::string_type subString = FileHandleTest::lorumIpsum.substringFromIndex(500);

    EXPECT_EQ(d, subString.getAsData());

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, TruncateFileTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile006.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());

    fh.truncateFileAtOffset(500);

    auto size = FileHandleTest::fileManager.sizeofFileAtPath(fileName);

    EXPECT_EQ(size, 500);

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, ReadFileBitsAtATimeTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile007.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForReadingAtPath(fileName);

    FileHandle::data_type d;

    while (d.length() != FileHandleTest::lorumIpsum.length())
    {
        auto readData = fh.readDataOfLength(10);
        d.append(readData);
    }

    FileHandle::string_type duplicate(d.bytes(), d.length());

    EXPECT_EQ(duplicate, FileHandleTest::lorumIpsum);

    fh.closeFile();

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}

TEST_F(FileHandleTest, AppendFileTest)
{
    FileHandle::string_type fileName =
        FileHandleTest::temporaryDirectory + FileManager::pathSeparator + "testFile008.txt";
    FileHandle fh = FileHandle::fileHandleForWritingAtPath(fileName);
    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    fh = FileHandle::fileHandleForAppendingAtPath(fileName);

    fh.writeData(FileHandleTest::lorumIpsum.getAsData());
    fh.closeFile();

    EXPECT_EQ(FileHandleTest::lorumIpsum.length() * 2, FileHandleTest::fileManager.sizeofFileAtPath(fileName));

    if (FileHandleTest::fileManager.isDeletableAtPath(fileName))
        FileHandleTest::fileManager.removeItemAtPath(fileName);
}