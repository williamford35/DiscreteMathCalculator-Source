#ifndef LISTINGTOOLONGEXCEPTION_H_INCLUDED
#define LISTINGTOOLONGEXCEPTION_H_INCLUDED

#include <string>

using namespace std;

class listingTooLongException
{
private:
	string msgString;

public:
	listingTooLongException (const string& str = "") :
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

#endif // LISTINGTOOLONGEXCEPTION_H_INCLUDED
