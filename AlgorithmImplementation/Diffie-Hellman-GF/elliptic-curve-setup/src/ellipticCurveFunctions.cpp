#include "ellipticCurveFunctions.h"
#include "schoof.h"
#include "schoofException.h"
#include "collectBigFactors.h"
#include "miracl/p1363.h"
#include <NTL/ZZ.h>
#include <sstream>

// Miracl precision=5000;

p_and_seed generate_p_and_seed(long numbits)
{
	Big myPrime;
	p_and_seed v;
	miracl *mip = mirsys(5000,2);
	Big seed = Big("551747651955593436870873524161067349837666988529");
	mip->IOBASE = 10;

	myPrime = generatePrime(numbits);
	v.p = myPrime;
	v.seed = seed;

	return v;
}

NTL_CLIENT

Big generatePrime (int nbits)
{
	ZZ ntlkbitprime;
	Big kbitprime;
	ostringstream numstr;
	char buf[500];
	miracl *mip;
	// std::setprecision(m);

	mip = mirsys(nbits,0);
	ntlkbitprime = RandomPrime_ZZ ( (long) nbits );
	numstr << ntlkbitprime;
	strcpy (buf, (char *) numstr.str().c_str() );
	// cout << "buf = " << buf << endl;
	kbitprime = Big (buf);

	return kbitprime;
}

/* using prime and seed, arrange that A,B are determined by the prime and seed,
   followed by computing curve order, base point, base point order, and cofactor */
additionalCurveParameters CreateTheCurve(Big& p, Big& seed)
{
	ECn basePoint;
	additionalCurveParameters v;
	Big A, B, order, xcoor, ycoor;
	Big value, basepointOrder, cofactor;

	// build coeffcients A and B
	findVerifyableCurveCoefficients (p, seed, A, B);
	v.A = A;
	v.B = B;

	// find the curve order
	order = schoof (A, B, p);
	v.order = order;
	// find a basepoint
	while (true)
	{
		xcoor = rand (p);

		if (is_on_curve (xcoor) )
		{
			value = (xcoor*xcoor*xcoor + A*xcoor + B) % p;
			ycoor = sqrt (value,p);
			basePoint = ECn (xcoor, ycoor);
			if (!basePoint.iszero() )
				break;
		}
	}

	v.basePoint = basePoint;

	// find the basepoint order
	pointOrder (basePoint, order, basepointOrder);
	v.basepointOrder = basepointOrder;

	// find the cofactor
	cofactor = order/basepointOrder;
	v.cofactor = cofactor;

	return v;
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
}

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