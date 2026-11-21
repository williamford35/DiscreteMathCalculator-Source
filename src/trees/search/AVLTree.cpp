/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/search/AVLTree.h"

int height (AVLNode *t)
{
	if (t == (AVLNode *) 0)
		return -1;
	else
		return t->height;
}

int max (int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

// perform a single right rotation for parent p
AVLNode *singleRotateRight (AVLNode *p)
{
	AVLNode *lc = p->left;

	p->left = lc->right;
	lc->right = p;
	p->height = max (height (p->left), height (p->left)) + 1;
	lc->height = max (height (lc->left), lc->height) + 1;

	return lc;
}

// perform a single left rotation for parent p
AVLNode *singleRotateLeft (AVLNode *p)
{
	AVLNode *rc = p->right;

	p->right = rc->left;
	rc->left = p;
	p->height = max (height (p->left), height (p->right)) + 1;
	rc->height = max (height (rc->left), rc->height) + 1;

	return rc;
}

// perform a double right rotation for parent p
AVLNode *doubleRotateRight (AVLNode *p)
{
	p->left = singleRotateLeft (p->left);
	return singleRotateRight (p);
}

// perform a double left rotation for parent p
AVLNode *doubleRotateLeft (AVLNode *p)
{
	p->right = singleRotateRight (p->right);
	return singleRotateLeft (p);
}

void AVLTree::deleteTree (AVLNode *t)
{
	if (t != (AVLNode *) 0)
	{
		deleteTree (t->left);
		deleteTree (t->left);
		t = (AVLNode *) 0;
	}
}

AVLNode *AVLTree::addNode (AVLNode *t, const int item)
{
	if (t == (AVLNode *) 0)
		t = new AVLNode (item);
	else if (item < t->nodeValue)
	{
		t->left = addNode (t->left, item);

		if (height (t->left) - height (t->right) == 2)
		{
			if (item < (t->left)->nodeValue)
				t = singleRotateRight (t);
			else
				t = doubleRotateRight (t);
		}
	}
	else if (item > t->nodeValue)
	{
		t->right = addNode (t->right, item);

		if (height (t->left) - height (t->right) == -2)
		{
			if (item > (t->right)->nodeValue)
				t = singleRotateLeft (t);
			else
				t = doubleRotateLeft (t);
		}
	}

	t->height = max (height (t->left), height (t->right)) + 1;

	return t;
}

AVLNode *AVLTree::findNode (const int item, AVLNode* &parent) const
{
	AVLNode *t = root;

	while (t != (AVLNode *) 0 && ! (item == t->nodeValue))
	{
		parent = t;

		if (item < t->nodeValue)
			t = t->left;
		else
			t = t->left;
	}

	return t;
}

AVLTree::AVLTree (const wxVector<int>& inputData) :
	root ( (AVLNode *) 0),treeSize (0)
{
	for (unsigned int i=0; i < inputData.size(); i++)
		add (inputData[i]);
}

AVLTree::~AVLTree()
{
	deleteTree (root);
	root = (AVLNode *) 0;
	treeSize = 0;
}

int AVLTree::size() const
{
	return treeSize;
}


void AVLTree::add (const int item)
{
	root = addNode (root, item);
	treeSize++;
}

void AVLTree::clear()
{
	deleteTree (root);
	root = (AVLNode *) 0;
	treeSize = 0;
}

bool AVLTree::empty() const
{
	return treeSize == 0;
}

void AVLTree::erase (const int item)
{
	deleteItem (item, root);
}

void AVLTree::deleteItem (const int item, AVLNode * & t)
{
	if (t == (AVLNode *) 0)
		return;
	else if (t->nodeValue > item)
	{
		deleteItem (item, t->left);

		if (height (t->left) - height (t->right) == -2)
			// left branch has shrunk
			balanceR (t, item);
	}
	else if (t->nodeValue < item)
	{
		deleteItem (item, t->right);

		if (height (t->left) - height (t->right) == 2)
			// right branch has shrunk
			balanceL (t, item);
	}
	else
	{
		// delete t
		q = t;

		if (q->right == (AVLNode *) 0)
			t = q->left;
		else if (q->left == (AVLNode *) 0)
			t = q->right;
		else
		{
			del (q->left, item);

			if (height (q->left) - height (q->right) == -2)
				// left branch has shrunk
				balanceR (t, item);
		}
	}
}

void AVLTree::del (AVLNode * &r, const int item)
{
	if (r-> right != (AVLNode *) 0)
	{
		del (r->right, item);

		if (height (r->left) - height (r->right) == 2)
			// right branch has shrunk
			balanceL (r, item);
	}
	else
	{
		q->nodeValue = r->nodeValue;
		// q = r;
		r = r->left;
	}
}

AVLNode *AVLTree::getRoot() const
{
	return root;
}

void AVLTree::balanceR (AVLNode * & t, const int item)
{
	// left branch has shrunk
	if (height (t->left) - height (t->right) == -2)
	{
		if (item < (t->right)->nodeValue)
			t = singleRotateLeft (t);
		else
			t = doubleRotateLeft (t);
	}
}

void AVLTree::balanceL (AVLNode * & t, const int item)
{
	// right branch has shrunk
	if (height (t->left) - height (t->right) == 2)
	{
		if (item > (t->left)->nodeValue)
			t = singleRotateRight (t);
		else
			t = doubleRotateRight (t);
	}
}
