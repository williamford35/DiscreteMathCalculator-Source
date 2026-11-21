#ifndef EXPRESSIONERROREXCEPTION_H
#define EXPRESSIONERROREXCEPTION_H

#include <string>

using namespace std;

class ExpressionErrorException
{
private:
	string msgString;

public:
	ExpressionErrorException (const string& str = "") :
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

#endif	// EXPRESSIONERROREXCEPTION_H
