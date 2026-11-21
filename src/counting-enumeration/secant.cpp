/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <cmath>
#include <float.h>

using namespace std;

// secant finds a root of f using the Secant method.
// Input variables:
// f      A function that calculates f for a given x.
// a, b   Two points in the neighborhood of the root (on either side, or the
//        same side of the root).
// tol    Maximum error tolerance.
// n      Maximum number of iterations
// Output variable:
// xr     Approximate root

double secant (double f (double x),double a,double b,double tol,int n)
{
	double xi_minus1, xi, xi_plus1, fval_xi, xr;
	int i;

	xi_minus1 = a;
	xi = b;

	for (i=1; i <= n; i++)
	{
		fval_xi = f (xi);
		xi_plus1 = xi - fval_xi* (xi_minus1-xi) / (f (xi_minus1)-fval_xi);

		if (abs ( (xi_plus1 - xi) /xi) < tol)
		{
			xr = xi_plus1;
			break;
		}

		xi_minus1 = xi;
		xi = xi_plus1;
	}

	if (i > n)
		xr = DBL_MAX;

	return xr;
}
