/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Aggregator for calendar tasks.
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
#include "TaskAggregator.h"

#include <simple-base/base.h>

icalcomponent_kind TaskAggregator::aggregatedComponentType() const {
	return ICAL_VTODO_COMPONENT;
}

void TaskAggregator::processComponent(const ComponentWrapper& component)
{
	//We are only interested in todo nodes
	if (component.isType(ICAL_VTODO_COMPONENT) == false) {
		return;
	}
	
	//Group the task based on its category, if it has one
	string category = component.getPropertyValue(ICAL_CATEGORIES_PROPERTY);
	this->addComponentToGroup((!category.empty()) ? category : "Uncategoried", component);
}
