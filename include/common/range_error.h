#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <sstream>
#include <string>

using namespace std;

class baseException
{
public:
	baseException (const string& str = "") :
		msgString (str)
	{
		if (msgString == "")
			msgString = "Unspecified exception";
	}

	string what() const
	{
		return msgString;
	}

protected:
	string msgString;
};

// index out of range
class indexRangeError: public baseException
{
public:
	indexRangeError (const string& msg, int i, int size) :
		baseException()
	{
		ostringstream indexErr;

		indexErr << msg << "  index " << i << "  size = " << size << ends;
		msgString = indexErr.str();
	}
};

#endif	// EXCEPTION_CLASSES
