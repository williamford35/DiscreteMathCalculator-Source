/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/matching/StableMarriage.h"

bool StableMarriage::prefers (int w, int x, int y)
{
	for (int i = 0; i < n; i++)
	{
		int pref = womanPref[w][i];

		if (pref == x)
			return true;

		if (pref == y)
			return false;
	}

	return true;
}

StableMarriage::StableMarriage (const matrix<int>& men, const matrix<int>& women) :
	manPref (men), womanPref (women)
{
	n = manPref.rows();
}

StableMarriage::~StableMarriage()
{
}

vector<int> StableMarriage::stable()
{
	// Indicates that woman i is currently engaged to
	// the man v[i].
	int NOT_ENGAGED = -1;
	vector<int> current (n, NOT_ENGAGED);
	// List of men that are not currently engaged.
	list<int> freeMen;

	for (int i=0; i < n; i++)
		freeMen.push_back (i);

	// next[i] is the next woman to whom i has not yet proposed.
	vector<int> next (n,0);

	while (!freeMen.empty())
	{
		int m = freeMen.front();
		int w = manPref[m][next[m]];

		freeMen.pop_front();
		next[m]++;

		if (current[w] == NOT_ENGAGED)
			current[w] = m;
		else
		{
			int m1 = current[w];

			if (prefers (w, m, m1))
			{
				current[w] = m;
				freeMen.push_back (m1);
			}
			else
			{
				freeMen.push_back (m);
			}
		}
	}

	return current;
}
