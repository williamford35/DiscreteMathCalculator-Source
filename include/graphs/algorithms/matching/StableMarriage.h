#ifndef STABLEMARRIAGE_H
#define STABLEMARRIAGE_H


#include <iostream>
#include <vector>
#include <list>
#include <string>

#include "common/matrix.h"

using namespace std;

class StableMarriage
{
public:

	StableMarriage (const matrix<int>& men, const matrix<int>& women);
	~StableMarriage();
	vector<int> stable();

private:
	/**
	* Returns true iff w prefers x to y.
	*/
	bool prefers (int w, int x, int y);

	/**
	* Returns a stable marriage in the form an integer vector v,
	* where v[i] is the man married to woman i.
	*/

	// Preference tables (size n x n)
	int n;
	matrix<int> manPref;
	matrix<int> womanPref;
};

#endif // STABLEMARRIAGE_H
