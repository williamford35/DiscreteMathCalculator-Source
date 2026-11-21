/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/matching/StableMarriageDialog.h"

#include <wx/intl.h>
#include <wx/string.h>

#include "common/randomc.h"
#include "graphs/algorithms/matching/StableMarriage.h"
#include "graphs/build/GraphEdgeStyles.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/GraphType.h"
#include <wx/msgdlg.h>

wxString men[10] =
{
	wxT ("Armstrong"),
	wxT ("Chris"),
	wxT ("Harry"),
	wxT ("Laurence"),
	wxT ("Porter"),
	wxT ("Quincy"),
	wxT ("Rodney"),
	wxT ("Spencer"),
	wxT ("Tom"),
	wxT ("Zane")
};

wxString women[10] =
{
	wxT ("Becca"),
	wxT ("Cholie"),
	wxT ("Danielle"),
	wxT ("Matilda"),
	wxT ("Penny"),
	wxT ("Samantha"),
	wxT ("Shirley"),
	wxT ("Ursala"),
	wxT ("Violet"),
	wxT ("Wendy")
};

StableMarriageDialog::StableMarriageDialog (wxWindow* parent, DisplayAlgorithmResults **pairg,
      wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), menPref (NULL), womenPref (NULL),
	  myParent (parent), pairingGraph (pairg)
{
	wxFlexGridSizer* flexGridSizer122 = new wxFlexGridSizer (3, 1, 0, 0);
	flexGridSizer122->SetFlexibleDirection (wxBOTH);
	flexGridSizer122->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	flexGridSizer122->AddGrowableCol (0);
	flexGridSizer122->AddGrowableRow (1);
	this->SetSizer (flexGridSizer122);

	ID_MEN_WOMEN_LABEL_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (930,30), wxTAB_TRAVERSAL);

	flexGridSizer122->Add (ID_MEN_WOMEN_LABEL_PANEL, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	wxBoxSizer* numberOfMenAndWomenBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ID_MEN_WOMEN_LABEL_PANEL->SetSizer (numberOfMenAndWomenBoxSizer);

	ID_NUMBER_OF_MEN_AND_WOMEN_PANEL = new wxPanel (ID_MEN_WOMEN_LABEL_PANEL, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	numberOfMenAndWomenBoxSizer->Add (ID_NUMBER_OF_MEN_AND_WOMEN_PANEL, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	wxBoxSizer* nBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ID_NUMBER_OF_MEN_AND_WOMEN_PANEL->SetSizer (nBoxSizer);

	ID_N_STATICTEXT = new wxStaticText (ID_NUMBER_OF_MEN_AND_WOMEN_PANEL, wxID_ANY, _ ("n"), wxDefaultPosition, wxSize (-1,-1), wxALIGN_CENTRE);

	nBoxSizer->Add (ID_N_STATICTEXT, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);

	ID_N_TEXTCTRL = new wxTextCtrl (ID_NUMBER_OF_MEN_AND_WOMEN_PANEL, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (25,-1), 0);
#if wxVERSION_NUMBER >= 3000
	ID_N_TEXTCTRL->SetHint (wxT ("") );
#endif

	nBoxSizer->Add (ID_N_TEXTCTRL, 5, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	numberOfMenAndWomenBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	ID_PARTNERS_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	flexGridSizer122->Add (ID_PARTNERS_PANEL, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	wxBoxSizer* partnersBoxSizer = new wxBoxSizer (wxVERTICAL);
	ID_PARTNERS_PANEL->SetSizer (partnersBoxSizer);

	ID_PARTNERS_LISTCTRL = new wxListCtrl (ID_PARTNERS_PANEL, wxID_ANY, wxDefaultPosition, wxSize (930,220), wxLC_REPORT);

	partnersBoxSizer->Add (ID_PARTNERS_LISTCTRL, 1, wxALL|wxALIGN_LEFT, 1);

	ID_BUTTONS_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	flexGridSizer122->Add (ID_BUTTONS_PANEL, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	wxBoxSizer* buttonsPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ID_BUTTONS_PANEL->SetSizer (buttonsPanelBoxSizer);

	buttonsPanelBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_BOTTOM, 1);

	buttonsPanelBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_BOTTOM, 1);

	buttonsPanelBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_BOTTOM, 1);

	buttonsPanelBoxSizer->Add (0, 0, 1, wxALL|wxALIGN_BOTTOM, 1);

	ID_CREATE_PREFERENCES_BUTTON = new wxButton (ID_BUTTONS_PANEL, wxID_ANY, _ ("Create Preferences"), wxDefaultPosition, wxSize (-1,-1), 0);
	ID_CREATE_PREFERENCES_BUTTON->SetToolTip (_ ("Create n stable marriages.") );

	buttonsPanelBoxSizer->Add (ID_CREATE_PREFERENCES_BUTTON, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	ID_MATCH_BUTTON = new wxButton (ID_BUTTONS_PANEL, wxID_ANY, _ ("Match"), wxDefaultPosition, wxSize (-1,-1), 0);
	ID_MATCH_BUTTON->SetDefault();
	ID_MATCH_BUTTON->SetToolTip (_ ("Draw the bipartite graph and list the pairings.") );

	buttonsPanelBoxSizer->Add (ID_MATCH_BUTTON, 1, wxALL|wxALIGN_BOTTOM, 1);

	ID_CLOSE_BUTTON = new wxButton (ID_BUTTONS_PANEL, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);
	ID_CLOSE_BUTTON->SetToolTip (_ ("Cancel the dialog.") );

	buttonsPanelBoxSizer->Add (ID_CLOSE_BUTTON, 1, wxALL|wxALIGN_BOTTOM, 1);

	SetSizeHints (930,500);

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	Centre (wxBOTH);
	// Connect events
	ID_CREATE_PREFERENCES_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (StableMarriageDialog::OncreatePreferencesButtonClick), NULL, this);
	ID_MATCH_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (StableMarriageDialog::OnmatchButtonClick), NULL, this);
	ID_CLOSE_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (StableMarriageDialog::OncloseButtonClick), NULL, this);
}

