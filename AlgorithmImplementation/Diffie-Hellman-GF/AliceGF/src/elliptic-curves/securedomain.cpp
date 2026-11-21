#include "elliptic-curves/securedomain.h"
#include "elliptic-curves/schoofException.h"
#include "elliptic-curves/ecurvelib.h"

Miracl precision=5000;

namespace
{
FPoly T[25];  // Reduced class Polynomial.
BOOL three = false;
char *s;
Big order, xcoor, ycoor, value, basepointOrder, cofactor;
Big A, B;
int base;
};

/* using prime and seed, arrange that A,B are determined by the prime and seed,
   followed by computing curve order, base point, base point order, and cofactor */
additionalCurveParameters CreateTheCurve(Big& p, Big& seed)
{
	ECn basePoint;
	additionalCurveParameters v;

	// build coeffcients A and B
	findVerifyableCurveCoefficients (p, seed, A, B);
	v.A = A;
	v.B = B;

	// find the curve order
	order = schoof (A, B, p);
	v.order = order;
	// find a basepoint
	while (true)
	{
		xcoor = rand (p);

		if (is_on_curve (xcoor) )
		{
			value = (xcoor*xcoor*xcoor + A*xcoor + B) % p;
			ycoor = sqrt (value,p);
			basePoint = ECn (xcoor, ycoor);
			if (!basePoint.iszero() )
				break;
		}
	}

	v.basePoint = basePoint;

	// find the basepoint order
	pointOrder (basePoint, order, basepointOrder);
	v.basepointOrder = basepointOrder;

	// find the cofactor
	cofactor = order/basepointOrder;
	v.cofactor = cofactor;

	return v;
}

/*
 * cm.cpp - Finding an elliptic curve and point of nearly prime order
 * See IEEE 1363 Annex A for documentation!
 *
 * !!! New much improved version - March 2002
 * !!! Now tested for D up to 10^7
 *
 * !!! New faster version - uses Floats instead of Flashs - November 2003
 * !!! Now 100 times faster!
 * !!! Now tested for D up to and beyond 10^9
 *
 * !!! New invariants - gamma2, w3, w5, w7 and w13. Thanks to Marcel Martin.
 *
 * Sometimes its better to use the -IEEE flag, as this can often be faster
 * than the Gamma2 invariant, as although it requires a bigger "class number",
 * it needs less precision.
 *
 * Uses functions from the MIRACL multiprecision library, specifically
 * classes:-
 * Float   - Big floating point
 * Complex - Big Complex float
 * Big     - Big integer
 * ZZn     - Big integers mod an integer
 * FPoly   - Big Float polynomial
 * Poly    - Big ZZn polynomial
 *
 * Written by Mike Scott, Dublin, Ireland. March 1998 - March 2004
 *
 */

// F(z) Function A.13.3

Complex Fz (Complex z)
{
	Complex t;
	int sign=1;
	Complex osum,sum= (Float) 1;

	if (z.iszero() ) return sum;

	Complex zi=z;
	Complex zj=z*z;
	Complex r=z;
	Complex z3=zj*z;

	forever
	{
		// do 2 terms at a time....
		t=zi+zj;
		osum=sum;

		if (sign) sum-=t;
		else      sum+=t;

		if (sum==osum) break;

		r*=z3;
		zi*=r;
		zj*=r;
		zj*=z;

		sign=1-sign;
	}

	return sum;
}

// Fj(A,B,C) function A.13.3

Complex F (int j,Big A,Big B,Big C,Big D,int N)
{
	Complex t,theta24,theta,theta2,theta3,theta6;
	Float sd;

	if (j>=3)
	{
		// Gamma 2 and Morain's invariants
		sd=-sqrt ( (Float) D);
		t=Complex (sd*fpi(), (fpi() * (Float) B) );
		t/= ( (Float) A);
		theta=exp (t);

		if (j==3)
		{
			theta3=theta;
			theta3*=theta3;
			theta3*=theta;
			theta6=theta3;
			theta6*=theta6;
			t=theta*pow ( (Fz (theta6) /Fz (theta3) ),8);
			return (256*t*t*t+1) /t;
		}

		if (j==4)
		{
			return (pow (Fz (theta) /Fz (pow (theta,N) ),24/ (N-1) ) /theta);
		}

		return 0;
	}

	sd=-sqrt ( (Float) D);
	t=Complex (sd*fpi(), (fpi() * (Float) B) );

	t/= (Float) (24*A);
	theta24=exp (t);                               // theta^(1/24)
	theta=pow (theta24,24);                        // theta

	if (j!=2)
		t=recip (theta24);     // -24th root
	else
		t=theta24*theta24;     // 12th root

	theta2=theta;
	theta2*=theta2;

	if (j==0) return (t*Fz (-theta) /Fz (theta2) );

	if (j==1) return (t*Fz (theta) /Fz (theta2) );

	if (j==2) return (sqrt ( (Float) 2) *t*Fz (theta2*theta2) /Fz (theta2) );

	return 0;
}

