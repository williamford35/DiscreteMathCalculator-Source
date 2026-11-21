#ifndef GRAPH_FILE_FORMAT_EXCEPTION_H
#define GRAPH_FILE_FORMAT_EXCEPTION_H

#include <string>

using namespace std;

class graphFileFormatException
{
private:
	string msgString;

public:
	graphFileFormatException (const string& str = "") :
		msgString (str)
	{
		if (msgString == "")
			msgString = "Unspecified exception";
	}

	string what() const
	{
		return msgString;
	}

};

#endif
