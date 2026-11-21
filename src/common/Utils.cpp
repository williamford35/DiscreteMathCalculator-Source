/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "common/Utils.h"
#include <wx/dcmemory.h>

using namespace std;

void i4vec_indicator (int n, int a[])

//****************************************************************************80
//
//  Purpose:
//
//    I4VEC_INDICATOR sets an I4VEC to the indicator vector.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 February 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of elements of A.
//
//    Output, int A[N], the initialized array.
//
{
	int i;

	for (i = 0; i < n; i++)
	{
		a[i] = i + 1;
	}
}

int zzdigits (const ZZ& n)
{
	ostringstream zzout;

	zzout << n;

	return zzout.str().length();
}

int getStringExtent (const wxString& vertexLabel, const wxFont& font)
{
	wxMemoryDC dc;
	wxCoord w, h;

	dc.SetFont (font);
	dc.GetTextExtent (vertexLabel, &w, &h);

	return w;
}
