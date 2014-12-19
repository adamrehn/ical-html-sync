/*
//  iCalendar HTML Sync (icalsync)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Class to parse configuration data.
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
#include "ConfigParser.h"

#include <simple-base/base.h>
#include <stdexcept>
#include <vector>
using std::vector;

#define DEFAULT_CONFIG_DATA "/path/to/calendar.ics\nCalendar Events.html\nCalendar Tasks.html"

string ConfigParser::getConfigDirectory()
{
	//Determine the default config directory location, based on platform
	#ifdef _WIN32
		return get_env("APPDATA") + "/ical-html-sync";
	#else
		return get_env("HOME") + "/.config/ical-html-sync";
	#endif
}

Config ConfigParser::parseDefaultConfigFile()
{
	//Determine the default config file location
	string defaultConfigFile = ConfigParser::getConfigDirectory() + "/icalsync.conf";
	
	//Create the config file's parent directory if it doesn't already exist
	string configDir = dirname(defaultConfigFile);
	if (!file_exists(configDir)) {
		make_dir(configDir);
	}
	
	//Create the config file itself if it doesn't already exist
	if (!file_exists(defaultConfigFile)) {
		file_put_contents(defaultConfigFile, DEFAULT_CONFIG_DATA);
	}
	
	//Attempt to parse the default config file
	return ConfigParser::parseConfigFile(defaultConfigFile);
}

Config ConfigParser::parseConfigFile(const string& configFile)
{
	//Attempt to parse the specified config file
	Config config;
	string data = file_get_contents(configFile);
	vector<string> lines = explode("\n", str_replace("\r\n", "\n", data));
	
	//Check that at least one input file and one output file were specified
	bool valid = true;
	valid = valid && (lines.size() > 1);
	valid = valid && (!lines[0].empty());
	valid = valid && (!lines[1].empty() || (lines.size() > 2 && !lines[2].empty()));
	if (valid == false) {
		throw std::runtime_error("failed to parse config file \"" + configFile + "\"");
	}
	
	//Store the config data
	config.calendarFile   = lines[0];
	config.eventsHtmlFile = lines[1];
	config.tasksHtmlFile  = ((lines.size() > 2) ? lines[2] : "");
	
	return config;
}
