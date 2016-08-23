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

// The contents of this file are currently quite Unix specific.

#include <syslog.h>
#include "tflog.hpp"

namespace TF
{

	namespace Foundation
	{


#		define TEST_AND_SET_OPTION(eval, sysval) \
		if(static_cast<LogOption>(options | static_cast<int>(eval)) == eval) \
			openOptions |= sysval

		Logger::Logger(int options)
		{
			int openOptions = 0;
			
			TEST_AND_SET_OPTION(LogOption::Console, LOG_CONS);
			TEST_AND_SET_OPTION(LogOption::NoDelay, LOG_NDELAY);
			TEST_AND_SET_OPTION(LogOption::NoWait, LOG_NOWAIT);
			TEST_AND_SET_OPTION(LogOption::OpenDelay, LOG_ODELAY);
			TEST_AND_SET_OPTION(LogOption::Stderr, LOG_ERR);
			TEST_AND_SET_OPTION(LogOption::ProcessID, LOG_PID);
				
			openlog(nullptr, openOptions, LOG_SYSLOG);
		}
			

		Logger::~Logger()
		{
			int priority = 0;
			
			switch(thePriority)
			{
				case LogPriority::Alert:
					priority = LOG_ALERT;
					break;
				case LogPriority::Critical:
					priority = LOG_CRIT;
					break;
				case LogPriority::Debug:
					priority = LOG_DEBUG;
					break;
				case LogPriority::Emergency:
					priority = LOG_EMERG;
					break;
				case LogPriority::Error:
					priority = LOG_ERR;
					break;
				case LogPriority::Info:
					priority = LOG_INFO;
					break;
				case LogPriority::Notice:
					priority = LOG_NOTICE;
					break;
				case LogPriority::Warning:
					priority = LOG_WARNING;
					break;
				default:
					;
			}

			syslog(priority, "%s", logMessage.c_str());

			closelog();
		}


	} // Foundation

} // TF

