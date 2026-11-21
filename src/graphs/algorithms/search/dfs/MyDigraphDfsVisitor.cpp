/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/search/dfs/MyDigraphDfsVisitor.h"
#include "graphs/algorithms/ShowSequence.h"

MyDigraphDfsVisitor::MyDigraphDfsVisitor (wxWindow* parent, wxTextCtrl *out,
      ListDigraph::NodeMap<wxString> *nodeMap, ListDigraph::ArcMap<int> *amap,
      ListDigraph *grph, GraphType type) :
	cout (out), diGraphNodeMap (nodeMap), arcMap(amap), g (grph),
	time (1), genOutput (false), graphType (type),
	leaveCount (0), numVertices (0), myParent(parent)
{
	if (graphType == digraphw)
		graphType = digraph;

	dfsTree = new BuildGraph (wxT (""), graphType, hierarchical, false);

	// add all of the vertices
	for (ListDigraph::NodeIt n (*g); n != INVALID; ++n)
	{
		dfsTree->insertEdge ( (*diGraphNodeMap) [n], wxT ("#"));
		numVertices++;
	}

	visitList.push_back (wxT ("DFS list:\n\n"));
}

MyDigraphDfsVisitor::~MyDigraphDfsVisitor()
{
}

void MyDigraphDfsVisitor::start (const Node &node)
{}

void MyDigraphDfsVisitor::stop (const Node &node)
{}

void MyDigraphDfsVisitor::reach (const Node &node)
{
	if (!genOutput)
	{
		d[node] = time;
		time++;
	}
}

void MyDigraphDfsVisitor::discover (const Arc &arc)
{
	if (genOutput)
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
		dfsTree->insertEdge (u_str, v_str, wxT(""),wxTREE_EDGE_PEN);
	}
}

void MyDigraphDfsVisitor::examine (const Arc &arc)
{
	if (genOutput)
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

		if ( ( (d[v] < d[u]) && (d[u] < f[u]) && (f[u] < f[v])) || (u == v))
		{
			dfsTree->insertEdge (u_str, v_str, w, wxBACK_EDGE_PEN);
			//output.Printf(wxT("Back edge: %s -> %s\n"), u_str.c_str(), v_str.c_str());
		}
		else if ( (f[u] < d[v]) || (f[v] < d[u]))
		{
			dfsTree->insertEdge (u_str, v_str, w, wxCROSS_EDGE_PEN);
			//output.Printf(wxT("Cross edge: %s -> %s\n"), u_str.c_str(), v_str.c_str());
		}
		else
		{
			dfsTree->insertEdge (u_str, v_str, w, wxFORWARD_EDGE_PEN);
		}
	}
}

void MyDigraphDfsVisitor::leave (const Node &node)
{
	if (!genOutput)
	{
		f[node] = time;
		time++;
	}

	leaveCount++;

	if (leaveCount<= numVertices)
	{
		u_str = (*diGraphNodeMap) [node];
		visitList.push_back (u_str + wxT (" "));
	}
}

void MyDigraphDfsVisitor::backtrack (const Arc &arc)
{}

void MyDigraphDfsVisitor::generateOutput (bool b)
{
	genOutput = b;
}

void MyDigraphDfsVisitor::drawTree()
{
	dfsTree->forceDirectedLayout();
	theTree = new DisplayAlgorithmResults (myParent, dfsTree, graphType, hierarchical, DisplayAlgorithmResults::DFSTREE);
	ShowSequence *dfsListing;
	vector<wxString>::const_iterator visitListIter;
	int itemNumber = 0, line = 1;
	wxString tempResult;

	// dfsListing = new ShowSequence (myParent,"dfs-digraph.png",ShowSequence::DFS);

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
