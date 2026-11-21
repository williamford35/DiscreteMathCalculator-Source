/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "elliptic-curves/ecdlp.h"
#include "elliptic-curves/DiscreteLogarithmEc.h"
#include "common/ImageFiles.h"
#include "common/report.h"

#include "wx/log.h"
#include <wx/utils.h>

DiscreteLogarithmEc::DiscreteLogarithmEc (wxWindow* parent, const char *image, const Big& prime,
      const Big& acoeff, const Big& bcoeff, const Big& ptOrder, const ECn& ptP, const ECn& ptQ, int b,
      wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), A (acoeff), B (bcoeff), n (ptOrder),
	  P (ptP), Q (ptQ), base (b), p(prime)
{
	wxBoxSizer* DiscreteLogBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (DiscreteLogBoxSizer);

	DiscreteLogStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );

	DiscreteLogBoxSizer->Add (DiscreteLogStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogStaticText = new wxStaticText (this, wxID_ANY, _ ("Algorithm to Apply"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);

	DiscreteLogBoxSizer->Add (DiscreteLogStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	/*wxString __wxRadioBoxChoices[3] = { _ ("Pollard rho - O(sqrt n) in time and O(1) in space"), _ ("Baby-step-Giant-Step - O(sqrt n) in time and space"), _ ("Brute force - O(n)") };
	DiscreteLogRadioBox = new wxRadioBox (this,
	      wxID_ANY,
	      _ ("n = base point order"),
	      wxDefaultPosition,
	      wxSize (-1,-1),
	      3,
	      __wxRadioBoxChoices,
	      1,
	      wxRA_HORIZONTAL,
	      wxDefaultValidator,
	      _T ("") );
	DiscreteLogBoxSizer->Add (DiscreteLogRadioBox, 0, wxALL, WXC_FROM_DIP (5) );*/

	wxArrayString DiscreteLogRadioBoxArr;
	DiscreteLogRadioBoxArr.Add (_ ("Pollard rho - O(sqrt n) in time and O(1) in space") );
	DiscreteLogRadioBoxArr.Add (_ ("Baby-step-Giant-Step - O(sqrt n) in time and space") );
	DiscreteLogRadioBoxArr.Add (_ ("Brute force - (O(n))") );
	DiscreteLogRadioBox = new wxRadioBox (this, wxID_ANY, _ ("n = base point order"),
	                                      wxDefaultPosition, wxSize (-1,-1), DiscreteLogRadioBoxArr, 0, wxRA_SPECIFY_ROWS);
	DiscreteLogRadioBox->SetSelection(0);
	DiscreteLogBoxSizer->Add (DiscreteLogRadioBox, 0, wxALL, WXC_FROM_DIP (5) );
	DiscreteLogpPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogpPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLogpPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogpPanel->SetSizer (DiscreteLogpPanelBoxSizer);

	DiscreteLognPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLognPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLognPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLognPanel->SetSizer (DiscreteLognPanelBoxSizer);

	DiscreteLognStaticText = new wxStaticText (DiscreteLognPanel, wxID_ANY, _ ("n"), wxDefaultPosition, wxDLG_UNIT (DiscreteLognPanel, wxSize (-1,-1) ), 0);

	DiscreteLognPanelBoxSizer->Add (DiscreteLognStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLognTextCtrl = new wxTextCtrl (DiscreteLognPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLognPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLognTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLognPanelBoxSizer->Add (DiscreteLognTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogPPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogPPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLogPPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogPPanel->SetSizer (DiscreteLogPPanelBoxSizer);

	DiscreteLogPStaticText = new wxStaticText (DiscreteLogPPanel, wxID_ANY, _ ("P"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogPPanel, wxSize (-1,-1) ), 0);

	DiscreteLogPPanelBoxSizer->Add (DiscreteLogPStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogxCoorofPStaticText = new wxStaticText (DiscreteLogPPanel, wxID_ANY, _ ("x:"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogPPanel, wxSize (-1,-1) ), 0);

	DiscreteLogPPanelBoxSizer->Add (DiscreteLogxCoorofPStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogxCoorofPTextCtrl = new wxTextCtrl (DiscreteLogPPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogPPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogxCoorofPTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogPPanelBoxSizer->Add (DiscreteLogxCoorofPTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogyCoorofPStaticText = new wxStaticText (DiscreteLogPPanel, wxID_ANY, _ ("y:"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogPPanel, wxSize (-1,-1) ), 0);

	DiscreteLogPPanelBoxSizer->Add (DiscreteLogyCoorofPStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogyCoorofPTextCtrl = new wxTextCtrl (DiscreteLogPPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogPPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogyCoorofPTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogPPanelBoxSizer->Add (DiscreteLogyCoorofPTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogQPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogQPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLogQPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogQPanel->SetSizer (DiscreteLogQPanelBoxSizer);

	DiscreteLogQStaticText = new wxStaticText (DiscreteLogQPanel, wxID_ANY, _ ("Q"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogQPanel, wxSize (-1,-1) ), 0);

	DiscreteLogQPanelBoxSizer->Add (DiscreteLogQStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogxCoorofQStaticText = new wxStaticText (DiscreteLogQPanel, wxID_ANY, _ ("x:"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogQPanel, wxSize (-1,-1) ), 0);

	DiscreteLogQPanelBoxSizer->Add (DiscreteLogxCoorofQStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogxCoorofQTextCtrl = new wxTextCtrl (DiscreteLogQPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogQPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogxCoorofQTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogQPanelBoxSizer->Add (DiscreteLogxCoorofQTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogyCoorofQStaticText = new wxStaticText (DiscreteLogQPanel, wxID_ANY, _ ("y:"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogQPanel, wxSize (-1,-1) ), 0);

	DiscreteLogQPanelBoxSizer->Add (DiscreteLogyCoorofQStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogyCoorofQTextCtrl = new wxTextCtrl (DiscreteLogQPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogQPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogyCoorofQTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogQPanelBoxSizer->Add (DiscreteLogyCoorofQTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogStaticLine = new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	DiscreteLogBoxSizer->Add (DiscreteLogStaticLine, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	DiscreteLogRunPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogRunPanel, 0, wxALL, WXC_FROM_DIP (5) );

	wxBoxSizer* DiscreteLogRunPanelBoxSixer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogRunPanel->SetSizer (DiscreteLogRunPanelBoxSixer);
	DiscreteLogTryButton = new wxButton (this, wxID_ANY, _ ("Try"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);
	DiscreteLogBoxSizer->Add (DiscreteLogTryButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );
	DiscreteLogResultPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogResultPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLogResultPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogResultPanel->SetSizer (DiscreteLogResultPanelBoxSizer);

	DiscreteLogResultStaticText = new wxStaticText (DiscreteLogResultPanel, wxID_ANY, _ ("Result"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogResultPanel, wxSize (-1,-1) ), 0);

	DiscreteLogResultPanelBoxSizer->Add (DiscreteLogResultStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	// DiscreteLogMsgTextCtrl = new wxTextCtrl (DiscreteLogResultPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogResultPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	// DiscreteLogMsgTextCtrl->SetHint (wxT ("") );
#endif

	// DiscreteLogResultPanelBoxSizer->Add (DiscreteLogMsgTextCtrl, 0, wxALL, WXC_FROM_DIP (1) );

	DiscreteLogResultTextCtrl = new wxTextCtrl (DiscreteLogResultPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogResultPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogResultTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogResultPanelBoxSizer->Add (DiscreteLogResultTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );
	DiscreteLogIterationsTimingPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogIterationsTimingPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLogIterationsTimingPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogIterationsTimingPanel->SetSizer (DiscreteLogIterationsTimingPanelBoxSizer);

	DiscreteLogIterationsTimingStaticText = new wxStaticText (DiscreteLogIterationsTimingPanel, wxID_ANY, _ ("Iterations"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogIterationsTimingPanel, wxSize (-1,-1) ), 0);

	DiscreteLogIterationsTimingPanelBoxSizer->Add (DiscreteLogIterationsTimingStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogIterationsTextCtrl = new wxTextCtrl (DiscreteLogIterationsTimingPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogIterationsTimingPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogIterationsTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogIterationsTimingPanelBoxSizer->Add (DiscreteLogIterationsTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogTimingStaticText = new wxStaticText (DiscreteLogIterationsTimingPanel, wxID_ANY, _ ("Time"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogIterationsTimingPanel, wxSize (-1,-1) ), 0);

	DiscreteLogIterationsTimingPanelBoxSizer->Add (DiscreteLogTimingStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogTimingTextCtrl = new wxTextCtrl (DiscreteLogIterationsTimingPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogIterationsTimingPanel, wxSize (-1,-1) ), wxTE_READONLY);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogTimingTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogIterationsTimingPanelBoxSizer->Add (DiscreteLogTimingTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogDisplayResultsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
	DiscreteLogBoxSizer->Add(DiscreteLogDisplayResultsPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP(10));
	wxBoxSizer* DiscreteLogDisplayResultsBoxizer = new wxBoxSizer(wxHORIZONTAL);
	DiscreteLogDisplayResultsPanel->SetSizer(DiscreteLogDisplayResultsBoxizer);
	DiscreteLogDisplayResultsStaticText = new wxStaticText(DiscreteLogDisplayResultsPanel, wxID_ANY, _("Check"), wxDefaultPosition, wxDLG_UNIT(DiscreteLogDisplayResultsPanel, wxSize(-1,-1)), 0);
	DiscreteLogDisplayResultsBoxizer->Add(DiscreteLogDisplayResultsStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP(5));
	DiscreteLogDisplayResultsTextCtrl = new wxTextCtrl(DiscreteLogDisplayResultsPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(DiscreteLogDisplayResultsPanel, wxSize(-1,-1)), 0);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogDisplayResultsTextCtrl->SetHint(wxT(""));
#endif
	DiscreteLogDisplayResultsBoxizer->Add(DiscreteLogDisplayResultsTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP(1));
	DiscreteLogControlPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogControlPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* DiscreteLogControlPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogControlPanel->SetSizer (DiscreteLogControlPanelBoxSizer);

	DiscreteLogControlPanelBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogCloseButton = new wxButton (DiscreteLogControlPanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogControlPanel, wxSize (-1,-1) ), 0);

	DiscreteLogControlPanelBoxSizer->Add (DiscreteLogCloseButton, 0, wxALL, WXC_FROM_DIP (5) );

	// SetName (wxT ("DiscreteLogarithmEc") );
	SetSize (400,350);

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	if (GetParent() )
	{
		CentreOnParent (wxBOTH);
	}
	else
	{
		CentreOnScreen (wxBOTH);
	}

#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this) )
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif
	// Connect events
	DiscreteLogTryButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLogarithmEc::OnDiscreteLogTryButtonPress), NULL, this);
	DiscreteLogCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLogarithmEc::OnDiscreteLogCloseButtonPress), NULL, this);

	fillInputValues();
}

DiscreteLogarithmEc::~DiscreteLogarithmEc()
{
	DiscreteLogTryButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLogarithmEc::OnDiscreteLogTryButtonPress), NULL, this);
	DiscreteLogCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLogarithmEc::OnDiscreteLogCloseButtonPress), NULL, this);
}

void DiscreteLogarithmEc::OnDiscreteLogCloseButtonPress (wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void DiscreteLogarithmEc::fillInputValues()
{
	miracl *mip = get_mip();
	char buf[625];
	Big X, Y;

	if (base == 10)
		mip->IOBASE = 10;
	else
		mip->IOBASE = 16;

	buf << n;
	DiscreteLognTextCtrl->SetValue (buf);
	P.get (X,Y);
	buf << X;
	DiscreteLogxCoorofPTextCtrl->SetValue (buf);
	buf << Y;
	DiscreteLogyCoorofPTextCtrl->SetValue (buf);
	Q.get (X,Y);
	buf << X;
	DiscreteLogxCoorofQTextCtrl->SetValue (buf);
	buf << Y;
	DiscreteLogyCoorofQTextCtrl->SetValue (buf);
}

void DiscreteLogarithmEc::OnDiscreteLogTryButtonPress (wxCommandEvent& event)
{
	// bool runUntilCompletion;
	wxString ecdlpProcess, Acoeff, Bcoeff;
	double work_time;
	Big result;
	Big iterations;
	bool success;

	clearAll();
	algorithmSelection = DiscreteLogRadioBox->GetSelection();

	wxString x1 = DiscreteLogxCoorofPTextCtrl->GetValue(),
	         y1 = DiscreteLogyCoorofPTextCtrl->GetValue(),
	         x2 = DiscreteLogxCoorofQTextCtrl->GetValue(),
	         y2 = DiscreteLogyCoorofQTextCtrl->GetValue();

	char Px[1024], Py[1024],
	     Qx[1024], Qy[1024];

	strncpy(Px, (const char*)x1.mb_str(wxConvUTF8), 1023);
	strncpy(Py, (const char*)y1.mb_str(wxConvUTF8), 1023);
	strncpy(Qx, (const char*)x2.mb_str(wxConvUTF8), 1023);
	strncpy(Qy, (const char*)y2.mb_str(wxConvUTF8), 1023);

	wxBeginBusyCursor();

	Big disclog, steps;

	switch(algorithmSelection)
	{
	case 0:
		success = pollard (P, Q, n, disclog, iterations, work_time);
		break;

	case 1:
		success = discreteLogBabyStepGiantStep (P, Q, n, disclog, iterations, work_time);
		break;

	case 2:
		success = discreteLogBruteForce (P, Q, n, disclog, work_time, iterations);
	}

	wxEndBusyCursor();

	if(success)
		respondToResults(disclog, iterations, work_time);
	else
	{
		switch (algorithmSelection)
		{
		case 0:
			::report (wxT("Maybe try Pollard's rho again."), wxT("Note"));
			break;
		case 1:
			::report (wxT("Maybe try Baby-Step-Giant-Step again."), wxT("Note"));
			break;
		};
	}

	return;
}

void DiscreteLogarithmEc::respondToResults (Big& discreteLog, Big& iterations, double& seconds)
{
	char s[128];
	char str[256];
	miracl *mip;

	mip=get_mip();
	mip->IOBASE=10;
	big bg = discreteLog.getbig();
	int ab = cotstr (bg, s);

	wxString mystring = wxString::FromUTF8(s);
	wxString discreteLogarithm = mystring;
	DiscreteLogResultTextCtrl->SetValue(mystring);

	str << iterations;
	mystring = wxString::FromUTF8(str);
	DiscreteLogIterationsTextCtrl->SetValue(mystring);

	mystring = wxString::Format(wxT("%e"),seconds);
	DiscreteLogTimingTextCtrl->SetValue(mystring);

	mystring = wxT("P = Q");
	mystring = discreteLogarithm + wxT("*") + mystring;;
	DiscreteLogDisplayResultsTextCtrl->SetValue(mystring);
}

void DiscreteLogarithmEc::clearAll()
{
	DiscreteLogResultTextCtrl->ChangeValue(wxT(""));
	DiscreteLogIterationsTextCtrl->ChangeValue(wxT(""));
	DiscreteLogTimingTextCtrl->ChangeValue(wxT(""));
	DiscreteLogDisplayResultsTextCtrl->ChangeValue(wxT(""));
}
