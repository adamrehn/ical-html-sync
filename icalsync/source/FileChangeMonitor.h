/*
//  iCalendar HTML Sync (icalsync)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Class to monitor modifications to an individual file.
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
#ifndef _ICALSYNC_FILE_CHANGE_MONITOR
#define _ICALSYNC_FILE_CHANGE_MONITOR

#include <simple-base/base.h>
#include <stdexcept>
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace FileChangeUtil {

template <typename CallbackTy>
class FileChangeMonitor
{
	public:
		
		FileChangeMonitor(const string& filepath)
		{
			if (!file_exists(filepath)) {
				throw std::runtime_error("file to be monitored does not exist: \"" + filepath + "\"");
			}
			
			this->lastModifiedTime = file_last_modified(filepath);
			this->filepath = filepath;
		}
		
		//Registers a file modification notification listener
		void addListener(CallbackTy callback) {
			this->callbacks.push_back(callback);
		}
		
		//Starts file monitoring
		void startMonitoring() {
			MonitorDirectoryForFileWrites( dirname(this->filepath), *this );
		}
		
		//Called internally by MonitorDirectoryForFileWrites, do not call from other code
		bool operator()()
		{
			//Determine if the file has changed
			time_t newLastModified = file_last_modified(this->filepath);
			if (newLastModified == this->lastModifiedTime) {
				return true;
			}
			
			//Keep track of the new modification time
			this->lastModifiedTime = newLastModified;
			
			//Invoke each registered callback in turn
			for (auto callback : this->callbacks)
			{
				//If any callback returns false, stop monitoring
				if (callback() == false) {
					return false;
				}
			}
			
			//Continue monitoring
			return true;
		}
		
	protected:
		
		vector<CallbackTy> callbacks;
		time_t lastModifiedTime;
		string filepath;
};

//Convenience method to save having to explicitly specify CallbackTy, for use with the C++11 auto keyword
template <typename CallbackTy>
FileChangeMonitor<CallbackTy> createMonitor(const string& filepath, CallbackTy callback)
{
	FileChangeMonitor<CallbackTy> monitor(filepath);
	monitor.addListener(callback);
	return monitor;
}

} //End namespace FileChangeMonitor

#endif
