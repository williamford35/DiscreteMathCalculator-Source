/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "RelationsPartialOrders/pset.h"

#include <wx/msgdlg.h>

#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

// modification of member function listAllSubsets from class Subsets
vector<set<char> > powerset (int n)
{
	vector<set<char> > latticeVertex;

	if (n > 6)
	{
		wxMessageDialog dialog (NULL, wxT("The Hasse diagram has more than 64 vertices."), wxT("Note"), wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return latticeVertex;
	}

	char *a = new char[n];
	int i, size;
	bool more;
	set<char> subset;

	for (i=0; i < n; i++)
		a[i] = 'a' + i;

	more = false;

	latticeVertex.clear();

	for (;;)
	{
		subset_by_size_next (n, a, &size, &more);

		if (size > 0)
		{
			subset.insert(a[0]-1);

			for (i = 1; i < size-1; i++)
				subset.insert(a[i]-1);

			if (size > 1)
				subset.insert(a[size-1]-1);

			latticeVertex.push_back(subset);
		}
		else
		{
			subset.clear();
			latticeVertex.push_back(subset);
		}

		if (!more)
			break;

		subset.clear();
	}

	delete[] a;
	return latticeVertex;
}


//****************************************************************************80

// modification of member function subset_by_size from class Subsets
void subset_by_size_next (int n, char a[], int *size, bool *more)
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
	if (*size > 0)
	{
		ksub_next (n, *size, a, &more2);
	}
	else if (*size == 0)
	{
		*more = false;
	}
}

void ksub_next (int n, int k, char a[], bool *more)
{
	int j;
	static int m = 0;
	static int m2 = 0;

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
		m2 = a[k-m] - 'a';
	}

	for (j = 1; j <= m; j++)
	{
		a[k+j-m-1] = m2 + 'a' + j;
	}

	*more = (a[0] - 'a' != (n-k+1));
}

bool subset(const set<char>& setA, const set<char>& setB)
{
	return intersection(setA, setB).size() == setA.size();
}

set<char> intersection (const set<char>& setA, const set<char>& setB)
{
	set<char> setIntersection;
	char item;
	set<char>::const_iterator iterA = setA.begin();

	while (iterA != setA.end())
	{
		item = *iterA;

		if (setB.find(item) != setB.end())
			setIntersection.insert(item);

		iterA++;
	}

	return setIntersection;
}
