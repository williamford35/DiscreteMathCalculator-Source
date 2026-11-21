/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

/*
	Thanks to Michael Orlov for this code.
	Modified by William Ford, May 2009
*/

#include "subsets/Subsets.h"
#include "common/Utils.h"
#include "common/listingTooLongException.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Subsets::Subsets (wxTextCtrl *listing, wxTextCtrl *subsets, wxTextCtrl *kEltSubsets,
                  CombinationsPermutations *cp) :
	subsetsListing (listing), numSubsets (subsets),
	numKElementSubsets (kEltSubsets), perm_comb (cp)
{}

void Subsets::numberOfSubsets (unsigned int n)
{
	ostringstream powerSetSizeStream;
	ZZ sizePowerSet;
	wxString powerSetSizeText;

	sizePowerSet = power2_ZZ (n);
	powerSetSizeStream << sizePowerSet;

	powerSetSizeText = wxString (powerSetSizeStream.str().c_str(),wxConvUTF8);
	numSubsets->ChangeValue (powerSetSizeText);
}

void Subsets::numberOfKElementSubsets (unsigned int n, unsigned int k)
{
	ostringstream numKEltSubsetsStream;
	ZZ numKEltSubsets;
	wxString numKElementSubs;

	numKEltSubsets = perm_comb->comb (n, k);
	numKEltSubsetsStream << numKEltSubsets;

	numKElementSubs = wxString (numKEltSubsetsStream.str().c_str(),wxConvUTF8);
	numKElementSubsets->ChangeValue (numKElementSubs);
}

void Subsets::listAllSubsets (unsigned int n)
{
	unsigned int *a = new unsigned int[n], i, size;
	bool more;
	ostringstream subsets;
	wxString line;
	ZZ numKEltSubsets;

	if (power2_ZZ (n) > 4096)
		throw listingTooLongException ("The listing of the powerset will exceed 5000 lines.");

	more = false;

	subsetsListing->ChangeValue (wxT (""));

	for (;;)
	{
		subset_by_size_next (n, a, &size, &more);

		if (0 < size)
		{
			subsets << "{" << a[0];

			for (i = 1; i < size-1; i++)
				subsets << setw (4) << a[i];

			if (size > 1)
				subsets << setw (4) << a[size-1] << "}\n";
			else
				subsets << "}\n";

			line = wxString (subsets.str().c_str(),wxConvUTF8);
			subsetsListing->AppendText (line);
		}
		else
			subsetsListing->AppendText (wxT ("{}"));

		if (!more)
			break;

		subsets.str ("");
	}

	delete[] a;
}

//****************************************************************************80

void Subsets::subset_by_size_next (unsigned int n, unsigned int a[],
                                   unsigned int *size, bool *more)

//****************************************************************************80
//
//  Purpose:
//
//    SUBSET_BY_SIZE_NEXT returns all subsets of an N set, in order of size.
//
//  Example:
//
//    N = 4:
//
//    1 2 3 4
//    1 2 3
//    1 2 4
//    1 3 4
//    1 3
//    1 4
//    2 3
//    1
//    2
//    3
//    (the empty set)
//
//  Discussion:
//
//    The subsets are returned in decreasing order of size, with the
//    empty set last.
//
//    For a given size K, the K subsets are returned in lexicographic order.
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
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the size of the set.
//
//    Input/output, int A[N].  The entries A(1:SIZE) contain
//    the elements of the subset.  The elements are given in ascending
//    order.
//
//    Output, int *SIZE, the number of elements in the subset.
//
//    Input/output, bool *MORE.  Set MORE = FALSE before first call
//    for a new sequence of subsets.  It then is set and remains
//    TRUE as long as the subset computed on this call is not the
//    final one.  When the final subset is computed, MORE is set to
//    FALSE as a signal that the computation is done.
//
{
	static bool more2 = false;

	if (! (*more))
	{
		*more = true;
		more2 = false;
		*size = n;
	}
	else if (!more2)
	{
		*size = *size - 1;
	}

//
//  Compute the next subset of size SIZE.
//
	if (0 < *size)
	{
		ksub_next (n, *size, a, &more2);
	}
	else if (*size == 0)
	{
		*more = false;
	}
}

void Subsets::listAllKElementSubsets (unsigned int n, unsigned int k)
{
	if (k == 0)
	{
		subsetsListing->ChangeValue (wxT ("{}"));
		return;
	}

	unsigned int *a = new unsigned int[k], i;
	bool more;
	ostringstream ksubsets;
	wxString line;
	ZZ numKEltSubsets;

	if (perm_comb->comb (n, k) > 5000)
		throw listingTooLongException ("The listing of the subsets will exceed 5000 lines.");

	for (i = 0; i < k; i++)
		a[i] = 0;

	more = false;

	subsetsListing->ChangeValue (wxT (""));

	for (;;)
	{
		ksub_next (n, k, a, &more);

		ksubsets << "{" << a[0];

		for (i = 1; i < k-1; i++)
			ksubsets << setw (4) << a[i];

		if (k > 1)
			ksubsets << setw (4) << a[k-1] << "}\n";
		else
			ksubsets << "}\n";

		line = wxString (ksubsets.str().c_str(),wxConvUTF8);
		subsetsListing->AppendText (line);

		if (!more)
			break;

		ksubsets.str ("");
	}

	delete[] a;
}

//****************************************************************************80

void Subsets::ksub_next (unsigned int n, unsigned int k, unsigned int a[], bool *more)

//****************************************************************************80
//
//  Purpose:
//
//    KSUB_NEXT generates the subsets of size K from a set of size N.
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
//    FORTRAN77 original version by Albert Nijenhuis, Herbert Wilf.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms for Computers and Calculators,
//    Second Edition,
//    Academic Press, 1978,
//    ISBN: 0-12-519260-6,
//    LC: QA164.N54.
//
//  Parameters:
//
//    Input, int N, the size of the set from which subsets are drawn.
//
//    Input, int K, the desired size of the subsets.  K must
//    be between 0 and N.
//
//    Output, int A[K].  A[I] is the I-th element of the
//    subset.  Thus A[I] will be an integer between 1 and N.
//    Note that the routine will return the values in A
//    in sorted order: 1 <= A[0] < A[1] < ... < A[K-1] <= N
//
//    Input/output, bool *MORE.  Set MORE = FALSE before first call
//    for a new sequence of subsets.  It then is set and remains
//    TRUE as long as the subset computed on this call is not the
//    final one.  When the final subset is computed, MORE is set to
//    FALSE as a signal that the computation is done.
//
{
	unsigned int j;
	unsigned static int m = 0;
	unsigned static int m2 = 0;

	if (n < k)
	{
		ostringstream errorstr;

		errorstr << "Fatal error!\n";
		errorstr << "  n = " << n << "\n";
		errorstr << "  k = " << k << "\n";
		errorstr << "  but 0 <= k <= n is required!\n";
		delete[] a;
		throw ntlException (errorstr.str().c_str());
	}

	if (! (*more))
	{
		m2 = 0;
		m = k;
	}
	else
	{
		if (m2 < n-m)
		{
			m = 0;
		}

		m = m + 1;
		m2 = a[k-m];
	}

	for (j = 1; j <= m; j++)
	{
		a[k+j-m-1] = m2 + j;
	}

	*more = (a[0] != (n-k+1));
}
