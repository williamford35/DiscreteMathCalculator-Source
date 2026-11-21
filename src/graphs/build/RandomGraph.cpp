/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/build/RandomGraph.h"
#include "common/randomc.h"
#include "common/report.h"
#include "graphs/draw/DrawGraph.h"
#include "graphs/graphFileFormatException.h"
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/planarity/PlanRep.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>

#include <ogdf/basic/Queue.h>
#include <ogdf/basic/geometry.h>

#include <string>
using namespace std;

#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>

#include <wx/msgdlg.h>

//(*InternalHeadersPCH(RandomGraph)
#include <wx/intl.h>
#include <wx/string.h>
//*)
//(*InternalHeaders(RandomGraph)
//*)

//(*IdInit(RandomGraph)
const long RandomGraph::ID_RANDOM_GRAPH_VERTICES_STATICTEXT = wxNewId();
const long RandomGraph::ID_RANDOM_GRAPH_VERTICES_TEXTCTRL = wxNewId();
const long RandomGraph::ID_RANDOM_GRAPH_VERTEX_PANEL = wxNewId();
const long RandomGraph::ID_RANDOM_GRAPH_EDGES_STATICTEXT = wxNewId();
const long RandomGraph::ID_RANDOM_GRAPH_EDGES_TEXTCTRL = wxNewId();
const long RandomGraph::ID_RANDOM_GRAPH_EDGES_PANEL = wxNewId();
const long RandomGraph::ID_GRAPH_TYPE_RADIOBOX = wxNewId();
const long RandomGraph::ID_BUILD_BUTTON = wxNewId();
const long RandomGraph::ID_CANCEL_BUTTON = wxNewId();
const long RandomGraph::ID_RANDOM_GRAPH_CONTROL_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (RandomGraph,wxDialog)
	//(*EventTable(RandomGraph)
	//*)
END_EVENT_TABLE()

