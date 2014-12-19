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
#include "ReportImplementationFactory.h"

#include "aggregators/EventAggregator.h"
#include "aggregators/TaskAggregator.h"
#include "generators/EventHtmlGenerator.h"
#include "generators/TaskHtmlGenerator.h"

ComponentAggregatorBase* ReportImplementationFactory::getAggregatorForReportType(ReportType type)
{
	switch (type)
	{
		case ReportType::EventListByDay:
			return new EventAggregator();
		
		case ReportType::TodoTaskListByCategory:
			return new TaskAggregator();
	}
}

HtmlGeneratorBase* ReportImplementationFactory::getHtmlGeneratorForReportType(ReportType type)
{
	switch (type)
	{
		case ReportType::EventListByDay:
			return new EventHtmlGenerator();
		
		case ReportType::TodoTaskListByCategory:
			return new TaskHtmlGenerator();
	}
}
