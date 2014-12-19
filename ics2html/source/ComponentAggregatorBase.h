/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Abstract base class for classes that aggregate calendar components.
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
#ifndef _ICS2HTML_COMPONENT_AGGREGATOR_BASE
#define _ICS2HTML_COMPONENT_AGGREGATOR_BASE

#include "ical/ComponentWrapper.h"
using iCalAbstraction::ComponentWrapper;

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

class ComponentAggregatorBase
{
	public:
		
		virtual ~ComponentAggregatorBase();
		
		//Returns the component type that the concrete implementation is designed to aggregate
		virtual icalcomponent_kind aggregatedComponentType() const = 0;
		
		//Aggregates the supplied list of components
		void aggregateComponents(const vector<ComponentWrapper>& components);
		
		//Retrieves the list of keys from the aggregation mappings
		vector<string> getGroupings() const;
		
		//Retrieves a reference to the list of components for the specified aggregation group
		vector<ComponentWrapper>& getComponentsForGroup(const string& group);
		
	protected:
		
		//Adds the specified component to the specified group
		void addComponentToGroup(const string& group, const ComponentWrapper& component);
		
		//Processes an individual component and adds a mapping for it
		virtual void processComponent(const ComponentWrapper& component) = 0;
		
		map< string, vector<ComponentWrapper> > aggregatedComponents;
};

#endif
