/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Class to generate HTML reports.
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
#include "HtmlReportGenerator.h"

#include <simple-base/base.h>
#include <memory>

//The template tokens that are replaced with the generated content
#define TITLE_TOKEN "$$REPORT_TITLE$$"
#define BODY_TOKEN  "$$REPORT_BODY$$"

//The default template, used when the supplied template is not valid
#define DEFAULT_TEMPLATE "<!doctype html><html><head><title>$$REPORT_TITLE$$</title></head><body><h1>$$REPORT_TITLE$$</h1>$$REPORT_BODY$$</body></html>"

bool HtmlReportGenerator::isTemplateValid(const string& templateHTML) {
	return (in(TITLE_TOKEN, templateHTML) && in(BODY_TOKEN, templateHTML));
}

string HtmlReportGenerator::generateHtmlReport(ComponentWrapper& calendar, ReportType type, const string& templateHTML)
{
	//Retrieve the aggregator and HTML generator
	std::unique_ptr<ComponentAggregatorBase> aggregator   ( ReportImplementationFactory::getAggregatorForReportType(type)    );
	std::unique_ptr<HtmlGeneratorBase>       htmlGenerator( ReportImplementationFactory::getHtmlGeneratorForReportType(type) );
	
	//Aggregate the components from the supplied calendar
	aggregator->aggregateComponents( calendar.getChildNodes( aggregator->aggregatedComponentType() ) );
	
	//Start building the HTML string that will be placed in the template
	string reportContents = "";
	
	//Iterate over the groupings
	vector<string> groups = aggregator->getGroupings();
	for (auto group : groups)
	{
		//Generate the opening HTML for the group
		reportContents += htmlGenerator->generateOpeningHtmlForGroup(group);
		
		//Iterate over the components in the group
		vector<ComponentWrapper>& groupComponents = aggregator->getComponentsForGroup(group);
		for (auto component : groupComponents)
		{
			//Generate the HTML for each component
			reportContents += htmlGenerator->generateHtmlForComponent(component, group);
		}
		
		//Generate the closing HTML for the group
		reportContents += htmlGenerator->generateClosingHtmlForGroup(group);
	}
	
	//Insert the generated title and report body into the template HTML
	string finishedReport = (HtmlReportGenerator::isTemplateValid(templateHTML) ? templateHTML : DEFAULT_TEMPLATE);
	finishedReport = str_replace(TITLE_TOKEN, htmlGenerator->generateReportTitle(), finishedReport);
	finishedReport = str_replace(BODY_TOKEN,  reportContents,                       finishedReport);
	
	//Return the completed report HTML
	return finishedReport;
}
