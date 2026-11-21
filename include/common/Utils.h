#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>

#include "NTL/ZZ.h"
#include <wx/string.h>
#include <wx/font.h>

using namespace std;

NTL_CLIENT
void i4vec_indicator (int n, int a[]);
int zzdigits (const ZZ& n);
int getStringExtent (const wxString& vertexLabel, const wxFont& font);

#endif
