/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/general/GeneralTree.h"
#include "trees/btrFileFormatException.h"
#include <wx/tokenzr.h>
#include <wx/textfile.h>

#include <ogdf/basic/Queue.h>
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/fileformats/GraphIO.h>

#include <iostream>
using namespace std;
using namespace ogdf;

GeneralTree::GeneralTree (wxWindow *canvas, const wxString& title) :
	drawingCanvas (canvas), currMaxLabelWidth (0), treeFigureTitle (title)
{}

GeneralTree::~GeneralTree()
{}

void GeneralTree::buildTreeGTR_init()
{
	GA.init (G,
	         GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
	         GraphAttributes::nodeLabel | GraphAttributes::nodeStyle | GraphAttributes::nodeType |
	         GraphAttributes::edgeType | GraphAttributes::edgeStyle | GraphAttributes::edgeLabel |
	         GraphAttributes::edgeArrow | GraphAttributes::nodeTemplate  );
	GA.addAttributes(GraphAttributes::nodeId);
	nodeIndex = -1;
	currMaxLabelWidth = 0;

	cout << "treeFigureTitle = " << treeFigureTitle << endl;
	if (treeFigureTitle.length() != 0)
	{
		graphLabel = G.newNode();
		GA.shape (graphLabel) = ogdf::Shape::Rect;
		GA.width (graphLabel) = 0.0;
		GA.height (graphLabel) = 0.0;
		GA.strokeWidth (graphLabel) = 1.0;
		GA.label (graphLabel) = string ( (const char *) treeFigureTitle.c_str() );
	}
	GA.directed() = false;
}

/*
void  setAllWidth (double w)
  Sets the width of all nodes to w.

void  setAllHeight (double h)
  Sets the height of all nodes to h.

*/

void GeneralTree::maxLabelWidth (const wxString& s)
{
	int width, height;

	drawingCanvas->GetTextExtent (s, &width, &height);

	if (width > currMaxLabelWidth)
		currMaxLabelWidth = width;
}

void GeneralTree::buildGTR_node (const wxString& line)
{
	node currNode;
	wxString currNodeLabel;
	long parentIndex;
	NodeDescriptor *treeTableEntry;

	getnodeEntry (line, currNodeLabel, parentIndex);
	currNode = createNode (currNodeLabel);
	treeTableEntry = new NodeDescriptor (currNodeLabel, parentIndex, currNode);
	maxLabelWidth (currNodeLabel);
	treeCreationTable.push_back (treeTableEntry);
}

void GeneralTree::buildTreeGTR_end()
{
	NodeDescriptor t;
	double boundingSquareHeight;

	if (!treeCreationTable.empty() )
		root = treeCreationTable[0]->theNode;

	GA.label (root) = treeCreationTable[0]->nodeLabel;

	if (treeFigureTitle.length() != 0)
	{
		edge e = G.newEdge (graphLabel, root);
		GA.arrowType (e) = ogdf::EdgeArrow::None;
		// GA.type (e) = Graph::EdgeType::association;
		GA.strokeWidth (e) = 0.0;
	}

	for (unsigned int i=1; i < treeCreationTable.size(); i++)
	{
		t = * (treeCreationTable[i]);
		insertEdge (treeCreationTable[t.parentIndex]->theNode, t.theNode);
		GA.label (t.theNode) = t.nodeLabel;
	}

	for (unsigned int i=0; i < treeCreationTable.size(); i++)
		delete treeCreationTable[i];

	treeCreationTable.clear();

	boundingSquareHeight = currMaxLabelWidth > 25 ? currMaxLabelWidth : 25;
	GA.setAllWidth (boundingSquareHeight);
	GA.setAllHeight (boundingSquareHeight);

	TreeLayout gtreeLayout;

	gtreeLayout.siblingDistance (35.0);
	gtreeLayout.levelDistance (40.0);
	gtreeLayout.subtreeDistance (50.0);
	gtreeLayout.rootSelection (TreeLayout::RootSelectionType::Source);
	gtreeLayout.orientation (Orientation::topToBottom);


	gtreeLayout.call (GA);
	nodeIndex = -1;
}

void GeneralTree::buildTreeGTR (const wxString& gtrFileName)
{
	wxTextFile gtrInput (gtrFileName);
	wxString line;

	nodeIndex = -1;
	currMaxLabelWidth = 0;

	if (!gtrInput.Open() )
	{
		string errMsg ("Cannot open ");

		errMsg += string ( (const char *) gtrFileName.c_str() );
		throw btrFileFormatException (errMsg);
	}

	for (line = gtrInput.GetFirstLine(); !gtrInput.Eof(); line = gtrInput.GetNextLine() )
		buildGTR_node (line);

	buildTreeGTR_end();
}

