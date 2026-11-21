/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/build/HamCycle.h"
#include "common/report.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(HamCycle)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(HamCycle)
const long HamCycle::ID_HAM_CYCLE_STATICTEXT = wxNewId();
const long HamCycle::ID_HAM_CYCLE_NUMBER_OF_VERTICES_STATICTEXT = wxNewId();
const long HamCycle::ID_HAM_CYCLE_NUMBER_OF_VERTICES_TEXTCTRL = wxNewId();
const long HamCycle::ID_HAM_CYCLE_NUMBER_OF_VERTICES_PANEL = wxNewId();
const long HamCycle::ID_HAM_CYCLE_NUMBER_OF_EDGES_STATICTEXT = wxNewId();
const long HamCycle::ID_HAM_CYCLE_NUMBER_OF_EDGES_TEXTCTRL = wxNewId();
const long HamCycle::ID_HAM_CYCLE_NUMBER_OF_EDGES_PANEL = wxNewId();
const long HamCycle::ID_HAM_CYCLE_BUILD_BUTTON = wxNewId();
const long HamCycle::ID_HAM_CYCLE_CANCEL_BUTTON = wxNewId();
const long HamCycle::ID_HAM_CYCLE_BUTTON_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (HamCycle,wxDialog)
	//(*EventTable(HamCycle)
	//*)
END_EVENT_TABLE()

