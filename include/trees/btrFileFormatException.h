#ifndef CCT_FILE_FORMAT_EXCEPTION_H
#define CCT_FILE_FORMAT_EXCEPTION_H

#include <string>

using namespace std;

class btrFileFormatException
{
private:
	string msgString;

public:
	btrFileFormatException (const string& str = "") :
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
