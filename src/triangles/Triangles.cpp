/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

/*
	Thanks to Michael Orlov for this code.
	Modified by William Ford, May 2009
*/

#include "triangles/Triangles.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "NTL/ZZ.h"
#include "common/listingTooLongException.h"
#include "counting-enumeration/NamedSequences.h"
#include "common/matrix.h"
#include "triangles/OutputZZ.h"
#include "common/Utils.h"
#include <wx/sstream.h>

using namespace std;

NTL_CLIENT

Triangles::Triangles (wxTextCtrl *output) : triangleOutput (output)
{}

void Triangles::outputPascalTriangle (unsigned int n)
{
	matrix<ZZ> P (n+1,n+1);
	unsigned int i,j;
	ostringstream line;
	wxString triangleLine;
	wxString w;

	triangleOutput->ChangeValue (w);

	P[0][0] = 1;

	for (i = 1; i <= n; i++)
	{
		P[i][0] = 1;
		P[i][i] = 1;

		for (j = 1; j < i; j++)
			P[i][j] = P[i-1][j-1] + P[i-1][j];
	}

	for (i=0; i <= n; i++)
	{
		for (j=0; j <= i; j++)
			line << outputZZLeft (zzdigits (P[n][j]) +2, P[i][j]);

		line << endl;
		triangleLine = wxString (line.str().c_str(), wxConvUTF8);
		triangleOutput->AppendText (triangleLine);
		line.str ("");
	}
}

void Triangles::outputHosoyaTriangle (unsigned int n)
{
	matrix<ZZ> H (n+1,n+1);
	int i,j;
	wxString triangleLine;
	ostringstream decDigits, line;

	triangleOutput->ChangeValue (wxT (""));

	H[0][0] = H[1][0] = H[1][1] = H[2][1] = 1;

	for (i = 2; i <= (int) n; i++)
	{
		for (j = 0; j <= i; j++)
		{
			// find H(i,j) using one of the formulas
			// H(i,j) = H(i-1,j) + H(i-2,j)
			//        or
			// H(i,j) = H(i-1,j-1) + H(i-2,j-2)
			// In most cases, either one can be used; however
			// near the diagonals, only one applies

			if (i == 2 && j == 1)
				continue;

			if ( ( (i-1) >= 0 && (i-2) >= 0) && j <= (i-1))
				H[i][j] = H[i-1][j] + H[i-2][j];
			else
				H[i][j] = H[i-1][j-1] + H[i-2][j-2];
		}
	}

	for (i=0; i <= (int) n; i++)
	{
		for (j=0; j <= i; j++)
			line << outputZZLeft (zzdigits (H[n][j]) +2, H[i][j]);

		line << endl;
		triangleLine = wxString (line.str().c_str(), wxConvUTF8);
		triangleOutput->AppendText (triangleLine);
		line.str ("");
	}
}

void Triangles::outputCatalanTriangle (unsigned int n)
{
	matrix<ZZ> C (n+1,n+1);
	unsigned int i,j;
	ostringstream line;
	wxString triangleLine;

	triangleOutput->ChangeValue (wxT (""));

	C[0][0] = 1;
	C[1][0] = C[1][1] = 1;

	for (i = 2; i <= n; i++)
	{
		C[i][0] = 1;

		for (j = 1; j < i; j++)
			C[i][j] = C[i][j-1] + C[i-1][j];

		C[i][i] = C[i][i-1];
	}

	for (i=0; i <= n; i++)
	{
		for (j=0; j <= i; j++)
			line << outputZZLeft (zzdigits (C[n][j]) +2, C[i][j]);

		line << endl;
		triangleLine = wxString (line.str().c_str(), wxConvUTF8);
		triangleOutput->AppendText (triangleLine);
		line.str ("");
	}
}

void Triangles::outputSchroderTriangle (unsigned int lastRow)
{
	matrix<ZZ> S (lastRow+1,lastRow+1);
	unsigned int i,j, indent;
	wxString triangleLine;
	ostringstream decDigits, line;

	/*if (maxValue > to_ZZ("4294967295"))
		throw listingTooLongException("No number in the triangle can exceed 2^32-1");*/

	triangleOutput->ChangeValue (wxT (""));

	// fill row 0 with lastRow+1 ones
	for (j=0; j < lastRow+1; j++)
		S[0][j] = 1;

	for (i = 1; i <= lastRow; i++)
	{
		// now assign values to the elements of row i >= 1,
		// 1 <= i <= lastRow, from j = i to lastRow

		// S(i,i) = S(i-1,j-1) + S(i-1,j)
		// S(i,j) = S(i,j-1) + S(i-1,j-1) + S(i-1,j), i < j <= lastRow
		S[i][i] = S[i-1][i-1] + S[i-1][i];

		for (j=i+1; j <= lastRow; j++)
			S[i][j] = S[i][j-1] + S[i-1][j-1] + S[i-1][j];
	}

	indent = 0;

	for (i=0; i <= lastRow; i++)
	{
		if (i != 0)
		{
			indent += zzdigits (S[i-1][i-1]) + 2;
			line << setw (indent) << ' ';
		}

		for (j=i; j <= lastRow; j++)
			line << outputZZLeft (zzdigits (S[j][j]) +2, S[i][j]);

		line << endl;
		triangleLine = wxString (line.str().c_str(), wxConvUTF8);
		triangleOutput->AppendText (triangleLine);
		line.str ("");
	}
}

void Triangles::outputBellTriangle (unsigned int n)
{
	matrix<ZZ> B (n+1,n+1);
	unsigned int i,j;
	ostringstream line;
	wxString triangleLine;

	triangleOutput->ChangeValue (wxT (""));

	B[0][0] = 1;
	B[1][0] = 1;
	B[1][1] = 2;

	for (i = 2; i <= n; i++)
	{
		B[i][0] = B[i-1][i-1] ;

		for (j = 1; j <= i; j++)
			B[i][j] = B[i][j-1] + B[i-1][j-1];
	}

	for (i=0; i <= n; i++)
	{
		for (j=0; j <= i; j++)
			line << outputZZLeft (zzdigits (B[n][j]) +2, B[i][j]);

		line << endl;
		triangleLine = wxString (line.str().c_str(), wxConvUTF8);
		triangleOutput->AppendText (triangleLine);
		line.str ("");
	}
}
