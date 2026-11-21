/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/object.h>
#include <wx/rtti.h>

#include "trees/binary/BinaryTree.h"
#include "trees/btrFileFormatException.h"
#include <ogdf/basic/Queue.h>
#include <ogdf/tree/TreeLayout.h>
#include <wx/tokenzr.h>
#include <utility>

using namespace ogdf;
using namespace std;

bool equalNil (const node t, const GraphAttributes& GA)
{
	return GA.width (t) == 0.0;
}

BinaryTree::BinaryTree (const wxString& title) : treeFigureTitle (title)
{
}

BinaryTree::~BinaryTree()
{
}

void BinaryTree::buildBinaryTree_init()
{
	GA.init (G,
	         GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
	         GraphAttributes::nodeLabel | GraphAttributes::nodeStyle | GraphAttributes::nodeType |
	         GraphAttributes::edgeType | GraphAttributes::edgeStyle | GraphAttributes::edgeLabel |
	         GraphAttributes::edgeArrow | GraphAttributes::nodeTemplate | GraphAttributes::nodeId);
	// GA.addAttributes(GraphAttributes::nodeId);

	if (treeFigureTitle.length() != 0)
	{
		graphLabel = G.newNode();
		GA.shape (graphLabel) = ogdf::Shape::Rect;
		GA.width (graphLabel) = 0.0;
		GA.height (graphLabel) = 0.0;
		GA.strokeWidth (graphLabel) = 0.0;
		GA.label (graphLabel) = (const char *) (treeFigureTitle.c_str());
	}

	GA.directed() = false;

	/*nil = G.newNode();
	GA.label(nil) = string("");
	GA.strokeWidth(nil) = 0.0;
	GA.shape(nil) = ogdf::Shape::Ellipse;
	GA.width(nil) = 0.0;
	GA.height(nil) = 0.0;*/

	root = NULL;
}

void BinaryTree::buildBTR_node (const wxString& line)
{
	node currNode, leftChild = NULL, rightChild=NULL;
	wxString currNodeValue;
	int leftChildTag, rightChildTag;
	bool noLeftChild = true, noRightChild = true;

	noLeftChild = true;
	noRightChild = true;

	getnodeChildren (line, currNodeValue, leftChildTag, rightChildTag);

	// parent
	currNode = createNode (currNodeValue); // created with string "446@B"

	// right child
	if (rightChildTag == 1)
	{
		rightChild = s.top();
		s.pop();
		noRightChild = false;
	}

	// left child
	if (leftChildTag == 1)
	{
		leftChild = s.top();
		s.pop();
		noLeftChild = false;
	}

	if (noLeftChild)
		insertNullSubtree (currNode);
	else
		insertEdge (currNode, leftChild);

	if (noRightChild)
		insertNullSubtree (currNode);
	else
		insertEdge (currNode, rightChild);

	s.push (currNode);
}

void BinaryTree::buildTreeBTR_end()
{
	TreeLayout btreeLayout;

	if (!s.empty())
	{
		root = s.top();
		s.pop();
	}

	if (treeFigureTitle.length() != 0 && root != nullptr)
	{
		edge e = G.newEdge (graphLabel, root);
		GA.arrowType (e) = ogdf::EdgeArrow::None;
		GA.type (e) = Graph::EdgeType::association;
		GA.strokeType(e) = ogdf::StrokeType::None;
		GA.strokeWidth (e) = 0.0;
	}

	//btreeLayout.siblingDistance(75.0);
	btreeLayout.siblingDistance (35.0);
	//btreeLayout.levelDistance(50.0);
	btreeLayout.levelDistance (40.0);
	//btreeLayout.subtreeDistance(75.0);
	btreeLayout.subtreeDistance (35.0);
	btreeLayout.rootSelection (TreeLayout::RootSelectionType::Source);
#ifdef __APPLE__
	btreeLayout.orientation (Orientation::topToBottom);
#elif _WIN64
	btreeLayout.orientation (Orientation::topToBottom);
#else
	btreeLayout.orientation (Orientation::topToBottom);
#endif

	btreeLayout.call (GA);
}

