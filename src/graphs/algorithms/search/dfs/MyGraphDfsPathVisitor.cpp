/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/dfs/MyGraphDfsPathVisitor.h"

MyGraphDfsPathVisitor::MyGraphDfsPathVisitor (wxWindow* parent, ListGraph::NodeMap<wxString> *nodeMap,
      ListGraph::EdgeMap<int> *amap, ListGraph *grph,
      const std::map<lemon::ListGraph::Edge, bool>& ip, GraphType type) :
	graphNodeMap (nodeMap), g (grph), inPath (ip), graphType (type),
	myParent(parent)
{
	if (graphType == undirw)
		graphType = undir;

	dfsTree = new BuildGraph (wxT (""), graphType, hierarchical, false);

	// add all of the vertices
	for (ListGraph::NodeIt n (*g); n != INVALID; ++n)
		dfsTree->insertEdge ( (*graphNodeMap) [n], wxT ("#"));
}

MyGraphDfsPathVisitor::~MyGraphDfsPathVisitor()
{}

void MyGraphDfsPathVisitor::start (const Node &node)
{}

void MyGraphDfsPathVisitor::stop (const Node &node)
{}

void MyGraphDfsPathVisitor::reach (const Node &node)
{
}

void MyGraphDfsPathVisitor::discover (const Arc &arc)
{
	lemon::ListGraph::Edge edge = (lemon::ListGraph::Edge) arc;
	int weight;
	wxString w = wxT("");

	if (graphType == undirw)
	{
		weight = (*arcMap)[arc];
		w.Printf("%d", weight);
	}

	u = g->u (edge);
	u_str = (*graphNodeMap) [u];
	v = g->v (edge);
	v_str = (*graphNodeMap) [v];

	if (inPath[edge])
		dfsTree->insertEdge (u_str, v_str, w, wxTREE_EDGE_PEN);
	else
		dfsTree->insertEdge (u_str, v_str, w, wxNORMAL_PEN);
}

void MyGraphDfsPathVisitor::examine (const Arc &arc)
{
	lemon::ListGraph::Edge edge = (lemon::ListGraph::Edge) arc;
	int weight;
	wxString w = wxT("");

	if (graphType == undirw)
	{
		weight = (*arcMap)[arc];
		w.Printf("%d", weight);
	}

	u = g->u (edge);
	u_str = (*graphNodeMap) [u];
	v = g->v (edge);
	v_str = (*graphNodeMap) [v];

	dfsTree->insertEdge (u_str, v_str, wxT(""),wxNORMAL_PEN);
}

void MyGraphDfsPathVisitor::leave (const Node &node)
{
}

void MyGraphDfsPathVisitor::backtrack (const Arc &arc)
{}

void MyGraphDfsPathVisitor::drawTree()
{
	dfsTree->hierarchicalLayout();
	theTree = new DisplayAlgorithmResults (myParent, dfsTree, graphType, hierarchical,DisplayAlgorithmResults::DFSPATH);
	theTree->Show();
	wxTheApp ->SetTopWindow (theTree);
}
