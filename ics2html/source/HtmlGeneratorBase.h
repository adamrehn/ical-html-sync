/*
//  iCalendar HTML Report Generator (ics2html)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Abstract base class for classes that generate HTML for aggregated components.
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
#ifndef _ICS2HTML_HTML_GENERATOR_BASE
#define _ICS2HTML_HTML_GENERATOR_BASE

#include "ical/ComponentWrapper.h"
using iCalAbstraction::ComponentWrapper;

class HtmlGeneratorBase
{
	public:
		
		virtual ~HtmlGeneratorBase();
		
		//Generates the title for the report
		virtual string generateReportTitle() = 0;
		
		//Generate the opening and closing HTML for a grouping
		virtual string generateOpeningHtmlForGroup(const string& group) = 0;
		virtual string generateClosingHtmlForGroup(const string& group) = 0;
		
		//Generates the HTML for an individual component
		virtual string generateHtmlForComponent(const ComponentWrapper& component, const string& group) = 0;
		
	protected:
		
		//Escapes special characters as HTML character entity codes
		string escapeForHtml(const string& str);
};

#endif
