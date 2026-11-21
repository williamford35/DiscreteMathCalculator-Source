/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DiscreteMathCalculatorMain.h"
#include "graphs/algorithms/DigraphAlgorithmCanvas.h"
#include "graphs/algorithms/search/bfs/BfsSearch.h"
#include "graphs/algorithms/search/dfs/DfsSearch.h"
#include "graphs/algorithms/search/bfs/MyDigraphBfsVisitor.h"
#include "graphs/algorithms/search/bfs/MyDigraphBfsPathVisitor.h"
#include "graphs/algorithms/search/dfs/MyGraphDfsVisitor.h"
#include "graphs/algorithms/search/dfs/MyDigraphDfsVisitor.h"
#include "graphs/algorithms/search/dfs/MyDigraphDfsPathVisitor.h"
#include "graphs/algorithms/HamPathDialog.h"
#include "graphs/algorithms/LongestPath/LongestPathInput.h"
#include "common/colorPicker.h"
#include "common/ImageFiles.h"
#include "common/matrix.h"
#include "common/report.h"

#include <lemon/edmonds_karp.h>
#include <lemon/euler.h>
#include <lemon/min_cost_arborescence.h>

// #include "images/discmathcalc.xpm"
#include "AboutDiscreteMathCalculator.h"
#include "graphs/algorithms/Dijkstra/OptimumPathInput.h"
#include "graphs/algorithms/mincostarborescence/MinCostArborescenceDialog.h"

//(*InternalHeadersPCH(DigraphAlgorithmCanvas)
#include <wx/intl.h>
#include <wx/string.h>
//*)
//(*InternalHeaders(DigraphAlgorithmCanvas)
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

#include "NTL/tools.h"

NTL_CLIENT

#include <set>
#include <list>
#include <limits>
#include <functional>
#include <queue>

using namespace std;

//(*IdInit(DigraphAlgorithmCanvas)
const long DigraphAlgorithmCanvas::ID_GRAPH_DRAWING = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_FIGURE_SLIDER = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_FIGURE_PANEL = wxNewId();
const long DigraphAlgorithmCanvas::ID_ALGORITHMS_STATICTEXT = wxNewId();
const long DigraphAlgorithmCanvas::ID_ALGORITHM_CLASSES_COMBOBOX = wxNewId();
const long DigraphAlgorithmCanvas::ID_STATICTEXT1 = wxNewId();
const long DigraphAlgorithmCanvas::ID_ALGORITHMS_COMBOBOX = wxNewId();
const long DigraphAlgorithmCanvas::ID_RUN_ALGORITHM_BUTTON = wxNewId();
const long DigraphAlgorithmCanvas::ID_ALGORITHM_CANVAS_CLOSE_BUTTON = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHMS_PANEL = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_DRAWING_CANVAS_PANEL = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_QUIT_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_ABOUT_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_STATUSBAR = wxNewId();
//*)
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_PRINT_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_PRINT_PREVIEW_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_PAGESETUP_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_SAVE_AS_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM = wxNewId();
const long DigraphAlgorithmCanvas::ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (DigraphAlgorithmCanvas,wxFrame)
	EVT_MENU (wxID_EXIT, DigraphAlgorithmCanvas::OnQuit)
	EVT_CLOSE (DigraphAlgorithmCanvas::OnCloseWindow)
END_EVENT_TABLE()

