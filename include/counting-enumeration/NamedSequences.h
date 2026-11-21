#ifndef NAMED_SEQUENCES_H
#define NAMED_SEQUENCES_H

#include "NTL/ntlException.h"
#include "NTL/ZZ.h"

NTL_CLIENT

ZZ schroder (unsigned long n);
ZZ catalan (unsigned long n);
ZZ fibonacci (unsigned long n);
ZZ bell (unsigned long n);
ZZ stirling (unsigned int n, unsigned int k);

#endif
