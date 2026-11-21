#ifndef DIGRAPHDRAWINGCANVAS_H
#define DIGRAPHDRAWINGCANVAS_H

#include <lemon/concepts/digraph.h>
#include <lemon/maps.h>
#include <lemon/list_graph.h>
#include <lemon/bfs.h>
#include <lemon/dfs.h>
#include <lemon/connectivity.h>

#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"
#include "graphs/GraphType.h"
#include "graphs/algorithms/ShowSequence.h"
#include "graphs/algorithms/VertexInfo.h"
#include "common/DrawingCanvas.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include "graphs/algorithms/VertexInfo.h"

#include <vector>
#include <set>
#include <map>

using namespace std;
using namespace ogdf;

//(*HeadersPCH(DigraphAlgorithmCanvas)
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
//(*Headers(DigraphAlgorithmCanvas)
#include "common/DrawingCanvas.h"
//*)

class DigraphAlgorithmCanvas: public wxFrame
{
public:
	typedef lemon::ListDigraph::Arc Arc;

	DigraphAlgorithmCanvas (wxWindow* parent, wxSFDiagramManager mgr,
	                        double scale, ogdf::GraphAttributes *ga, ogdf::Graph *grph, wxWindowID id, const wxPoint& pos, const wxSize& size,
	                        GraphType gtype);
	virtual ~DigraphAlgorithmCanvas();

	//(*Declarations(DigraphAlgorithmCanvas)
	wxMenuItem* digraphAlgorithmTrianglesHelpMenuItem;
	wxMenuItem* digraphAlgorithmGraphsHelpMenuItem;
	wxComboBox* algorithmClassesComboBox;
	wxStaticText* algorithmsStaticText;
	wxMenuItem* digraphAlgorithmAboutMenuItem;
	wxMenuItem* digraphAlgorithmTreesHelpMenuItem;
	wxStatusBar* digraphAlgorithmStatusBar;
	wxComboBox* algorithmsComboBox;
	wxMenuItem* digraphAlgorithmQuitMenuItem;
	wxPanel* graphDrawingCanvasPanel;
	wxMenuBar* digraphAlgorithmCanvasMenuBar;
	wxButton* algorithmCanvasCloseButton;
	wxStaticText* StaticText1;
	wxPanel* graphFigurePanel;
	wxMenuItem* digraphAlgorithmCountingAndEnumerationHelpMenuItem;
	DrawingCanvas* graphDrawing;
	wxMenu* digraphAlgorithmHelpMenu;
	wxSlider* graphFigureSlider;
	wxMenuItem* digraphAlgorithmPartitionsHelpMenuItem;
	wxPanel* digraphAlgorithmsPanel;
	wxMenuItem* digraphAlgorithmSubsetsHelpMenuItem;
	wxMenu* digraphAlgorithmCanvasFileMenu;
	wxButton* runAlgorithmButton;
	//*)
	wxMenuItem* digraphAlgorithmCanvasPrintMenuItem;
	wxMenuItem* digraphAlgorithmCanvasPrintPreviewMenuItem;
	wxMenuItem* digraphAlgorithmCanvasPageSetupMenuItem;
	wxMenuItem* digraphAlgorithmCanvasSaveAsMenuItem;
	wxMenu* digraphAlgorithmCanvasEditMenu;
	wxMenuItem* digraphAlgorithmCanvasCopyImageMenuItem;
	wxMenu* digraphAlgorithmCanvasHelpMenu;
	wxMenuItem* digraphAlgorithmCanvasHelpMenuItem;

protected:
	static const long ID_GRAPH_DRAWING;
	static const long ID_GRAPH_FIGURE_SLIDER;
	static const long ID_GRAPH_FIGURE_PANEL;
	static const long ID_ALGORITHMS_STATICTEXT;
	static const long ID_ALGORITHM_CLASSES_COMBOBOX;
	static const long ID_STATICTEXT1;
	static const long ID_ALGORITHMS_COMBOBOX;
	static const long ID_RUN_ALGORITHM_BUTTON;
	static const long ID_ALGORITHM_CANVAS_CLOSE_BUTTON;
	static const long ID_GRAPH_ALGORITHMS_PANEL;
	static const long ID_GRAPH_DRAWING_CANVAS_PANEL;
	static const long ID_GRAPH_ALGORITHM_QUIT_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_ABOUT_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_STATUSBAR;
	//*)
	static const long ID_GRAPH_ALGORITHM_CANVAS_PRINT_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_PRINT_PREVIEW_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_PAGESETUP_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_SAVE_AS_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_COPY_IMAGE_MENUITEM;

private:

