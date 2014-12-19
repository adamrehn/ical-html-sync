/*
//  iCalendar HTML Sync (icalsync)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Class to monitor calendar files and trigger report generation upon modification.
//  
//  ---
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
#include "ReportGenerator.h"
#include "FileChangeMonitor.h"

#include <cstdlib>
#include <iostream>

namespace
{
	class ReportGeneratorHelper
	{
		public:
			ReportGeneratorHelper(const Config& config, const string& executableLoc)
			{
				//Store the configuration data
				this->config = config;
				
				//Determine the location of the ics2html executable, based on platform
				#ifdef _WIN32
					this->ics2htmlLoc = executableLoc + "ics2html.exe";
				#else
					this->ics2htmlLoc = "ics2html";
				#endif
			}
			
			//Called when the calendar file has been modified
			bool operator()()
			{
				//Determine if we are generating an events report
				if (this->config.eventsHtmlFile.empty() == false) {
					this->generateReport("events", this->config.eventsHtmlFile, this->config.eventsTemplateFile);
				}
				
				//Determine if we are generating a tasks report
				if (this->config.tasksHtmlFile.empty() == false) {
					this->generateReport("tasks", this->config.tasksHtmlFile, this->config.tasksTemplateFile);
				}
				
				//Continue monitoring
				return true;
			}
			
		private:
			
			void generateReport(const string& mode, const string& htmlFile, const string& templateFile)
			{
				//Build the argument string to pass to ics2html
				string args = mode + " \"" + this->config.calendarFile + "\" \"" + htmlFile + "\" \"" + templateFile + "\"";
				
				//TEMPORARY: DEBUG OUTPUT
				std::clog << string(this->ics2htmlLoc + " " + args) << std::endl;
				
				//Execute ics2html
				#ifdef _WIN32
					EasyShellExecuteEx("open", this->ics2htmlLoc.c_str(), args.c_str(), NULL, SW_HIDE);
				#else
					system(string(this->ics2htmlLoc + " " + args).c_str());
				#endif
			}
			
			Config config;
			string ics2htmlLoc;
	};
}

void ReportGenerator::MonitorCalendarAndGenerateReports(const Config& config, const string& executableLoc)
{
	//Create the report generator helper
	ReportGeneratorHelper helper(config, executableLoc);
	
	//Create the file monitor and begin monitoring the calendar file
	auto monitor = FileChangeUtil::createMonitor(config.calendarFile, helper);
	monitor.startMonitoring();
}
