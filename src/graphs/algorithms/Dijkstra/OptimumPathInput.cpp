/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/Dijkstra/OptimumPathInput.h"
#include "common/ImageFiles.h"
#include "common/report.h"

//(*InternalHeaders(OptimumPathInput)
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(OptimumPathInput)
const long OptimumPathInput::ID_DIJKSTRA_STATICBITMAP = wxNewId();
const long OptimumPathInput::ID_WIDESTPATH_STATICBITMAP = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_STARTING_VERTEX_STATICTEXT = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_STARTING_VERTEX_TEXTCTRL = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_STARTING_VERTEX_PANEL = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_STARGING_VERTEX_STATICTEXT = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_FINAL_VERTEX_TEXTCTRL = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_FINAL_VERTEX_PANEL = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_VERTICES_PANEL = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_EXECUTE_BUTTON = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_CANCEL_BUTTON = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_BUTTONS_PANEL = wxNewId();
const long OptimumPathInput::ID_DIJKSTRA_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (OptimumPathInput,wxDialog)
	//(*EventTable(OptimumPathInput)
	//*)
END_EVENT_TABLE()

OptimumPathInput::OptimumPathInput (wxWindow* parent, wxString *s, wxString *t,
                                    bool isDijkstra, GraphType graphType, wxWindowID id) :
	startingVertex (s), finalVertex (t), grphType(graphType)
{
	//(*Initialize(OptimumPathInput)
	wxBoxSizer* DijkstraStartingVertexBoxSizer;
	wxBoxSizer* DijkstraFinalVertexBoxSizer;
	wxBoxSizer* DijkstraVerticesBoxSizer;
	wxBoxSizer* DijkstraButtonsBoxSizer;
	wxFlexGridSizer* DijkstraFlexGridSizer;

	if (isDijkstra)
	{
		Create (parent, wxID_ANY, _ ("Dijkstra\'s Algorithm"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T ("wxID_ANY"));
		SetClientSize (wxSize (400,475));
	}
	else
	{
		Create (parent, wxID_ANY, _ ("Widest Path"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T ("wxID_ANY"));
		SetClientSize(wxSize(400,370));
	}

	DijkstraPanel = new wxPanel (this, ID_DIJKSTRA_PANEL, wxPoint (96,192), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DIJKSTRA_PANEL"));
	DijkstraFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);

	if (isDijkstra)
	{
		if (grphType == undirw)
			DijkstraStaticBitmap = new wxStaticBitmap (DijkstraPanel, ID_DIJKSTRA_STATICBITMAP,
			      wxBitmap (wxImage (imagefile ("Dijkstra-undir.png"))), wxDefaultPosition,
			      wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_DIJKSTRA_STATICBITMAP"));
		else
			DijkstraStaticBitmap = new wxStaticBitmap (DijkstraPanel, ID_DIJKSTRA_STATICBITMAP,
			      wxBitmap (wxImage (imagefile ("Dijkstra-digraph.png"))), wxDefaultPosition,
			      wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_DIJKSTRA_STATICBITMAP"));
		DijkstraFlexGridSizer->Add (DijkstraStaticBitmap, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	else
	{
		WidestPathStaticBitmap = new wxStaticBitmap (DijkstraPanel, ID_WIDESTPATH_STATICBITMAP, wxBitmap (wxImage (imagefile ("widest-path.png"))), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_WIDESTPATH_STATICBITMAP"));
		DijkstraFlexGridSizer->Add (WidestPathStaticBitmap, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}

	DijkstraVerticesPanel = new wxPanel (DijkstraPanel, ID_DIJKSTRA_VERTICES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DIJKSTRA_VERTICES_PANEL"));
	DijkstraVerticesBoxSizer = new wxBoxSizer (wxVERTICAL);
	DijkstraStartingVertexPanel = new wxPanel (DijkstraVerticesPanel, ID_DIJKSTRA_STARTING_VERTEX_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DIJKSTRA_STARTING_VERTEX_PANEL"));
	DijkstraStartingVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DijkstraStartingVertexStaticText = new wxStaticText (DijkstraStartingVertexPanel, ID_DIJKSTRA_STARTING_VERTEX_STATICTEXT, _ ("Starting vertex"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_DIJKSTRA_STARTING_VERTEX_STATICTEXT"));
	DijkstraStartingVertexBoxSizer->Add (DijkstraStartingVertexStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DijkstraStartingVertexTextCtrl = new wxTextCtrl (DijkstraStartingVertexPanel, ID_DIJKSTRA_STARTING_VERTEX_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DIJKSTRA_STARTING_VERTEX_TEXTCTRL"));
	DijkstraStartingVertexBoxSizer->Add (DijkstraStartingVertexTextCtrl, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DijkstraStartingVertexPanel->SetSizer (DijkstraStartingVertexBoxSizer);
	DijkstraStartingVertexBoxSizer->Fit (DijkstraStartingVertexPanel);
	DijkstraStartingVertexBoxSizer->SetSizeHints (DijkstraStartingVertexPanel);
	DijkstraVerticesBoxSizer->Add (DijkstraStartingVertexPanel, 1, wxALL|wxEXPAND, 1);
	DijkstraFinalVertexPanel = new wxPanel (DijkstraVerticesPanel, ID_DIJKSTRA_FINAL_VERTEX_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DIJKSTRA_FINAL_VERTEX_PANEL"));
	DijkstraFinalVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DijkstraFinalVertexStaticText2 = new wxStaticText (DijkstraFinalVertexPanel, ID_DIJKSTRA_STARGING_VERTEX_STATICTEXT, _ ("Final vertex"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_DIJKSTRA_STARGING_VERTEX_STATICTEXT"));
	DijkstraFinalVertexBoxSizer->Add (DijkstraFinalVertexStaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DijkstraFinalVertexTextCtrl = new wxTextCtrl (DijkstraFinalVertexPanel, ID_DIJKSTRA_FINAL_VERTEX_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DIJKSTRA_FINAL_VERTEX_TEXTCTRL"));
	DijkstraFinalVertexBoxSizer->Add (DijkstraFinalVertexTextCtrl, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DijkstraFinalVertexPanel->SetSizer (DijkstraFinalVertexBoxSizer);
	DijkstraFinalVertexBoxSizer->Fit (DijkstraFinalVertexPanel);
	DijkstraFinalVertexBoxSizer->SetSizeHints (DijkstraFinalVertexPanel);
	DijkstraVerticesBoxSizer->Add (DijkstraFinalVertexPanel, 1, wxALL|wxEXPAND, 1);
	DijkstraVerticesPanel->SetSizer (DijkstraVerticesBoxSizer);
	DijkstraVerticesBoxSizer->Fit (DijkstraVerticesPanel);
	DijkstraVerticesBoxSizer->SetSizeHints (DijkstraVerticesPanel);
	DijkstraFlexGridSizer->Add (DijkstraVerticesPanel, 1, wxALL|wxEXPAND, 1);
	DijkstraButtonsPanel = new wxPanel (DijkstraPanel, ID_DIJKSTRA_BUTTONS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DIJKSTRA_BUTTONS_PANEL"));
	DijkstraButtonsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DijkstraButtonsBoxSizer->Add (-1,-1,2, wxALL|wxEXPAND, 1);
	DijkstraExecuteButton = new wxButton (DijkstraButtonsPanel, ID_DIJKSTRA_EXECUTE_BUTTON, _ ("Execute"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DIJKSTRA_EXECUTE_BUTTON"));
	DijkstraButtonsBoxSizer->Add (DijkstraExecuteButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DijkstraCancelButton = new wxButton (DijkstraButtonsPanel, ID_DIJKSTRA_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_DIJKSTRA_CANCEL_BUTTON"));
	DijkstraButtonsBoxSizer->Add (DijkstraCancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	DijkstraButtonsPanel->SetSizer (DijkstraButtonsBoxSizer);
	DijkstraButtonsBoxSizer->Fit (DijkstraButtonsPanel);
	DijkstraButtonsBoxSizer->SetSizeHints (DijkstraButtonsPanel);
	DijkstraFlexGridSizer->Add (DijkstraButtonsPanel, 1, wxALL|wxEXPAND, 1);
	DijkstraPanel->SetSizer (DijkstraFlexGridSizer);
	DijkstraFlexGridSizer->Fit (DijkstraPanel);
	DijkstraFlexGridSizer->SetSizeHints (DijkstraPanel);

	Connect (ID_DIJKSTRA_EXECUTE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &OptimumPathInput::OnDijkstraExecuteButtonClick);
	Connect (ID_DIJKSTRA_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &OptimumPathInput::OnDijkstraCancelButtonClick);
	//*)

	Centre();
}

OptimumPathInput::~OptimumPathInput()
{
	//(*Destroy(OptimumPathInput)
	//*)
}


void OptimumPathInput::OnDijkstraExecuteButtonClick (wxCommandEvent& event)
{
	wxString s, t;

	s = DijkstraStartingVertexTextCtrl->GetValue();
	t = DijkstraFinalVertexTextCtrl->GetValue();

	if (s.length() == 0 || t.length() == 0)
	{
		report (wxT ("Either the starting or the ending vertex has no value."),"Note");
		return;
	}
	else
	{
		*startingVertex = s;
		*finalVertex = t;
	}

	EndModal (wxID_OK);
}

void OptimumPathInput::OnDijkstraCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
