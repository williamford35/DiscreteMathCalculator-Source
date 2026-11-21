/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "elliptic-curves/RSAFunctions.h"
#include "common/report.h"
#include "miracl/big.h"

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSource;
using CryptoPP::StringSink;

#include <stdexcept>
using std::runtime_error;

#include <string>
#include <iostream>
#include <sstream>

miracl *mip;

using std::istringstream;

using namespace std;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "cryptopp/integer.h"
using CryptoPP::Integer;

#include "cryptopp/nbtheory.h"
using CryptoPP::ModularExponentiation;

#include "cryptopp/dh.h"
using CryptoPP::DH;

#include "cryptopp/secblock.h"
using CryptoPP::SecByteBlock;

#include <NTL/ZZ.h>

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
	mip=mirsys(nbits,0);
	// std::setprecision(m);

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
	int len, extraChars;
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

Big getGenerator(const Big& p, long bits)
{
	AutoSeededRandomPool rnd;
	DH dh;
	Big g;
	std::stringstream buffer;
	string gstr;
	char buf[5000];

	dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, bits);
	if(!dh.GetGroupParameters().ValidateGroup(rnd, 3))
		throw runtime_error("Failed to validate prime and generator");

	const Integer& tmpg = dh.GetGroupParameters().GetGenerator();
	buffer << tmpg << endl;

	gstr = buffer.str();
	strcpy(buf,gstr.c_str());
	g = Big(buf);

	return g;
}

Big generateBig(int nbits)
{
	big rnd;
	Big myBig;

	miracl *mip = get_mip();
	mip->IOBASE = 10;
	rnd = mirvar (0);
	bigbits (nbits,rnd);
	myBig = Big (rnd);
	return myBig;
}

p_and_seed generate_p_and_seed(int numbits)
{
	big rnd;
	Big myPrime, myBigSeed;
	p_and_seed v;
	miracl *mip = mirsys(5000,2);

	mip->IOBASE = 10;

	myPrime = generatePrime(numbits);
	v.p = myPrime;

	rnd = mirvar (0);
	bigbits (160,rnd);
	myBigSeed = Big (rnd);
	v.seed = myBigSeed;

	return v;
}

