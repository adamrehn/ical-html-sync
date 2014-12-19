/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Factory class for event aggregators and HTML generators, paired by report type.
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
#ifndef _ICS2HTML_REPORT_IMPLEMENTATION_FACTORY
#define _ICS2HTML_REPORT_IMPLEMENTATION_FACTORY

#include "ComponentAggregatorBase.h"
#include "HtmlGeneratorBase.h"

enum class ReportType
{
	EventListByDay,         //List of events, grouped by day
	TodoTaskListByCategory  //List of todo tasks, grouped by category
};

class ReportImplementationFactory
{
	public:
		
		//Returns the component aggregator for the specified report type
		static ComponentAggregatorBase* getAggregatorForReportType(ReportType type);
		
		//Returns the HTML generator for the specified report type
		static HtmlGeneratorBase* getHtmlGeneratorForReportType(ReportType type);
};

#endif
