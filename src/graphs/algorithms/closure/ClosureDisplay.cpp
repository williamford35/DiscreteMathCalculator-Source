/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/closure/ClosureDisplay.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/build/GraphEdgeStyles.h"
#include "common/matrix.h"

ClosureDisplay::ClosureDisplay (ogdf::GraphAttributes *ga, ogdf::Graph *grph, ClosureType ct):
	GA(ga), G(grph), closureType(ct), digraphNodeMap(dig), arcMap(dig)
{
	buildRelationDigraphRepresentation();
}

void ClosureDisplay::buildRelationDigraphRepresentation()
{
	ogdf::List<node> vertices;
	ogdf::List<node>::iterator vertexIterator;
	ogdf::List<edge> edges;
	ogdf::List<edge>::iterator edgeIterator;
	wxString u, v;
	lemon::ListDigraph::Node node_u, node_v;
	lemon::ListDigraph::Arc e;
	wxString vertexLabel, edgeLabel;
	string nodeLabel;

	G->allNodes (vertices);

	for (vertexIterator = vertices.begin(); vertexIterator != vertices.end(); vertexIterator++)
	{
		if (GA->shape (*vertexIterator) == Shape::Ellipse)
			continue;

		node_u = dig.addNode();
		vertexLabel = wxString (GA->label (*vertexIterator).c_str(), wxConvUTF8);
		digraphNodeMap[node_u] = vertexLabel;
		wxString2DiNode[vertexLabel] = node_u;
	}

	G->allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		u = wxString (GA->label ( (**edgeIterator).source()).c_str(), wxConvUTF8);
		v = wxString (GA->label ( (**edgeIterator).target()).c_str(), wxConvUTF8);
		node_u = wxString2DiNode[u];
		node_v = wxString2DiNode[v];
		e = dig.addArc (node_u, node_v);
		arcMap[e] = (int) 1;
	}
}

void ClosureDisplay::drawRequestedClosure()
{
	switch (closureType)
	{
	case REFLEXIVE:
		DoReflexiveClosure();
		break;

	case SYMMETRIC:
		DoSymmetricClosure();
		break;

	case TRANSITIVE:
		DoTransitiveClosure();
		break;
	}
}

void ClosureDisplay::DoReflexiveClosure()
{
	typedef lemon::ListDigraph::Node Node;

	BuildGraph *reflexiveClosureConstruction;
	Node src, dest;
	wxString srcLabel, destLabel;
	DisplayAlgorithmResults *reflexiveClosure;
	ListDigraph::Arc a;
	bool hasSelfLoop;

	arcNew.clear();

	for (lemon::ListDigraph::NodeIt n (dig); n != lemon::INVALID; ++n)
	{
		hasSelfLoop = false;

		for (ListDigraph::OutArcIt a (dig, n); a!=INVALID; ++a)
		{
			dest = dig.target (a);

			if (dest == n)
			{
				hasSelfLoop = true;
				break;
			}

		}

		if (hasSelfLoop == false)
		{
			a = dig.addArc (n, n);
			arcNew[a] = true;
		}
	}

	reflexiveClosureConstruction = new BuildGraph (wxT (""), digraph, forcedirected, true);

	for (ListDigraph::ArcIt a (dig); a != INVALID; ++a)
	{
		src = dig.source (a);
		dest = dig.target (a);
		srcLabel = digraphNodeMap[src];
		destLabel = digraphNodeMap[dest];

		if (arcNew[a] == true)
			reflexiveClosureConstruction->insertEdge (srcLabel,
			      destLabel,
			      wxT (""), wxwxDASHED_GRAPH_EDGE_PEN,
			      wxColour(255,215,0), wxColour(255,215,0));
		else
			reflexiveClosureConstruction->insertEdge (srcLabel,
			      destLabel,
			      wxT (""), wxNORMAL_PEN,wxColour(255,215,0), wxColour(255,215,0));
	}

	reflexiveClosureConstruction->forceDirectedLayout();
	reflexiveClosure = new DisplayAlgorithmResults (NULL, reflexiveClosureConstruction, digraph, forcedirected,
	      DisplayAlgorithmResults::REFLEXIVECLOSURE);
// Construct the textual reflexive closure results.
wxString originalPairs;
wxString addedPairs;
wxString closurePairs;

for (ListDigraph::ArcIt arc(dig); arc != INVALID; ++arc)
{
    src = dig.source(arc);
    dest = dig.target(arc);

    wxString pair = wxT("(") +
                    digraphNodeMap[src] +
                    wxT(",") +
                    digraphNodeMap[dest] +
                    wxT(")");

    // Add the pair to the complete reflexive closure.
    if (!closurePairs.IsEmpty())
        closurePairs += wxT(", ");

    closurePairs += pair;

    // Determine whether this pair was newly added.
    if (arcNew[arc])
    {
        if (!addedPairs.IsEmpty())
            addedPairs += wxT(", ");

        addedPairs += pair;
    }
    else
    {
        if (!originalPairs.IsEmpty())
            originalPairs += wxT(", ");

        originalPairs += pair;
    }
}

// Format the results.
	wxString resultText =
	    wxString(wxT("REFLEXIVE CLOSURE\n\n")) +
	    wxT("Original relation:\n{") + originalPairs +
	    wxT("}\n\n") +
	    wxT("Added ordered pairs:\n{") + addedPairs +
	    wxT("}\n\n") +
	    wxT("Reflexive closure:\n{") + closurePairs +
	    wxT("}\n");

	// Write the results into the Algorithm output window.
	reflexiveClosure->writeOutputLine(resultText);
	reflexiveClosure->Show();
	wxTheApp->SetTopWindow(reflexiveClosure);

	delete reflexiveClosureConstruction;
}

