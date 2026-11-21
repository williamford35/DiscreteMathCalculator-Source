/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/LongestPath/LongestPathInput.h"
#include "common/ImageFiles.h"
#include "common/report.h"

//(*InternalHeaders(LongestPathInput)
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(LongestPathInput)
const long LongestPathInput::ID_LONGESTPATH_STATICBITMAP = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_STARTING_VERTEX_STATICTEXT = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_STARTING_VERTEX_TEXTCTRL = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_STARTING_VERTEX_PANEL = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_STARGING_VERTEX_STATICTEXT = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_FINAL_VERTEX_TEXTCTRL = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_FINAL_VERTEX_PANEL = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_VERTICES_PANEL = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_EXECUTE_BUTTON = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_CANCEL_BUTTON = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_BUTTONS_PANEL = wxNewId();
const long LongestPathInput::ID_LONGESTPATH_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (LongestPathInput,wxDialog)
	//(*EventTable(LongestPathInput)
	//*)
END_EVENT_TABLE()

LongestPathInput::LongestPathInput (wxWindow* parent, wxString *s, wxString *t, wxWindowID id) :
	startingVertex (s), finalVertex (t)
{
	//(*Initialize(LongestPathInput)
	wxBoxSizer* LongestPathStartingVertexBoxSizer;
	wxBoxSizer* LongestPathFinalVertexBoxSizer;
	wxBoxSizer* LongestPathVerticesBoxSizer;
	wxBoxSizer* LongestPathButtonsBoxSizer;
	wxFlexGridSizer* LongestPathFlexGridSizer;

	Create (parent, wxID_ANY, _ ("LongestPath Algorithm"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T ("wxID_ANY"));
	SetClientSize (wxSize (350,200));
	LongestPathPanel = new wxPanel (this, ID_LONGESTPATH_PANEL, wxPoint (96,192), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_LONGESTPATH_PANEL"));
	LongestPathFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	LongestPathStaticBitmap = new wxStaticBitmap (LongestPathPanel, ID_LONGESTPATH_STATICBITMAP, wxBitmap (wxImage (imagefile ("longestpath-dialog.png"))), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_LONGESTPATH_STATICBITMAP"));
	LongestPathFlexGridSizer->Add (LongestPathStaticBitmap, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	LongestPathVerticesPanel = new wxPanel (LongestPathPanel, ID_LONGESTPATH_VERTICES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_LONGESTPATH_VERTICES_PANEL"));
	LongestPathVerticesBoxSizer = new wxBoxSizer (wxVERTICAL);
	LongestPathStartingVertexPanel = new wxPanel (LongestPathVerticesPanel, ID_LONGESTPATH_STARTING_VERTEX_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_LONGESTPATH_STARTING_VERTEX_PANEL"));
	LongestPathStartingVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	LongestPathStartingVertexStaticText = new wxStaticText (LongestPathStartingVertexPanel, ID_LONGESTPATH_STARTING_VERTEX_STATICTEXT, _ ("Starting vertex"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_LONGESTPATH_STARTING_VERTEX_STATICTEXT"));
	LongestPathStartingVertexBoxSizer->Add (LongestPathStartingVertexStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LongestPathStartingVertexTextCtrl = new wxTextCtrl (LongestPathStartingVertexPanel, ID_LONGESTPATH_STARTING_VERTEX_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_LONGESTPATH_STARTING_VERTEX_TEXTCTRL"));
	LongestPathStartingVertexBoxSizer->Add (LongestPathStartingVertexTextCtrl, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LongestPathStartingVertexPanel->SetSizer (LongestPathStartingVertexBoxSizer);
	LongestPathStartingVertexBoxSizer->Fit (LongestPathStartingVertexPanel);
	LongestPathStartingVertexBoxSizer->SetSizeHints (LongestPathStartingVertexPanel);
	LongestPathVerticesBoxSizer->Add (LongestPathStartingVertexPanel, 1, wxALL|wxEXPAND, 1);
	LongestPathFinalVertexPanel = new wxPanel (LongestPathVerticesPanel, ID_LONGESTPATH_FINAL_VERTEX_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_LONGESTPATH_FINAL_VERTEX_PANEL"));
	LongestPathFinalVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	LongestPathFinalVertexStaticText2 = new wxStaticText (LongestPathFinalVertexPanel, ID_LONGESTPATH_STARGING_VERTEX_STATICTEXT, _ ("Final vertex"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_LONGESTPATH_STARGING_VERTEX_STATICTEXT"));
	LongestPathFinalVertexBoxSizer->Add (LongestPathFinalVertexStaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LongestPathFinalVertexTextCtrl = new wxTextCtrl (LongestPathFinalVertexPanel, ID_LONGESTPATH_FINAL_VERTEX_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_LONGESTPATH_FINAL_VERTEX_TEXTCTRL"));
	LongestPathFinalVertexBoxSizer->Add (LongestPathFinalVertexTextCtrl, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LongestPathFinalVertexPanel->SetSizer (LongestPathFinalVertexBoxSizer);
	LongestPathFinalVertexBoxSizer->Fit (LongestPathFinalVertexPanel);
	LongestPathFinalVertexBoxSizer->SetSizeHints (LongestPathFinalVertexPanel);
	LongestPathVerticesBoxSizer->Add (LongestPathFinalVertexPanel, 1, wxALL|wxEXPAND, 1);
	LongestPathVerticesPanel->SetSizer (LongestPathVerticesBoxSizer);
	LongestPathVerticesBoxSizer->Fit (LongestPathVerticesPanel);
	LongestPathVerticesBoxSizer->SetSizeHints (LongestPathVerticesPanel);
	LongestPathFlexGridSizer->Add (LongestPathVerticesPanel, 1, wxALL|wxEXPAND, 1);
	LongestPathButtonsPanel = new wxPanel (LongestPathPanel, ID_LONGESTPATH_BUTTONS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_LONGESTPATH_BUTTONS_PANEL"));
	LongestPathButtonsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	LongestPathButtonsBoxSizer->Add (-1,-1,2, wxALL|wxEXPAND, 1);
	LongestPathExecuteButton = new wxButton (LongestPathButtonsPanel, ID_LONGESTPATH_EXECUTE_BUTTON, _ ("Execute"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_LONGESTPATH_EXECUTE_BUTTON"));
	LongestPathButtonsBoxSizer->Add (LongestPathExecuteButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LongestPathCancelButton = new wxButton (LongestPathButtonsPanel, ID_LONGESTPATH_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_LONGESTPATH_CANCEL_BUTTON"));
	LongestPathButtonsBoxSizer->Add (LongestPathCancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	LongestPathButtonsPanel->SetSizer (LongestPathButtonsBoxSizer);
	LongestPathButtonsBoxSizer->Fit (LongestPathButtonsPanel);
	LongestPathButtonsBoxSizer->SetSizeHints (LongestPathButtonsPanel);
	LongestPathFlexGridSizer->Add (LongestPathButtonsPanel, 1, wxALL|wxEXPAND, 1);
	LongestPathPanel->SetSizer (LongestPathFlexGridSizer);
	LongestPathFlexGridSizer->Fit (LongestPathPanel);
	LongestPathFlexGridSizer->SetSizeHints (LongestPathPanel);

	Connect (ID_LONGESTPATH_EXECUTE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &LongestPathInput::OnLongestPathExecuteButtonClick);
	Connect (ID_LONGESTPATH_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &LongestPathInput::OnLongestPathCancelButtonClick);
	//*)

	Centre();
}

LongestPathInput::~LongestPathInput()
{
	//(*Destroy(LongestPathInput)
	//*)
}


void LongestPathInput::OnLongestPathExecuteButtonClick (wxCommandEvent& event)
{
	wxString s, t;

	s = LongestPathStartingVertexTextCtrl->GetValue();
	t = LongestPathFinalVertexTextCtrl->GetValue();

	if (s.length() == 0 || t.length() == 0)
	{
		reportError (wxT ("Either the starting or the ending vertex has no value."),wxT(""));
		return;
	}
	else
	{
		*startingVertex = s;
		*finalVertex = t;
	}

	EndModal (wxID_OK);
}

void LongestPathInput::OnLongestPathCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
