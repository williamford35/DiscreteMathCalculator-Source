#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

#include <wx/string.h>

#include <ogdf/basic/Graph.h>

using namespace ogdf;


class HuffmanNode
{
public:
	wxChar data;
	int frequency;
	node myNodeInTree;

	HuffmanNode()
	{}

	HuffmanNode (const wxChar dat, int freq, const node& t = NULL) :
		data (dat), frequency (freq), myNodeInTree (t)
	{}
};

#endif
