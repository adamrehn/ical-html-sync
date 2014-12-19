/*
//  iCalendar HTML Sync (icalsync)
//  Copyright (c) 2011-2014, Adam Rehn
//  
//  ---
//  
//  Class to detect the location of the user's Dropbox directory.
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
#include "DropboxDirectoryDetection.h"
#include <simple-base/base.h>
#include <stdexcept>
#include <memory>

namespace
{
	extern "C" {
		#include <b64/cdecode.h>
	}
	
	string base64_decode(const string& str)
	{
		//Initialise the decoder state
		base64_decodestate state;
		base64_init_decodestate(&state);
		
		//Create a buffer to hold the output
		std::unique_ptr<char[]> buffer( new char[ str.size() ] );
		
		//Decode
		int decodedLength = base64_decode_block(str.c_str(), str.size(), buffer.get(), &state);
		return string(buffer.get(), decodedLength);
	}
}

string DropboxDirectoryDetection::detect()
{
	#ifdef WIN32
	string hostFileLoc = get_env("APPDATA") + "/Dropbox/host.db";
	#else
	string hostFileLoc = get_env("HOME") + "/.dropbox/host.db";
	#endif
	
	//Attempt to retrieve the Dropbox directory location
	vector<string> lines = explode("\n", file_get_contents(hostFileLoc));
	if (lines.size() > 1) {
		return base64_decode(lines[1]);
	}
	
	throw std::runtime_error("failed to detect Dropbox directory location");
}
