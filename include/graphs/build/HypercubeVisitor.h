#ifndef HYPERCUBEVISITOR_H
#define HYPERCUBEVISITOR_H
#include <lemon/list_graph.h>
#include <lemon/bfs.h>

using namespace lemon;

#include <map>
#include <set>
using namespace std;

#include <wx/textctrl.h>
#include <wx/string.h>
#include "graphs/build/GraphEdgeStyles.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"
#include "graphs/GraphType.h"
#include <lemon/hypercube_graph.h>

class HypercubeVisitor: public BfsVisitor<HypercubeGraph>
{
public:
	HypercubeVisitor (HypercubeGraph *grph, BuildGraph *myGraph);

	virtual ~HypercubeVisitor();

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

private:
	wxString toBinary (int dim, int n);
	map<lemon::HypercubeGraph::Node,wxString> graphNodeMap;
	wxString output, msg;
	Node u, v;
	HypercubeGraph *g;
	wxString u_str, v_str;
	BuildGraph *hypercubeGraph;
	std::set<lemon::HypercubeGraph::Edge> id;
	std::set<lemon::HypercubeGraph::Edge>::iterator iter;
};

#endif // MYGRAPHBFSVISITOR_H
