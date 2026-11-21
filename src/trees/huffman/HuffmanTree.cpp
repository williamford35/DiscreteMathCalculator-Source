/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/huffman/HuffmanTree.h"
#include "trees/huffman/HuffmanNode.h"
#include "trees/btrFileFormatException.h"
#include <ogdf/fileformats/GraphIO.h>

bool operator < (const HuffmanNode& lhs, const HuffmanNode& rhs)
{
	return lhs.frequency > rhs.frequency;
}

HuffmanTree::HuffmanTree (const wxString& title) : treeFigureTitle (title)
{
	init();
}

HuffmanTree::~HuffmanTree()
{}

void HuffmanTree::init()
{
	GA.init (G,
	         GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
	         GraphAttributes::nodeLabel | GraphAttributes::nodeType |
	         GraphAttributes::edgeStyle | GraphAttributes::edgeLabel |
	         GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate |
	         GraphAttributes::edgeArrow | GraphAttributes::edgeIntWeight | GraphAttributes::edgeType);

	if (treeFigureTitle.length() != 0)
	{
		graphLabel = G.newNode();
		GA.shape (graphLabel) = Shape::Ellipse;
		GA.width (graphLabel) = 0.0;
		GA.height (graphLabel) = 0.0;
		GA.strokeWidth (graphLabel) = 0.0;
		GA.label (graphLabel) = (const char *) (treeFigureTitle.c_str() );
	}

	GA.directed() = false;
}

void HuffmanTree::layout()
{
	TreeLayout btreeLayout;

	btreeLayout.siblingDistance (35.0);
	btreeLayout.levelDistance (40.0);
	btreeLayout.subtreeDistance (50.0);
	btreeLayout.rootSelection (TreeLayout::RootSelectionType::Source);
	btreeLayout.orientation (Orientation::topToBottom);
	btreeLayout.call (GA);
}

node HuffmanTree::insertNode (const wxString& label)
{
	node t = G.newNode();

	GA.shape (t) = Shape::Rect;
	GA.label (t) = (const char *) (label.c_str() );
	GA.strokeWidth (t) = 1.0;
	GA.width (t) = 50.0;
	GA.height (t) = 50.0;

	return t;
}

node HuffmanTree::createLeafNode (const HuffmanNode& h)
{
	wxString label;
	node t;

	if (h.data != (wxChar) '\0')
	{
		label.Printf (wxT ("%c: %d"), h.data, h.frequency);
		t = insertNode (label.c_str() );
	}
	else
	{
		t = G.newNode();
		GA.shape (t) = Shape::Rect;
		GA.label(t) = string("");
		GA.strokeWidth (t) = 0.0;
		GA.width (t) = 0.0;
		GA.height (t) = 0.0;
	}

	return t;
}

node HuffmanTree::createInteriorNode (const HuffmanNode& h)
{
	wxString label;
	node t;

	label.Printf (wxT ("%d"), h.frequency);
	t = insertNode (label);

	return t;
}

void HuffmanTree::buildHuffmanTree (const vector<HuffmanNode>& tnodes)
{
	priority_queue<HuffmanNode, vector<HuffmanNode>,less<vector<HuffmanNode>::value_type> > minHuffmanPQ;
	unsigned int numberOfLeaves = tnodes.size();
	HuffmanNode ht, x, y, parent_x_y;
	node t, parent, lchild, rchild;
	edge e;
	wxString label;
	unsigned int i;

	init();

	uncompressed_cost = 0;

	// push the leaf nodes on the priority queue
	for (i=0; i < tnodes.size(); i++)
	{
		ht = tnodes[i];
		t = createLeafNode (ht);
		ht.myNodeInTree = t;
		minHuffmanPQ.push (ht);
		uncompressed_cost += 8*ht.frequency;
		leaf.push_back (t);

	}

	if (numberOfLeaves == 1)
	{
		ht = HuffmanNode ( (wxChar) '\0', 0);
		t = createLeafNode (ht);
		ht.myNodeInTree = t;
		minHuffmanPQ.push (ht);
		numberOfLeaves = 2;
	}

	// we will accumulate cost as interior nodes are generated
	cost = 0;

	// for numberLeaves-1 iterations, remove pairs of HuffmanNodes,
	// create the two children and the parent, and then insert into the tree
	for (i=0; i< numberOfLeaves-1; i++)
	{
		// remove the two nodes with least frequency
		x = minHuffmanPQ.top();
		minHuffmanPQ.pop();
		lchild = x.myNodeInTree;
		GA.fillColor(lchild) = ogdf::Color(255, 215, 0);
		y = minHuffmanPQ.top();
		minHuffmanPQ.pop();
		rchild = y.myNodeInTree;
		GA.fillColor(rchild) = ogdf::Color(255, 215, 0);
		cost += x.frequency + y.frequency;
		parent_x_y = HuffmanNode ( (wxChar) '\0', x.frequency + y.frequency, NULL);
		parent = createInteriorNode (parent_x_y);
		parent_x_y.myNodeInTree = parent;

		if (x.frequency != 0)
		{
			e = G.newEdge (parent, lchild);
			GA.label (e) = "0";
			GA.arrowType (e) = ogdf::EdgeArrow::None;
		}
		else
			G.delNode (lchild);

		e = G.newEdge (parent, rchild);
		GA.label (e) = "1";
		GA.arrowType (e) = ogdf::EdgeArrow::None;
		minHuffmanPQ.push (parent_x_y);
	}

	root = minHuffmanPQ.top().myNodeInTree;
	GA.fillColor(root) = ogdf::Color(255, 215, 0);
	minHuffmanPQ.pop();

	if (treeFigureTitle.length() != 0)
	{
		edge e = G.newEdge (graphLabel, root);
		GA.arrowType (e) = ogdf::EdgeArrow::None;
		GA.type (e) = Graph::EdgeType::association;
		GA.strokeWidth (e) = 1.0;
	}

	layout();
}

