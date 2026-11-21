/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/bfs/MyDigraphBfsVisitor.h"
#include "graphs/algorithms/ShowSequence.h"

MyDigraphBfsVisitor::MyDigraphBfsVisitor (wxWindow* parent, ListDigraph::NodeMap<wxString> *nodeMap,
      ListDigraph::ArcMap<int> *emap, ListDigraph *grph, GraphType type, GraphLayout layout,
      DisplayAlgorithmResults::Algorithm myTreeType,
      bool displayList, ListDigraph::NodeMap<wxColor> *colorMap, const wxString& m) :
	diGraphNodeMap (nodeMap), edgeMap (emap), g (grph), callShowSequence (displayList), coloringTheGraph (false),
	msg (m), graphType (type), graphLayout (layout), treeType (myTreeType),
	myParent (parent)
{
	if (graphType == digraphw)
		graphType = digraph;

	// add all of the vertices
	if (colorMap != NULL)
	{
		coloringTheGraph = true;
		bfsTree = new BuildGraph (wxT(""), digraph, graphLayout, false);

		for (ListDigraph::NodeIt n (*g); n != INVALID; ++n)
			bfsTree->insertEdge ( (*diGraphNodeMap) [n], wxT ("#"), wxT (""), *wxBLACK_PEN, (*colorMap) [n]);
	}
	else
	{
		bfsTree = new BuildGraph (wxT (""), digraph, graphLayout, false);

		for (ListDigraph::NodeIt n (*g); n != INVALID; ++n)
			bfsTree->insertEdge ( (*diGraphNodeMap) [n], wxT ("#") );
	}

	visitList.push_back (wxT ("BFS list:\n\n") );
}

MyDigraphBfsVisitor::~MyDigraphBfsVisitor()
{
}

void MyDigraphBfsVisitor::start (const Node &node)
{}

void MyDigraphBfsVisitor::reach (const Node &node)
{
}

void MyDigraphBfsVisitor::discover (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*diGraphNodeMap) [u];
	v = g->target (arc);
	v_str = (*diGraphNodeMap) [v];
	distance[v]= distance[u] + 1;
	int weight;
	wxString w = wxT ("");

	if (graphType == digraphw)
	{
		weight = (*edgeMap) [arc];
		w.Printf ("%d", weight);
	}

	if (coloringTheGraph)
		bfsTree->insertEdge (u_str, v_str, wxT (""));
	else
		bfsTree->insertEdge (u_str, v_str, wxT (""),wxTREE_EDGE_PEN);
}

void MyDigraphBfsVisitor::examine (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*diGraphNodeMap) [u];
	v = g->target (arc);
	v_str = (*diGraphNodeMap) [v];
	int weight;

	if (coloringTheGraph)
		bfsTree->insertEdge (u_str, v_str, wxT (""),wxNORMAL_PEN);
	else
		bfsTree->insertEdge (u_str, v_str, wxT (""), wxCROSS_EDGE_PEN);
}

void MyDigraphBfsVisitor::process (const Node &node)
{
	u_str = (*diGraphNodeMap) [node];
	visitList.push_back (u_str + wxT (" ") );
}

void MyDigraphBfsVisitor::drawTree()
{
	switch (graphLayout)
	{
	case hierarchical:
		bfsTree->hierarchicalLayout();
		break;

	case forcedirected:
		bfsTree->forceDirectedLayout();
		break;

	case orthogonal:
		bfsTree->orthogonalLayout();
		break;

	case circular:
		bfsTree->circularLayout();
		break;
	}

	if (coloringTheGraph)
	{
		theTree = new DisplayAlgorithmResults (myParent, bfsTree, graphType, graphLayout, treeType, wxT(""));
	}
	else
		theTree = new DisplayAlgorithmResults (myParent, bfsTree, graphType, graphLayout, treeType);

	if (callShowSequence)
	{
		vector<wxString>::const_iterator visitListIter;
		int itemNumber = 0, items = 1;
		wxString tempResult;

		visitListIter = visitList.begin();

		if (msg == wxT(""))
			while (visitListIter != visitList.end() )
			{
				tempResult.Printf (wxT ("%s "), *visitListIter);
				theTree->writeOutputLine (tempResult);

				if (items % 55 == 0)
					theTree->writeOutputLine (wxT ("\n") );

				items++;
				visitListIter++;
			}
		else
			theTree->writeOutputLine(msg);
	}

	theTree->Show();
	wxTheApp ->SetTopWindow (theTree);
}
