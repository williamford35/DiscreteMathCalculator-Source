#include <string>

using namespace std;

class spellNumberException
{
private:
	string msgString;

public:
	spellNumberException (const string& str = "") :
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
