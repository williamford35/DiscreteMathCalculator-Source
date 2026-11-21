#include "miracl/big.h"
#include "miracl/ecn.h"
#include "elliptic-curves/ecdlp.h"
#include "elliptic-curves/ecurvelib.h"
#include "elliptic-curves/hashf.h"
#include "report.h"

int f (const ECn &p, int arg);
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define NUMBER_OF_POLLARD_TRIES 5

#include <iostream>
using namespace std;

#ifdef _WIN32
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
		// cout << "Time expired = " << double((clock()-time_start)/(double)CLOCKS_PER_SEC) <<  endl;
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
			logarithm = logarithm%n;

			bool check = (logarithm*P == Q);
			if (check == false)
				return false;

			disclog = logarithm;
			work_time = (clock() - time_start) / (double)CLOCKS_PER_SEC;
			return true;
		}

		steps = steps + 1;
		R += S;
		// cout << "Time expired = " << double((clock()-time_start)/(double)CLOCKS_PER_SEC) <<  endl;

		if (double((clock()-time_start)/(double)CLOCKS_PER_SEC) > 210)
		{
			::report(wxT("Time has expired."), wxT("Note"));
			return false;
		}
	}
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

	for (int t=0; i< NUMBER_OF_POLLARD_TRIES; t++)
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

			// cout << "Time expired = " << double((clock()-time_start)/(double)CLOCKS_PER_SEC) <<  endl;
			if (double((clock()-time_start)/(double)CLOCKS_PER_SEC) > 180)
			{
				::report(wxT("Time has expired."), wxT("Note"));
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
			work_time = (clock() - work_time) / (double) CLOCKS_PER_SEC;
			disclog = discretelog;
			return true;
		}
	}
}

#else
static bool timer_expired = false;
static void alarm_handler(int sig)
{
	timer_expired = true;
}

struct sigaction init_ctx = {.sa_handler = alarm_handler};
struct sigaction* ctx = &init_ctx;

bool discreteLogBruteForce ( const ECn& P, const ECn& Q, const Big& n,
                             Big& disclog, double& work_time, Big &iterations)
{
	/*
	 * Given Q = kP, find k.
	 * Compute 1*P, 2*p, ..., n*P until kP matches Q
	*/
	Big k;
	ECn R = P, PP = P;

	// Set up a signal hander to call alarm_handler() when SIGALRM is raised
	sigaction(SIGALRM, ctx, NULL);

	timer_expired = false;
	alarm(60); // raise SIGALRM in 60 seconds

	while (!timer_expired)
	{
		work_time = clock();

		for(k=Big(1); k<=n && !timer_expired; k=k+Big(1))
		{
			if (R == Q)
				break;
			R += P;
		}
	}
	if(timer_expired)
	{
		::report(wxT("Time has expired."), wxT("Note"));
		work_time = (clock() - work_time) / (double) CLOCKS_PER_SEC;
		disclog = k;

		return false;
	}

	work_time = (clock() - work_time) / (double) CLOCKS_PER_SEC;
	disclog = k;
	iterations = k;

	return true;
}

bool discreteLogBabyStepGiantStep (const ECn& P, const ECn& Q, const Big& n, Big& disclog, Big& steps, double& work_time)
{
	Big m, a, b;
	Big logarithm;
	ECn G, R, S;
	unordered_map<ECn,Big,hFBig> babyStepGiantStepMap;
	double wktime;

	wktime = clock();

	steps = 0;
	m = sqrt (n) + Big (1);

	// Set up a signal hander to call alarm_handler() when SIGALRM is raised
	sigaction(SIGALRM, ctx, NULL);
	timer_expired = false;
	alarm(210); // raise SIGALRM in 70 seconds

	while (!timer_expired)
	{
		for (a = 1; a < m && !timer_expired; a=a+1)
		{
			G = a*P;
			babyStepGiantStepMap[G] = a;
		}
		if (timer_expired)
			break;

		steps += m;

		R = Q;
		S = m * (-P);
		steps += 2;

		for (b = 0; b < m && !timer_expired; b=b+1)
		{
			if (babyStepGiantStepMap.find (R) != babyStepGiantStepMap.end() )
			{
				a = babyStepGiantStepMap[R];
				logarithm = a + m*b;
				logarithm = logarithm%n;

				bool check = (logarithm*P == Q);
				if (check == false)
					return false;

				disclog = logarithm;
				work_time = (clock() - wktime) / (double)CLOCKS_PER_SEC;
				return true;
			}

			steps = steps + 1;
			R += S;
		}
		if (timer_expired)
		{
			::report(wxT("Time has expired."), wxT("Note"));
			return false;
		}
	}
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
	int i=0, j;
	Big x, yy, discretelog;

	// Set up a signal hander to call alarm_handler() when SIGALRM is raised
	sigaction(SIGALRM, ctx, NULL);
	timer_expired = false;
	alarm(180); // raise SIGALRM in 180 seconds

	for (int t=0; i< NUMBER_OF_POLLARD_TRIES; t++)
	{
		work_time = clock();

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

			iterations = iterations + Big(1);
		}
		while (X1 != X2 && !timer_expired);

		if (timer_expired)
		{
			::report(wxT("pollard rho did not succeed in five executions of the algorithm."), wxT("Note"));
			return false;
		}

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
			work_time = (clock() - work_time) / (double) CLOCKS_PER_SEC;
			disclog = discretelog;
			return true;
		}
	}
}

#endif
