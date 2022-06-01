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

// The contents of this file are currently quite Unix specific.

#include <sys/time.h>
#define NEEDS_IOSTREAM
#define NEEDS_IOMANIP
#define NEEDS_FSTREAM
#define NEEDS_TIME
#include "tfheaders.hpp"
#include "tflog.hpp"
#include "tfdateclocks.hpp"

namespace TF
{

    namespace Foundation
    {

        class DeleterFunctor
        {
        public:
            explicit DeleterFunctor(bool del = true) : m_delete_pointer{del} {}

            virtual ~DeleterFunctor() {}

            virtual void operator()(std::unique_ptr<Logger::stream_type>::pointer p)
            {
                if (m_delete_pointer)
                {
                    std::default_delete<Logger::stream_type> deleter;
                    deleter(p);
                }
            }

        private:
            bool m_delete_pointer{false};
        };

        std::ostream & operator<<(std::ostream & o, const LogPriority & p)
        {
            switch (p)
            {
                case LogPriority::Critical:
                    o << "CRITICAL";
                    break;
                case LogPriority::Error:
                    o << "ERROR";
                    break;
                case LogPriority::Fine:
                    o << "FINE";
                    break;
                case LogPriority::Finer:
                    o << "FINER";
                    break;
                case LogPriority::Finest:
                    o << "FINEST";
                    break;
                case LogPriority::Info:
                    o << "INFO";
                    break;
                case LogPriority::Warning:
                    o << "WARNING";
                    break;
            }

            return o;
        }

        Logger::priority_type Logger::defaultPriority = LogPriority::Info;

        Logger::file_map_type Logger::logFiles;

        Logger::~Logger()
        {
            if (thePriority >= defaultPriority)
            {
                std::ostringstream preamble;
                std::ostringstream priority;
                priority << '[' << thePriority << ']';
                preamble << calculateTimeForPreamble() << " ";
                preamble << std::setw(10) << std::right << priority.str();

                for (auto & member : logFiles)
                {
                    try
                    {
                        *member.second << preamble.str() << " ";

                        *member.second << logMessage.str() << std::endl << std::flush;
                    }
                    catch (...)
                    {
                        ;
                    }
                }
            }
        }

        void Logger::logToFileAtPath(const string_type & path)
        {
            auto pathStr = path.cStr();
            auto theStream = unique_ptr_type(new std::fstream(pathStr.get(), std::ios::out | std::ios::app));
            if (dynamic_cast<std::fstream *>(theStream.get())->is_open())
            {
                logFiles.insert(std::make_pair(path, std::move(theStream)));
            }
        }

        void Logger::logToFileAtPath(const char * path)
        {
            string_type thePath(path);
            logToFileAtPath(thePath);
        }

        void Logger::closeFiles()
        {
            for (auto & member : logFiles)
            {
                if (member.first != "stdout" && member.first != "stderr")
                {
                    dynamic_cast<std::fstream *>(member.second.get())->close();
                }
            }

            logFiles.clear();
        }

        void Logger::logToStdOutput()
        {
            if (logFiles.count("stdout") == 0)
            {
                auto cout_wrapper = unique_ptr_type(&std::cout, DeleterFunctor(false));
                logFiles.insert(std::make_pair(string_type("stdout"), std::move(cout_wrapper)));
            }
        }

        void Logger::logToStdError()
        {
            if (logFiles.count("stderr") == 0)
            {
                auto cerr_wrapper = unique_ptr_type(&std::cerr, DeleterFunctor(false));
                logFiles.insert(std::make_pair(string_type("stderr"), std::move(cerr_wrapper)));
            }
        }

        Logger::string_type Logger::calculateTimeForPreamble()
        {
            SystemDate now;
            SystemDateFormatter formatter("yyyy-MM-dd HH:mm:ss:SSSS");

            auto theFormattedDate = formatter.stringFromDate(now);

            return theFormattedDate;
        }

    } // namespace Foundation

} // namespace TF
