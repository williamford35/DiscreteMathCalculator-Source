/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/bfs/MyGraphBfsPathVisitor.h"

MyGraphBfsPathVisitor::MyGraphBfsPathVisitor (wxWindow* parent, ListGraph::NodeMap<wxString> *nodeMap,
      ListGraph::EdgeMap<int> *emap, ListGraph *grph, GraphType type,
      const std::map<lemon::ListGraph::Edge, bool>& ip, const wxString& distStr) :
	graphNodeMap (nodeMap), edgeMap(emap), g (grph), inPath (ip), distanceLabel (distStr), graphType (type),
	myParent(parent)
{
	if (graphType == undirw)
		graphType = undir;

	bfsTree = new BuildGraph (wxT (""), digraph, hierarchical, false);

	// add all of the vertices
	for (ListGraph::NodeIt n (*g); n != INVALID; ++n)
		bfsTree->insertEdge ( (*graphNodeMap) [n], wxT ("#"));
}

MyGraphBfsPathVisitor::~MyGraphBfsPathVisitor()
{}

void MyGraphBfsPathVisitor::start (const Node &node)
{}

void MyGraphBfsPathVisitor::reach (const Node &node)
{
}

void MyGraphBfsPathVisitor::process (const Node &node)
{
}

void MyGraphBfsPathVisitor::discover (const Arc &arc)
{
	lemon::ListGraph::Edge edge = (lemon::ListGraph::Edge) arc;
	Node src, dest;
	wxString w = wxT("");
	int weight;

	src = g->u (edge);
	u_str = (*graphNodeMap) [src];
	dest = g->v (edge);
	v_str = (*graphNodeMap) [dest];

	if (inPath[edge])
	{
		wxString currStrEdge;

		bfsTree->insertEdge (u_str, v_str, w, wxTREE_EDGE_PEN);
		currStrEdge.Printf(wxT("%s-%s  "), u_str, v_str);
		bfsPath.push_back(currStrEdge);
	}
	else
		bfsTree->insertEdge (u_str, v_str, w, wxNORMAL_PEN);
}

void MyGraphBfsPathVisitor::examine (const Arc &arc)
{
	lemon::ListGraph::Edge edge = (lemon::ListGraph::Edge) arc;
	Node src, dest;
	wxString w = wxT("");
	int weight;

	src = g->u (edge);
	u_str = (*graphNodeMap) [src];
	dest = g->v (edge);
	v_str = (*graphNodeMap) [dest];

	bfsTree->insertEdge (u_str, v_str, w);
}

void MyGraphBfsPathVisitor::drawTree()
{
	int numberEdges = 1;

	bfsTree->hierarchicalLayout();
	theTree = new DisplayAlgorithmResults (myParent, bfsTree, graphType, hierarchical,
	                                       DisplayAlgorithmResults::BFSPATH, distanceLabel);
	theTree->writeOutputLine(wxT("BFS path edges:\n\n"));

	for (int i=0; i < bfsPath.size(); i++)
	{
		theTree->writeOutputLine(bfsPath[i]);

		if (numberEdges % 25 == 0)
			theTree->writeOutputLine(wxT("\n"));

		numberEdges++;
	}

	theTree->Show();
	wxTheApp ->SetTopWindow (theTree);
}
