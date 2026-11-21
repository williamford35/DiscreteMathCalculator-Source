/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/AnalyzeGraph.h"
#include <wx/dialog.h>

AnalyzeGraph::AnalyzeGraph (wxWindow* parent, const AnalyzeGraphData& gdata, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style)
{
	wxFlexGridSizer* AnalyzeGraphFlexGridSizer = new wxFlexGridSizer (1, 1, 0, 0);
	AnalyzeGraphFlexGridSizer->SetFlexibleDirection (wxBOTH);
	AnalyzeGraphFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	AnalyzeGraphFlexGridSizer->AddGrowableCol (0);
	AnalyzeGraphFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (AnalyzeGraphFlexGridSizer);

	AnalyzeGraphNotebook = new wxNotebook (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxBK_DEFAULT);
	AnalyzeGraphNotebook->SetName (wxT ("AnalyzeGraphNotebook"));

	AnalyzeGraphFlexGridSizer->Add (AnalyzeGraphNotebook, 0, wxALL|wxEXPAND, 5);

	AnalyzeGraphGeneralPanel = new wxPanel (AnalyzeGraphNotebook, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	AnalyzeGraphNotebook->AddPage (AnalyzeGraphGeneralPanel, _ ("General"), false);

	wxGridSizer* AnalyzeGraphGeneralGridSizer = new wxGridSizer (6, 2, 0, 0);
	AnalyzeGraphGeneralPanel->SetSizer (AnalyzeGraphGeneralGridSizer);

	AnalyzeGraphGeneralGraphTypeStaticText = new wxStaticText (AnalyzeGraphGeneralPanel, wxID_ANY, _ ("Undirected or directed"), wxDefaultPosition, wxSize (-1,-1), wxALIGN_LEFT);

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralGraphTypeStaticText, 0, wxALL|wxALIGN_LEFT, 1);

	AnalyzeGraphGeneralGraphTypeTextCtrl = new wxTextCtrl (AnalyzeGraphGeneralPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphGeneralGraphTypeTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralGraphTypeTextCtrl, 0, wxALL|wxALIGN_RIGHT, 1);

	AnalyzeGraphGeneralNumberOfVerticesStaticText = new wxStaticText (AnalyzeGraphGeneralPanel, wxID_ANY, _ ("Number of vertices"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfVerticesStaticText, 0, wxALL, 5);

	AnalyzeGraphGeneralNumberOfVerticesTextCtrl = new wxTextCtrl (AnalyzeGraphGeneralPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphGeneralNumberOfVerticesTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfVerticesTextCtrl, 0, wxALL|wxALIGN_RIGHT, 1);

	AnalyzeGraphGeneralNumberOfEdgesStaticText = new wxStaticText (AnalyzeGraphGeneralPanel, wxID_ANY, _ ("Number of edges"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfEdgesStaticText, 0, wxALL, 5);

	AnalyzeGraphGeneralNumberOfEdgesTextCtrl = new wxTextCtrl (AnalyzeGraphGeneralPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphGeneralNumberOfEdgesTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfEdgesTextCtrl, 0, wxALL|wxALIGN_RIGHT, 1);

	AnalyzeGraphGeneralNumberOfSelfLoopsStaticText = new wxStaticText (AnalyzeGraphGeneralPanel, wxID_ANY, _ ("Number of self-loops"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfSelfLoopsStaticText, 0, wxALL, 5);

	AnalyzeGraphGeneralNumberOfSelfLoopsTextCtrl = new wxTextCtrl (AnalyzeGraphGeneralPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphGeneralNumberOfSelfLoopsTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfSelfLoopsTextCtrl, 0, wxALL|wxALIGN_RIGHT, 1);

	AnalyzeGraphGeneralNumberOfParallelEdgesStaticText = new wxStaticText (AnalyzeGraphGeneralPanel, wxID_ANY, _ ("Parallel edges"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfParallelEdgesStaticText, 0, wxALL, 5);

	AnalyzeGraphGeneralNumberOfParallelEdgesTextCtrl = new wxTextCtrl (AnalyzeGraphGeneralPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphGeneralNumberOfParallelEdgesTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphGeneralGridSizer->Add (AnalyzeGraphGeneralNumberOfParallelEdgesTextCtrl, 0, wxALL|wxALIGN_RIGHT, 1);
	AnalyzeGraphCloseButton = new wxButton(AnalyzeGraphGeneralPanel, wxID_ANY, _("Close"), wxDefaultPosition, wxDLG_UNIT(AnalyzeGraphGeneralPanel, wxSize(-1,-1)), 0);
	AnalyzeGraphGeneralGridSizer->Add(AnalyzeGraphCloseButton, 0, wxALL, WXC_FROM_DIP(5));
	AnalyzeGraphTopologyPanel = new wxPanel (AnalyzeGraphNotebook, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	AnalyzeGraphNotebook->AddPage (AnalyzeGraphTopologyPanel, _ ("Topology"), false);

	wxGridSizer* AnalyzeGraphTopologyGridSizer = new wxGridSizer (4, 2, 0, 0);
	AnalyzeGraphTopologyPanel->SetSizer (AnalyzeGraphTopologyGridSizer);

	AnalyzeGraphTopologyLayoutStaticText = new wxStaticText (AnalyzeGraphTopologyPanel, wxID_ANY, _ ("Layout"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyLayoutStaticText, 0, wxALL, 5);

	AnalyzeGraphTopologyLayoutTextCtrl = new wxTextCtrl (AnalyzeGraphTopologyPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphTopologyLayoutTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyLayoutTextCtrl, 0, wxALL|wxALIGN_RIGHT, 5);

	AnalyzeGraphTopologyPlanarStaticText = new wxStaticText (AnalyzeGraphTopologyPanel, wxID_ANY, _ ("Planar"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyPlanarStaticText, 0, wxALL, 5);

	AnalyzeGraphTopologyPlanarTextCtrl = new wxTextCtrl (AnalyzeGraphTopologyPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphTopologyPlanarTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyPlanarTextCtrl, 0, wxALL|wxALIGN_RIGHT, 5);

	AnalyzeGraphTopologyAcyclicStaticText = new wxStaticText (AnalyzeGraphTopologyPanel, wxID_ANY, _ ("Acyclic"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyAcyclicStaticText, 0, wxALL, 5);

	AnalyzeGraphTopologyAcyclicTextCtrl = new wxTextCtrl (AnalyzeGraphTopologyPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphTopologyAcyclicTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyAcyclicTextCtrl, 0, wxALL|wxALIGN_RIGHT, 5);

	if ((gdata.gtype == undir) || (gdata.gtype == undirw))
		AnalyzeGraphTopologyComponentsStaticText = new wxStaticText (AnalyzeGraphTopologyPanel, wxID_ANY, _ ("Components"), wxDefaultPosition, wxSize (-1,-1), 0);
	else
		AnalyzeGraphTopologyComponentsStaticText = new wxStaticText (AnalyzeGraphTopologyPanel, wxID_ANY, _ ("Strongly connected components"), wxDefaultPosition, wxSize (-1,-1), 0);

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyComponentsStaticText, 0, wxALL, 5);

	AnalyzeGraphTopologyComponentsTextCtrl = new wxTextCtrl (AnalyzeGraphTopologyPanel, wxID_ANY, wxT (""),
	      wxDefaultPosition, wxSize (75,-1), 0);
#if wxVERSION_NUMBER >= 3000
	AnalyzeGraphTopologyComponentsTextCtrl->SetHint (wxT (""));
#endif

	AnalyzeGraphTopologyGridSizer->Add (AnalyzeGraphTopologyComponentsTextCtrl, 0, wxALL|wxALIGN_RIGHT, 5);


#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (AnalyzeGraphNotebook))
	{
		wxPersistenceManager::Get().RegisterAndRestore (AnalyzeGraphNotebook);
	}
	else
	{
		wxPersistenceManager::Get().Restore (AnalyzeGraphNotebook);
	}

#endif

	SetName (wxT ("AnalyzeGraph"));
	SetSize(wxDLG_UNIT(this, wxSize(600,400)));

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	CentreOnParent (wxBOTH);
#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this))
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

	AnalyzeGraphCloseButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnalyzeGraph::OnAnalyzeGraphCloseButtonPressed), NULL, this);

#endif
	AnalyzeGraphNotebook->SetSelection(0);
	fillInTextFieldsWithData(gdata);
}

AnalyzeGraph::~AnalyzeGraph()
{
	AnalyzeGraphCloseButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnalyzeGraph::OnAnalyzeGraphCloseButtonPressed), NULL, this);
}

void AnalyzeGraph::OnAnalyzeGraphCloseButtonPressed(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void AnalyzeGraph::fillInTextFieldsWithData(const AnalyzeGraphData& graphData)
{
	wxString gtype, numVertices, numEdges, numSelfLoops, hasParallelEdges,
	         graphLayout, isPlanar, isAcyclic, numComponents;

	switch (graphData.gtype)
	{
	case undir:
		gtype = wxT("undirected");
		break;

	case undirw:
		gtype = wxT("weighted undirected");
		break;

	case digraph:
		gtype = wxT("digraph");
		break;

	case digraphw:
		gtype = wxT("weighted digraph");
		break;
	}

	numVertices.Printf(wxT("%d"), graphData.numVertices);
	numEdges.Printf(wxT("%d"), graphData.numEdges);
	numSelfLoops.Printf(wxT("%d"), graphData.numSelfLoops);
	hasParallelEdges = (graphData.parallelEdges) ? wxT("true") : wxT("false");

	switch (graphData.glayout)
	{
	case hierarchical:
		graphLayout = wxT("hierarchical");
		break;

	case forcedirected:
		graphLayout = wxT("forcedirected");
		break;

	case orthogonal:
		graphLayout = wxT("orthogonal");
		break;

	case circular:
		graphLayout = wxT("circular");
		break;
	}

	isPlanar = (graphData.planar) ? wxT("true") : wxT("false");
	isAcyclic = (graphData.acyclic) ? wxT("true") : wxT("false");
	numComponents.Printf(wxT("%d"), graphData.numComponents);

	AnalyzeGraphGeneralGraphTypeTextCtrl->SetValue(gtype);
	AnalyzeGraphGeneralNumberOfVerticesTextCtrl->SetValue(numVertices);
	AnalyzeGraphGeneralNumberOfEdgesTextCtrl->SetValue(numEdges);
	AnalyzeGraphGeneralNumberOfSelfLoopsTextCtrl->SetValue(numSelfLoops);
	AnalyzeGraphGeneralNumberOfParallelEdgesTextCtrl->SetValue(hasParallelEdges);
	AnalyzeGraphTopologyLayoutTextCtrl->SetValue(graphLayout);
	AnalyzeGraphTopologyPlanarTextCtrl->SetValue(isPlanar);
	AnalyzeGraphTopologyAcyclicTextCtrl->SetValue(isAcyclic);
	AnalyzeGraphTopologyComponentsTextCtrl->SetValue(numComponents);
}
