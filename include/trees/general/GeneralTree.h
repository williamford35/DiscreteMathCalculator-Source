#ifndef GENERALTREE_H_INCLUDED
#define GENERALTREE_H_INCLUDED

#include <vector>

#include <wx/window.h>
#include <wx/string.h>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>

#include <stack>

using namespace ogdf;
using namespace std;

bool equal (const ogdf::node lhs, const ogdf::node rhs, const GraphAttributes& GA);
bool notequal (const ogdf::node lhs, const ogdf::node rhs, const GraphAttributes& GA);

struct NodeDescriptor
{
	string nodeLabel;
	long parentIndex;
	ogdf::node theNode;

	NodeDescriptor()
	{}

	NodeDescriptor (const wxString& label, long pindex, ogdf::node t) :
		nodeLabel ( (const char *) label.c_str()), parentIndex (pindex), theNode (t)
	{}
};

class GeneralTree
{
public:
	GeneralTree (wxWindow *canvas, const wxString& title);
	virtual ~GeneralTree();

	void buildTreeGTR (const wxString& gtrFileName);
	void buildGeneralTreeGML (const wxString& gfileName);
	void buildTreeGTR_init();
	void buildGTR_node (const wxString& line);
	void buildTreeGTR_end();
	ogdf::node getRoot();
	void breadthFirstSearch (vector<string>& label);
	void depthFirstSearch (vector<string>& label);
	void convertToBinaryTree();
	void clear();

	ogdf::Graph G;
	ogdf::GraphAttributes GA;

private:
	ogdf::node root;
	vector<NodeDescriptor *> treeCreationTable;
	int nodeIndex;
	wxWindow *drawingCanvas;
	double currMaxLabelWidth;
	wxString treeFigureTitle;
	node graphLabel;

	ogdf::node createNode (const wxString& label);
	void insertNullSubtree (ogdf::node currNode);
	void insertEdge (ogdf::node parent, ogdf::node child);
	void getnodeEntry (const wxString& line, wxString& currNode, long& parentIndex);
	bool isValidNodeValue (const wxChar ch);
	void appendNodeLabel (ogdf::node t, vector<string>& label);
	void dfs (ogdf::node t, vector<string>& label);
	void bfs (ogdf::node t, vector<string>& label);
	void maxLabelWidth (const wxString& s);
};

#endif // GENERALTREE_H_INCLUDED
