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

#ifndef TFLOG_HPP
#define TFLOG_HPP

#define NEEDS_STRING
#define NEEDS_SSTREAM
#define NEEDS_OSTREAM
#define NEEDS_MAP
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{
	
	namespace Foundation
	{

		enum class LogPriority
		{
            Finest = 0,
            Finer,
            Fine,
            Info,
            Warning,
            Error,
            Critical
		};

        std::ostream& operator<<(std::ostream &o, const LogPriority &p);


		class Logger : public AllocatorInterface
		{
		public:
		
			using priority_type = LogPriority;
			
			using string_type = std::string;
		
			explicit Logger(const priority_type &p) : thePriority(p) {};
			
			~Logger() override;
			
			void log(const string_type &format)
			{
				logMessage << format;
			}

			template<class Arg, class ...Args>
			void log(const string_type &format, Arg value, Args ... args)
			{
				
				for(string_type::size_type i = 0; i < format.size();
					i++)
				{
					if(format[i] == '%')
					{
						std::stringstream formatter;
						formatter << value;
						logMessage << formatter.str();
						log(format.substr(i + 1), args...);
						return;
					}
					logMessage << format[i];
				}
			}
			
            static priority_type getPriority() { return defaultPriority; }

            static void setPriority(const priority_type &p) { defaultPriority = p; }

            static void logToFileAtPath(const string_type &path);

            static void logToFileAtPath(const char *path);

			static void closeFiles();

            static void logToStdOutput();

            static void logToStdError();

		private:

            using stream_type = std::ostream;

            using file_map_type = std::map<string_type, stream_type *>;

            static priority_type defaultPriority;

            static file_map_type logFiles;

			priority_type thePriority;
			
			std::ostringstream logMessage;
		};
		
	} // Foundation

} // TF

#define LOG_TO_FILE_AT_PATH(path) \
		TF::Foundation::Logger::logToFileAtPath(path)

#define LOG_TO_STDOUT() \
		TF::Foundation::Logger::logToStdOutput()

#define LOG_TO_STDERR() \
		TF::Foundation::Logger::logToStdError()

#define LOG_SET_PRIORITY(p) \
		TF::Foundation::Logger::setPriority(p)

#define LOG(priority, format, args...) \
		{ \
			TF::Foundation::Logger theLog(priority); \
			theLog.log(format , ##args); \
		}

#endif // TFLOG_HPP

