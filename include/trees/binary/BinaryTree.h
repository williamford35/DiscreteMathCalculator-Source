#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/fileformats/GraphIO.h>

#include <string>
#include <stack>

#include <wx/string.h>
#include <wx/vector.h>

#include "trees/search/stree.h"

using namespace ogdf;
using namespace std;

bool equal (const node lhs, const node rhs, const GraphAttributes& GA);
bool notequal (const node lhs, const node rhs, const GraphAttributes& GA);

class BinaryTree
{
public:
	BinaryTree (const wxString& title);
	virtual ~BinaryTree();

	void buildBinaryTree_init();
	void buildBTR_node (const wxString& line);
	void buildTreeBTR_end();
	void buildBinaryTreeGML
	(const wxString& gfileName, bool rbtree = false);
	void inorder (wxVector<string>& label);
	void reverseInorder (wxVector<string>& label);
	void postorder (wxVector<string>& label);
	void reversePostorder (wxVector<string>& label);
	void preorder (wxVector<string>& label);
	void reversePreorder (wxVector<string>& label);
	void levelorder (wxVector<string>& label);
	node getRoot();
	void clear();
	wxString getTreeLabel();

	ogdf::Graph G;
	ogdf::GraphAttributes GA;

private:
	node root;
	node nil;
	stack<node> s;
	node graphLabel;
	wxString treeFigureTitle;

	node createNode (const wxString& label);
	void insertNullSubtree (node currNode);
	void insertEdge (node parent, node child);
	void getnodeChildren (const wxString& line, wxString& currNode, int& leftChildTag, int& rightChildTag);
	void appendNodeLabel (node t, wxVector<string>& label);
	bool isValidNodeValue (const wxChar ch);
	void LNR (node t, wxVector<string>& label);
	void RNL (node t, wxVector<string>& label);
	void LRN (node t, wxVector<string>& label);
	void RLN (node t, wxVector<string>& label);
	void NRL (node t, wxVector<string>& label);
	void NLR (node t, wxVector<string>& label);
	void lOrder (node t, wxVector<string>& label);
};

#endif // BINARYTREE_H
