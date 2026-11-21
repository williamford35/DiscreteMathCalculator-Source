#ifndef SCHOOFEXCEPTION_H
#define SCHOOFEXCEPTION_H

#include <string>

using namespace std;

class schoofException
{
private:
	string msgString;

public:
	schoofException (const string& str = "") : msgString (str)
	{
		if (msgString == "")
			msgString = "Unspecified exception";
	}

	string what() const
	{
		return msgString;
	}

};

#endif // SCHOOFEXCEPTION_H
