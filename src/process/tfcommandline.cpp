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

#include "tfcommandline.hpp"

namespace TF::Foundation
{
    static bool arg_is_quoted(const String & arg)
    {
        if (arg.empty())
        {
            return false;
        }

        if (arg[0] != '\'' && arg[0] != '\"')
        {
            return false;
        }

        return arg[0] == arg[arg.length() - 1];
    }

    String convert_args_to_command_line(int argc, const char ** argv)
    {
        static String::range_type not_found_range{};
        String collected_command_line{};

        for (int i = 0; i < argc; i++)
        {
            String arg{argv[i]};
            bool requires_quote_mark = false;
            auto range_of_space = arg.rangeOfString(" ");
            if (range_of_space != not_found_range && (! arg_is_quoted(arg)))
            {
                requires_quote_mark = true;
                collected_command_line += "'";
            }
            collected_command_line += arg;
            if (requires_quote_mark)
            {
                collected_command_line += "'";
            }
            if (i < (argc - 1))
            {
                collected_command_line += " ";
            }
        }

        return collected_command_line;
    }

    bool convert_command_line_to_vector(const String & command_line, std::vector<String> & list)
    {
        if (command_line.empty())
        {
            return false;
        }

        bool handling_a_quote{false};
        String arg_being_processed{};
        String::unicode_point_type quote_character;

        for (String::size_type i = 0; i < command_line.length(); i++)
        {
            switch (command_line[i])
            {
                case '\'':
                case '"':
                    if (handling_a_quote)
                    {
                        if (command_line[i] == quote_character)
                        {
                            handling_a_quote = false;
                        }
                        else
                        {
                            arg_being_processed += command_line[i];
                        }
                    }
                    else
                    {
                        handling_a_quote = true;
                        quote_character = command_line[i];
                    }
                    break;
                case ' ':
                    if (handling_a_quote)
                    {
                        arg_being_processed += " ";
                    }
                    else
                    {
                        list.emplace_back(arg_being_processed);
                        arg_being_processed = "";
                    }
                    break;
                default:
                    arg_being_processed += command_line[i];
            }
        }

        if (arg_being_processed.length() > 0)
        {
            list.emplace_back(arg_being_processed);
        }

        return true;
    }

} // namespace TF::Foundation
