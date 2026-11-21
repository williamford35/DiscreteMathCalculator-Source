/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <functional>
#include "DiscreteMathCalculatorMain.h"
#include "graphs/algorithms/GraphAlgorithmCanvas.h"
#include "graphs/algorithms/search/bfs/BfsSearch.h"
#include "graphs/algorithms/search/dfs/DfsSearch.h"
#include "graphs/algorithms/search/dfs/MyGraphDfsVisitor.h"
#include "graphs/algorithms/search/dfs/MyGraphDfsPathVisitor.h"
#include "graphs/algorithms/search/bfs/MyGraphBfsPathVisitor.h"
#include "graphs/algorithms/search/bfs/MyGraphBfsVisitor.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/build/GraphEdgeStyles.h"
#include "graphs/GraphType.h"
#include "graphs/algorithms/hard-problems/Clique.h"
#include "graphs/algorithms/hard-problems/EnterNumberOfGraphColors.h"
#include "graphs/algorithms/Dijkstra/OptimumPathInput.h"
#include "graphs/algorithms/ShowSequence.h"
#include "graphs/algorithms/WidestPath/WidestPathDrawingCanvas.h"
#include "wx/string.h"
#include "DiscreteMathCalculatorMain.h"
#include "common/ImageFiles.h"
#include "common/report.h"

#include "common/colorPicker.h"
#include <lemon/euler.h>
#include <lemon/insertion_tsp.h>
#include <lemon/planarity.h>
#include <lemon/matching.h>
#include <lemon/kruskal.h>

// #include "images/discmathcalc.xpm"
#include "AboutDiscreteMathCalculator.h"

//(*InternalHeadersPCH(GraphAlgorithmCanvas)
#include <wx/intl.h>
#include <wx/string.h>
//*)
//(*InternalHeaders(GraphAlgorithmCanvas)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

#include <wx/colour.h>

#include <set>
#include <string>
#include <iostream>
using namespace std;

#include "NTL/tools.h"

NTL_CLIENT

//(*IdInit(GraphAlgorithmCanvas)
const long GraphAlgorithmCanvas::ID_GRAPH_DRAWING = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_FIGURE_SLIDER = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_FIGURE_PANEL = wxNewId();
const long GraphAlgorithmCanvas::ID_ALGORITHMS_STATICTEXT = wxNewId();
const long GraphAlgorithmCanvas::ID_ALGORITHM_CLASSES_COMBOBOX = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_ALGORITHMS_STATICTEXT = wxNewId();
const long GraphAlgorithmCanvas::ID_ALGORITHMS_COMBOBOX = wxNewId();
const long GraphAlgorithmCanvas::ID_RUN_ALGORITHM_BUTTON = wxNewId();
const long GraphAlgorithmCanvas::ID_ALGORITHM_CANVAS_CLOSE_BUTTON = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHMS_PANEL = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_DRAWING_CANVAS_PANEL = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_QUIT_MENUITEM = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_STATUSBAR = wxNewId();
//*)
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_PRINT_MENUITEM = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_PRINT_PREVIEW_MENUITEM = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_PAGESETUP_MENUITEM = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_SAVE_AS_MENUITEM = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM = wxNewId();
const long GraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (GraphAlgorithmCanvas,wxFrame)
	EVT_MENU (wxID_EXIT, GraphAlgorithmCanvas::OnQuit)
	EVT_CLOSE (GraphAlgorithmCanvas::OnCloseWindow)
END_EVENT_TABLE()

