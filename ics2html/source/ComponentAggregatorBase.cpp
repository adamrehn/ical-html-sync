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
#include "ComponentAggregatorBase.h"

ComponentAggregatorBase::~ComponentAggregatorBase() {}

void ComponentAggregatorBase::aggregateComponents(const vector<ComponentWrapper>& components)
{
	//Process each component in turn
	for (auto component : components) {
		this->processComponent(component);
	}
}

vector<string> ComponentAggregatorBase::getGroupings() const
{
	vector<string> keys;
	
	for (auto mapping : aggregatedComponents) {
		keys.push_back(mapping.first);
	}
	
	return keys;
}

vector<ComponentWrapper>& ComponentAggregatorBase::getComponentsForGroup(const string& group)
{
	//Determine if the requested group already has a mapping
	map< string, vector<ComponentWrapper> >::iterator groupIter = this->aggregatedComponents.find(group);
	if (groupIter == this->aggregatedComponents.end())
	{
		//Create a new mapping and return a reference to it
		auto insertResult = this->aggregatedComponents.insert( std::make_pair(group, vector<ComponentWrapper>()) );
		return (insertResult.first)->second;
	}
	
	//Return a reference to the existing mapping
	return groupIter->second;
}

void ComponentAggregatorBase::addComponentToGroup(const string& group, const ComponentWrapper& component) {
	this->getComponentsForGroup(group).push_back(component);
}