int geti (Big D)
{
	Big d=D%8;

	if (d==1 || d==2 || d==6 || d==7) return 3;

	if (d==3)
	{
		if (D%3==0) return 2;
		else        return 0;
	}

	if (d==5) return 6;

	return 0;
}

int getk (Big D)
{
	Big d=D%8;

	if (d==1 || d==2 || d==6) return 2;

	if (d==3 || d==7) return 1;

	if (d==5) return 4;

	return 0;
}

int getN (Big D)
{
	if (D%13==0) return 13;

	if (D%7==0)  return 7;

	if (D%5==0)  return 5;

	if (D%3==0)  return 3;

	return 2;
}

// A.13.3

void class_poly (Complex& lam,Float *Fi2,Big A,Big B,Big C,Big D,BOOL conj,BOOL P1363)
{
	Big ac,l,t;
	int i,j,k,g,e,m,n,dm8;
	Complex cinv;

	if (P1363)
	{
		g=1;

		if (D%3==0) g=3;

		ac=A*C;

		if (ac%2==1)
		{
			j=0;
			l=A-C+A*A*C;
		}

		if (C%2==0)  j=1;

		if (A%2==0)  j=2;

		if (A%2==0)
		{
			t= (C*C-1) /8;

			if (t%2==0) m=1;
			else        m=-1;
		}
		else
		{
			t= (A*A-1) /8;

			if (t%2==0) m=1;
			else        m=-1;
		}

		dm8=D%8;
		i=geti (D);
		k=getk (D);

		switch (dm8)
		{
		case 1:
		case 2:
			n=m;

			if (C%2==0) l=A+2*C-A*C*C;

			if (A%2==0) l=A-C-A*C*C;

			break;

		case 3:
			if (ac%2==1) n=1;
			else         n=-m;

			if (C%2==0) l=A+2*C-A*C*C;

			if (A%2==0) l=A-C+5*A*C*C;

			break;

		case 5:
			n=1;

			if (C%2==0) l=A-C+A*A*C;

			if (A%2==0) l=A-C-A*C*C;

			break;

		case 6:
			n=m;

			if (C%2==0) l=A+2*C-A*C*C;

			if (A%2==0) l=A-C-A*C*C;

			break;

		case 7:
			if (ac%2==0) n=1;
			else         n=m;

			if (C%2==0) l=A+2*C-A*C*C;

			if (A%2==0) l=A-C-A*C*C;

			break;

		default:
			break;
		}

		e= (k*B*l) %48;

		if (e<0) e+=48;

		cinv=pow (lam,e);
		cinv*= (n*Fi2[i]);
		cinv=pow (cinv*pow (F (j,A,B,C,D,0),k),g);
	}
	else
	{
		int N=getN (D);
// adjust A and B

		if (N==2)
		{
			j=3;

			if (A%3!=0)
			{
				if (B%3!=0)
				{
					if ( (B+A+A) %3!=0) B+= (4*A);
					else              B+= (A+A);
				}
			}
			else
			{
				if (B%3!=0)
				{
					if (C%3!=0)
					{
						if ( (C+B) %3!=0) B+= (4*A);
						else            B+= (A+A);
					}
				}
			}

			A*=3;
		}
		else
		{
			j=4;

			if ( (A%N) ==0)
			{
				A=C;
				B=-B;
			}

			while (B%N!=0) B+= (A+A);

			A*=N;
		}

		cinv=F (j,A,B,C,D,N);
	}


// multiply polynomial by new term(s)

	FPoly F;

	if (conj)
	{
		// conjugate pair
		// t^2-2a+(a^2+b^2) , where cinv=a+ib
		F.addterm ( (Float) 1,2);
		F.addterm (-2*real (cinv),1);
		F.addterm (real (cinv) *real (cinv) +imaginary (cinv) *imaginary (cinv),0);
	}
	else
	{
		// t-cinv
		F.addterm ( (Float) 1,1);
		F.addterm (-real (cinv),0);

// store as a linear polynomial, or combine 2 to make a quadratic
		if (T[0].iszero() )
		{
			T[0]=F;
			return;
		}
		else
		{
			F=T[0]*F;      // got a quadratic
			T[0].clear();
		}
	}

// accumulate Polynomial as 2^m degree components
// This allows the use of karatsuba via the "special" function
// This is the time critical bit....

	for (i=1;; i++)
	{
		if (T[i].iszero() )
		{
			T[i]=F;             // store this 2^i degree polynomial
			break;
		}
		else
		{
			F=special (T[i],F); // e.g. if i=1 two quadratics make a quartic..
			T[i].clear();
		}
	}
}

