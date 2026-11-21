#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <wx/textctrl.h>

#include "counting-enumeration/CombinationsPermutations.h"

class Triangles
{
private:

	wxTextCtrl *triangleOutput;

public:
	Triangles (wxTextCtrl *output);
	~Triangles()
	{}
	void outputPascalTriangle (unsigned int n);
	void outputHosoyaTriangle (unsigned int n);
	void outputCatalanTriangle (unsigned int n);
	void outputSchroderTriangle (unsigned int n);
	void outputBellTriangle (unsigned int n);
};

#endif // TRIANGLES_H
