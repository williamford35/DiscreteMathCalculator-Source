#ifndef GRAPHTYPE_H_INCLUDED
#define GRAPHTYPE_H_INCLUDED

enum GraphType
{
	undir,
	undirw,
	digraph,
	digraphw,
	undirtree,
	notagraph
};

enum GraphLayout
{
	hierarchical,
	forcedirected,
	orthogonal,
	circular,
	tree
};

enum RandomGraphType
{
	general,
	simple,
	biconnected,
	triconnected,
	planar,
	acyclic,
	bipartite
};

enum RandomDigraphType
{
	randomDigraph,
	strongly_connected,
	dag,
	tournament
};

#endif // GRAPHTYPE_H_INCLUDED
