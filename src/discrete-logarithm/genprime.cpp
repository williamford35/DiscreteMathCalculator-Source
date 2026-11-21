/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

/*
 *   Program to generate "trap-door primes".
 *   Generates primes for use with "index" program
 *
 *   Hard to find factors of the product of two of them n=p.q
 *   - but easy to find discrete logs wrt p & q
 *   See "Non-Interactive Public-Key Cryptography", Maurer & Yacobi
 *   Eurocrypt '91
 *
 *   Requires: big.cpp crt.cpp
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/time.h>

#include "miracl/big.h"   /* include MIRACL system */
#include "discrete-logarithm/genprime.h"
#include "common/randomc.h"

// #define NPRIMES 9  /* =9 for > 256 bit primes */
#define PROOT 2

using namespace std;

Big genprime (int NPRIMES, vector<Big>& factors)
{
	/* program to find a trap-door prime */
	BOOL found;
	int i,spins;
	long seed;
	Big s;
	Big *pp = new Big[NPRIMES],q,p,t;
	CRandomMersenne rnd;
	char rndDigit;
	string digit;

	for (i=0; i<9; i++)
	{
		rndDigit = (char)rnd.IRandomX (48, 57);
		digit += rndDigit;
	}

	digit.clear();

	for (i=0; i<4; i++)
	{
		rndDigit = (char)rnd.IRandomX(48,57);
		digit += rndDigit;
	}

	pp[0]=2;

	do
	{
		/* find prime p = 2.pp[1].pp[2]....+1 */
		p=2;

		for (i=1; i<NPRIMES-1; i++)
		{
			/* generate all but last prime */
			q=rand (i+6,10);
			pp[i]=nextprime (q);
			p*=pp[i];
		}

		do
		{
			/* find last prime component such that p is prime */
			q=nextprime (q);
			pp[NPRIMES-1]=q;
			t=p*pp[NPRIMES-1];
			t+=1;
		}
		while (!prime (t) );

		p=t;
		found=TRUE;

		for (i=0; i<NPRIMES; i++)
		{
			/* check that PROOT is a primitive root */
			if (pow (PROOT, (p-1) /pp[i],p) ==1)
			{
				found=FALSE;
				break;
			}
		}
	}
	while (!found);

	factors.clear();

	for (i=0; i<NPRIMES; i++)
		factors.push_back (pp[i]);

	delete[] pp;

	cout << "p = " << p << endl;
	return p;
}
