/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/build/RandomClique.h"

//(*InternalHeaders(RandomClique)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/msgdlg.h>

//(*IdInit(RandomClique)
const long RandomClique::ID_CLIQUE_DIALOG_STATICTEXT = wxNewId();
const long RandomClique::ID_CLIQUE_SIZE_STATICTEXT = wxNewId();
const long RandomClique::ID_CLIQUE_SIZE_TEXTCTRL = wxNewId();
const long RandomClique::ID_CLIQUE_SIZE_PANEL = wxNewId();
const long RandomClique::ID_NUMBER_OF_VERTICES_STATICTEXT = wxNewId();
const long RandomClique::ID_NUMBER_OF_VERTICES_TEXTCTRL = wxNewId();
const long RandomClique::ID_NUMBER_OF_VERTICES_PANEL = wxNewId();
const long RandomClique::ID_NUMBER_OF_EDGES_STATICTEXT = wxNewId();
const long RandomClique::ID_NUMBER_OF_EDGES_TEXTCTRL = wxNewId();
const long RandomClique::ID_NUMBER_OF_EDGES_PANEL = wxNewId();
const long RandomClique::ID_BUILD_BUTTON = wxNewId();
const long RandomClique::ID_CONTROL_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (RandomClique,wxDialog)
	//(*EventTable(RandomClique)
	//*)
END_EVENT_TABLE()

RandomClique::RandomClique (wxWindow* parent, long *cSize, long *nvert, long *nedges,
                            wxWindowID id,const wxPoint& pos,
                            const wxSize& size) :
	cliqueSize (cSize), numberOfVertices (nvert), numberOfEdges (nedges)

