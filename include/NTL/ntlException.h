#ifndef NTLEXCEPTION_H
#define NTLEXCEPTION_H

#include <string.h>
#include <string>

using namespace std;

class ntlException
{
private:
	string msgString;

public:
	ntlException (const string& str = "") : msgString (str)
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