GraphAlgorithmCanvas::GraphAlgorithmCanvas (wxWindow* parent, wxSFDiagramManager mgr,
      double scale, ogdf::GraphAttributes *ga, ogdf::Graph *grph, wxWindowID id, const wxPoint& pos, const wxSize& size,
      GraphType gtype, BuildGraph *bg) :
	m_canvas (mgr), graphType (gtype), displayMaximumIndependentSet(true),
	G (grph),GA (ga),node2wxString (ug),
	edgeMap (ug), builtGraph (bg), myParent (parent), sortedWeights (ug)
{
	//(*Initialize(GraphAlgorithmCanvas)
	wxFlexGridSizer* graphDrawingCanvasFlexGridSizer;
	wxFlexGridSizer* graphFigureFlexGridSizer;
	wxFlexGridSizer* graphAlgorithmsFlexGridSizer;

	Create (parent, wxID_ANY, _ ("Graph Algorithms"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T ("wxID_ANY") );
	SetClientSize (wxSize (1024,768) );
	{
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("combcalc.xpm") ) ) );
		SetIcon (FrameIcon);
	}
	graphDrawingCanvasPanel = new wxPanel (this, ID_GRAPH_DRAWING_CANVAS_PANEL, wxPoint (432,208), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_GRAPH_DRAWING_CANVAS_PANEL") );
	graphDrawingCanvasFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	graphDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	graphDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	graphFigurePanel = new wxPanel (graphDrawingCanvasPanel, ID_GRAPH_FIGURE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_GRAPH_FIGURE_PANEL") );
	graphFigureFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	graphFigureFlexGridSizer->AddGrowableCol (0);
	graphFigureFlexGridSizer->AddGrowableRow (0);
	graphDrawing = new DrawingCanvas (&m_canvas, graphFigurePanel, ID_GRAPH_DRAWING, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL);
	graphFigureFlexGridSizer->Add (graphDrawing, 1, wxALL|wxEXPAND, 1);
	graphFigureSlider = new wxSlider (graphFigurePanel, ID_GRAPH_FIGURE_SLIDER, 50, 2, 99, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_GRAPH_FIGURE_SLIDER") );
	graphFigureFlexGridSizer->Add (graphFigureSlider, 1, wxALL|wxEXPAND, 1);
	graphFigurePanel->SetSizer (graphFigureFlexGridSizer);
	graphFigureFlexGridSizer->Fit (graphFigurePanel);
	graphFigureFlexGridSizer->SetSizeHints (graphFigurePanel);
	graphDrawingCanvasFlexGridSizer->Add (graphFigurePanel, 1, wxALL|wxEXPAND, 1);
	graphAlgorithmsPanel = new wxPanel (graphDrawingCanvasPanel, ID_GRAPH_ALGORITHMS_PANEL, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T ("ID_GRAPH_ALGORITHMS_PANEL") );
	graphAlgorithmsFlexGridSizer = new wxFlexGridSizer (1, 7, 0, 0);
	graphAlgorithmsFlexGridSizer->AddGrowableCol (0);
	graphAlgorithmsFlexGridSizer->AddGrowableCol (5);
	graphAlgorithmsFlexGridSizer->AddGrowableRow (0);
	graphAlgorithmsFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 1);
	algorithmsStaticText = new wxStaticText (graphAlgorithmsPanel, ID_ALGORITHMS_STATICTEXT, _ ("Algorithm Class"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ALGORITHMS_STATICTEXT") );
	graphAlgorithmsFlexGridSizer->Add (algorithmsStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	algorithmClassesComboBox = new wxComboBox (graphAlgorithmsPanel, ID_ALGORITHM_CLASSES_COMBOBOX, wxEmptyString, wxDefaultPosition, wxSize (172,25), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T ("ID_ALGORITHM_CLASSES_COMBOBOX") );
	algorithmClassesComboBox->Append (_ ("Graph Search") );
	algorithmClassesComboBox->Append (_ ("Connectivity") );
	algorithmClassesComboBox->Append (_ ("Minimum Path") );
	algorithmClassesComboBox->Append (_ ("Widest Path") );
	algorithmClassesComboBox->Append (_ ("Planarity") );
	algorithmClassesComboBox->Append (_ ("Maximum Indep. Edge Set") );
	algorithmClassesComboBox->Append (_ ("NP-complete Problems") );
	algorithmClassesComboBox->SetToolTip (_ ("Algorithms Arranged in Classes") );
	graphAlgorithmsFlexGridSizer->Add (algorithmClassesComboBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	graphAlgorithmCanvasAlgorithmStaticText = new wxStaticText (graphAlgorithmsPanel, ID_GRAPH_ALGORITHM_CANVAS_ALGORITHMS_STATICTEXT, _ ("Algorithm"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_GRAPH_ALGORITHM_CANVAS_ALGORITHMS_STATICTEXT") );
	graphAlgorithmsFlexGridSizer->Add (graphAlgorithmCanvasAlgorithmStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	algorithmsComboBox = new wxComboBox (graphAlgorithmsPanel, ID_ALGORITHMS_COMBOBOX, wxEmptyString, wxDefaultPosition, wxSize (260,25), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T ("ID_ALGORITHMS_COMBOBOX") );
	algorithmsComboBox->Append (_ ("BFS") );
	algorithmsComboBox->Append (_ ("DFS") );
	algorithmsComboBox->Append (_ ("Check for Cycle") );
	algorithmsComboBox->SetToolTip (_ ("The algorithms available for the current class.") );
	graphAlgorithmsFlexGridSizer->Add (algorithmsComboBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	runAlgorithmButton = new wxButton (graphAlgorithmsPanel, ID_RUN_ALGORITHM_BUTTON, _ ("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RUN_ALGORITHM_BUTTON") );
	runAlgorithmButton->SetDefault();
	graphAlgorithmsFlexGridSizer->Add (runAlgorithmButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	algorithmCanvasCloseButton = new wxButton (graphAlgorithmsPanel, ID_ALGORITHM_CANVAS_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ALGORITHM_CANVAS_CLOSE_BUTTON") );
	graphAlgorithmsFlexGridSizer->Add (algorithmCanvasCloseButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	graphAlgorithmsPanel->SetSizer (graphAlgorithmsFlexGridSizer);
	graphAlgorithmsFlexGridSizer->Fit (graphAlgorithmsPanel);
	graphAlgorithmsFlexGridSizer->SetSizeHints (graphAlgorithmsPanel);
	graphDrawingCanvasFlexGridSizer->Add (graphAlgorithmsPanel, 1, wxALL|wxEXPAND, 1);
	graphDrawingCanvasPanel->SetSizer (graphDrawingCanvasFlexGridSizer);
	graphDrawingCanvasFlexGridSizer->Fit (graphDrawingCanvasPanel);
	graphDrawingCanvasFlexGridSizer->SetSizeHints (graphDrawingCanvasPanel);

	graphAlgorithmCanvasMenuBar = new wxMenuBar();
	graphAlgorithmCanvasFileMenu = new wxMenu();
	graphAlgorithmCanvasFileMenu->Append (wxID_EXIT, wxGetStockLabel (wxID_EXIT) );
#ifndef __APPLE__
	graphAlgorithmCanvasMenuBar->Append (graphAlgorithmCanvasFileMenu, wxT ("&File") );
#endif

	graphAlgorithmCanvasHelpMenu = new wxMenu();
	graphAlgorithmCanvasHelpMenu->Append (wxID_ABOUT);
	graphAlgorithmCanvasHelpMenuItem = new wxMenuItem (graphAlgorithmCanvasHelpMenu, ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	graphAlgorithmCanvasHelpMenu->Append (graphAlgorithmCanvasHelpMenuItem);
	graphAlgorithmCanvasMenuBar->Append (graphAlgorithmCanvasHelpMenu, _ ("&Help") );

	SetMenuBar (graphAlgorithmCanvasMenuBar);
	graphAlgorithmStatusBar = new wxStatusBar (this, ID_GRAPH_ALGORITHM_STATUSBAR, 0, _T ("ID_GRAPH_ALGORITHM_STATUSBAR") );
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	graphAlgorithmStatusBar->SetFieldsCount (1,__wxStatusBarWidths_1);
	graphAlgorithmStatusBar->SetStatusStyles (1,__wxStatusBarStyles_1);
	SetStatusBar (graphAlgorithmStatusBar);

	Connect (ID_GRAPH_FIGURE_SLIDER,wxEVT_SCROLL_THUMBTRACK, (wxObjectEventFunction) &GraphAlgorithmCanvas::OngraphFigureSliderCmdScrollThumbTrack);
	Connect (ID_ALGORITHM_CLASSES_COMBOBOX,wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction) &GraphAlgorithmCanvas::OnalgorithmClassesComboBoxSelect);
	Connect (ID_RUN_ALGORITHM_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &GraphAlgorithmCanvas::OnrunAlgorithmButtonClick);
	Connect (ID_ALGORITHM_CANVAS_CLOSE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &GraphAlgorithmCanvas::OngraphAlgorithmCloseButtonPressed);
	Connect (ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &GraphAlgorithmCanvas::handleMenuChoice);
	Connect (ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (GraphAlgorithmCanvas::OnGraphAlgorithmCanvasHelpMenuItemSelected) );

	algorithmClassesComboBox->SetSelection (0);
	algorithmsComboBox->SetSelection (0);
	graphDrawing->SetScale (scale);
	graphDrawing->setSliderAddress (graphFigureSlider);
	graphFigureSlider->SetValue (50*scale);
	dirty = false;
	graphDrawing->getDirty (&dirty);
	tree_map = NULL;
	widestpath_map = NULL;

	graphDrawing->RemoveStyle (wxSFShapeCanvas::sfsMULTI_SELECTION);
	graphDrawing->RemoveStyle (wxSFShapeCanvas::sfsMULTI_SIZE_CHANGE);
	graphDrawing->RemoveStyle (wxSFShapeCanvas::sfsDND);
	graphDrawing->RemoveStyle (wxSFShapeCanvas::sfsHOVERING);
	graphDrawing->RemoveStyle (wxSFShapeCanvas::sfsUNDOREDO);
	graphDrawing->RemoveStyle (wxSFShapeCanvas::sfsHIGHLIGHTING);

	menuHandler = new MenuEventHandler (graphDrawing,
	                                    ID_GRAPH_ALGORITHM_CANVAS_PRINT_MENUITEM,
	                                    ID_GRAPH_ALGORITHM_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_GRAPH_ALGORITHM_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_GRAPH_ALGORITHM_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, graphType);

	// removeAndSaveGraphTitle(GA, G);
	buildGraphRepresentation();
	Centre();
}

GraphAlgorithmCanvas::~GraphAlgorithmCanvas()
{
	//(*Destroy(GraphAlgorithmCanvas)
	//*)
	if (tree_map != NULL)
		delete tree_map;

	if (widestpath_map != NULL)
		delete widestpath_map;
}

void GraphAlgorithmCanvas::OngraphAlgorithmCloseButtonPressed (wxCommandEvent &event)
{
	Destroy();
}

void GraphAlgorithmCanvas::OnQuit (wxCommandEvent& event)
{
	( (DiscreteMathCalculatorFrame *) myParent)->Close (true);
}

void GraphAlgorithmCanvas::OnCloseWindow (wxCloseEvent& event)
{
	Destroy();
}

void GraphAlgorithmCanvas::OnalgorithmClassesComboBoxSelect (wxCommandEvent& event)
{
	int selectedAlgorithmClass = algorithmClassesComboBox->GetCurrentSelection();

	algorithmsComboBox->Clear();

	switch (selectedAlgorithmClass)
	{
	case GraphSearch:
		algorithmsComboBox->Append (wxT ("BFS") );
		algorithmsComboBox->Append (wxT ("DFS") );
		algorithmsComboBox->Append (wxT ("Check for cycles") );
		break;

	case Connectivity:
		algorithmsComboBox->Append (wxT ("Connected") );
		algorithmsComboBox->Append (wxT ("Connected Components") );
		algorithmsComboBox->Append (wxT ("Euler Tour") );
		break;

	case MinimumPath:
		algorithmsComboBox->Append (wxT ("Single Source Single Destination") );
		algorithmsComboBox->Append (wxT ("Minimum Spanning Tree") );
		break;

	case WidestPath:
		algorithmsComboBox->Append (wxT ("Widest Path") );
		break;

	case Planarity:
		algorithmsComboBox->Append (wxT ("Planarity checking") );;
		break;

	case MaximumIndendentEdgeSet:
		algorithmsComboBox->Append (wxT ("Cardinality matching") );;
		algorithmsComboBox->Append (wxT ("Weighted matching") );;
		break;

	case NPHardProblems:
        algorithmsComboBox->Append(wxT("Maximum Clique"));
        algorithmsComboBox->Append(wxT("Independent Set/Vertex Cover"));
        algorithmsComboBox->Append(wxT("Hamiltonian Cycle"));
        algorithmsComboBox->Append(wxT("Hamiltonian Path"));
        algorithmsComboBox->Append(wxT("TSP"));
        algorithmsComboBox->Append(wxT("Graph Coloring"));
		break;
	}

	algorithmsComboBox->SetSelection (0);
}

void GraphAlgorithmCanvas::OnrunAlgorithmButtonClick (wxCommandEvent& event)
{
	int selectedAlgorithmClass = algorithmClassesComboBox->GetCurrentSelection();
	int selectedAlgorithm = algorithmsComboBox->GetCurrentSelection();
	int numberOfComponents;
	wxString msg;

	switch (selectedAlgorithmClass)
	{
	case GraphSearch:
		switch (selectedAlgorithm)
		{
		case BFS:
			graphBfs();
			break;

		case DFS:
			graphDfs();
			break;

		case CYCLE:
			if (lemon::acyclic (ug) )
				::report (wxT ("The graph has no cycles"),"Note");
			else
				::report (wxT ("The graph has a cycle"),"Note" );

			break;
		}

		break;

	case Connectivity:
		switch (selectedAlgorithm)
		{
		case CONNECTED:
			numberOfComponents = countConnectedComponents (ug);

			if (numberOfComponents == 1)
				::report (wxT ("The graph is connected."),"Note");
			else
			{
				msg.Printf (wxT ("The graph has %d components."), numberOfComponents);
				::report (msg,"Note");
			}

			break;

		case CONNECTED_COMPONENTS:
			drawComponents();
			break;

		case EULER_TOUR:
			eulerTour (ug, ug.nodeFromId (0) );
			break;
		}

		break;

	case MinimumPath:
		switch (selectedAlgorithm)
		{
		case DIJKSTRA:
			Dijkstra();
			break;

		case PRIM:
			Prim();
			break;
		}

		break;

	case WidestPath:
		FindWidestPath();
		break;

	case Planarity:
		isPlanar();
		break;

	case MaximumIndendentEdgeSet:
		switch (selectedAlgorithm)
		{
		case MAXIMUMCARDINALITY:
			drawMaxIndependentEdgeSetCardinalityMatching();
			break;

		case MAXIMUMWEIGHTED:
			drawMaxIndependentEdgeSetWeightedMatching();
			break;
		}

		break;

	case NPHardProblems:
		switch (selectedAlgorithm)
		{
		case MAXIMUM_CLIQUE:
			drawMaximumClique();
			break;

		case INDEPENDENT_SET_VERTEX_COVER:
			displayMaximumIndependentSet = true;
			drawMaximumIndependentSet();
			break;

		case HAMILTONIAN_CYCLE:
			drawHamiltonianCycle();
			break;

        case HAMILTONIAN_PATH:
            drawHamiltonianPath();
            break;

		case TSP:
			solveTSP();
			break;

		case GRAPH_COLORING:
			beginColoring();
			break;
		}

		break;
	}
}

void GraphAlgorithmCanvas::OngraphFigureSliderCmdScrollThumbTrack (wxScrollEvent& event)
{
	graphDrawing->SetScale (double (graphFigureSlider->GetValue() ) /50);
	graphDrawing->Refresh (false);
}

void GraphAlgorithmCanvas::OnGraphAlgorithmCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}

void GraphAlgorithmCanvas::OngraphAlgorithmAboutMenuItemSelected (wxCommandEvent& event)
{
	try
	{
		AboutDiscreteMathCalculator aboutDiscreteMathCalculator (this, 0);

		aboutDiscreteMathCalculator.ShowModal();
	}

	catch (FileErrorObject&)
	{
		return;
	}
}

void GraphAlgorithmCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,4);
}

void GraphAlgorithmCanvas::buildGraphRepresentation()
{
	ogdf::List<node> vertices;
	ogdf::List<node>::iterator vertexIterator;
	ogdf::List<edge> edges;
	ogdf::List<edge>::iterator edgeIterator;
	wxString u, v;
	lemon::ListGraph::Node node_u, node_v;
	lemon::ListGraph::Edge e;
	wxString vertexLabel, edgeLabel;
	bool addEdgeWeights = false;
	double weight;

	G->allNodes (vertices);

	for (vertexIterator = vertices.begin(); vertexIterator != vertices.end(); vertexIterator++)
	{
		node_u = ug.addNode();
		vertexLabel = wxString (GA->label (*vertexIterator).c_str(), wxConvUTF8);
		node2wxString[node_u] = vertexLabel;
		wxString2Node[vertexLabel] = node_u;
	}

	if (graphType == undirw)
		addEdgeWeights = true;

	G->allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		u = wxString (GA->label ( (**edgeIterator).source() ).c_str(), wxConvUTF8);
		v = wxString (GA->label ( (**edgeIterator).target() ).c_str(), wxConvUTF8);

		if (addEdgeWeights)
		{
			edgeLabel = wxString (GA->label (*edgeIterator).c_str(), wxConvUTF8);
			edgeLabel.ToDouble (&weight);
		}
		else
			weight = 1;

		node_u = wxString2Node[u];
		node_v = wxString2Node[v];
		e = ug.addEdge (node_u, node_v);
		edgeMap[e] = weight;
	}
}

void GraphAlgorithmCanvas::graphBfs()
{
	wxString *vertex = new wxString[2];
	lemon::ListGraph::Node s, t;
	map<wxString, lemon::ListGraph::Node>::iterator iter;
	wxString output, s_str, t_str;
	bool from_s_only = false;

	vertex[0] = vertex[1] = wxT ("");

	BfsSearch searchData (this, vertex, graphType);

	if (searchData.ShowModal() == wxID_CANCEL)
	{
		delete[] vertex;
		return;
	}

	if (vertex[1] == wxT ("") )
		from_s_only = true;

	if (from_s_only)
	{
		vector<ListGraph::Node> treeNodes;

		s_str = vertex[0];
		treeNodes = checkAndArrange (wxString2Node[s_str], vertex[0]);

		if (treeNodes.size() == 0)
			return;

		MyGraphBfsVisitor visitor (myParent, &node2wxString,  &edgeMap, &ug,
		                           graphType, forcedirected, DisplayAlgorithmResults::BFSTREE,
		                           true,NULL,wxT (""),s_str);
		lemon::BfsVisit<lemon::ListGraph, MyGraphBfsVisitor> myBfsVisit (ug, visitor);

		myBfsVisit.init();

		for (unsigned int i=0; i < treeNodes.size(); i++)
			if (!myBfsVisit.reached (treeNodes[i]) )
			{
				myBfsVisit.addSource (treeNodes[i]);
				myBfsVisit.start();
			}

		visitor.drawTree();
	}
	else
	{
		lemon::Bfs<lemon::ListGraph> mybfs (ug);
		map<lemon::ListGraph::Edge, bool> bfsPath;
		vector<ListGraph::Node> treeNodes;
		bool success = false;
		wxString distanceStr;
		int distance;

		s = wxString2Node[vertex[0]];
		treeNodes = checkAndArrange (s, vertex[0]);

		if (treeNodes.size() == 0)
			return;

		t = wxString2Node[vertex[1]];

		if (!check (t,vertex[1]) )
			return;

		mybfs.run (s);
		success = false;

		for (unsigned int i=0; i < treeNodes.size(); i++)
		{
			if (mybfs.reached (treeNodes[i]) )
			{
				if (treeNodes[i] == t)
				{
					distance = mybfs.dist (t);
					lemon::ListGraph::Edge prevEdge;
					lemon::ListGraph::Node prevNode = t;

					success = true;
					prevEdge = (lemon::ListGraph::Edge) (mybfs.predArc (t) );

					if (prevEdge != lemon::INVALID)
						bfsPath[prevEdge] = true;

					prevNode = mybfs.predNode (prevNode);

					while (prevNode != lemon::INVALID)
					{
						if (prevEdge != lemon::INVALID)
						{
							bfsPath[prevEdge] = true;
							prevEdge = (lemon::ListGraph::Edge) (mybfs.predArc (prevNode) );
						}

						prevNode = mybfs.predNode (prevNode);
					}

					break;
				}
				else
				{
					mybfs.addSource (treeNodes[i]);
					mybfs.start();
				}
			}
		}

		if (!success)
		{
			::report (wxT ("The requested path does not exist."),"Note");
			return;
		}

		distanceStr.Printf (wxT ("The distance from %s to %s is %d"),
		                    vertex[0].c_str(), vertex[1].c_str(), distance);
		MyGraphBfsPathVisitor pathVisitor (myParent, &node2wxString, &edgeMap, &ug, graphType, bfsPath, distanceStr);
		lemon::BfsVisit<lemon::ListGraph, MyGraphBfsPathVisitor> myBfsPathVisit (ug, pathVisitor);

		myBfsPathVisit.init();

		for (unsigned int i=0; i < treeNodes.size(); i++)
			if (!myBfsPathVisit.reached (treeNodes[i]) )
			{
				myBfsPathVisit.addSource (treeNodes[i]);
				myBfsPathVisit.start();
			}

		pathVisitor.drawTree();
	}

	delete[] vertex;
}

void GraphAlgorithmCanvas::graphDfs()
{
	wxString *vertex = new wxString[2];
	lemon::ListGraph::Node s, t;
	map<wxString, lemon::ListGraph::Node>::iterator iter;
	wxString output, s_str, t_str;
	bool from_s_only = false;

	vertex[0] = vertex[1] = wxT ("");
	DfsSearch searchData (this, vertex, graphType);

	if (searchData.ShowModal() == wxID_CANCEL)
	{
		delete[] vertex;
		return;
	}

	if (vertex[1] == wxT ("") )
		from_s_only = true;

	if (from_s_only)
	{
		vector<ListGraph::Node> treeNodes;

		s_str = vertex[0];
		treeNodes = checkAndArrange (wxString2Node[s_str], s_str);

		if (treeNodes.size() == 0)
			return;

		MyGraphDfsVisitor visitor (myParent, algorithmOutput, &node2wxString, &edgeMap, &ug, graphType);
		lemon::DfsVisit<lemon::ListGraph, MyGraphDfsVisitor> myDfsVisit (ug, visitor);

		myDfsVisit.init();

		for (unsigned int i=0; i < treeNodes.size(); i++)
			if (!myDfsVisit.reached (treeNodes[i]) )
			{
				myDfsVisit.addSource (treeNodes[i]);
				myDfsVisit.start();
			}

		visitor.generateOutput (true);
		myDfsVisit.init();

		for (unsigned int i=0; i < treeNodes.size(); i++)
			if (!myDfsVisit.reached (treeNodes[i]) )
			{
				myDfsVisit.addSource (treeNodes[i]);
				myDfsVisit.start();
			}

		visitor.drawTree();
	}
	else
	{
		lemon::Dfs<lemon::ListGraph> mydfs (ug);
		map<lemon::ListGraph::Edge, bool> dfsPath;
		vector<ListGraph::Node> treeNodes;
		bool success = false;

		s = wxString2Node[vertex[0]];
		treeNodes = checkAndArrange (s, vertex[0]);

		if (treeNodes.size() == 0)
			return;

		t = wxString2Node[vertex[1]];

		if (!check (t, vertex[1]) )
			return;

		mydfs.run (s);
		success = false;

		for (unsigned int i=0; i < treeNodes.size(); i++)
		{
			if (mydfs.reached (treeNodes[i]) )
			{
				if (treeNodes[i] == t)
				{
					lemon::ListGraph::Edge prevEdge;
					lemon::ListGraph::Node prevNode = t;

					success = true;
					prevEdge = (lemon::ListGraph::Edge) mydfs.predArc (t);

					if (prevEdge != lemon::INVALID)
						dfsPath[prevEdge] = true;

					prevNode = mydfs.predNode (prevNode);

					while (prevNode != lemon::INVALID)
					{
						if (prevEdge != lemon::INVALID)
						{
							dfsPath[prevEdge] = true;
							prevEdge = (lemon::ListGraph::Edge) mydfs.predArc (prevNode);
						}

						prevNode = mydfs.predNode (prevNode);
					}

					break;
				}
				else
				{
					mydfs.addSource (treeNodes[i]);

					while (!mydfs.emptyQueue() )
						mydfs.processNextArc();
				}
			}
		}

		if (!success)
		{
			::report (wxT ("The requested path does not exist."),"Note");
			return;
		}

		MyGraphDfsPathVisitor pathVisitor (myParent, &node2wxString, &edgeMap, &ug, dfsPath, graphType);
		lemon::DfsVisit<lemon::ListGraph, MyGraphDfsPathVisitor> myDfsPathVisit (ug, pathVisitor);

		myDfsPathVisit.init();

		for (unsigned int i=0; i < treeNodes.size(); i++)
			if (!myDfsPathVisit.reached (treeNodes[i]) )
			{
				myDfsPathVisit.addSource (treeNodes[i]);
				myDfsPathVisit.start();
			}

		pathVisitor.drawTree();
	}

	delete[] vertex;
}


vector<lemon::ListGraph::Node> GraphAlgorithmCanvas::checkAndArrange (lemon::ListGraph::Node t,
      const wxString& vertexLabel)
{
	wxString output;
	vector<ListGraph::Node> treeNodes;
	bool success = false;

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		if (n == t)
		{
			success = true;

			if (treeNodes.empty() )
				treeNodes.push_back (t);
			else
			{
				treeNodes.push_back (treeNodes[0]);
				treeNodes[0] = t;
			}
		}
		else
			treeNodes.push_back (n);
	}

	if (!success)
	{
		output.Printf (wxT ("%s is not a vertex."), vertexLabel.c_str() );
		::report (output,"Note");
		treeNodes.clear();
	}

	return treeNodes;
}

bool GraphAlgorithmCanvas::check (lemon::ListGraph::Node t, const wxString& vertexLabel)
{
	wxString output;
	bool success = false;

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
		if (n == t)
		{
			success = true;
			break;
		}

	if (!success)
	{
		output.Printf (wxT ("%s is not a vertex."), vertexLabel.c_str() );
		::report (output,"Note");
	}

	return success;
}

void GraphAlgorithmCanvas::drawComponents()
{
	int numComp;
	lemon::ListGraph::NodeMap<int> compMap (ug);
	lemon::ListGraph::NodeMap<wxColour> colorMap (ug);
	wxString msg;
	bool exceededNumberOfColors = false;

	numComp = connectedComponents (ug, compMap);

	if (numComp >= 16)
	{
		::report(wxT("This application provides a maximum of 16 colors."),
		         wxT("Note"));
		exceededNumberOfColors = true;
	}
	vector<wxColour> componentColor;
	colorPicker componentColors;

	componentColors.PickColor (componentColor, numComp);

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		if (exceededNumberOfColors)
			colorMap[n] = wxColour(255,255,255);
		else
			colorMap[n] = componentColor[compMap[n]];
	}

	if (numComp != 1)
		msg.Printf(wxT("There are %d components."), numComp);
	else
		msg.Printf(wxT("There is %d component."), numComp);

	MyGraphBfsVisitor visitor (myParent, &node2wxString, &edgeMap, &ug, graphType, hierarchical,
	                           DisplayAlgorithmResults::COMPONENTS, false,
	                           &colorMap, msg);
	lemon::BfsVisit<lemon::ListGraph, MyGraphBfsVisitor> myBfsVisit (ug, visitor);

	myBfsVisit.init();

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
		if (!myBfsVisit.reached (n) )
		{
			myBfsVisit.addSource (n);
			myBfsVisit.start();
		}

	visitor.drawTree();
}

void GraphAlgorithmCanvas::eulerTour (const ListGraph& g,
                                      const ListGraph::Node& start)
{
	ListGraph::EdgeMap<int> visitationNumber (g, 0);
	vector<ListGraph::Node> nodeSequence;

	EulerIt<ListGraph> e (g, start);

	if (e == INVALID)
		return;

	ListGraph::Node firstNode = g.source (ListGraph::Arc (e) );
	ListGraph::Node lastNode = g.target (ListGraph::Arc (e) );

	if (start == INVALID)
	{
		if (firstNode != start)
		{
			::report (wxT ("Incorrect first node."),"Note");
			return;
		}
	}

	nodeSequence.push_back (firstNode);

	for (; e != INVALID; ++e)
	{
		if (e != INVALID)
		{
			lastNode = g.target (ListGraph::Arc (e) );
			nodeSequence.push_back (lastNode);
		}

		++visitationNumber[e];
	}

	if (firstNode != lastNode)
	{
		::report (wxT ("There is no Euler tour."),"Note");
		return;
	}

	for (ListGraph::EdgeIt e (g); e != INVALID; ++e)
	{
		if (visitationNumber[e] != 1)
		{
			::report (wxT ("Edge not visited or an edge visited multiple times."),"Note");
			return;
		}
	}

	drawEulerTour (nodeSequence);
}

void GraphAlgorithmCanvas::drawEulerTour (const vector<ListGraph::Node>& nodeList)
{
	BuildGraph *myEulerTour;
	wxString src, dest;
	DisplayAlgorithmResults *myEulerCircuit;
	int edgeNumber = 1;
	wxString edgeLabel, temp;

	myEulerTour = new BuildGraph (wxT (""), undirw, hierarchical, true);

	for (unsigned int i=0; i < nodeList.size()-1; i++)
	{
		src = node2wxString[nodeList[i]];
		dest = node2wxString[nodeList[i+1]];
		edgeLabel.Printf (wxT ("%d"), edgeNumber);
		edgeNumber++;
		myEulerTour->insertEdge (src, dest, edgeLabel, wxwxDASHED_GRAPH_EDGE_PEN,
		                         wxColour(255,215,0), wxColour(255,215,0));
	}

	myEulerTour->hierarchicalLayout();
	myEulerCircuit = new DisplayAlgorithmResults (myParent, myEulerTour, undirw, hierarchical,
	      DisplayAlgorithmResults::EULERTOUR);

	for (unsigned int i=0; i < nodeList.size()-1; i++)
	{
		src = node2wxString[nodeList[i]];
		dest = node2wxString[nodeList[i+1]];
		temp.Printf (wxT (" %s-%s  "), src, dest);
		myEulerCircuit->writeOutputLine (temp);
	}

	myEulerCircuit->Show();
	wxTheApp ->SetTopWindow (myEulerCircuit);

	delete myEulerTour;
}

vector<int> GraphAlgorithmCanvas::searchForMaximumClique (const ListGraph& g,
      map<int, ListGraph::Node>& intToNode,
      map<ListGraph::Node, int>& nodeToInt)
{
	map<ListGraph::Edge,bool> edgeOutput;
	int id = 1;
	unsigned int i;
	string dimacs = "p edge ", numNodes, numEdges;
	stringstream out;
	ListGraph::Node nodeS, nodeT;
	ListGraph::Edge edge;
	string currNodeId;
	vector<int> maximumClique;

	numNodes = intToStr (countNodes (g) );
	numEdges = intToStr (countEdges (g) );
	dimacs += numNodes + " " + numEdges + "\n";

	for (ListGraph::NodeIt n (g); n != INVALID; ++n)
	{
		intToNode[id] = n;
		nodeToInt[n] = id;
		id++;
	}

	for (i=0; i < intToNode.size(); i++)
	{
		nodeS = intToNode[i+1];
		currNodeId = intToStr (i+1);

		for (ListGraph::IncEdgeIt e (g, nodeS); e != INVALID; ++e)
		{
			if (!edgeOutput[e])
			{
				nodeT = g.target (e);
				dimacs += string ("e ") + currNodeId + " " + intToStr (nodeToInt[nodeT]) + "\n";
				edgeOutput[e] = true;
			}
		}
	}

	Clique myClique (dimacs, 0.025);

	myClique.findMaximumClique();
	maximumClique = myClique.maximumClique();

	return maximumClique;
}

void GraphAlgorithmCanvas::drawMaximumIndependentSet()
{
	map<ListGraph::Node,ListGraph::Node> ugToComp;
	lemon::ListGraph::NodeMap<wxString> compNodeMap (compGraph);
	ListGraph::Node newNode;
	ListGraph::Node uc, vc;
	bool isEdge;
	// wxString maxIndependentSetLabel, counts;
	vector<int> maximumClique;
	map<int, ListGraph::Node> intToNode;
	map<ListGraph::Node, int> nodeToInt;
	std::set<wxString> independentSetNode;
	wxString independentSetListing,vertexCoverListing,
	         tempResultIndepSet, tempResultVertexCover;
	ListGraph::Node src, dest;
	DisplayAlgorithmResults *myMaxIndependentSetGraph;
	int maxIndependentSetSize = 0, vertexCoverSize = 0;
	unsigned int i;

	indepSetVertex.clear();
	vertexCoverVertex.clear();

	for (ListGraph::NodeIt u (ug); u != INVALID; ++u)
	{
		newNode = compGraph.addNode();
		ugToComp[u]= newNode;
		compNodeMap[newNode]= node2wxString[u];
	}

	for (ListGraph::NodeIt u (ug); u != INVALID; ++u)
	{
		uc = ugToComp[u];

		for (ListGraph::NodeIt v (ug); v != INVALID; ++v)
		{
			isEdge = false;

			if (u != v)
			{
				if (findEdge (ug,u,v) != INVALID)
				{
					isEdge = true;
				}

				if (!isEdge)
				{
					vc = ugToComp[v];

					if (findEdge (compGraph, vc, uc) == INVALID)
						compGraph.addEdge (uc, vc);
				}
			}
		}
	}

	maximumClique = searchForMaximumClique (compGraph, intToNode, nodeToInt);

	for (i=0; i < maximumClique.size()-1; i++)
	{
		wxString str = compNodeMap[intToNode[maximumClique[i]+1]];
		independentSetNode.insert (compNodeMap[intToNode[maximumClique[i]+1]]);
	}

	myMaxIndependentSet = new BuildGraph (wxT (""), undir, circular, true);

	for (ListGraph::NodeIt u (ug); u != INVALID; ++u)
	{
		if (independentSetNode.find (node2wxString[u]) != independentSetNode.end() )
		{
			myMaxIndependentSet->insertEdge (node2wxString[u], wxT ("#"), wxT (""), wxNORMAL_PEN,
			                                 wxColour (137, 172, 255),wxColour (137, 172, 255 ));
			indepSetVertex.push_back (node2wxString[u]);
			maxIndependentSetSize++;
		}
		else
		{
			myMaxIndependentSet->insertEdge (node2wxString[u], wxT ("#"), wxT (""), wxNORMAL_PEN,
			                                 wxColour(192,192,192), wxColour(192,192,192));
			vertexCoverVertex.push_back(node2wxString[u]);
			vertexCoverSize++;
		}
	}

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		src = ug.u (e);
		dest = ug.v (e);
		myMaxIndependentSet->insertEdge (node2wxString[src],
		                                 node2wxString[dest],
		                                 wxT (""), wxNORMAL_PEN,
		                                 wxColour(255,215,0), wxColour(255,215,0));
	}
	// int maxIndependentSetSize = 0, vertexCoverSize = 0;

	myMaxIndependentSet->circularLayout();
	if (displayMaximumIndependentSet)
	{
		// maxIndependentSize = maximumClique[maximumClique.size()-1];
		myMaxIndependentSetGraph = new DisplayAlgorithmResults (myParent, myMaxIndependentSet, undir, circular,
		      DisplayAlgorithmResults::INDEPENDENTSET);

		independentSetListing += wxT("Maximum independent set: ");
		for (i = 0; i < indepSetVertex.size(); i++)
		{
			tempResultIndepSet.Printf (wxT ("%s  "), indepSetVertex[i].c_str() );
			independentSetListing += tempResultIndepSet;
		}

		vertexCoverListing += wxT("Vertex cover set: ");
		for (i = 0; i < vertexCoverVertex.size(); i++)
		{
			tempResultVertexCover.Printf (wxT ("%s  "), vertexCoverVertex[i].c_str() );
			vertexCoverListing += tempResultVertexCover;
		}

		independentSetListing += wxT ("\n");
		tempResultIndepSet.Printf (wxT ("%d"), maxIndependentSetSize);
		independentSetListing += wxT ("Maximum independent set size: ") + tempResultIndepSet + wxT ("\n");
		myMaxIndependentSetGraph->writeOutputLine (independentSetListing);

		vertexCoverListing += wxT ("\n");
		tempResultVertexCover.Printf (wxT ("%d"), vertexCoverSize);
		vertexCoverListing += wxT ("Vertex cover set size: ") + tempResultVertexCover + wxT ("\n");
		myMaxIndependentSetGraph->writeOutputLine (vertexCoverListing);

		myMaxIndependentSetGraph->Show();
		wxTheApp ->SetTopWindow (myMaxIndependentSetGraph);
	}
}

bool GraphAlgorithmCanvas::findIndependentSetNode(const vector<wxString>&  v, const wxString& n)
{
	for (int i=0; i<v.size(); i++)
		if (v[i] == n)
			return true;
	return false;
}

void GraphAlgorithmCanvas::drawMaximumClique()
{
	map<int, ListGraph::Node> intToNode;
	map<ListGraph::Node, int> nodeToInt;
	wxString tempResult;
	wxString cliqueGraphLabel, temp;
	BuildGraph *myMaxClique;
	DisplayAlgorithmResults *myMaxCliqueGraph;
	ListGraph::Node src, dest;
	int srcIndex, destIndex;
	vector<int> maximumClique;
	vector<wxString> vertexInClique;
	bool srcInClique, destInClique;

	maximumClique = searchForMaximumClique (ug, intToNode, nodeToInt);

	/*if (originalTitle.length() != 0)
		cliqueGraphLabel += wxString(originalTitle.c_str()) + "\n" + wxT ("Maximum clique: ");
	else
		cliqueGraphLabel += wxT("Maximum clique: ");*/

	/*for (unsigned int i = 0; i < maximumClique.size()-1; i++)
	{
		tempResult.Printf (wxT ("%s  "), node2wxString[intToNode[maximumClique[i]+1]].c_str() );
		cliqueGraphLabel += tempResult;
	}

	cliqueGraphLabel += wxT ("\n");
	tempResult.Printf (wxT ("%d"), maximumClique[maximumClique.size()-1]);
	cliqueGraphLabel += wxT ("Maximum clique size: ") + tempResult + wxT ("\n");*/

	myMaxClique = new BuildGraph (wxT (""), undir, circular, true);

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		if (inVector (maximumClique, nodeToInt[n]-1) )
		{
			myMaxClique->insertEdge (node2wxString[n], wxT ("#"), wxT (""), wxNORMAL_PEN, wxColour (137, 172, 255) );
			vertexInClique.push_back (node2wxString[n]);
		}
		else
			myMaxClique->insertEdge (node2wxString[n], wxT ("#"), wxT (""), wxNORMAL_PEN,
			                         wxColour(255,215,0), wxColour(255,215,0));
	}

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		src = ug.u (e);
		dest = ug.v (e);
		srcIndex = nodeToInt[src];
		destIndex = nodeToInt[dest];
		srcInClique = inVector (maximumClique, srcIndex-1);
		destInClique = inVector (maximumClique, destIndex-1);

		if (srcInClique && destInClique && destIndex != srcIndex)
		{
			myMaxClique->insertEdge (node2wxString[src],
			                         node2wxString[dest],
			                         wxT (""), wxTREE_EDGE_PEN,
			                         wxColour(255,215,0), wxColour(255,215,0));
		}
		else
			myMaxClique->insertEdge (node2wxString[src],
			                         node2wxString[dest],
			                         wxT (""), wxNORMAL_PEN,
			                         wxColour(255,215,0), wxColour(255,215,0));
	}

	myMaxClique->circularLayout();
	myMaxCliqueGraph = new DisplayAlgorithmResults (myParent, myMaxClique, undir, circular,
	      DisplayAlgorithmResults::CLIQUE);
	myMaxCliqueGraph->writeOutputLine (wxT ("Maximum clique:\n") );

	for (int i=0; i < vertexInClique.size(); i++)
	{
		temp.Printf (wxT ("%s  "), vertexInClique[i]);
		myMaxCliqueGraph->writeOutputLine (temp);
	}

	myMaxCliqueGraph->Show();
	wxTheApp->SetTopWindow (myMaxCliqueGraph);

	delete myMaxClique;
}

bool GraphAlgorithmCanvas::cycleSearch (ListGraph::Node sVertex, ListGraph::Node currVertex,
                                        vector<wxString>& cycleVertices,
                                        int d)
{
	ListGraph::Node neighborVertex;

	if (currVertex == sVertex)
	{
		if (d == 0)
		{
			cycleVertices.push_back (node2wxString[currVertex]);
			return true;
		}
		else
			return false;
	}

	color[currVertex] = GraphAlgorithmCanvas::BLACK;

	for (ListGraph::OutArcIt a (ug, currVertex); a != INVALID; ++a)
	{
		neighborVertex = ug.target (a);

		if (color[neighborVertex] == GraphAlgorithmCanvas::WHITE)
			if (cycleSearch (sVertex,neighborVertex,cycleVertices,d-1) )
			{
				cycleVertices.push_back (node2wxString[currVertex]);
				return true;
			}
	}

	color[currVertex] = GraphAlgorithmCanvas::WHITE;
	return false;
}

bool GraphAlgorithmCanvas::lookForHamiltonianCycle (const ListGraph& g,
      vector<wxString>& cycleVertices)
{
	ListGraph::NodeIt firstNode (ug);
	ListGraph::Node neighborNode;

	if (lemon::countNodes (ug) == 1)
	{
		cycleVertices.push_back (node2wxString[firstNode]);
		return true;
	}

	// color all the vertices WHITE
	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		color[n] = GraphAlgorithmCanvas::WHITE;
	}

	// start off by iterating through the edges incident to firstNode
	for (ListGraph::OutArcIt a (ug, firstNode); a != INVALID; ++a)
	{
		neighborNode = ug.target (a);

		if (cycleSearch (firstNode, neighborNode, cycleVertices,
		                 lemon::countNodes (ug)-1) )
		{
			cycleVertices.push_back (node2wxString[firstNode]);
			return true;
		}
	}

	return false;
}

void GraphAlgorithmCanvas::drawHamiltonianCycle()
{
	wxString tempResult, vertexListing, srcLabel, destLabel;
	BuildGraph *hamCycle;
	DisplayAlgorithmResults *hamiltonianCycleGraph;
	ListGraph::Node src, dest;
	vector<wxString> hamiltonianCycle;
	bool haveHamiltonEdge;

	if (!lookForHamiltonianCycle (ug, hamiltonianCycle) )
	{
		::report (wxT ("There is no Hamiltonian cycle."),"Note");
		return;
	}

	hamiltonianCycle.push_back (hamiltonianCycle[0]);
	hamCycle = new BuildGraph (wxT (""), undir, forcedirected, true);

	for (ListGraph::ArcIt a (ug); a != INVALID; ++a)
	{
		src = ug.source (a);
		dest = ug.target (a);
		srcLabel = node2wxString[src];
		destLabel = node2wxString[dest];
		haveHamiltonEdge = edgeInCycle (hamiltonianCycle, srcLabel, destLabel);

		if (haveHamiltonEdge)
			hamCycle->insertEdge (node2wxString[src],
			                      node2wxString[dest],
			                      wxT (""), wxTREE_EDGE_PEN,
			                      wxColour(255,215,0), wxColour(255,215,0));
		else
			hamCycle->insertEdge (node2wxString[src],
			                      node2wxString[dest],
			                      wxT (""), wxwxDASHED_GRAPH_EDGE_PEN,
			                      wxColour(255,215,0), wxColour(255,215,0));
	}

	hamCycle->circularLayout();
	hamiltonianCycleGraph = new DisplayAlgorithmResults (myParent, hamCycle, undir, circular,
	      DisplayAlgorithmResults::HAMILTONIAN_CYCLE);

	vertexListing = wxT ("Hamiltonian cycle: ");

	for (unsigned int i = 0; i < hamiltonianCycle.size()-1; i++)
	{
		tempResult.Printf (wxT ("%s  "), hamiltonianCycle[i]);
		vertexListing += tempResult;
	}

	hamiltonianCycleGraph->writeOutputLine(vertexListing);
	hamiltonianCycleGraph->Show();
	wxTheApp ->SetTopWindow (hamiltonianCycleGraph);

	delete hamCycle;
}

template <typename T>
bool GraphAlgorithmCanvas::inVector (const vector<T>& v, T target)
{
	for (unsigned int i=0; i < v.size()-1; i++)
		if (v[i] == target)
			return true;

	return false;
}


void GraphAlgorithmCanvas::drawHamiltonianPath()
{
    typedef lemon::ListGraph::Node Node;
    const int count = lemon::countNodes(ug);
    if (count == 0)
    {
        ::report(wxT("The graph has no vertices."), "Note");
        return;
    }

    lemon::ListGraph::NodeMap<bool> visited(ug, false);
    std::vector<Node> path;

    // Backtracking: try every vertex as a possible starting point.
    std::function<bool(Node)> search = [&](Node current) -> bool
    {
        visited[current] = true;
        path.push_back(current);
        if (static_cast<int>(path.size()) == count)
            return true;

        for (lemon::ListGraph::OutArcIt a(ug, current);
             a != lemon::INVALID; ++a)
        {
            Node next = ug.target(a);
            if (!visited[next] && search(next))
                return true;
        }
        path.pop_back();
        visited[current] = false;
        return false;
    };

    bool found = false;
    for (lemon::ListGraph::NodeIt start(ug);
         start != lemon::INVALID; ++start)
    {
        if (search(start))
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        ::report(wxT("There is no Hamiltonian path."), "Note");
        return;
    }

    std::vector<wxString> names;
    for (Node v : path)
        names.push_back(node2wxString[v]);

    BuildGraph *graph = new BuildGraph(wxT(""), undir, circular, true);
    // Include isolated vertices, including a one-vertex graph.
    for (lemon::ListGraph::NodeIt n(ug); n != lemon::INVALID; ++n)
        graph->insertEdge(node2wxString[n], wxT("#"), wxT(""),
                          wxNORMAL_PEN, wxColour(255,215,0), wxColour(255,215,0));

    for (lemon::ListGraph::EdgeIt e(ug); e != lemon::INVALID; ++e)
    {
        const wxString a = node2wxString[ug.u(e)];
        const wxString b = node2wxString[ug.v(e)];
        bool inPath = false;
        for (size_t i = 0; i + 1 < names.size(); ++i)
        {
            if ((names[i] == a && names[i+1] == b) ||
                (names[i] == b && names[i+1] == a))
            {
                inPath = true;
                break;
            }
        }
        graph->insertEdge(a, b, wxT(""),
                          inPath ? wxTREE_EDGE_PEN : wxwxDASHED_GRAPH_EDGE_PEN,
                          wxColour(255,215,0), wxColour(255,215,0));
    }
    graph->circularLayout();
    DisplayAlgorithmResults *window = new DisplayAlgorithmResults(
        myParent, graph, undir, circular, DisplayAlgorithmResults::HAMILTONIAN_PATH);
    wxString output = wxT("Hamiltonian path: ");
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (i) output += wxT(" -> ");
        output += names[i];
    }
    output += wxT("\nHamiltonian path length (edges): ");
    output += wxString::Format(wxT("%lu"),
                               static_cast<unsigned long>(names.size()-1));
    window->writeOutputLine(output);
    window->Show();
    wxTheApp->SetTopWindow(window);
    delete graph;
}

bool GraphAlgorithmCanvas::edgeInCycle(
    const vector<wxString>& hamiltonianCycle,
    const wxString& src,
    const wxString& dest)
{
    for (size_t i = 0; i + 1 < hamiltonianCycle.size(); ++i)
    {
        const wxString& a = hamiltonianCycle[i];
        const wxString& b = hamiltonianCycle[i + 1];

        if ((a == src && b == dest) ||
            (a == dest && b == src))
        {
            return true;
        }
    }

    return false;
}

void GraphAlgorithmCanvas::Dijkstra()
{
	typedef lemon::ListGraph::Node Node;

	wxString wxStringStartingVertex, wxStringFinalVertex;
	wxString src;
	Node s, t, currVertex, dest;
	long INF = std::numeric_limits<long>::max();
	priority_queue<VertexInfo,vector<VertexInfo>,greater<vector<VertexInfo>::value_type> > minPQ;
	list<wxString> path;
	bool foundMinPath = false;
	int newMinWeight = 0;
	VertexInfo vInfo, neighbor_vInfo;
	wxString str;

	if (graphType != undirw)
	{
		::report (wxT ("The graph must be weighted"),"Note");
		return;
	}

	// Dijkstra's algorithm requires nonnegative edge weights.
	for (ListGraph::EdgeIt e(ug); e != INVALID; ++e)
	{
	    if (edgeMap[e] < 0)
	    {
	        ::report(
	            wxT("Dijkstra's algorithm cannot be used with negative edge weights."),
	            "Note");
	        return;
	    }
	}

	if (lemon::countNodes (ug) == 0)
	{
		::report (wxT ("The graph is empty."),"Note" );
		return;
	}

	OptimumPathInput dijkstraInput (this, &wxStringStartingVertex, &wxStringFinalVertex, true, undirw);

	if (dijkstraInput.ShowModal() == wxID_CANCEL)
		return;

	s = wxString2Node[wxStringStartingVertex];
	t = wxString2Node[wxStringFinalVertex];

	if (!ug.valid (s) || !ug.valid (t) )
	{
		::report (wxT ("Either the starting or ending vertex is not in the graph"),"Note");
		return;
	}

	// initialize all vertices to unmarked (WHITE) and dataValues to INF
	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		str = node2wxString[n];
		vInfo = VertexInfo (str,wxT (""),INF);
		nodeColor[str] = GraphAlgorithmCanvas::WHITE;
		node2VInfo[str] = vInfo;
	}

	// formulate initial priority queue entry
	str = node2wxString[s];
	vInfo = VertexInfo (str,wxT (""),0);
	node2VInfo[str] = vInfo;
	minPQ.push (vInfo);

	// process vertices until we find a minimum path to
	// t or the priority queue is empty
	while (!minPQ.empty() )
	{
		vInfo = minPQ.top();
		minPQ.pop();

		// if we are at t, we have found the minimum
		// path from s to t
		str = vInfo.node;
		currVertex = wxString2Node[str];

		if (currVertex == t)
		{
			foundMinPath = true;
			break;
		}

		if (nodeColor[str] != GraphAlgorithmCanvas::BLACK)
		{
			// mark the vertex so we don't look at it again
			nodeColor[str] = GraphAlgorithmCanvas::BLACK;

			// find all neighbors of the current vertex vInfo.node. for each
			// neighbor that has not been visited, generate a VertexInfo
			// object and insert it into the priority queue provided the
			// total weight to get to the neighbor is better than the
			// current dataValue in the neighbor
			for (ListGraph::OutArcIt a (ug, currVertex); a!=INVALID; ++a)
			{
				dest = ug.target (a);
				src = node2wxString[dest];
				neighbor_vInfo = node2VInfo[src];

				if (nodeColor[neighbor_vInfo.node] == GraphAlgorithmCanvas::WHITE)
				{
					// compare total weight of adding edge
					if ( (newMinWeight = vInfo.dataValue +
					                     edgeMap[a]) < neighbor_vInfo.dataValue)
					{
						// add VertexInfo object for new vertex
						neighbor_vInfo.node = src;
						neighbor_vInfo.dataValue = newMinWeight;
						neighbor_vInfo.parent = node2wxString[currVertex];
						node2VInfo[src] = neighbor_vInfo;
						minPQ.push (neighbor_vInfo);
					}	// end "if" that checks weights
				}	// end "if" that checks if neighbor is not marked
			}	// end "for"
		}	// end "if" vertex not already marked
	}	// end "while"

	if (foundMinPath)
	{
		currVertex = t;

		while (currVertex != s)
		{
			src = node2wxString[currVertex];
			path.push_front (src);
			src = node2VInfo[src].parent;
			currVertex = wxString2Node[src];
		}

		path.push_front (node2wxString[s]);
		minPathLength = node2VInfo[node2wxString[t]].dataValue;
		drawDijkstraPath (path);
	}
	else
		::report(wxT ("There is no path between the vertices."),"Note");
}