void HuffmanTree::buildFromGML(const wxString& gfileName)
{
	string sgfilename ( (const char *) gfileName.c_str() );
	std::ifstream gmlIn(sgfilename.c_str());
	//TreeLayout btreeLayout;

	if (!GraphIO::readGML (G, gmlIn) )
		throw btrFileFormatException ("Failure reading GML file");

	if (G.numberOfNodes() == 0)
		return;

	List<node> allTreeNodes;
	ListIterator<node> iter;

	G.allNodes (allTreeNodes);

	// Is there a tree label?
	iter = allTreeNodes.begin();

	while (iter != allTreeNodes.end() )
	{
		if (GA.shape (*iter) == Shape::Ellipse)
		{
			List<edge> outEdge;
			edge edgeToRoot;

			treeFigureTitle = wxString (GA.label (*iter).c_str(),wxConvUTF8);
			(*iter)->outEdges (outEdge);
			edgeToRoot = * (outEdge.begin() );
			root = (*edgeToRoot).target();

			return;
		}

		iter++;
	}

	// There is no tree label. Find root.
	iter = allTreeNodes.begin();

	while (iter != allTreeNodes.end() )
	{
		if ( (**iter).indeg() == 0)
		{
			root = *iter;
			break;
		}

		iter++;
	}
}

void HuffmanTree::outputBitCodes (wxTextCtrl *bitCodes)
{
	vector<int> bit;

	List<edge> parent;
	ListIterator<edge> iter;
	node t;
	edge e;
	int weight;
	char leafChar;
	wxString s;
	ogdf::Shape sh;

	s.Printf (wxT ("\nCompression ratio = %.3f\n\n"), (double) uncompressed_cost/cost);
	bitCodes->AppendText (s);

	// cycle through the leaves moving up to the root in order
	// to find the bit codes
	for (unsigned int i=0; i < leaf.size(); i++)
	{
		if (GA.shape(leaf[i]) == Shape::Ellipse)
			continue;

		bit.clear();
		leafChar = (GA.label (leaf[i]) ) [0];
		leaf[i]->inEdges (parent);
		iter = parent.begin();

		// if we are not at the root, find the edge weight (0 or 1) and the parent node
		// insert the weight into bit
		while (true)
		{
			e = *iter;
			weight = (GA.label (e) ) [0] - '0';
			bit.push_back (weight);
			t = (*e).source();
			parent.clear();
			t->inEdges (parent);

			if (parent.empty() )
				break;

			iter = parent.begin();
		}

		// now output the leaf and the contents of bits in reverse
		s.Printf (wxT ("%c: "), leafChar);
		bitCodes->AppendText (s);

		while (!bit.empty() )
		{
			int b = bit.back();

			if (b >= 0)
			{
				s.Printf (wxT ("%d "), bit.back() );
				bitCodes->AppendText (s);
			}
			bit.pop_back();
		}

		s.Printf (wxT ("\n") );
		bitCodes->AppendText (s);
	}

	bitCodes->SetInsertionPoint (0);
}

node HuffmanTree::getRoot()
{
	return root;
}

// called when opening a Huffman tree GML file
void HuffmanTree::outputGMLData (wxTextCtrl *bitCodes)
{
	uncompressed_cost = 0;
	cost = 0;
	leaf.clear();

	computeCosts (root);
	outputBitCodes (bitCodes);
}

void HuffmanTree::computeCosts (node t)
{
	List<edge> children;
	ListIterator<edge> iter;

	t->outEdges (children);

	if (children.empty() )
	{
		// leaf node
		uncompressed_cost += 8*atoi (GA.label (t).c_str() +2);
		leaf.push_back (t);
		return;
	}
	else
	{
		// interior node has two children, unless there
		// there is only one leaf node
		cost += atoi (GA.label (t).c_str() );
		iter = children.begin();
		computeCosts ( (**iter).target() );
		iter++;

		if (iter != children.end() )
			computeCosts ( (**iter).target() );
	}
}
