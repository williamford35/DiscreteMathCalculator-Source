#ifndef POWERSET_H
#define POWERSET_H

#include "wx/string.h"
#include <set>
#include <vector>

using namespace std;

vector<std::set<char>> powerset (int n);
void subset_by_size_next (int n, char a[], int *size, bool *more);
void ksub_next (int n, int k, char a[], bool *more);
std::set<char> intersection(const std::set<char>& lhs,const std::set<char>& rhs);
bool subset(const std::set<char>& setA, const std::set<char>& setB);

#endif // POWERSET_H
