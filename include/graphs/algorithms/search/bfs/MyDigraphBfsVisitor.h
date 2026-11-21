#ifndef MYDIGRAPHBFSVISITOR_H
#define MYDIGRAPHBFSVISITOR_H

#include <lemon/list_graph.h>
#include <lemon/bfs.h>
using namespace lemon;

#include <map>

#include <wx/textctrl.h>
#include <wx/string.h>
#include "graphs/build/GraphEdgeStyles.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"
#include "graphs/GraphType.h"

class MyDigraphBfsVisitor: public BfsVisitor<ListDigraph>
{
public:
	MyDigraphBfsVisitor (wxWindow* parent, ListDigraph::NodeMap<wxString> *nodeMap,
	                     ListDigraph::ArcMap<int> *emap, ListDigraph *grph, GraphType type, GraphLayout layout,
	                     DisplayAlgorithmResults::Algorithm myTreeType, bool displayList =false,
	                     ListDigraph::NodeMap<wxColor> *colorMap = NULL,
	                     const wxString& m = wxT (""));
	virtual ~MyDigraphBfsVisitor();

	// Called for the source node(s) of the BFS. .
	void start (const Node &node);

	// Called when a node is reached the first time.
	void reach (const Node &node);

	// Called when a node is processed.
	void process (const Node &node);

	// Called when an arc reaches a new node.
	void discover (const Arc &arc);

	// Called when an arc is examined but its target node is already discovered.
	void examine (const Arc &arc);

	void drawTree();

private:

	std::map<Node, int> distance;
	ListDigraph::NodeMap<wxString> *diGraphNodeMap;
	ListDigraph::ArcMap<int> *edgeMap;
	ListDigraph *g;
	bool coloringTheGraph;
	wxString output, msg;
	Node u, v;
	wxString u_str, v_str;
	int time;
	BuildGraph *bfsTree;
	DisplayAlgorithmResults *theTree;
	GraphType graphType;
	GraphLayout graphLayout;
	DisplayAlgorithmResults::Algorithm treeType;
	vector<wxString> visitList;
	bool callShowSequence;
	wxWindow* myParent;
};

#endif // MYDIGRAPHBFSVISITOR_H
