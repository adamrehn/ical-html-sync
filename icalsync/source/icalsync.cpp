/*
//  iCalendar HTML Sync (icalsync)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
*/
#include "ConfigParser.h"
#include "DropboxDirectoryDetection.h"
#include "ReportGenerator.h"
#include "TemplateManager.h"

#include <simple-base/base.h>
#include <iostream>
#include <stdexcept>

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#endif

int main (int argc, char* argv[])
{
	//Under POSIX operating systems, we support running as a daemon
	#ifndef WIN32
	if (argc > 1 && string(argv[1]) == "--daemon")
	{
		//If a custom home directory was specified, use it
		if (argc > 3 && string(argv[2]) == "--homedir") {
			setenv("HOME", argv[3], true);
		}
		
		//Attempt to create a child process
		pid_t pid = fork();
		
		//If we failed to fork(), exit immediately
		if (pid < 0) {
			return -1;
		}
		
		//Terminate the parent process
		if (pid > 0) {
			return 0;
		}
		
		//Change the file mode mask
		umask(0);
		
		//Attempt to create a new SID for the child process
		pid_t sid = setsid();
		if (sid < 0) {
			return -1;
		}
		
		//Change the current working directory
		chdir("/");
		
		//Close the standard file descriptors
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	#endif
	
	try
	{
		//Parse the default config file
		Config config = ConfigParser::parseDefaultConfigFile();
		
		//Prefix the output HTML file paths with the user's Dropbox directory location
		string dropboxDir = DropboxDirectoryDetection::detect();
		config.eventsHtmlFile = (config.eventsHtmlFile.empty()) ? "" : dropboxDir + "/" + config.eventsHtmlFile;
		config.tasksHtmlFile  = (config.tasksHtmlFile.empty())  ? "" : dropboxDir + "/" + config.tasksHtmlFile;
		
		//Use the default HTML template files
		config.eventsTemplateFile = TemplateManager::getEventsReportTemplateLocation();
		config.tasksTemplateFile = TemplateManager::getTasksReportTemplateLocation();
		
		//Begin monitoring the calendar file
		ReportGenerator::MonitorCalendarAndGenerateReports(config, dirname(argv[0]));
	}
	catch (std::runtime_error& e) {
		std::clog << "Error: " << e.what() << std::endl;
	}
	
	return 0;
}
