/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/search/RBTree.h"

RBNode *RBTree::NIL = NULL;

void RBTree::makeEmptyTree()
{
	if (NIL == NULL)
	{
		// first time an RBTree object created. initialize
		// NIL. all its pointers are NULL, and its color
		// is BLACK

		NIL = new RBNode;

		NIL->right = NULL;
		NIL->left = NULL;
		NIL->parent = NULL;
		NIL->color = BLACK;
	}

	// root is NIL, tree size is 0
	root = NIL;
	treeSize = 0;
}

void RBTree::deleteTree (RBNode *t)
{
	if (t != NULL)
	{
		deleteTree (t->left);
		deleteTree (t->right);
		t = NULL;
	}
}

void RBTree::split4Node (RBNode *x)
{
	// perform the color flip
	x->color = RED;
	x->left->color = BLACK;
	x->right->color = BLACK;

	// if we split the root, we are done
	if (x == root)
		return;

	// to see if a rotation is required, we need the
	// parent of x. x is not root, so p != NIL
	RBNode *p = x->parent;

	// a rotation is needed if the parent of x is RED
	if (p->color == RED)
	{
		// we need the grandparent of x. since the root
		// is BLACK, p cannot be root, so the grandparent
		// exists
		RBNode *g = x->parent->parent;

		// the grandparent of x will be RED
		g->color = RED;

		// a double rotation is required if x is an inside
		// grandchild. check this by seeing if the orientations
		// of p to g and x to p are different
		if (p == g->left && x == p->right)
		{
			// perform a double right rotation
			// first move x up one level and p down
			rotateLeft (x);

			// node x will be BLACK
			x->color = BLACK;
			// prepare for a right single rotation
			p = x;
		}
		else if (p == g->right && x == p->left)
		{
			// perform a double left rotation
			// first move x up one level and p down
			rotateRight (x);

			// node x will be BLACK
			x->color = BLACK;
			// prepare for a left single rotation
			p = x;
		}
		else
			// single rotation. parent will be BLACK
			p->color = BLACK;

		// perform a single rotation
		// move p up and g down
		if (p == g->left)
			rotateRight (p);
		else
			rotateLeft (p);
	}
}

void RBTree::rotateRight (RBNode *pivot)
{
	// need the parent and grandparent of pivot
	RBNode *p = pivot->parent, *g = pivot->parent->parent;

	// adjust right and left pointers
	p->left = pivot->right;
	pivot->right = p;

	// adjust parent pointers
	pivot->parent = g;
	p->parent = pivot;

	// don't reset the parent link of the left child of p
	// if the left child is NIL. this will interfere with
	// the use of NIL in rbDeleteFixup()
	if (p->left != NIL)
		p->left->parent = p;

	if (p == root)
		// pivot is the new root
		root = pivot;
	else if (p == g->right)
		// right link of g must point at pivot now
		g->right = pivot;
	else
		// left link of g must point at pivot now
		g->left = pivot;
}

// code is the same as the single right rotation,
// with left and right interchanged
void RBTree::rotateLeft (RBNode *pivot)
{
	RBNode *p = pivot->parent, *g = pivot->parent->parent;

	p->right = pivot->left;
	pivot->left = p;

	pivot->parent = g;
	p->parent = pivot;

	if (p->right != NIL)
		p->right->parent = p;

	if (p == root)
		root = pivot;
	else if (p == g->right)
		g->right = pivot;
	else
		g->left = pivot;
}

