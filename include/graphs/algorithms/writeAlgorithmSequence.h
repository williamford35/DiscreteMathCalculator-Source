#include <wx/textctrl.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include "graphs/algorithms/DisplayAlgorithmResults.h"

void writeAlgorithmSequence(wxTextCtrl* output,  	vector<wxString> visitList, int n,
                            DisplayAlgorithmResults::Algorithm algorithm);
