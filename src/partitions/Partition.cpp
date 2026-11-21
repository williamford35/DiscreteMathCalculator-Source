/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

/*
	Thanks to Michael Orlov for this code.
	Modified by William Ford, May 2009
*/

#include <algorithm>
#include <stdexcept>

#include "partitions/Partition.h"
#include "counting-enumeration/NamedSequences.h"
#include "common/listingTooLongException.h"
#include <wx/textctrl.h>

#include "NTL/ZZ.h"

NTL_CLIENT

using namespace std;

Partition::Partition (wxTextCtrl *tctrl) : listing (tctrl)
{}

void Partition::listAllPartitions (int n)
{
	ZZ bellNumber = bell (n);

	if (n > 10)
		throw listingTooLongException ("n must be <= 10");

	// initialize the set
	vector<int> v (n);

	for (int i = 0; i < n; ++i)
		v[i] = i+1;

	Partition::iterator it (n);

	listing->ChangeValue (wxT (""));

	line << "All " << bell (n) << " partitions of " << (n != 8 ? "a " : "an ") << n << "-element set";
	line << "\n\n";
	ouputPartitionLine();

	try
	{
		while (true)
		{
			line << it.subsets() << " : ";

			auto_ptr<vector<vector<int> > > part = it[v];
			line << *part << '\n';
			ouputPartitionLine();

			++it;
		}
	}

	catch (overflow_error&)
	{
		listing->SetInsertionPoint (0);
	}
}

void Partition::listPartitionsSizeK (int n, int k)
{
	ZZ stirlingNumber = stirling (n,k);

	if (n > 10)
		throw listingTooLongException ("n must be <= 25");

	// initialize the set
	vector<int> v (n);

	for (int i = 0; i < n; ++i)
		v[i] = i+1;

	Partition::iterator_k it (n, k);

	listing->ChangeValue (wxT (""));

	line << "All " << stirling (n,k) << " partitions with "
	     << k << " subsets in " << (n != 8 ? "a " : "an ") << n << "-element set:";
	line << "\n\n";
	ouputPartitionLine();

	try
	{
		while (true)
		{
			auto_ptr<vector<vector<int> > >
			part = it[v];
			line << *part << '\n';
			ouputPartitionLine();

			++it;
		}
	}

	catch (overflow_error&)
	{
		listing->SetInsertionPoint (0);
	}
}

void Partition::ouputPartitionLine()
{
	listing->AppendText (wxString (line.str().c_str(),wxConvUTF8));
	line.str ("");
}

Partition::iterator::iterator (unsigned n, bool first) : kappa (n), M (n)
{
	if (n <= 0)
		throw invalid_argument ("Partition listing: n must be positive");

	if (!first)
		for (unsigned i = 1; i < n; ++i)
		{
			kappa[i] = i;
			M[i] = i;
		}

	// integrityCheck();
}

Partition::iterator& Partition::iterator::operator++()
{
	const unsigned n = size();

	for (unsigned i = n-1; i > 0; --i)
		if (kappa[i] <= M[i-1])
		{
			++kappa[i];

			const unsigned new_max = max (M[i], kappa[i]);

			M[i] = new_max;

			for (unsigned j = i + 1; j < n; ++j)
			{
				kappa[j] = 0;
				M[j] = new_max;
			}

			// integrityCheck();
			return *this;
		}

	throw overflow_error ("Partition listing: tried to pass beyond the end of a partition");
}

Partition::iterator& Partition::iterator::operator--()
{
	const unsigned n = size();

	for (unsigned i = n-1; i > 0; --i)
		if (kappa[i] > 0)
		{
			--kappa[i];

			const unsigned m_i = M[i-1];
			M[i] = m_i;

			for (unsigned j = i + 1; j < n; ++j)
			{
				const unsigned new_max = m_i + j - i;

				kappa[j] = new_max;
				M[j] = new_max;
			}

			// integrityCheck();
			return *this;
		}

	throw underflow_error ("Partition listing: tried to move backward at the start of a partition");
}

void Partition::iterator::integrityCheck()
{
	const unsigned n = size();
	unsigned max = kappa[0];

	for (unsigned i = 0; i < n; ++i)
	{
		max = std::max (max,kappa[i]);

		if (max != M[i])
			throw domain_error ("Partition listing: integrity check failed");
	}
}

ostream& operator<< (ostream& out, Partition::iterator &it)
{
	return out;
}

Partition::iterator_k::iterator_k (unsigned n, unsigned psize, bool first)
	: iterator (n, first), psize (psize)
{
	if (psize <= 0 || psize > n)
		throw invalid_argument ("Partition listing: k must be in [1..n]");

	if (first)
	{
		const unsigned offset = n - psize;

		for (unsigned i = offset + 1; i < n; ++i)
		{
			kappa[i] = i - offset;
			M[i] = i - offset;
		}
	}
	else
	{
		fill (kappa.begin() + psize, kappa.end(), psize - 1);
		fill (M.begin() + psize,     M.end(),     psize - 1);
	}
}

Partition::iterator_k& Partition::iterator_k::operator++()
{
	const unsigned n = size();

	for (unsigned i = n-1; i > 0; --i)
		if (kappa[i] < psize-1 && kappa[i] <= M[i-1])
		{
			++kappa[i];

			const unsigned new_max = max (M[i], kappa[i]);
			M[i] = new_max;

			for (unsigned j = i + 1; j <= n - (psize-new_max); ++j)
			{
				kappa[j] = 0;
				M[j] = new_max;
			}

			for (unsigned j = n - (psize-new_max) + 1; j < n; ++j)
			{
				const unsigned new_max = psize - (n - j);

				kappa[j] = new_max;
				M[j] = new_max;
			}

			// integrityCheck();
			return *this;
		}

	throw overflow_error ("Partition listing: tried to pass beyond the end of a k-partition");
}

Partition::iterator_k& Partition::iterator_k::operator--()
{
	const unsigned n = size();

	for (unsigned i = n-1; i > 0; --i)
		if (kappa[i] > 0 && psize - M[i-1] <= n - i)
		{
			--kappa[i];

			const unsigned m_i = M[i-1];
			M[i] = m_i;

			for (unsigned j = i + 1; j < i + (psize - m_i); ++j)
			{
				const unsigned new_max = m_i + j - i;
				kappa[j] = new_max;
				M[j] = new_max;
			}

			for (unsigned j = i + (psize - m_i); j < n; ++j)
			{
				kappa[j] = psize - 1;
				M[j] = psize - 1;
			}

			// integrityCheck();
			return *this;
		}

	throw underflow_error ("Partition listing: tried to move backward at the start of a k-partition");
}

void Partition::iterator_k::integrityCheck()
{
	iterator::integrityCheck();

	if (subsets() != iterator::subsets())
		throw domain_error ("Partiton listing: integrity check failed");
}