void GraphAlgorithmCanvas::drawDijkstraPath (const list<wxString>& path)
{
	typedef lemon::ListGraph::Node Node;

	BuildGraph *DijkstraPath;
	wxString pathListing, tempResult, label,
	         srcLabel, destLabel, vertexWeight;
	list<wxString>::const_iterator iter;
	Node src, dest;
	bool minPathEdge;
	int edgeWeight;
	DisplayAlgorithmResults *minPathGraph;

	DijkstraPath = new BuildGraph (wxT(""), undirw, forcedirected, true);

	for (ListGraph::ArcIt a (ug); a != INVALID; ++a)
	{
		src = ug.source (a);
		dest = ug.target (a);
		srcLabel = node2wxString[src];
		destLabel = node2wxString[dest];
		minPathEdge = edgeInPath (path, srcLabel, destLabel);
		edgeWeight = edgeMap[ug.arcFromId (ug.id (a) )];
		vertexWeight.Printf (wxT ("%d"),edgeWeight);

		if (minPathEdge)
			DijkstraPath->insertEdge (srcLabel,
			                          destLabel,
			                          vertexWeight, wxSHORTEST_PATH_PEN,
			                          wxColour (255, 215, 0, 0),
			                          wxColour (255, 215, 0, 0),
			                          true);
		else if (!edgeInPath (path,destLabel,srcLabel))
			DijkstraPath->insertEdge (srcLabel,
			                          destLabel,
			                          vertexWeight, wxNORMAL_PEN,
			                          wxColour(255,215,0), wxColour(255,215,0));
	}

	/* Hack because I cannot determine why one or more vertices of the
	   resulting graph do not have the color (255,215,0) */
	Graph *g = DijkstraPath->getGraph();
	for (node v : g->nodes)
	{
		DijkstraPath->getGraphAttributes()->fillColor(v) =  ogdf::Color(255,215,0);
	}

	DijkstraPath->forceDirectedLayout();
	minPathGraph = new DisplayAlgorithmResults (myParent, DijkstraPath, undirw, forcedirected,
	      DisplayAlgorithmResults::MINIMUM_PATH);
	pathListing.Printf (wxT ("minPathLength = %d\n"),minPathLength);
	pathListing += wxT("Minimum path: ");

	iter = path.begin();

	while (iter != path.end() )
	{
		tempResult.Printf (wxT ("%s  "), *iter);
		pathListing += tempResult;
		iter++;
	}

	minPathGraph->writeOutputLine(pathListing);

	minPathGraph->Show();

	minPathGraph->getDisplayAlgorithmResultsTextCtrl()->Show();
	minPathGraph->getDisplayAlgorithmResultsTextCtrl()->Refresh();
	minPathGraph->getDisplayAlgorithmResultsTextCtrl()->Update();

	wxTheApp->SetTopWindow(minPathGraph);

	delete DijkstraPath;
}

