#ifndef OUTPUTZZ_H
#define OUTPUTZZ_H

#include <iostream>
#include <sstream>
#include <string>

#include "NTL/ZZ.h"

using namespace std;

NTL_CLIENT

class omanip
{
private:
	// pointer to the function that does the work
	ostream& (*func) (ostream& ostr, const int& x, const ZZ& y);
	// the two manipulator parameters stored in the class
	int parm;
	ZZ n;

public:
	omanip (ostream& (*f) (ostream & ostr, const int& x, const ZZ& p),
	        const int& w, const ZZ& p) : func (f), parm (w), n (p)
	{}

	// evaluate f.func with parameters f.parm1 and f.parm2.
	// this function returns an ostream&
	friend ostream& operator<< (ostream& ostr, const omanip& f)
	{
		return (*f.func) (ostr, f.parm, f.n);
	}
};

ostream& srRight (ostream& ostr, const int& w, const ZZ& n)
{
	ostringstream output;
	int len;

	output << n;
	len = output.str().length();

	if (len < w)
	{
		len = w - len;

		for (int i=0; i < len; i++)
			ostr << ' ';
	}

	ostr << output.str().c_str();

	return ostr;
}

ostream& srLeft (ostream& ostr, const int& w, const ZZ& n)
{
	ostringstream output;
	int len;

	output << n;
	ostr << output.str().c_str();
	len = output.str().length();

	if (len < w)
	{
		len = w - len;

		for (int i=0; i < len; i++)
			ostr << ' ';
	}

	return ostr;
}

omanip outputZZRight (const int& w, const ZZ& n)
{
	return omanip (srRight,w,n);
}

omanip outputZZLeft (const int& w, const ZZ& n)
{
	return omanip (srLeft,w,n);
}

#endif
