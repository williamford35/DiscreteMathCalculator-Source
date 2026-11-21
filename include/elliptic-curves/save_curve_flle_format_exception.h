class saveCurveFileFormatException
{
private:
	string msgString;

public:
	saveCurveFileFormatException (const string& str = "") :
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