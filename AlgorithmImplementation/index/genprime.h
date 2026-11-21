#ifndef GENPRIME_H
#define GENPRIME_H

#include "miracl/big.h"

#include <vector>
using namespace std;

Big genprime(int NPRIMES, vector<Big>& factors);

#endif	// GENPRIME_H