bool GraphAlgorithmCanvas::edgeInPath (const list<wxString>& path,
                                       const wxString& src, const wxString& dest)
{
	list<wxString>::const_iterator srcIter, destIter;

	if (path.size() == 1)
		return true;

	srcIter = path.begin();
	destIter = path.begin();
	destIter++;

	while (destIter != path.end() )
	{
		if ( (*srcIter == src) && (*destIter == dest) )
			return true;

		srcIter++;
		destIter++;
	}

	return false;
}

void GraphAlgorithmCanvas::Prim()
{
	typedef lemon::ListGraph::Node Node;

	wxString startingVertex, str, u,v, spanningTreeLabel, parent;
	wxString src, totalWeightText;
	ListGraph::NodeIt sVertex (ug);
	Node currVertex, parentVertex, source, target, dest;
	long INF = std::numeric_limits<long>::max();
	priority_queue<VertexInfo,vector<VertexInfo>,greater<vector<VertexInfo>::value_type> > minPQ;
	list<wxString> path;
	int totalWeight = 0, numberOfNodes = lemon::countNodes (ug),
	    countOfNodes = 0, edgeWeight;
	VertexInfo vInfo, neighbor_vInfo;
	BuildGraph *spanningTree;
	DisplayAlgorithmResults *minSpanningTreeGraph;
	lemon::ListGraph::Arc e;
	std::set<lemon::ListGraph::Arc>::iterator edgeNotInSpanningTree = spanningTreeEdge.end();

	if (graphType != undirw)
	{
		::report (wxT ("The graph must be weighted"),"Note");
		return;
	}

	numberOfNodes = lemon::countNodes (ug);

	if (numberOfNodes == 0)
	{
		::report (wxT ("The graph is empty."),"Note");
		return;
	}

	// initialize all vertices to unmarked (WHITE) and dataValues to INF
	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		str = node2wxString[n];
		vInfo = VertexInfo (str,wxT (""),INF);
		nodeColor[str] = GraphAlgorithmCanvas::WHITE;
		node2VInfo[str] = vInfo;
	}

	// formulate initial priority queue entry
	startingVertex = node2wxString[sVertex];
	vInfo = VertexInfo (startingVertex,wxT (""),0);
	node2VInfo[startingVertex] = vInfo;
	minPQ.push (vInfo);

	// process vertices until we find the spanning tree of there
	// is an error.
	for (;;)
	{
		if (minPQ.empty() && countOfNodes < numberOfNodes)
		{
			::report (wxT ("The graph is not connected"),"Note");
			return;
		}

		vInfo = minPQ.top();
		minPQ.pop();


		str = vInfo.node;
		parent = vInfo.parent;
		currVertex = wxString2Node[str];

		if (nodeColor[str] == GraphAlgorithmCanvas::WHITE)
		{
			countOfNodes++;
			totalWeight += vInfo.dataValue;

			if (parent.length() != 0)
			{
				parentVertex = wxString2Node[parent];
				e = lemon::findArc (ug,parentVertex,currVertex);
				spanningTreeEdge.insert (e);
			}

			if (countOfNodes == numberOfNodes)
				break;

			// mark the vertex so we don't look at it again
			nodeColor[str] = GraphAlgorithmCanvas::BLACK;

			// find all neighbors of the current vertex vInfo.node. for each
			// neighbor that has not been visited, generate a VertexInfo
			// object and insert it into the priority queue provided its
			// edge weight is better going through currVertex.
			for (ListGraph::OutArcIt a (ug, currVertex); a!=INVALID; ++a)
			{
				dest = ug.target (a);
				src = node2wxString[dest];
				neighbor_vInfo = node2VInfo[src];

				if (nodeColor[neighbor_vInfo.node] == GraphAlgorithmCanvas::WHITE)
				{
					edgeWeight = edgeMap[a];

					// compare total weight of adding the edge to the spanning tree
					if (edgeWeight < neighbor_vInfo.dataValue)
					{
						// add VertexInfo object for new vertex
						neighbor_vInfo.node = src;
						neighbor_vInfo.dataValue = edgeWeight;
						neighbor_vInfo.parent = node2wxString[currVertex];
						node2VInfo[src] = neighbor_vInfo;
						minPQ.push (neighbor_vInfo);
					}	// end "if" that checks weights
				}	// end "if" that checks if neighbor is not marked
			}	// end "for"
		}	// end "if" vertex not already marked
	}	// end "while"

	spanningTree = new BuildGraph (wxT (""), undirw, forcedirected, true);

	for (ListGraph::ArcIt a (ug); a != INVALID; ++a)
	{
		if (spanningTreeEdge.find (a) != edgeNotInSpanningTree)
		{
			source = ug.source (a);
			target = ug.target (a);
			u = node2wxString[source];
			v = node2wxString[target];
			src.Printf (wxT ("%.2d"),edgeMap[a]);
			spanningTree->insertEdge (u,
			                          v,
			                          src, wxNORMAL_PEN,
			                          wxColour (255, 215, 0, 0),
			                          wxColour (255, 215, 0, 0) );
		}
	}

	spanningTree->hierarchicalLayout();
	minSpanningTreeGraph = new DisplayAlgorithmResults (myParent, spanningTree, undirw, forcedirected,
	      DisplayAlgorithmResults::PRIM);
	totalWeightText.Printf(wxT("Total weight: %d"), totalWeight);
	minSpanningTreeGraph->writeOutputLine(totalWeightText);
	minSpanningTreeGraph->Show();
	wxTheApp->SetTopWindow (minSpanningTreeGraph);
}

