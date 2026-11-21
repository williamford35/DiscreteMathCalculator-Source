/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/dfs/MyDigraphDfsPathVisitor.h"

MyDigraphDfsPathVisitor::MyDigraphDfsPathVisitor (wxWindow* parent, ListDigraph::NodeMap<wxString> *nodeMap,
      ListDigraph::ArcMap<int> *amap, ListDigraph *grph,
      const std::map<lemon::ListDigraph::Arc, bool>& ip, GraphType type) :
	diGraphNodeMap (nodeMap), arcMap(amap), g (grph), inPath (ip), graphType (type),
	myParent(parent)
{
	dfsTree = new BuildGraph (wxT (""), graphType, hierarchical, false);

	// add all of the vertices
	for (ListDigraph::NodeIt n (*g); n != INVALID; ++n)
		dfsTree->insertEdge ( (*diGraphNodeMap) [n], wxT ("#"));
}

MyDigraphDfsPathVisitor::~MyDigraphDfsPathVisitor()
{}

void MyDigraphDfsPathVisitor::start (const Node &node)
{}

void MyDigraphDfsPathVisitor::stop (const Node &node)
{}

void MyDigraphDfsPathVisitor::reach (const Node &node)
{
}

void MyDigraphDfsPathVisitor::discover (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*diGraphNodeMap) [u];
	v = g->target (arc);
	v_str = (*diGraphNodeMap) [v];
	int weight;
	wxString w = wxT("");

	if (graphType == digraphw)
	{
		weight = (*arcMap)[arc];
		w.Printf("%d", weight);
	}

	if (inPath[arc])
		dfsTree->insertEdge (u_str, v_str, w, wxTREE_EDGE_PEN);
	else
		dfsTree->insertEdge (u_str, v_str, w, wxNORMAL_PEN);
}

void MyDigraphDfsPathVisitor::examine (const Arc &arc)
{
	int weight;
	wxString w = wxT("");

	if (graphType == digraphw)
	{
		weight = (*arcMap)[arc];
		w.Printf("%d", weight);
	}

	u = g->source (arc);
	u_str = (*diGraphNodeMap) [u];
	v = g->target (arc);
	v_str = (*diGraphNodeMap) [v];

	dfsTree->insertEdge (u_str, v_str, w, wxNORMAL_PEN);
}

void MyDigraphDfsPathVisitor::leave (const Node &node)
{
}

void MyDigraphDfsPathVisitor::backtrack (const Arc &arc)
{}

void MyDigraphDfsPathVisitor::drawTree()
{
	dfsTree->hierarchicalLayout();
	theTree = new DisplayAlgorithmResults (myParent, dfsTree, graphType, hierarchical, DisplayAlgorithmResults::DFSPATH);
	theTree->Show();
	wxTheApp ->SetTopWindow (theTree);
}
