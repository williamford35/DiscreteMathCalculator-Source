/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/bfs/MyDigraphBfsPathVisitor.h"

MyDigraphBfsPathVisitor::MyDigraphBfsPathVisitor (wxWindow* parent, ListDigraph::NodeMap<wxString> *nodeMap,
      ListDigraph::ArcMap<int> *emap, ListDigraph *grph, GraphType type,
      const std::map<lemon::ListDigraph::Arc, bool>& ip, const wxString& distStr) :
	diGraphNodeMap (nodeMap), edgeMap(emap), g (grph), inPath (ip), distanceLabel (distStr), graphType (type),
	myParent(parent)
{
	if (graphType == digraphw)
		graphType = digraph;

	bfsTree = new BuildGraph (wxT (""), digraph, hierarchical, false);

	// add all of the vertices
	for (ListDigraph::NodeIt n (*g); n != INVALID; ++n)
		bfsTree->insertEdge ( (*diGraphNodeMap) [n], wxT ("#"));
}

MyDigraphBfsPathVisitor::~MyDigraphBfsPathVisitor()
{}

void MyDigraphBfsPathVisitor::start (const Node &node)
{}

void MyDigraphBfsPathVisitor::reach (const Node &node)
{
}

void MyDigraphBfsPathVisitor::process (const Node &node)
{
}

void MyDigraphBfsPathVisitor::discover (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*diGraphNodeMap) [u];
	v = g->target (arc);
	v_str = (*diGraphNodeMap) [v];
	int weight;
	wxString w = wxT("");

	if (inPath[arc])
	{
		wxString currStrEdge;

		bfsTree->insertEdge (u_str, v_str, wxT (""), wxSHORTEST_PATH_PEN);
		currStrEdge.Printf(wxT("%s->%s  "), u_str, v_str);
		bfsPath.push_back(currStrEdge);
	}
	else
		bfsTree->insertEdge (u_str, v_str, wxT (""), wxNORMAL_PEN);
}

void MyDigraphBfsPathVisitor::examine (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*diGraphNodeMap) [u];
	v = g->target (arc);
	v_str = (*diGraphNodeMap) [v];
	int weight;
	wxString w = wxT("");

	bfsTree->insertEdge (u_str, v_str, w, wxNORMAL_PEN);
}

void MyDigraphBfsPathVisitor::drawTree()
{
	int numberEdges = 1;

	bfsTree->forceDirectedLayout();
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
