/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "RelationsPartialOrders/RelationInput.h"
#include "common/ImageFiles.h"

#include <wx/intl.h>
#include <wx/string.h>

BEGIN_EVENT_TABLE (RelationInput,wxDialog)
END_EVENT_TABLE()

long RelationInput::TEMPLATE1TEXTCTRL_ID[10];
long RelationInput::TEMPLATESTATICTEXT_ID[10];
long RelationInput::TEMPLATE2TEXTCTRL_ID[10];

RelationInput::RelationInput (wxWindow* parent, const char *imageFileName)
{
	Create (parent, wxID_ANY, _ ("Input a relation"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T ("wxID_ANY"));
	SetClientSize (wxDefaultSize);
	Move (wxDefaultPosition);

	wxBoxSizer* RelationInputBoxSizer = new wxBoxSizer (wxVERTICAL);
	wxGridSizer* GridSizer = new wxGridSizer (5, 2, 0, 0);

	this->SetSizer (RelationInputBoxSizer);

	RelationInputStaticBitMap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (imageFileName) ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
	RelationInputBoxSizer->Add (RelationInputStaticBitMap, 0, wxALL|wxALIGN_CENTER, 1);
	RelationInputDataPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
	RelationInputBoxSizer->Add(RelationInputDataPanel, 0, wxALL, 1);
	RelationInputDataPanel->SetSizer(GridSizer);

	for (int i = 0; i < 10; i++)
	{
		TEMPLATE1TEXTCTRL_ID[i] = wxNewId();
		TEMPLATE2TEXTCTRL_ID[ i] = wxNewId();
		TEMPLATESTATICTEXT_ID[i] = wxNewId();
		addCell (i, GridSizer);
	}

	RelationInputButtonsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	RelationInputBoxSizer->Add (RelationInputButtonsPanel, 0, wxALL|wxEXPAND, 1);
	wxBoxSizer* RelationInputButtonsPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	RelationInputButtonsPanel->SetSizer (RelationInputButtonsPanelBoxSizer);
	RelationInputButtonsPanelBoxSizer->Add (0, 0, 10, wxALL, 1);
	RelationInputCancelButton = new wxButton (RelationInputButtonsPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);
	RelationInputButtonsPanelBoxSizer->Add (RelationInputCancelButton, 0, wxALL|wxALIGN_BOTTOM, 8);
	RelationInputDisplayButton = new wxButton (RelationInputButtonsPanel, wxID_ANY, _ ("Display"), wxDefaultPosition, wxSize (-1,-1), 0);
	RelationInputButtonsPanelBoxSizer->Add (RelationInputDisplayButton, 0, wxALL|wxALIGN_BOTTOM, 8);

	if ( GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	CentreOnParent (wxBOTH);
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
	RelationInputCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RelationInput::OnRelationInputCancelButtonPress), NULL, this);
	RelationInputDisplayButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RelationInput::OnRelationInputDisplayButtonPress), NULL, this);
}

RelationInput::~RelationInput()
{
	RelationInputCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RelationInput::OnRelationInputCancelButtonPress), NULL, this);
	RelationInputDisplayButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RelationInput::OnRelationInputDisplayButtonPress), NULL, this);

}

void RelationInput::OnRelationInputCancelButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void RelationInput::OnRelationInputDisplayButtonPress (wxCommandEvent& event)
{
	wxString sourceVertex, destVertex;
	RelationInputCell cell;

	// read through the panels and build the relation digraph
	for (int i=0; i < 10; i++)
	{
		cell = theCell[i];
		sourceVertex = cell.sourceTextCtrl->GetValue();

		if (sourceVertex.length() == 0)
			break;

		destVertex = cell.destTextCtrl->GetValue();

		diGraphToBuild.push_back (RelationGraphEdge (sourceVertex, destVertex));
	}

	EndModal (wxID_OK);
}

void RelationInput::addCell (int i, wxGridSizer *grid)
{
	wxBoxSizer* templateBoxSizer;
	wxPanel* panel = new wxPanel(RelationInputDataPanel, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
	wxStaticText* EdgeNumber;
	wxTextCtrl* template1TextCtrl;
	wxTextCtrl* template1TextCtr2;
	wxStaticText* goesTo;
	wxString numericLabel;

	templateBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	numericLabel.Printf(wxT("%1d."), i+1);
	EdgeNumber = new wxStaticText(panel, wxID_ANY, numericLabel, wxDefaultPosition, wxSize(-1,-1), 0);
	templateBoxSizer->Add(EdgeNumber, 0, wxALL, 1);

	template1TextCtrl = new wxTextCtrl(panel, TEMPLATE1TEXTCTRL_ID[i], wxT(""), wxDefaultPosition, wxSize(50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	template1TextCtrl->SetHint(wxT(""));
#endif

	templateBoxSizer->Add(template1TextCtrl, 0, wxALL, 1);

	goesTo = new wxStaticText(panel, TEMPLATESTATICTEXT_ID[i], _("-->"), wxDefaultPosition, wxSize(-1,-1), 0);

	templateBoxSizer->Add(goesTo, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	template1TextCtr2 = new wxTextCtrl(panel, TEMPLATE2TEXTCTRL_ID[i], wxT(""), wxDefaultPosition, wxSize(50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	template1TextCtr2->SetHint(wxT(""));
#endif

	templateBoxSizer->Add(template1TextCtr2, 0, wxALL, 1);
	panel->SetSizer (templateBoxSizer);
	templateBoxSizer->Fit (panel);

	theCell.push_back (RelationInputCell(template1TextCtrl, template1TextCtr2));
	grid->Add(panel, 1, wxALL|wxALIGN_CENTER, 5);
}

vector<RelationGraphEdge> RelationInput::getGraphParameters()
{
	return diGraphToBuild;
}
