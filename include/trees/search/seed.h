//---------------------------------------------------------------------------
//
// Name:        bigintcalcApp.cpp
// Author:      William Ford
// Created:     3/18/2008 11:09:34 AM
// Description:
//
//---------------------------------------------------------------------------

#include <time.h>

NTL_CLIENT

// get a seed for the NTL random number generator
ZZ getSeed()
{
	ZZ seed;

	// get time of day in seconds since 12:00 AM, January 1, 1970
	long t_time = long (time (NULL)), s;

	// mix-up bits by squaring
	t_time *= t_time;

	// result can overflow. handle cases > 0, < 0, = 0
	if (t_time > 0)
		s = t_time ^ 0x5EECE66DL;
	else if (t_time < 0)
		s = (t_time & 0x7fffffff) ^ 0x5EECE66DL;
	else
		s = 0x5EECE66DL;

	seed = s;

	return seed;
}
