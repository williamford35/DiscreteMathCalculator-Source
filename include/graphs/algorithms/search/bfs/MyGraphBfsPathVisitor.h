#ifndef MYGRAPHBFSPATHVISITOR_H
#define MYGRAPHBFSPATHVISITOR_H

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

class MyGraphBfsPathVisitor: public BfsVisitor<ListGraph>
{
public:
	MyGraphBfsPathVisitor (wxWindow* parent, ListGraph::NodeMap<wxString> *nodeMap,
	                       ListGraph::EdgeMap<int> *emap, ListGraph *grph, GraphType type,
	                       const std::map<lemon::ListGraph::Edge, bool>& ip,
	                       const wxString& distStr);
	virtual ~MyGraphBfsPathVisitor();

	// Called for the source node of the BFS.
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

	ListGraph::NodeMap<wxString> *graphNodeMap;
	ListGraph::EdgeMap<int> *edgeMap;
	ListGraph *g;
	std::map<lemon::ListGraph::Edge, bool> inPath;
	wxString output;
	Node u, v;
	vector<wxString> bfsPath;
	wxString u_str, v_str, distanceLabel;
	BuildGraph *bfsTree;
	DisplayAlgorithmResults *theTree;
	GraphType graphType;
	wxWindow* myParent;
};

#endif // MYGRAPHBFSPATHVISITOR_H
