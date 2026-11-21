/**************************   mersenne.cpp   **********************************
* Author:        Agner Fog
* Date created:  2001
* Last modified: 2008-11-16
* Project:       randomc.h
* Platform:      Any C++
* Description:
* Random Number generator of type 'Mersenne Twister'
*
* This random number generator is described in the article by
* M. Matsumoto & T. Nishimura, in:
* ACM Transactions on Modeling and Computer Simulation,
* vol. 8, no. 1, 1998, pp. 3-30.
* Details on the initialization scheme can be found at
* http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
*
* Further documentation:
* The file ran-instructions.pdf contains further documentation and
* instructions.
*
* Copyright 2001-2008 by Agner Fog.
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*******************************************************************************/

#include "randomc.h"
#include <time.h>

void CRandomMersenne::Init0 (int seed)
{
	// Seed generator
	const uint32_t factor = 1812433253UL;
	mt[0]= seed;

	for (mti=1; mti < MERS_N; mti++)
	{
		mt[mti] = (factor * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
	}
}

void CRandomMersenne::RandomInit (int seed)
{
	if (seed == 0)
	{
		// get time of day in seconds since 12:00 AM, January 1, 1970
		/*long t_time = long(time(NULL));

		// mix-up bits by squaring
		t_time *= t_time;

		// result can overflow. handle cases > 0, < 0, = 0
		if (t_time > 0)
			seed = t_time ^ 0x5EECE66DL;
		else if (t_time < 0)
			seed = (t_time & 0x7fffffff) ^ 0x5EECE66DL;
		else
			seed = 0x5EECE66DL;*/
		seed = time (0);
	}

	// Initialize and seed
	Init0 (seed);

	// Randomize some more
	for (int i = 0; i < 37; i++)
		BRandom();
}


uint32_t CRandomMersenne::BRandom()
{
	// Generate 32 random bits
	uint32_t y;

	if (mti >= MERS_N)
	{
		// Generate MERS_N words at one time
		const uint32_t LOWER_MASK = (1LU << MERS_R) - 1;       // Lower MERS_R bits
		const uint32_t UPPER_MASK = 0xFFFFFFFF << MERS_R;      // Upper (32 - MERS_R) bits
		static const uint32_t mag01[2] = {0, MERS_A};

		int kk;

		for (kk=0; kk < MERS_N-MERS_M; kk++)
		{
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk+MERS_M] ^ (y >> 1) ^ mag01[y & 1];
		}

		for (; kk < MERS_N-1; kk++)
		{
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk+ (MERS_M-MERS_N)] ^ (y >> 1) ^ mag01[y & 1];
		}

		y = (mt[MERS_N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[MERS_N-1] = mt[MERS_M-1] ^ (y >> 1) ^ mag01[y & 1];
		mti = 0;
	}

	y = mt[mti++];

	// Tempering (May be omitted):
	y ^=  y >> MERS_U;
	y ^= (y << MERS_S) & MERS_B;
	y ^= (y << MERS_T) & MERS_C;
	y ^=  y >> MERS_L;

	return y;
}


double CRandomMersenne::Random()
{
	// Output random float number in the interval 0 <= x < 1
	// Multiply by 2^(-32)
	return (double) BRandom() * (1./ (65536.*65536.));
}


int CRandomMersenne::IRandomX (int min, int max)
{
	// Output random integer in the interval min <= x <= max
	// Each output value has exactly the same probability.
	// This is obtained by rejecting certain bit values so that the number
	// of possible bit values is divisible by the interval length
	if (max <= min)
	{
		if (max == min) return min;
		else return 0x80000000;
	}

	uint32_t interval;                    // Length of interval
	uint32_t bran;                        // Random bits
	uint32_t iran;                        // bran / interval
	uint32_t remainder;                   // bran % interval

	interval = uint32_t (max - min + 1);

	if (interval != LastInterval)
	{
		// Interval length has changed. Must calculate rejection limit
		// Reject when iran = 2^32 / interval
		// We can't make 2^32 so we use 2^32-1 and correct afterwards
		RLimit = (uint32_t) 0xFFFFFFFF / interval;

		if ( (uint32_t) 0xFFFFFFFF % interval == interval - 1) RLimit++;
	}

	do   // Rejection loop
	{
		bran = BRandom();
		iran = bran / interval;
		remainder = bran % interval;
	}
	while (iran >= RLimit);

	// Convert back to signed and return result
	return (int32_t) remainder + min;
}
