#ifndef AVLTREE_H
#define AVLTREE_H

#include <wx/vector.h>

using namespace std;

class AVLNode
{
public:
	int nodeValue;
	AVLNode *left, *right;
	int height;

	AVLNode (const int value, AVLNode *lptr = (AVLNode *) 0, AVLNode *rptr = (AVLNode *) 0, int h = 0) :
		nodeValue (value), left (lptr), right (rptr), height (h)
	{}
};

// perform a single right rotation for parent p
AVLNode *singleRotateRight (AVLNode *p);

// perform a single left rotation for parent p
AVLNode *singleRotateLeft (AVLNode *p);

// perform a double right rotation for parent p
AVLNode *doubleRotateRight (AVLNode *p);

// perform a single left rotation for parent p
AVLNode *doubleRotateLeft (AVLNode *p);

int height (AVLNode *t);

int max (int a, int b);


class AVLTree
{
public:

	AVLTree (const wxVector<int>& inputData);
	virtual ~AVLTree();
	int size() const;
	void add (const int item);
	void clear();
	bool empty() const;
	void erase (const int item);
	AVLNode *getRoot() const;

private:

	AVLNode *root;
	AVLNode *q;
	int treeSize;

	void deleteTree (AVLNode *t);
	AVLNode *addNode (AVLNode *t, const int item);
	AVLNode *findNode (const int item, AVLNode* &parent) const;
	void deleteItem (const int item, AVLNode * & t);
	void del (AVLNode * &t, const int item);
	void balanceR (AVLNode * &p, const int item);
	void balanceL (AVLNode * &p, const int item);
};

#endif // AVLTREE_H
