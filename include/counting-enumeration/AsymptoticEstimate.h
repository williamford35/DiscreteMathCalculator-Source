#ifndef ASYMPTOTICESTIMATE_H
#define ASYMPTOTICESTIMATE_H

#include <wx/settings.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/button.h>

#include "counting-enumeration/CombinationsPermutations.h"
#include "NTL/RR.h"
#include "NTL/ZZ.h"

NTL_CLIENT

class AsymptoticEstimate : public wxDialog
{
private:
	wxPanel* asymptoticPanel;
	wxStaticText* asymptoticNStaticText;
	wxTextCtrl* asymptoticNTextCtrl;
	wxStaticText* asymptoticKStaticText;
	wxTextCtrl* asymptoticKTextCtrl;
	wxStaticText* asymptoticEstimateStaticText;
	wxTextCtrl* asymptoticEstimateTextCtrl;
	wxRadioBox* asymptoticRadioBox;
	wxButton* asymptoticComputeButton;
	wxButton* asymptoticCloseButton;

	static long N, K;
	static const double inf;


private:
	virtual void OnAsymptoticComputeButtonClick (wxCommandEvent& event);
	virtual void OnAsymptoticCloseButtonClick (wxCommandEvent& event);

	RR desy_lambert_W (RR& x);
	RR lambertW (RR& x, double prec=1.e-12, int maxiterations=100);
	RR estimateBell (unsigned long n);
	RR estimateCatalan (unsigned long n);
	RR estimateCombination (unsigned int n, unsigned int k);
	RR estimateFactorial (unsigned long n);
	RR estimateFibonacci (unsigned long n);
	RR estimateSchroder (unsigned long n);
	RR estimateStirling (unsigned int n,unsigned int k);
	RR stirlingFormula (unsigned int n);
	RR fact (unsigned int n);
	RR permutation (unsigned int n,unsigned int k);
	friend RR saddlePt (const RR& x);
	RR secant (RR f (const RR& x), const RR& a, const RR& b, const RR& tol, int n);
	RR estimateNumberOfSubsets (unsigned int n);
	RR estimateNumberOfKelementSubsets (unsigned int n, unsigned int k);

	int page;
	CombinationsPermutations *perm_comb;

public:
	AsymptoticEstimate (wxWindow* parent, CombinationsPermutations *cp, int p,
	                    wxWindowID id = wxID_ANY,
	                    const wxString& title = _ ("Asymptotic Estimate"),
	                    const wxPoint& pos = wxDefaultPosition,
	                    const wxSize& size = wxSize (-1,-1),
	                    long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~AsymptoticEstimate();
	bool ProcessEvent (wxEvent& event);
};

#endif