void BinaryTree::buildBinaryTreeGML
(const wxString& gfileName, bool rbtree)
{
	string sgfilename ( (const char *) gfileName.c_str());
	//TreeLayout btreeLayout;
	std::ifstream gmlIn(sgfilename.c_str());
	bool foundLabel = false;

	if (!GraphIO::readGML (GA, G, gmlIn))
		throw btrFileFormatException ("Failure reading GML file");

	/*btreeLayout.siblingDistance(75.0);
	btreeLayout.levelDistance(50.0);
	btreeLayout.subtreeDistance(75.0);
	btreeLayout.rootSelection(TreeLayout::rootIsSource);
	btreeLayout.orientation(topToBottom);
	btreeLayout.call(GA);*/

	if (G.numberOfNodes() == 0)
		return;

	List<node> allTreeNodes;
	ListIterator<node> iter;

	G.allNodes (allTreeNodes);

	// Is there a tree label? Check for red-black tree also.
	iter = allTreeNodes.begin();

	while (iter != allTreeNodes.end())
	{
		if (rbtree)
		{
			if (GA.fillColor(*iter) == ogdf::Color::Name::Red)
				GA.label(*iter) += "@R";
			else if (GA.fillColor(*iter) == ogdf::Color::Name::Lightgray)
				GA.label(*iter) += "@B";
		}

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

node BinaryTree::createNode (const wxString& label)
{
	node t = G.newNode();

	GA.shape (t) = ogdf::Shape::Ellipse;
	GA.label (t) = (const char *) (label.c_str());
	GA.strokeWidth (t) = 1.0;
	GA.width (t) = 25.0;
	GA.height (t) = 25.0;
	GA.fillColor(t) = ogdf::Color(255, 215, 0);

	return t;
}

void BinaryTree::insertNullSubtree (node currNode)
{
	node t = G.newNode();
	edge e;

	GA.shape (t) = ogdf::Shape::Ellipse;
	GA.label (t) = "";
	GA.strokeWidth (t) = 0.0;
	GA.width (t) = 0.0;
	GA.height (t) = 0.0;
	e = G.newEdge (currNode,t);
	GA.strokeWidth(e) = 0.0;
	GA.type (e) = Graph::EdgeType::association;
	GA.arrowType(e) = ogdf::EdgeArrow::None;
}

void BinaryTree::insertEdge (node parent, node child)
{
	edge e = G.newEdge (parent, child);

	GA.arrowType (e) = ogdf::EdgeArrow::None;
	GA.type (e) = Graph::EdgeType::association;
	GA.strokeWidth (e) = 1.0;
}

bool BinaryTree::isValidNodeValue (const wxChar ch)
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

void BinaryTree::getnodeChildren (const wxString& line, wxString& currNode, int& leftChildTag, int& rightChildTag)
{
	wxStringTokenizer in (line.c_str());
	wxString s;
	int count = 0;

	while (in.HasMoreTokens())
	{
		s = in.GetNextToken();
		count++;

		if (count == 1)
		{
			if (!isValidNodeValue (s[0]))
				throw btrFileFormatException ("Your tree description file format is incorrect.");

			currNode = s;
		}
		else
		{
			if (count <= 3)
			{
				if (s.length() > 1 || (s[0] != '0' && s[0] != '1'))
					throw btrFileFormatException ("Your tree description file format is incorrect.");

				if (count == 2)
					leftChildTag = (int) (s[0] - '0');
				else if (count == 3)
					rightChildTag = (int) (s[0] - '0');
			}
		}
	}

	if (count != 3)
		throw btrFileFormatException ("Your tree description file format is incorrect.");
}

void BinaryTree::appendNodeLabel (node t, wxVector<string>& label)
{
	label.push_back (GA.label(t));
}

void BinaryTree::inorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	LNR (t, label);
}
void BinaryTree::LNR (node t, wxVector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node child;

	t->outEdges (children);	// get the 2 edges from t
	iter = children.begin();
	child = (**iter).target();

	if (!equalNil (child, GA))	// left child
		LNR (child, label);

	// output the current node
	appendNodeLabel (t, label);
	iter++;
	child = (**iter).target();

	if (!equalNil (child, GA))	// right child
		LNR (child, label);
}

void BinaryTree::reverseInorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	RNL (t, label);
}