void GeneralTree::buildGeneralTreeGML(const wxString& gfileName)
{
	string sgfilename ( (const char *) gfileName.c_str());
	//TreeLayout btreeLayout;
	std::ifstream gmlIn(sgfilename.c_str());
	bool foundLabel = false;

	if (!GraphIO::readGML (GA, G, gmlIn))
		throw btrFileFormatException ("Failure reading GML file");

	if (G.numberOfNodes() == 0)
		return;

	List<node> allTreeNodes;
	ListIterator<node> iter;

	G.allNodes (allTreeNodes);

	// Is there a tree label? Check for red-black tree also.
	iter = allTreeNodes.begin();

	while (iter != allTreeNodes.end())
	{
		if (GA.shape (*iter) == ogdf::Shape::Rect)
		{
			List<edge> outEdge;
			edge edgeToRoot;

			treeFigureTitle = wxString (GA.label (*iter).c_str(),wxConvUTF8);
			(*iter)->outEdges (outEdge);
			edgeToRoot = * (outEdge.begin());
			root = (*edgeToRoot).target();

			foundLabel = true;
		}

		iter++;
	}

	// There is no tree label. Find root.
	if (!foundLabel)
	{
		iter = allTreeNodes.begin();

		while (iter != allTreeNodes.end())
		{
			if ( (**iter).indeg() == 0)
			{
				root = *iter;
				break;
			}

			iter++;
		}
	}
}

node GeneralTree::createNode (const wxString& label)
{
	node t = G.newNode();

	GA.shape (t) = Shape::Ellipse;
	GA.label (t) = string ( (const char *) label.c_str() );
	GA.strokeWidth (t) = 1.0;
	GA.width (t) = 25.0;
	GA.height (t) = 25.0;
	GA.fillColor (t) = ogdf::Color (255, 215, 0);

	return t;
}

void GeneralTree::insertEdge (node parent, node child)
{
	edge e = G.newEdge (parent, child);

	GA.arrowType (e) = ogdf::EdgeArrow::None;
	// GA.type (e) = Graph::EdgeType::association;
	GA.strokeWidth (e) = 1.0;
}

bool GeneralTree::isValidNodeValue (const wxChar ch)
{
	return
	   (ch >= 'A' && ch <= 'Z')  ||
	   (ch >= 'a' && ch <= 'z')  ||
	   (ch >= '0' && ch <= '9')  ||
	   ch == '+'					  ||
	   ch == '-'					  ||
	   ch == '*'					  ||
	   ch == '/'					  ||
	   ch == '^'					  ||
	   ch == '%';
}

void GeneralTree::getnodeEntry (const wxString& line, wxString& currNode, long& parentIndex)
{
	wxStringTokenizer in (line.c_str() );
	wxString s;
	int count = 0;
	long currentNodeIndex;

	while (in.HasMoreTokens() )
	{
		s = in.GetNextToken();
		count++;

		if (count == 1)
		{
			if (!s.ToLong (&currentNodeIndex) )
				throw btrFileFormatException ("A node index entry is not a valid integer.");
			else if (currentNodeIndex != nodeIndex + 1)
				throw btrFileFormatException ("Your node index entries are not in sequence.");

			nodeIndex++;
		}
		else if (count == 2)
		{
			if (!isValidNodeValue (s[0]) )
				throw btrFileFormatException ("Your tree description file format is incorrect.");

			currNode = s;
		}
		else if (count == 3)
		{
			if (nodeIndex == 0)
			{
				if (s[0] != '#')
					throw btrFileFormatException ("Your tree description file format is incorrect.");
			}
			else if (!s.ToLong (&parentIndex) )
				throw btrFileFormatException ("Your tree description file format is incorrect.");
		}
	}

	if (count != 3)
		throw btrFileFormatException ("Your tree description file format is incorrect.");
}

void GeneralTree::appendNodeLabel (node t, vector<string>& label)
{
	label.push_back (GA.label (t) );
}

void GeneralTree::depthFirstSearch (vector<string>& label)
{
	node t = root;

	label.clear();

	if (G.numberOfNodes() == 0)
		return;

	dfs (t, label);
}

void GeneralTree::dfs (node t, vector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node child;

	// output the current node
	appendNodeLabel (t, label);
	t->outEdges (children);	// get the edges from t
	iter = children.begin();

	while (iter != children.end() )
	{
		child = (**iter).target();
		dfs (child, label);
		iter++;
	}
}

void GeneralTree::breadthFirstSearch (vector<string>& label)
{
	node t = root;

	label.clear();

	if (G.numberOfNodes() == 0)
		return;

	bfs (t, label);
}

void GeneralTree::bfs (node t, vector<string>& label)
{
	// store children of each node in a queue so that they are
	// visited in order at the next level of the tree
	Queue<node> q;
	node parent = t, child;
	List<edge> children;
	ListIterator<edge> iter;

	// initialize the queue by inserting the root in the queue
	q.append (parent);

	// continue the iterative process until the queue is empty
	while (!q.empty() )
	{
		// delete front node from queue and output the node value
		parent = q.pop();

		appendNodeLabel (parent, label);

		parent->outEdges (children);		// get the edges from t
		iter = children.begin();

		while (iter != children.end() )
		{
			child = (**iter).target();
			q.append (child);
			iter++;
		}
	}
}

void GeneralTree::clear()
{
	G.clear();
	nodeIndex = -1;
}

node GeneralTree::getRoot()
{
	return root;
}