// A.13.2
// Set P1363 to False to calculate class number a la Cohen

int groups (Complex& lam,Float *Fi2,unsigned long D,BOOL doit,BOOL P1363)
{
	unsigned long s,t,A,C,B,TB,lim;
	int cn=0;
	s=mr_lsqrt (D/3,1);

	for (B=0; B<=s; B+=1)
	{
		t=D+B*B;

		if (!P1363)
		{
			if (t%4!=0) continue;
			else t/=4;
		}

		lim=mr_lsqrt (t,1);

		if (P1363) A=2*B;
		else       A=B;

		if (A==0) A+=1;

		for (;;)
		{
			while (t%A!=0)
			{
				A+=1;

				if (A>lim) break;
			}

			if (A>lim) break;

			C=t/A;

			TB=B;

			if (P1363) TB*=2;

			if (lgcd (lgcd (A,TB),C) ==1)
			{
				// output more class group members
				BOOL conj;

				if (TB>0 && C>A && A>TB)
				{
					conj=TRUE;
					cn+=2;

					if (doit)
					{
					}
				}
				else
				{
					conj=FALSE;
					cn+=1;

					if (doit)
					{
						// if (true) cout << "." << flush;
					}
				}

				if (doit) class_poly (lam,Fi2, (Big) A, (Big) B, (Big) C, (Big) D,conj,P1363);
			}

			A+=1;
		}
	}

	return cn;         // class number
}

// check congruence conditions A14.2.1

BOOL isaD (unsigned long d,int pm8,Big k)
{
	unsigned int dm8;
	unsigned long i;
	BOOL sqr;
	dm8=d%8;

	if (k==1 && dm8!=3) return FALSE;

	if ( (k==2 || k==3) && dm8==7) return FALSE;

	if (pm8==3 && (dm8==1 || dm8==4 || dm8==5 || dm8==6) ) return FALSE;

	if (pm8==5 && dm8%2==0) return FALSE;

	if (pm8==7 && (dm8==1 || dm8==2 || dm8==4 || dm8==5) ) return FALSE;

	sqr=FALSE;

	for (i=2;; i++)
	{
		if (d% (i*i) ==0)
		{
			sqr=TRUE;
			break;
		}

		if (i*i>d) break;
	}

	if (sqr) return FALSE;

	return TRUE;
}

// Testing for CM discriminants A.14.2.2

Big floor (Big N,Big D)
{
	Big R;

	if (N==0) return 0;

	if (N>0 && D>0) return N/D;

	if (N<0 && D<0) return (-N) / (-D);

	R=N/D;

	if (N%D!=0) R-=1;

	return R;
}

BOOL isacm (Big p,unsigned long D,Big &W,Big &V)
{
	Big B2,A,B,C,t,X,Y,ld,delta;
	B=sqrt (p- (Big) D,p);
	A=p;
	C= (B*B+ (Big) D) /p;
	X=1;
	Y=0;
	ld=0;

	while (1)
	{
		if (C>=A)
		{
			B2=2*B;

			if (B2<0) B2=-B2;

			if (A>=B2) break;
		}

		delta=floor (2*B+C,2*C);

// are we stuck in hopeless loop? This can't happen now - thanks Marcel

		if (delta==0 && ld==0) return FALSE;

		ld=delta;
		t=X;
		X= (delta*X+Y);
		Y=-t;

		t=C;

		C= (A+C*delta*delta-2*B*delta);
		B= (t*delta-B);
		A=t;
	}

	if (D==11 && A==3)
	{
		t=X;
		X=Y;
		Y=-t;
		B=-B;
		t=C;
		C=A;
		A=t;
	}

	if (D==1 || D==3)
	{
		W=2*X;
		V=2*Y;
		return TRUE;
	}
	else V=0;

	if (A==1)
	{
		W=2*X;
		return TRUE;
	}

	if (A==4)
	{
		W=4*X+B*Y;
		return TRUE;
	}

	return FALSE;
}


