/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/HamPathDialog.h"
#include "common/ImageFiles.h"
#include "common/report.h"

#include <wx/msgdlg.h>

HamPathDialog::HamPathDialog (wxWindow* parent, wxString *v, const char *imageFileName, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style),verticesSelected (v)
{
	wxFlexGridSizer* hamiltonianPathFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	hamiltonianPathFlexGridSizer->SetFlexibleDirection (wxBOTH);
	hamiltonianPathFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	hamiltonianPathFlexGridSizer->AddGrowableCol (0);
	hamiltonianPathFlexGridSizer->AddGrowableRow (0);
	hamiltonianPathFlexGridSizer->AddGrowableRow (1);
	this->SetSizer (hamiltonianPathFlexGridSizer);

	wxBoxSizer* hamiltonianPathBoxSizer = new wxBoxSizer (wxVERTICAL);

	hamiltonianPathFlexGridSizer->Add (hamiltonianPathBoxSizer, 1, wxALL|wxEXPAND, 1);

	ID_HAMILTONIAN_PATH_STATICBITMAP = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (imageFileName))), wxDefaultPosition, wxSize (-1,-1), 0);

	hamiltonianPathBoxSizer->Add (ID_HAMILTONIAN_PATH_STATICBITMAP, 5, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	ID_HAMTONIAN_PATH_N_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	hamiltonianPathBoxSizer->Add (ID_HAMTONIAN_PATH_N_PANEL, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* resultHamiltonianPath_s_BoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ID_HAMTONIAN_PATH_N_PANEL->SetSizer (resultHamiltonianPath_s_BoxSizer);

	ID_HAMILTONIAN_PATH_S_STATICTEXT = new wxStaticText (ID_HAMTONIAN_PATH_N_PANEL, wxID_ANY, _ ("s"), wxDefaultPosition, wxSize (-1,-1), 0);

	resultHamiltonianPath_s_BoxSizer->Add (ID_HAMILTONIAN_PATH_S_STATICTEXT, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_HAMILTONIAN_PATH_S_TEXTCTRL = new wxTextCtrl (ID_HAMTONIAN_PATH_N_PANEL, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	ID_HAMILTONIAN_PATH_S_TEXTCTRL->SetHint (wxT (""));
#endif

	resultHamiltonianPath_s_BoxSizer->Add (ID_HAMILTONIAN_PATH_S_TEXTCTRL, 6, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_HAMILTONIAN_PATH_T_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	hamiltonianPathBoxSizer->Add (ID_HAMILTONIAN_PATH_T_PANEL, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* hamiltonianPath_t_BoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ID_HAMILTONIAN_PATH_T_PANEL->SetSizer (hamiltonianPath_t_BoxSizer);

	ID_HAMILTONIAN_PATH_T_STATICTEXT = new wxStaticText (ID_HAMILTONIAN_PATH_T_PANEL, wxID_ANY, _ ("t"), wxDefaultPosition, wxSize (-1,-1), 0);

	hamiltonianPath_t_BoxSizer->Add (ID_HAMILTONIAN_PATH_T_STATICTEXT, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_HAMILTONIAN_PATH_T_TEXTCTRL = new wxTextCtrl (ID_HAMILTONIAN_PATH_T_PANEL, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	ID_HAMILTONIAN_PATH_T_TEXTCTRL->SetHint (wxT (""));
#endif

	hamiltonianPath_t_BoxSizer->Add (ID_HAMILTONIAN_PATH_T_TEXTCTRL, 6, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_HAMILTONIAN_PATH_BUTTON_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	hamiltonianPathFlexGridSizer->Add (ID_HAMILTONIAN_PATH_BUTTON_PANEL, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* hamiltonianButtonBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ID_HAMILTONIAN_PATH_BUTTON_PANEL->SetSizer (hamiltonianButtonBoxSizer);

	hamiltonianButtonBoxSizer->Add (0, 0, 1, wxALL|wxEXPAND, 5);

	hamiltonianButtonBoxSizer->Add (0, 0, 1, wxALL|wxEXPAND, 5);

	ID_HAMILTONIAN_PATH_COMPUTE_BUTTON = new wxButton (ID_HAMILTONIAN_PATH_BUTTON_PANEL, wxID_ANY, _ ("Compute"), wxDefaultPosition, wxSize (-1,-1), 0);
	ID_HAMILTONIAN_PATH_COMPUTE_BUTTON->SetDefault();

	hamiltonianButtonBoxSizer->Add (ID_HAMILTONIAN_PATH_COMPUTE_BUTTON, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_HAMILTONIAN_PATH_CANCEL_BUTTON = new wxButton (ID_HAMILTONIAN_PATH_BUTTON_PANEL, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);

	hamiltonianButtonBoxSizer->Add (ID_HAMILTONIAN_PATH_CANCEL_BUTTON, 0, wxALL, 5);

	SetSizeHints (-1,-1);

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	Centre (wxHORIZONTAL);

	// Connect events
	ID_HAMILTONIAN_PATH_COMPUTE_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HamPathDialog::OnComputeButtonClicked), NULL, this);
	ID_HAMILTONIAN_PATH_CANCEL_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HamPathDialog::OnCancelButtonClicked), NULL, this);
}

HamPathDialog::~HamPathDialog()
{
	ID_HAMILTONIAN_PATH_COMPUTE_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HamPathDialog::OnComputeButtonClicked), NULL, this);
	ID_HAMILTONIAN_PATH_CANCEL_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HamPathDialog::OnCancelButtonClicked), NULL, this);
}

void HamPathDialog::OnComputeButtonClicked (wxCommandEvent& event)
{
	wxString s, t;

	s = ID_HAMILTONIAN_PATH_S_TEXTCTRL->GetValue();
	t = ID_HAMILTONIAN_PATH_T_TEXTCTRL->GetValue();

	if (s.length() == 0 || t.length() == 0)
	{
		report (wxT ("Either s or t has no value."),"Note");
		return;
	}
	else
	{
		verticesSelected[0] = s;
		verticesSelected[1] = t;
	}

	EndModal (wxID_OK);
}

void HamPathDialog::OnCancelButtonClicked (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
