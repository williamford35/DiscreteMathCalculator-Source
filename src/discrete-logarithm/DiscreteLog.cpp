#include "discrete-logarithm/DiscreteLog.h"
#include "discrete-logarithm/genprime.h"
#include "common/report.h"
#include "common/itoa.h"
#include "common/runApplication.h"

DiscreteLog::DiscreteLog (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style)
{
	wxBoxSizer* DiscreteLogBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (DiscreteLogBoxSizer);

	DiscreteLogStaticText = new wxStaticText (this, wxID_ANY, _ ("Given y and prime p, solve\ny = 2^x (mod p) for x,\nwhere p-1 has small factors"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);
	wxFont DiscreteLogStaticTextFont (14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("Segoe UI") );
	DiscreteLogStaticText->SetFont (DiscreteLogStaticTextFont);

	DiscreteLogBoxSizer->Add (DiscreteLogStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	DiscreteLogRadioButtonsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogRadioButtonsPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* DiscreteLogRadioButtonsPanelBoxSizer = new wxBoxSizer (wxVERTICAL);
	DiscreteLogRadioButtonsPanel->SetSizer (DiscreteLogRadioButtonsPanelBoxSizer);
	DiscreteLogNPRIMES = new wxStaticText (DiscreteLogRadioButtonsPanel, wxID_ANY, _ ("Number of Primes"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 1);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogNPRIMES, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogFourPrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY, _ ("4"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogFourPrimeFactorsRadioButton->SetValue (0);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogFourPrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogFivePrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY, _ ("5"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogFivePrimeFactorsRadioButton->SetValue (1);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogFivePrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogSixPrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY, _ ("6"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogSixPrimeFactorsRadioButton->SetValue (0);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogSixPrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogSevenPrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY, _ ("7"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogSevenPrimeFactorsRadioButton->SetValue (0);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogSevenPrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogEightPrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY, _ ("8"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogEightPrimeFactorsRadioButton->SetValue (0);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogEightPrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogNinePrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY,
	      _ ("9"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogNinePrimeFactorsRadioButton->SetValue (false);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogNinePrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogTenPrimeFactorsRadioButton = new wxRadioButton (DiscreteLogRadioButtonsPanel, wxID_ANY,
	      _ ("10"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogRadioButtonsPanel, wxSize (-1,-1) ), 0);
	DiscreteLogTenPrimeFactorsRadioButton->SetValue (false);
	DiscreteLogRadioButtonsPanelBoxSizer->Add (DiscreteLogTenPrimeFactorsRadioButton, 0, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogBuildProblemButton = new wxButton (this, wxID_ANY, _ ("Build problem"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);
	DiscreteLogBuildProblemButton->SetToolTip(wxT("Construct the factors, the prime, and y."));
	DiscreteLogBoxSizer->Add (DiscreteLogBuildProblemButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	// DiscreteLogStaticLine1 = new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxLI_HORIZONTAL);
	// DiscreteLogBoxSizer->Add (DiscreteLogStaticLine1, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

    DiscreteLogProblemPanel = new wxPanel(this, wxID_ANY);

    DiscreteLogBoxSizer->Add(
        DiscreteLogProblemPanel, 0, wxALL | wxEXPAND, WXC_FROM_DIP(1));

    wxBoxSizer* DiscreteLogProblemBoxSizer =
        new wxBoxSizer(wxHORIZONTAL);

    DiscreteLogProblemPanel->SetSizer(DiscreteLogProblemBoxSizer);

    DiscreteLogpProblemStaticText =
        new wxStaticText(DiscreteLogProblemPanel, wxID_ANY, _("p"));

    DiscreteLogProblemBoxSizer->Add(
        DiscreteLogpProblemStaticText, 0,
        wxALL | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    DiscreteLogpValueTextCtrl =
        new wxTextCtrl(DiscreteLogProblemPanel, wxID_ANY, wxT(""),
                       wxDefaultPosition, wxSize(500, -1));

    DiscreteLogProblemBoxSizer->Add(
        DiscreteLogpValueTextCtrl, 1,
        wxALL | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxBoxSizer* factorsSizer = new wxBoxSizer(wxVERTICAL);

    DiscreteLogPrimeFactorsStaticText =
        new wxStaticText(DiscreteLogProblemPanel, wxID_ANY,
                         _("factors of p-1"));

    factorsSizer->Add(
        DiscreteLogPrimeFactorsStaticText, 0,
        wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, WXC_FROM_DIP(5));

    DiscreteLogPrimeFactorsTextCtrl =
        new wxTextCtrl(DiscreteLogProblemPanel, wxID_ANY, wxT(""),
                       wxDefaultPosition, wxSize(100, 100),
                       wxTE_MULTILINE);

    factorsSizer->Add(
        DiscreteLogPrimeFactorsTextCtrl, 0,
        wxALIGN_CENTER_HORIZONTAL);

    DiscreteLogProblemBoxSizer->Add(
        factorsSizer, 0, wxALL | wxALIGN_CENTER_VERTICAL,
        WXC_FROM_DIP(5));

	DiscreteLogSolvePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogSolvePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* DiscreteLogSolvePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogSolvePanel->SetSizer (DiscreteLogSolvePanelBoxSizer);

	DiscreteLogAutogenerateyButton = new wxButton (DiscreteLogSolvePanel, wxID_ANY, _ ("autogenerate y"),
	      wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolvePanel, wxSize (-1,-1) ), 0);
	DiscreteLogAutogenerateyButton->SetToolTip(wxT("Automatically generate the value of y."));
	DiscreteLogSolvePanelBoxSizer->Add (DiscreteLogAutogenerateyButton, 0, wxALL, WXC_FROM_DIP (1) );
	DiscreteLogyValueStaticText = new wxStaticText (DiscreteLogSolvePanel, wxID_ANY, _ ("y"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolvePanel, wxSize (-1,-1) ), 0);
	DiscreteLogSolvePanelBoxSizer->Add (DiscreteLogyValueStaticText, 0, wxALL, WXC_FROM_DIP (1) );
	DiscreteLogyValueTextCtrl = new wxTextCtrl (DiscreteLogSolvePanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolvePanel, wxSize (300,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogyValueTextCtrl->SetHint (wxT ("") );
#endif
	DiscreteLogSolvePanelBoxSizer->Add (DiscreteLogyValueTextCtrl, 1, wxALL | wxEXPAND, WXC_FROM_DIP (1) );

	TryAndNumberOfBitsInPrimePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
	DiscreteLogBoxSizer->Add(TryAndNumberOfBitsInPrimePanel, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP(5));
	wxBoxSizer* TryAndNumberOfBitsInPrimeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	TryAndNumberOfBitsInPrimePanel->SetSizer(TryAndNumberOfBitsInPrimeBoxSizer);
	DiscreteLogButton = new wxButton(TryAndNumberOfBitsInPrimePanel, wxID_ANY, _("Try"), wxDefaultPosition, wxDLG_UNIT(TryAndNumberOfBitsInPrimePanel, wxSize(-1,-1)), 0);
	DiscreteLogButton->SetToolTip(wxT("Execute Pollard's rho algorithm."));
	TryAndNumberOfBitsInPrimeBoxSizer->Add(DiscreteLogButton, 0, wxALL, WXC_FROM_DIP(5));
	NumberOfBitsInPrimeStaticText = new wxStaticText(TryAndNumberOfBitsInPrimePanel, wxID_ANY, _("Bit size"), wxDefaultPosition, wxDLG_UNIT(TryAndNumberOfBitsInPrimePanel, wxSize(-1,-1)), 0);
	TryAndNumberOfBitsInPrimeBoxSizer->Add(NumberOfBitsInPrimeStaticText, 0, wxALL, WXC_FROM_DIP(5));
	NumberOfBitsInPrimeTextCtrl = new wxTextCtrl(TryAndNumberOfBitsInPrimePanel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(55,-1), 0);
#if wxVERSION_NUMBER >= 3000
	NumberOfBitsInPrimeTextCtrl->SetHint(wxT(""));
#endif
	TryAndNumberOfBitsInPrimeBoxSizer->Add(NumberOfBitsInPrimeTextCtrl, 0, wxALL, WXC_FROM_DIP(5));

	/* DiscreteLogStaticLine2 = new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxLI_HORIZONTAL);
	DiscreteLogBoxSizer->Add (DiscreteLogStaticLine2, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) ); */

	DiscreteLogOutcomePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogOutcomePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* DiscreteLogOutcomePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogOutcomePanel->SetSizer (DiscreteLogOutcomePanelBoxSizer);
	DiscreteLogIterationsStaticText = new wxStaticText (DiscreteLogOutcomePanel, wxID_ANY, _ ("Iterations"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogOutcomePanel, wxSize (-1,-1) ), 0);

	DiscreteLogOutcomePanelBoxSizer->Add (DiscreteLogIterationsStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiscreteLogIterationsTextCtrl = new wxTextCtrl (DiscreteLogOutcomePanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxDLG_UNIT (DiscreteLogOutcomePanel, wxSize (55,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogIterationsTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogOutcomePanelBoxSizer->Add (DiscreteLogIterationsTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiscreteLogTimeStaticText = new wxStaticText (DiscreteLogOutcomePanel, wxID_ANY, _ ("Time"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogOutcomePanel, wxSize (-1,-1) ), 0);

	DiscreteLogOutcomePanelBoxSizer->Add (DiscreteLogTimeStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiscreteLogTimeTextCtrl = new wxTextCtrl (DiscreteLogOutcomePanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxDLG_UNIT (DiscreteLogOutcomePanel, wxSize (55,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogTimeTextCtrl->SetHint (wxT ("") );
#endif

	DiscreteLogOutcomePanelBoxSizer->Add (DiscreteLogTimeTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiscreteLogSolutionPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogSolutionPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* DiscreteLogSolutionPanelBoxSizer = new wxBoxSizer (wxVERTICAL);
	DiscreteLogSolutionPanel->SetSizer (DiscreteLogSolutionPanelBoxSizer);

	wxBoxSizer* xRow = new wxBoxSizer(wxHORIZONTAL);

	DiscreteLogSolutionPanelBoxSizer->Add(xRow, 0, wxEXPAND);

	DiscreteLogSolutionxStaticText = new wxStaticText (DiscreteLogSolutionPanel, wxID_ANY, _ ("x"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolutionPanel, wxSize (-1,-1) ), 0);

	xRow->Add (DiscreteLogSolutionxStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (5) );

	DiscreteLogSolutionxTextCtrl = new wxTextCtrl (DiscreteLogSolutionPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolutionPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogSolutionxTextCtrl->SetHint (wxT ("") );
#endif

	xRow->Add (DiscreteLogSolutionxTextCtrl, 1, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (5) );

	wxBoxSizer* checkRow = new wxBoxSizer(wxHORIZONTAL);

	DiscreteLogSolutionPanelBoxSizer->Add(checkRow, 0, wxEXPAND);

	DiscreteLogCheckSolutionStaticText = new wxStaticText (DiscreteLogSolutionPanel, wxID_ANY, _ ("check: 2^x (mod p) ="), wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolutionPanel, wxSize (-1,-1) ), 0);

	checkRow->Add (DiscreteLogCheckSolutionStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (5) );

	DiscreteLogCheckSolutionTextCtrl = new wxTextCtrl (DiscreteLogSolutionPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiscreteLogSolutionPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiscreteLogCheckSolutionTextCtrl->SetHint (wxT ("") );
#endif

	checkRow->Add (DiscreteLogCheckSolutionTextCtrl, 1, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (5) );

	/*DiscreteLogStaticLine3 = new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxLI_HORIZONTAL);
	DiscreteLogBoxSizer->Add (DiscreteLogStaticLine3, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) ); */

	DiscreteLogClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DiscreteLogBoxSizer->Add (DiscreteLogClosePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* DiscreteLogCancelClosePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiscreteLogClosePanel->SetSizer (DiscreteLogCancelClosePanelBoxSizer);

	DiscreteLogCancelClosePanelBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	DiscreteLogCloseButton = new wxButton (DiscreteLogClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxDLG_UNIT (DiscreteLogClosePanel, wxSize (-1,-1) ), 0);
	DiscreteLogCloseButton->SetToolTip(wxT("Close the discrete log dialog."));

	DiscreteLogCancelClosePanelBoxSizer->Add (DiscreteLogCloseButton, 0, wxALL, WXC_FROM_DIP (20) );
	SetName (wxT ("DiscreteLog") );
	SetSize (785,650);

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
	// Ensure the restored dialog is wide enough for large integers.
    // Establish minimum dimensions from the complete dialog layout.
    Layout();

    wxSize minimumSize = GetSizer()->CalcMin();
    minimumSize.IncTo(wxSize(1050, 0));

    SetMinClientSize(minimumSize);

    wxSize currentClientSize = GetClientSize();
    currentClientSize.IncTo(minimumSize);

    SetClientSize(currentClientSize);
    Layout();

    // Connect events
	DiscreteLogBuildProblemButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogBuildProblemButtonPress), NULL, this);
	DiscreteLogAutogenerateyButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogAutogenerateyButtonPress), NULL, this);
	DiscreteLogButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogTryToSolveButtonPress), NULL, this);
	DiscreteLogCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogCloseButtonPress), NULL, this);
	DiscreteLogFourPrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogFivePrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogSixPrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogSevenPrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogEightPrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogNinePrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED,
	      wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogTenPrimeFactorsRadioButton->Connect (wxEVT_COMMAND_RADIOBUTTON_SELECTED,
	      wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);

	mip = get_mip();

	RadioButton[0] = DiscreteLogFourPrimeFactorsRadioButton;
	RadioButton[1] = DiscreteLogFivePrimeFactorsRadioButton;
	RadioButton[2] = DiscreteLogSixPrimeFactorsRadioButton;
	RadioButton[3] = DiscreteLogSevenPrimeFactorsRadioButton;
	RadioButton[4] = DiscreteLogEightPrimeFactorsRadioButton;
	RadioButton[5] = DiscreteLogNinePrimeFactorsRadioButton;
	RadioButton[6] = DiscreteLogTenPrimeFactorsRadioButton;
}

DiscreteLog::~DiscreteLog()
{
	DiscreteLogBuildProblemButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogBuildProblemButtonPress), NULL, this);
	DiscreteLogAutogenerateyButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogAutogenerateyButtonPress), NULL, this);
	DiscreteLogButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogTryToSolveButtonPress), NULL, this);
	DiscreteLogCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteLog::OnDiscreteLogCloseButtonPress), NULL, this);
	DiscreteLogFourPrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogFivePrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogSixPrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogSevenPrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogEightPrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogNinePrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED,
	      wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
	DiscreteLogTenPrimeFactorsRadioButton->Disconnect (wxEVT_COMMAND_RADIOBUTTON_SELECTED,
	      wxCommandEventHandler (DiscreteLog::OnRadioButtonClicked), NULL, this);
}

void DiscreteLog::writeTextCtrl (wxTextCtrl *t, const Big& v)
{
	char buf[300];

	mip->IOBASE = 10;

	buf << v;
	t->SetValue (buf);
}

void DiscreteLog::ScanRadioButtons ()
{
	int i;

	for (i=0; i < 7; i++)
		if (RadioButton[i]->GetValue() )
			break;

	switch (i)
	{
	case 0:
		NPRIMES = 4;
		radioButtonSelected = 0;
		break;

	case 1:
		NPRIMES = 5;
		radioButtonSelected = 1;
		break;

	case 2:
		NPRIMES = 6;
		radioButtonSelected = 2;
		break;

	case 3:
		NPRIMES = 7;
		radioButtonSelected = 3;
		break;

	case 4:
		NPRIMES = 8;
		radioButtonSelected = 4;
		break;
	case 5:
		NPRIMES = 9;
		radioButtonSelected = 5;
		break;

	case 6:
		NPRIMES = 10;
		radioButtonSelected = 6;
		break;
	}
}

void DiscreteLog::OnDiscreteLogAutogenerateyButtonPress (wxCommandEvent& event)
{
	y = rand (p);
	writeTextCtrl (DiscreteLogyValueTextCtrl, y);
}

void DiscreteLog::OnDiscreteLogBuildProblemButtonPress (wxCommandEvent& event)
{
	Big primeFactor;
	int i;
	char buf[300];
	wxString s;
	int numberOfBitsInPrime;

	ScanRadioButtons();
	DiscreteLogPrimeFactorsTextCtrl->Clear();
	p = genprime (NPRIMES, factors);
	writeTextCtrl (DiscreteLogpValueTextCtrl, p);

	for (i=0; i < NPRIMES; i++)
	{
		primeFactor = factors[i];
		buf << primeFactor;
		s = wxString (buf) + wxT ("\n");
		*DiscreteLogPrimeFactorsTextCtrl << s;
	}
	numberOfBitsInPrime = bits(p);
	NumberOfBitsInPrimeTextCtrl->SetValue(wxString(itoa(numberOfBitsInPrime)));
}

void DiscreteLog::OnDiscreteLogTryToSolveButtonPress (wxCommandEvent& event)
{
	// bool runUntilCompletion;
	wxString *dlpProcess = new wxString[NPRIMES+3];
	wxArrayString dlAlgorithmOutput;
	int i, j;
	char buf[625];
	long wxExecuteReturnValue;
	miracl *mip = get_mip();

	// clearAll();
	ScanRadioButtons();

	/*if (DiscreteLogRunUntilCompletionRadioButton->GetValue() )
		runUntilCompletion = true;
	else
		runUntilCompletion = false;*/

	mip->IOBASE = 10;

	sprintf (buf, "%d", NPRIMES);
	dlpProcess[0] = wxString (buf);

	j = 1;

	for (i=0; i < NPRIMES; i++)
	{
		buf << factors[i];
		dlpProcess[j] = wxString (buf);
		j++;
	}

	dlpProcess[j] = DiscreteLogyValueTextCtrl->GetValue();
	j++;

	/* if (runUntilCompletion)
		dlpProcess[j] = wxT ("0");
	else
	if (DiscreteLogRunForMSecondsTextCtrl->GetValue().length() == 0)
	{
		report (wxT ("Specify the number of seconds to run the algorithm"), wxT ("Note") );
		return;
	}*/

	// dlpProcess[j] = DiscreteLogRunForMSecondsTextCtrl->GetValue();
	for (int k=0; k<dlAlgorithmOutput.GetCount(); k++)
	{
		cout << dlpProcess[k] << endl;
	}

	dlAlgorithmOutput = runApplication(wxT("index"), dlpProcess, NPRIMES+3);

	if (dlAlgorithmOutput.GetCount() == 0)
		return;

	respondToResults (dlAlgorithmOutput);
	delete[] dlpProcess;
}

void DiscreteLog::respondToResults (const wxArrayString& dlAlgorithmOutput)
{
	int i,k,count;
	wxString arg,id,msg;
	char str[1024];


	count = dlAlgorithmOutput.GetCount();

	for(k=0; k<count; k++)
	{
		arg = dlAlgorithmOutput[k];
		id = arg.SubString(0,1);
		msg = arg.AfterFirst(':');
		strncpy(str, (const char*)msg.mb_str(wxConvUTF8), 1023);


// assuming you want UTF-8, change the wxConv* parameter as needed

		if (id == wxT("M:"))
		{
			reportError(wxT("The modulus is not prime."),wxT("Note"));
			EndModal (wxID_CANCEL);
		}
		else if (id == wxT("F:"))
		{
			reportError(wxT("Failure to compute the discrete logarithm."),wxT("Note"));
			EndModal (wxID_CANCEL);
		}
		/*else if(id == wxT("Z:"))
		{
			reportError(wxT("The algorithm timed out."),wxT("Note"));
			EndModal (wxID_CANCEL);
		}*/
		else if (id == wxT("X:"))
		{
			writeTextCtrl(DiscreteLogSolutionxTextCtrl,Big(str));
		}
		/* else if(id == wxT("W:"))
		{
			writeTextCtrl(DiscreteLogOutcomeTextCtrl,Big(str));
		} */
		else if(id == wxT("I:"))
		{
			writeTextCtrl(DiscreteLogIterationsTextCtrl,Big(str));
		}
		else if(id == wxT("T:"))
		{
			writeTextCtrl(DiscreteLogTimeTextCtrl,Big(str));
		}
		else if(id == wxT("C:"))
		{
			writeTextCtrl(DiscreteLogCheckSolutionTextCtrl, Big(str));
		}
	}
}

void DiscreteLog::OnDiscreteLogCloseButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_OK);
}

void DiscreteLog::OnRadioButtonClicked (wxCommandEvent& event)
{
	ScanRadioButtons();
}
