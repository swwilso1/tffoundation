/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2016 Tectiform Inc.

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
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{
	
	namespace Foundation
	{
		
		enum class LogOption
		{
			Console = 1,
			NoDelay,
			NoWait,
			OpenDelay,
			Stderr,
			ProcessID,
		};


		enum class LogPriority
		{
			Emergency,
			Alert,
			Critical,
			Error,
			Warning,
			Notice,
			Info,
			Debug
		};


		class Logger : public AllocatorInterface
		{
		public:
		
			using option_type = LogOption;
			
			using priority_type = LogPriority;
			
			using string_type = std::string;
		
			Logger(int options);
			
			~Logger();
			
			void log(priority_type priority, const string_type &format)
			{
				thePriority = priority;
				logMessage += format;
			}

			template<class Arg, class ...Args>
			void log(priority_type priority, const string_type &format,
				Arg value, Args ... args)
			{
				
				for(string_type::size_type i = 0; i < format.size();
					i++)
				{
					if(format[i] == '%')
					{
						std::stringstream formatter;
						formatter << value;
						logMessage += formatter.str();
						log(priority, formatter.str().substr(i+1),
							args...);
						return;
					}
					logMessage += format[i];
				}
			}
			
		private:
		
			priority_type thePriority;
			
			string_type logMessage;

		};
		
	} // Foundation

} // TF

#define LOG(options, priority, format, args...) \
		{ \
			TF::Foundation::Logger theLog(options); \
			theLog.log(priority, format , ##args); \
		}

#endif // TFLOG_HPP

