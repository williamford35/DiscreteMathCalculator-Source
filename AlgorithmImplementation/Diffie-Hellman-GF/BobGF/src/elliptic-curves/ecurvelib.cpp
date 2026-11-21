#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "elliptic-curves/ecurvelib.h"
#include "elliptic-curves/schoofException.h"
#include "elliptic-curves/collectBigFactors.h"
#include "report.h"
#include "miracl/big.h"
#include <wx/listctrl.h>
#include <wx/tokenzr.h>

void convert_big_octet (_MIPD_ big w,octet *s)
{
	/* internal:- converts from big integer to octet format */
	s->len= (oltype) big_to_bytes (_MIPP_ 0,w,s->val,FALSE);
}

void OS2FEP (_MIPD_ octet *s,big w)
{
	/* internal:- converts from octet to big format */
	bytes_to_big (_MIPP_ s->len,s->val,w);
}

#include <iostream>

using namespace std;

BOOL modsqrt(const Big& x,const Big& p, Big& z)
{
	Big y;
	BOOL haveModSqrt;

	haveModSqrt = sqroot(x.fn,p.fn,y.fn);
	z = y;

	return haveModSqrt;
}

// brute force approach for small p only
vector<pair<long,long> > ecBruteForceOrder (long& a, long& b, long& p)
{
	int ord = 0;
	long i, value, order, tmp;
	Big bigtmp, bigvalue, bigp;
	vector<pair<long,long> > pt;
	pair<long,long> point;

	for (i = 0; i < p; i = i + 1)
	{
		value = (i*i*i + a*i + b) % p;
		bigvalue = Big (value);
		bigp = Big (p);

		if (modsqrt (bigvalue, p, bigtmp) != 0)
			if (bigtmp  == 0)
			{
				ord++;
				tmp = toint (bigtmp);
				point.first = i;
				point.second = tmp;
				pt.push_back (point);
			}
			else
			{
				ord += 2;
				tmp = toint (bigtmp);
				point.first = i;
				point.second = tmp;
				pt.push_back (point);
				point.second = p-tmp;
				pt.push_back (point);
			}
	}

	ord++; // add point at infinity
	order= ord;

	return pt;
}

ECn findBasePoint (const Big& A, const Big& B, const Big& p, const Big& subgroupOrder, const Big& order)
{
	Big hB, xcoor, value, ycoor, x, y;
	ECn G, P;

	// the subgroup order has been chosen
	if (!prime (subgroupOrder) )
	{
		report (wxT ("The subgroup order must be prime."), wxT ("Note") );
		return ECn();
	}

	// hB = order/subgroupOrder;

	while (true)
	{
		xcoor = rand (p);

		if (is_on_curve (xcoor) )
		{
			value = (xcoor*xcoor*xcoor + A*xcoor + B) % p;
			ycoor = sqrt (value,p);
			P = ECn (xcoor, ycoor);
			// G = hB *P;

			if (!P.iszero() )
				break;
		}
	}

	return P;
}

bool pointOnCurve (ECn& P)
{
	Big x, y;

	P.get (x, y);
	return P.set (x,y);
}

void pointOrder (const ECn& P, const Big& curveOrder, Big& epointOrder)
{
	Big t = curveOrder;
	vector<Factor> f;
	int i;
	ECn P1;

	try
	{
		f = collectBigFactors (curveOrder);
	}

	catch (const schoofException& factErr)
	{
		throw schoofException (factErr.what() );
	}

	for (i=0; i < f.size(); i++)
	{
		t = t/f[i].product;
		P1 = t*P;

		while (!P1.iszero() )
		{
			P1 = (f[i].number) *P1;
			t = t*f[i].number;
		}
	}

	epointOrder = t;

	// cout << "curveOrder/epointOrder = " << curveOrder/epointOrder << endl;
}

/* The code is a translation from Python to C++ of a program verifyrandom.py
 * written by Andrea Cobellini. Frankly, it is much easier to code in Python.
*/
bool verify_curve (const Big& p, const Big& seedE, const Big& A, const Big& B)
{
	/*
	Verifies whether the A and B parameters of the given curve were generated
	from the seed. Returns true if the curve is valid and false if not.
	What follows is the implementation of the verification algorithm
	described in "The Elliptic Curve Digital Signature Algorithm (ECDSA)",
	from Certicom. The document from Certicom allows arbritrary seeds with bit
	length >= 160; here we only use seeds that are exactly 160-bit long.
	*/
	char hash[20], si[20];
	int t, s, v, i, k, q;
	int len,ch;
	sha sh;
	Big h, c0, w0, wi, z, zi, c, x;
	Big one = Big (1);
	char buf[625];
	octet ow0;
	Big two_to_160 = Big ("1461501637330902918203684832716283019655932542976");

	OCTET_INIT (&ow0, 100);

	if (bits (seedE) > 160)
		return false;

	// step 1:

	// define t, s and v as specified in the document.
	t = bits (p);
	s = (t - 1) / 160;
	v = t - 160 * s;

	to_binary (seedE,20,hash,TRUE);

	/* Compute h = SHA-1(seedE) and let c0 denote the bit string of
	   length v bits obtained by taking the v rightmost bits of h.*/
	shs_init (&sh);

	for (i=0; i < 20; i++)
		shs_process (&sh,hash[i]);

	shs_hash (&sh,hash);
	h = from_binary (20,hash);
	c0 = (one << v) - 1;
	c0 = land (c0,h);

	// step 2:

	/*cout << "h = " << h << endl;
	cout << "c0 = " << c0 << endl;*/

	/* Let w0 denote the bit string of length v bits obtained by setting
	   the leftmost bit of c0 to 0.*/
	w0 = (one << (v-1) ) - 1;
	w0 = land (c0,w0);
	convert_big_octet (w0.getbig(),&ow0);

	// step 3:

	/* Let z be the integer whose binary expansion is given by 160-bit string
	   seedE. */
	z = seedE;

	// step 4.

	for (i=1; i <= s; i++)
	{
		/* Let s_i be 160-bit string which is the binary expansion of the
		   integer (z + i) % (2 ** 160) */

		// 2**160 = 1461501637330902918203684832716283019655932542976
		zi = (z + Big (i) ) % two_to_160;
		to_binary (zi,20,si,TRUE);
		shs_init (&sh);

		for (k=0; k < 20; k++)
			shs_process (&sh,si[k]);

		shs_hash (&sh,si);
		OCTET_JOIN_BYTES (si,20,&ow0);
	}

	// step 5.

	/* Let c be the integer whose integer expansion is given by w. */
	big bow0;

	bow0 = mirvar (0);
	OS2FEP (&ow0, bow0);
	c = Big (bow0);

	if (  (B*B*c - A*A*A) % p == 0)
		return true;
	else
		return false;
}

