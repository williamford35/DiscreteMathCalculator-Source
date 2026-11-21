/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/bfs/BfsSearch.h"
#include "common/ImageFiles.h"
#include "common/report.h"

//(*InternalHeadersPCH(BfsSearch)
#include <wx/string.h>
#include <wx/intl.h>
//*)
//(*InternalHeaders(BfsSearch)
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(BfsSearch)
const long BfsSearch::ID_BFS_S_RADIOBUTTON = wxNewId();
const long BfsSearch::ID_BFS_S_TEXTCTRL = wxNewId();
const long BfsSearch::ID_BFS_S_PANEL = wxNewId();
const long BfsSearch::ID_BFS_S_T_RADIOBUTTON = wxNewId();
const long BfsSearch::ID_BFS_S_2_TEXTCTRL = wxNewId();
const long BfsSearch::ID_BFS_T_TEXTCTRL = wxNewId();
const long BfsSearch::ID_BFS_S_T_TEXT_PANEL = wxNewId();
const long BfsSearch::ID_BFS_S_T_PANEL = wxNewId();
const long BfsSearch::ID_BUILD_BFS_PANEL = wxNewId();
const long BfsSearch::ID_BFS_STATICBITMAP = wxNewId();
const long BfsSearch::ID_BFS_RUN_BUTTON = wxNewId();
const long BfsSearch::ID_BFS_CANCEL_BUTTON = wxNewId();
const long BfsSearch::ID_BFS_RUN_CANCEL_PANEL = wxNewId();
const long BfsSearch::ID_BFS_CONTROL_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (BfsSearch,wxDialog)
	//(*EventTable(BfsSearch)
	//*)
END_EVENT_TABLE()