void GraphAlgorithmCanvas::beginColoring()
{
	int numColors;
	map<lemon::ListGraph::Node,int>::iterator iter;

	EnterNumberOfGraphColors numColorsInput (this, &numColors);

	if (numColorsInput.ShowModal() == wxID_CANCEL)
		return;

	// setInProcess();

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
		colorOfNode[n] = -1;

	ListGraph::NodeIt start (ug);

	if (colorGraph (start, numColors) )
	{
		wxString msg;

		iter = colorOfNode.begin();

		while (iter != colorOfNode.end() )
		{
			if (iter->second == -1)
			{
				msg.Printf (wxT ("There is no coloring with %d colors."),numColors);
				::report (msg,"Note");
				return;
			}

			iter++;
		}

		drawColoredGraph (numColors);
	}
	else
	{
		wxString msg;

		msg.Printf (wxT ("There is no coloring with %d colors."),numColors);
		::report (msg,"Note");
	}
}

bool GraphAlgorithmCanvas::colorGraph (lemon::ListGraph::Node n, unsigned numColors)
{
	int c;
	lemon::ListGraph::NodeIt w (ug,n);

	if (n != INVALID)
	{
		// Try to color vertex n, then recursively color the rest.
		c = chooseColor (n, numColors, -1);

		while (c >= 0 && c < (int) numColors)
		{
			colorOfNode[n] = c;
			++w;

			if (colorGraph (w,numColors) )
				return true;  // A solution has been found
			else
				// We have backtracked to here - try a different color for n
				c = chooseColor (n, numColors, c);
		}

		// If we exit the above loop, no solution is possible given the
		// colors that had already been assigned to n.
		colorOfNode[n] = -1;
		return false;
	}
	else
		return true;
}