// Constructing a Curve (and Point if order is prime)
// A.14.4

BOOL get_curve (Complex& lam,Float *Fi2,const Big& r,const Big& other_r,const Big& ord,unsigned long D,const Big& p,const Big& W,int m,BOOL always,
                Big& prime, Big& A, Big& B, Big& ptOrder, Big& xcoor, Big& ycoor, Big& k)
{
	Poly polly;
	Big A0,B0;
	BOOL unstable;
	// char c;
	int i,j,terms/*class_number, textLen*/;
	BOOL P1363,pord,fout = true;
	// char buf[625];
	wxString s;
	// ostream stream (ECCCMInformationPanelTextCtrl);
	wxString message, caption = wxT ("Building a secure domain");
	// int response;
	miracl *mip = get_mip();

	if (isprime (ord.getbig() ) )
		pord = true;
	else
		pord = false;

	P1363=TRUE;

	if (!always && D>3 && D%8==3) P1363=FALSE;

	k=r;

	while (k%ord==0)  k/=ord;

	/*   if (true)
	   {
	       sprintf (buf, "%ld", D);

	       if (D>1000 && D%3==0) *ECCCMInformationPanelTextCtrl  << "discriminant = " << buf << " (Divisible by 3 - might need extra precision)\n";
	       else *ECCCMInformationPanelTextCtrl  << "discriminant = " << buf << "\n";

	       if (P1363) ; //*ECCCMInformationPanelTextCtrl << "IEEE-1363 invariant\n";
	       else
	       {
	           switch (getN (D) )
	           {
	               case 2:
	                   // *ECCCMInformationPanelTextCtrl << "Gamma2 invariant\n";
	                   break;

	               case 3:
	                   // *ECCCMInformationPanelTextCtrl << "w3 invariant\n";
	                   break;

	               case 5:
	                   // *ECCCMInformationPanelTextCtrl << "w5 invariant\n";
	                   break;

	               case 7:
	                   // *ECCCMInformationPanelTextCtrl << "w7 invariant\n";
	                   break;

	               case 13:
	                   // *ECCCMInformationPanelTextCtrl << "w13 invariant\n";
	                   break;
	           }
	       }

	       sprintf (buf, "%ld", D%24);
	       *ECCCMInformationPanelTextCtrl << "D mod 24 = " << buf << "\n";

	       // buf << k;

	       // if (isprime (ord.getbig() ) ) *ECCCMInformationPanelTextCtrl << "cofactor = " << buf << "\n";
	   } */

	// class_number=groups (lam,Fi2,D,FALSE,P1363);

	// *ECCCMInformationPanelTextCtrl << "class number= " << class_number << "\n";

	if (base == 10)
		mip->IOBASE = 10;
	else
		mip->IOBASE = 16;
	// buf << ord;
	// *ECCCMInformationPanelTextCtrl << "Group order =  " << buf;
	// modulo (p);

// A.14.4.1

	if (D==1)
	{
		A0=1;
		B0=0;
	}

	if (D==3)
	{
		A0=0;
		B0=1;
	}

	if (D!=1 && D!=3)
	{
		FPoly Acc;

		for (i=0; i<25; i++) T[i].clear();

		// if (true) *ECCCMInformationPanelTextCtrl << "constructing polynomial";

		groups (lam,Fi2,D,TRUE,P1363);

// Construct Polynomial from its 2^m degree components..

		for (i=24; i>=0; i--)
		{
			if (!T[i].iszero() )
			{
				Acc=T[i];              // find the first component..
				T[i].clear();
				break;
			}
		}

		if (i>0)
		{
			for (j=i-1; j>0; j--)
			{
				if (!T[j].iszero() )
				{
					Acc=special (Acc,T[j]); // special karatsuba function
					T[j].clear();          // multiply into accumulator
				}
			}

			if (!T[0].iszero() )
			{
				// check for a left-over linear poly
				Acc=Acc*T[0];
				T[0].clear();
			}
		}

		for (i=0; i<25; i++) T[i].clear();

		terms=degree (Acc);
		Float f,rem;
		Big whole;
		int nbits,maxbits=0;

		unstable=FALSE;

		for (i=terms; i>=0; i--)
		{
			f=Acc.coeff (i);

			if (f>0)
				f+=makefloat (1,10000);
			else f-=makefloat (1,10000);

			whole=f.trunc (&rem);
			nbits=bits (whole);

			if (nbits>maxbits) maxbits=nbits;

			polly.addterm ( (ZZn) whole,i);

			if (fabs (rem) >makefloat (1,100) )
			{
				unstable=TRUE;
				break;
			}
		}

		Acc.clear();

		// if (true) *ECCCMInformationPanelTextCtrl << "\n";

		if (unstable)
		{
			// if (true) *ECCCMInformationPanelTextCtrl << "Curve abandoned - numerical instability!\n";

			// if (true) *ECCCMInformationPanelTextCtrl << "Curve abandoned - double precision and try again!\n";

			// if (true) *ECCCMInformationPanelTextCtrl << "finding a curve...\n";

			return FALSE;
		}

		if (true)
		{
			// *ECCCMInformationPanelTextCtrl << "\n";
			// *ECCCMInformationPanelTextCtrl << "Maximum precision required in bits= " << maxbits << "\n";
		}
	}

// save space with smaller miracl

	mirexit();
	mip=mirsys (128,0);
	modulo (p);
	ECn pt,G;
	Big a,b,x,y;
	Big w,eps;
	int at;
	Poly g,spolly=polly;     // smaller polly
	polly.clear();
	forever
	{
		if (D!=1 && D!=3)
		{
			// if (true) *ECCCMInformationPanelTextCtrl << "Factoring polynomial of degree " << degree (spolly) << " .....\n";

			if (P1363)
			{
				if (W%2==0)
				{
					ZZn V;
					g=factor (spolly,1);
					V=-g.coeff (0);
					V=pow (V,24/ (lgcd (D,3) *getk (D) ) );
					V*=pow ( (ZZn) 2, (4*geti (D) ) /getk (D) );

					if (D%2!=0) V=-V;

					A0= (Big) ( (ZZn) (-3) * (V+64) * (V+16) );
					B0= (Big) ( (ZZn) 2* (V+64) * (V+64) * (V-8) );
				}
				else
				{
					Poly V,w,w1,w2,w3,a,b;

					g=factor (spolly,3);

					if (D%3!=0)
						w.addterm (-1,24);
					else
						w.addterm (-256,8);

					V=w%g;
					w.clear();
					w1=V;
					w2=V;
					w3=V;
					w1.addterm (64,0);
					w2.addterm (256,0);
					w3.addterm (-512,0);
					a=w1*w2;
					a.multerm (-3,0);
					a=a%g;
					b=w1*w1*w3;
					b.multerm (2,0);
					b=b%g;

					a= ( (a*a) *a) %g;
					b= (b*b) %g;

					for (int d=degree (g)-1; d>=0; d--)
					{
						ZZn t,c=a.coeff (d);

						if (c!= (ZZn) 0)
						{
							t=b.coeff (d);
							A0= (Big) (c*t);
							B0= (Big) (c*t*t);

							if (d==1) break;
						}
					}
				}
			}
			else
			{
				ZZn X,J,X2,K;

				g=factor (spolly,1);
				X=-g.coeff (0);
				X2=X*X;

				switch (getN (D) )
				{
				case 2:
					J=X2*X;
					break;

				case 3:
					J= (pow ( (X+3),3) * (X+27) ) /X;
					break;

				case 5:
					J=pow ( (X2+10*X+5),3) /X;
					break;

				case 7:
					J= (pow ( (X2+5*X+1),3) * (X2+13*X+49) ) /X;
					break;

				case 13:
					J= (pow ( (X2*X2+7*X2*X+20*X2+19*X+1),3) * (X2+5*X+13) ) /X;

				default:
					break;
				}

				K=J/ (J-1728);
				A0=-3*K;
				B0=2*K;
			}

		}

// A.14.4.2
// but try -3 first, followed by small positive values for A

		a=A0;
		b=B0;
		at=-3;

		if (D==3) at=1;

		forever
		{
			if (D==1)
			{
				if (at<100)
					eps= (ZZn) at/ (ZZn) A0;
				else eps=rand (p);

				a=modmult (A0,eps,p);
			}

			if (D==3)
			{
				if (at<100)
					eps= (ZZn) at/ (ZZn) B0;
				else eps=rand (p);

				b=modmult (B0,eps,p);
			}

			if (D!=1 && D!=3)
			{
				if (at<100)
				{
					// transform a to be simple if possible
					w= (ZZn) at/ZZn (A0);

					if (jacobi (w,p) !=1)
					{
						if (at==-3) at=1;
						else at++;

						continue;
					}

					eps=sqrt (w,p);
				}
				else eps=rand (p);

				a=modmult (A0,pow (eps,2,p),p);
				b=modmult (B0,pow (eps,3,p),p);
			}

			ecurve (a,b,p,MR_PROJECTIVE);

			for (int xc=1;; xc++)
			{
				if (!pt.set ( (Big) xc) ) continue;

				pt*=k;

				if (pt.iszero() ) continue;

				break;
			}

			G=pt;                  // check its not the other one...

			if (r!=ord || ! (other_r*G).iszero() )
			{
				pt*=ord;

				if (pt.iszero() ) break;
			}

			if (at==-3) at=1;
			else at++;
		}

		if (a== (p-3) ) a=-3;

		if (D!=1 && D!=3 && three && a!=-3) continue;

// check MOV condition
// A.12.1

		BOOL MOV=TRUE;

		w=1;

		for (i=1; i<100; i++)
		{
			w=modmult (w,p,ord);

			if (w==1)
			{
				MOV=FALSE;

				if (true)
				{
					/*if (i==1 || pord) *ECCCMInformationPanelTextCtrl << "\n*** Failed MOV condition - k = " << i << "\n";
					else *ECCCMInformationPanelTextCtrl << "\n*** Failed MOV condition - k <= " << i << "\n";*/
				}

				break;
			}
		}

		/* if (true)
		 {
		     // if (MOV)  *ECCCMInformationPanelTextCtrl << "MOV condition OK\n";

		     if (pord) *ECCCMInformationPanelTextCtrl << "\nCurve and Point Found\n";
		     else      *ECCCMInformationPanelTextCtrl << "\nCurve Found\n";
		 }

		 if (base == 10)
		     mip->IOBASE = 10;
		 else
		     mip->IOBASE = 16;
		 buf << a;
		 *ECCCMInformationPanelTextCtrl << "A = " << buf << "\n";
		 buf << b;
		 *ECCCMInformationPanelTextCtrl << "B = " << buf << "\n";
		 G.get (x,y);
		 buf << p;
		 *ECCCMInformationPanelTextCtrl << "P = " << buf << "\n";
		 buf << ord;

		 if (pord)
		 {
		     *ECCCMInformationPanelTextCtrl << "R = " << buf << "," ;
		     *ECCCMInformationPanelTextCtrl << " a " << bits (ord) << " bit prime"  << "\n";
		 }
		 else
		     *ECCCMInformationPanelTextCtrl << "R= " << buf << "\n" ;

		 if (pord)
		 {
		     buf << x;
		     *ECCCMInformationPanelTextCtrl << "X= " << buf << "\n";
		     buf<< y;
		     *ECCCMInformationPanelTextCtrl << "Y= " << buf << "\n";
		 }
		 else
		     *ECCCMInformationPanelTextCtrl << " NOT prime\n";

		 *ECCCMInformationPanelTextCtrl << "\n";

		 if (D!=1 && D!=3 )
		 {
		     message.Clear();
		     message.append (wxT ("Is this random factorisation OK (Y/N)?") );
		     wxMessageDialog dialog (this, message, caption, wxYES_NO | wxICON_QUESTION);
		     response = dialog.ShowModal();

		     if (response == wxID_NO)
		         continue;
		 }*/

		break;
	}

	message.Clear();

	/*if (pord)
	    message.append (wxT ("Curve and Point OK (Y/N)?") );
	else
	    message.append (wxT ("Curve OK (Y/N)?") );

	wxMessageDialog dialog2 (this, message, caption, wxYES_NO | wxICON_QUESTION);
	response = dialog2.ShowModal();

	if (response == wxID_NO)
	{
	    if (true) *ECCCMInformationPanelTextCtrl << "finding another curve...\n";

	    mirexit();
	    int precision= (1<<m);
	    mip=mirsys (precision+2,0);    // restart with new precision
	    setprecision (m);
	    gprime (1000000);
	    mip->RPOINT=ON;

	    return FALSE;
	}*/

	if (fout)
	{
		prime = p;
		A = a;
		B = b;
		ptOrder = ord;

		if (pord)
		{
			xcoor = x;
			ycoor = y;
		}
	}

	return TRUE;
}

