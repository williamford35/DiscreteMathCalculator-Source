#ifndef ELLIPTIC_CURVE_FUNCTIONS_H
#define ELLIPTIC_CURVE_FUNCTIONS_H

#include "miracl/big.h"
#include "miracl/ecn.h"
#include "miracl/p1363.h"

struct p_and_seed
{
	Big p;
	Big seed;
};

struct additionalCurveParameters
{
	Big A;
	Big B;
	ECn basePoint;
	Big order;
	Big basepointOrder;
	Big cofactor;
};

p_and_seed generate_p_and_seed(long numbits);
Big generatePrime (int nbits);
Big generateBig(int nbits);
additionalCurveParameters CreateTheCurve(Big& p, Big& seed);
void findVerifyableCurveCoefficients (const Big& p, Big& seed, Big& A, Big& B);
bool verify_curve (const Big& p, const Big& seedE, const Big& A, const Big& B);
void pointOrder (const ECn& P, const Big& curveOrder, Big& epointOrder);
void convert_big_octet (_MIPD_ big w,octet *s);
void OS2FEP (_MIPD_ octet *s,big w);

#endif // ELLIPTIC_CURVE_FUNCTIONS_H

