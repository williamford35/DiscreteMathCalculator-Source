#include "wx/string.h"

#include <lemon/concepts/digraph.h>
#include <lemon/maps.h>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>
#include <lemon/bfs.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include <vector>
#include <set>
#include <map>

using namespace std;
using namespace ogdf;

class ClosureDisplay
{
public:

	enum ClosureType
	{
		REFLEXIVE,
		SYMMETRIC,
		TRANSITIVE
	};

	ClosureDisplay (ogdf::GraphAttributes *ga, ogdf::Graph *grph, ClosureType ct);
	void drawRequestedClosure();

private:
	void buildRelationDigraphRepresentation();
	void DoReflexiveClosure();
	void DoSymmetricClosure();
	void DoTransitiveClosure();

	lemon::ListDigraph dig;
	ogdf::GraphAttributes *GA;
	ogdf::Graph *G;
	ClosureType closureType;
	lemon::ListDigraph::NodeMap<wxString> digraphNodeMap;
	map<wxString, lemon::ListDigraph::Node> wxString2DiNode;
	lemon::ListDigraph::ArcMap<int> arcMap;
	map<lemon::ListDigraph::Arc, bool> arcNew;
};
