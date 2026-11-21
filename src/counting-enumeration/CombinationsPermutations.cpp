/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

/*
	Thanks to Michael Orlov for this code.
	Modified by William Ford, May 2009
*/

#include <iostream>
#include <iomanip>
#include <sstream>

#include <wx/string.h>
#include "counting-enumeration/CombinationsPermutations.h"
#include "counting-enumeration/useAsymptoticEstimateException.h"
#include "common/listingTooLongException.h"
#include "common/Utils.h"
#include "NTL/ntlException.h"

using namespace std;

bool CombinationsPermutations::get_nk()
{
	bool isn_k = true;

	nValue = nTextEdit->GetValue();

	if (nValue == wxT (""))
		isn_k = false;

	if (isn_k)
	{
		n = atoi ( (const char *) nValue.c_str());

		kValue = kTextEdit->GetValue();

		if (kValue == wxT (""))
			isn_k = false;
		else
			k = atoi ( (const char *) kValue.c_str());
	}

	return isn_k;
}

bool CombinationsPermutations::get_n()
{
	bool isn = true;

	nValue = nTextEdit->GetValue();

	if (nValue == wxT (""))
		isn = false;
	else
		n = atoi ( (const char *) nValue.c_str());

	return isn;
}

bool CombinationsPermutations::get_k()
{
	bool isk = true;

	kValue = kTextEdit->GetValue();

	if (kValue == wxT (""))
		isk = false;
	else
		k = atoi ( (const char *) kValue.c_str());

	return isk;
}

void CombinationsPermutations::getResult()
{
	resultValue = resultTextEdit->GetValue();
	conv (result, (const char *) resultValue.c_str());
}

void CombinationsPermutations::setValue()
{
	// convert ZZ object result back to a string
	ostringstream numstr;

	numstr << result;
	resultTextEdit->ChangeValue (wxString (numstr.str().c_str(),wxConvUTF8));
}

CombinationsPermutations::CombinationsPermutations (wxTextCtrl *nPtr, wxTextCtrl *kPtr,
      wxTextCtrl *resultPtr, wxTextCtrl *listingPtr) :
	nTextEdit (nPtr), kTextEdit (kPtr), resultTextEdit (resultPtr), listingTextEdit (listingPtr)
{}

ZZ CombinationsPermutations::fact (unsigned int n)
{
	ZZ nfact;
	unsigned int i;

	nfact = 1;

	for (i = 2; i <= n; i++)
		nfact = nfact * i;

	return nfact;
}

ZZ CombinationsPermutations::factorial()
{
	ZZ i;

	if (!get_n())
		throw ntlException ("n has no value");

	if (n < 0)
		throw ntlException ("Factorial: n < 0");

	if (n > 25000)
		throw useAsymptoticEstimateException ("n is too large. Do an asymptotic estimate.");
	else
	{
		result = fact (n);
		setValue();
	}

	return result;
}

ZZ CombinationsPermutations::perm (unsigned int n, unsigned int k)
{
	ZZ i, permValue, lastTerm;

	permValue = 1;
	lastTerm = n-k+1;

	for (i=n; i >= lastTerm; i--)
		permValue = permValue * i;

	return permValue;
}

ZZ CombinationsPermutations::permutation()
{
	ZZ i, lastTerm;

	if (!get_nk())
		throw ntlException ("n or k has no value");

	if (n < 0 || k < 0)
		throw ntlException ("Permutation: n or k < 0");

	result = perm (n,k);
	setValue();

	return result;
}

void CombinationsPermutations::listPermutations()
{
	int num_n, num_k;

	if (!get_n())
		throw ntlException ("n has no value");

	num_n = to_int (n);

	if (get_k())
	{
		num_k = to_int (k);

		if (num_k > num_n)
			throw ntlException ("List Permutations: k > n");
	}
	else
		num_k = num_n;

	// How many lines will we have?
	if (perm (num_n, num_k) > 5000)
		throw listingTooLongException ("The enumeration of the permutations will exceed 5000 lines.");

	enumeratePermutations (num_n,num_k);
}

void CombinationsPermutations::perm_lex_next (int n, int p[], bool *more)

