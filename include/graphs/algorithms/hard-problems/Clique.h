#ifndef CLIQUE_H_INCLUDED
#define CLIQUE_H_INCLUDED

#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

/*
  INPUT: Graph G = (V,E) must given :
  V ... ARRAY[1..N] of vertices
  E ... ARRAY of edges for each vertex (there are as many arrays as there are vertices)
*/


#define MAX_VERTICES 4000

class ELEMENT
{
public:
	int point;
	int N;
};

class ARRAY
{
public:
	int *ele;
	int size;
	int first;
};

class ARRAY2
{
public:
	ELEMENT *ele;
	int size;
	int first;
};

class ARRAY_FIX
{
public:
	int ele[MAX_VERTICES];
	int size;
	int first;
};

class Clique
{
public:
	Clique (const string& dimacs_input, double nlevel);
	~Clique();
	void findMaximumClique();
	vector<int> maximumClique();

private:
	int CUT1 (int p, ARRAY_FIX &B);
	int CUT2 (int p, ARRAY2 B, ARRAY2 &C);
	void NUMBER_SORT (ARRAY2 &R);
	void COPY (ARRAY_FIX &A, ARRAY_FIX B);
	void DEGREES_SORT (ARRAY2 R);
	void EXPAND (ARRAY2 R, int level);
	void MCQ();
	void read_dimacs (istringstream& f);

	ARRAY_FIX *C;
	ARRAY *E;
	ARRAY2 V;
	ARRAY_FIX Q;
	ARRAY_FIX QMAX;
	int maxno;
	int max_degree;
	int pk, nk, nk_size;
	int **e;
	ARRAY2 S;
	double num_level, dens;
};


#endif // CLIQUE_H_INCLUDED
