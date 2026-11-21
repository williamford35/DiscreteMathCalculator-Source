#ifndef GRAPHEXCEPTION_CLASS_H
#define GRAPHEXCEPTION_CLASS_H

#include <string>

using namespace std;

// error in graph class
class GraphException
{
private:
	string msgString;

public:
	GraphException(const string& msg = ""): msgString(msg)
	{
		if (msgString == "")
			msgString = "Unspecified exception";
	}

	string what() const
	{
		return msgString;
	}
};

#endif	// GRAPHEXCEPTION_CLASS_H
