#ifndef USE_ASYMPTOTIC_ESTIMATE_EXCEPTION_H
#define USE_ASYMPTOTIC_ESTIMATE_EXCEPTION_H

#include <string>

using namespace std;

class useAsymptoticEstimateException
{
private:
	string msgString;

public:
	useAsymptoticEstimateException (const string& str = "") :
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