void BinaryTree::RNL (node t, wxVector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node lchild, rchild;

	t->outEdges (children);	// get the 2 edges from t
	iter = children.begin();
	lchild = (**iter).target();
	iter++;
	rchild = (**iter).target();

	if (!equalNil (rchild, GA))	// right child
		RNL (rchild, label);

	// output the current node
	appendNodeLabel (t, label);

	if (!equalNil (lchild, GA))	// left child
		RNL (lchild, label);
}

void BinaryTree::postorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	LRN (t, label);
}

void BinaryTree::LRN (node t, wxVector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node child;

	t->outEdges (children);	// get the 1-2 edges from t
	iter = children.begin();
	child = (**iter).target();

	if (!equalNil (child, GA))	// left child
		LRN (child, label);

	iter++;
	child = (**iter).target();

	if (!equalNil (child, GA))	// right child
		LRN (child, label);

	appendNodeLabel (t, label);
}

void BinaryTree::reversePostorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	RLN (t, label);
}

void BinaryTree::RLN (node t, wxVector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node lchild, rchild;

	t->outEdges (children);	// get the 2 edges from t
	iter = children.begin();
	lchild = (**iter).target();
	iter++;
	rchild = (**iter).target();

	if (!equalNil (rchild, GA))	// right child
		RLN (rchild, label);

	if (!equalNil (lchild, GA))	// left child
		RLN (lchild, label);

	// output the current node
	wxString s (wxString (GA.label (t).c_str(),wxConvUTF8));
	appendNodeLabel (t, label);
}

void BinaryTree::reversePreorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	NRL (t, label);
}

void BinaryTree::NRL (node t, wxVector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node lchild, rchild;

	// output the current node
	appendNodeLabel (t, label);
	t->outEdges (children);	// get the 2 edges from t
	iter = children.begin();
	lchild = (**iter).target();
	iter++;
	rchild = (**iter).target();

	if (!equalNil (rchild, GA))	// right child
		NRL (rchild, label);

	if (!equalNil (lchild, GA))	// left child
		NRL (lchild, label);
}

void BinaryTree::preorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	NLR (t, label);
}

void BinaryTree::NLR (node t, wxVector<string>& label)
{
	List<edge> children;
	ListIterator<edge> iter;
	node child;

	// output the current node
	appendNodeLabel (t, label);
	t->outEdges (children);	// get the 1-2 edges from t
	iter = children.begin();
	child = (**iter).target();

	if (!equalNil (child, GA))	// left child
		NLR (child, label);

	iter++;
	child = (**iter).target();

	if (!equalNil (child, GA))	// right child
		NLR (child, label);
}

void BinaryTree::levelorder (wxVector<string>& label)
{
	node t = root;

	if (root == NULL)
		return;

	label.clear();

	if (equalNil (t, GA))
		return;

	lOrder (t, label);
}

void BinaryTree::lOrder (node t, wxVector<string>& label)
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
	while (!q.empty())
	{
		// delete front node from queue and output the node value
		parent = q.pop();
		appendNodeLabel (parent, label);

		parent->outEdges (children);		// get the 1-2 edges from t
		iter = children.begin();
		child = (**iter).target();

		if (!equalNil (child, GA))		// left child
			q.append (child);

		iter++;
		child = (**iter).target();

		if (!equalNil (child, GA))		// left child
			q.append (child);
	}
}

node BinaryTree::getRoot()
{
	return root;
}

void BinaryTree::clear()
{
	G.clear();
}

wxString BinaryTree::getTreeLabel()
{
	return treeFigureTitle;
}
