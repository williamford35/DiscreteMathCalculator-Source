#ifndef ECURVELIB_H
#define ECURVELIB_H

#include "miracl/ecn.h"
#include "miracl/p1363.h"

#include "wx/string.h"
#include "wx/textctrl.h"

#include "miracl/big.h"
#include "miracl/flash.h"

#include <vector>
using namespace std;

vector<pair<long,long> > ecBruteForceOrder (long& a, long& b, long& p);
Big schoof (const Big& A, const Big& B, Big& p);
ECn findBasePoint (const Big& A, const Big& B, const Big& p, const Big& subgroupOrder, const Big& order);
bool pointOnCurve (ECn& P);
void pointOrder (const ECn& P, const Big& curveOrder, Big& basePointOrder);
void convert_big_octet (_MIPD_ big w,octet *s);
void OS2FEP(_MIPD_ octet *s,big w);
bool verify_curve (const Big& p, const Big& seedE, const Big& A, const Big& B);
wxString DecimalToHex (const Big& d);
wxString BigTowxString(const Big& b);
Big wxStringToBig(const wxString& str);
void wxStringToCstring(const wxString& str, char* cstr);
void findVerifyableCurveCoefficients (const Big& prime, Big& seed, Big& A, Big& B);
void factorOrder(const Big& curveOrder, wxString& strBasePointOrder);

#endif // ECURVELIB_H