StableMarriageDialog::~StableMarriageDialog()
{
	ID_CREATE_PREFERENCES_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (StableMarriageDialog::OncreatePreferencesButtonClick), NULL, this);
	ID_MATCH_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (StableMarriageDialog::OnmatchButtonClick), NULL, this);
	ID_CLOSE_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (StableMarriageDialog::OncloseButtonClick), NULL, this);

	if (menPref != NULL)
	{
		delete menPref;
		delete womenPref;
	}
}

void StableMarriageDialog::OncreatePreferencesButtonClick (wxCommandEvent& event)
{
	wxString nString;
	long tmpn;

	nString = ID_N_TEXTCTRL->GetValue();
	nString.ToLong (&tmpn);
	n = (int) tmpn;

	if (n > 10)
	{
		wxString message, caption;

		message.append (wxT ("The maximum problem size is 10.") );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return;
	}

	ID_PARTNERS_LISTCTRL->ClearAll();
	InitWithMenAndWomen();
}

void StableMarriageDialog::OnmatchButtonClick (wxCommandEvent& event)
{
	vector<int> partners;

	if (menPref == NULL)
	{
		wxString message, caption;

		message.append (wxT ("No preferences have been created.") );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return;
	}

	StableMarriage  marriagePartners (*menPref, *womenPref);


	partners = marriagePartners.stable();

	drawMatching (partners);
}

void StableMarriageDialog::InitWithMenAndWomen()
{
	wxListItem partnersCol;

	partnersCol.SetImage (-1);

	partnersCol.SetAlign (wxLIST_FORMAT_LEFT);
	partnersCol.SetText (_T ("Men") );
	ID_PARTNERS_LISTCTRL->InsertColumn (0, partnersCol);
	ID_PARTNERS_LISTCTRL->SetColumnWidth (0,60);

	partnersCol.SetText (_T ("Preferences") );
	//partnersCol.SetAlign(wxLIST_FORMAT_LEFT);
	ID_PARTNERS_LISTCTRL->InsertColumn (1, partnersCol);
	ID_PARTNERS_LISTCTRL->SetColumnWidth (1,400);

	partnersCol.SetText (_T ("Women") );
	//partnersCol.SetAlign(wxLIST_FORMAT_LEFT);
	ID_PARTNERS_LISTCTRL->InsertColumn (2, partnersCol);
	ID_PARTNERS_LISTCTRL->SetColumnWidth (2,60);

	partnersCol.SetText (_T ("Preferences") );
	//partnersCol.SetAlign(wxLIST_FORMAT_LEFT);
	ID_PARTNERS_LISTCTRL->InsertColumn (3, partnersCol);
	ID_PARTNERS_LISTCTRL->SetColumnWidth (3,400);

	// to speed up inserting we hide the controls temporarily
	ID_PARTNERS_LISTCTRL->Hide();

	if (menPref != NULL)
	{
		delete menPref;
		delete womenPref;
	}

	menPref = new matrix<int> (n,n);
	womenPref = new matrix<int> (n,n);

	for (int i=0; i <n; i++)
		InsertItem (ID_PARTNERS_LISTCTRL, i);

	ID_PARTNERS_LISTCTRL->Show();
}

