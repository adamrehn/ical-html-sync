/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Aggregator for calendar events.
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
#include "EventAggregator.h"

#include <simple-base/base.h>

icalcomponent_kind EventAggregator::aggregatedComponentType() const {
	return ICAL_VEVENT_COMPONENT;
}

void EventAggregator::processComponent(const ComponentWrapper& component)
{
	//We are only interested in event nodes
	if (component.isType(ICAL_VEVENT_COMPONENT) == false) {
		return;
	}
	
	//Process each of the event's recurrences
	vector<time_t> recurrences = component.getEventRecurrences();
	for (auto recurrence : recurrences)
	{
		//Extract the date portion of the timestamp, without the time
		string dateStr = date("%Y-%m-%d", recurrence);
		time_t date = createTime(
			to<int>(dateStr.substr(0, 4)),
			to<int>(dateStr.substr(5, 2)),
			to<int>(dateStr.substr(8, 2))
		);
		
		//Add the event to the group for the day the recurrence falls on
		this->addComponentToGroup(toString(date), component);
	}
}
