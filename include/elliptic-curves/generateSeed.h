#ifndef GENERATE_SEED_H
#define GENERATE_SEED_H

#include "miracl/big.h"

Big generateSeed(int n)
{
	big rnd;
	Big myBigSeed;
	miracl *mip = get_mip();

	mip->IOBASE = 10;
	rnd = mirvar (0);
	bigbits (n,rnd);
	myBigSeed = Big (rnd);
	return myBigSeed;
}

#endif	// GENERATE_SEED
