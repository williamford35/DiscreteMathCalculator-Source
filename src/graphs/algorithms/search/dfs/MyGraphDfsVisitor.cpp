/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/dfs/MyGraphDfsVisitor.h"
#include "graphs/algorithms/ShowSequence.h"

MyGraphDfsVisitor::MyGraphDfsVisitor (wxWindow* parent, wxTextCtrl *out,
                                      ListGraph::NodeMap<wxString> *nodeMap,
                                      ListGraph::EdgeMap<int> *emap,
                                      ListGraph *grph, GraphType type) :
	cout (out), graphNodeMap (nodeMap), edgeMap(emap), g (grph),
	time (1), genOutput (false), graphType (type), numVertices (0),
	leaveCount (0), myParent(parent)
{
	if (graphType == undirw)
		graphType = undir;

	dfsTree = new BuildGraph (wxT (""), graphType, hierarchical, false);

	// add all of the vertices
	for (ListGraph::NodeIt n (*g); n != INVALID; ++n)
	{
		dfsTree->insertEdge ( (*graphNodeMap) [n], wxT ("#"));
		numVertices++;
	}

	visitList.push_back (wxT ("DFS list:\n\n"));
}

MyGraphDfsVisitor::~MyGraphDfsVisitor()
{
}

void MyGraphDfsVisitor::start (const Node &node)
{}

void MyGraphDfsVisitor::stop (const Node &node)
{}

void MyGraphDfsVisitor::reach (const Node &node)
{
	if (!genOutput)
	{
		d[node] = time;
		time++;
	}
}

void MyGraphDfsVisitor::discover (const Arc &arc)
{
	lemon::ListGraph::Edge edge = (lemon::ListGraph::Edge) arc;

	if (genOutput)
	{
		int weight;
		wxString w = wxT("");

		if (graphType == undirw)
		{
			weight = (*edgeMap)[arc];
			w.Printf("%d", weight);
		}

		u = g->u (edge);
		u_str = (*graphNodeMap) [u];
		v = g->v (edge);
		v_str = (*graphNodeMap) [v];
		dfsTree->insertEdge (u_str, v_str, w, wxTREE_EDGE_PEN);
	}
}

void MyGraphDfsVisitor::examine (const Arc &arc)
{
	lemon::ListGraph::Edge edge = (lemon::ListGraph::Edge) arc;

	if (genOutput)
	{
		int weight;
		wxString w = wxT("");

		if (graphType == undirw)
		{
			weight = (*edgeMap)[arc];
			w.Printf("%d", weight);
		}

		u = g->u (edge);
		u_str = (*graphNodeMap) [u];
		v = g->v (edge);
		v_str = (*graphNodeMap) [v];

		if ( ( (d[v] < d[u]) && (d[u] < f[u]) && (f[u] < f[v])) || u == v)
			dfsTree->insertEdge (u_str, v_str, w, wxBACK_EDGE_PEN);
	}
}

void MyGraphDfsVisitor::leave (const Node &node)
{
	if (!genOutput)
	{
		f[node] = time;
		time++;
	}

	leaveCount++;

	if (leaveCount<= numVertices)
	{
		u_str = (*graphNodeMap) [node];
		visitList.push_back (u_str + wxT (" "));
	}
}

void MyGraphDfsVisitor::backtrack (const Arc &arc)
{}

void MyGraphDfsVisitor::generateOutput (bool b)
{
	genOutput = b;
}

void MyGraphDfsVisitor::drawTree()
{
	dfsTree->forceDirectedLayout();
	theTree = new DisplayAlgorithmResults (myParent, dfsTree, graphType, hierarchical, DisplayAlgorithmResults::DFSTREE);

	vector<wxString>::const_iterator visitListIter;
	int itemNumber = 0, line = 1;
	wxString tempResult;

	// dfsListing = new ShowSequence (myParent,"dfs-graph.png",ShowSequence::DFS);

	visitListIter = visitList.begin();

	while (visitListIter != visitList.end())
	{
		if (itemNumber == 0)
			tempResult.Printf (wxT ("%s"), *visitListIter);
		else if (itemNumber != visitList.size()-1)
			tempResult.Printf (wxT ("%s -> "), *visitListIter);
		else
			tempResult.Printf (wxT ("%s"), *visitListIter);

		theTree->writeOutputLine (tempResult);

		if (line % 55 == 0)
			theTree->writeOutputLine (wxT ("\n"));

		itemNumber++;
		line++;
		visitListIter++;
	}

	theTree->Show();
	wxTheApp ->SetTopWindow (theTree);
}