BfsSearch::BfsSearch (wxWindow* parent, wxString* v, GraphType type,
                      wxWindowID id, const wxPoint& pos, const wxSize& size) :
	vertex (v), graphType (type)
{
	//(*Initialize(BfsSearch)
	wxFlexGridSizer* bfs_s_t_FlexGridSizer;
	wxFlexGridSizer* buildBFSFlexGridSizer;
	wxBoxSizer* bfs_s_t_text_BoxSizer;
	wxFlexGridSizer* bfsRunCancelFlexGridSizer;
	wxFlexGridSizer* bfsFlexGridSizer;
	wxFlexGridSizer* bfsControlPanelFlexGridSizer;
	wxFlexGridSizer* bfs_s_FlexGridSizer;
	wxBoxSizer* bfsRunCancelBoxSizer;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	bfsFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	bfs_s_Panel = new wxPanel (this, ID_BFS_S_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BFS_S_PANEL"));
	bfs_s_FlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	bfs_s_FlexGridSizer->AddGrowableCol (1);
	bfs_s_RadioButton = new wxRadioButton (bfs_s_Panel, ID_BFS_S_RADIOBUTTON, _ ("BFS() Start"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE, wxDefaultValidator, _T ("ID_BFS_S_RADIOBUTTON"));
	bfs_s_FlexGridSizer->Add (bfs_s_RadioButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	bfs_s_TextCtrl = new wxTextCtrl (bfs_s_Panel, ID_BFS_S_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BFS_S_TEXTCTRL"));
	bfs_s_FlexGridSizer->Add (bfs_s_TextCtrl, 1, wxALL|wxEXPAND, 9);
	bfs_s_Panel->SetSizer (bfs_s_FlexGridSizer);
	bfs_s_FlexGridSizer->Fit (bfs_s_Panel);
	bfs_s_FlexGridSizer->SetSizeHints (bfs_s_Panel);
	bfsFlexGridSizer->Add (bfs_s_Panel, 1, wxALL|wxEXPAND, 1);
#if !defined(__LINUX__) && !defined(__APPLE__) && !defined(_WIN32)
	bfs_s_t_Panel = new wxPanel (this, ID_BFS_S_T_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BFS_S_T_PANEL"));
	bfs_s_t_FlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	bfs_s_t_FlexGridSizer->AddGrowableCol (1);
	bfs_s_t_RadioButton = new wxRadioButton (bfs_s_t_Panel, ID_BFS_S_T_RADIOBUTTON, _ ("BFS(s,t)"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE, wxDefaultValidator, _T ("ID_BFS_S_T_RADIOBUTTON"));
	bfs_s_t_FlexGridSizer->Add (bfs_s_t_RadioButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	bfs_s_t_Text_Panel = new wxPanel (bfs_s_t_Panel, ID_BFS_S_T_TEXT_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BFS_S_T_TEXT_PANEL"));
	bfs_s_t_text_BoxSizer = new wxBoxSizer (wxVERTICAL);
	bfs_s_2_TextCtrl = new wxTextCtrl (bfs_s_t_Text_Panel, ID_BFS_S_2_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BFS_S_2_TEXTCTRL"));
	bfs_s_t_text_BoxSizer->Add (bfs_s_2_TextCtrl, 1, wxALL|wxEXPAND, 1);
	bfs_t_TextCtrl = new wxTextCtrl (bfs_s_t_Text_Panel, ID_BFS_T_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BFS_T_TEXTCTRL"));
	bfs_s_t_text_BoxSizer->Add (bfs_t_TextCtrl, 1, wxALL|wxEXPAND, 1);
	bfs_s_t_Text_Panel->SetSizer (bfs_s_t_text_BoxSizer);
	bfs_s_t_text_BoxSizer->Fit (bfs_s_t_Text_Panel);
	bfs_s_t_text_BoxSizer->SetSizeHints (bfs_s_t_Text_Panel);
	bfs_s_t_FlexGridSizer->Add (bfs_s_t_Text_Panel, 1, wxALL|wxEXPAND, 1);
	bfs_s_t_Panel->SetSizer (bfs_s_t_FlexGridSizer);
	bfs_s_t_FlexGridSizer->Fit (bfs_s_t_Panel);
	bfs_s_t_FlexGridSizer->SetSizeHints (bfs_s_t_Panel);
	bfsFlexGridSizer->Add (bfs_s_t_Panel, 1, wxALL|wxEXPAND, 1);
#endif
	buildBFSPanel = new wxPanel (this, ID_BUILD_BFS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BUILD_BFS_PANEL"));
	buildBFSFlexGridSizer = new wxFlexGridSizer (0, 1, 0, 0);
	buildBFSFlexGridSizer->AddGrowableCol (0);
	buildBFSFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 1);
	buildBFSPanel->SetSizer (buildBFSFlexGridSizer);
	buildBFSFlexGridSizer->Fit (buildBFSPanel);
	buildBFSFlexGridSizer->SetSizeHints (buildBFSPanel);
	bfsFlexGridSizer->Add (buildBFSPanel, 1, wxALL|wxEXPAND, 1);
	bfsControlPanel = new wxPanel (this, ID_BFS_CONTROL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BFS_CONTROL_PANEL"));
	bfsControlPanelFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);

	if (graphType == undir || graphType == undirw)
		bfsStaticBitmap = new wxStaticBitmap (bfsControlPanel, ID_BFS_STATICBITMAP, wxBitmap (wxImage (imagefile ("bfs-graph.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_BFS_STATICBITMAP"));
	else
		bfsStaticBitmap = new wxStaticBitmap (bfsControlPanel, ID_BFS_STATICBITMAP, wxBitmap (wxImage (imagefile ("bfs-digraph.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_BFS_STATICBITMAP"));

	bfsControlPanelFlexGridSizer->Add (bfsStaticBitmap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	brsRunCancelPanel = new wxPanel (bfsControlPanel, ID_BFS_RUN_CANCEL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BFS_RUN_CANCEL_PANEL"));
	bfsRunCancelFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	bfsRunCancelFlexGridSizer->AddGrowableRow (0);
	bfsRunCancelFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 1);
	bfsRunCancelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	bfsRunButton = new wxButton (brsRunCancelPanel, ID_BFS_RUN_BUTTON, _ ("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BFS_RUN_BUTTON"));
	bfsRunButton->SetDefault();
	bfsRunCancelBoxSizer->Add (bfsRunButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	bfsCancelButton = new wxButton (brsRunCancelPanel, ID_BFS_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BFS_CANCEL_BUTTON"));
	bfsRunCancelBoxSizer->Add (bfsCancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	bfsRunCancelFlexGridSizer->Add (bfsRunCancelBoxSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	brsRunCancelPanel->SetSizer (bfsRunCancelFlexGridSizer);
	bfsRunCancelFlexGridSizer->Fit (brsRunCancelPanel);
	bfsRunCancelFlexGridSizer->SetSizeHints (brsRunCancelPanel);
	bfsControlPanelFlexGridSizer->Add (brsRunCancelPanel, 1, wxALL|wxEXPAND, 1);
	bfsControlPanel->SetSizer (bfsControlPanelFlexGridSizer);
	bfsControlPanelFlexGridSizer->Fit (bfsControlPanel);
	bfsControlPanelFlexGridSizer->SetSizeHints (bfsControlPanel);
	bfsFlexGridSizer->Add (bfsControlPanel, 1, wxALL|wxEXPAND, 1);
	SetSizer (bfsFlexGridSizer);
	bfsFlexGridSizer->Fit (this);
	bfsFlexGridSizer->SetSizeHints (this);

	Connect (ID_BFS_S_RADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED, (wxObjectEventFunction) &BfsSearch::Onbfs_s_RadioButtonSelect);
	Connect (ID_BFS_S_T_RADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED, (wxObjectEventFunction) &BfsSearch::Onbfs_s_t_RadioButtonSelect);
	Connect (ID_BFS_RUN_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &BfsSearch::OnbfsRunButtonClick);
	Connect (ID_BFS_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &BfsSearch::OnbfsCancelButtonClick);
	//*)

	bfs_s_RadioButton->SetValue (true);
	//bfs_s_TextCtrl->Enable (true);
	/*bfs_s_t_RadioButton->SetValue (false);
	bfs_s_2_TextCtrl->Enable (false);
	bfs_t_TextCtrl->Enable (false);*/
}

BfsSearch::~BfsSearch()
{
	//(*Destroy(BfsSearch)
	//*)
}

void BfsSearch::OnbfsRunButtonClick (wxCommandEvent& event)
{
	wxString s, t;

	if (bfs_s_RadioButton->GetValue())
	{
		s = bfs_s_TextCtrl->GetValue();
		vertex[0] = s;
		vertex[1] = wxT ("");
	}
	else
	{
		s = bfs_s_2_TextCtrl->GetValue();
		t = bfs_t_TextCtrl->GetValue();

		if (s.length() == 0 || t.length() == 0)
		{
			report (wxT ("Either s or t has no value."),"Note");
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

void BfsSearch::OnbfsCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void BfsSearch::Onbfs_s_RadioButtonSelect (wxCommandEvent& event)
{
	bfs_s_RadioButton->SetValue(true);
	bfs_s_RadioButton->SetValue(false);

	// bfs_s_t_RadioButton->SetValue(true);
	// bfs_s_RadioButton->SetValue (true);
	// bfs_s_t_RadioButton->SetValue (false);
	// bfs_s_TextCtrl->Enable (true);
	// bfs_s_2_TextCtrl->Enable (false);
	// bfs_t_TextCtrl->Enable (false);
}

void BfsSearch::Onbfs_s_t_RadioButtonSelect (wxCommandEvent& event)
{
	bfs_s_t_RadioButton->SetValue(true);
	bfs_s_RadioButton->SetValue(false);

	//bfs_s_RadioButton->SetValue(true);
	/*bfs_s_RadioButton->SetValue (false);
	bfs_s_2_TextCtrl->Enable (true);
	bfs_t_TextCtrl->Enable (true);
	bfs_s_TextCtrl->Enable (false);*/
}
