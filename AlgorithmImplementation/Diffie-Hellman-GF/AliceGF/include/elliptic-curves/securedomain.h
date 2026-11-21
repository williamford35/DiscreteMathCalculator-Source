#ifndef CONSTRUCTSECUREVERIFYABLECURVE_H
#define CONSTRUCTSECUREVERIFYABLECURVE_H

#include <iostream>
#include <cmath>
#include <cstring>
#include "miracl/complex.h"
#include "miracl/flpoly.h"
#include "miracl/poly.h"
#include "miracl/big.h"
#include "miracl/ecn.h"
#include "miracl/zzn.h"
#include "elliptic-curves/securedomain.h"

#include "elliptic-curves/ecurvelib.h"

using namespace std;

struct additionalCurveParameters
{
	Big A;
	Big B;
	ECn basePoint;
	Big order;
	Big basepointOrder;
	Big cofactor;
};

/* FPoly T[25];  // Reduced class Polynomial.
BOOL three = false;
char *s;
Big order, xcoor, ycoor, value, basepointOrder, cofactor;
Big A, B;*/
additionalCurveParameters CreateTheCurve(Big& p, Big& seed);
Complex Fz (Complex z);
Complex F (int j,Big A,Big B,Big C,Big D,int N);

int geti (Big D);
int getk (Big D);
int getN (Big D);
void class_poly (Complex& lam,Float *Fi2,Big A,Big B,Big C,Big D,BOOL conj,BOOL P1363);
int groups (Complex& lam,Float *Fi2,unsigned long D,BOOL doit,BOOL P1363);
BOOL isaD (unsigned long d,int pm8,Big k);
Big floor (Big N,Big D);
BOOL isacm (Big p,unsigned long D,Big &W,Big &V);
BOOL get_curve (Complex& lam,Float *Fi2,const Big& r,const Big& other_r,const Big& ord,unsigned long D,const Big& p,const Big& W,int m,BOOL always,
                Big& primeValue, Big& A, Big& B, Big& ptOrder, Big& xcoor, Big& ycoor, Big& cofactor);
void eval_cmpower (Big& oldn,Big& n,char op);
void eval_cmproduct (Big& oldn,Big& n,char op);
void eval_cmsum (Big& oldn,Big& n,char op);
void eval (Big& t);
bool cm (Big& prime, Big& A, Big& B, Big& ptOrder, Big& Gx, Big& Gy, Big& cofactor, int m);

#endif // secure domain header