RandomGraph::RandomGraph (wxWindow* parent, GraphType gtype,
                          RandomGraphType *rtype, RandomDigraphType *rdtype,
                          int *m, int *n,
                          wxWindowID id, const wxPoint& pos,const wxSize& size) :
	graphType (gtype), randomGraphType (rtype),
	randomDigraphType (rdtype),
	numberOfVertices (m), numberOfEdges (n)
{
	//(*Initialize(RandomGraph)
	wxFlexGridSizer* randomGraphFlexGridSizer;
	wxFlexGridSizer* randomGraphControlFlexGridSizer;
	wxBoxSizer* randomGraphVertexBoxSizer;
	wxBoxSizer* randomGraphEdgesBoxSizer;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T ("wxID_ANY"));
	randomGraphFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	randomGraphVertexPanel = new wxPanel (this, ID_RANDOM_GRAPH_VERTEX_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_RANDOM_GRAPH_VERTEX_PANEL"));
	randomGraphVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	randomGraphVerticesStaticText = new wxStaticText (randomGraphVertexPanel, ID_RANDOM_GRAPH_VERTICES_STATICTEXT, _ ("Number of vertices"), wxDefaultPosition, wxSize (210,20), wxALIGN_LEFT, _T ("ID_RANDOM_GRAPH_VERTICES_STATICTEXT"));
	randomGraphVertexBoxSizer->Add (randomGraphVerticesStaticText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
	randomGraphVerticesTextCtrl = new wxTextCtrl (randomGraphVertexPanel, ID_RANDOM_GRAPH_VERTICES_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RANDOM_GRAPH_VERTICES_TEXTCTRL"));
	randomGraphVertexBoxSizer->Add (randomGraphVerticesTextCtrl, 2, wxALL|wxALIGN_CENTER_VERTICAL, 10);
	randomGraphVertexPanel->SetSizer (randomGraphVertexBoxSizer);
	randomGraphVertexBoxSizer->Fit (randomGraphVertexPanel);
	randomGraphVertexBoxSizer->SetSizeHints (randomGraphVertexPanel);
	randomGraphFlexGridSizer->Add (randomGraphVertexPanel, 1, wxALL|wxEXPAND, 5);
	randomGraphEdgesPanel = new wxPanel (this, ID_RANDOM_GRAPH_EDGES_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_RANDOM_GRAPH_EDGES_PANEL"));
	randomGraphEdgesBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	randomGraphEdgesStaticText = new wxStaticText (randomGraphEdgesPanel, ID_RANDOM_GRAPH_EDGES_STATICTEXT, _ ("Number of edges"), wxDefaultPosition, wxSize (210,20), wxALIGN_LEFT, _T ("ID_RANDOM_GRAPH_EDGES_STATICTEXT"));
	randomGraphEdgesBoxSizer->Add (randomGraphEdgesStaticText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 1);
	randomGraphEdgesTextCtrl = new wxTextCtrl (randomGraphEdgesPanel, ID_RANDOM_GRAPH_EDGES_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RANDOM_GRAPH_EDGES_TEXTCTRL"));
	randomGraphEdgesBoxSizer->Add (randomGraphEdgesTextCtrl, 2, wxALL|wxALIGN_CENTER_VERTICAL, 10);
	randomGraphEdgesPanel->SetSizer (randomGraphEdgesBoxSizer);
	randomGraphEdgesBoxSizer->Fit (randomGraphEdgesPanel);
	randomGraphEdgesBoxSizer->SetSizeHints (randomGraphEdgesPanel);
	randomGraphFlexGridSizer->Add (randomGraphEdgesPanel, 1, wxALL|wxEXPAND, 5);
	randomGraphControlPanel = new wxPanel (this, ID_RANDOM_GRAPH_CONTROL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_RANDOM_GRAPH_CONTROL_PANEL"));
	randomGraphControlFlexGridSizer = new wxFlexGridSizer (0, 4, 0, 0);
	randomGraphControlFlexGridSizer->AddGrowableCol (0);
	randomGraphControlFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 1);
	wxString wxRadioBoxChoices[7];

	if ( (graphType == undir) || (graphType == undirw))
	{
		wxRadioBoxChoices[0] = _ ("General");
		wxRadioBoxChoices[1] = _ ("Simple Connected");
		wxRadioBoxChoices[2] = _ ("Biconnected");
		wxRadioBoxChoices[3] = _ ("Triconnected");
		wxRadioBoxChoices[4] = _ ("Planar");
		wxRadioBoxChoices[5] = _ ("Acyclic");
		wxRadioBoxChoices[6] = _ ("Complete Bipartite");
		graphTypeRadioBox = new wxRadioBox (randomGraphControlPanel, ID_GRAPH_TYPE_RADIOBOX,
		                                    _ ("Type"), wxDefaultPosition, wxDefaultSize, 7, wxRadioBoxChoices, 7,
		                                    wxRA_SPECIFY_ROWS, wxDefaultValidator, _T ("ID_GRAPH_TYPE_RADIOBOX"));
	}
	else
	{
		wxRadioBoxChoices[0] = _ ("Strongly connected");
		wxRadioBoxChoices[1] = _ ("Strongly connected components");
		wxRadioBoxChoices[2] = _ ("Acyclic");
		wxRadioBoxChoices[3] = _ ("Tournament");
		graphTypeRadioBox = new wxRadioBox (randomGraphControlPanel, ID_GRAPH_TYPE_RADIOBOX, _ ("Type"),
		                                    wxDefaultPosition, wxDefaultSize, 4, wxRadioBoxChoices, 4,
		                                    wxRA_SPECIFY_ROWS, wxDefaultValidator, _T ("ID_GRAPH_TYPE_RADIOBOX"));
	}

	randomGraphControlFlexGridSizer->Add (graphTypeRadioBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	buildButton = new wxButton (randomGraphControlPanel, ID_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BUILD_BUTTON"));
	randomGraphControlFlexGridSizer->Add (buildButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 35);
	caancelButton = new wxButton (randomGraphControlPanel, ID_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_CANCEL_BUTTON"));
	randomGraphControlFlexGridSizer->Add (caancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	randomGraphControlPanel->SetSizer (randomGraphControlFlexGridSizer);
	randomGraphControlFlexGridSizer->Fit (randomGraphControlPanel);
	randomGraphControlFlexGridSizer->SetSizeHints (randomGraphControlPanel);
	randomGraphFlexGridSizer->Add (randomGraphControlPanel, 1, wxALL|wxEXPAND, 1);
	SetSizer (randomGraphFlexGridSizer);
	randomGraphFlexGridSizer->Fit (this);
	randomGraphFlexGridSizer->SetSizeHints (this);

	if ( (graphType == undir) || (graphType == undirw))
	{
		randomGraphEdgesStaticText->wxWindow::Show (true);
		randomGraphEdgesTextCtrl->wxWindow::Show (true);
	}
	else
	{
		randomGraphEdgesStaticText->wxWindow::Show (false);
		randomGraphEdgesTextCtrl->wxWindow::Show (false);
	}

    SetSize(650, 400);
	Connect (ID_GRAPH_TYPE_RADIOBOX,wxEVT_COMMAND_RADIOBOX_SELECTED, (wxObjectEventFunction) &RandomGraph::OngraphTypeRadioBoxSelect);
	Connect (ID_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &RandomGraph::OnrandomGraphBuildButtonClick);
	Connect (ID_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &RandomGraph::OnrandomGraphCancelButtonClick);
	//*)
	//randomGraphVerticesStaticText->Wrap(400);
	biPartite = false;
	isAcyclic = false;
}

RandomGraph::~RandomGraph()
{
	//(*Destroy(RandomGraph)
	//*)
}

void RandomGraph::OnrandomGraphBuildButtonClick (wxCommandEvent& event)
{
	wxString strNumberOfVertices, strNumberOfEdges;
	int selectedGraphType;
	long numVertices, numEdges = 0;

	strNumberOfVertices = randomGraphVerticesTextCtrl->GetValue();
	selectedGraphType = graphTypeRadioBox->GetSelection();

	if ( (graphType == undir) || (graphType == undirw))
	{
		if (strNumberOfVertices.length() == 0)
		{
			if (!biPartite)
				report (wxT ("You have not entered the number of vertices in the graph."),"Note");
			else
				report (wxT ("You have not entered the size of first partition."),"Note");

			return;
		}

		if (!strNumberOfVertices.ToLong (&numVertices))
		{
			if (!biPartite)
				report (wxT ("The number of vertices is not in correct integer format."),"Note");
			else
				report (wxT ("The size of first partition is not in correct integer format."),"Note");

			return;
		}
	}
	else
	{
		if (strNumberOfVertices.length() == 0)
		{
			if (selectedGraphType != 1)
				report (wxT ("You have not entered the number of vertices in the graph."),"Note");
			else
				report (wxT ("You have not entered the number of strong components."),"Note");

			return;
		}

		if (!strNumberOfVertices.ToLong (&numVertices))
		{
			if (selectedGraphType != 1)
				report (wxT ("The number of vertices is not in correct integer format."),"Note");
			else
				report (wxT ("The number of strong components is not in correct integer format."),"Note");

			return;
		}
	}

	strNumberOfEdges = randomGraphEdgesTextCtrl->GetValue();

	if (!isAcyclic)
	{
		if ( (graphType == undir) || (graphType == undirw))
		{
			if (strNumberOfEdges.length() == 0)
			{
				if (!biPartite && !isAcyclic)
					report (wxT ("You have not entered the number of edges in the graph."),"Note");
				else if (!isAcyclic)
					report (wxT ("You have not entered the size of second partition."),"Note");

				return;
			}

			if (!strNumberOfEdges.ToLong (&numEdges))
			{
				if (!biPartite)
					report (wxT ("The number of edges is not in correct integer format."),"Note");
				else if (!isAcyclic)
					report (wxT ("The size of second partition is not in correct integer format."),"Note");

				if (!isAcyclic)
					return;
			}
		}
		else
		{
			if (strNumberOfEdges.length() == 0)
			{
				if ( selectedGraphType == 1)
				{
					report (wxT ("You have not entered the maximum size of a strong component."),"Note");
					return;
				}
			}

			if ( (selectedGraphType != 0) && (selectedGraphType != 2) && (selectedGraphType != 3) && !strNumberOfEdges.ToLong (&numEdges))
			{
				report (wxT ("The maximum size of a strong component is not in correct integer format."),"Note");
				return;
			}
		}
	}

	*numberOfVertices = (int) numVertices;
	*numberOfEdges = (int) numEdges;

	if ( (graphType == undir) || (graphType == undirw))
		switch (selectedGraphType)
		{
		case 0:
			*randomGraphType = general;
			break;

		case 1:
			*randomGraphType = simple;
			break;

		case 2:
			*randomGraphType = biconnected;
			break;

		case 3:
			*randomGraphType = triconnected;
			break;

		case 4:
			*randomGraphType = planar;
			break;

		case 5:
			*randomGraphType = acyclic;
			break;

		case 6:
			*randomGraphType = bipartite;
			break;
		}
	else
		switch (selectedGraphType)
		{
		case 0:
			*randomDigraphType = randomDigraph;
			break;

		case 1:
			*randomDigraphType = strongly_connected;
			break;

		case 2:
			*randomDigraphType = dag;
			break;

		case 3:
			*randomDigraphType = tournament;
			break;
		}

	EndModal (wxID_OK);
}

void RandomGraph::OnrandomGraphCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void RandomGraph::OngraphTypeRadioBoxSelect (wxCommandEvent& event)
{
	int buttonSelected = graphTypeRadioBox->GetSelection();

	if ( (graphType == undir) || (graphType == undirw))
		if (buttonSelected == 6)
		{
			randomGraphVerticesStaticText->SetLabel (wxT ("Size of first partition"));
			randomGraphEdgesStaticText->SetLabel (wxT ("Size of second partition"));
			randomGraphEdgesStaticText->wxWindow::Show (true);
			randomGraphEdgesTextCtrl->wxWindow::Show (true);
			biPartite = true;
		}
		else if (buttonSelected == 5)
		{
			randomGraphVerticesStaticText->SetLabel (wxT ("Number of vertices"));
			randomGraphEdgesStaticText->wxWindow::Show (false);
			randomGraphEdgesTextCtrl->wxWindow::Show (false);
			isAcyclic = true;
		}
		else
		{
			randomGraphVerticesStaticText->SetLabel (wxT ("Number of vertices"));
			randomGraphEdgesStaticText->SetLabel (wxT ("Number of edges"));
			randomGraphEdgesStaticText->wxWindow::Show (true);
			randomGraphEdgesTextCtrl->wxWindow::Show (true);
			biPartite = false;
		}
	else
	{
		if ( (buttonSelected == 0) || (buttonSelected == 2) || (buttonSelected == 3))
		{
			randomGraphVerticesStaticText->SetLabel (wxT ("Number of vertices"));
			randomGraphEdgesStaticText->wxWindow::Show (false);
			randomGraphEdgesTextCtrl->wxWindow::Show (false);
		}
		else
		{
			randomGraphVerticesStaticText->SetLabel (wxT ("Number of strong components"));
			randomGraphEdgesStaticText->SetLabel (wxT ("Max vertices per component"));
			randomGraphEdgesStaticText->wxWindow::Show (true);
			randomGraphEdgesTextCtrl->wxWindow::Show (true);
		}
	}
}
