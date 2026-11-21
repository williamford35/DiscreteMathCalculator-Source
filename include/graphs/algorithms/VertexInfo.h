#ifndef VERTEXINFO_H
#define VERTEXINFO_H

#include <lemon/concepts/digraph.h>
#include <wx/string.h>

class VertexInfo
{
public:
	enum NodeColor
	{
		WHITE,
		BLACK
	};

	VertexInfo();
	VertexInfo (const wxString& n, const wxString& p, long v = 0);
	virtual ~VertexInfo();
	VertexInfo (const VertexInfo& other);
	VertexInfo& operator= (const VertexInfo& other);
	friend bool operator> (const VertexInfo& lhs, const VertexInfo& rhs);

	wxString node, parent;
	long dataValue;

};

#endif // VERTEXINFO_H
