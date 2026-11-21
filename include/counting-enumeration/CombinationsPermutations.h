#ifndef COMBINATIONS_PERMUTATIONS_H
#define COMBINATIONS_PERMUTATIONS_H

#include <wx/textctrl.h>

#include "NTL/ntlException.h"
#include "NTL/ZZ.h"
#include "NTL/RR.h"

NTL_CLIENT

class CombinationsPermutations
{
private:
	wxTextCtrl *nTextEdit, *kTextEdit, *resultTextEdit, *listingTextEdit;
	wxString nValue, kValue, resultValue, listingText;
	int n, k;
	ZZ result;

	bool get_nk();
	bool get_n();
	bool get_k();
	void getResult();
	void setValue();
	void perm_lex_next (int n, int p[], bool *more);
	void enumeratePermutations (int n, int k);
	void permuteCombination (int a[], int k);
	void perm_print (int n, int p[]);
	void perm_print (int a[], int n, int p[]);
	void comb_next (int n, int k, int a[], bool *done);
	ZZ fact (unsigned int n);
	ZZ perm (unsigned int n, unsigned int k);

public:
	CombinationsPermutations (wxTextCtrl *nPtr, wxTextCtrl *kPtr, wxTextCtrl *resultPtr, wxTextCtrl *listingPtr);
	~CombinationsPermutations()
	{}

	ZZ factorial();
	ZZ permutation();
	ZZ combination();
	void listPermutations();
	void listCombinations();
	ZZ comb (unsigned int n, unsigned int k);
};

#endif