void StableMarriageDialog::InsertItem (wxListCtrl* listCtrl, int i)
{
	wxString partnerList;
	long tmp = listCtrl->InsertItem (i, men[i], 0);
	listCtrl->SetItemData (tmp, i);
	int k;

	wxListItem partnersCol;
	vector<int> womenForMen (n), menForWomen (n);

	for (k=0; k < n; k++)
	{
		womenForMen[k] = k;
		menForWomen[k] = k;
	}

	Shuffle (womenForMen, menForWomen);

	for (k=0; k < n; k++)
		partnerList += women[womenForMen[k]] + wxT ("  ");

	listCtrl->SetItem (tmp, 1, partnerList);
	(*menPref) [i] = womenForMen;
	listCtrl->SetItem (tmp, 2, women[i]);

	Shuffle (womenForMen, menForWomen);

	partnerList.Clear();

	for (k=0; k < n; k++)
		partnerList += men[menForWomen[k]] + wxT ("  ");

	(*womenPref) [i] = menForWomen;
	listCtrl->SetItem (tmp, 3, partnerList);
}

void StableMarriageDialog::Shuffle (vector<int>& womenForMen,
                                    vector<int>& menForWomen)
{
	// an object uses one sequence of random numbers
	static CRandomMersenne rnd;
	int randIndex;
	int tmp;

	// shuffle the men and women
	for (int i=0; i < n; i++)
	{
		// compute a random index from i to n-1
		randIndex = rnd.IRandomX (i, n-1);
		// exchange womenForMen[i] with womenForMen[randIndex]
		tmp = womenForMen[i];
		womenForMen[i] = womenForMen[randIndex];
		womenForMen[randIndex] = tmp;

		randIndex = rnd.IRandomX (i, n-1);
		tmp = menForWomen[i];
		menForWomen[i] = menForWomen[randIndex];
		menForWomen[randIndex] = tmp;
	}
}

void StableMarriageDialog::OncloseButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void StableMarriageDialog::drawMatching (const vector<int>& partners)
{
	wxString u_str, v_str;
	BuildGraph *matching;
	int i;

	matching = new BuildGraph (wxT (""), undir, hierarchical, false);

	for (i=n-1; i >= 0; i--)
		matching->insertEdge (men[i], wxT ("#"),wxT(""), *wxBLACK_PEN, wxColour(255,215,0),
		                      wxColour(255,215,0));

	for (i=n-1; i >= 0; i--)
		matching->insertEdge (women[i], wxT ("#"),wxT("" ), *wxBLACK_PEN,wxColour(43,255,0),
		                      wxColour(43,255,0));

	// woman i marries man partners[i]
	for (unsigned i=0; i < partners.size(); i++)
		matching->insertEdge (men[partners[i]], women[i],wxT(""), *wxBLACK_PEN, wxColour(255,215,0),
		                      wxColour(255,215,0));

	matching->hierarchicalLayout();

	*pairingGraph = new DisplayAlgorithmResults (myParent, matching, undir,
	      hierarchical, DisplayAlgorithmResults::STABLEMARRIAGE);
	wxTextCtrl* textOutput = (**pairingGraph).getDisplayAlgorithmResultsTextCtrl();
	textOutput->Hide();
	wxStaticText*stableMarriageLabel = (**pairingGraph).getDisplayAlgorithmResultsStaticText();
	stableMarriageLabel->Hide();

	EndModal (wxID_OK);
}
