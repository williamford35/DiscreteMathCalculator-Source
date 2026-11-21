#ifndef DISCRETELOG_H
#define DISCRETELOG_H

#include "miracl/big.h"
#include "discrete-logarithm/genprime.h"

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <vector>
using namespace std;

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

class DiscreteLog : public wxDialog
{
protected:
	wxStaticText* DiscreteLogStaticText;
	wxPanel* DiscreteLogRadioButtonsPanel;
	wxRadioButton* DiscreteLogFourPrimeFactorsRadioButton;
	wxRadioButton* DiscreteLogFivePrimeFactorsRadioButton;
	wxRadioButton* DiscreteLogSixPrimeFactorsRadioButton;
	wxRadioButton* DiscreteLogSevenPrimeFactorsRadioButton;
	wxRadioButton* DiscreteLogEightPrimeFactorsRadioButton;
	wxRadioButton* DiscreteLogNinePrimeFactorsRadioButton;
	wxRadioButton* DiscreteLogTenPrimeFactorsRadioButton;
	wxButton* DiscreteLogBuildProblemButton;
	wxStaticLine* DiscreteLogStaticLine1;
	wxPanel* DiscreteLogProblemPanel;
	wxStaticText* DiscreteLogpProblemStaticText;
	wxTextCtrl* DiscreteLogpValueTextCtrl;
	wxStaticText* DiscreteLogPrimeFactorsStaticText;
	wxTextCtrl* DiscreteLogPrimeFactorsTextCtrl;
	wxPanel* DiscreteLogPrimeFactorsPanel;
	wxPanel* DiscreteLogSolvePanel;
	wxButton* DiscreteLogAutogenerateyButton;
	wxStaticText* DiscreteLogyValueStaticText;
	wxTextCtrl* DiscreteLogyValueTextCtrl;
	wxPanel* DiscreteLogSetTimePanel;
	wxRadioButton* DiscreteLogRunForMSecondsRadioButton;
	wxStaticText* DiscreteLogRunForMSecondsStaticText;
	wxTextCtrl* DiscreteLogRunForMSecondsTextCtrl;
	wxRadioButton* DiscreteLogRunUntilCompletionRadioButton;
	wxButton* DiscreteLogTryToSolveButton;
	wxStaticLine* DiscreteLogStaticLine2;
	wxPanel* DiscreteLogSolutionPanel;
	wxStaticText* DiscreteLogSolutionxStaticText;
	wxTextCtrl* DiscreteLogSolutionxTextCtrl;
	wxStaticText* DiscreteLogCheckSolutionStaticText;
	wxTextCtrl* DiscreteLogCheckSolutionTextCtrl;
	wxStaticLine* DiscreteLogStaticLine3;
	wxPanel* DiscreteLogClosePanel;
	wxButton* DiscreteLogCloseButton;
	wxPanel* DiscreteLogOutcomePanel;
	wxStaticText* DiscreteLogIterationsStaticText;
	wxTextCtrl* DiscreteLogIterationsTextCtrl;
	wxStaticText* DiscreteLogTimeStaticText;
	wxTextCtrl* DiscreteLogTimeTextCtrl;
	wxStaticText* DiscreteLogNPRIMES;
	wxPanel* TryAndNumberOfBitsInPrimePanel;
	wxButton* DiscreteLogButton;
	wxStaticText* NumberOfBitsInPrimeStaticText;
	wxTextCtrl* NumberOfBitsInPrimeTextCtrl;
protected:
	virtual void OnDiscreteLogBuildProblemButtonPress (wxCommandEvent& event);
	virtual void OnDiscreteLogAutogenerateyButtonPress (wxCommandEvent& event);
	virtual void OnDiscreteLogTryToSolveButtonPress (wxCommandEvent& event);
	virtual void OnDiscreteLogCloseButtonPress (wxCommandEvent& event);
	virtual void OnRadioButtonClicked (wxCommandEvent& event);

public:
	DiscreteLog (wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT (""),
	             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (785,650),
	             long style = wxRESIZE_BORDER|wxCLOSE_BOX);
	virtual ~DiscreteLog();

private:
	miracl *mip;
	Big r, p, y;

	void ScanRadioButtons();
	void writeTextCtrl (wxTextCtrl *t, const Big& v);
	Big readTextCtrl (wxTextCtrl *t);
	void respondToResults (const wxArrayString& stddout);

	int NPRIMES, radioButtonSelected;
	vector<Big> factors;

	wxRadioButton* RadioButton[7];
};

#endif	// DISCRETELOG_H
