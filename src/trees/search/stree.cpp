/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/search/stree.h"

void stree::add (const int item)
{
	stnode *t = root, *parent = (stnode *) 0, *newNode;

	while (t != (stnode *) 0)
	{
		parent = t;

		if (item == t->nodeValue)
			return;
		else if (item < t->nodeValue)
			t = t->left;
		else
			t = t->right;
	}

	newNode = new stnode (item, (stnode *) 0, (stnode *) 0);

	if (parent == (stnode *) 0)
		root = newNode;
	else if (item < parent->nodeValue)
		parent->left = newNode;
	else
		parent->right = newNode;

	treeSize++;
	inputData.push_back (item);
}

void stree::clear()
{
	deleteTree (root);
	root = (stnode *) 0;
	treeSize = 0;
}

void stree::deleteTree (stnode *t)
{
	if (t != (stnode *) 0)
	{
		deleteTree (t->left);
		deleteTree (t->right);
		delete t;
	}
}

stnode *stree::findNode (const int item, stnode* &parent) const
{
	stnode *t = root;
	parent = (stnode *) 0;

	while (t != (stnode *) 0 && ! (item == t->nodeValue))
	{
		parent = t;

		if (item < t->nodeValue)
			t = t->left;
		else
			t = t->right;
	}

	return t;
}

stree::stree (const wxVector<int>& data) :
	root ( (stnode *) 0),treeSize (0)
{
	for (unsigned int i=0; i< data.size(); i++)
		add (data[i]);
}

stree::~stree()
{
	deleteTree (root);
}

int stree::empty() const
{
	return root == (stnode *) 0;
}

int stree::size() const
{
	return treeSize;
}

void stree::erase (stnode *p, stnode *parent)
{
	// dNodePtr = pointer to node D that is deleted
	// pNodePtr = pointer to parent P of node D
	// rNodePtr = pointer to node R that replaces D
	stnode *dNodePtr = p, *pNodePtr, *rNodePtr;

	if (treeSize == 0)
		return;

	pNodePtr = parent;

	// If D has a NULL pointer, the
	// replacement node is the other child
	if (dNodePtr->left == (stnode *) 0 || dNodePtr->right == (stnode *) 0)
	{
		if (dNodePtr->right == (stnode *) 0)
			rNodePtr = dNodePtr->left;
		else
			rNodePtr = dNodePtr->right;
	}
	// both pointers of dNodePtr are non-NULL.
	else
	{
		// find and unlink replacement node for D.
		// starting at the right child of node D,
		// find the node whose value is the smallest of all
		// nodes whose values are greater than the value in D.
		// unlink the node from the tree.

		// pOfRNodePtr = pointer to parent of replacement node
		stnode *pOfRNodePtr = dNodePtr;

		// first possible replacement is right child of D
		rNodePtr = dNodePtr->right;

		// descend down left subtree of the right child of D,
		// keeping a record of current node and its parent.
		// when we stop, we have found the replacement
		while (rNodePtr->left != (stnode *) 0)
		{
			pOfRNodePtr = rNodePtr;
			rNodePtr = rNodePtr->left;
		}

		if (pOfRNodePtr == dNodePtr)
			// right child of deleted node is the replacement.
			// assign left subtree of D to left subtree of R
			rNodePtr->left = dNodePtr->left;
		else
		{
			// we moved at least one node down a left branch
			// of the right child of D. unlink R from tree by
			// assigning its right subtree as the left child of
			// the parent of R
			pOfRNodePtr->left = rNodePtr->right;

			// put replacement node in place of dNodePtr
			// assign children of R to be those of D
			rNodePtr->left = dNodePtr->left;
			rNodePtr->right = dNodePtr->right;
		}
	}

	// deleting the root node. assign new root
	if (pNodePtr == (stnode *) 0)
		root = rNodePtr;
	// attach R to the correct branch of P
	else if (dNodePtr->nodeValue < pNodePtr->nodeValue)
		pNodePtr->left = rNodePtr;
	else
		pNodePtr->right = rNodePtr;

	// delete the node from memory and decrement tree size
	delete dNodePtr;
	treeSize--;
}

void stree::erase (const int item)
{
	stnode *p, *parent = root;

	p = findNode (item, parent);

	// if item found, delete the node
	if (p != (stnode *) 0)
		erase (p, parent);
}

wxVector<int> stree::getInputData() const
{
	return inputData;
}

stnode *stree::getRoot() const
{
	return root;
}
