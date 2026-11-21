/*
 *   Elliptic Curve Digital Signature Algorithm (ECDSA)
 *
 *
 *   This program generates one set of public and private keys in files
 *   public.ecs and private.ecs respectively. Notice that the public key
 *   can be much shorter in this scheme, for the same security level.
 *
 *   It is assumed that Curve parameters are to be found in file common.ecs
 *
 *   The curve is y^2=x^3+Ax+B mod p
 *
 *   The file common.ecs is presumed to exist, and to contain the domain
 *   information {p,A,B,q,x,y}, where A and B are curve parameters, (x,y) are
 *   a point of order q, p is the prime modulus, and q is the order of the
 *   point (x,y). In fact normally q is the prime number of points counted
 *   on the curve.
 *
 *   Requires: big.cpp ecn.cpp
 */

#include <iostream>
#include <fstream>
#include "ecn.h"
#include "string.h"
#include <cstdlib>
using namespace std;

// if MR_STATIC defined, it should be 20

#ifndef MR_NOFULLWIDTH
Miracl precision=20;
#else
Miracl precision(20,MAXBASE);
#endif

int main(int argc, char *argv[])
{
	char buf[256];
	char ecsLocation[256], publicLocation[256],
	     privateLocation[256] ;
	strcpy(buf,argv[1]);
	strcpy(ecsLocation, buf);
	strcat(ecsLocation,"/common.ecs");
	strcpy(publicLocation, buf);
	strcat(publicLocation, "/public.ecs");
	strcpy(privateLocation, buf);
	strcat(privateLocation, "/private.ecs");

	//cout << "publicLocation = " << publicLocation << endl;
	//cout << "privateLocation = " << privateLocation << endl;
	ifstream common(ecsLocation);    /* construct file I/O streams */
	ofstream public_key(publicLocation);
	ofstream private_key(privateLocation);
	int bits,ep;
	miracl *mip=&precision;

	ECn G,W;
	Big a,b,p,q,x,y,d;
	long seed;
	int digit, tentoi = 1;

	for (int i=0; i<10; i++)
	{
		digit = std::rand()%10;
		seed =  digit*tentoi;
		tentoi *= 10;
	}
	irand(seed);

	common >> bits;
	mip->IOBASE=10;
	common >> p >> a >> b >> q >> x >> y;

	ecurve(a,b,p,MR_PROJECTIVE);

	if (!G.set(x,y))
	{
		cout << "Problem - point (x,y) is not on the curve" << endl;
		return 0;
	}

	W=G;
	W*=q;

	if (!W.iszero())
	{
		cout << "Problem - point (x,y) is not of order q" << endl;
		return 0;
	}

	/* generate public/private keys */

	d=rand(q);
//   for (int i=0;i<=10000;i++)
	G*=d;
	ep=G.get(x);
	public_key << ep << " " << x << endl;
	cout << "public key = (" << ep << ", " << x << ")" << endl;
	private_key << d << endl;
	cout << "private key = " << d << endl;

	return 0;
}

