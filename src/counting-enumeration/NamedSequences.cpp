/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <vector>
#include <string>
using namespace std;

#include "counting-enumeration/NamedSequences.h"
#include "counting-enumeration/useAsymptoticEstimateException.h"

ZZ schroder (unsigned long n)

//****************************************************************************80
//
//  Purpose:
//
//    SCHRODER generates the Schroder numbers.
//
//  Discussion:
//
//    The Schroder number S(n) counts the number of ways to move one step
//    at a time from (0,0) to (n,n) in the first quadrant of the plane
//    if for each point XR = (xr,yr) the next point is
//    (xr+1,yr), (xr, yr+1), or (xr+1,yr+1).
//    S(0) = 1
//    S(1) = 2
//
//    S(n-1) = 1
//    S(n-2) = 1
//    S(n) = ( ( 6 * n - 9 ) * S(n-1) - ( n - 3 ) * S(n-2) ) / n,
//    2 <= n <= n+1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Author:
//
//    John Burkardt
//		Modified by William Ford 04 May 2009
//
//  Reference:
//
//    R P Stanley,
//    Hipparchus, Plutarch, Schroeder, and Hough,
//    American Mathematical Monthly,
//    Volume 104, Number 4, 1997, pages 344-350.
//
//    Laurent Habsieger, Maxim Kazarian, Sergei Lando,
//    On the Second Number of Plutarch,
//    American Mathematical Monthly, May 1998, page 446.
//
//  Parameters:
//
//    Input n, Schroder number desired.
//
//    Output, the Schroder number n.
//
{
	ZZ sn_minus2, sn_minus1, sn;
	unsigned long i;

	/*if ( n <= 0 )
	{
	  sn = -1;
	  return sn;
	}*/

	if (n == 0)
		return to_ZZ (1);
	else if (n == 1)
		return to_ZZ (2);

	if (n > 50000)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");

	sn_minus2 = 1;
	sn_minus1 = 1;

	for (i = 2; i <= n+1; i++)
	{
		sn = ( (6 * i - 9) * sn_minus1 - (i - 3) * sn_minus2) / i;
		sn_minus2 = sn_minus1;
		sn_minus1 = sn;
	}

	return sn;
}

//****************************************************************************80

ZZ catalan (unsigned long n)

//****************************************************************************80
//
//  Purpose:
//
//    CATALAN computes the Catalan numbers, from C(0) to C(N).
//
//  First values:
//
//     C(0)     1
//     C(1)     1
//     C(2)     2
//     C(3)     5
//     C(4)    14
//     C(5)    42
//     C(6)   132
//     C(7)   429
//     C(8)  1430
//     C(9)  4862
//    C(10) 16796
//
//  Formula:
//
//    C(N) = (2*N)! / ( (N+1) * (N!) * (N!) )
//         = 1 / (N+1) * COMB ( 2N, N )
//         = 1 / (2N+1) * COMB ( 2N+1, N+1).
//
//  Recursion:
//
//    C(N) = 2 * (2*N-1) * C(N-1) / (N+1)
//    C(N) = sum ( 1 <= I <= N-1 ) C(I) * C(N-I)
//
//  Discussion:
//
//    The Catalan number C(N) counts:
//
//    1) the number of binary trees on N vertices;
//    2) the number of ordered trees on N+1 vertices;
//    3) the number of full binary trees on 2N+1 vertices;
//    4) the number of well formed sequences of 2N parentheses;
//    5) the number of ways 2N ballots can be counted, in order,
//       with N positive and N negative, so that the running sum
//       is never negative;
//    6) the number of standard tableaus in a 2 by N rectangular Ferrers diagram;
//    7) the number of monotone functions from [1..N} to [1..N} which
//       satisfy f(i) <= i for all i;
//    8) the number of ways to triangulate a polygon with N+2 vertices.
//
//  Example:
//
//    N = 3
//
//    ()()()
//    ()(())
//    (()())
//    (())()
//    ((()))
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 May 2003
//
//  Author:
//
//    John Burkardt
//		Modified by William Ford 04 April 2009
//
//  Reference:
//
//    Dennis Stanton, Dennis White,
//    Constructive Combinatorics,
//    Springer, 1986,
//    ISBN: 0387963472,
//    LC: QA164.S79.
//
//  Parameters:
//
//    Input, int N, the number of Catalan numbers desired.
//
//    Output, int C[N+1], the Catalan numbers from C(0) to C(N).
//
{
	ZZ cn_minus1, cn;
	unsigned long i;

	/*if ( n < 0 )
	{
	  cn = -1;
	  return cn;
	}*/

	if (n <= 1)
	{
		cn = 1;
		return cn;
	}

	if (n > 50000)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");

	cn_minus1 = 1;

//
//  The extra parentheses ensure that the integer division is
//  done AFTER the integer multiplication.
//
	for (i = 2; i <= n; i++)
	{
		cn = (cn_minus1 * 2 * (2 * i - 1)) / (i + 1);
		cn_minus1 = cn;
	}

	return cn;
}