	//(*Handlers(DigraphAlgorithmCanvas)
	void OnalgorithmClassesComboBoxSelect (wxCommandEvent& event);
	void OngraphFigureSliderCmdScrollThumbTrack (wxScrollEvent& event);
	void OndigraphAlgorithmAboutMenuItemSelected (wxCommandEvent& event);
	void OnDigraphAlgorithmCloseButtonPressed (wxCommandEvent& event);
	void OnDiGraphAlgorithmCanvasHelpMenuItemSelected (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);

	//*)
	void handleMenuChoice (wxCommandEvent& event);
	enum ColorOfVertex
	{
		WHITE,
		BLACK
	};

	enum AlgorithmClass
	{
		GraphSearch,
		Connectivity,
		DijkstraAlg,
		MaximumFlowAlg,
		MinCostArborescence,
		LongestPathInDag,
		NP_Complete
	};

	enum SearchAlgorithms
	{
		BFS,
		DFS,
		CYCLE,
		TOPOLOGICAL_SORT
	};

	enum ConnectivityAlgorithms
	{
		STRONGLY_CONNECTED,
		STRONGLY_CONNECTED_COMPONENTS,
		EULER_TOUR
	};

	enum DijkstraAlgorithm
	{
		SINGLE_SOURCE_SHORTEST_PATH_TO_SINGLE_VERTEX
	};

	enum MinCostArborescenceAlgorithm
	{
		EDMONDSALGORITHM
	};

	enum LongestPathInDirAcyclicGraph
	{
		LONGESTPATH
	};

	void OnrunAlgorithmButtonClick (wxCommandEvent& event);
	void buildDigraphRepresentation();
	void buildUndirectedGraphRepresentation();
	void digraphBfs();
	void graphBfs();
	void digraphDfs();
	void topSort();
	void Dijkstra();
	void MaxFlow();
	void digraphEulerTour();
	void drawDigraphEulerTour (const vector<lemon::ListDigraph::Node>& nodeList);
	void createOutputFrame();
	void isStronglyConnected();
	vector<lemon::ListDigraph::Node> checkAndArrange (lemon::ListDigraph::Node t,
	      const wxString& vertexLabel);
	bool check (lemon::ListDigraph::Node t, const wxString& vertexLabel);
	void drawStrongComponents();
	bool findVertex (const string& v, node& n);
	void drawDijkstraPath (const list<wxString>& path);
	int minPathLength;

	bool edgeInPath (const list<wxString>& path,
	                 const wxString& src,
	                 const wxString& dest);
	bool pathSearch (lemon::ListDigraph::Node sVertex, lemon::ListDigraph::Node tVertex,
	                 lemon::ListDigraph::Node currVertex,
	                 vector<wxString>& pathVertices, int d);
	void longestPathInADag();
	void drawMinCostArborescence();

	wxSFDiagramManager m_canvas;
	bool dirty;
	MenuEventHandler *menuHandler;
	GraphType graphType;
	ogdf::Graph *G;
	ogdf::GraphAttributes *GA;

	// declarations for a digraph
	lemon::ListDigraph dig;
	lemon::ListDigraph::NodeMap<wxString> digraphNodeMap;
	map<wxString, lemon::ListDigraph::Node> wxString2DiNode;
	lemon::ListDigraph::ArcMap<int> arcMap;
	map<wxString, VertexInfo> node2VInfo;
	map<wxString,VertexInfo::NodeColor> colorNode;
	map<lemon::ListDigraph::Node,VertexInfo::NodeColor> hamPathColor;
	map<int,lemon::ListDigraph::Arc> arcFromId;
	map<lemon::ListDigraph::Arc, bool> arcNew;
	wxTextCtrl *algorithmOutput;
	bool figureSaved;
	wxWindow* myParent;

	DECLARE_EVENT_TABLE()
};

#endif
