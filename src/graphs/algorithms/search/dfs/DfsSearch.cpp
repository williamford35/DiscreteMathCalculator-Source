/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/dfs/DfsSearch.h"
#include "common/ImageFiles.h"
#include "common/report.h"

//(*InternalHeadersPCH(DfsSearch)
#include <wx/string.h>
#include <wx/intl.h>
//*)
//(*InternalHeaders(DfsSearch)
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

#include <wx/msgdlg.h>

//(*IdInit(DfsSearch)
const long DfsSearch::ID_DFS_SEARCH_DFS_RADIO_RADIOBUTTON = wxNewId();
const long DfsSearch::ID_DFS_START_TEXTCTRL = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_DFS_PANEL = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_DFS_VISIT_RADIOBUTTON = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_DFS_ENTER_S_TEXTCTRL = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_DFS_ENTER_T_TEXTCTRL = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_ENTER_S_T_PANEL = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_DFS_VISIT_PANEL = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_STATICBITMAP = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_RUN_BUTTON = wxNewId();
const long DfsSearch::ID_DFS_VISIT_CANCEL_BUTTON = wxNewId();
const long DfsSearch::ID_DFS_SEARCH_CONTROL_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (DfsSearch,wxDialog)
	//(*EventTable(DfsSearch)
	//*)
END_EVENT_TABLE()

