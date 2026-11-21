#ifndef COLLECTBIGFACTORS_H
#define COLLECTBIGFACTORS_H

#include <vector>
#include <string>

#include "miracl/big.h"

using namespace std;

struct Factor
{
	int k;
	Big number;
	Big product;
};

vector<Factor> collectBigFactors(const Big& n);

#endif // COLLECTBIGFACTORS_H