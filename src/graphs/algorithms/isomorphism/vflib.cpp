/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "vflib/argedit.h"
#include "vflib/vf2_sub_state.h"
#include "vflib/argraph.h"
#include "vflib/match.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <map>

#include "graphs/algorithms/isomorphism/IsomorphismOutput.h"
#include <wx/msgdlg.h>

void convertToVflib (ogdf::GraphAttributes *GA, ogdf::Graph *G, ARGEdit& ed, wxString *vertex);
extern void report (const wxString& msg, const wxString& caption);

#define MAXNODES 200

void testForSubgraphIsomorphism (ogdf::GraphAttributes *GA1, ogdf::Graph *G1,
                                 ogdf::GraphAttributes *GA2, ogdf::Graph *G2)
{
	int n1 = G1->numberOfNodes(), n2 = G2->numberOfNodes();
	wxString *v1 = new wxString[n1], *v2 = new wxString[n2];
	ARGEdit ed1, ed2;
	wxString matching;

	convertToVflib (GA1, G1, ed1, v1);
	Graph g1 (&ed1);
	convertToVflib (GA2, G2, ed2, v2);
	Graph g2 (&ed2);

	VF2SubState s0 (&g1, &g2);

	int numberMatchingVertices;
	node_id ni1[MAXNODES], ni2[MAXNODES];

	if (!match (&s0, &numberMatchingVertices, ni1, ni2) )
		report (wxT ("There is no subgraph isomorphism."), wxT ("Result") );
	else
	{
		int ni1_int[MAXNODES], ni2_int[MAXNODES];

		for (int i=0; i < numberMatchingVertices; i++)
		{
			ni1_int[i] = (int)ni1[i];
			ni2_int[i] = (int)ni2[i];
		}

		matching.Printf (wxT ("Found a matching with %d nodes:\n"), numberMatchingVertices);
		report (matching, wxT ("Result") );

		IsomorphismOutput vf2Output (NULL, ni1_int, v1, ni2_int, v2, numberMatchingVertices, "isomorphism.png");
		vf2Output.ShowModal();
	}

	delete[] v1;
	delete[] v2;
}

void convertToVflib (ogdf::GraphAttributes *GA, ogdf::Graph *G, ARGEdit& ed, wxString *vertex)
{
	ogdf::List<ogdf::edge> edges;
	ogdf::List<ogdf::edge>::iterator edgeIterator;
	ogdf::node u, v;
	int i, j, k;
	node_id id;
	wxString vertexLabel_src, vertexLabel_target;
	int n = G->numberOfNodes();

	for (i=0; i < n; i++)
		ed.InsertNode (NULL);

	G->allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		ogdf::edge e = *edgeIterator;
		ogdf::node source = (*e).source();
		ogdf::node target = (*e).target();

		vertexLabel_src = wxString (GA->label (source).c_str(), wxConvUTF8);
		vertexLabel_target = wxString (GA->label (target).c_str(), wxConvUTF8);
		i = source->index();
		j = target->index();
		ed.InsertEdge (i,j,NULL);
		ed.InsertEdge (j,i,NULL);
		vertex[i] = vertexLabel_src;
		vertex[j] = vertexLabel_target;
	}
}
