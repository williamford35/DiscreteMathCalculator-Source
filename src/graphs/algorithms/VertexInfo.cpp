/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/VertexInfo.h"

VertexInfo::VertexInfo()
{
}

VertexInfo::VertexInfo (const wxString& n, const wxString& p, long v) :
	node (n), parent (p), dataValue (v)
{
}

VertexInfo::~VertexInfo()
{
}

VertexInfo::VertexInfo (const VertexInfo& other)
{
	node = other.node;
	parent = other.parent;
	dataValue = other.dataValue;
}

VertexInfo& VertexInfo::operator= (const VertexInfo& rhs)
{
	if (this == &rhs) return *this; // handle self assignment

	node = rhs.node;
	parent = rhs.parent;
	dataValue = rhs.dataValue;

	return *this;
}

bool operator> (const VertexInfo& lhs, const VertexInfo& rhs)
{
	return lhs.dataValue > rhs.dataValue;
}