DigraphAlgorithmCanvas::DigraphAlgorithmCanvas (wxWindow* parent, wxSFDiagramManager mgr,
      double scale, ogdf::GraphAttributes *ga, ogdf::Graph *grph, wxWindowID id, const wxPoint& pos, const wxSize& size,
      GraphType gtype) :
	m_canvas (mgr), graphType (gtype), myParent (parent),
	G (grph),GA (ga),digraphNodeMap (dig),
	arcMap (dig)
{
	//(*Initialize(DigraphAlgorithmCanvas)
	wxFlexGridSizer* graphDrawingCanvasFlexGridSizer;
	wxFlexGridSizer* graphFigureFlexGridSizer;
	wxFlexGridSizer* digraphAlgorithmsFlexGridSizer;

	Create (parent, wxID_ANY, _ ("Graph Algorithms"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T ("wxID_ANY") );
	SetClientSize (wxSize (1024,768) );
	{
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm") ) ) );
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
	digraphAlgorithmsPanel = new wxPanel (graphDrawingCanvasPanel, ID_GRAPH_ALGORITHMS_PANEL, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T ("ID_GRAPH_ALGORITHMS_PANEL") );
	digraphAlgorithmsFlexGridSizer = new wxFlexGridSizer (1, 7, 0, 0);
	digraphAlgorithmsFlexGridSizer->AddGrowableCol (0);
	digraphAlgorithmsFlexGridSizer->AddGrowableCol (5);
	digraphAlgorithmsFlexGridSizer->AddGrowableRow (0);
	digraphAlgorithmsFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 1);
	algorithmsStaticText = new wxStaticText (digraphAlgorithmsPanel, ID_ALGORITHMS_STATICTEXT, _ ("Algorithm Class"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ALGORITHMS_STATICTEXT") );
	digraphAlgorithmsFlexGridSizer->Add (algorithmsStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	algorithmClassesComboBox = new wxComboBox (digraphAlgorithmsPanel, ID_ALGORITHM_CLASSES_COMBOBOX, wxEmptyString, wxDefaultPosition, wxSize (350,25), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T ("ID_ALGORITHM_CLASSES_COMBOBOX") );
	algorithmClassesComboBox->Append (_ ("Graph Search") );
	algorithmClassesComboBox->Append (_ ("Connectivity") );
	algorithmClassesComboBox->Append (_ ("Dijkstra single source") );
	algorithmClassesComboBox->Append (_ ("Maximum flow") );
	algorithmClassesComboBox->Append (_ ("Min Cost Arborescence") );
	algorithmClassesComboBox->Append (_ ("Longest Path in a Digraph") );
	algorithmClassesComboBox->SetToolTip (_ ("Algorithms Arranged in Classes") );
	digraphAlgorithmsFlexGridSizer->Add (algorithmClassesComboBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText1 = new wxStaticText (digraphAlgorithmsPanel, ID_STATICTEXT1, _ ("Algorithm"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_STATICTEXT1") );
	digraphAlgorithmsFlexGridSizer->Add (StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	algorithmsComboBox = new wxComboBox (digraphAlgorithmsPanel, ID_ALGORITHMS_COMBOBOX, wxEmptyString, wxDefaultPosition, wxSize (350,25), 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T ("ID_ALGORITHMS_COMBOBOX") );
	algorithmsComboBox->Append (_ ("BFS") );
	algorithmsComboBox->Append (_ ("DFS") );
	algorithmsComboBox->Append (_ ("DAG") );
	algorithmsComboBox->Append (_ ("Topological Sort") );
	algorithmsComboBox->SetToolTip (_ ("The algorithms available for the current class.") );
	digraphAlgorithmsFlexGridSizer->Add (algorithmsComboBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	runAlgorithmButton = new wxButton (digraphAlgorithmsPanel, ID_RUN_ALGORITHM_BUTTON, _ ("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RUN_ALGORITHM_BUTTON") );
	runAlgorithmButton->SetDefault();
	digraphAlgorithmsFlexGridSizer->Add (runAlgorithmButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	algorithmCanvasCloseButton = new wxButton (digraphAlgorithmsPanel, ID_ALGORITHM_CANVAS_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ALGORITHM_CANVAS_CLOSE_BUTTON") );
	digraphAlgorithmsFlexGridSizer->Add (algorithmCanvasCloseButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	digraphAlgorithmsPanel->SetSizer (digraphAlgorithmsFlexGridSizer);
	digraphAlgorithmsFlexGridSizer->Fit (digraphAlgorithmsPanel);
	digraphAlgorithmsFlexGridSizer->SetSizeHints (digraphAlgorithmsPanel);
	graphDrawingCanvasFlexGridSizer->Add (digraphAlgorithmsPanel, 1, wxALL|wxEXPAND, 1);
	graphDrawingCanvasPanel->SetSizer (graphDrawingCanvasFlexGridSizer);
	graphDrawingCanvasFlexGridSizer->Fit (graphDrawingCanvasPanel);
	graphDrawingCanvasFlexGridSizer->SetSizeHints (graphDrawingCanvasPanel);
	digraphAlgorithmCanvasMenuBar = new wxMenuBar();

	digraphAlgorithmCanvasMenuBar = new wxMenuBar();
	digraphAlgorithmCanvasFileMenu = new wxMenu();
	digraphAlgorithmCanvasFileMenu->Append (wxID_EXIT, wxGetStockLabel (wxID_EXIT) );
#ifndef __APPLE__
	digraphAlgorithmCanvasMenuBar->Append (digraphAlgorithmCanvasFileMenu, wxT ("&File") );
#endif

	digraphAlgorithmHelpMenu = new wxMenu();
	digraphAlgorithmAboutMenuItem = new wxMenuItem (digraphAlgorithmHelpMenu, ID_GRAPH_ALGORITHM_ABOUT_MENUITEM, _ ("About"), wxEmptyString, wxITEM_NORMAL);
	digraphAlgorithmHelpMenu->Append (digraphAlgorithmAboutMenuItem);
	digraphAlgorithmHelpMenu->AppendSeparator();
	digraphAlgorithmCanvasHelpMenu = new wxMenu();
	digraphAlgorithmCanvasHelpMenu->Append (wxID_ABOUT);
	digraphAlgorithmCanvasHelpMenuItem = new wxMenuItem (digraphAlgorithmCanvasHelpMenu, ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	digraphAlgorithmCanvasHelpMenu->Append (digraphAlgorithmCanvasHelpMenuItem);
	digraphAlgorithmCanvasMenuBar->Append (digraphAlgorithmCanvasHelpMenu, _ ("&Help") );
	SetMenuBar (digraphAlgorithmCanvasMenuBar);
	digraphAlgorithmStatusBar = new wxStatusBar (this, ID_GRAPH_ALGORITHM_STATUSBAR, 0, _T ("ID_GRAPH_ALGORITHM_STATUSBAR") );
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	digraphAlgorithmStatusBar->SetFieldsCount (1,__wxStatusBarWidths_1);
	digraphAlgorithmStatusBar->SetStatusStyles (1,__wxStatusBarStyles_1);
	SetStatusBar (digraphAlgorithmStatusBar);
	Center();

	Connect (ID_GRAPH_FIGURE_SLIDER,wxEVT_SCROLL_THUMBTRACK, (wxObjectEventFunction) &DigraphAlgorithmCanvas::OngraphFigureSliderCmdScrollThumbTrack);
	Connect (ID_ALGORITHM_CLASSES_COMBOBOX,wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction) &DigraphAlgorithmCanvas::OnalgorithmClassesComboBoxSelect);
	Connect (ID_ALGORITHM_CANVAS_CLOSE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &DigraphAlgorithmCanvas::OnDigraphAlgorithmCloseButtonPressed);
	Connect (ID_GRAPH_ALGORITHM_ABOUT_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &DigraphAlgorithmCanvas::OndigraphAlgorithmAboutMenuItemSelected);
	Connect (ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (DigraphAlgorithmCanvas::OnDiGraphAlgorithmCanvasHelpMenuItemSelected) );
	//*)
	Connect (ID_RUN_ALGORITHM_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &DigraphAlgorithmCanvas::OnrunAlgorithmButtonClick);
	Connect (ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &DigraphAlgorithmCanvas::handleMenuChoice);

	algorithmClassesComboBox->SetSelection (0);
	algorithmsComboBox->SetSelection (0);
	graphDrawing->SetScale (scale);
	graphDrawing->setSliderAddress (graphFigureSlider);
	graphFigureSlider->SetValue (50*scale);
	dirty = false;
	graphDrawing->getDirty (&dirty);

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

	buildDigraphRepresentation();
}

DigraphAlgorithmCanvas::~DigraphAlgorithmCanvas()
{
	//(*Destroy(DigraphAlgorithmCanvas)
	//*)

}

void DigraphAlgorithmCanvas::OnalgorithmClassesComboBoxSelect (wxCommandEvent& event)
{
	int selectedAlgorithmClass = algorithmClassesComboBox->GetCurrentSelection();

	algorithmsComboBox->Clear();

	switch (selectedAlgorithmClass)
	{
	case GraphSearch:
		algorithmsComboBox->Append (wxT ("BFS") );
		algorithmsComboBox->Append (wxT ("DFS") );
		algorithmsComboBox->Append (wxT ("DAG") );
		algorithmsComboBox->Append (wxT ("Topological sort") );
		break;

	case Connectivity:
		algorithmsComboBox->Append (wxT ("Strongly connected") );
		algorithmsComboBox->Append (wxT ("Strongly connected components") );
		algorithmsComboBox->Append (wxT ("Euler tour") );
		break;

	case DijkstraAlg:
		algorithmsComboBox->Append (wxT ("Dikjstra single source") );
		break;

	case MaximumFlowAlg:
		algorithmsComboBox->Append (wxT ("Edmonds-Karp maximum flow") );
		break;

	case MinCostArborescence:
		algorithmsComboBox->Append (wxT ("Edmond's algorithm") );
		break;

	case LongestPathInDag:
		algorithmsComboBox->Append (wxT ("Longest paths") );
		break;
	}

	algorithmsComboBox->SetSelection (0);
}

void DigraphAlgorithmCanvas::OnrunAlgorithmButtonClick (wxCommandEvent& event)
{
	int selectedAlgorithmClass = algorithmClassesComboBox->GetCurrentSelection();
	int selectedAlgorithm = algorithmsComboBox->GetCurrentSelection();
	int numberOfConnectedComponents;
	wxString msg;

	switch (selectedAlgorithmClass)
	{
	case GraphSearch:
		switch (selectedAlgorithm)
		{
		case BFS:
			digraphBfs();
			break;

		case DFS:
			digraphDfs();
			break;

		case CYCLE:
			if (lemon::dag (dig) )
				report(wxT ("The digraph has no cycles"),wxT("Note"));
			else
				report (wxT ("The digraph has a cycle"),wxT("Note") );

			break;

		case TOPOLOGICAL_SORT:
			topSort();
			break;
		}

		break;

	case Connectivity:
		switch (selectedAlgorithm)
		{
		case STRONGLY_CONNECTED:
			numberOfConnectedComponents = countStronglyConnectedComponents (dig);

			if (numberOfConnectedComponents == 1)
				report (wxT ("The digraph is strongly connected."),wxT("Note"));
			else
			{
				msg.Printf (wxT ("  The digraph has %d strong components."), numberOfConnectedComponents);
				::report (msg,"Note");
			}

			break;

		case STRONGLY_CONNECTED_COMPONENTS:
			drawStrongComponents();
			break;

		case EULER_TOUR:
			digraphEulerTour();
			break;
		}

		break;

	case DijkstraAlg:
		Dijkstra();
		break;

	case MaximumFlowAlg:
		MaxFlow();
		break;

	case MinCostArborescence:
		switch (selectedAlgorithm)
		{
		case EDMONDSALGORITHM:
			drawMinCostArborescence();
			break;
		}

		break;

	case LongestPathInDag:
		switch (selectedAlgorithm)
		{
		case LONGESTPATH:
			longestPathInADag();
			break;
		}

		break;
	}
}

void DigraphAlgorithmCanvas::OngraphFigureSliderCmdScrollThumbTrack (wxScrollEvent& event)
{
	graphDrawing->SetScale (double (graphFigureSlider->GetValue() ) /50);
	graphDrawing->Refresh (false);
}

void DigraphAlgorithmCanvas::OnDiGraphAlgorithmCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}

void DigraphAlgorithmCanvas::OndigraphAlgorithmAboutMenuItemSelected (wxCommandEvent& event)
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

void DigraphAlgorithmCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,4);
}

void DigraphAlgorithmCanvas::OnDigraphAlgorithmCloseButtonPressed (wxCommandEvent& event)
{
	Destroy();
}

void DigraphAlgorithmCanvas::OnQuit (wxCommandEvent& event)
{
	( (DiscreteMathCalculatorFrame *) myParent)->Close (true);
}

void DigraphAlgorithmCanvas::OnCloseWindow (wxCloseEvent& event)
{
	Destroy();
}

void DigraphAlgorithmCanvas::buildDigraphRepresentation()
{
	ogdf::List<node> vertices;
	ogdf::List<node>::iterator vertexIterator;
	ogdf::List<edge> edges;
	ogdf::List<edge>::iterator edgeIterator;
	wxString u, v;
	lemon::ListDigraph::Node node_u, node_v;
	lemon::ListDigraph::Arc e;
	wxString vertexLabel, edgeLabel;
	bool addEdgeWeights = false;
	long weight;
	string nodeLabel;
	int vertexNumber = 1;
	bool handleNoLabels = GA->label (G->firstNode() ) == "";

	G->allNodes (vertices);

	for (vertexIterator = vertices.begin(); vertexIterator != vertices.end(); vertexIterator++)
	{
		if (GA->shape (*vertexIterator) == Shape::Ellipse)
			continue;

		if (handleNoLabels)
		{
			char tmp[10];

			sprintf (tmp, "~%d", vertexNumber++);
			nodeLabel = string (tmp);
			GA->label (*vertexIterator) = nodeLabel;
		}

		node_u = dig.addNode();
		vertexLabel = wxString (GA->label (*vertexIterator).c_str(), wxConvUTF8);
		digraphNodeMap[node_u] = vertexLabel;
		wxString2DiNode[vertexLabel] = node_u;
	}

	if (graphType == digraphw)
		addEdgeWeights = true;

	G->allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		u = wxString (GA->label ( (**edgeIterator).source() ).c_str(), wxConvUTF8);
		v = wxString (GA->label ( (**edgeIterator).target() ).c_str(), wxConvUTF8);

		if (addEdgeWeights)
		{
			edgeLabel = wxString (GA->label (*edgeIterator).c_str(), wxConvUTF8);
			edgeLabel.ToLong (&weight);
		}
		else
			weight = 1;

		node_u = wxString2DiNode[u];
		node_v = wxString2DiNode[v];
		e = dig.addArc (node_u, node_v);
		arcMap[e] = (int) weight;
	}
}

void DigraphAlgorithmCanvas::digraphBfs()
{
	wxString *vertex = new wxString[2];
	lemon::ListDigraph::Node s, t;
	map<wxString, lemon::ListDigraph::Node>::iterator iter;
	wxString output, s_str, t_str;
	bool from_s_only = false;

	vertex[0] = vertex[1] = wxT ("");
	BfsSearch searchData (this, vertex, digraph);

	if (searchData.ShowModal() == wxID_CANCEL)
	{
		delete[] vertex;
		return;
	}

	if (vertex[1] == wxT ("") )
		from_s_only = true;

	if (from_s_only)
	{
		vector<ListDigraph::Node> treeNodes;

		s_str = vertex[0];
		treeNodes = checkAndArrange (wxString2DiNode[s_str], vertex[0]);

		if (treeNodes.size() == 0)
			return;

		MyDigraphBfsVisitor visitor (myParent, &digraphNodeMap, &arcMap, &dig, graphType, forcedirected,
		                             DisplayAlgorithmResults::BFSTREE,true);
		lemon::BfsVisit<lemon::ListDigraph, MyDigraphBfsVisitor> myBfsVisit (dig, visitor);

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
		lemon::Bfs<lemon::ListDigraph> mybfs (dig);
		map<lemon::ListDigraph::Arc, bool> bfsPath;
		vector<ListDigraph::Node> treeNodes;
		bool success = false;
		wxString distanceStr;
		int distance;

		s = wxString2DiNode[vertex[0]];
		treeNodes = checkAndArrange (s, vertex[0]);

		if (treeNodes.size() == 0)
			return;

		t = wxString2DiNode[vertex[1]];

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
					lemon::ListDigraph::Arc prevArc;
					lemon::ListDigraph::Node prevNode = t;

					success = true;
					prevArc = mybfs.predArc (t);

					if (prevArc != lemon::INVALID)
						bfsPath[prevArc] = true;

					prevNode = mybfs.predNode (prevNode);

					while (prevNode != lemon::INVALID)
					{
						if (prevArc != lemon::INVALID)
						{
							bfsPath[prevArc] = true;
							prevArc = mybfs.predArc (prevNode);
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
			reportError (wxT ("The requested path does not exist."),wxT("Error"));
			return;
		}

		distanceStr.Printf (wxT ("The distance from %s to %s is %d"),
		                    vertex[0].c_str(), vertex[1].c_str(), distance);
		MyDigraphBfsPathVisitor pathVisitor (this, &digraphNodeMap, &arcMap, &dig, graphType, bfsPath, distanceStr);
		lemon::BfsVisit<lemon::ListDigraph, MyDigraphBfsPathVisitor> myBfsPathVisit (dig, pathVisitor);

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

void DigraphAlgorithmCanvas::digraphDfs()
{
	wxString *vertex = new wxString[2];
	lemon::ListDigraph::Node s, t;
	map<wxString, lemon::ListDigraph::Node>::iterator iter;
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
		vector<ListDigraph::Node> treeNodes;

		s_str = vertex[0];
		treeNodes = checkAndArrange (wxString2DiNode[s_str], s_str);

		if (treeNodes.size() == 0)
			return;

		MyDigraphDfsVisitor visitor (myParent, algorithmOutput, &digraphNodeMap, &arcMap,
		                             &dig, graphType);
		lemon::DfsVisit<lemon::ListDigraph, MyDigraphDfsVisitor> myDfsVisit (dig, visitor);

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
		lemon::Dfs<lemon::ListDigraph> mydfs (dig);
		map<lemon::ListDigraph::Arc, bool> dfsPath;
		vector<ListDigraph::Node> treeNodes;
		bool success = false;

		s = wxString2DiNode[vertex[0]];
		treeNodes = checkAndArrange (s, vertex[0]);

		if (treeNodes.size() == 0)
			return;

		t = wxString2DiNode[vertex[1]];

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
					lemon::ListDigraph::Arc prevArc;
					lemon::ListDigraph::Node prevNode = t;

					success = true;
					prevArc = mydfs.predArc (t);

					if (prevArc != lemon::INVALID)
						dfsPath[prevArc] = true;

					prevNode = mydfs.predNode (prevNode);

					while (prevNode != lemon::INVALID)
					{
						if (prevArc != lemon::INVALID)
						{
							dfsPath[prevArc] = true;
							prevArc = mydfs.predArc (prevNode);
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
			reportError (wxT ("The requested path does not exist."),wxT("Error") );
			return;
		}

		MyDigraphDfsPathVisitor pathVisitor (myParent, &digraphNodeMap, &arcMap, &dig, dfsPath, graphType);
		lemon::DfsVisit<lemon::ListDigraph, MyDigraphDfsPathVisitor> myDfsPathVisit (dig, pathVisitor);

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

void DigraphAlgorithmCanvas::topSort()
{
	if (!lemon::dag (dig) )
	{
		reportError (wxT ("Your digraph is not acyclic.\nThere is no topological sort."),wxT("Error"));
		return;
	}

	lemon::ListDigraph::NodeMap<int> topOrderMap (dig);
	map<int, lemon::ListDigraph::Node> positionToVertex;
	int pos, n;
	wxString output;
	lemon::ListDigraph::Node vertex;
	int vertexCount = 0, maxVertexLength = 0, len;
	wxString formatSpecs;
	ShowSequence* topologicalListing;

	n = countNodes (dig);
	lemon::topologicalSort (dig, topOrderMap);

	for (lemon::ListDigraph::NodeIt n (dig); n != lemon::INVALID; ++n)
	{
		pos = topOrderMap[n];
		positionToVertex[pos] = n;

		if ( (len = digraphNodeMap[n].length() ) > maxVertexLength)
			maxVertexLength = len;
	}

	topologicalListing = new ShowSequence (this,"bfs-digraph.png",
	                                       ShowSequence::TOPOLOGICALSORT);

	formatSpecs.Printf (wxT ("%%-%ds "),maxVertexLength+3);

	for (int i=0; i < n; i++)
	{
		output.Printf (formatSpecs, digraphNodeMap[positionToVertex[i]].c_str() );
		topologicalListing->WriteLine (output);
		vertexCount++;

		if (vertexCount % 8 == 0)
			topologicalListing->WriteLine (wxT ("\n") );
	}

	topologicalListing->WriteLine (wxT ("\n") );
	topologicalListing->Show (true);
}

vector<lemon::ListDigraph::Node> DigraphAlgorithmCanvas::checkAndArrange (lemon::ListDigraph::Node t,
      const wxString& vertexLabel)
{
	wxString output;
	vector<ListDigraph::Node> treeNodes;
	bool success = false;

	for (ListDigraph::NodeIt n (dig); n != INVALID; ++n)
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
		reportError (output,wxT("Error"));
		treeNodes.clear();
	}

	return treeNodes;
}

bool DigraphAlgorithmCanvas::check (lemon::ListDigraph::Node t, const wxString& vertexLabel)
{
	wxString output;
	bool success = false;

	for (ListDigraph::NodeIt n (dig); n != INVALID; ++n)
		if (n == t)
		{
			success = true;
			break;
		}

	if (!success)
	{
		output.Printf (wxT ("%s is not a vertex."), vertexLabel.c_str() );
		reportError (output,wxT("Error"));
	}

	return success;
}

void DigraphAlgorithmCanvas::drawStrongComponents()
{
	int numStrongComp;
	lemon::ListDigraph::NodeMap<int> compMap (dig);
	lemon::ListDigraph::NodeMap<wxColour> colorMap (dig);
	wxString msg;


	numStrongComp = stronglyConnectedComponents (dig, compMap);
// Build a list of vertices belonging to each strong component.
    msg.Printf(
        wxT("Number of strongly connected components: %d\n"),
        numStrongComp);

    for (int component = 0; component < numStrongComp; ++component)
    {
        wxString members;

        for (ListDigraph::NodeIt n(dig); n != INVALID; ++n)
        {
            if (compMap[n] == component)
            {
                if (!members.IsEmpty())
                    members += wxT(", ");

                members += digraphNodeMap[n];
            }
        }

        wxString componentLine;
        componentLine.Printf(
            wxT("Component %d: %s\n"),
            component + 1,
            members.c_str());

        msg += componentLine;
    }


	vector<wxColour> componentColor;
	colorPicker strongComponentColors;

	strongComponentColors.PickColor (componentColor, numStrongComp);

	for (ListDigraph::NodeIt n (dig); n != INVALID; ++n)
	{
		wxColour c = componentColor[compMap[n]];

		colorMap[n] = componentColor[compMap[n]];
	}

	MyDigraphBfsVisitor visitor (myParent, &digraphNodeMap, &arcMap, &dig, digraph,
	                             circular, DisplayAlgorithmResults::COMPONENTS, true, &colorMap, msg);
	lemon::BfsVisit<lemon::ListDigraph, MyDigraphBfsVisitor> myBfsVisit (dig, visitor);

	myBfsVisit.init();

	for (ListDigraph::NodeIt n (dig); n != INVALID; ++n)
		if (!myBfsVisit.reached (n) )
		{
			myBfsVisit.addSource (n);
			myBfsVisit.start();
		}

	visitor.drawTree();
}

void DigraphAlgorithmCanvas::Dijkstra()
{
	typedef lemon::ListDigraph::Node Node;

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

	if (graphType != digraphw)
	{
		::report (wxT ("The graph must be weighted"),"Note");
		return;
	}
// Dijkstra requires nonnegative edge weights.
for (ListDigraph::ArcIt a(dig); a != INVALID; ++a)
{
    if (arcMap[a] < 0)
    {
        ::report(
            wxT("Dijkstra cannot be used with negative edge weights."),
            "Note");
        return;
    }
}
	OptimumPathInput dijkstraInput (this, &wxStringStartingVertex, &wxStringFinalVertex, true, digraphw);

	if (dijkstraInput.ShowModal() == wxID_CANCEL)
		return;

	s = wxString2DiNode[wxStringStartingVertex];
	t = wxString2DiNode[wxStringFinalVertex];

	if (!dig.valid (s) || !dig.valid (t) )
	{
		::report (wxT ("Either the starting or ending vertex is not in the graph"),"Note");
		return;
	}

	// initialize all vertices to unmarked (WHITE) and dataValues to INF
	for (ListDigraph::NodeIt n (dig); n != INVALID; ++n)
	{
		str = digraphNodeMap[n];
		vInfo = VertexInfo (str,wxT (""),INF);
		colorNode[str] = VertexInfo::WHITE;
		node2VInfo[str] = vInfo;
	}

	// formulate initial priority queue entry
	str = digraphNodeMap[s];
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
		currVertex = wxString2DiNode[str];

		if (currVertex == t)
		{
			foundMinPath = true;
			break;
		}

		if (colorNode[str] != VertexInfo::BLACK)
		{
			// mark the vertex so we don't look at it again
			colorNode[str] = VertexInfo::BLACK;

			// find all neighbors of the current vertex vInfo.node. for each
			// neighbor that has not been visited, generate a VertexInfo
			// object and insert it into the priority queue provided the
			// total weight to get to the neighbor is better than the
			// current dataValue in the neighbor
			for (ListDigraph::OutArcIt a (dig, currVertex); a!=INVALID; ++a)
			{
				dest = dig.target (a);
				src = digraphNodeMap[dest];
				neighbor_vInfo = node2VInfo[src];

				if (colorNode[neighbor_vInfo.node] == VertexInfo::WHITE)
				{
					// compare total weight of adding edge
					if ( (newMinWeight = vInfo.dataValue +
					                     arcMap[a]) < neighbor_vInfo.dataValue)
					{
						// add VertexInfo object for new vertex
						neighbor_vInfo.node = src;
						neighbor_vInfo.dataValue = newMinWeight;
						neighbor_vInfo.parent = digraphNodeMap[currVertex];
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
			src = digraphNodeMap[currVertex];
			path.push_front (src);
			src = node2VInfo[src].parent;
			currVertex = wxString2DiNode[src];
		}

		path.push_front (digraphNodeMap[s]);
		minPathLength = node2VInfo[digraphNodeMap[t]].dataValue;
		drawDijkstraPath (path);
	}
	else
		::report (wxT ("There is no path between the vertices."),"Note");
}

void DigraphAlgorithmCanvas::drawDijkstraPath (const list<wxString>& path)
{
	typedef lemon::ListDigraph::Node Node;

	BuildGraph *DijkstraPath;
	wxString DijkstraGraphLabel, tempResult, label,
	         srcLabel, destLabel, vertexWeight;
	list<wxString>::const_iterator iter;
	Node src, dest;
	bool minPathEdge;
	int arcWeight;
	DisplayAlgorithmResults *minPathGraph;

	DijkstraPath = new BuildGraph (wxT(""), digraphw, hierarchical, true);
	for (ListDigraph::ArcIt a (dig); a != INVALID; ++a)
	{
		src = dig.source (a);
		dest = dig.target (a);
		srcLabel = digraphNodeMap[src];
		destLabel = digraphNodeMap[dest];
		minPathEdge = edgeInPath (path, srcLabel, destLabel);
		arcWeight = arcMap[dig.arcFromId (dig.id (a) )];
		vertexWeight.Printf (wxT ("%d"),arcWeight);

		if (minPathEdge)
			DijkstraPath->insertEdge (srcLabel,
			                          destLabel,
			                          vertexWeight, wxSHORTEST_PATH_PEN,
			                          wxColour(255,215,0), wxColour(255,215,0));
		else if (!edgeInPath (path,destLabel,srcLabel) )
			DijkstraPath->insertEdge (srcLabel,
			                          destLabel,
			                          vertexWeight, wxNORMAL_PEN,
			                          wxColour(255,215,0), wxColour(255,215,0));
	}

	DijkstraPath->forceDirectedLayout();
	minPathGraph = new DisplayAlgorithmResults (this, DijkstraPath, digraphw, hierarchical,
	      DisplayAlgorithmResults::MINIMUM_PATH);

	DijkstraGraphLabel.Printf (wxT ("Minimum path length = %d\n"),minPathLength);

	iter = path.begin();

	while (iter != path.end() )
	{
		tempResult.Printf (wxT ("%s  "), *iter);
		DijkstraGraphLabel += tempResult;
		iter++;
	}
	minPathGraph->writeOutputLine(DijkstraGraphLabel);

	minPathGraph->Show();

	minPathGraph->getDisplayAlgorithmResultsTextCtrl()->Show();
	minPathGraph->getDisplayAlgorithmResultsTextCtrl()->Refresh();
	minPathGraph->getDisplayAlgorithmResultsTextCtrl()->Update();

	wxTheApp->SetTopWindow(minPathGraph);
}

bool DigraphAlgorithmCanvas::edgeInPath (const list<wxString>& path,
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

void DigraphAlgorithmCanvas::MaxFlow()
{
	typedef lemon::ListDigraph::Node Node;

	int numsource = 0, numsink = 0;
	Node source, sink;


	if (graphType != digraphw)
	{
		::report (wxT ("The digraph must be weighted."),"Note");
		return;
	}
	// Maximum Flow requires nonnegative edge capacities.
	for (ListDigraph::ArcIt a(dig); a != INVALID; ++a)
	{
	    if (arcMap[a] < 0)
	    {
	        ::report(
	            wxT("Maximum Flow requires nonnegative edge capacities."),
	            "Note");
	        return;
	    }
	}

	for (lemon::ListDigraph::NodeIt n (dig); n != lemon::INVALID; ++n)
	{
		if ( (lemon::countInArcs (dig, n) == 0) && (lemon::countOutArcs (dig, n) > 0) )
		{
			numsource++;
			source = n;
		}
		else if ( (lemon::countOutArcs (dig, n) == 0) && (lemon::countInArcs (dig, n) > 0) )
		{
			numsink++;
			sink = n;
		}
	}

	if (numsource != 1)
	{
		::report (wxT ("There must be exactly one source."),"Note");
		return;
	}

	if (numsink != 1)
	{
		report (wxT ("There must be exactly one sink."),"Note");
		return;
	}

	lemon::EdmondsKarp< lemon::ListDigraph, lemon::ListDigraph::ArcMap<int>>
	      maxFlow (dig,arcMap,source,sink);
	int maximumFlow;
	map<Node,bool> onSourceSide;

	maxFlow.run();
	maximumFlow = maxFlow.flowValue();

	lemon::ListDigraph::ArcMap<int> flow (dig);

	for (lemon::ListDigraph::ArcIt a (dig); a != INVALID; ++a)
	{
		flow[a] = maxFlow.flowMap() [a];
	}

	for (lemon::ListDigraph::NodeIt n (dig); n != lemon::INVALID; ++n)
	{
		onSourceSide[n] = maxFlow.minCut (n);
	}

	BuildGraph *maxFlowDiGraph;
	wxString maxFlowLabel, edgeFlow, srcLabel, destLabel;
	Node src, dest;
	DisplayAlgorithmResults *maxFlowDiGraphDisplay;
	wxColour destSideOfCut (255, 215, 0, 0), srcColour, destColour;


	// Calculate the capacity of the minimum cut.
    int minimumCutCapacity = 0;

    for (ListDigraph::ArcIt a(dig); a != INVALID; ++a)
    {
        if (onSourceSide[dig.source(a)] &&
            !onSourceSide[dig.target(a)])
        {
            minimumCutCapacity += arcMap[a];
        }
    }

    // List the vertices on each side of the minimum cut.
    wxString sourceSide, sinkSide;

    for (ListDigraph::NodeIt n(dig); n != INVALID; ++n)
    {
        wxString label = digraphNodeMap[n];

        if (onSourceSide[n])
        {
            if (!sourceSide.IsEmpty())
                sourceSide += wxT(", ");

            sourceSide += label;
        }
        else
        {
            if (!sinkSide.IsEmpty())
                sinkSide += wxT(", ");

            sinkSide += label;
        }
    }

    maxFlowLabel.Printf(
        wxT("Maximum flow = %d\n"
            "Minimum cut capacity = %d\n\n"
            "Source side: %s\n"
            "Sink side: %s\n"),
        maximumFlow,
        minimumCutCapacity,
        sourceSide.c_str(),
        sinkSide.c_str());

	maxFlowDiGraph = new BuildGraph (wxT(""), digraphw, forcedirected, true);

	for (ListDigraph::ArcIt a (dig); a != INVALID; ++a)
	{
		edgeFlow.Clear();
		src = dig.source (a);
		dest = dig.target (a);
		srcLabel = digraphNodeMap[src];
		destLabel = digraphNodeMap[dest];
		edgeFlow.Printf (wxT ("%d"),flow[a]);

		if (onSourceSide[src])
			srcColour = *wxRED;
		else
			srcColour = destSideOfCut;

		if (onSourceSide[dest])
			destColour = *wxRED;
		else
			destColour = destSideOfCut;

		if (edgeFlow[0] != wxUniChar ('0') )
			maxFlowDiGraph->insertEdge (srcLabel,
			                            destLabel,
			                            edgeFlow, wxNORMAL_PEN, wxColour(255,215,0), wxColour(255,215,0));
		else
			maxFlowDiGraph->insertEdge (srcLabel,
			                            destLabel,
			                            edgeFlow, wxCROSS_EDGE_PEN, wxColour(255,215,0), wxColour(255,215,0));
	}

	maxFlowDiGraph->hierarchicalLayout();
	maxFlowDiGraphDisplay = new DisplayAlgorithmResults (this, maxFlowDiGraph, digraphw, hierarchical,
	      DisplayAlgorithmResults::MAXFLOW);
	maxFlowDiGraphDisplay->writeOutputLine(maxFlowLabel);
	maxFlowDiGraphDisplay->Show();
	wxTheApp ->SetTopWindow (maxFlowDiGraphDisplay);
}

bool DigraphAlgorithmCanvas::pathSearch (ListDigraph::Node sVertex,
      ListDigraph::Node tVertex,
      ListDigraph::Node currVertex,
      vector<wxString>& pathVertices,
      int d)
{
	ListDigraph::Node neighborVertex;

	if (currVertex == tVertex)
	{
		if (d == 0)
		{
			pathVertices.push_back (digraphNodeMap[currVertex]);
			return true;
		}
		else
			return false;
	}

	hamPathColor[currVertex] = VertexInfo::BLACK;

	for (ListDigraph::OutArcIt a (dig, currVertex); a != INVALID; ++a)
	{
		neighborVertex = dig.target (a);

		if (hamPathColor[neighborVertex] == VertexInfo::WHITE)
			if (pathSearch (sVertex,tVertex,neighborVertex,pathVertices,d-1) )
			{
				pathVertices.push_back (digraphNodeMap[currVertex]);
				return true;
			}
	}

	hamPathColor[currVertex] = VertexInfo::WHITE;
	return false;
}

void DigraphAlgorithmCanvas::digraphEulerTour()
{
	ListDigraph::ArcMap<int> visitationNumber (dig, 0);
	vector<ListDigraph::Node> nodeSequence;

	lemon::DiEulerIt<ListDigraph> e (dig);

	if (e == INVALID)
		return;

	lemon::ListDigraph::Node firstNode = dig.source (ListDigraph::Arc (e) );
	lemon::ListDigraph::Node lastNode = dig.target (ListDigraph::Arc (e) );

	nodeSequence.push_back (firstNode);

	for (; e != INVALID; ++e)
	{
		if (e != INVALID)
		{
			lastNode = dig.target (ListDigraph::Arc (e) );
			nodeSequence.push_back (lastNode);
		}

		visitationNumber[e] = visitationNumber[e] + 1;
	}

	if (firstNode != lastNode)
	{
		::report (wxT ("First and last nodes are not the same."),"Note");
		return;
	}

	for (ListDigraph::ArcIt e (dig); e != INVALID; ++e)
	{
		if (visitationNumber[e] != 1)
		{
			::report (wxT ("Edge not visited or an edge visited multiple times."),"Note");
			return;
		}
	}

	drawDigraphEulerTour (nodeSequence);
}

void DigraphAlgorithmCanvas::drawDigraphEulerTour (const vector<ListDigraph::Node>& nodeList)
{
	BuildGraph *myDigraphEulerTour;
	wxString src, dest;
	DisplayAlgorithmResults *myDigraphEulerCircuit;
	int edgeNumber = 1;
	wxString edgeLabel, temp;

	myDigraphEulerTour = new BuildGraph (wxT (""), digraphw, hierarchical, true);

	for (unsigned int i=0; i < nodeList.size()-1; i++)
	{
		src = digraphNodeMap[nodeList[i]];
		dest = digraphNodeMap[nodeList[i+1]];
		edgeLabel.Printf (wxT ("%d"), edgeNumber);
		edgeNumber++;
		myDigraphEulerTour->insertEdge (src, dest, edgeLabel, wxwxDASHED_GRAPH_EDGE_PEN,
		                                wxColour(255,215,0), wxColour(255,215,0));
	}

	myDigraphEulerTour->hierarchicalLayout();
	myDigraphEulerCircuit = new DisplayAlgorithmResults (this, myDigraphEulerTour, digraphw, hierarchical,
	      DisplayAlgorithmResults::EULERTOUR);

	for (unsigned int i=0; i < nodeList.size()-1; i++)
	{
		src = digraphNodeMap[nodeList[i]];
		dest = digraphNodeMap[nodeList[i+1]];
		temp.Printf (wxT (" %s-%s  "), src, dest);
		myDigraphEulerCircuit->writeOutputLine (temp);
	}

	myDigraphEulerCircuit->Show();
	wxTheApp ->SetTopWindow (myDigraphEulerCircuit);
}

void DigraphAlgorithmCanvas::drawMinCostArborescence()
{
	if (graphType != digraphw)
	{
		::report (wxT ("Your digraph must be weighted."),"Note");
		return;
	}

	if (!lemon::dag (dig) )
	{
		::report (wxT ("Your digraph is not acyclic."),"Note");
		return;
	}

	typedef ListDigraph::ArcMap<int> CostMap;
	typedef lemon::ListDigraph::Node Node;

	CostMap cost (dig);
	int totalcost;
	Node source;
	lemon::ListDigraph::ArcMap<bool> arborescence (dig);
	BuildGraph *arborescenceGraphConstruction;
	DisplayAlgorithmResults *arborescenceGraph;
	Node src, dest;
	wxString srcLabel, destLabel, edgeWeight;
	wxString *sourceVertex = new wxString (wxT ("") );
	wxString totalMinCost;

	MinCostArborescenceDialog minCostArborescenceData (this, sourceVertex, "mincostarborescence-dialog.png");

	if (minCostArborescenceData.ShowModal() == wxID_CANCEL)
	{
		delete sourceVertex;
		return;
	}

	source = wxString2DiNode[*sourceVertex];

	for (ListDigraph::ArcIt a (dig); a != INVALID; ++a)
		cost[a] = (int) arcMap[a];

	totalcost = lemon::minCostArborescence (dig, cost, source, arborescence);

	// build the arborescence graph
	arborescenceGraphConstruction = new BuildGraph (wxT(""), digraphw,
	      hierarchical, true);

	for (ListDigraph::ArcIt a (dig); a != INVALID; ++a)
	{
		src = dig.source (a);
		dest = dig.target (a);
		srcLabel = digraphNodeMap[src];
		destLabel = digraphNodeMap[dest];

		edgeWeight.Printf (wxT ("%d"),  cost[a]);

		if (arborescence[a] == true)
			arborescenceGraphConstruction->insertEdge (srcLabel,
			      destLabel,
			      edgeWeight, wxNORMAL_PEN,
			      wxColour(255,215,0), wxColour(255,215,0));
	}

	arborescenceGraphConstruction->hierarchicalLayout();
	arborescenceGraph = new DisplayAlgorithmResults (this, arborescenceGraphConstruction, digraphw, hierarchical,
	      DisplayAlgorithmResults::MINCOSTARBORESCENCE);
	totalMinCost.Printf (wxT ("The minimum cost from %s is %d."),
	                     *sourceVertex, totalcost );
	arborescenceGraph->writeOutputLine(totalMinCost);

	arborescenceGraph->Show();
	wxTheApp->SetTopWindow (arborescenceGraph);
}

void DigraphAlgorithmCanvas::longestPathInADag()
{
	if (graphType != digraphw)
	{
		::report (wxT ("Your digraph must be weighted."),"Note" );
		return;
	}

	if (!lemon::dag (dig) )
	{
		::report (wxT ("Your digraph is not acyclic."),"Note");
		return;
	}

	typedef lemon::ListDigraph::Node Vertex;
	typedef lemon::ListDigraph::Arc Arc;

	Vertex source, target;
	wxString wxStringStartingVertex, wxStringFinalVertex;

	LongestPathInput longestPathInput (this, &wxStringStartingVertex, &wxStringFinalVertex);

	if (longestPathInput.ShowModal() == wxID_CANCEL)
		return;

	source = wxString2DiNode[wxStringStartingVertex];
	target = wxString2DiNode[wxStringFinalVertex];

	if (!dig.valid (source) || !dig.valid (target) )
	{
		reportError (wxT ("Either the starting or ending vertex is not in the graph"),"Note");
		return;
	}

	lemon::ListDigraph::NodeMap<int> topOrderMap (dig);
	map<int, Vertex> positionToVertex;
	map<Vertex,int> longestDistFromSource;
	map<Vertex,Vertex> parent;
	int pos, numVertices, i;
	Vertex currVertex, neighbor, p;
	std::set<Arc> pathVertices;
	Arc a;
	BuildGraph *longestPathGraphConstruction;
	DisplayAlgorithmResults *longestPathGraph;
	Vertex src, dest;
	wxString srcLabel, destLabel, edgeWeight, longestPathValue;

	numVertices = G->numberOfNodes();
	lemon::topologicalSort (dig, topOrderMap);

	for (lemon::ListDigraph::NodeIt n (dig); n != lemon::INVALID; ++n)
	{
		pos = topOrderMap[n];
		positionToVertex[pos] = n;
	}

	const int MINUSINFINITY = -2147483648;

	for (i = 0; i < numVertices; i++)
		longestDistFromSource[positionToVertex[i]] = MINUSINFINITY;

	longestDistFromSource[source] = 0;
	parent[source] = source;

	for (i = 0; i < numVertices; i++)
	{
		currVertex = positionToVertex[i];

		for (ListDigraph::OutArcIt a (dig, currVertex); a!=INVALID; ++a)
		{
			neighbor = dig.target (a);

			if (longestDistFromSource[neighbor] <
			      longestDistFromSource[currVertex] + arcMap[a])
			{
				longestDistFromSource[neighbor] =
				   longestDistFromSource[currVertex] + arcMap[a];
				parent[neighbor] = currVertex;
			}
		}
	}

	// draw the dag and mark the longest path from source to target.
	currVertex = target;

	while (parent[currVertex] != currVertex)
	{
		p = parent[currVertex];
		a = lemon::findArc (dig,p,currVertex);
		pathVertices.insert (a);
		currVertex = p;
	}


	// build the longest path graph
	longestPathGraphConstruction = new BuildGraph (wxT(""), digraphw,
	      hierarchical, true);

	for (ListDigraph::ArcIt a (dig); a != INVALID; ++a)
	{
		src = dig.source (a);
		dest = dig.target (a);
		srcLabel = digraphNodeMap[src];
		destLabel = digraphNodeMap[dest];

		edgeWeight.Printf (wxT ("%d"), arcMap[a]);

		if (pathVertices.find (a) != pathVertices.end() )
			longestPathGraphConstruction->insertEdge (srcLabel,
			      destLabel,
			      edgeWeight, wxPen(wxColour(220,0,0), 2, wxPENSTYLE_SOLID),
			      wxColour(220,0,0), wxColour(220,0,0));
		else
			longestPathGraphConstruction->insertEdge (srcLabel,
			      destLabel,
			      edgeWeight, wxPen(wxColour(0,0,0), 1, wxPENSTYLE_SOLID),
			      wxColour(0,0,0), wxColour(0,0,0));
	}

	longestPathGraphConstruction->hierarchicalLayout();


	longestPathGraph = new DisplayAlgorithmResults (this, longestPathGraphConstruction, digraphw, hierarchical,
	      DisplayAlgorithmResults::MINCOSTARBORESCENCE);
	longestPathValue.Printf (wxT ("Longest path from %s to %s has value %d."),
	                         wxStringStartingVertex, wxStringFinalVertex,
	                         longestDistFromSource[target] );
	longestPathGraph->writeOutputLine(longestPathValue);
	longestPathGraph->Show();
	wxTheApp ->SetTopWindow (longestPathGraph);
}