void RBTree::rbDeleteFixup (RBNode *x)
{
	RBNode *siblingOfx;

	while (x != root && x->color == BLACK)

		if (x == x->parent->left)
		{
			siblingOfx = x->parent->right;

			if (siblingOfx->color == RED)
			{
				// CASE 1:
				//    sibling of x is RED. perform
				//    color changes and a left rotation.
				//    this produces a configuration that
				//    corresponds to cases 2, 3, or 4
				siblingOfx->color = BLACK;
				x->parent->color = RED;
				rotateLeft (siblingOfx);
				siblingOfx = x->parent->right;
			}

			if (siblingOfx->left->color == BLACK &&
			      siblingOfx->right->color == BLACK)
			{
				// CASE 2:
				//    both the children of the siblingOfx are BLACK.
				//    take a BLACK off of x and siblingOfx. x now
				//    has only one BLACK, and siblingOfx is RED.
				//    consider parent(x) to have an extra BLACK.
				//    if we enter this case after executing case 1,
				//    x becomes RED, and the loop terminates
				siblingOfx->color = RED;
				x = x->parent;
			}
			else
			{
				if (siblingOfx->right->color == BLACK)
				{
					// CASE 3:
					//    siblingOfx is BLACK, its left child is
					//    RED, and its right child is BLACK. perform
					//    color changes and right rotation. this
					//    transforms the node configuration into
					//    case 4, which termintes the loop

					siblingOfx->left->color = BLACK;
					siblingOfx->color = RED;
					rotateRight (siblingOfx->left);
					siblingOfx = x->parent->right;
				}

				// CASE 4:
				//    siblingOfx is BLACK and siblingOfx has a
				//    RED right child. after color changes
				//    and a left rotation, the extra BLACK on
				//    x is removed. set x to the root to terminate
				//    the loop
				siblingOfx->color = x->parent->color;
				x->parent->color = BLACK;
				siblingOfx->right->color = BLACK;
				rotateLeft (siblingOfx);
				x = root;
			}
		}
		else	// same as x == x->parent->left, except that
			// "left" and "right" are interchanged
		{
			siblingOfx = x->parent->left;

			if (siblingOfx->color == RED)
			{
				siblingOfx->color = BLACK;
				x->parent->color = RED;
				rotateRight (siblingOfx);
				siblingOfx = x->parent->left;
			}

			if (siblingOfx->right->color == BLACK &&
			      siblingOfx->left->color == BLACK)
			{
				siblingOfx->color = RED;
				x = x->parent;
			}
			else
			{
				if (siblingOfx->left->color == BLACK)
				{
					siblingOfx->right->color = BLACK;
					siblingOfx->color = RED;
					rotateLeft (siblingOfx->right);
					siblingOfx = x->parent->left;
				}

				siblingOfx->color = x->parent->color;
				x->parent->color = BLACK;
				siblingOfx->left->color = BLACK;
				rotateRight (siblingOfx);
				x = root;
			}
		}

	x->color = BLACK;
}

RBNode *RBTree::findNode (const int item) const
{
	// cycle t through the tree starting with root
	RBNode *t = root;

	// terminate on on empty subtree
	while (t != NIL && ! (item == t->nodeValue))
		if (item < t->nodeValue)
			t = t->left;
		else
			t = t->right;

	// return pointer to node; NIL if not found
	return t;
}

RBTree::RBTree (const wxVector<int>& inputData)
{
	makeEmptyTree();

	for (unsigned int i=0; i < inputData.size(); i++)
		add (inputData[i]);
}

RBTree::~RBTree()
{
	deleteTree (root);
	root = NULL;
	treeSize = 0;
}

int RBTree::size() const
{
	return treeSize;
}

void RBTree::add (const int item)
{
	// TOP-DOWN INSERTION

	// declare pointers to the current node and its parent
	RBNode *curr = root, *parent = NIL, *newNode;

	// loop until we find the value in the tree or locate
	// the insertion point
	while (curr != NIL)
	{
		if (curr->nodeValue == item)
			return;

		// a node split is required if both children of curr are RED
		if (curr->left->color == RED && curr->right->color == RED)
			split4Node (curr);

		// move down the tree
		parent = curr;

		if (item < curr->nodeValue)
			curr = curr->left;
		else
			curr = curr->right;
	}

	// create the new node
	newNode = new RBNode (item, NIL, NIL, parent, RED);

	// is item the first node inserted into the tree?
	if (parent == NIL)
		// item is at the root of a brand new tree.
		root = newNode;
	else
	{
		// link the new node into its parent
		if (item < parent->nodeValue)
			// insert as left child
			parent->left = newNode;
		else
			// insert as right child
			parent->right = newNode;

		// if the new node's parent is RED, we
		// must perform a rotation
		if (parent->color == RED)
			split4Node (newNode);
	}

	// the color of the root must be BLACK
	root->color = BLACK;

	// the tree has one more node
	treeSize++;
}

void RBTree::clear()
{
	deleteTree (root);
	root = NULL;
	treeSize = 0;
}

bool RBTree::empty() const
{
	return treeSize == 0;
}

void RBTree::erase (const int item)
{
	// search tree for item
	RBNode *p = findNode (item);

	p = findNode (item);

	// if item found, delete the node
	if (p != NIL)
		eraseNode (p);
}

