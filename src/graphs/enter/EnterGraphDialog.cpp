/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/enter/EnterGraphDialog.h"
#include "common/ImageFiles.h"
#include <wx/msgdlg.h>

#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>


const long EnterGraphDialog::ID_ENTER_GRAPH_STATICBITMAP = wxNewId();
const long EnterGraphDialog::ID_ENTER_GRAPH_BUILD_BUTTON = wxNewId();
const long EnterGraphDialog::ID_ENTER_GRAPH_CANCEL_BUTTON = wxNewId();
const long EnterGraphDialog::ID_ENTER_GRAPH_BUTTONS_PANEL = wxNewId();
const long EnterGraphDialog::ID_ENTER_GRAPH_BUTTON_PANEL = wxNewId();
const long EnterGraphDialog::ID_ENTER_GRAPH_BITMAP_PANEL = wxNewId();

BEGIN_EVENT_TABLE (EnterGraphDialog,wxDialog)
END_EVENT_TABLE()

EnterGraphDialog::EnterGraphDialog (wxWindow* parent,wxWindowID id, GraphType gt) :
	graphType (gt)
{
	int i;
	wxFlexGridSizer* enterGraphFlexGridSizer;
	wxBoxSizer* enterGraphButtonsBoxSizer;
	wxFlexGridSizer* enterGraphButtonsFlexGridSizer;

	for (i=0; i <36; i++)
	{
		ID_ENTER_GRAPH_EDGE_STATICTEXT[i] = wxNewId();
		ID_ENTER_GRAPH_SOURCE_VERTEX_TEXTCTRL[i] = wxNewId();
		ID_ENTER_GRAPH_DESTINATION_VERTEX_TEXTCTRL[i] = wxNewId();
		ID_ENTER_GRAPH_EDGE_ENTRY_PANEL[i] = wxNewId();
		ID_ENTER_GRAPH_EDGE_PANEL[i] = wxNewId();
		ID_ENTER_GRAPH_WEIGHT_STATICTEXT[i] = wxNewId();
		ID_ENTER_GRAPH_EDGE_WEIGHT_TEXTCTRL[i] = wxNewId();
		ID_ENTER_GRAPH_WEIGHT_ENTRY_PANEL[i] = wxNewId();
		ID_ENTER_GRAPH_VERTEX_EDGE_ENTRY_PANEL[i] = wxNewId();
	}

	Create (parent, id, _ ("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T ("id"));
	enterGraphFlexGridSizer = new wxFlexGridSizer (13, 3, 0, 8);
	enterGraphFlexGridSizer->AddGrowableCol (0);

	for (i=0; i < 36; i++)
		enterGraphFlexGridSizer->Add (buildGraphEntry (i), 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	enterGraphFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 2);
	enterBitMapPanel = new wxPanel (this, ID_ENTER_GRAPH_BITMAP_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ENTER_GRAPH_BITMAP_PANEL"));

	switch (graphType)
	{
	case undir:
		enterGraphStaticBitmap = new wxStaticBitmap (enterBitMapPanel, ID_ENTER_GRAPH_STATICBITMAP, wxBitmap (wxImage (imagefile ("uedgenoweight.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ENTER_GRAPH_STATICBITMAP"));
		break;

	case undirw:
		enterGraphStaticBitmap = new wxStaticBitmap (enterBitMapPanel, ID_ENTER_GRAPH_STATICBITMAP, wxBitmap (wxImage (imagefile ("uedgeweight.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ENTER_GRAPH_STATICBITMAP"));
		break;

	case digraph:
		enterGraphStaticBitmap = new wxStaticBitmap (enterBitMapPanel, ID_ENTER_GRAPH_STATICBITMAP, wxBitmap (wxImage (imagefile ("digraphedgenoweight.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ENTER_GRAPH_STATICBITMAP"));
		break;

	case digraphw:
		enterGraphStaticBitmap = new wxStaticBitmap (enterBitMapPanel, ID_ENTER_GRAPH_STATICBITMAP, wxBitmap (wxImage (imagefile ("digraphedgeweight.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ENTER_GRAPH_STATICBITMAP"));
		break;
	}

	enterGraphFlexGridSizer->Add (enterBitMapPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	enterGraphButtonPanel = new wxPanel (this, ID_ENTER_GRAPH_BUTTON_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ENTER_GRAPH_BUTTON_PANEL"));
	enterGraphButtonsFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	enterGraphButtonsFlexGridSizer->AddGrowableCol (0);
	enterGraphButtonsFlexGridSizer->AddGrowableRow (0);
	enterGraphButtonsFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 2);
	enterGraphButtonsFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 2);

	enterGraphButtonsPanel = new wxPanel (enterGraphButtonPanel, ID_ENTER_GRAPH_BUTTONS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ENTER_GRAPH_BUTTONS_PANEL"));
	enterGraphButtonsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	enterGraphBuildButton = new wxButton (enterGraphButtonsPanel, ID_ENTER_GRAPH_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ENTER_GRAPH_BUILD_BUTTON"));
	enterGraphBuildButton->SetDefault();
	enterGraphButtonsBoxSizer->Add (enterGraphBuildButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphCancelButton = new wxButton (enterGraphButtonsPanel, ID_ENTER_GRAPH_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ENTER_GRAPH_CANCEL_BUTTON"));
	enterGraphButtonsBoxSizer->Add (enterGraphCancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphButtonsPanel->SetSizer (enterGraphButtonsBoxSizer);
	enterGraphButtonsBoxSizer->Fit (enterGraphButtonsPanel);
	enterGraphButtonsBoxSizer->SetSizeHints (enterGraphButtonsPanel);
	enterGraphButtonsFlexGridSizer->Add (enterGraphButtonsPanel, 1, wxALL|wxEXPAND, 1);
	enterGraphButtonPanel->SetSizer (enterGraphButtonsFlexGridSizer);
	enterGraphButtonsFlexGridSizer->Fit (enterGraphButtonPanel);
	enterGraphButtonsFlexGridSizer->SetSizeHints (enterGraphButtonPanel);
	enterGraphFlexGridSizer->Add (enterGraphButtonPanel, 1, wxALL|wxEXPAND, 1);
	SetSizer (enterGraphFlexGridSizer);
	enterGraphFlexGridSizer->Fit (this);
	enterGraphFlexGridSizer->SetSizeHints (this);

	Centre();

	Connect (ID_ENTER_GRAPH_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterGraphDialog::OnenterGraphBuildButtonClick);
	Connect (ID_ENTER_GRAPH_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterGraphDialog::OnCancel);
}

EnterGraphDialog::~EnterGraphDialog()
{
}

void EnterGraphDialog::OnCancel (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void EnterGraphDialog::OnenterGraphBuildButtonClick (wxCommandEvent& event)
{
	GraphEdgeTextCtrls e;
	wxString sourceVertex, destVertex, edgeWeight;

	// read through the panels and build the graph
	for (int i=0; i < 36; i++)
	{
		e = edge[i];
		sourceVertex = (e.edgeSource)->GetValue();

		if (sourceVertex.length() == 0)
			break;

		destVertex = (e.edgeDest)->GetValue();

		if (e.edgeWeight != NULL)
			edgeWeight = (e.edgeWeight)->GetValue();

		graphToBuild.push_back (GraphEdge (sourceVertex, destVertex, edgeWeight));
	}

	EndModal (wxID_OK);
}

wxPanel* EnterGraphDialog::buildGraphEntry (int i)
{
	wxBoxSizer* enterGraphVertexEdgeBoxSizer;
	wxBoxSizer* enterGraphEdgeEntryBoxSizer;
	wxFlexGridSizer* enterGraphVertexEdgeEntryFlexGridSizer;
	wxBoxSizer* enterGraphWeightBoxSizer;
	wxPanel* enterGraphVertexEdgeEntryPanel;

	enterGraphVertexEdgeEntryPanel = new wxPanel (this, ID_ENTER_GRAPH_VERTEX_EDGE_ENTRY_PANEL[i], wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, buildStringID ("ID_ENTER_GRAPH_VERTEX_EDGE_ENTRY_PANEL",i));

	if (graphType == undirw || graphType == digraphw)
		enterGraphVertexEdgeEntryFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	else
		enterGraphVertexEdgeEntryFlexGridSizer = new wxFlexGridSizer (0, 1, 0, 0);

	enterGraphEdgePanel = new wxPanel (enterGraphVertexEdgeEntryPanel, ID_ENTER_GRAPH_EDGE_PANEL[i], wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, buildStringID ("ID_ENTER_GRAPH_EDGE_PANEL",i));
	enterGraphVertexEdgeBoxSizer = new wxBoxSizer (wxVERTICAL);
	enterGraphEdgeStaticText = new wxStaticText (enterGraphEdgePanel, ID_ENTER_GRAPH_EDGE_STATICTEXT[i], _ ("Edge"), wxDefaultPosition, wxDefaultSize, 0, buildStringID ("ID_ENTER_GRAPH_EDGE_STATICTEXT",i));
	enterGraphVertexEdgeBoxSizer->Add (enterGraphEdgeStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphEdgeEntryPanel = new wxPanel (enterGraphEdgePanel, ID_ENTER_GRAPH_EDGE_ENTRY_PANEL[i], wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, buildStringID ("ID_ENTER_GRAPH_EDGE_ENTRY_PANEL",i));
	enterGraphEdgeEntryBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	enterGraphSourceVertexTextCtrl = new wxTextCtrl (enterGraphEdgeEntryPanel, ID_ENTER_GRAPH_SOURCE_VERTEX_TEXTCTRL[i],
	      wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, buildStringID ("ID_ENTER_GRAPH_SOURCE_VERTEX_TEXTCTRL",i));
	enterGraphEdgeEntryBoxSizer->Add (enterGraphSourceVertexTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphDestinationVertexTextCtrl = new wxTextCtrl (enterGraphEdgeEntryPanel, ID_ENTER_GRAPH_DESTINATION_VERTEX_TEXTCTRL[i],
	      wxEmptyString, wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, buildStringID ("ID_ENTER_GRAPH_DESTINATION_VERTEX_TEXTCTRL",i));
	enterGraphEdgeEntryBoxSizer->Add (enterGraphDestinationVertexTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphEdgeEntryPanel->SetSizer (enterGraphEdgeEntryBoxSizer);
	enterGraphEdgeEntryBoxSizer->Fit (enterGraphEdgeEntryPanel);
	enterGraphEdgeEntryBoxSizer->SetSizeHints (enterGraphEdgeEntryPanel);
	enterGraphVertexEdgeBoxSizer->Add (enterGraphEdgeEntryPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphEdgePanel->SetSizer (enterGraphVertexEdgeBoxSizer);
	enterGraphVertexEdgeBoxSizer->Fit (enterGraphEdgePanel);
	enterGraphVertexEdgeBoxSizer->SetSizeHints (enterGraphEdgePanel);
	enterGraphVertexEdgeEntryFlexGridSizer->Add (enterGraphEdgePanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	if (graphType == undirw || graphType == digraphw)
	{
		enterGraphWeightEntryPanel = new wxPanel (enterGraphVertexEdgeEntryPanel, ID_ENTER_GRAPH_WEIGHT_ENTRY_PANEL[i], wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, buildStringID ("ID_ENTER_GRAPH_WEIGHT_ENTRY_PANEL",i));
		enterGraphWeightBoxSizer = new wxBoxSizer (wxVERTICAL);
		enterGraphWeightStaticText = new wxStaticText (enterGraphWeightEntryPanel, ID_ENTER_GRAPH_WEIGHT_STATICTEXT[i], _ ("Weight"), wxDefaultPosition, wxDefaultSize, 0, buildStringID ("ID_ENTER_GRAPH_WEIGHT_STATICTEXT",i));
		enterGraphWeightBoxSizer->Add (enterGraphWeightStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	enterGraphEdgeWeightTextCtrl = new wxTextCtrl (
			enterGraphWeightEntryPanel,
			ID_ENTER_GRAPH_EDGE_WEIGHT_TEXTCTRL[i],
			wxEmptyString,
			wxDefaultPosition,
			wxSize(110, -1),
			0,
			wxDefaultValidator,
			buildStringID("ID_ENTER_GRAPH_EDGE_WEIGHT_TEXTCTRL", i));
		enterGraphWeightBoxSizer->Add (enterGraphEdgeWeightTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		enterGraphWeightEntryPanel->SetSizer (enterGraphWeightBoxSizer);
		enterGraphWeightBoxSizer->Fit (enterGraphWeightEntryPanel);
		enterGraphWeightBoxSizer->SetSizeHints (enterGraphWeightEntryPanel);
		enterGraphVertexEdgeEntryFlexGridSizer->Add (enterGraphWeightEntryPanel, 1, wxALL|wxEXPAND, 1);
	}

	enterGraphVertexEdgeEntryPanel->SetSizer (enterGraphVertexEdgeEntryFlexGridSizer);
	enterGraphVertexEdgeEntryFlexGridSizer->Fit (enterGraphVertexEdgeEntryPanel);
	enterGraphVertexEdgeEntryFlexGridSizer->SetSizeHints (enterGraphVertexEdgeEntryPanel);

	edge.push_back (GraphEdgeTextCtrls (enterGraphSourceVertexTextCtrl, enterGraphDestinationVertexTextCtrl,
	                                    (graphType == undirw || graphType == digraphw) ? enterGraphEdgeWeightTextCtrl : NULL));

	return enterGraphVertexEdgeEntryPanel;
}

wxString EnterGraphDialog::buildStringID (const char *id, int i)
{
	wxString idstr;

	idstr.Printf (wxT ("%s%d"), id, i);

	return idstr;
}

vector<GraphEdge> EnterGraphDialog::getGraphParameters()
{
	return graphToBuild;
}
