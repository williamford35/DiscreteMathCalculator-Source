#ifndef SUBSETS_H_INCLUDED
#define SUBSETS_H_INCLUDED

#include <wx/textctrl.h>
#include "counting-enumeration/CombinationsPermutations.h"
#include "NTL/ZZ.h"

NTL_CLIENT

class Subsets
{
private:
	wxTextCtrl *subsetsListing, *numSubsets,
	           *numKElementSubsets;
	CombinationsPermutations *perm_comb;

	void ksub_next (unsigned int n, unsigned int k, unsigned int a[], bool *done);
	void subset_by_size_next (unsigned int n, unsigned int a[],
	                          unsigned int *size, bool *more);


public:

	Subsets (wxTextCtrl *listing, wxTextCtrl *subsets,
	         wxTextCtrl *kEltSubsets, CombinationsPermutations *cp);
	~Subsets()
	{}
	void numberOfSubsets (unsigned int n);
	void numberOfKElementSubsets (unsigned int n, unsigned int k);
	void listAllSubsets (unsigned int n);
	void listAllKElementSubsets (unsigned int n, unsigned int k);
};


#endif // SUBSETS_H_INCLUDED
