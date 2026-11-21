/*****************************   randomc.h   **********************************
* Author:        Agner Fog
* Date created:  1997
* Last modified: 2008-11-16
* Project:       randomc.h
* Source URL:    www.agner.org/random
*
* Description:
* This header file contains class declarations and other definitions for the
* randomc class library of uniform random number generators in C++ language.
*
* Overview of classes:
* ====================
*
* class CRandomMersenne:
* Random number generator of type Mersenne twister.
* Source file mersenne.cpp
*
* Member functions (methods):
* ===========================
*
* Constructor(int seed):
* The seed can be any integer. The time may be used as seed.
* Executing a program twice with the same seed will give the same sequence
* of random numbers. A different seed will give a different sequence.
*
* void RandomInit(int seed);
* Re-initializes the random number generator with a new seed.
*
* double Random();
* Gives a floating point random number in the interval 0 <= x < 1.
* The resolution is 32 bits.
*
* int IRandom(int min, int max);
* Gives an integer random number in the interval min <= x <= max.
* (max-min < MAXINT).
* The precision is 2^-32 (defined as the difference in frequency between
* possible output values). The frequencies are exact if max-min+1 is a
* power of 2.
*
* int IRandomX(int min, int max);
* The frequencies of all output values are exactly the same for an
* infinitely long sequence. (Only relevant for extremely long sequences).
*
* uint32_t BRandom();
* Gives 32 random bits.
*******************************************************************************/

#ifndef RANDOMC_H
#define RANDOMC_H

typedef signed int          int32_t;
typedef unsigned int       uint32_t;
//typedef long long           int64_t;
//typedef unsigned long long uint64_t;


/***********************************************************************
Define the random number generator class
***********************************************************************/

class CRandomMersenne
{
	// Encapsulate random number generator
// Choose which version of Mersenne Twister you want:
#define MERS_N   624
#define MERS_M   397
#define MERS_R   31
#define MERS_U   11
#define MERS_S   7
#define MERS_T   15
#define MERS_L   18
#define MERS_A   0x9908B0DF
#define MERS_B   0x9D2C5680
#define MERS_C   0xEFC60000

public:
	CRandomMersenne (int seed = 0)
	{
		// Constructor
		RandomInit (seed);
		LastInterval = 0;
	}

	void RandomInit (int seed = 0);     // Re-seed
	int IRandomX (int min, int max);    // Output random integer
	double Random();                    // Output random float
	uint32_t BRandom();                 // Output random bits
private:
	void Init0 (int seed);              // Basic initialization procedure
	uint32_t mt[MERS_N];                // State vector
	int mti;                            // Index into mt
	uint32_t LastInterval;              // Last interval length for IRandomX
	uint32_t RLimit;                    // Rejection limit used by IRandomX
};

#endif
