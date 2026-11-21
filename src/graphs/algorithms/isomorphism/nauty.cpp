/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/isomorphism/nauty.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <map>

#include "graphs/algorithms/isomorphism/IsomorphismOutput.h"
#include <wx/msgdlg.h>
#include "common/report.h"

void convertToNauty (ogdf::GraphAttributes *GA, ogdf::Graph *G, graph *g, int m, wxString* vg);

void testForIsomorphism (ogdf::GraphAttributes *GA1, ogdf::Graph *G1,
                         ogdf::GraphAttributes *GA2, ogdf::Graph *G2)
{
	DYNALLSTAT (int,lab1,lab1_sz);
	DYNALLSTAT (int,lab2,lab2_sz);
	DYNALLSTAT (int,ptn,ptn_sz);
	DYNALLSTAT (int,orbits,orbits_sz);
	DYNALLSTAT (int,map,map_sz);
	DYNALLSTAT (graph,g1,g1_sz);
	DYNALLSTAT (graph,g2,g2_sz);
	DYNALLSTAT (graph,cg1,cg1_sz);
	DYNALLSTAT (graph,cg2,cg2_sz);
	static DEFAULTOPTIONS_GRAPH (options);
	statsblk stats;

	long n = G1->numberOfNodes(), m, i;
	size_t k;
	wxString *vg1 = new wxString[n], *vg2 = new wxString[n];

	/* Select option for canonical labelling */

	options.getcanon = TRUE;

	m = SETWORDSNEEDED (n);
	nauty_check (WORDSIZE,m,n,NAUTYVERSIONID);

	DYNALLOC1 (int,lab1,lab1_sz,n,"malloc");
	DYNALLOC1 (int,lab2,lab2_sz,n,"malloc");
	DYNALLOC1 (int,ptn,ptn_sz,n,"malloc");
	DYNALLOC1 (int,orbits,orbits_sz,n,"malloc");
	DYNALLOC1 (int,map,map_sz,n,"malloc");

	/* Now convert the first graph */

	DYNALLOC2 (graph,g1,g1_sz,n,m,"malloc");
	EMPTYGRAPH (g1,m,n);

	convertToNauty (GA1, G1, g1, m, vg1);

	/* Now convert the second graph */

	DYNALLOC2 (graph,g2,g2_sz,n,m,"malloc");
	EMPTYGRAPH (g2,m,n);

	convertToNauty (GA2, G2, g2, m, vg2);

	/* Create canonical graphs */

	DYNALLOC2 (graph,cg1,cg1_sz,n,m,"malloc");
	DYNALLOC2 (graph,cg2,cg2_sz,n,m,"malloc");

	densenauty (g1,lab1,ptn,orbits,&options,&stats,m,n,cg1);
	densenauty (g2,lab2,ptn,orbits,&options,&stats,m,n,cg2);

	/* Compare canonically labelled graphs */

	for (k = 0; k < m* (size_t) n; ++k)
		if (cg1[k] != cg2[k]) break;

	if (k == m* (size_t) n)
	{
		report (wxT ("The graphs are isomorphic."), wxT ("Note"));

		if (n <= 50)
		{
			/*
				Write the isomorphism.  For each i, vertex lab1[i]
				of cg1 maps onto vertex lab2[i] of cg2.  We compute
				the map in order of labelling because it looks better.
			*/

			for (i = 0; i < n; ++i) map[lab1[i]] = lab2[i];

			IsomorphismOutput nautyOutput (NULL, lab1, vg1, lab2, vg2, n, "isomorphism.png");
			nautyOutput.ShowModal();
		}
	}
	else
		report (wxT ("The graphs are not isomorphic."), wxT ("Note"));

	delete[] vg1;
	delete[] vg2;
}

void convertToNauty (ogdf::GraphAttributes *GA, ogdf::Graph *G, graph *g, int m, wxString *vg)
{
	// ogdf::List<ogdf::node> vertices;
	// ogdf::List<ogdf::node>::iterator vertexIterator;
	ogdf::List<ogdf::edge> edges;
	ogdf::List<ogdf::edge>::iterator edgeIterator;
	ogdf::node u, v;
	int i, j, k;
	// std::map<int,int> OGDFtoNauty;
	wxString vertexLabel_src, vertexLabel_target;

	// G->allNodes (vertices);

	// j = 0;

	/*for (vertexIterator = vertices.begin(); vertexIterator != vertices.end(); vertexIterator++)
	{
		ogdf::node p = *vertexIterator;

		i = (*p).index();
		OGDFtoNauty[i] = j;
		j++;
	}*/

	G->allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		ogdf::edge e = *edgeIterator;
		ogdf::node source = (*e).source();
		ogdf::node target = (*e).target();
		char *s1, *s2;

		vertexLabel_src = wxString (GA->label (source).c_str(), wxConvUTF8);
		s1 = (char *)GA->label (source).c_str();
		vertexLabel_target = wxString (GA->label (target).c_str(), wxConvUTF8);
		s2 = (char *)GA->label (target).c_str();

		/*i = OGDFtoNauty[source->index()];
		j = OGDFtoNauty[target->index()];*/
		i = source->index();
		j = target->index();
		vg[i] = vertexLabel_src;
		vg[j] = vertexLabel_target;
		ADDONEEDGE (g,i,j,m);
	}
}