int GraphAlgorithmCanvas::chooseColor (lemon::ListGraph::Node n, unsigned numColors, int startingColor)
{
	vector<bool> colorHasBeenSeen (numColors,false);
	lemon::ListGraph::Node neighborVertex;
	int c;
	vector<bool>::iterator iter = colorHasBeenSeen.begin();

	for (ListGraph::OutArcIt a (ug, n); a != INVALID; ++a)
	{
		neighborVertex = ug.target (a);
		c = colorOfNode[neighborVertex];

		if ( (c >= 0) && !colorHasBeenSeen[c])
			colorHasBeenSeen[c] = true;
	}

	unsigned int chosen = find (iter+startingColor+1, iter+numColors, false) - iter;
	return (chosen < numColors) ? chosen : -1;
}

void GraphAlgorithmCanvas::drawColoredGraph (int numColors)
{
	lemon::ListGraph::NodeMap<wxColour> colorMap (ug);
	vector<wxColour> vertexColours;
	colorPicker colorsToUse;
	wxString msg;
	map<lemon::ListGraph::Node,int>::iterator iter;
	int maxColor = -1, value;

	// clearSetInProcess();

	for (iter = colorOfNode.begin(); iter != colorOfNode.end(); iter++)
	{
		value = iter->second;

		if (value > maxColor)
			maxColor = value;
	}

	colorsToUse.PickColor(vertexColours, numColors);

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		wxColour c = vertexColours[colorOfNode[n]];
		colorMap[n] = c;
	}

	msg.Printf (wxT ("%d"),maxColor+1);
	MyGraphBfsVisitor visitor (myParent, &node2wxString, &edgeMap, &ug, undir, forcedirected,
	                           DisplayAlgorithmResults::GRAPHCOLORING,
	                           false, &colorMap, msg);
	lemon::BfsVisit<lemon::ListGraph, MyGraphBfsVisitor> myBfsVisit (ug, visitor);

	myBfsVisit.init();

	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
		if (!myBfsVisit.reached (n) )
		{
			myBfsVisit.addSource (n);
			myBfsVisit.start();
		}

	// setInProcess();
	visitor.drawTree();
}

/*for (ListGraph::NodeIt n(ug); n != INVALID;++n)
{
	if (inVector(hamiltonianCycle, node2wxString[n]))
		hamCycle->insertEdge(node2wxString[n], wxT("#"), wxT(""), NORMAL_PEN, wxColour(37, 72, 255));
	else
		hamCycle->insertEdge(node2wxString[n], wxT("#"), wxT(""), NORMAL_PEN);
}*/