void ClosureDisplay::DoSymmetricClosure()
{
	typedef lemon::ListDigraph::Node Node;

	int numVertices = G->numberOfNodes();
	matrix<int> adjMatrix (numVertices, numVertices, 0);
	int i, j;
	Node rowNode;
	BuildGraph *symmetricClosureConstruction;
	Node src, dest;
	ListDigraph::Arc arc;
	wxString srcLabel, destLabel;
	DisplayAlgorithmResults *symmetricClosure;

	arcNew.clear();

	// build the adjacency matrix
	for (i = 0; i < numVertices; i++)
	{
		rowNode = dig.nodeFromId (i);

		for (ListDigraph::OutArcIt a (dig, rowNode); a!=INVALID; ++a)
			adjMatrix[i][ dig.id((dig.target (a)))] = 1;
	}

	// assure symmetry
	for (i = 0; i < numVertices; i++)
		for (j = 0; j < numVertices; j++)
			if ( (j != i) && (adjMatrix[i][j] == 1))
				if (adjMatrix[j][i] == 0)
				{
					src = dig.nodeFromId(j);
					dest = dig.nodeFromId(i);
					arcNew[dig.addArc(src,dest)] = true;
					adjMatrix[j][i] = 1;
				}

	// build the symmetric closure graph
	symmetricClosureConstruction = new BuildGraph (wxT (""), digraph,
	      forcedirected, true);

	for (i = 0; i < numVertices; i++)
		for (j = 0; j < numVertices; j++)
			if (adjMatrix[i][j] == 1)
			{
				src = dig.nodeFromId(i);
				srcLabel = digraphNodeMap[src];
				dest = dig.nodeFromId(j);
				destLabel = digraphNodeMap[dest];
				arc = lemon::findArc (dig,src,dest);

				if (arcNew[arc])
					symmetricClosureConstruction->insertEdge(srcLabel, destLabel, wxT(""), wxwxDASHED_GRAPH_EDGE_PEN,
					      wxColour(255,215,0), wxColour(255,215,0));
				else
					symmetricClosureConstruction->insertEdge(srcLabel, destLabel, wxT(""), wxNORMAL_PEN,
					      wxColour(255,215,0), wxColour(255,215,0));
			}

	symmetricClosureConstruction->hierarchicalLayout();
	symmetricClosure = new DisplayAlgorithmResults (NULL, symmetricClosureConstruction, digraph, hierarchical,
	      DisplayAlgorithmResults::SYMMETRICCLOSURE);
// Construct the textual symmetric closure results.
wxString originalPairs;
wxString addedPairs;
wxString closurePairs;

for (int row = 0; row < numVertices; ++row)
{
    for (int col = 0; col < numVertices; ++col)
    {
        if (adjMatrix[row][col] == 0)
            continue;

        Node sourceNode = dig.nodeFromId(row);
        Node targetNode = dig.nodeFromId(col);

        wxString pair = wxT("(") +
                        digraphNodeMap[sourceNode] +
                        wxT(",") +
                        digraphNodeMap[targetNode] +
                        wxT(")");

        if (!closurePairs.IsEmpty())
            closurePairs += wxT(", ");
        closurePairs += pair;

        ListDigraph::Arc currentArc =
            lemon::findArc(dig, sourceNode, targetNode);

        if (arcNew[currentArc])
        {
            if (!addedPairs.IsEmpty())
                addedPairs += wxT(", ");
            addedPairs += pair;
        }
        else
        {
            if (!originalPairs.IsEmpty())
                originalPairs += wxT(", ");
            originalPairs += pair;
        }
    }
}

	// Format the results.
	wxString resultText =
	    wxString(wxT("SYMMETRIC CLOSURE\n\n")) +
	    wxT("Original relation:\n{") + originalPairs +
	    wxT("}\n\n") +
	    wxT("Added ordered pairs:\n{") + addedPairs +
	    wxT("}\n\n") +
	    wxT("Symmetric closure:\n{") + closurePairs +
	    wxT("}\n");

	// Write the results into the Algorithm output window.
	symmetricClosure->writeOutputLine(resultText);



	symmetricClosure->Show();
symmetricClosure->getDisplayAlgorithmResultsTextCtrl()->Show();
symmetricClosure->getDisplayAlgorithmResultsTextCtrl()->Refresh();
symmetricClosure->getDisplayAlgorithmResultsTextCtrl()->Update();
	wxTheApp ->SetTopWindow (symmetricClosure);

	delete symmetricClosureConstruction;
}

