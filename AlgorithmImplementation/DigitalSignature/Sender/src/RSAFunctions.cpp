#include "RSAFunctions.h"
#include "report.h"

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSource;
using CryptoPP::StringSink;

#include "NTL/ZZ.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;

NTL_CLIENT

Big generatePrime (int nbits)
{
	ZZ ntlkbitprime;
	Big kbitprime;
	ostringstream numstr;
	char buf[500];

	ntlkbitprime = RandomPrime_ZZ ( (long) nbits );
	numstr << ntlkbitprime;
	strcpy (buf, (char *) numstr.str().c_str() );
	kbitprime = Big (buf);

	return kbitprime;
}

Big eulerPhi (const Big& p, const Big& q)
{
	Big result;

	result = (p - 1) * (q - 1);

	return result;
}

Big findCoprime (const Big& t)
{
	long numberBits;
	Big e;
	big rnd;

	numberBits = bits (t);
	rnd = mirvar (0);

	do
	{
		// generate a random integer with 3/4*numberBits
		bigbits (long (.75 * numberBits),rnd);
		e = Big (rnd);
	}
	while (gcd (e, t) != 1);

	return e;
}

Big inverseModn (const Big& a, const Big& n)
{
	Big result;

	result = inverse (a, n);

	return result;
}

Big powerModn (const Big& a, const Big&b, const Big& n)
{
	Big result;

	result = pow (a, b, n);

	return result;
}

string prepareKey (char *buf)
{
	int len, extraChars, remainingChars;
	string s = buf;

	len = s.length();
	extraChars = len % 8;
	s = s.substr (0, len-extraChars);

	return s;
}

string prepareMsg (const string& s, unsigned char& padding)
{
	int len = s.length(), p;
	string t = s;

	if ( (p = len % 8) > 0)
		p = (len/8 + 1) *8;

	padding = p - len;

	for (int i=0; i < padding; i++)
		t += '\0';

	return t;
}

void convertStringToHex (const string& inputStr, wxString& outputStr)
{
	// convert buffer to a string of hex digits
	string encodedTextInHex;

	StringSource (inputStr, true,
	              new HexEncoder (
	                 new StringSink (encodedTextInHex),
	                 true,
	                 2,
	                 " "
	              ) // HexEncoder
	             ); // StringSource

	outputStr = wxString::FromUTF8 (encodedTextInHex.c_str() );
}