void findVerifyableCurveCoefficients (const Big& p, Big& seed, Big& A, Big& B)
{
	Big h, c0, w0, one = Big (1), z, zi, x, c;
	Big two_to_160 = Big ("1461501637330902918203684832716283019655932542976");
	Big zero = Big ("0");
	sha sh;
	char seedE[20], si[20];
	int t, s, v, i, k;
	octet ow0;
	big bow0, rnd;
	bool complete = false;
	Big myprime, myseed, a, b;

	myprime = p;
	myseed = seed;

	// now build a, b
	OCTET_INIT (&ow0, 200);

	t = bits (p);
	s = (t-1) /160;
	v = t - 160*s;

	while (complete == false)
	{
		OCTET_EMPTY (&ow0);

		/* Compute h = SHA-1(seedE) and let c0 denote the bit string of
		   length v bits obtained by taking the v rightmost bits of h.*/
		rnd = mirvar (0);
		bigbits (160,rnd);
		seed = Big (rnd);

		to_binary (seed,20,seedE,TRUE);
		shs_init (&sh);

		for (i=0; i < 20; i++)
			shs_process (&sh,seedE[i]);

		shs_hash (&sh,seedE);
		h = from_binary (20,seedE);
		c0 = (one << v) - 1;
		c0 = land (c0,h);
		/* Let w0 denote the bit string of length v bits obtained by setting
			the leftmost bit of c0 to 0.*/
		w0 = (one << (v-1) ) - 1;
		w0 = land (c0,w0);
		convert_big_octet (w0.getbig(),&ow0);
		/* Let z be the integer whose binary expansion is given by 160-bit string
			seedE. */
		z = seed;

		for (i=1; i <= s; i++)
		{
			/* Let s_i be 160-bit string which is the binary expansion of the
				integer (z + i) % (2 ** 160) */

			// 2**160 = 1461501637330902918203684832716283019655932542976
			zi = (z + Big (i) ) % two_to_160;
			to_binary (zi,20,si,TRUE);
			shs_init (&sh);

			for (k=0; k < 20; k++)
				shs_process (&sh,si[k]);

			shs_hash (&sh,si);
			// x = from_binary (20,si);
			// convert_big_octet (x.getbig(),&osi);
			OCTET_JOIN_BYTES (si,20,&ow0);
		}

		/* Let c be the integer whose integer expansion is given by ow0. */
		bow0 = mirvar (0);
		OS2FEP (&ow0, bow0);
		c = Big (bow0);

		if ( (c == zero) || ( (4*c + Big (27) ) % p == zero) )
		{
			complete = false;
			// cout << "Making another try." << endl;
		}
		else
			complete = true;
	}


	Big F;

	while (true)
	{
		rnd = mirvar (0);
		bigbits (t,rnd);
		a = Big (rnd);
		// F = a*a*a;
		F = pow(a,3,p);
		F = moddiv (F,c,p);
		b = sqrt (F,p);

		if (b != Big (0) )
			break;
	}

	A = a;
	B = b;

	// now build the order, process, basepoint, and cofactor
}

wxString DecimalToHex (const Big& d)
{
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	                 'A', 'B', 'C', 'D', 'E', 'F'
	                };
	Big decimalBig = d;
	int rem;
	wxString hexstr;

	while (decimalBig != 0)
	{
		rem = decimalBig % 16;
		decimalBig /= 16;
		hexstr = digits[rem] + hexstr;
	}

	return hexstr;
}

wxString BigTowxString(const Big& b)
{
	ostringstream bigToString;
	string str;

	bigToString << b;
	str = bigToString.str();

	return wxString(str.c_str(), wxConvUTF8);
}

Big wxStringToBig(const wxString& str)
{
	char cstring[128];

	strncpy(cstring, (const char*)str.mb_str(wxConvUTF8), 128);
	//cout << "cstring = " << cstring << endl;

	return Big(cstring);
}

void wxStringToCstring(const wxString& str, char* cstr)
{
	strncpy(cstr, (const char*)str.mb_str(wxConvUTF8), 128);
}
