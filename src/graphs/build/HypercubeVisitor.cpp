/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/build/HypercubeVisitor.h"

// HypercubeVisitor::HypercubeVisitor(HypercubeGraph *grph): g(grph), graphNodeMap(*g)
HypercubeVisitor::HypercubeVisitor (HypercubeGraph *grph, BuildGraph *myGraph) : g (grph), hypercubeGraph(myGraph)
{
	int vertexIndex;
	wxString vertexName;

	// add all of the vertices
	for (HypercubeGraph::NodeIt n (*g); n != INVALID; ++n)
	{
		vertexIndex = g->index (n);
		vertexName = toBinary (g->dimension(), vertexIndex);
		hypercubeGraph->insertEdge (vertexName,wxT("#"),wxT(""),wxNORMAL_PEN,
		                            wxColour(255,215,0), wxColour(255,215,0));
		graphNodeMap[n] = vertexName;
	}
}

HypercubeVisitor::~HypercubeVisitor()
{
}

void HypercubeVisitor::start (const Node &node)
{}

void HypercubeVisitor::reach (const Node &node)
{
}

void HypercubeVisitor::discover (const Arc &arc)
{
	lemon::HypercubeGraph::Edge edge = (lemon::HypercubeGraph::Edge) arc;

	if (id.find (edge) == id.end())
	{
		id.insert (edge);
		u = g->u (edge);
		u_str = graphNodeMap[u];
		v = g->v (edge);
		v_str = graphNodeMap[v];
		hypercubeGraph->insertEdge (u_str, v_str, wxT (""), wxNORMAL_PEN,
		                            wxColour(255,215,0), wxColour(255,215,0));
	}
}

void HypercubeVisitor::examine (const Arc &arc)
{
	lemon::HypercubeGraph::Edge edge = (lemon::HypercubeGraph::Edge) arc;

	if (id.find (edge) == id.end())
	{
		id.insert (edge);
		u = g->u (edge);
		u_str = graphNodeMap[u];
		v = g->v (edge);
		v_str = graphNodeMap[v];
		hypercubeGraph->insertEdge (u_str, v_str, wxT (""), wxNORMAL_PEN,
		                            wxColour(255,215,0), wxColour(255,215,0));
	}
}

void HypercubeVisitor::process (const Node &node)
{
}

wxString HypercubeVisitor::toBinary (int dim, int n)
{
	wxString binaryRep;
	short bit;

	for (int i=1; i <= dim; i++)
	{
		bit = n & 1;
		n = n >> 1;
		binaryRep = (bit==0 ? wxT ("0") : wxT ("1")) + binaryRep;
	}

	return binaryRep;
}