void GraphAlgorithmCanvas::solveTSP()
{
	if (graphType != undirw)
	{
		::report (wxT ("The graph must be weighted"),"Note");
		return;
	}

	// verify that ug is a complete graph having no parallel edges

	// first verify graph has no parallel edges.
	if (builtGraph->hasParallelEdges() )
	{
		report (wxT ("The graph has parallel edges."),"Note");
		return;
	}

	// verify that the graph has an edge between every pair of vertices
	ogdf::List<node> vertices;
	ogdf::List<edge> edges;
	int n, numedges;

	G->allNodes (vertices);
	n = vertices.size();
	G->allEdges (edges);
	numedges = edges.size();

	if (numedges != (n* (n - 1) ) / 2)
	{
		::report (wxT ("The graph is not complete."),"Note");
		return;
	}

	// convert ug to a FullGraph
	lemon::FullGraph tsp (n);
	// lemon::FullGraph::NodeMap<wxString> fullGraph_node2wxString(tsp);
	map<lemon::FullGraph::Node, wxString> fullGraph_node2wxString;
	map<wxString, lemon::FullGraph::Node> fullGraph_wxString2Node;
	wxString s,t;
	FullGraph::NodeIt nfull (tsp);
	FullGraph::Node nsrc, ndest;
	FullGraph::Edge efull;
	ListGraph::Node src, dest;
	double edgeWeight;
	lemon::FullGraph::EdgeMap<double> cost (tsp);
	double approxMinCost;
	vector<lemon::FullGraph::Node> tour;
	vector<lemon::FullGraph::Node>::iterator tourIter, tourStart;
	lemon::FullGraph::Node tourStop;
	wxString wxStringTourStop;
	list<wxString> tourPath;

	// assign all the vertex labels in ug to tsp
	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		s = node2wxString[n];
		fullGraph_wxString2Node[s] = nfull;
		fullGraph_node2wxString[nfull] = s;
		++nfull;
	}

	// assign the edge weights in ug to tsp
	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		edgeWeight = edgeMap[e];
		src = ug.u (e);
		s = node2wxString[src];
		nsrc = fullGraph_wxString2Node[s];
		dest = ug.v (e);
		t = node2wxString[dest];
		ndest = fullGraph_wxString2Node[t];
		efull = tsp.edge (nsrc, ndest);
		cost[efull] = edgeWeight;
	}

	// apply InsertionTsp
	lemon::InsertionTsp< FullGraph::EdgeMap<double> > tspalg (tsp, cost);

	tspalg.run();

	// output the path and approximate minimum cost
	approxMinCost = tspalg.tourCost();

	tour = tspalg.tourNodes();
	tourIter = tour.begin();
	tourStart = tourIter;

	while (tourIter != tour.end() )
	{
		tourStop = *tourIter;
		wxStringTourStop = fullGraph_node2wxString[tourStop];
		tourPath.push_back (wxStringTourStop);
		tourIter++;
	}

	tourPath.push_back (fullGraph_node2wxString[*tourStart]);

	// draw the TSP path
	typedef lemon::FullGraph::Node Node;

	BuildGraph *TSPPath;
	wxString TSPGraphLabel, tempResult, label,
	         srcLabel, destLabel, vertexWeight;
	wxString wxSrc, wxDest, wxEdgeWeight;
	bool minPathEdge;
	DisplayAlgorithmResults *TSPCycle;
	// ShowSequence* cycleOutput;
	list<wxString>::const_iterator pathIter, pathIterPred;
	int line = 1, vertexNum = 1;

	/*TSPGraphLabel.Printf (wxT ("Minimum cost = %d\n"),int (approxMinCost) );
	if (originalTitle.c_str() != 0)
		TSPGraphLabel = wxString(originalTitle.c_str()) + "\n" + TSPGraphLabel;
	//TSPGraphLabel += wxT("Cycle: ");*/

	// cycleOutput = new ShowSequence (this,"tspcycle.png",ShowSequence::TSP);

	TSPPath = new BuildGraph (wxT (""), undirw, circular, true);

	for (FullGraph::ArcIt a (tsp); a != INVALID; ++a)
	{
		nsrc = tsp.source (a);
		ndest = tsp.target (a);
		wxSrc = fullGraph_node2wxString[nsrc];
		wxDest = fullGraph_node2wxString[ndest];
		minPathEdge = edgeInPath (tourPath, wxSrc, wxDest);
		edgeWeight = cost[tsp.arcFromId (tsp.id (a) )];
		wxEdgeWeight.Printf (wxT ("%.1f"),edgeWeight);

		if (minPathEdge)
			TSPPath->insertEdge (wxSrc,wxDest,wxEdgeWeight, wxNORMAL_PEN,
			                     wxColour(255,215,0), wxColour(255,215,0));
	}

	
TSPPath->circularLayout();

// Arrange TSP vertices in tour order to prevent crossing edges.
{
    ogdf::Graph *tourGraph = TSPPath->getGraph();
    ogdf::GraphAttributes *tourGA = TSPPath->getGraphAttributes();

    std::vector<wxString> tourVertices;

    for (const wxString& vertex : tourPath)
    {
        if (std::find(tourVertices.begin(),
                      tourVertices.end(), vertex) == tourVertices.end())
            tourVertices.push_back(vertex);
    }

    const double pi = 3.14159265358979323846;
    const double radius = 180.0;
    const double centerX = 250.0;
    const double centerY = 250.0;

    const size_t count = tourVertices.size();

    if (count > 0)
    {
        for (ogdf::node v : tourGraph->nodes)
        {
            wxString vertexLabel(
                tourGA->label(v).c_str(), wxConvUTF8);

            auto position = std::find(
                tourVertices.begin(),
                tourVertices.end(),
                vertexLabel);

            if (position == tourVertices.end())
                continue;

            size_t index = std::distance(
                tourVertices.begin(), position);

            double angle =
                2.0 * pi * static_cast<double>(index) /
                static_cast<double>(count);

            tourGA->x(v) = centerX + radius * std::cos(angle);
            tourGA->y(v) = centerY + radius * std::sin(angle);
        }
    }
}

	TSPCycle = new DisplayAlgorithmResults (myParent, TSPPath, undirw, circular,
	                                        DisplayAlgorithmResults::TSP);
	pathIter = tourPath.begin();

	char buf[25];
	sprintf(buf,"Minimum cost = %d\n",(int)approxMinCost);
	TSPCycle->writeOutputLine(buf);

	while (pathIter != tourPath.end() )
	{
		if (vertexNum++ != n + 1)
			tempResult.Printf (wxT ("%s -> "), *pathIter);
		else
			tempResult.Printf (wxT ("%s"), *pathIter);

		TSPCycle->writeOutputLine(tempResult);

		if (line++ % 8 == 0)
			TSPCycle->writeOutputLine(wxT("\n"));

		pathIter++;
	}

	TSPCycle->Show();
	// cycleOutput->Show (true);
	wxTheApp->SetTopWindow (TSPCycle);
}

void GraphAlgorithmCanvas::isPlanar()
{
	bool isGraphPlanar = lemon::checkPlanarity (ug);

	if (isGraphPlanar)
		::report (wxT ("The graph is planar."),"Note" );
	else
		::report (wxT ("The graph is not planar."),"Note" );
}
void GraphAlgorithmCanvas::drawMaxIndependentEdgeSetCardinalityMatching()
{
	lemon::MaxMatching<ListGraph> mm (ug);
	int matchSize,numberOfEdges=0;
	BuildGraph *maxIndepEdgeSetCardinality;
	DisplayAlgorithmResults *maxIndependentEdgeSetGraph;
	ListGraph::Node src, dest;
	wxString srcLabel, destLabel, tempResult;

	mm.run();

	matchSize = mm.matchingSize();
	tempResult.Printf (wxT ("Maximum independent edge set\nmeasured by number of edges.\nNumber of edges = %d"), matchSize);
	maxIndepEdgeSetCardinality = new BuildGraph (wxT (""), undir, forcedirected, true);

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		src = ug.u (e);
		dest = ug.v (e);
		srcLabel = node2wxString[src];
		destLabel = node2wxString[dest];

		if (mm.matching (e) )
		{
			tempResult.Printf (wxT ("%s - %s\n"), srcLabel, destLabel);
			maxIndepEdgeSetCardinality->insertEdge (srcLabel, destLabel,
			                                        wxT (""), wxSHORTEST_PATH_PEN,
			                                        wxColour(255,215,0), wxColour(255,215,0));
			numberOfEdges++;
		}
		else
			maxIndepEdgeSetCardinality->insertEdge (srcLabel,
			                                        destLabel,
			                                        wxT (""), wxNORMAL_PEN,
			                                        wxColour(255,215,0), wxColour(255,215,0));
	}

	maxIndepEdgeSetCardinality->circularLayout();
	maxIndependentEdgeSetGraph = new DisplayAlgorithmResults (myParent, maxIndepEdgeSetCardinality, undir, circular,
	      DisplayAlgorithmResults::CARDMATCHING);

	tempResult.Printf(wxT("There are %d independent edges.\n"),numberOfEdges);
	maxIndependentEdgeSetGraph->writeOutputLine (tempResult);
	maxIndependentEdgeSetGraph->writeOutputLine (wxT("\n"));
	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		src = ug.u (e);
		dest = ug.v (e);
		srcLabel = node2wxString[src];
		destLabel = node2wxString[dest];

		if (mm.matching (e) )
		{
			tempResult.Printf (wxT ("%s - %s\n"), srcLabel, destLabel);
			maxIndependentEdgeSetGraph->writeOutputLine (tempResult);
		}
	}

	maxIndependentEdgeSetGraph->Show();
	wxTheApp ->SetTopWindow (maxIndependentEdgeSetGraph);

	// delete maxIndepEdgeSetCardinality;
}

void GraphAlgorithmCanvas::drawMaxIndependentEdgeSetWeightedMatching()
{
	if (graphType != undirw)
	{
		::report (wxT ("The graph must be weighted"),"Note");
		return;
	}

	int matchSize;
	BuildGraph *maxIndepEdgeSetWeightedGraph;
	DisplayAlgorithmResults *maxIndependentEdgeSetGraph;
	ListGraph::Node src, dest;
	wxString srcLabel, destLabel, tempResult;
	wxString strEdgeWeight;
	lemon::ListGraph::EdgeMap<int> w (ug);
	lemon::MaxWeightedMatching<lemon::ListGraph> mm (ug, w);

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
		w[e] = (int) edgeMap[e];

	mm.run();

	matchSize = mm.matchingSize();
	maxIndepEdgeSetWeightedGraph = new BuildGraph (wxT (""), undirw, forcedirected, true);

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		strEdgeWeight = wxString::Format(wxT("%i"),edgeMap[e]);

		src = ug.u (e);
		dest = ug.v (e);
		srcLabel = node2wxString[src];
		destLabel = node2wxString[dest];

		if (mm.matching (e) )
		{
			tempResult.Printf (wxT ("%s - %s\n"), srcLabel,destLabel);
			maxIndepEdgeSetWeightedGraph->insertEdge (srcLabel, destLabel,
			      strEdgeWeight, wxSHORTEST_PATH_PEN,
			      wxColour(255,215,0), wxColour(255,215,0));
		}
		else
			maxIndepEdgeSetWeightedGraph->insertEdge (srcLabel,
			      destLabel, strEdgeWeight, wxNORMAL_PEN,
			      wxColour(255,215,0), wxColour(255,215,0));
	}

	maxIndepEdgeSetWeightedGraph->forceDirectedLayout();
	maxIndependentEdgeSetGraph = new DisplayAlgorithmResults (myParent, maxIndepEdgeSetWeightedGraph, undirw, forcedirected,
	      DisplayAlgorithmResults::WEIGHTEDMATCHING);

	tempResult.Printf(wxT("There are %d independent edges.\n"),matchSize);
    maxIndependentEdgeSetGraph->writeOutputLine (tempResult);
	maxIndependentEdgeSetGraph->writeOutputLine (wxT("\n"));

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		src = ug.u (e);
		dest = ug.v (e);
		srcLabel = node2wxString[src];
		destLabel = node2wxString[dest];

		if (mm.matching (e) )
		{
			tempResult.Printf (wxT ("%s - %s\n"), srcLabel, destLabel);
			maxIndependentEdgeSetGraph->writeOutputLine (tempResult);
		}
	}

	maxIndependentEdgeSetGraph->Show();
	wxTheApp ->SetTopWindow (maxIndependentEdgeSetGraph);

	// delete maxIndepEdgeSetWeightedGraph;
}