// Code to parse formula
// This code isn't mine, but it is in the public domain
// Shamefully I forget the source
//
// NOTE: It may be necessary on some platforms to change the operators * and #

#if defined(unix)
#define TIMES '.'
#define RAISE '^'
#else
#define TIMES '*'
#define RAISE '#'
#endif

void eval_cmpower (Big& oldn,Big& n,char op)
{
	if (op) n=pow (oldn,toint (n) ); // power(oldn,size(n),n,n);
}

void eval_cmproduct (Big& oldn,Big& n,char op)
{
	switch (op)
	{
	case TIMES:
		n*=oldn;
		break;

	case '/':
		n=oldn/n;
		break;

	case '%':
		n=oldn%n;
	}
}

void eval_cmsum (Big& oldn,Big& n,char op)
{
	switch (op)
	{
	case '+':
		n+=oldn;
		break;

	case '-':
		n=oldn-n;
	}
}

void eval (Big& t)
{
	Big oldn[3];
	Big n;
	int i;
	char oldop[3];
	char op;
	char minus;

	for (i=0; i<3; i++)
	{
		oldop[i]=0;
	}

LOOP:

	while (*s==' ')
		s++;

	if (*s=='-')    /* Unary minus */
	{
		s++;
		minus=1;
	}
	else
		minus=0;

	while (*s==' ')
		s++;

	if (*s=='(' || *s=='[' || *s=='{')    /* Number is subexpression */
	{
		s++;
		eval (t);
		n=t;
	}
	else            /* Number is decimal value */
	{
		for (i=0; s[i]>='0' && s[i]<='9'; i++)
			;

		//if (!i)         /* No digits found */
		//  {
		// *ECCCMInformationPanelTextCtrl <<  "Error - invalid number\n";
		// throw schoofException ("");
		//  }

		op=s[i];
		s[i]=0;
		n=atoi (s);
		s+=i;
		*s=op;
	}

	if (minus) n=-n;

	do
		op=*s++;

	while (op==' ');

	if (op==0 || op==')' || op==']' || op=='}')
	{
		eval_cmpower (oldn[2],n,oldop[2]);
		eval_cmproduct (oldn[1],n,oldop[1]);
		eval_cmsum (oldn[0],n,oldop[0]);
		t=n;
		return;
	}
	else
	{
		if (op==RAISE)
		{
			eval_cmpower (oldn[2],n,oldop[2]);
			oldn[2]=n;
			oldop[2]=RAISE;
		}
		else
		{
			if (op==TIMES || op=='/' || op=='%')
			{
				eval_cmpower (oldn[2],n,oldop[2]);
				oldop[2]=0;
				eval_cmproduct (oldn[1],n,oldop[1]);
				oldn[1]=n;
				oldop[1]=op;
			}
			else
			{
				if (op=='+' || op=='-')
				{
					eval_cmpower (oldn[2],n,oldop[2]);
					oldop[2]=0;
					eval_cmproduct (oldn[1],n,oldop[1]);
					oldop[1]=0;
					eval_cmsum (oldn[0],n,oldop[0]);
					oldn[0]=n;
					oldop[0]=op;
				}
				else    /* Error - invalid operator */
				{
					// *ECCCMInformationPanelTextCtrl <<  "Error - invalid operator\n";
					throw schoofException ("");
				}
			}
		}
	}

	goto LOOP;
}

bool cm (Big& prime, Big& A, Big& B, Big& ptOrder, Big& Gx, Big& Gy,
         Big& cofactor, int m)
{
	return true;
}

