/*
//  iCalendar HTML Report Generator (ics2html)
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
#include "ical/ComponentWrapper.h"
#include "HtmlReportGenerator.h"
#include "ReportImplementationFactory.h"

#include <simple-base/base.h>
#include <memory>
#include <iostream>
#include <vector>
using std::vector;
using std::clog;
using std::endl;

using iCalAbstraction::ComponentWrapper;

int main (int argc, char* argv[])
{
	//Determine if the correct number of arguments have been supplied
	if (argc > 3)
	{
		//Parse the supplied arguments
		ReportType mode     = ((strtolower(argv[1]) == "tasks") ? ReportType::TodoTaskListByCategory : ReportType::EventListByDay);
		string icalFile     = argv[2];
		string htmlFile     = argv[3];
		string templateHtml = ((argc > 4) ? file_get_contents(argv[4]) : "");
		
		//Attempt to parse the iCal file
		ComponentWrapper calendar = ComponentWrapper::fromFile(icalFile);
		if (calendar.valid())
		{
			//Generate the HTML report
			string report = HtmlReportGenerator::generateHtmlReport(calendar, mode, templateHtml);
			
			//Attempt to write the generated HTML to file
			if (file_put_contents(htmlFile, report) == false) {
				clog << "Error: failed to write to output file \"" << htmlFile << "\"!" << endl;
			}
		}
		else {
			clog << "Error: failed to open input file \"" << icalFile << "\"!" << endl;
		}
	}
	else {
		clog << "Usage syntax:" << endl << "ic2html [events|tasks] INPUT.ICS OUTPUT.HTML [TEMPLATE.HTML]" << endl;
	}
	
	return 0;
}
