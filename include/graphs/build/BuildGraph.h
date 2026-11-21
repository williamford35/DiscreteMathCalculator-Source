#ifndef BUILDGRAPH_H_INCLUDED
#define BUILDGRAPH_H_INCLUDED

#include <lemon/list_graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include "graphs/GraphType.h"
#include "graphs/draw/DrawGraph.h"
#include "graphs/enter/EnterGraphDialog.h"
#include "RelationsPartialOrders/RelationInput.h"

#include <wx/textfile.h>
#include <wx/font.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <map>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/EdgeComparer.h>

#include "common/randomc.h"

using namespace lemon;
using namespace std;
using namespace ogdf;

class BuildGraph: public wxEvtHandler
{
public:

	BuildGraph (const wxString& title, GraphType type, GraphLayout layout,
	            bool forceFFD700 = false, bool parallelEdges = false, wxWindow* parent = NULL);
	virtual ~BuildGraph();
	void insertEdge (const wxString& src, const wxString& dest,
	                 const wxString& strWeight = wxT (""),
	                 const wxPen& edgePen = *wxBLACK_PEN,
	                 const wxColour& srcColor = wxColour (255, 215, 0, 0),
	                 const wxColour& destColor = wxColour (255, 215, 0, 0),
	                 bool addSpecialArrow = false);
	void addLabel (const wxString& label);
	void hierarchicalLayout (bool arrangeCCs = true);
	void forceDirectedLayout (int unitEdgeLen = 100);
	bool orthogonalLayout();
	void circularLayout();
	void treeLayout();
	void buildFromGML(const wxString& gfilename, GraphType *gtype,
	                  bool *parallelEdges, bool hideEdges = false, bool standardVertexColor = false);
	bool buildRelationFromREL(const wxString& relFileName,bool hideEdges = false);
	bool buildRelationFromGML (const wxString& gfilename, bool isHasseDiagram);
	void buildFromGRPH (const wxString& gfilename, bool hideEdges = false, bool useStandardVertexColor = false);
	ogdf::Graph *getGraph();
	GraphAttributes *getGraphAttributes();
	void setDrawingCanvas (DrawingCanvas *canvas);
	void eraseCanvas(bool eraseGraph = false);
	void addVertexToEmptyGraph (const wxString& v);
	bool addVertex (const wxString& v);
	bool eraseVertex (const wxString& v);
	bool addEdge (const wxString& v, const wxString& w, long weight = -2147483647L);

	bool eraseEdge (const wxString& v, const wxString& w, bool drawGraph = true);
	void redraw (GraphType type, GraphLayout layout, double scale, bool hideEdges, wxWindow *discMathCalcMain);
	void getGraphType (GraphType& gtype, bool& parallelEdges);
	void buildTournamentDigraph (int numVertices);
	bool buildDigraphWithStrongComponents (int numComponents,int maxPerComponent);
	void drawRandomGraph (RandomGraphType rtype, int m, int n);
	bool drawRandomDigraph (RandomDigraphType rdtype, int m, int n, wxRadioBox* graphLayoutRadioBox);
	void drawKn (int n);
	void drawGridGraph(int m, int n);
	void drawTSP (int n);
	void drawHamCycle (int numVertices, int numEdges);
	void drawHypergraph (int n);
	void buildDigraphRepresentation (ListDigraph& g, ListDigraph::NodeMap<wxString>& nodeMap, ListDigraph::ArcMap<int>& arcMap,
	                                 map<wxString, ListDigraph::Node>& wxStr2DiNode);
	void buildUndirectedGraphRepresentation (ListGraph& g, ListGraph::NodeMap<wxString>& nodeMap, ListGraph::EdgeMap<int>& edgeMap,
	      map<wxString, ListGraph::Node>& wxStr2UNode);

	void launchRunAlgorithms(wxWindow* parent = NULL);
	void drawMyDialogGraph (const vector<GraphEdge>& g);
	void drawGeneralizedPetersonGraph (int n, int k);
	void drawGraphWithClique (int cliqueSize, int numberOfVertices, int numberOfEdges);
	void drawTransposeGraph(wxWindow *discMathCalcMain);
	void drawComplementaryGraph(wxWindow *discMathCalcMain);
	bool hasParallelEdges();
	GraphLayout getGraphLayout();
	int numberOfSelfLoops();
	void drawMyRelationGraph (const vector<RelationGraphEdge>& dig, bool drawDirectedGraph, wxWindow *discMathCalcMain,
                             const vector<wxString>& vertices = vector<wxString>());
	void buildFromTSP (const wxString& tspfilename, bool hideEdges = false);

private:
	void init();
	void buildGraphFromTextfile (wxTextFile *grphFile,bool hideEdges = false);
	void buildGraphForTSP (wxTextFile *grphFile, bool hideEdges = false);
	node findVertex (const wxString& v);
	ogdf::edge findEdge (const wxString& v, const wxString& w);
	void setLayout (GraphLayout graphLayout);
	bool parallelEdges (const List<ogdf::edge>& edgeList);
	bool checkForSelfLoops();
	void assignVertexLabelsAndWeights();
	bool edgeBetween (node u, node v);
	bool originalEdge (const vector<edge>& originalEdges, edge e);
	void listCombinations (int n, int cn3);
	void comb_next (int n, int a[], bool *done);
	void makeStrongComponent (ogdf::Graph &g, int numVertices);
	void addStrongComponent (ogdf::Graph& G, Graph& component);
	void randomGraph(Graph &G, int n, int m);
	void labelTSPvertices(int n, vector<node>& nodeArr);
	void generate_TSP_graph(int n);

	node graphLabel;
	wxString graphFigureTitle;
	GraphType graphType;
	GraphLayout graphLayout;
	ogdf::Graph G;
	ogdf::GraphAttributes GA;
	map<wxString, node> str2node;
	DrawingCanvas *drawingCanvas;
	wxFont vertexFont;
	DrawGraph *drawMyGraph;
	bool allowParallelEdges;
	wxColour edgeColor;
	int edgeStyle;
	bool drawingCompleteGraph;
	vector< pair<int, int> > coordinates;
	map< string, pair<int,int> > node2Coor;
	CRandomMersenne rnd;
	bool requireFFD700;

	struct threeVertices
	{
		int node[3];
	};

	threeVertices *triangle;
};

#endif // BUILDGRAPH_H_INCLUDED
