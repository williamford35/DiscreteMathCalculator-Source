#ifndef GRAPHDRAWINGCANVAS_H
#define GRAPHDRAWINGCANVAS_H

#include <lemon/concepts/graph.h>
#include <lemon/maps.h>
#include <lemon/list_graph.h>
#include <lemon/bfs.h>
#include <lemon/dfs.h>
#include <lemon/connectivity.h>
#include <lemon/full_graph.h>

#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"
#include "graphs/GraphType.h"
#include "common/DrawingCanvas.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/algorithms/VertexInfo.h"
#include "graphs/algorithms/WidestPathVertexInfo.h"
#include "graphs/algorithms/InputIntegerData.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <sstream>

using namespace std;
// using namespace ogdf;
// using namespace lemon;

//(*HeadersPCH(GraphAlgorithmCanvas)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/combobox.h>
#include <wx/statusbr.h>
//*)
//(*Headers(GraphAlgorithmCanvas)
#include "common/DrawingCanvas.h"
//*)

class GraphAlgorithmCanvas : public wxFrame
{
public:

	GraphAlgorithmCanvas (wxWindow* parent, wxSFDiagramManager mgr,
	                      double scale, ogdf::GraphAttributes *ga, ogdf::Graph *grph, wxWindowID id, const wxPoint& pos, const wxSize& size,
	                      GraphType gtype, BuildGraph *bg = NULL);
	virtual ~GraphAlgorithmCanvas();

	//(*Declarations(GraphAlgorithmCanvas)
	wxComboBox* algorithmClassesComboBox;
	wxStaticText* algorithmsStaticText;
	wxStatusBar* graphAlgorithmStatusBar;
	wxComboBox* algorithmsComboBox;
	wxMenuItem* graphAlgorithmQuitMenuItem;
	wxStaticText* graphAlgorithmCanvasAlgorithmStaticText;
	wxPanel* graphDrawingCanvasPanel;
	wxMenuBar* graphAlgorithmCanvasMenuBar;
	wxButton* algorithmCanvasCloseButton;
	wxPanel* graphFigurePanel;
	DrawingCanvas* graphDrawing;
	wxSlider* graphFigureSlider;
	wxPanel* graphAlgorithmsPanel;
	wxMenu* graphAlgorithmCanvasFileMenu;
	wxButton* runAlgorithmButton;
	//*)
	wxMenuItem* graphAlgorithmCanvasPrintMenuItem;
	wxMenuItem* graphAlgorithmCanvasPrintPreviewMenuItem;
	wxMenuItem* graphAlgorithmCanvasPageSetupMenuItem;
	wxMenuItem* graphAlgorithmCanvasSaveAsMenuItem;
	wxMenu* graphAlgorithmCanvasEditMenu;
	wxMenuItem* graphAlgorithmCanvasCopyImageMenuItem;
	wxMenu* graphAlgorithmCanvasHelpMenu;
	wxMenuItem* graphAlgorithmCanvasHelpMenuItem;

protected:
	//(*Identifiers(GraphAlgorithmCanvas)
	static const long ID_GRAPH_DRAWING;
	static const long ID_GRAPH_FIGURE_SLIDER;
	static const long ID_GRAPH_FIGURE_PANEL;
	static const long ID_ALGORITHMS_STATICTEXT;
	static const long ID_ALGORITHM_CLASSES_COMBOBOX;
	static const long ID_GRAPH_ALGORITHM_CANVAS_ALGORITHMS_STATICTEXT;
	static const long ID_ALGORITHMS_COMBOBOX;
	static const long ID_RUN_ALGORITHM_BUTTON;
	static const long ID_ALGORITHM_CANVAS_CLOSE_BUTTON;
	static const long ID_GRAPH_ALGORITHMS_PANEL;
	static const long ID_GRAPH_DRAWING_CANVAS_PANEL;
	static const long ID_GRAPH_ALGORITHM_QUIT_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_STATUSBAR;
	//*)
	static const long ID_GRAPH_ALGORITHM_CANVAS_PRINT_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_PRINT_PREVIEW_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_PAGESETUP_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_SAVE_AS_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM;

private:

	//(*Handlers(GraphAlgorithmCanvas)
	void OnalgorithmClassesComboBoxSelect (wxCommandEvent& event);
	void OnrunAlgorithmButtonClick (wxCommandEvent& event);
	void OngraphFigureSliderCmdScrollThumbTrack (wxScrollEvent& event);
	void OngraphAlgorithmAboutMenuItemSelected (wxCommandEvent& event);
	void OngraphAlgorithmCloseButtonPressed (wxCommandEvent& event);
	void OnGraphAlgorithmCanvasHelpMenuItemSelected (wxCommandEvent& event);
	//*)
	void handleMenuChoice (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);

	enum AlgorithmClass
	{
		GraphSearch,
		Connectivity,
		MinimumPath,
		WidestPath,
		Planarity,
		MaximumIndendentEdgeSet,
		NPHardProblems
	};

	enum SearchAlgorithms
	{
		BFS,
		DFS,
		CYCLE,
	};

