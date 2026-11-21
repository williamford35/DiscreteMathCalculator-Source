/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/WidestPathVertexInfo.h"

WidestPathVertexInfo::WidestPathVertexInfo()
{
}

WidestPathVertexInfo::WidestPathVertexInfo (lemon::ListGraph::Node cnode, Node p) :
	currNode (cnode), parent (p)
{
}

WidestPathVertexInfo::~WidestPathVertexInfo()
{
}

WidestPathVertexInfo& WidestPathVertexInfo::operator= (const WidestPathVertexInfo& rhs)
{
	if (this == &rhs)
		return *this; // handle self assignment

	currNode = rhs.currNode;
	parent = rhs.parent;

	return *this;
}