void GraphAlgorithmCanvas::MaximumSpanningTree()
{
	lemon::ListGraph::Node currVertex, parentVertex, source, target, dest;
	wxString u,v,src;

	// make the weights negative and find the minimum
	// spanning tree using the Kruskal algorithm
	typedef lemon::ListGraph::Node Node;

	//  build the input map for the Kruskal algorithm. The edge keys and
	// corresponding costs must be assigned in cost-ascending order.
	makeKruskalMapInput();
	lemon::kruskal (ug, sortedWeights, *tree_map);

	/*if ( originalTitle.length() != 0)
	{
		maxSpanningTree = new BuildGraph (wxString(originalTitle.c_str()), undirw , hierarchical);
		combinedGraphs = new BuildGraph (wxString(originalTitle.c_str()), undirw, hierarchical);
	}
	else
	{*/
	maxSpanningTree = new BuildGraph (wxT (""), undirw, hierarchical);
	combinedGraphs = new BuildGraph (wxT (""), undirw, hierarchical);
	//}

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		int nx;

		if ( (*tree_map) [e])
		{
			source = ug.u (e);
			target = ug.v (e);
			u = wxT (" ") + node2wxString[source] + wxT (" ");
			v = wxT (" ") + node2wxString[target] + wxT (" ");
			nx = -sortedWeights[e];
			src.Printf (wxT ("%d"), nx);
			maxSpanningTree->insertEdge (u,
			                             v,
			                             src, wxNORMAL_PEN,
			                             wxColour (255, 215, 0, 0),
			                             wxColour (255, 215, 0, 0) );
			combinedGraphs->insertEdge (u,
			                            v,
			                            src, wxNORMAL_PEN,
			                            wxColour (255, 215, 0, 0),
			                            wxColour (255, 215, 0, 0) );
		}
	}

	maxSpanningTree->hierarchicalLayout();
}

void GraphAlgorithmCanvas::FindWidestPath()
{
	wxString output;
    int numberOfComponents;

	if (graphType != undirw)
	{
		::report (wxT ("The graph must be weighted"),"Note");
		return;
	}

	if (lemon::countNodes (ug) == 0)
	{
		::report (wxT ("The graph is empty."),"Note");
		return;
	}

    numberOfComponents = countConnectedComponents (ug);
    if (numberOfComponents != 1)
    {
        ::report(wxT("The graph must be connected"), wxT("Note"));
        return;
    }

    WidestPathDrawingCanvas *maxSpanningTreeGraph;
	wxString s, t;

	OptimumPathInput widestPathInput (this, &s, &t, false, undirw);

	if (widestPathInput.ShowModal() == wxID_CANCEL)
		return;

	if (!checkVertexLabel (s) )
	{
		output.Printf (wxT ("%s is not a vertex."), s.c_str() );
		::report (output,"Note");
		return;
	}
	else if (!checkVertexLabel (t) )
	{
		output.Printf (wxT ("%s is not a vertex."), t.c_str() );
		::report (output,"Note");
		return;
	}

	if (tree_map != NULL)
		delete tree_map;

	tree_map = new EBoolMap (ug);

	if (widestpath_map != NULL)
		delete widestpath_map;

	widestpath_map = new EBoolMap (ug);

	MaximumSpanningTree();
	widestPath (s, t);
	drawWidestPathGraph ();
	int treeWeight = 0;
	int limitingWeight = std::numeric_limits<int>::max();
	bool pathHasEdges = false;

	for (ListGraph::EdgeIt e(ug); e != INVALID; ++e)
	{
		if ((*tree_map)[e])
			treeWeight += edgeMap[e];

		if ((*widestpath_map)[e])
		{
			pathHasEdges = true;
			if (edgeMap[e] < limitingWeight)
					limitingWeight = edgeMap[e];
		}
	}

	wxString treeResult;
	treeResult.Printf(wxT("Total weight: %d"), treeWeight);


    // Reconstruct the path from target to source.
    wxString pathSequence;
    lemon::ListGraph::Node current = INVALID;

    for (ListGraph::NodeIt n(ug); n != INVALID; ++n)
    {
        if (node2wxString[n] == t)
        {
            current = n;
            break;
        }
    }


    // Follow parents back to the starting vertex.
    int steps = 0;
    int maxSteps = lemon::countNodes(ug);

    while (current != INVALID && steps < maxSteps)
    {
        wxString label = node2wxString[current];

        if (pathSequence.IsEmpty())
            pathSequence = label;
        else
            pathSequence = label + wxT(" -> ") + pathSequence;

        if (label == s)
            break;

        current = widestPathNode2VInfo[current].parent;
        ++steps;
    }


	wxString pathResult;
	if (pathHasEdges)
	{
		pathResult.Printf(
			wxT("Path: %s\nLimiting weight: %d"),
			pathSequence.c_str(), limitingWeight);
	}
	else
	{
		pathResult.Printf(
			wxT("From %s to %s\nNo edges in path"),
			s.c_str(), t.c_str());
	}



	// draw the spanning tree and the widest path graph on a
	// WidestPathDrawingCanvas
	maxSpanningTreeGraph = new WidestPathDrawingCanvas (myParent, maxSpanningTree, widestPathGraph, combinedGraphs);
	maxSpanningTreeGraph->SetResults(treeResult, pathResult);
	maxSpanningTreeGraph->DrawMaximumSpanningTree();
	maxSpanningTreeGraph->DrawWidestPath();
	maxSpanningTreeGraph->Show();
	wxTheApp->SetTopWindow (maxSpanningTreeGraph);
}

// do BFS from s to t to find the widest path and record it in
// the map widestpath_map
void GraphAlgorithmCanvas::widestPath (const wxString& s, const wxString& t)
{
	typedef lemon::ListGraph::Node Node;
	typedef lemon::ListGraph::Edge Edge;

	Node startingNode;
	Node currNode, parentNode, targetNode;
	queue<WidestPathVertexInfo> q;
	WidestPathVertexInfo vInfo, neighbor_vInfo;
	Edge e;

	// initialize all vertices to unmarked (WHITE).
	// locate s
	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		widestPathNodeColor[n] = GraphAlgorithmCanvas::WHITE;

		if (node2wxString[n] == s)
			startingNode = n;
	}

	// formulate initial queue entry
	vInfo = WidestPathVertexInfo (startingNode, INVALID);
	q.push (vInfo);

	// process vertices until we find t
	widestPathNode2VInfo[startingNode] = vInfo;

	for (;;)
	{
		vInfo = q.front();
		currNode = vInfo.currNode;
		q.pop();

		// mark the vertex BLACK so we don't look at it again
		widestPathNodeColor[currNode] = GraphAlgorithmCanvas::BLACK;

		if (node2wxString[currNode] == t)
			break;

		// find all neighbors of the current vertex currNode in the
		// maximum spanning tree for each neighbor that has not been visited,
		// generate a WidestPathVertexInfo object and insert it into the queue
		for (ListGraph::OutArcIt a (ug, currNode); a !=INVALID; ++a)
		{
			targetNode = ug.target (a);

			if ( (*tree_map) [a])
			{
				if (widestPathNodeColor[targetNode] == GraphAlgorithmCanvas::WHITE)
				{
					neighbor_vInfo.currNode = targetNode;
					neighbor_vInfo.parent = currNode;
					widestPathNode2VInfo[targetNode] = neighbor_vInfo;

					q.push (neighbor_vInfo);
				} // end "if" that checks if neighbor is not  discovered
			} // end checking only vertices on maximum spanning tree
		} 	// end "for"
	}	// end "for (;;)"

	currNode = vInfo.currNode;
	parentNode = vInfo.parent;

	while (parentNode != INVALID )
	{
		e = lemon::findEdge (ug,currNode,parentNode);
		(*widestpath_map) [e] = true;
		currNode = parentNode;
		parentNode = widestPathNode2VInfo[parentNode].parent;
	}
}

void GraphAlgorithmCanvas::drawWidestPathGraph ()
{
	lemon::ListGraph::Node source, target;
	wxString u,v,src;

	/*if (originalTitle.length() != 0)
		widestPathGraph = new BuildGraph (originalTitle.c_str(), undirw, hierarchical);
	else*/
	widestPathGraph = new BuildGraph (wxT (""), undirw, hierarchical);

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		int nx;
		source = ug.u (e);
		target = ug.v (e);
		u = node2wxString[source];
		v = node2wxString[target];

		nx = edgeMap[e];
		src.Printf (wxT ("%d"), nx);

		if ( (*widestpath_map) [e])
		{
			widestPathGraph->insertEdge (u,
			                             v,
			                             src, wxSHORTEST_PATH_PEN,
			                             wxColour (255, 215, 0, 0),
			                             wxColour (255, 215, 0, 0) );
			combinedGraphs->insertEdge (u,
			                            v,
			                            src, wxSHORTEST_PATH_PEN,
			                            wxColour (255, 215, 0, 0),
			                            wxColour (255, 215, 0, 0) );
		}
		else
		{
			widestPathGraph->insertEdge (u,
			                             v,
			                             src, wxwxDASHED_GRAPH_EDGE_PEN,
			                             wxColour (255, 215, 0, 0),
			                             wxColour (255, 215, 0, 0) );
			combinedGraphs->insertEdge (u,
			                            v,
			                            src, wxwxDASHED_GRAPH_EDGE_PEN,
			                            wxColour (255, 215, 0, 0),
			                            wxColour (255, 215, 0, 0) );
		}
	}

	widestPathGraph->hierarchicalLayout();
	combinedGraphs->hierarchicalLayout();
}

bool compp (pair<lemon::ListGraph::Edge,double> lhs, pair<lemon::ListGraph::Edge,double> rhs)
{
	return lhs.second < rhs.second;
}

void GraphAlgorithmCanvas::makeKruskalMapInput()
{
	vector<pair<lemon::ListGraph::Edge,double> > v;

	pair<lemon::ListGraph::Edge,double> p;

	for (ListGraph::EdgeIt e (ug); e != INVALID; ++e)
	{
		p.first = e;
		p.second = -edgeMap[e];
		v.push_back (p);
	}

	sort (v.begin(), v.end(), compp);

	vector<pair<lemon::ListGraph::Edge,double> >::iterator viter = v.begin();

	while (viter != v.end() )
	{
		sortedWeights[ (*viter).first] = (*viter).second;
		viter++;
	}
}

bool GraphAlgorithmCanvas::checkVertexLabel (const wxString& s)
{
	for (ListGraph::NodeIt n (ug); n != INVALID; ++n)
	{
		if (node2wxString[n] == s)
			return true;
	}

	return false;
}

string GraphAlgorithmCanvas::intToStr (int n)
{
	stringstream out;

	out << n;
	return out.str();
}