HamCycle::HamCycle (wxWindow* parent,int *n, int *m, wxWindowID id,const wxPoint& pos,const wxSize& size) :
	pnumberOfVertices (n),pnumberOfEdges (m)
{
	//(*Initialize(HamCycle)
	wxBoxSizer* HamCycleBoxSizer;
	wxBoxSizer* hamCycleButtonBoxSizer;
	wxBoxSizer* hamCycleNumberOfEdgesBoxSizer;
	wxBoxSizer* hamCycleNumberOfVerticesBoxSizer;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	HamCycleBoxSizer = new wxBoxSizer (wxVERTICAL);
	HamCycleStaticText = new wxStaticText (this, ID_HAM_CYCLE_STATICTEXT, _ ("Create a Graph for Hamiltonian Cycle Determination"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_HAM_CYCLE_STATICTEXT"));
	HamCycleBoxSizer->Add (HamCycleStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleNumberOfVerticesPanel = new wxPanel (this, ID_HAM_CYCLE_NUMBER_OF_VERTICES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_HAM_CYCLE_NUMBER_OF_VERTICES_PANEL"));
	hamCycleNumberOfVerticesBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	hamCycleNumberOfVerticesStaticText = new wxStaticText (hamCycleNumberOfVerticesPanel, ID_HAM_CYCLE_NUMBER_OF_VERTICES_STATICTEXT, _ ("Number of vertices"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_HAM_CYCLE_NUMBER_OF_VERTICES_STATICTEXT"));
	hamCycleNumberOfVerticesBoxSizer->Add (hamCycleNumberOfVerticesStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleNumberOfVerticesTextCtrl = new wxTextCtrl (hamCycleNumberOfVerticesPanel, ID_HAM_CYCLE_NUMBER_OF_VERTICES_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_HAM_CYCLE_NUMBER_OF_VERTICES_TEXTCTRL"));
	hamCycleNumberOfVerticesBoxSizer->Add (hamCycleNumberOfVerticesTextCtrl, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleNumberOfVerticesPanel->SetSizer (hamCycleNumberOfVerticesBoxSizer);
	hamCycleNumberOfVerticesBoxSizer->Fit (hamCycleNumberOfVerticesPanel);
	hamCycleNumberOfVerticesBoxSizer->SetSizeHints (hamCycleNumberOfVerticesPanel);
	HamCycleBoxSizer->Add (hamCycleNumberOfVerticesPanel, 1, wxALL|wxEXPAND, 5);
	hamCycleNumberOfEdgesPanel = new wxPanel (this, ID_HAM_CYCLE_NUMBER_OF_EDGES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_HAM_CYCLE_NUMBER_OF_EDGES_PANEL"));
	hamCycleNumberOfEdgesBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	hamCycleNumberOfEdgesStaticText = new wxStaticText (hamCycleNumberOfEdgesPanel, ID_HAM_CYCLE_NUMBER_OF_EDGES_STATICTEXT, _ ("Number of edges"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_HAM_CYCLE_NUMBER_OF_EDGES_STATICTEXT"));
	hamCycleNumberOfEdgesBoxSizer->Add (hamCycleNumberOfEdgesStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleNumberOfEdgesTextCtrl = new wxTextCtrl (hamCycleNumberOfEdgesPanel, ID_HAM_CYCLE_NUMBER_OF_EDGES_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_HAM_CYCLE_NUMBER_OF_EDGES_TEXTCTRL"));
	hamCycleNumberOfEdgesBoxSizer->Add (hamCycleNumberOfEdgesTextCtrl, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleNumberOfEdgesPanel->SetSizer (hamCycleNumberOfEdgesBoxSizer);
	hamCycleNumberOfEdgesBoxSizer->Fit (hamCycleNumberOfEdgesPanel);
	hamCycleNumberOfEdgesBoxSizer->SetSizeHints (hamCycleNumberOfEdgesPanel);
	HamCycleBoxSizer->Add (hamCycleNumberOfEdgesPanel, 1, wxALL|wxEXPAND, 5);
	hamCycleButtonPanel = new wxPanel (this, ID_HAM_CYCLE_BUTTON_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_HAM_CYCLE_BUTTON_PANEL"));
	hamCycleButtonBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	hamCycleButtonBoxSizer->Add (-1,-1,1, wxALL|wxEXPAND, 5);
	hamCycleBuildButton = new wxButton (hamCycleButtonPanel, ID_HAM_CYCLE_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_HAM_CYCLE_BUILD_BUTTON"));
	hamCycleBuildButton->SetDefault();
	hamCycleButtonBoxSizer->Add (hamCycleBuildButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleCancelButton = new wxButton (hamCycleButtonPanel, ID_HAM_CYCLE_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_HAM_CYCLE_CANCEL_BUTTON"));
	hamCycleButtonBoxSizer->Add (hamCycleCancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	hamCycleButtonPanel->SetSizer (hamCycleButtonBoxSizer);
	hamCycleButtonBoxSizer->Fit (hamCycleButtonPanel);
	hamCycleButtonBoxSizer->SetSizeHints (hamCycleButtonPanel);
	HamCycleBoxSizer->Add (hamCycleButtonPanel, 1, wxALL|wxEXPAND, 5);
	SetSizer (HamCycleBoxSizer);
	HamCycleBoxSizer->Fit (this);
	HamCycleBoxSizer->SetSizeHints (this);
	//*)
	Connect (ID_HAM_CYCLE_BUILD_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &HamCycle::OnhamCycleBuildButtonClick);
	Connect (ID_HAM_CYCLE_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &HamCycle::OnhamCycleCancelButtonClick);
}

HamCycle::~HamCycle()
{
	//(*Destroy(HamCycle)
	//*)
}


void HamCycle::OnhamCycleBuildButtonClick (wxCommandEvent& event)
{
	wxString nStr,mStr;
	long longn, longm;

	nStr = hamCycleNumberOfVerticesTextCtrl->GetValue();

	if (!nStr.ToLong (&longn) || nStr.length() == 0)
	{
		reportError (wxT ("The number of vertices is not a valid number."),"Note");
		return;
	}

	mStr = hamCycleNumberOfEdgesTextCtrl->GetValue();
	mStr.ToLong (&longm);

	if (!mStr.ToLong (&longm) || mStr.length() == 0)
	{
		reportError (wxT ("The number of edges is not a valid number."),"Note");
		return;
	}

	*pnumberOfVertices = (int) longn;
	*pnumberOfEdges = (int) longm;

	EndModal (wxID_OK);
}

void HamCycle::OnhamCycleCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
