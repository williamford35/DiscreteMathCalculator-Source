class readCurveFileFormatException
{
private:
	string msgString;

public:
	readCurveFileFormatException (const string& str = "") :
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