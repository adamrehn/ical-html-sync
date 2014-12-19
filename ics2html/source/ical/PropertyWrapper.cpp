/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Wrapper class to abstract interacting with libical iCal property instances.
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
#include "PropertyWrapper.h"

#include <simple-base/base.h>

namespace iCalAbstraction {

PropertyWrapper::PropertyWrapper(const icalproperty* property) {
	this->property = property;
}

bool PropertyWrapper::valid() const {
	return (this->property != NULL);
}

string PropertyWrapper::value() const
{
	if (this->valid() == false) {
		return "";
	}
	
	//Retrieve the iCal-encoded value and unescape newlines (since libical doesn't appear to decode them properly)
	string encodedValue = string(icalproperty_get_value_as_string(this->property));
	encodedValue = str_replace("\\n", "\n", encodedValue);
	
	//Decode the value using libical
	size_t bufSize = encodedValue.length() + 1;
	char* decodedValue = new char[bufSize];
	icalvalue_decode_ical_string(encodedValue.c_str(), decodedValue, bufSize);
	
	//Wrap the result in a string, freeing the buffer
	string result = string(decodedValue);
	delete[] decodedValue;
	return result;
}

} //End namespace iCalAbstraction
