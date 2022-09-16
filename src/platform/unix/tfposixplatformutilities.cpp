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
SOFTWARE.


******************************************************************************/

#define NEEDS_CHRONO
#include "tfheaders.hpp"
#include "tfposixplatformutilities.hpp"
#include "tfprocess.hpp"
#include "tfsleep.hpp"
#include "tfdata.hpp"
#include "tfenvironmentsettings.hpp"
#include "tffilemanager.hpp"

namespace TF::Foundation
{

    auto run_command(const String & command) -> String
    {
        Process p{};
        auto std_out_handle = p.handle_for_standard_out();

        p.launch(command);

        Data data_from_process{};

        while (p.is_alive())
        {
            auto data = std_out_handle.readToEndOfFile();
            if (data.length() > 0)
            {
                data_from_process.append(data);
            }

            Sleep(std::chrono::milliseconds{10});

            data = std_out_handle.readToEndOfFile();
            if (data.length() > 0)
            {
                data_from_process.append(data);
            }

            p.update_exit_status();
        }
        data_from_process.append(std_out_handle.readToEndOfFile());
        String result_string{data_from_process};
        if (result_string.last() == '\n')
        {
            return result_string.substringToIndex(result_string.length() - 1);
        }
        return result_string;
    }

    auto find_path_to_binary(const String & binary) -> std::optional<String>
    {
        FileManager manager{};
        EnvironmentSettings settings{};
        auto path = settings.getValueForVariable("PATH");
        auto path_array = path.split(":");

        for (auto & path_value : path_array)
        {
            auto full_path = path_value + FileManager::pathSeparator + binary;
            if (manager.fileExistsAtPath(full_path))
            {
                return {full_path};
            }
        }

        return {};
    }

} // namespace TF::Foundation
