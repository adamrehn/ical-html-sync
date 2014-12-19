/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Wrapper class to abstract interacting with libical iCal component instances.
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
#include "ComponentWrapper.h"

#include <simple-base/base.h>
#include <stdexcept>
#include <iostream>

namespace iCalAbstraction {

ComponentWrapper ComponentWrapper::fromFile(const string& filename) {
	return ComponentWrapper::fromString( file_get_contents(filename) );
}

ComponentWrapper ComponentWrapper::fromString(const string& iCalData)
{
	if (iCalData.empty()) {
		return ComponentWrapper(NULL);
	}
	
	icalcomponent* rootNode = icalparser_parse_string(iCalData.c_str());
	return ComponentWrapper(rootNode, true);
}

ComponentWrapper::ComponentWrapper(icalcomponent* component, bool ownsPointer)
{
	this->component = component;
	this->ownsPointer = ((this->component != NULL) ? ownsPointer : false);
}

ComponentWrapper::ComponentWrapper(const ComponentWrapper& other) {
	this->copyFrom(other);
}

ComponentWrapper::ComponentWrapper(ComponentWrapper&& other)
{
	//Perform a shallow copy
	this->component   = other.component;
	this->ownsPointer = other.ownsPointer;
	
	//Leave the other instance empty
	other.component = NULL;
	other.ownsPointer = false;
}

ComponentWrapper& ComponentWrapper::operator=(const ComponentWrapper& other)
{
	this->copyFrom(other);
	return *this;
}

ComponentWrapper::~ComponentWrapper()
{
	//Only free the wrapped pointer if we actually have ownership of the memory
	if (this->valid() == true && this->ownsPointer == true)
	{
		icalcomponent_free(this->component);
		this->component = NULL;
	}
}

bool ComponentWrapper::valid() const {
	return (this->component != NULL);
}

string ComponentWrapper::toString() const {
	return (this->valid() ? string(icalcomponent_as_ical_string(this->component)) : "");
}

bool ComponentWrapper::isType(icalcomponent_kind type) const {
	return (this->valid() && icalcomponent_isa(this->component) == type);
}

vector<ComponentWrapper> ComponentWrapper::getChildNodes(icalcomponent_kind type)
{
	vector<ComponentWrapper> nodes;
	
	if (this->valid() == false) {
		return nodes;
	}
	
	//Iterate over the child nodes of the specified type and collect them in a list
	icalcomponent* currChild = icalcomponent_get_first_component(this->component, type);
	while (currChild != NULL)
	{
		//Add the current child node to the list
		nodes.push_back( ComponentWrapper(currChild, false) );
		currChild = icalcomponent_get_next_component(this->component, type);
	}
	
	return nodes;
}

vector<PropertyWrapper> ComponentWrapper::getProperties(icalproperty_kind type) const
{
	vector<PropertyWrapper> properties;
	
	if (this->valid() == false) {
		return properties;
	}
	
	//Iterate over the child nodes of the specified type and collect them in a list
	const icalproperty* currProperty = icalcomponent_get_first_property(this->component, type);
	while (currProperty != NULL)
	{
		//Add the current property to the list
		properties.push_back( PropertyWrapper(currProperty) );
		currProperty = icalcomponent_get_next_property(this->component, type);
	}
	
	return properties;
}

ComponentWrapper ComponentWrapper::getFirstChild(icalcomponent_kind type)
{
	vector<ComponentWrapper> children = this->getChildNodes(type);
	if (!children.empty()) {
		return children.front();
	}
	
	//No component found with the specified type
	return ComponentWrapper(NULL);
}

PropertyWrapper ComponentWrapper::getFirstProperty(icalproperty_kind type) const
{
	vector<PropertyWrapper> properties = this->getProperties(type);
	if (!properties.empty()) {
		return properties.front();
	}
	
	//No property found with the specified type
	return PropertyWrapper(NULL);
}

string ComponentWrapper::getPropertyValue(icalproperty_kind type) const {
	return this->getFirstProperty(type).value();
}

pair<time_t, time_t> ComponentWrapper::getEventSpan() const
{
	//Check that the wrapped component is actually a VEVENT node
	if (this->valid() == false || this->isType(ICAL_VEVENT_COMPONENT) == false) {
		throw std::runtime_error("attempted to retrieve span for a non-event component");
	}
	
	//Retrieve the start and end times
	icaltime_span span = icalcomponent_get_span(this->component);
	return std::make_pair( span.start, span.end );
}

vector<time_t> ComponentWrapper::getEventRecurrences() const
{
	vector<time_t> recurrences;
	
	//Check that the wrapped component is actually a VEVENT node
	if (this->valid() == false || this->isType(ICAL_VEVENT_COMPONENT) == false) {
		throw std::runtime_error("attempted to retrieve recurrences for a non-event component");
	}
	
	//Retrieve the recurrence rule and start time for the event
	const icalproperty* rrule = icalcomponent_get_first_property(this->component, ICAL_RRULE_PROPERTY);
	const icalproperty* start = icalcomponent_get_first_property(this->component, ICAL_DTSTART_PROPERTY);
	struct icalrecurrencetype recur = icalproperty_get_rrule(rrule);
	struct icaltimetype dtstart     = icalproperty_get_dtstart(start);
	
	//Create an iterator and iterate over the occurrences
	icalrecur_iterator* ritr = icalrecur_iterator_new(recur, dtstart);
	for
	(
		struct icaltimetype next = icalrecur_iterator_next(ritr);
		!icaltime_is_null_time(next);
		next = icalrecur_iterator_next(ritr)
	)
	{
		//Add the recurrence to the list
		recurrences.push_back( icaltime_as_timet(next) );
	}
	
	return recurrences;
}

bool ComponentWrapper::isAllDayEvent() const
{
	if (this->valid() == false) {
		return false;
	}
	
	//Retrieve the event span
	std::pair<time_t, time_t> span = this->getEventSpan();
	
	//All-day events are represented by libical as 00:00:00 on the day to 23:59:59 the next day (172799 seconds apart)
	return (
		(date("%H:%M:%S", span.first)  == "00:00:00") &&
		(date("%H:%M:%S", span.second) == "23:59:59") &&
		(span.second - span.first == 172799)
	);
}

void ComponentWrapper::copyFrom(const ComponentWrapper& other)
{
	//If the other instance owns its pointer, we need to perform a deep copy
	if (other.ownsPointer == true && other.component != NULL)
	{
		this->component   = icalcomponent_new_clone(other.component);
		this->ownsPointer = other.ownsPointer;
	}
	else
	{
		//The other instance doesn't own the memory, so neither do we
		this->component = other.component;
		this->ownsPointer = other.ownsPointer;
	}
}

} //End namespace iCalAbstraction
