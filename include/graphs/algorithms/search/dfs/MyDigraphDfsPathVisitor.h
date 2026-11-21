#ifndef MYDIGRAPHDFSPATHVISITOR_H
#define MYDIGRAPHDFSPATHVISITOR_H

#include <lemon/list_graph.h>
#include <lemon/dfs.h>
using namespace lemon;

#include <map>

#include <wx/textctrl.h>
#include <wx/string.h>
#include "graphs/build/GraphEdgeStyles.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"
#include "graphs/GraphType.h"

class MyDigraphDfsPathVisitor: public DfsVisitor<ListDigraph>
{
public:
	MyDigraphDfsPathVisitor (wxWindow* parent, ListDigraph::NodeMap<wxString> *nodeMap,
	                         ListDigraph::ArcMap<int> *amap, ListDigraph *grph,
	                         const std::map<lemon::ListDigraph::Arc, bool>& ip, GraphType type);
	virtual ~MyDigraphDfsPathVisitor();

	// Called for the source node of the DFS.
	void start (const Node &node);

	// Called when the source node has finished.
	void stop (const Node &node);

	// Called when a node is reached the first time.
	void reach (const Node &node);

	// Called when an arc reaches a new node.
	// This is a tree edge
	void discover (const Arc &arc);

	// Called when an arc is examined but its target node is already discovered.
	void examine (const Arc &arc);

	// Called when the DFS steps back from a node.
	void leave (const Node &node);

	// Called when the DFS steps back on an arc.
	void backtrack (const Arc &arc);

	void generateOutput (bool b);
	void drawTree();

private:

	ListDigraph::NodeMap<wxString> *diGraphNodeMap;
	ListDigraph::ArcMap<int> *arcMap;
	ListDigraph *g;
	std::map<lemon::ListDigraph::Arc, bool> inPath;
	wxString output;
	Node u, v;
	wxString u_str, v_str;
	BuildGraph *dfsTree;
	DisplayAlgorithmResults *theTree;
	GraphType graphType;
	wxWindow* myParent;
};

#endif // MYDIGRAPHDFSPATHVISITOR_H