ZZ fibonacci (unsigned long n)
{
	ZZ fi_m2, fi_m1, fi;
	unsigned long i;

	if (n > 300000)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");

	if (n <= 1)
	{
		fi = n;
		return fi;
	}

	fi_m2 = 0;
	fi_m1 = 1;

	for (i=2; i <= n; i++)
	{
		fi = fi_m1 + fi_m2;
		fi_m2 = fi_m1;
		fi_m1 = fi;
	}

	return fi;
}


/* This algorithm is based on the Java package ca.ubc.cs.kisynski.bell developed by
   author Jacek Kisynski */

// First sixteen Bell numbers (n = 0, ..., 15)
unsigned long intBells[] = {1,1,2,5,15,52,203,877,4140,21147,115975,678570,4213597,27644437,190899322,1382958545};
vector<unsigned long> smallBellNumbers (intBells, intBells+16);


// The first Bell number (n = 0)
ZZ bigOnes[] = {to_ZZ (1), to_ZZ (1) };
vector<ZZ> bigBellNumbers (bigOnes, bigOnes+2);

// The second Bell number (n = 1)
ZZ *bigBellTriangleRow;
bool firstCall = true;


/**
* Computes n-th Bell number using Bell triangle.
*
* To compute n-th Bell number for the first time we use two arrays of size n +1 and n and perform (n^2 - n) / 2 additions. Once we have computed
* n-th Bell number, all predecessing Bell numbers are computed and stored in memory to avoid unnecessary recomputation.
*/

/* ZZ bell (unsigned long n)
{
	if (n < 0)
		throw ntlException("n less then zero.");

	if (n > 2500)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");

	if (firstCall)
	{
		bigBellTriangleRow = new ZZ[1];
		bigBellTriangleRow[0] = to_ZZ (1);
		firstCall = false;
	}

	if (n >= smallBellNumbers.size())
	{
		if (n >=bigBellNumbers.size())
		{
			// Create bigger lookup table
			unsigned long m = bigBellNumbers.size();
			ZZ *temp, *save;

			bigBellNumbers.resize (n+1);

			// Fill in empty space
			for (unsigned long i = m; i < bigBellNumbers.size(); i++)
			{
				// Update Bell triangle row
				temp = new ZZ[i];
				temp[0] = bigBellNumbers[i - 1];

				for (unsigned long j = 1; j < i; j++)
					temp[j] = temp[j-1] + bigBellTriangleRow[j-1];

				save = bigBellTriangleRow;
				bigBellTriangleRow = temp;
				delete[] save;
            delete[] temp;

				bigBellNumbers[i] = bigBellTriangleRow[i - 1];
			}
		}

		return bigBellNumbers[n];
	}
	else
		return to_ZZ (smallBellNumbers[n]);

   delete bigBellTriangleRow;
}*/

/* Slow algorithm, but for some reason the algorithm above stopped working
 * due to NTL failure to allocate memory even for reasonably small n.
 */

ZZ bell(unsigned long n)
{
	ZZ belln = to_ZZ(0);

	if (n > 350)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");

	if (n < 0)
		throw ntlException("n less then zero.");

	for (int i=1; i<=n; i++)
		belln += stirling(n,i);

	return belln;
}

/**
	Computes the Stirling number of the second kind S(n,k) using the Stirling triangle.
*/
void nextStirlingRow (unsigned int n, vector<ZZ>& stirlingTriangleRow)
{
	unsigned int k;
	vector<ZZ> temp (&stirlingTriangleRow[0], &stirlingTriangleRow[n-1] + 1);

	stirlingTriangleRow[n] = to_ZZ (1);

	for (k=2; k <= n-1; k++)
		stirlingTriangleRow[k] = temp[k-1] + k*temp[k];
}

ZZ stirling (unsigned int n, unsigned int k)
{
	vector<ZZ> stirlingTriangleRow (n+1);
	unsigned int i;

	if (n > 2500)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");

	if (n == 1 && k == 1)
		return to_ZZ (1);

	stirlingTriangleRow[0] = to_ZZ (0);
	stirlingTriangleRow[1] = to_ZZ (1);

	for (i=2; i <= n; i++)
		nextStirlingRow (i, stirlingTriangleRow);

	return stirlingTriangleRow[k];
}