//****************************************************************************80
//
//  Purpose:
//
//    PERM_LEX_NEXT generates permutations in lexical order, one at a time.
//
//  Example:
//
//    N = 3
//
//    1   1 2 3
//    2   1 3 2
//    3   2 1 3
//    4   2 3 1
//    5   3 1 2
//    6   3 2 1
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    29 May 2003
//
//  Author:
//
//    FORTRAN77 original version by Mok-Kong Shen.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Mok-Kong Shen,
//    Algorithm 202: Generation of Permutations in Lexicographical Order,
//    Communications of the ACM,
//    Volume 6, September 1963, page 517.
//
//  Parameters:
//
//    Input, int N, the number of elements being permuted.
//
//    Input/output, int P[N], the permutation, in standard index form.
//    The user should not alter the elements of P between successive
//    calls.  The routine uses the input value of P to determine
//    the output value.
//
//    Input/output, bool *MORE.
//    On the first call, the user should set MORE =FALSE which signals
//    the routine to do initialization.
//    On return, if MORE is TRUE then another permutation has been
//    computed and returned, while if MORE is FALSE there are no more
//    permutations.
//
{
	int j, k, temp, u, w;

	//
	//  Initialization.
	//
	if (! (*more))
	{
		i4vec_indicator (n, p);
		*more = true;
	}
	else
	{
		if (n <= 1)
		{
			*more = false;
			return;
		}

		w = n;

		while (p[w-1] < p[w-2])
		{
			if (w == 2)
			{
				*more = false;
				return;
			}

			w = w - 1;
		}

		u = p[w-2];

		for (j = n; w <= j; j--)
		{
			if (u < p[j-1])
			{
				p[w-2] = p[j-1];
				p[j-1] = u;

				for (k = 0; k <= (n - w - 1) / 2; k++)
				{
					temp = p[n-k-1];
					p[n-k-1] = p[w+k-1];
					p[w+k-1] = temp;
				}

				return;
			}
		}
	}

	return;
}

void CombinationsPermutations::perm_print (int n, int p[])

//****************************************************************************80
//
//  Purpose:
//
//    PERM_PRINT prints a permutation of n elements.
//
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 March 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int n, the number of objects.
//
//    Input, int P[N], the permutation, in standard index form.
//
//
{
	int i, ihi, ilo, inc = 20;
	ostringstream ostr;

	for (ilo = 0; ilo < n; ilo = ilo + inc)
	{
		ihi = ilo + inc;

		if (n < ihi)
		{
			ihi = n;
		}

		ostr << "  ";

		for (i = ilo; i < ihi; i++)
		{
			ostr << setw (4) << p[i];
		}

		ostr << endl;
	}

	listingText += wxString (ostr.str().c_str(),wxConvUTF8);
}

RR testCombNumerator (unsigned int n, unsigned int k)
{
	RR value = to_RR (1);
	ZZ i, lastTerm = to_ZZ (n-k+1);

	for (i=to_ZZ (n); i >= lastTerm; i--)
		value = value * to_RR (i);

	return value;
}

ZZ CombinationsPermutations::comb (unsigned int n, unsigned int k)
{
	ZZ combValue;
	unsigned int i, lastTerm;
	RR maxAllowed = to_RR (0.5272) *power (to_RR (10), 4434);

	if (testCombNumerator (n,k) > maxAllowed)
		throw useAsymptoticEstimateException ("Combination value is too large. Do an asymptotic estimate.");

	combValue = 1;
	lastTerm = n-k+1;

	for (i=n; i >= lastTerm; i--)
		combValue = combValue * i;

	if (k > 1600)
		throw useAsymptoticEstimateException ("Combination value is too large. Do an asymptotic estimate.");

	combValue = combValue/fact (k);

	return combValue;
}

ZZ CombinationsPermutations::combination()
{
	if (!get_nk())
		throw ntlException ("n or k has no value");

	if (n < 0 || k < 0)
		throw ntlException ("Combination: n or k < 0");

	result = comb (n,k);
	setValue();

	return result;
}

