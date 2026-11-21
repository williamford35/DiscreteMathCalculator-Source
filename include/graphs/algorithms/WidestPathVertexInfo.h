#ifndef WIDESTPATHVERTEXINFO_H
#define WIDESTPATHVERTEXINFO_H

#include <lemon/concepts/graph.h>
#include <lemon/list_graph.h>

class WidestPathVertexInfo
{
public:
	typedef lemon::ListGraph::Node Node;

	WidestPathVertexInfo();
	WidestPathVertexInfo (Node cNode, Node p);
	virtual ~WidestPathVertexInfo();
	WidestPathVertexInfo& operator= (const WidestPathVertexInfo& other);

	lemon::ListGraph::Node currNode, parent;

};

#endif // WIDESTPATHVERTEXINFO_H
