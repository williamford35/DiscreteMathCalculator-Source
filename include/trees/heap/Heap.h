#ifndef HEAP_FUNCTIONS_H
#define HEAP_FUNCTIONS_H

#include <wx/vector.h>
#include <functional>

using namespace std;

template <typename Compare>
void pushHeap (wxVector<int>& v, int last, Compare comp);

template <typename Compare>
void adjustHeap (wxVector<int>& v, int first, int last, Compare comp);

template <typename Compare>
void popHeap (wxVector<int>& v, int last, Compare comp);

template <typename Compare>
void makeHeap (wxVector<int>& v, Compare comp);

template <typename Compare>
bool isHeap (const wxVector<int>& v, int last, Compare comp);

template <typename Compare>
void pushHeap (wxVector<int>& v, int last, Compare comp)
{
	// assume the new item is at location v[last-1] and that
	// the elements v[0] to v[last-2] are in heap order
	int currentPos, parentPos;
	int target;

	// currentPos is an index that traverses path of parents.
	// target is value hlist[i] and is repositioned in path
	currentPos = last-1;
	parentPos = (currentPos-1) /2;
	target = v[last-1];

	// traverse path of parents up to the root
	while (currentPos != 0)
	{
		// compare target and parent value
		if (comp (target,v[parentPos]))
		{
			// move data from parent position to current
			// position. update current position to parent
			// position. compute next parent
			v[currentPos] = v[parentPos];
			currentPos = parentPos;
			parentPos = (currentPos-1) /2;
		}
		else
			// if !comp(target, parentvalue), heap condition is ok. break
			break;
	}

	// the correct location has been discovered. assign target
	v[currentPos] = target;
}


template <typename Compare>
void adjustHeap (wxVector<int>& v, int first, int last, Compare comp)
{
	int currentPos, childPos;
	int target;

	// start at first and filter target down the heap
	currentPos = first;
	target = v[first];

	// compute the left child index and begin a scan down
	// path of children, stopping at end of list (last)
	// or when we find a place for target
	childPos = 2 * currentPos + 1;

	while (childPos <= last-1)
	{
		// index of right child is childPos+1. compare the
		// two children. change childPos if
		// comp(v[childPos+1], v[childPos]) is true
		if ( (childPos+1 <= last-1) &&
		      comp (v[childPos+1], v[childPos]))
			childPos = childPos + 1;

		// compare selected child to target
		if (comp (v[childPos],target))
		{
			// comp(selected child, target) is true.
			// move selected child to the parent;
			// position of selected child is now vacated
			v[currentPos] = v[childPos];

			// update indices to continue the scan
			currentPos = childPos;
			childPos = 2 * currentPos + 1;
		}
		else
			// target belongs at currentPos
			break;
	}

	v[currentPos] = target;
}

template <typename Compare>
void popHeap (wxVector<int>& v, int last, Compare comp)
{
	v[0] = v[last-1];

	// filter down the root over the range [0, last-1)
	adjustHeap (v, 0, last-1, comp);
}

template <typename Compare>
void makeHeap (wxVector<int>& v, Compare comp)
{
	int heapPos, lastPos;

	// compute the size of teh heap and the index
	// of the last parent
	lastPos = v.size();
	heapPos = (lastPos - 2) /2;

	// filter down every parent in order from last parent
	// down to root
	while (heapPos >= 0)
	{
		adjustHeap (v,heapPos, lastPos, comp);
		heapPos--;
	}
}

template <typename Compare>
bool isHeap (const wxVector<int>& v, int last, Compare comp)
{
	int parent, lchild, rchild;

	// parent is the index of the last nonleaf node in
	// the tree (unless the tree has 1 node)
	parent = (last-2) /2;
	// the children of the last nonleaf node
	lchild = 2*parent + 1;
	rchild = lchild + 1;

	if (last == 1)
		// tree has 1 node and is a heap
		return true;
	else
	{
		// the tree has at least 2 nodes, so parent has a left
		// child. make sure parent and left child are in the
		// correct order
		if (!comp (v[parent], v[lchild]))
			// wrong order. return false
			return false;

		// verify that parent has a right child
		if (rchild < last)

			// parent has a right child. make sure parent
			// and right child are in the correct order
			if (!comp (v[parent], v[rchild]))
				return false;
	}

	// all remaining nonleaf nodes have 2 children.
	// move up to parent = 0 (root) and verify that
	// the heap condition is true
	parent--;

	while (parent >= 0)
	{
		lchild = 2*parent + 1;
		rchild = lchild + 1;

		if (!comp (v[parent], v[lchild]))
			return false;

		if (rchild < last)
			if (!comp (v[parent], v[rchild]))
				return false;

		parent--;
	}

	return true;
}

#endif	// HEAP_FUNCTIONS