void RBTree::eraseNode (RBNode *pos)
{
	// BOTTOM-UP ERASE

	// dNodePtr = pointer to node D that is deleted
	// pNodePtr = pointer to parent P of node D
	// rNodePtr = pointer to node R that replaces D
	// spliceOut = pointer to the node that is spliced out of the tree
	// childOfSpliceOut = pointer to the child of the node we splice out
	RBNode *dNodePtr = pos, *pNodePtr, *rNodePtr,
	        *spliceOut, *childOfSpliceOut;
	// saves color of node that is spliced out
	Color spliceoutColor;

	// assign pNodePtr the address of P
	pNodePtr = dNodePtr->parent;

	// If D has a NIL pointer, the
	// replacement node is the other child
	if (dNodePtr->right == NIL || dNodePtr->left == NIL)
	{
		if (dNodePtr->right == NIL)
			rNodePtr = dNodePtr->left;
		else
			rNodePtr = dNodePtr->right;

		// the parent of R is now the parent of D
		// NOTE: rNodePtr may be NIL
		rNodePtr->parent = pNodePtr;

		// we are splicing dNodePtr out of the tree
		spliceOut = dNodePtr;
		// save color of dNodePtr
		spliceoutColor = spliceOut->color;
		// record the child of our spliceout node for possible
		// balancing operations
		childOfSpliceOut = rNodePtr;
	}
	// both pointers of dNodePtr are non-NIL.
	else
	{
		// find and unlink replacement node for D.
		// starting at the right child of node D,
		// find the node whose value is the smallest of all
		// nodes whose values are greater than the value in D.
		// unlink the node from the tree.

		// pOfRNodePtr = pointer to parent of replacement node
		RBNode *pOfRNodePtr = dNodePtr;

		// first possible replacement is right child of D
		rNodePtr = dNodePtr->right;

		// descend down left subtree of the right child of D,
		// keeping a record of current node and its parent.
		// when we stop, we have found the replacement
		while (rNodePtr->left != NIL)
		{
			pOfRNodePtr = rNodePtr;
			rNodePtr = rNodePtr->left;
		}

		// we are splicing rNodePtr out of the tree at its
		// current location
		spliceOut = rNodePtr;
		// save color of the node we will splice out
		spliceoutColor = spliceOut->color;
		// we will replace dNodPtr by spliceOut. make the color
		// of spliceOut the color of dNodePtr
		spliceOut->color = dNodePtr->color;
		// record the child of our spliceout node for possible
		// balancing operations
		childOfSpliceOut = rNodePtr->right;

		if (pOfRNodePtr == dNodePtr)
		{
			// right child of deleted node is the replacement.
			// assign left subtree of D to left subtree of R
			rNodePtr->left = dNodePtr->left;
			// assign the parent of D as the parent of R
			rNodePtr->parent = pNodePtr;
			// assign the left child of D to have parent R
			dNodePtr->left->parent = rNodePtr;

			// if the right child of rNodePtr is NIL,
			// make rNodrPtr the parent of NIL, since any
			// required fixup will start with the child
			if (rNodePtr->right == NIL)
				rNodePtr->right->parent = rNodePtr;
		}
		else
		{
			// we moved at least one node down a left branch
			// of the right child of D. unlink R from tree by
			// assigning its right subtree as the left child of
			// the parent of R
			pOfRNodePtr->left = rNodePtr->right;

			// the parent of the right child of R is the
			// parent of R
			// NOTE: rNodePtr may be NIL
			rNodePtr->right->parent = pOfRNodePtr;

			// put replacement node in place of dNodePtr
			// assign children of R to be those of D
			rNodePtr->left = dNodePtr->left;
			rNodePtr->right = dNodePtr->right;
			// assign the parent of R to be the parent of D
			rNodePtr->parent = pNodePtr;
			// assign the parent pointer in the children
			// of R to point at R
			rNodePtr->left->parent = rNodePtr;
			rNodePtr->right->parent = rNodePtr;
		}
	}

	// complete the link to the parent node.

	// deleting the root node. assign new root
	if (pNodePtr == NIL)
		root = rNodePtr;
	// attach R to the correct branch of P
	else if (dNodePtr->nodeValue < pNodePtr->nodeValue)
		pNodePtr->left = rNodePtr;
	else
		pNodePtr->right = rNodePtr;

	// delete the node from memory and decrement tree size
	delete dNodePtr;

	// fixup the tree if the node spliced out is BLACK
	if (spliceoutColor == BLACK)
		rbDeleteFixup (childOfSpliceOut);

	treeSize--;
}

RBNode *RBTree::getRoot() const
{
	return root;
}