	enum ConnectivityAlgorithms
	{
		CONNECTED,
		CONNECTED_COMPONENTS,
		EULER_TOUR,
	};

	enum MinimumPathAlgorithms
	{
		DIJKSTRA,
		PRIM
	};

	enum WidestPathAlgorithm
	{
		WIDESTPATH
	};

	enum PlanarityAlgorithm
	{
		ISPLANAR
	};

	enum MaxIndependentEdgeSet
	{
		MAXIMUMCARDINALITY,
		MAXIMUMWEIGHTED
	};

	enum NPHardProblems
	{
		MAXIMUM_CLIQUE,
		INDEPENDENT_SET_VERTEX_COVER,
		HAMILTONIAN_CYCLE,
        HAMILTONIAN_PATH,
		TSP,
		GRAPH_COLORING
	};

	enum ColorOfNode
	{
		WHITE,
		BLACK
	};

	void buildGraphRepresentation();
	void graphBfs();
	void graphDfs();
	vector<lemon::ListGraph::Node> checkAndArrange (lemon::ListGraph::Node t,
	      const wxString& vertexLabel);
	bool check (lemon::ListGraph::Node t, const wxString& vertexLabel);
	void drawComponents();
	void eulerTour(const ListGraph& g, const ListGraph::Node& start = INVALID);
	void Dijkstra();
	void Prim();
	void MaximumSpanningTree();
	void FindWidestPath();
	void drawEulerTour (const vector<lemon::ListGraph::Node>& nodeList);
	void drawMaxIndependentEdgeSetCardinalityMatching ();
	void drawMaxIndependentEdgeSetWeightedMatching ();
	vector<int> searchForMaximumClique (const ListGraph& g,
	                                    map<int, ListGraph::Node>& intToNode,
	                                    map<ListGraph::Node, int>& nodeToInt);
	string intToStr (int n);
	void drawMaximumClique();
	void drawMaximumIndependentSet();
	void drawVertexCover();
	bool cycleSearch (lemon::ListGraph::Node sVertex, lemon::ListGraph::Node currVertex,
	                  vector<wxString>& cycleVertices,
	                  int d);
	bool lookForHamiltonianCycle (const lemon::ListGraph& g,
	                              vector<wxString>& cycleVertices);
	void drawHamiltonianCycle();
    void drawHamiltonianPath();
	void beginColoring();
	void solveTSP();
	bool colorGraph (lemon::ListGraph::Node n, unsigned numColors);
	int  chooseColor (lemon::ListGraph::Node n, unsigned numColors, int startingColor);
	void drawColoredGraph (int numColors);

	bool edgeInPath (const list<wxString>& path,
	                 const wxString& src,
	                 const wxString& dest);

	template <typename T>
	bool inVector (const vector<T>& v, T target);
	bool edgeInCycle (const vector<wxString>& hamiltonianCycle,
	                  const wxString& src, const wxString& dest);
	void drawDijkstraPath (const list<wxString>& path);
	void drawWidestPathGraph ();
	void isPlanar();
	void makeKruskalMapInput();
	typedef ListGraph::EdgeMap<bool> EBoolMap;
	void widestPath(const wxString& s, const wxString& t);
	bool checkVertexLabel(const wxString& s);
	bool findIndependentSetNode(const vector<wxString>& v, const wxString& n);
	// void removeAndSaveGraphTitle(GraphAttributes *ga, Graph *g);

	wxSFDiagramManager m_canvas;
	bool dirty;
	MenuEventHandler *menuHandler;
	GraphType graphType;
	ogdf::Graph *G;
	ogdf::GraphAttributes *GA;
	int minPathLength;

	// declarations for an undirected graph
	lemon::ListGraph ug;
	lemon::ListGraph compGraph;
	lemon::ListGraph::NodeMap<wxString> node2wxString;
	lemon::ListGraph::EdgeMap<int> edgeMap;
	map<wxString, lemon::ListGraph::Node> wxString2Node;
	map<lemon::ListGraph::Node, ColorOfNode> color;
	map<wxString, ColorOfNode> nodeColor;
	map<lemon::ListGraph::Node, ColorOfNode> widestPathNodeColor;
	map<wxString, VertexInfo> node2VInfo;
	map<lemon::ListGraph::Node, WidestPathVertexInfo> widestPathNode2VInfo;
	std::set<lemon::ListGraph::Arc> spanningTreeEdge;
	map<lemon::ListGraph::Node, int> colorOfNode;
	wxTextCtrl *algorithmOutput;
	bool figureSaved;
	BuildGraph *builtGraph;
	wxWindow* myParent;
	lemon::ListGraph::EdgeMap<double> sortedWeights;
	BuildGraph *maxSpanningTree, *widestPathGraph, *combinedGraphs;
	EBoolMap *tree_map;
	EBoolMap *widestpath_map;
	string originalTitle;
	BuildGraph* myMaxIndependentSet;
	vector<wxString> indepSetVertex;
	vector<wxString> vertexCoverVertex;
	bool displayMaximumIndependentSet;

	DECLARE_EVENT_TABLE()
};

#endif // GRAPHDRAWINGCANVAS_H
