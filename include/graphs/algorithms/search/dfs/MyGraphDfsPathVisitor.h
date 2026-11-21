#ifndef MYGRAPHDFSPATHVISITOR_H
#define MYGRAPHDFSPATHVISITOR_H

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

class MyGraphDfsPathVisitor: public DfsVisitor<ListGraph>
{
public:
	MyGraphDfsPathVisitor (wxWindow* parent, ListGraph::NodeMap<wxString> *nodeMap,
	                       ListGraph::EdgeMap<int> *amap, ListGraph *grph,
	                       const std::map<lemon::ListGraph::Edge, bool>& ip, GraphType type);
	virtual ~MyGraphDfsPathVisitor();

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

	ListGraph::NodeMap<wxString> *graphNodeMap;
	ListGraph::ArcMap<int> *arcMap;
	ListGraph *g;
	std::map<lemon::ListGraph::Edge, bool> inPath;
	wxString output;
	Node u, v;
	wxString u_str, v_str;
	BuildGraph *dfsTree;
	DisplayAlgorithmResults *theTree;
	GraphType graphType;
	wxWindow* myParent;
};

#endif // MYGRAPHDFSPATHVISITOR_H
