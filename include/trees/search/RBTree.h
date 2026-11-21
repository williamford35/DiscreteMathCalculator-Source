#ifndef RBTREE_H
#define RBTREE_H

#include <wx/vector.h>
#include <stddef.h>

enum Color
{
	RED,
	BLACK
};

class RBNode
{
public:
	int nodeValue;
	RBNode *left, *right, *parent;
	::Color color;

	RBNode()
	{}

	RBNode (const int value, RBNode *lptr = NULL, RBNode *rptr = NULL,
	        RBNode *p = NULL, ::Color colour = BLACK) :
		nodeValue (value), left (lptr), right (rptr), parent (p), color (colour)
	{}
};

class RBTree
{
public:

	RBTree (const wxVector<int>& inputData);
	virtual ~RBTree();
	int size() const;
	void add (const int item);
	void clear();
	bool empty() const;
	void erase (const int item);
	RBNode *getRoot() const;

	static RBNode *NIL;

private:

	RBNode *root;
	int treeSize;

	void deleteTree (RBNode *t);
	RBNode *addNode (RBNode *t, const int item);
	RBNode *findNode (const int item) const;

	// The first time an RBTree object created, initialize
	// NIL. Its pointers are NULL, and its color is BLACK
	void makeEmptyTree();

	// break up a 4-node, performing a rotation, if necessary
	void split4Node (RBNode *x);

	// perform a single right rotation
	void rotateRight (RBNode *pivot);

	// perform a single left rotation
	void rotateLeft (RBNode *pivot);

	// fix up the tree when x is the child of
	// a BLACK node that was unlinked from its
	// position in the tree
	void rbDeleteFixup (RBNode *x);

	void eraseNode (RBNode* pos);
};

#endif