DfsSearch::DfsSearch (wxWindow* parent,wxString *v, GraphType type,
                      wxWindowID id,const wxPoint& pos,const wxSize& size) :
	vertex (v), graphType (type)
{
	//(*Initialize(DfsSearch)
	wxFlexGridSizer* dfsSearchFlexGridSizer;
	wxFlexGridSizer* dfsSearchDfsVisitFlexGridSizer;
	wxBoxSizer* dfsSearchDfsEnter_s_t_BoxSizer;
	wxFlexGridSizer* dfsSearchControlPanelFlexGridSizer;
	wxFlexGridSizer* dfsSearchDfsFlexGridSizer;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	dfsSearchFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	dfsSearchFlexGridSizer->AddGrowableCol (0);
	dfsSearchFlexGridSizer->AddGrowableRow (0);
	dfsSearchFlexGridSizer->AddGrowableRow (1);
	dfsSearchFlexGridSizer->AddGrowableRow (2);
	dfsSearchFlexGridSizer->AddGrowableRow (3);
	dfsSearchDfsPanel = new wxPanel (this, ID_DFS_SEARCH_DFS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DFS_SEARCH_DFS_PANEL"));
	dfsSearchDfsFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	dfsSearchDfsFlexGridSizer->AddGrowableCol (2);
	dfsSearchDfsRadioButton = new wxRadioButton (dfsSearchDfsPanel, ID_DFS_SEARCH_DFS_RADIO_RADIOBUTTON, _ ("Dfs() Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_SEARCH_DFS_RADIO_RADIOBUTTON"));
	dfsSearchDfsRadioButton->SetValue (true);
	dfsSearchDfsFlexGridSizer->Add (dfsSearchDfsRadioButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 10);
	dfsStartTextCtrl = new wxTextCtrl (dfsSearchDfsPanel, ID_DFS_START_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_START_TEXTCTRL"));
	dfsSearchDfsFlexGridSizer->Add (dfsStartTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	dfsSearchDfsFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 5);
	dfsSearchDfsPanel->SetSizer (dfsSearchDfsFlexGridSizer);
	dfsSearchDfsFlexGridSizer->Fit (dfsSearchDfsPanel);
	dfsSearchDfsFlexGridSizer->SetSizeHints (dfsSearchDfsPanel);
	dfsSearchFlexGridSizer->Add (dfsSearchDfsPanel, 1, wxALL|wxEXPAND, 1);
	dfsSearchDfsVisitPanel = new wxPanel (this, ID_DFS_SEARCH_DFS_VISIT_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DFS_SEARCH_DFS_VISIT_PANEL"));
	dfsSearchDfsVisitFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	dfsSearchDfsVisitFlexGridSizer->AddGrowableCol (1);
#if !defined(__LINUX__) && !defined(__APPLE__) && !defined(_WIN32)
	dfsSearchDfsVisitRadioButton = new wxRadioButton (dfsSearchDfsVisitPanel, ID_DFS_SEARCH_DFS_VISIT_RADIOBUTTON, _ ("Dfsvisit(s,t)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_SEARCH_DFS_VISIT_RADIOBUTTON"));
	dfsSearchDfsVisitFlexGridSizer->Add (dfsSearchDfsVisitRadioButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	dfsSearchEnter_s_t_Panel = new wxPanel (dfsSearchDfsVisitPanel, ID_DFS_SEARCH_ENTER_S_T_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DFS_SEARCH_ENTER_S_T_PANEL"));
	dfsSearchDfsEnter_s_t_BoxSizer = new wxBoxSizer (wxVERTICAL);
	dfsSearchDfsEnter_s_TextCtrl = new wxTextCtrl (dfsSearchEnter_s_t_Panel, ID_DFS_SEARCH_DFS_ENTER_S_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_SEARCH_DFS_ENTER_S_TEXTCTRL"));
	dfsSearchDfsEnter_s_t_BoxSizer->Add (dfsSearchDfsEnter_s_TextCtrl, 1, wxALL|wxEXPAND, 1);
	dfsSearchDfsEnter_t_TextCtrl = new wxTextCtrl (dfsSearchEnter_s_t_Panel, ID_DFS_SEARCH_DFS_ENTER_T_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_SEARCH_DFS_ENTER_T_TEXTCTRL"));
	dfsSearchDfsEnter_s_t_BoxSizer->Add (dfsSearchDfsEnter_t_TextCtrl, 1, wxALL|wxEXPAND, 1);
	dfsSearchEnter_s_t_Panel->SetSizer (dfsSearchDfsEnter_s_t_BoxSizer);
	dfsSearchDfsEnter_s_t_BoxSizer->Fit (dfsSearchEnter_s_t_Panel);
	dfsSearchDfsEnter_s_t_BoxSizer->SetSizeHints (dfsSearchEnter_s_t_Panel);
	dfsSearchDfsVisitFlexGridSizer->Add (dfsSearchEnter_s_t_Panel, 1, wxALL|wxEXPAND, 1);
	dfsSearchDfsVisitPanel->SetSizer (dfsSearchDfsVisitFlexGridSizer);
#endif
	dfsSearchDfsVisitFlexGridSizer->Fit (dfsSearchDfsVisitPanel);
	dfsSearchDfsVisitFlexGridSizer->SetSizeHints (dfsSearchDfsVisitPanel);
	dfsSearchFlexGridSizer->Add (dfsSearchDfsVisitPanel, 1, wxALL|wxEXPAND, 1);
	dfsSearchControlPanel = new wxPanel (this, ID_DFS_SEARCH_CONTROL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DFS_SEARCH_CONTROL_PANEL"));
	dfsSearchControlPanelFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);

	if ( (graphType == digraph) || (graphType == digraphw))
		dfsSearchStaticBitmap = new wxStaticBitmap (dfsSearchControlPanel, ID_DFS_SEARCH_STATICBITMAP, wxBitmap (wxImage (imagefile ("dfs-digraph.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_DFS_SEARCH_STATICBITMAP"));
	else
		dfsSearchStaticBitmap = new wxStaticBitmap (dfsSearchControlPanel, ID_DFS_SEARCH_STATICBITMAP, wxBitmap (wxImage (imagefile ("dfs-graph.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_DFS_SEARCH_STATICBITMAP"));

	dfsSearchControlPanelFlexGridSizer->Add (dfsSearchStaticBitmap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	dfsSearchRunButton = new wxButton (dfsSearchControlPanel, ID_DFS_SEARCH_RUN_BUTTON, _ ("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_SEARCH_RUN_BUTTON"));
	dfsSearchControlPanelFlexGridSizer->Add (dfsSearchRunButton, 1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	dfsVisitCancelButton = new wxButton (dfsSearchControlPanel, ID_DFS_VISIT_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DFS_VISIT_CANCEL_BUTTON"));
	dfsSearchControlPanelFlexGridSizer->Add (dfsVisitCancelButton, 1, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	dfsSearchControlPanel->SetSizer (dfsSearchControlPanelFlexGridSizer);
	dfsSearchControlPanelFlexGridSizer->Fit (dfsSearchControlPanel);
	dfsSearchControlPanelFlexGridSizer->SetSizeHints (dfsSearchControlPanel);
	dfsSearchFlexGridSizer->Add (dfsSearchControlPanel, 1, wxALL|wxEXPAND, 1);
	SetSizer (dfsSearchFlexGridSizer);
	dfsSearchFlexGridSizer->Fit (this);
	dfsSearchFlexGridSizer->SetSizeHints (this);

	Connect (ID_DFS_SEARCH_DFS_RADIO_RADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED, (wxObjectEventFunction) &DfsSearch::OndfsSearchDfsRadioButtonSelect);
	Connect (ID_DFS_SEARCH_DFS_VISIT_RADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED, (wxObjectEventFunction) &DfsSearch::OndfsSearchDfsVisitRadioButtonSelect);
	Connect (ID_DFS_SEARCH_RUN_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &DfsSearch::OndfsSearchRunButtonClick);
	Connect (ID_DFS_VISIT_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &DfsSearch::OndfsVisitCancelButtonClick);
	//*)
}

DfsSearch::~DfsSearch()
{
	//(*Destroy(DfsSearch)
	//*)
}


void DfsSearch::OndfsSearchRunButtonClick (wxCommandEvent& event)
{
	wxString s, t;

	if (dfsSearchDfsRadioButton->GetValue())
	{
		s = dfsStartTextCtrl->GetValue();
		vertex[0] = s;
		vertex[1] = wxT ("");
	}
	else
	{
		s = dfsSearchDfsEnter_s_TextCtrl->GetValue();
		t = dfsSearchDfsEnter_t_TextCtrl->GetValue();

		if (s.length() == 0 || t.length() == 0)
		{
			::report(wxT ("Either s or t has no value."),"Note");
			return;
		}
		else
		{
			vertex[0] = s;
			vertex[1] = t;
		}
	}

	EndModal (wxID_OK);
}

void DfsSearch::OndfsVisitCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void DfsSearch::OndfsSearchDfsRadioButtonSelect (wxCommandEvent& event)
{
	dfsSearchDfsVisitRadioButton->SetValue (false);
	dfsSearchDfsEnter_s_TextCtrl->Enable (false);
	dfsSearchDfsEnter_t_TextCtrl->Enable (false);
}

void DfsSearch::OndfsSearchDfsVisitRadioButtonSelect (wxCommandEvent& event)
{
	dfsSearchDfsRadioButton->SetValue (false);
	dfsSearchDfsEnter_s_TextCtrl->Enable (true);
	dfsSearchDfsEnter_t_TextCtrl->Enable (true);
}
