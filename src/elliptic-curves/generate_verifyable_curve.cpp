#include "elliptic-curves/ecurvelib.h"

#include "common/report.h"


GenerateRandomVerifyableCurve::GenerateRandomVerifyableCurve (wxWindow* parent, const char *imageFileName, VerifyData *v, int b, const Big& prime, const Big& s, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), vdat (v), generateCalled (false), base (b), p (prime), seed(s)
{
	wxBoxSizer* GenerateRandomVerifyableCurveBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (GenerateRandomVerifyableCurveBoxSizer);

	GenerateRandomVerifyableCurveStaticBitMap = new wxStaticBitmap (this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurveStaticBitMap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveStaiicText = new wxStaticText (this, wxID_ANY, _ ("The prime and a random 160-bit seed create verifyable ec coeffcients A and B."), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurveStaiicText, 0, wxALL, WXC_FROM_DIP (5) );

	GenerateRandomVerifyableCurvePrimePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurvePrimePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* GenerateRandomVerifyableCurvePrimePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GenerateRandomVerifyableCurvePrimePanel->SetSizer (GenerateRandomVerifyableCurvePrimePanelBoxSizer);

	GenerateRandomVerifyableCurvePrimePanelStaticText = new wxStaticText (GenerateRandomVerifyableCurvePrimePanel, wxID_ANY, _ ("prime"), wxDefaultPosition, wxDLG_UNIT (GenerateRandomVerifyableCurvePrimePanel, wxSize (-1,-1) ), 0);

	GenerateRandomVerifyableCurvePrimePanelBoxSizer->Add (GenerateRandomVerifyableCurvePrimePanelStaticText, 0, wxALL, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurvePrimePanelTextCtrl = new wxTextCtrl (GenerateRandomVerifyableCurvePrimePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (GenerateRandomVerifyableCurvePrimePanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	GenerateRandomVerifyableCurvePrimePanelTextCtrl->SetHint (wxT ("") );
#endif

	GenerateRandomVerifyableCurvePrimePanelBoxSizer->Add (GenerateRandomVerifyableCurvePrimePanelTextCtrl, 1, wxALL, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveSeedPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurveSeedPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* GenerateRandomVerifyableCurveSeedPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GenerateRandomVerifyableCurveSeedPanel->SetSizer (GenerateRandomVerifyableCurveSeedPanelBoxSizer);

	GenerateRandomVerifyableCurveSeedStaticText = new wxStaticText (GenerateRandomVerifyableCurveSeedPanel, wxID_ANY, _ ("seed"), wxDefaultPosition, wxDLG_UNIT (GenerateRandomVerifyableCurveSeedPanel, wxSize (-1,-1) ), 0);

	GenerateRandomVerifyableCurveSeedPanelBoxSizer->Add (GenerateRandomVerifyableCurveSeedStaticText, 0, wxALL, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveSeedTextCtrl = new wxTextCtrl (GenerateRandomVerifyableCurveSeedPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (GenerateRandomVerifyableCurveSeedPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	GenerateRandomVerifyableCurveSeedTextCtrl->SetHint (wxT ("") );
#endif

	GenerateRandomVerifyableCurveSeedPanelBoxSizer->Add (GenerateRandomVerifyableCurveSeedTextCtrl, 1, wxALL, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveStaticLine = new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurveStaticLine, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	GenerateRandomVerifyableCurveGenerateButton = new wxButton (this, wxID_ANY, _ ("Generate"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurveGenerateButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	GenerateRamdomVerifyableCurveAPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRamdomVerifyableCurveAPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* GenerateRamdomVerifyableCurveAPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GenerateRamdomVerifyableCurveAPanel->SetSizer (GenerateRamdomVerifyableCurveAPanelBoxSizer);

	GenerateRandomVerifyableCurveAStaticText = new wxStaticText (GenerateRamdomVerifyableCurveAPanel, wxID_ANY, _ ("A:"), wxDefaultPosition, wxDLG_UNIT (GenerateRamdomVerifyableCurveAPanel, wxSize (-1,-1) ), 0);

	GenerateRamdomVerifyableCurveAPanelBoxSizer->Add (GenerateRandomVerifyableCurveAStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveATextCtrl = new wxTextCtrl (GenerateRamdomVerifyableCurveAPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (GenerateRamdomVerifyableCurveAPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	GenerateRandomVerifyableCurveATextCtrl->SetHint (wxT ("") );
#endif

	GenerateRamdomVerifyableCurveAPanelBoxSizer->Add (GenerateRandomVerifyableCurveATextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	GenerateRamdomVerifyableCurveBPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRamdomVerifyableCurveBPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* GenerateRamdomVerifyableCurveBPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GenerateRamdomVerifyableCurveBPanel->SetSizer (GenerateRamdomVerifyableCurveBPanelBoxSizer);

	GenerateRandomVerifyableCurveBStaticText = new wxStaticText (GenerateRamdomVerifyableCurveBPanel, wxID_ANY, _ ("B:"), wxDefaultPosition, wxDLG_UNIT (GenerateRamdomVerifyableCurveBPanel, wxSize (-1,-1) ), 0);

	GenerateRamdomVerifyableCurveBPanelBoxSizer->Add (GenerateRandomVerifyableCurveBStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveBTextCtrl = new wxTextCtrl (GenerateRamdomVerifyableCurveBPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (GenerateRamdomVerifyableCurveBPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	GenerateRandomVerifyableCurveBTextCtrl->SetHint (wxT ("") );
#endif

	GenerateRamdomVerifyableCurveBPanelBoxSizer->Add (GenerateRandomVerifyableCurveBTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	GenerateRandomVerifyableCurveRandomControlPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	GenerateRandomVerifyableCurveBoxSizer->Add (GenerateRandomVerifyableCurveRandomControlPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (10) );

	wxBoxSizer* GenerateRandomVerifyableCurveRandomControlPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GenerateRandomVerifyableCurveRandomControlPanel->SetSizer (GenerateRandomVerifyableCurveRandomControlPanelBoxSizer);

	GenerateRandomVerifyableCurveRandomControlPanelBoxSizer->Add (0, 0, 1, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	GenerateRandomVerifyableCurveRandomControlPanelCancelButton = new wxButton (GenerateRandomVerifyableCurveRandomControlPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxDLG_UNIT (GenerateRandomVerifyableCurveRandomControlPanel, wxSize (-1,-1) ), 0);

	GenerateRandomVerifyableCurveRandomControlPanelBoxSizer->Add (GenerateRandomVerifyableCurveRandomControlPanelCancelButton, 0, wxALL, WXC_FROM_DIP (10) );

	GenerateRandomVerifyableCurveRandomControlPanelSaveButton = new wxButton (GenerateRandomVerifyableCurveRandomControlPanel, wxID_ANY, _ ("Save"), wxDefaultPosition, wxDLG_UNIT (GenerateRandomVerifyableCurveRandomControlPanel, wxSize (-1,-1) ), 0);

	GenerateRandomVerifyableCurveRandomControlPanelBoxSizer->Add (GenerateRandomVerifyableCurveRandomControlPanelSaveButton, 0, wxALL, WXC_FROM_DIP (10) );

	SetName (wxT ("GenerateRandomVerifyableCurve") );
	SetSize (500,550);

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
	GenerateRandomVerifyableCurveGenerateButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveGenerateButtonPress), NULL, this);
	GenerateRandomVerifyableCurveRandomControlPanelCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveRandomCancelButton), NULL, this);
	GenerateRandomVerifyableCurveRandomControlPanelSaveButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveRandomSaveButton), NULL, this);

	WriteResult (GenerateRandomVerifyableCurvePrimePanelTextCtrl, p);
	WriteResult (GenerateRandomVerifyableCurveSeedTextCtrl, seed);
}

GenerateRandomVerifyableCurve::~GenerateRandomVerifyableCurve()
{

	GenerateRandomVerifyableCurveGenerateButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveGenerateButtonPress), NULL, this);
	GenerateRandomVerifyableCurveRandomControlPanelCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveRandomCancelButton), NULL, this);
	GenerateRandomVerifyableCurveRandomControlPanelSaveButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveRandomSaveButton), NULL, this);
}

void GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveRandomCancelButton (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveRandomSaveButton (wxCommandEvent& event)
{
	if (generateCalled == false)
	{
		report (wxT ("Generation of A and B not done."),  wxT ("Note") );
		return;
	}

	vdat->A = A;
	vdat->B = B;

	EndModal (wxID_OK);
}

void GenerateRandomVerifyableCurve::WriteResult (wxTextCtrl* t, const Big& v)
{
	char buf[625];
	wxString s;
	miracl *mip = get_mip();

	if (base == 10)
		mip->IOBASE = 10;
	else
		mip->IOBASE = 16;

	buf << v;
	t->SetValue (buf);
}

void GenerateRandomVerifyableCurve::OnGenerateRandomVerifyableCurveGenerateButtonPress (wxCommandEvent& event)
{
	wxString str;
	char buf[625];
	miracl *mip = get_mip();

	str = GenerateRandomVerifyableCurvePrimePanelTextCtrl->GetValue();
	strcpy (buf, (const char *) str.mb_str() );
	if (base == 10)
		mip->IOBASE = 10;
	else
		mip->IOBASE = 16;
	p = Big (buf);
	if (prime (p) == 0)
	{
		report (wxT ("The number is not prime."), wxT ("Note") );
		generateCalled = false;
		return;
	}

	str = GenerateRandomVerifyableCurveSeedTextCtrl->GetValue();
	strcpy (buf, (const char *) str.mb_str() );
	if (base == 10)
		mip->IOBASE = 10;
	else
		mip->IOBASE = 16;
	seed = Big (buf);

	generateCalled = true;
	generateRandomVerifiableEllipticCurve (p, seed, A, B);
	WriteResult (GenerateRandomVerifyableCurveSeedTextCtrl, seed);
	WriteResult (GenerateRandomVerifyableCurveATextCtrl, A);
	WriteResult (GenerateRandomVerifyableCurveBTextCtrl, B);
}
