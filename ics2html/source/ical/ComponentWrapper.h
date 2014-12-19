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
#ifndef _ICS2HTML_COMPONENT_WRAPPER
#define _ICS2HTML_COMPONENT_WRAPPER

#include "PropertyWrapper.h"

#include <libical/ical.h>
#include <sys/time.h>
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::pair;

namespace iCalAbstraction {

class ComponentWrapper
{
	public:
		static ComponentWrapper fromFile(const string& filename);
		static ComponentWrapper fromString(const string& iCalData);
		
		ComponentWrapper(icalcomponent* component, bool ownsPointer = false);
		ComponentWrapper(const ComponentWrapper& other);
		ComponentWrapper(ComponentWrapper&& other);
		ComponentWrapper& operator=(const ComponentWrapper& other);
		~ComponentWrapper();
		
		//Determines if the wrapped component is valid
		bool valid() const;
		
		//Generates the iCal string representation of the component
		string toString() const;
		
		//Determines if the component is of the specified component type (enum defined in <libical/icalenums.h>)
		bool isType(icalcomponent_kind type) const;
		
		//Retrieves the list of child nodes with the specified type (enum defined in <libical/icalenums.h>)
		vector<ComponentWrapper> getChildNodes(icalcomponent_kind type = ICAL_ANY_COMPONENT);
		
		//Retrieves the list of properties with the specified type (enum defined in <libical/icalderivedproperty.h>)
		vector<PropertyWrapper> getProperties(icalproperty_kind type = ICAL_ANY_PROPERTY) const;
		
		//Retrieves the first child node with the specified type
		ComponentWrapper getFirstChild(icalcomponent_kind type);
		
		//Retrieves the first property with the specified type
		PropertyWrapper getFirstProperty(icalproperty_kind type) const;
		
		//Retrieves the value of the first property with the specified type
		string getPropertyValue(icalproperty_kind type) const;
		
		//Retrieves the start and end times for an event
		pair<time_t, time_t> getEventSpan() const;
		
		//Retrieves an event's recurrences
		vector<time_t> getEventRecurrences() const;
		
		//Determines if an event is an all-day event
		bool isAllDayEvent() const;
		
	private:
		void copyFrom(const ComponentWrapper& other);
		
		icalcomponent* component;
		bool ownsPointer;
};

} //End namespace iCalAbstraction

#endif
