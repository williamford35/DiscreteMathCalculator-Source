#ifndef MYGRAPHDFSVISITOR_H
#define MYGRAPHDFSVISITOR_H

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

class MyGraphDfsVisitor: public DfsVisitor<ListGraph>
{
public:
	MyGraphDfsVisitor (wxWindow* parent, wxTextCtrl *out,
	                   ListGraph::NodeMap<wxString> *nodeMap,
	                   ListGraph::EdgeMap<int> *emap,
	                   ListGraph *grph, GraphType type);
	virtual ~MyGraphDfsVisitor();

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
	std::map<Node, int> d, f;
	wxTextCtrl *cout;
	ListGraph::NodeMap<wxString> *graphNodeMap;
	ListGraph::EdgeMap<int> *edgeMap;
	wxString output;
	Node u, v;
	ListGraph *g;
	wxString u_str, v_str;
	int time;
	bool genOutput;
	BuildGraph *dfsTree;
	DisplayAlgorithmResults *theTree;
	GraphType graphType;
	int numVertices, leaveCount;
	vector<wxString> visitList;
	wxWindow* myParent;
};

#endif // MYGRAPHDFSVISITOR_H