{
	//(*Initialize(RandomClique)
	wxBoxSizer* numberOfVerticesBoxSizer;
	wxGridSizer* cliqueGridSizer;
	wxBoxSizer* numberOfEdgesBoxSizer;
	wxBoxSizer* controlPanelBoxSizer;
	wxBoxSizer* cliqueSizeBoxSizer;

	Create (parent, wxID_ANY, _ ("Build a Random Graph with a Clique"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	cliqueGridSizer = new wxGridSizer (5, 1, 0, 0);
	cliqueDialogStaticText = new wxStaticText (this, ID_CLIQUE_DIALOG_STATICTEXT, _ ("Random Graph with a Clique"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_CLIQUE_DIALOG_STATICTEXT"));
	cliqueGridSizer->Add (cliqueDialogStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	cliqueSizePanel = new wxPanel (this, ID_CLIQUE_SIZE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_CLIQUE_SIZE_PANEL"));
	cliqueSizeBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	cliqueSizeStaticText = new wxStaticText (cliqueSizePanel, ID_CLIQUE_SIZE_STATICTEXT, _ ("Clique size"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T ("ID_CLIQUE_SIZE_STATICTEXT"));
	cliqueSizeBoxSizer->Add (cliqueSizeStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	cliqueSizeTextCtrl = new wxTextCtrl (cliqueSizePanel, ID_CLIQUE_SIZE_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_CLIQUE_SIZE_TEXTCTRL"));
	cliqueSizeBoxSizer->Add (cliqueSizeTextCtrl, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	cliqueSizePanel->SetSizer (cliqueSizeBoxSizer);
	cliqueSizeBoxSizer->Fit (cliqueSizePanel);
	cliqueSizeBoxSizer->SetSizeHints (cliqueSizePanel);
	cliqueGridSizer->Add (cliqueSizePanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	numberOfVerticesPanel = new wxPanel (this, ID_NUMBER_OF_VERTICES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_NUMBER_OF_VERTICES_PANEL"));
	numberOfVerticesBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	numberOfVerticesStaticText = new wxStaticText (numberOfVerticesPanel, ID_NUMBER_OF_VERTICES_STATICTEXT, _ ("Number of vertices"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T ("ID_NUMBER_OF_VERTICES_STATICTEXT"));
	numberOfVerticesBoxSizer->Add (numberOfVerticesStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	numberOfVerticesTextCtrl = new wxTextCtrl (numberOfVerticesPanel, ID_NUMBER_OF_VERTICES_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_NUMBER_OF_VERTICES_TEXTCTRL"));
	numberOfVerticesBoxSizer->Add (numberOfVerticesTextCtrl, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	numberOfVerticesPanel->SetSizer (numberOfVerticesBoxSizer);
	numberOfVerticesBoxSizer->Fit (numberOfVerticesPanel);
	numberOfVerticesBoxSizer->SetSizeHints (numberOfVerticesPanel);
	cliqueGridSizer->Add (numberOfVerticesPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	numberOfEdgesPanel = new wxPanel (this, ID_NUMBER_OF_EDGES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_NUMBER_OF_EDGES_PANEL"));
	numberOfEdgesBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	numberOfEdgesStaticText = new wxStaticText (numberOfEdgesPanel, ID_NUMBER_OF_EDGES_STATICTEXT, _ ("Number of edges"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T ("ID_NUMBER_OF_EDGES_STATICTEXT"));
	numberOfEdgesBoxSizer->Add (numberOfEdgesStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	numberOfEdgesTextCtrl = new wxTextCtrl (numberOfEdgesPanel, ID_NUMBER_OF_EDGES_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_NUMBER_OF_EDGES_TEXTCTRL"));
	numberOfEdgesBoxSizer->Add (numberOfEdgesTextCtrl, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	numberOfEdgesPanel->SetSizer (numberOfEdgesBoxSizer);
	numberOfEdgesBoxSizer->Fit (numberOfEdgesPanel);
	numberOfEdgesBoxSizer->SetSizeHints (numberOfEdgesPanel);
	cliqueGridSizer->Add (numberOfEdgesPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	controlPanel = new wxPanel (this, ID_CONTROL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_CONTROL_PANEL"));
	controlPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	controlPanelBoxSizer->Add (-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	buildButton = new wxButton (controlPanel, ID_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BUILD_BUTTON"));
	buildButton->SetDefault();
	controlPanelBoxSizer->Add (buildButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	controlPanel->SetSizer (controlPanelBoxSizer);
	controlPanelBoxSizer->Fit (controlPanel);
	controlPanelBoxSizer->SetSizeHints (controlPanel);
	cliqueGridSizer->Add (controlPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	SetSizer (cliqueGridSizer);
	cliqueGridSizer->Fit (this);
	cliqueGridSizer->SetSizeHints (this);

	Connect (ID_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &RandomClique::OnBuildRandomClique);
	//*)
}

RandomClique::~RandomClique()
{
	//(*Destroy(RandomClique)
	//*)
}

void RandomClique::OnBuildRandomClique(wxCommandEvent& event)
{
	long m, n, p;

	if (!checkValue (m, CLIQUE_SIZE))
		return;
	else
		*cliqueSize = m;

	if (!checkValue (n, NUMBER_OF_VERTICES))
		return;
	else
		*numberOfVertices = n;

	if (!checkValue (p, NUMBER_OF_EDGES))
		return;
	else
		*numberOfEdges = p;

	if (n < m)
	{
		wxMessageDialog dialog (this, wxT ("The number of vertices is less than\nthe number in the clique."), wxT ("Note"), wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return;
	}

	if ( (m*(m+1)/2) > p)
	{
		wxMessageDialog dialog (this, wxT ("The number of edges in the clique\nis larger than the number in the graph."), wxT ("Note"), wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return;
	}

	EndModal (wxOK);
}

bool RandomClique::checkValue (long& n, TextControlType type)
{
	wxString str, errMsg;
	bool error = false;

	switch (type)
	{
	case CLIQUE_SIZE:
		str = cliqueSizeTextCtrl->GetValue();

		if (str.length() == 0)
		{
			errMsg = wxT ("You have not specified a clique size.");
			error = true;
			break;
		}

		if (!str.ToLong (&n))
		{
			errMsg = wxT ("The clique size is not a valid integer.");
			error = true;
			break;
		}

		break;

	case NUMBER_OF_VERTICES:
		str = numberOfVerticesTextCtrl->GetValue();

		if (str.length() == 0)
		{
			errMsg = wxT ("You have not specified the number of vertices.");
			error = true;
			break;
		}

		if (!str.ToLong (&n))
		{
			errMsg = wxT ("The number of vertices is not a valid integer.");
			error = true;
			break;
		}

		break;

	case NUMBER_OF_EDGES:
		str = numberOfEdgesTextCtrl->GetValue();

		if (str.length() == 0)
		{
			errMsg = wxT ("You have not specified the number of edges.");
			error = true;
			break;
		}

		if (!str.ToLong (&n))
		{
			errMsg = wxT ("The number of edges is not a valid integer.");
			error = true;
			break;
		}

		break;
	}

	if (error)
	{
		wxMessageDialog dialog (this, errMsg, wxT ("Note"), wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return false;
	}
	else
		return true;
}