void ClosureDisplay::DoTransitiveClosure()
{
	typedef lemon::ListDigraph::Node Node;

	Bfs<ListDigraph> bfs_test(dig);
	BuildGraph *transitiveClosureConstruction;
	Node src, dest, rowNode;
	wxString srcLabel, destLabel;
	DisplayAlgorithmResults *transitiveClosure;
	int i, j;
	int numVertices = G->numberOfNodes();

	matrix<int> adjMatrix (numVertices, numVertices, 0);

	// build the adjacency matrix
	for (i = 0; i < numVertices; i++)
	{
		rowNode = dig.nodeFromId (i);

		for (ListDigraph::OutArcIt a (dig, rowNode); a!=INVALID; ++a)
			adjMatrix[i][ dig.id((dig.target (a)))] = 1;
	}

matrix<int> reachabilityMatrix(numVertices, numVertices, 0);

// Initialize with the original relation.
for (int i = 0; i < numVertices; ++i)
{
    for (int j = 0; j < numVertices; ++j)
        reachabilityMatrix[i][j] = adjMatrix[i][j];
}

	// Warshall's algorithm.
	for (int k = 0; k < numVertices; ++k)
	{
	    for (int i = 0; i < numVertices; ++i)
	    {
	        for (int j = 0; j < numVertices; ++j)
	        {
	            if (reachabilityMatrix[i][k] &&
	                reachabilityMatrix[k][j])
	            {
	                reachabilityMatrix[i][j] = 1;
	            }
	        }
	    }
	}

	// build the transitive closure graph
	transitiveClosureConstruction = new BuildGraph (wxT (""), digraph,
	      forcedirected, true);

	for (i = 0; i < numVertices; i++)
		for (j = 0; j < numVertices; j++)
			if (reachabilityMatrix[i][j] == 1)
			{
				src = dig.nodeFromId(i);
				srcLabel = digraphNodeMap[src];
				dest = dig.nodeFromId(j);
				destLabel = digraphNodeMap[dest];

				if (adjMatrix[i][j] == 0)
					transitiveClosureConstruction->insertEdge(srcLabel, destLabel, wxT(""), wxwxDASHED_GRAPH_EDGE_PEN,
					      wxColour(255,215,0), wxColour(255,215,0));
				else
					transitiveClosureConstruction->insertEdge(srcLabel, destLabel, wxT(""), wxNORMAL_PEN,
					      wxColour(255,215,0), wxColour(255,215,0));
			}

	transitiveClosureConstruction->forceDirectedLayout();
	transitiveClosure = new DisplayAlgorithmResults (NULL, transitiveClosureConstruction, digraph, hierarchical,
	      DisplayAlgorithmResults::TRANSITIVECLOSURE);
// Construct the textual transitive closure results.
	wxString originalPairs;
	wxString addedPairs;
	wxString closurePairs;

	for (int row = 0; row < numVertices; ++row)
	{
	    for (int col = 0; col < numVertices; ++col)
	    {
	        Node sourceNode = dig.nodeFromId(row);
	        Node targetNode = dig.nodeFromId(col);

	        wxString pair = wxT("(") +
	                        digraphNodeMap[sourceNode] +
	                        wxT(",") +
	                        digraphNodeMap[targetNode] +
	                        wxT(")");

	        // Original relation.
	        if (adjMatrix[row][col] != 0)
	        {
	            if (!originalPairs.IsEmpty())
	                originalPairs += wxT(", ");

	            originalPairs += pair;
	        }

	        // Closure relation, including original edges.
	        if (adjMatrix[row][col] != 0 ||
	            reachabilityMatrix[row][col] != 0)
	        {
	            if (!closurePairs.IsEmpty())
	                closurePairs += wxT(", ");

	            closurePairs += pair;
	        }

	        // Newly added pairs.
	        if (reachabilityMatrix[row][col] != 0 &&
	            adjMatrix[row][col] == 0)
	        {
	            if (!addedPairs.IsEmpty())
	                addedPairs += wxT(", ");

	            addedPairs += pair;
	        }
	    }
	}

	// Format the results.
	wxString resultText =
		    wxString(wxT("TRANSITIVE CLOSURE\n\n")) +
		    wxT("Original relation:\n{") + originalPairs +
		    wxT("}\n\n") +
		    wxT("Added ordered pairs:\n{") + addedPairs +
		    wxT("}\n\n") +
		    wxT("Transitive closure:\n{") + closurePairs +
		    wxT("}\n");

	// Write the results into the Algorithm output window.
	transitiveClosure->writeOutputLine(resultText);
	transitiveClosure->Show();
	wxTheApp ->SetTopWindow (transitiveClosure);

	delete transitiveClosureConstruction;
}
