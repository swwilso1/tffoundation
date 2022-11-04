/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

TEST(DLLTest, BasicTest)
{
    using function_type = int (*)();
    FileManager file_manager{};
    DLL dll_test_lib{};

    auto current_dir = file_manager.currentWorkingDirectoryPath();
    auto path_to_dll = current_dir + FileManager::pathSeparator + "libdlltest.so";

    EXPECT_NO_THROW(dll_test_lib = DLLLoader::load_library(path_to_dll););

    DLL::function_handle test_function_1{};
    EXPECT_NO_THROW(test_function_1 = dll_test_lib.load_function("tf1"););
    EXPECT_NE(test_function_1, nullptr);
    auto fp1 = reinterpret_cast<function_type>(test_function_1);
    EXPECT_EQ(fp1(), 1);

    DLL::function_handle test_function_2{};
    EXPECT_NO_THROW(test_function_2 = dll_test_lib.load_function("tf2"););
    EXPECT_NE(test_function_2, nullptr);
    auto fp2 = reinterpret_cast<function_type>(test_function_2);
    EXPECT_EQ(fp2(), 2);

    EXPECT_NO_THROW(dll_test_lib.close(););
}
