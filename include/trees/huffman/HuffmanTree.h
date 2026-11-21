#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <wx/string.h>
#include <wx/textctrl.h>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>
#include "trees/huffman/HuffmanNode.h"

#include <queue>
#include <functional>
#include <vector>
#include <map>

using namespace ogdf;
using namespace std;

class HuffmanTree
{
public:
	HuffmanTree (const wxString& title);
	virtual ~HuffmanTree();
	void buildHuffmanTree (const vector<HuffmanNode>& tnodes);
	void outputBitCodes (wxTextCtrl *bitCodes);
	void buildFromGML (const wxString& gfilename);
	void outputGMLData (wxTextCtrl *bitCodes);
	void init();

	ogdf::Graph G;
	GraphAttributes GA;
	node getRoot();

private:
	vector<node> leaf;
	int cost, uncompressed_cost;
	node root;
	node graphLabel;
	wxString treeFigureTitle;

	void layout();
	node insertNode (const wxString& label);
	node createLeafNode (const HuffmanNode& h);
	node createInteriorNode (const HuffmanNode& h);
	void computeCosts (node t);
};

#endif // HUFFMANTREE_H
