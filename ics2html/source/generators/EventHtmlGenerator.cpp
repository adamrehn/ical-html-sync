/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  HTML generator for aggregated events.
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
#include "EventHtmlGenerator.h"

#include <simple-base/base.h>

EventHtmlGenerator::EventHtmlGenerator() {
	this->timezoneOffset = get_current_timezone_offset();
}

string EventHtmlGenerator::generateReportTitle() {
	return "Calendar Events";
}

string EventHtmlGenerator::generateOpeningHtmlForGroup(const string& group)
{
	//Generate a title containing the human-readable version of the day's timestamp
	string day = date("%A the %d%suf of %B %Y", to<time_t>(group));
	return "<h2>" + this->escapeForHtml(day) + "</h2><ul>";
}

string EventHtmlGenerator::generateClosingHtmlForGroup(const string& group) {
	return "</ul>";
}

string EventHtmlGenerator::generateHtmlForComponent(const ComponentWrapper& component, const string& group)
{
	//Determine if the event is an all-day event
	string timePrefix = "";
	if (component.isAllDayEvent() == false)
	{
		//Retrieve the span of the event and extract the start and end times
		std::pair<time_t, time_t> eventSpan = component.getEventSpan();
		string startTime = date("%I:%M%p", eventSpan.first  + this->timezoneOffset);
		string endTime   = date("%I:%M%p", eventSpan.second + this->timezoneOffset);
		
		//Prefix the event summary with the start and end times
		timePrefix = ("<span class=\"time\">" + ((startTime != endTime) ? startTime + " - " + endTime : startTime) + "</span><br>");
	}
	
	//Determine if the event has a location specified
	string location = component.getPropertyValue(ICAL_LOCATION_PROPERTY);
	string locationSuffix = ((!location.empty()) ? " at <span class=\"location\">" + this->escapeForHtml(location) + "</span>" : "");
	
	//Output the event summary, location, and description
	string html = string("<li>")
		+ "<p class=\"summary\">" + timePrefix + this->escapeForHtml(component.getPropertyValue(ICAL_SUMMARY_PROPERTY)) + locationSuffix + "</p>"
		+ "<p class=\"description\">" + str_replace("\n", "<br>", this->escapeForHtml(component.getPropertyValue(ICAL_DESCRIPTION_PROPERTY))) + "</p>"
		+ "</li>";
	
	//Return the generated HTML
	return html;
}