void CombinationsPermutations::listCombinations()
{
	int *a, num_n, num_k;
	bool done;
	int i;
	ostringstream ostr;

	if (!get_nk())
		throw ntlException ("n or k has no value");

	if (n < 0 || k < 0)
		throw ntlException ("Combination: n or k < 0");

	num_n = n;
	num_k = k;

	// How many lines will we have?
	if (comb (num_n, num_k) > 5000)
		throw listingTooLongException ("The enumeration of the combinations will exceed 5000 lines.");

	enumeratePermutations (num_n,num_k);

	a = new int[num_k];
	done = true;

	for (; ;)
	{
		comb_next (num_n,num_k, a, &done);

		if (done)
		{
			break;
		}

		for (i = 0; i < num_k; i++)
		{
			ostr << setw (6) << a[i];
		}

		ostr << "\n";
	}

	listingTextEdit->ChangeValue (wxString (ostr.str().c_str(),wxConvUTF8));

	delete[] a;
}

void CombinationsPermutations::comb_next (int n, int k, int a[], bool *done)

//****************************************************************************
//
//  Purpose:
//
//    COMB_NEXT computes combinations of K things out of N.
//
//  Discussion:
//
//    The combinations are computed one at a time, in lexicographical order.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    21 June 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Charles Mifsud,
//    Combination in Lexicographic Order,
//    ACM algorithm 154,
//    Communications of the ACM,
//    March 1963.
//
//  Parameters:
//
//    Input, int N, the total number of things.
//
//    Input, int K, the number of things in each combination.
//
//    Input/output, int A[K], contains the list of elements in
//    the current combination.
//
//    Input/output, bool DONE.  Set DONE to FALSE before the first call,
//    and then use the output value from the previous call on subsequent
//    calls.  The output value will be TRUE as long as there are more
//    combinations to compute, and FALSE when the list is exhausted.
//
{
	int i, j;

	if (*done)
	{
		i4vec_indicator (k, a);

		if (1 < k)
		{
			*done = false;
		}
		else
		{
			*done = true;
		}
	}
	else
	{
		if (a[k-1] < n)
		{
			a[k-1] = a[k-1] + 1;
			return;
		}

		for (i = k; 2 <= i; i--)
		{
			if (a[i-2] < n-k+i-1)
			{
				a[i-2] = a[i-2] + 1;

				for (j = i; j <= k; j++)
				{
					a[j-1] = a[i-2] + j - (i-1);
				}

				return;

			}

		}

		*done = true;

	}

	return;
}

void CombinationsPermutations::permuteCombination (int a[], int k)
{
	bool more;
	int *p;

	p = new int[k];
	more = false;

	for (;;)
	{
		perm_lex_next (k, p, &more);

		if (!more)
			break;

		perm_print (a, k, p);
	}

	delete[] p;
}

void CombinationsPermutations::perm_print (int a[], int n, int p[])

//****************************************************************************80
//
//  Purpose:
//
//    PERM_PRINT prints a permutation of n elements whose actual values
//    are in a.
//
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 March 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int n, the number of objects.
//
//    Input, int P[N], the permutation, in standard index form.
//
//
{
	int i, ihi, ilo, inc = 20;

	for (ilo = 0; ilo < n; ilo = ilo + inc)
	{
		ostringstream numstr;

		ihi = ilo + inc;

		if (n < ihi)
		{
			ihi = n;
		}

		numstr << "  ";

		for (i = ilo; i < ihi; i++)
		{
			numstr << setw (4) << a[p[i]-1];
		}

		numstr << endl;
		listingTextEdit->AppendText (wxString (numstr.str().c_str(),wxConvUTF8));
	}
}

void CombinationsPermutations::enumeratePermutations (int n, int k)
{
	// int *a, num_n, num_k;
	int *a;
	bool done;

	listingTextEdit->ChangeValue (wxT (""));

	a = new int[k];
	done = true;

	for (; ;)
	{
		comb_next (n, k, a, &done);

		if (done)
		{
			break;
		}

		// a[i], 0 <= i < k contains the combination
		permuteCombination (a, k);
	}

	delete[] a;

	return;
}
