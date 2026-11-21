/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/msgdlg.h>

#include "counting-enumeration/AsymptoticEstimate.h"
#include "counting-enumeration/useAsymptoticEstimateException.h"
#include "counting-enumeration/NamedSequences.h"
#include "common/report.h"
#include <float.h>
#include <cmath>
#include <sstream>

using namespace std;

const double AsymptoticEstimate::inf = DBL_MAX;
long AsymptoticEstimate::N;
long AsymptoticEstimate::K;

#define F(x) (x)/(to_RR(1)-exp(-x))-N/K

AsymptoticEstimate::AsymptoticEstimate (wxWindow* parent, CombinationsPermutations *cp, int p, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), page (p), perm_comb (cp)
{
	wxFlexGridSizer* asymptoticflexGridSizer = new wxFlexGridSizer (4, 2, 0, 0);
	asymptoticflexGridSizer->SetFlexibleDirection (wxBOTH);
	asymptoticflexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	asymptoticflexGridSizer->AddGrowableCol (0);
	asymptoticflexGridSizer->AddGrowableCol (1);
	this->SetSizer (asymptoticflexGridSizer);

	asymptoticflexGridSizer->Add (0, 0, 1, wxALL|wxEXPAND, 1);

	asymptoticPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	asymptoticflexGridSizer->Add (asymptoticPanel, 1, wxALL|wxEXPAND, 1);

	wxFlexGridSizer* asymptoticDataFlexGridSizer = new wxFlexGridSizer (0, 4, 0, 0);
	asymptoticDataFlexGridSizer->SetFlexibleDirection (wxBOTH);
	asymptoticDataFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	asymptoticDataFlexGridSizer->AddGrowableCol (1);
	asymptoticDataFlexGridSizer->AddGrowableCol (3);
	asymptoticPanel->SetSizer (asymptoticDataFlexGridSizer);

	asymptoticNStaticText = new wxStaticText (asymptoticPanel, wxID_ANY, _ ("n"), wxDefaultPosition, wxSize (-1,-1), 0);

	asymptoticDataFlexGridSizer->Add (asymptoticNStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	asymptoticNTextCtrl = new wxTextCtrl (asymptoticPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
/* #if wxVERSION_NUMBER >= 3000
	asymptoticNTextCtrl->SetHint (wxT ("Number of elements"));
#endif */

	asymptoticDataFlexGridSizer->Add (asymptoticNTextCtrl, 1, wxALL|wxEXPAND, 5);

	asymptoticKStaticText = new wxStaticText (asymptoticPanel, wxID_ANY, _ ("k"), wxDefaultPosition, wxSize (-1,-1), 0);

	asymptoticDataFlexGridSizer->Add (asymptoticKStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	asymptoticKTextCtrl = new wxTextCtrl (asymptoticPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	asymptoticKTextCtrl->SetHint (wxT (""));
#endif

	asymptoticDataFlexGridSizer->Add (asymptoticKTextCtrl, 1, wxALL|wxEXPAND, 1);

	asymptoticEstimateStaticText = new wxStaticText (this, wxID_ANY, _ ("Result"), wxDefaultPosition, wxSize (-1,-1), 0);

	asymptoticflexGridSizer->Add (asymptoticEstimateStaticText, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

	asymptoticEstimateTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (250,-1), 0);
#if wxVERSION_NUMBER >= 3000
	asymptoticEstimateTextCtrl->SetHint (wxT (""));
#endif

	asymptoticflexGridSizer->Add (asymptoticEstimateTextCtrl, 1, wxALL|wxEXPAND, 1);

	wxArrayString asymptoticRadioBoxArr;

	if (page == 1)
	{
		asymptoticRadioBoxArr.Add (wxT ("Bell"));
		asymptoticRadioBoxArr.Add (wxT ("Catalan"));
		asymptoticRadioBoxArr.Add (wxT ("Combination"));
		asymptoticRadioBoxArr.Add (wxT ("Factorial"));
		asymptoticRadioBoxArr.Add (wxT ("Fibonacci"));
		asymptoticRadioBoxArr.Add (wxT ("Permutation"));
		asymptoticRadioBoxArr.Add (wxT ("Schroder"));
		asymptoticRadioBoxArr.Add (wxT ("Stirling"));
		asymptoticRadioBoxArr.Add (wxT ("Power tower of order 3"));
	}
	else if (page == 3)
	{
		asymptoticRadioBoxArr.Add (wxT ("Number of subsets"));
		asymptoticRadioBoxArr.Add (wxT ("Number of K-element subsets"));
	}
	else
	{
		asymptoticRadioBoxArr.Add (wxT ("Bell"));
		asymptoticRadioBoxArr.Add (wxT ("Stirling"));
	}

	asymptoticRadioBox = new wxRadioBox (this, wxID_ANY, _ ("Numbers"), wxDefaultPosition, wxSize (-1,-1), asymptoticRadioBoxArr, 0, wxRA_SPECIFY_ROWS);
	asymptoticRadioBox->SetSelection (0);
	asymptoticflexGridSizer->Add (asymptoticRadioBox, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	asymptoticComputeButton = new wxButton (this, wxID_ANY, _ ("Compute"), wxDefaultPosition, wxSize (-1,-1), 0);

	asymptoticflexGridSizer->Add (asymptoticComputeButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	asymptoticflexGridSizer->Add (0, 0, 1, wxALL|wxEXPAND, 1);

	asymptoticCloseButton = new wxButton (this, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);
	asymptoticCloseButton->SetDefault();

	asymptoticflexGridSizer->Add (asymptoticCloseButton, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

	SetSizeHints (-1,-1);

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	Centre (1);
	// Connect events
	asymptoticComputeButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (AsymptoticEstimate::OnAsymptoticComputeButtonClick), NULL, this);
	asymptoticCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (AsymptoticEstimate::OnAsymptoticCloseButtonClick), NULL, this);
}

AsymptoticEstimate::~AsymptoticEstimate()
{
	asymptoticComputeButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (AsymptoticEstimate::OnAsymptoticComputeButtonClick), NULL, this);
	asymptoticCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (AsymptoticEstimate::OnAsymptoticCloseButtonClick), NULL, this);
}

bool AsymptoticEstimate::ProcessEvent (wxEvent& event)
{
	try
	{
		return wxEvtHandler::ProcessEvent (event);
	}

	catch (const ntlException& ntle)
	{
		wxString message, caption;

		message.append (wxString (ntle.what().c_str(), wxConvUTF8));
		caption.append (wxT ("Error"));

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
		dialog.ShowModal();
		return true;
	}

	catch (const useAsymptoticEstimateException& uaee)
	{
		wxString message, caption;

		message.append (wxString (uaee.what().c_str(), wxConvUTF8));
		caption.append (wxT ("Note"));

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return true;
	}
}

void AsymptoticEstimate::OnAsymptoticCloseButtonClick (wxCommandEvent& event)
{
	Close();
}

void AsymptoticEstimate::OnAsymptoticComputeButtonClick (wxCommandEvent& event)
{
	unsigned long n, k;
	RR asympApprox, twoTower, numberOfDigits, n_RR;
	ostringstream numstr;
	bool result1, result2, tower = false;

	result1 = (asymptoticNTextCtrl->GetValue()).ToULong (&n);
	result2 = (asymptoticKTextCtrl->GetValue()).ToULong (&k);

	if (!result1)
	{
		report ("n has no value","Note");
		return;
	}

	if (page == 1)
		switch (asymptoticRadioBox->GetSelection())
		{
		case 0:
			asympApprox = estimateBell (n);
			break;

		case 1:
			asympApprox = estimateCatalan (n);
			break;

		case 2:
			if (!result2)
			{
				report("k has no value","Note");
				return;
			}

			asympApprox = estimateCombination (n, k);
			break;

		case 3:
			asympApprox = estimateFactorial (n);
			break;

		case 4:
			asympApprox = estimateFibonacci (n);
			break;

		case 5:
			if (!result2)
			{
				report ("k has no value","Note");
				return;
			}

			asympApprox = permutation (n, k);
			break;

		case 6:
			asympApprox = estimateSchroder (n);
			break;

		case 7:
			if (!result2)
			{
				report("k has no value","Note");
				return;
			}

			asympApprox = estimateStirling (n,k);
			break;

		case 8:
			n_RR = to_RR (n);
			twoTower = pow (n_RR,n_RR);
			numberOfDigits = trunc (1 +  twoTower*log10 (n_RR));
			tower = true;
			break;
		}
	else if (page == 3)
		switch (asymptoticRadioBox->GetSelection())
		{
		case 0:
			asympApprox = estimateNumberOfSubsets (n);
			break;

		case 1:
			if (!result2)
			{
				report ("k has no value","Note");
				return;
			}

			asympApprox = estimateNumberOfKelementSubsets (n,k);
			break;
		}
	else
		switch (asymptoticRadioBox->GetSelection())
		{
		case 0:
			asympApprox = estimateBell (n);
			break;

		case 1:
			if (!result2)
			{
				::report("k has no value","Note");
				return;
			}

			asympApprox = estimateStirling (n,k);
			break;
		}

	if (!tower)
	{
		numstr << asympApprox;
		asymptoticEstimateTextCtrl->ChangeValue (wxString (numstr.str().c_str(), wxConvUTF8));
	}
	else
	{
		numstr << "The tower has " << numberOfDigits << " digits.";
		asymptoticEstimateTextCtrl->ChangeValue (wxString (numstr.str().c_str(), wxConvUTF8));
	}
}

RR AsymptoticEstimate::desy_lambert_W (RR& x)
{
	RR lx1;

	if (x <= 500.0)
	{
		lx1 = log (x + 1.0);
		return 0.665 * (1 + 0.0195 * lx1) * lx1 + 0.04;
	}

	return log (x - 4.0) - (1.0 - 1.0/log (x)) * log (log (x));
}

RR AsymptoticEstimate::lambertW (RR& x, double prec, int maxiterations)
{
	RR w, we, w1e;

	w = desy_lambert_W (x);

	for (int i=0; i < maxiterations; i++)
	{
		we = w * exp (w);
		w1e = (w + 1) *exp (w);

		if (prec > abs ( (x - we) /w1e))
			return w;

		w -= (we - x) / (w1e - (w + 2) * (we - x) / (2*w+2));
	}

	throw useAsymptoticEstimateException ("The Lambert W function did not converge fast enough.");
}

RR AsymptoticEstimate::estimateBell (unsigned long n)
{
	if (n <= 400)
		return to_RR (bell (n));

	RR RRn = to_RR (n), lambda_n = RRn/lambertW (RRn);

	return trunc (pow (lambda_n, RRn + .5) *exp (lambda_n - RRn - 1) /sqrt (RRn));
}

RR AsymptoticEstimate::estimateCatalan (unsigned long n)
{
	if (n <= 50000)
		return to_RR (catalan (n));

	RR nflt;

	conv (nflt, n);

	return trunc (pow (to_RR (4),nflt) / ( (nflt + 1) *sqrt (ComputePi_RR() *nflt)));
}

RR AsymptoticEstimate::estimateFactorial (unsigned long n)
{
	RR nflt;

	if (n <= 25000)
		return fact (n);
	else
		conv (nflt, n);

	return trunc (sqrt (to_RR (2) *ComputePi_RR() *nflt) * pow (nflt/exp (to_RR (1.0)), nflt));
}

RR AsymptoticEstimate::estimateFibonacci (unsigned long n)
{
	if (n <= 300000)
		return to_RR (fibonacci (n));

	RR nflt;

	conv (nflt, n);

	return trunc (pow ( (to_RR (1) + sqrt (to_RR (5))) /to_RR (2),nflt) /sqrt (to_RR (5)));
}


RR AsymptoticEstimate::estimateSchroder (unsigned long n)
//****************************************************************************80
//
//    John Burkardt
//		Modified by William Ford 06 May 2009
//
//
{
	if (n <= 50000)
		return to_RR (schroder (n));

	RR nflt, two = to_RR (2), sqrt2 = sqrt (to_RR (2));

	conv (nflt,n);

	return trunc (pow (1+sqrt2,2*nflt+1) / (nflt*sqrt (two*sqrt2*ComputePi_RR() *nflt)));
}

unsigned int N, K;

RR AsymptoticEstimate::stirlingFormula (unsigned int n)
{
	RR nflt;

	conv (nflt, n);

	return trunc (sqrt (to_RR (2) *ComputePi_RR() *nflt) * pow (nflt/exp (to_RR (1.0)), nflt));
}

RR AsymptoticEstimate::fact (unsigned int n)
{
	RR nfact;
	unsigned int i;

	if (n > 25000)
		nfact = trunc (stirlingFormula (n));
	else
	{
		nfact = 1;

		for (i = 2; i <= n; i++)
			nfact = nfact * i;
	}

	return nfact;
}

RR AsymptoticEstimate::permutation (unsigned int n, unsigned int k)
{
	unsigned int i, lastTerm;
	RR permValue;

	permValue = 1;
	lastTerm = n-k+1;

	for (i=n; i >= lastTerm; i--)
		permValue = permValue * i;

	return permValue;
}

RR AsymptoticEstimate::estimateCombination (unsigned int n, unsigned int k)
{
	RR combValue;
	unsigned int i, lastTerm;

	combValue = 1;
	lastTerm = n-k+1;

	for (i=n; i >= lastTerm; i--)
		combValue = combValue * i;

	combValue = combValue/fact (k);

	return trunc (combValue);
}

RR saddlePt (const RR& x)
{
	return x/ (to_RR (1)-exp (-x)) - to_RR (AsymptoticEstimate::N) /to_RR (AsymptoticEstimate::K);
}

RR AsymptoticEstimate::secant (RR f (const RR& x), const RR& a, const RR& b, const RR& tol, int n)
{
	RR xr, xi_minus1, xi, xi_plus1, fval_xi;
	int i;

	xi_minus1 = a;
	xi = b;

	for (i=1; i <= n; i++)
	{
		fval_xi = f (xi);
		xi_plus1 = xi - fval_xi* (xi_minus1-xi) / (f (xi_minus1)-fval_xi);

		if (abs ( (xi_plus1 - xi) /xi) < tol)
		{
			xr = xi_plus1;
			break;
		}

		xi_minus1 = xi;
		xi = xi_plus1;
	}

	if (i > n)
		xr = inf;

	return xr;
}

RR AsymptoticEstimate::estimateStirling (unsigned int n,unsigned int k)
{
	if (n <= 2500)
		return trunc (to_RR (stirling (n,k)));

	RR t0, x0, A, ft0, B;

	N = n;
	K = k;

	x0 = secant (saddlePt, to_RR (1.0), to_RR (N/double (K)), to_RR (.5e-10), 25);
	t0 = to_RR ( (N-K) /double (K));
	A = to_RR (K) *log (exp (x0) - to_RR (1)) - to_RR (N) *log (x0) - to_RR (K) *t0 + to_RR (N-K) *log (t0);
	ft0 = sqrt (t0/ ( (to_RR (1) +t0) * (x0-t0)));

	return trunc (exp (A) *power (to_RR (K), N-K) *ft0*estimateCombination (n,k));
}

RR AsymptoticEstimate::estimateNumberOfSubsets (unsigned int n)
{
	ZZ sizePowerSet;

	sizePowerSet = power2_ZZ (n);

	return to_RR (sizePowerSet);
}

RR AsymptoticEstimate::estimateNumberOfKelementSubsets (unsigned int n, unsigned int k)
{
	RR numKEltSubsets;

	numKEltSubsets = estimateCombination (n, k);
	return numKEltSubsets;
}
