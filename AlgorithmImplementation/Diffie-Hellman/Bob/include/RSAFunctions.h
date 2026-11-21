#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "miracl/big.h"
#include "wx/string.h"

Big generatePrime(int nbits);
Big eulerPhi(const Big& p, const Big& q);
Big findCoprime(const Big& t);
Big inverseModn(const Big& a, const Big& n);
Big powerModn(const Big& a, const Big& b, const Big& n);
string prepareKey (char *buf);
string prepareMsg (const string& s, unsigned char & padding);
void convertStringToHex (const string& inputStr, wxString& outputStr);
Big getGenerator(const Big& p, long bits);

#endif	// FUNCTIONS_H
