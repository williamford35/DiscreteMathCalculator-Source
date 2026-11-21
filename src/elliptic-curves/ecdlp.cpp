/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "miracl/big.h"
#include "miracl/ecn.h"
#include "elliptic-curves/ecdlp.h"
#include "elliptic-curves/ecurvelib.h"
#include "elliptic-curves/hashf.h"
#include "common/report.h"

// Miracl precision(200, 10);

int f (const ECn &p, int arg);
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define NUMBER_OF_POLLARD_TRIES 5

#include <iostream>
using namespace std;

bool discreteLogBruteForce ( const ECn& P, const ECn& Q, const Big& n,
                             Big& disclog, double& work_time, Big &iterations)
{
	/*
	 * Given Q = kP, find k.
	 * Compute 1*P, 2*p, ..., n*P until kP matches Q
	*/
	Big k;
	ECn R = P;
	clock_t time_start = clock();

	for(k=Big(1); k<=n; k=k+Big(1))
	{
		if (R == Q)
		{
			work_time = double((clock()-time_start) /(double)CLOCKS_PER_SEC);
			disclog = k;
			iterations = k;
			return true;
		}
		if (double((clock()-time_start)/(double)CLOCKS_PER_SEC) > 60)
		{
			::report(wxT("Time has expired."), wxT("Note"));
			work_time = (clock() - time_start) / (double) CLOCKS_PER_SEC;
			disclog = k;
			return false;
		}
		R += P;
	}
}

bool discreteLogBabyStepGiantStep (const ECn& P, const ECn& Q, const Big& n, Big& disclog, Big& steps, double& work_time)
{
	Big m, a, b;
	flash y;
	double f;
	Big logarithm;
	ECn G, R, S;
	unordered_map<ECn,Big,hFBig> babyStepGiantStepMap;
	clock_t time_start = clock();

	steps = 0;
	m = sqrt (n) + Big (1);

	for (a = 1; a < m; a=a+1)
	{
		G = a*P;
		babyStepGiantStepMap[G] = a;
		if (double((clock()-time_start)/(double)CLOCKS_PER_SEC) > 210)
		{
			::report(wxT("Time has expired."), wxT("Note"));
			work_time = (clock() - time_start) / (double)CLOCKS_PER_SEC;
			return false;
		}
	}

	steps += m;

	int msize = babyStepGiantStepMap.size();

	R = Q;
	S = m * (-P);
	steps += 2;

	for (b = 0; b < m; b=b+1)
	{
		if (babyStepGiantStepMap.find (R) != babyStepGiantStepMap.end() )
		{
			a = babyStepGiantStepMap[R];
			logarithm = a + m*b;
			logarithm = logarithm % n;

			bool check = (logarithm*P == Q);

			work_time = (clock() - time_start) / (double)CLOCKS_PER_SEC;
			if (check == false)
				return false;

			disclog = logarithm;
			return true;
		}

		steps = steps + 1;
		R += S;

		if (double((clock()-time_start)/(double)CLOCKS_PER_SEC) > 210)
		{
			::report(wxT("Time has expired."), wxT("Note"));
			work_time = (clock() - time_start) / (double)CLOCKS_PER_SEC;
			return false;
		}
	}
	return false;
}

// Play a role of an "easy-calculatable" function, that is used in
// Pollards rho-method. This function returns an integer that is calculated
// as a result of bitwise AND operation with arguments arg and
// the lowest bits of a point's X-coordinate.
int f (const ECn &p, int arg)
{
	char d[100];
	Big X;
	int len;

	p.getx (X);
	len = to_binary (X,100,d,FALSE);
	return d[len-1] & arg;
}

// Pollards rho-method ECDLP solver.
bool pollard (ECn& P, ECn& Q, Big& n, Big& disclog, Big& iterations, double& work_time)
{
	int i, j;
	Big x, yy, discretelog;
	clock_t time_start = clock();

	for (int t=0; t< NUMBER_OF_POLLARD_TRIES; t++)
	{
		P.get(x,yy);
		Q.get(x,yy);
		if (!pointOnCurve (P) ) return false;
		if (!pointOnCurve (Q) ) return false;

		iterations = Big(0);

		ECn tempPoint, X1, X2;
		Big c1, d1, c2, d2;
		Big *a = new Big[POLLARD_SET_COUNT];
		Big *b = new Big[POLLARD_SET_COUNT];
		ECn *R = new ECn[POLLARD_SET_COUNT];

		for (i = 0; i < POLLARD_SET_COUNT; i++)
		{
			a[i] = rand (n);
			b[i] = rand(n);
			R[i] = a[i]*P;
			tempPoint = b[i]*Q;
			R[i] += tempPoint;
		}

		c1 = rand (n);
		d1 = rand (n);
		X1 = c1*P;
		tempPoint = d1*Q;
		X1 += tempPoint;
		X2 = X1;
		c2 = c1;
		d2 = d1;

		do
		{
			j = f (X1,POLLARD_SET_ARG);
			X1 += R[j];
			c1 += a[j]%n;
			d1 += b[j]%n;

			for (i = 0; i < 2; i++)
			{
				j = f (X2,POLLARD_SET_ARG);
				X2 += R[j];
				c2 += a[j]%n;
				d2 += b[j]%n;
			}

			if (double((clock()-time_start)/(double)CLOCKS_PER_SEC) > 210)
			{
				::report(wxT("Time has expired."), wxT("Note"));
				work_time = (clock() - time_start) / (double)CLOCKS_PER_SEC;
				return false;
			}
			iterations = iterations + Big(1);
		}
		while (X1 != X2);

		if(d1 == d2)
			continue;
		else
			discretelog = (c1-c2)*inverse(d2-d1,n);

		if (discretelog < 0)
		{
			while (discretelog < 0)
				discretelog += rand(n)* n;
			discretelog = discretelog % n;
		}

		bool check = (discretelog*P == Q);
		if (check == false)
			continue;
		else
		{
			work_time = (clock() - time_start) / (double)CLOCKS_PER_SEC;
			disclog = discretelog;
			return true;
		}
	}
	return false;
}

