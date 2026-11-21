#ifndef BINARY_SEARCH_TREE_CLASS
#define BINARY_SEARCH_TREE_CLASS

#include <wx/vector.h>

class stnode
{
public:
	int nodeValue;
	stnode *left;
	stnode *right;

	stnode (int item, stnode *lptr = (stnode *) 0,
	        stnode *rptr = (stnode *) 0) : nodeValue (item), left (lptr), right (rptr)
	{}

	virtual ~stnode()
	{}
};

class stree
{
public:

	stree (const wxVector<int>& data);
	virtual ~stree();

	int empty() const;
	int size() const;
	void add (const int item);
	void erase (const int item);
	void clear();
	wxVector<int> getInputData() const;
	stnode *getRoot() const;

private:
	stnode *root;
	int treeSize;
	wxVector<int> inputData;

	void deleteTree (stnode *t);
	stnode *findNode (const int item, stnode* &parent) const;
	void erase (stnode *p, stnode *parent);
};

#endif
