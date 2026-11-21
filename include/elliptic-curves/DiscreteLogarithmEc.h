#ifndef DISCRETELOGARITHM_BASE_H
#define DISCRETELOGARITHM_BASE_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <wx/arrstr.h>

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

#include <miracl/big.h>
#include <miracl/ecn.h>

#include "elliptic-curves/defineEllipticCurve.h"

#include "DiscreteMathCalculatorMain.h"

class ECdlpTimerThread;

class DiscreteLogarithmEc : public wxDialog
{
protected:
	wxStaticBitmap* DiscreteLogStaticBitmap;
	wxStaticText* DiscreteLogStaticText;
	wxRadioBox* DiscreteLogRadioBox;
	wxPanel* DiscreteLogpPanel;
	wxStaticText* DiscreteLogpStaticText;
	wxPanel* DiscreteLognPanel;
	wxStaticText* DiscreteLognStaticText;
	wxTextCtrl* DiscreteLognTextCtrl;
	wxPanel* DiscreteLogPPanel;
	wxStaticText* DiscreteLogPStaticText;
	wxPanel* DiscreteLogQPanel;
	wxStaticText* DiscreteLogQStaticText;
	wxStaticLine* DiscreteLogStaticLine;
	wxPanel* DiscreteLogRunPanel;
	wxRadioButton* DiscreteLogRunUntilCompletionRadioButton;
	wxRadioButton* DiscreteLogRunFormMinutesRadioButton;
	wxStaticText* DiscreteLogRunFormMinutesStaticText;
	wxTextCtrl* DiscreteLogRunFormMinutesTextCtrl;
	wxButton* DiscreteLogTryButton;
	wxPanel* DiscreteLogResultPanel;
	wxStaticText* DiscreteLogResultStaticText;
	wxTextCtrl* DiscreteLogMsgTextCtrl;
	wxTextCtrl* DiscreteLogResultTextCtrl;
	wxPanel* DiscreteLogIterationsTimingPanel;
	wxStaticText* DiscreteLogIterationsTimingStaticText;
	wxTextCtrl* DiscreteLogIterationsTextCtrl;
	wxStaticText* DiscreteLogTimingStaticText;
	wxTextCtrl* DiscreteLogTimingTextCtrl;
	wxPanel* DiscreteLogControlPanel;
	wxButton* DiscreteLogCloseButton;
	wxStaticText* DiscreteLogxCoorofPStaticText;
	wxTextCtrl* DiscreteLogxCoorofPTextCtrl;
	wxStaticText* DiscreteLogyCoorofPStaticText;
	wxTextCtrl* DiscreteLogyCoorofPTextCtrl;
	wxStaticText* DiscreteLogxCoorofQStaticText;
	wxTextCtrl* DiscreteLogxCoorofQTextCtrl;
	wxStaticText* DiscreteLogyCoorofQStaticText;
	wxTextCtrl* DiscreteLogyCoorofQTextCtrl;
	wxPanel* DiscreteLogDisplayResultsPanel;
	wxStaticText* DiscreteLogDisplayResultsStaticText;
	wxTextCtrl* DiscreteLogDisplayResultsTextCtrl;


protected:
	virtual void OnDiscreteLogTryButtonPress (wxCommandEvent& event);
	virtual void OnDiscreteLogCloseButtonPress (wxCommandEvent& event);

public:
	DiscreteLogarithmEc (wxWindow* parent, const char *image, const Big& prime,
	                     const Big& acoeff, const Big& bcoeff, const Big& ptOrder, const ECn& ptP,
	                     const ECn& ptQ, int b,
	                     wxWindowID id = wxID_ANY, const wxString& title = wxT (""),
	                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (400,350),
	                     long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	virtual ~DiscreteLogarithmEc();

private:
	Big p, A, B, n;
	ECn P, Q;
	int base, algorithmSelection;

	void fillInputValues();
	void respondToResults(Big& discreteLog, Big& iterations, double& work_time);
	void reportSuccess (const Big& k);
	void clearAll();
	// wxDECLARE_EVENT_TABLE();
};

#endif	// DISCRETELOGARITHM_BASE_H
