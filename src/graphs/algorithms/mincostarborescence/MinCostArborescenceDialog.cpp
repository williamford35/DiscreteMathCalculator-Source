/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/mincostarborescence/MinCostArborescenceDialog.h"
#include "common/ImageFiles.h"

#include <wx/msgdlg.h>

MinCostArborescenceDialog::MinCostArborescenceDialog (wxWindow* parent, wxString *source, const char *imageFileName, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) :
	wxDialog (parent, id, title, pos, size, style), sourceVertex(source)
{
	wxFlexGridSizer* minCostArborescenceFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	minCostArborescenceFlexGridSizer->SetFlexibleDirection (wxBOTH);
	minCostArborescenceFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	minCostArborescenceFlexGridSizer->AddGrowableCol (0);
	minCostArborescenceFlexGridSizer->AddGrowableRow (1);
	minCostArborescenceFlexGridSizer->AddGrowableRow (2);
	this->SetSizer (minCostArborescenceFlexGridSizer);

	MinCostArborescenceStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (imageFileName))), wxDefaultPosition, wxSize (-1,-1), 0);

	minCostArborescenceFlexGridSizer->Add (MinCostArborescenceStaticBitmap, 0, wxALL|wxALIGN_CENTER, 5);

	MinCostArborescenceSourceVertexPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	minCostArborescenceFlexGridSizer->Add (MinCostArborescenceSourceVertexPanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* MinCostArborescenceSourceVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	MinCostArborescenceSourceVertexPanel->SetSizer (MinCostArborescenceSourceVertexBoxSizer);

	MinCostArborescenceSourceVertexStaticText = new wxStaticText (MinCostArborescenceSourceVertexPanel, wxID_ANY, _ ("Source vertex"), wxDefaultPosition, wxSize (-1,-1), 0);

	MinCostArborescenceSourceVertexBoxSizer->Add (MinCostArborescenceSourceVertexStaticText, 0, wxALL|wxALIGN_BOTTOM, 5);

	MinCostArborescenceSourceVertexTextCtrl = new wxTextCtrl (MinCostArborescenceSourceVertexPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (100,-1), 0);
#if wxVERSION_NUMBER >= 3000
	MinCostArborescenceSourceVertexTextCtrl->SetHint (wxT (""));
#endif

	MinCostArborescenceSourceVertexBoxSizer->Add (MinCostArborescenceSourceVertexTextCtrl, 0, wxALL|wxALIGN_BOTTOM, 1);

	MinCostArborescenceButtonPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	minCostArborescenceFlexGridSizer->Add (MinCostArborescenceButtonPanel, 0, wxALL, 5);

	wxBoxSizer* MinCostArborescenceButtonBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	MinCostArborescenceButtonPanel->SetSizer (MinCostArborescenceButtonBoxSizer);

	MinCostArborescenceButtonBoxSizer->Add (125, 0, 1, wxALL, 5);

	MinCostArborescenceOKButton = new wxButton (MinCostArborescenceButtonPanel, wxID_ANY, _ ("OK"), wxDefaultPosition, wxSize (-1,-1), 0);
	MinCostArborescenceOKButton->SetDefault();

	MinCostArborescenceButtonBoxSizer->Add (MinCostArborescenceOKButton, 0, wxALL, 10);

	MinCostArborescenceCancelButton = new wxButton (MinCostArborescenceButtonPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);

	MinCostArborescenceButtonBoxSizer->Add (MinCostArborescenceCancelButton, 0, wxALL, 10);

	//SetName (wxT ("MinCostArborescence"));

    SetSize(250,150);

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	CentreOnParent (wxBOTH);
#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this))
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif
	// Connect events
	MinCostArborescenceOKButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MinCostArborescenceDialog::OnOKButtonPressed), NULL, this);
	MinCostArborescenceCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MinCostArborescenceDialog::OnCancelButtonPressed), NULL, this);
}

MinCostArborescenceDialog::~MinCostArborescenceDialog()
{
	MinCostArborescenceOKButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MinCostArborescenceDialog::OnOKButtonPressed), NULL, this);
	MinCostArborescenceCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MinCostArborescenceDialog::OnCancelButtonPressed), NULL, this);
}

void MinCostArborescenceDialog::OnCancelButtonPressed (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void MinCostArborescenceDialog::OnOKButtonPressed (wxCommandEvent& event)
{
	wxString s;

	s = MinCostArborescenceSourceVertexTextCtrl->GetValue();

	if (s.length() == 0)
	{
		reportError (wxT ("The source vertex has no value."));
		return;
	}
	else
		*sourceVertex = s;

	EndModal (wxID_OK);
}

void MinCostArborescenceDialog::reportError (const wxString& err)
{
	wxString message, caption;

	message.append (err);

	wxMessageDialog dialog (this, message, caption, wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}
