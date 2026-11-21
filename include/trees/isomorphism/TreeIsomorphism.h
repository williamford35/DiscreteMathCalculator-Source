#include <vector>
#include <map>
#include <stack>

#include <wx/window.h>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/basic/Queue.h>
#include <ogdf/tree/TreeLayout.h>

#include "trees/general/GeneralTree.h"
#include "trees/btrFileFormatException.h"
#include <wx/tokenzr.h>
#include <wx/textfile.h>

// #include "graphs/algorithms/ShowSequence.h"

using namespace ogdf;
using namespace std;

class TreeIsomorphism
{
private:
	Graph *tree1, *tree2;
	GraphAttributes *GAtree1, *GAtree2;
	node root1, root2;
	map<node,string> tree1CanonicalForm;
	map<node,string> tree2CanonicalForm;
	vector<string> childName;

public:
	TreeIsomorphism(Graph *t1, GraphAttributes *GAt1, node r1,
	                Graph *t2, GraphAttributes *GAt2, node r2);
	bool AreIsomorphic();

private:
	void AssignCanonicalNames(node v, int treeNo);
	void insertionSort(vector<string>&);
	vector<vector<node> > captureLevel(Graph *G, GraphAttributes *GA,
	                                   node root);
};
