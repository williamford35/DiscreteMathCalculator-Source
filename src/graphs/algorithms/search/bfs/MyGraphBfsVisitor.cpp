/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/bfs/MyGraphBfsVisitor.h"
#include "graphs/algorithms/ShowSequence.h"

MyGraphBfsVisitor::MyGraphBfsVisitor (wxWindow* parent, ListGraph::NodeMap<wxString> *nodeMap,
                                      ListGraph::EdgeMap<int> *emap, ListGraph *grph,
                                      GraphType type, GraphLayout layout,
                                      DisplayAlgorithmResults::Algorithm myTreeType,
                                      bool displayList,
                                      ListGraph::NodeMap<wxColor> *colorMap,
                                      const wxString& m, const wxString& s_str) :
	graphNodeMap (nodeMap), edgeMap(emap), g (grph), coloringTheGraph (false),
	msg (m), graphType (type), graphLayout (layout), callShowSequence (displayList),
	startVertex (s_str), treeType (myTreeType), myParent(parent)
{
	if (graphType == undirw)
		graphType = undir;

	// add all of the vertices
	if (colorMap != NULL)
	{
		coloringTheGraph = true;
		bfsTree = new BuildGraph (wxT(""), graphType, graphLayout, false);

		for (ListGraph::NodeIt n (*g); n != INVALID; ++n)
			bfsTree->insertEdge ( (*graphNodeMap) [n], wxT ("#"), wxT (""), *wxBLACK_PEN, (*colorMap) [n]);
	}
	else
	{
		bfsTree = new BuildGraph (wxT (""), graphType, graphLayout, false);

		for (ListGraph::NodeIt n (*g); n != INVALID; ++n)
			bfsTree->insertEdge ( (*graphNodeMap) [n], wxT ("#"));
	}

	visitList.push_back (wxT ("BFS list:\n\n"));
}

MyGraphBfsVisitor::~MyGraphBfsVisitor()
{
}

void MyGraphBfsVisitor::start (const Node &node)
{}

void MyGraphBfsVisitor::reach (const Node &node)
{
}

void MyGraphBfsVisitor::discover (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*graphNodeMap) [u];
	v = g->target (arc);
	v_str = (*graphNodeMap) [v];
	distance[v]= distance[u] + 1;
	int weight;
	wxString w = wxT("");

	if (graphType == undirw)
	{
		weight = (*edgeMap)[arc];
		w.Printf("%d", weight);
	}

	if (coloringTheGraph)
		bfsTree->insertEdge (u_str, v_str, wxT(""),wxNORMAL_PEN);
	else
		bfsTree->insertEdge (u_str, v_str, wxT(""),wxTREE_EDGE_PEN);
}

void MyGraphBfsVisitor::examine (const Arc &arc)
{
	u = g->source (arc);
	u_str = (*graphNodeMap) [u];
	v = g->target (arc);
	v_str = (*graphNodeMap) [v];
	int weight;
	wxString w = wxT("");

	if (coloringTheGraph)
		bfsTree->insertEdge (u_str, v_str, wxT(""),wxNORMAL_PEN);
	else
		bfsTree->insertEdge (u_str, v_str,wxT(""),wxCROSS_EDGE_PEN);
}

void MyGraphBfsVisitor::process (const Node &node)
{
	u_str = (*graphNodeMap) [node];
	visitList.push_back (u_str + wxT (" "));
}

void MyGraphBfsVisitor::drawTree()
{
	ShowSequence *bfsListing = NULL;

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

	/*if (relationPartialOrderGraphInProcess() == true)
	   coloringTheGraph = true;*/
	/* if (coloringTheGraph)
	{
		theTree = new DisplayAlgorithmResults (myParent, bfsTree, graphType, graphLayout, DisplayAlgorithmResults::GRAPHCOLORING, msg);
	}
	else*/
	theTree = new DisplayAlgorithmResults (myParent, bfsTree, graphType, graphLayout, treeType);

	if (callShowSequence)
	{
		vector<wxString>::const_iterator visitListIter;
		int itemNumber = 0, items = 1;
		wxString tempResult;

		visitListIter = visitList.begin();

		while (visitListIter != visitList.end())
		{
			tempResult.Printf (wxT ("%s "), *visitListIter);
			theTree->writeOutputLine (tempResult);

			if (items % 55 == 0)
				theTree->writeOutputLine (wxT ("\n"));

			items++;
			visitListIter++;
		}
	}

	if (msg.Length() != 0)
	{
		if (msg.IsNumber())
			msg.Printf (wxT ("Colored the graph using %s colors."), msg);

		theTree->writeOutputLine(msg);
	}

	theTree->Show();
	wxTheApp ->SetTopWindow (theTree);
}
