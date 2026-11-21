#ifndef DRAWTREE_H_INCLUDED
#define DRAWTREE_H_INCLUDED

#include <wx/wxsf/wxShapeFramework.h>
#include "common/DrawingCanvas.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>
#include <map>

using namespace std;
using namespace ogdf;

struct ShapeData
{
	long id;
	wxSFShapeBase *wxBase;

	ShapeData()
	{}

	ShapeData (long ident, wxSFShapeBase *b) :
		id (ident), wxBase (b)
	{}
};

class DrawTree: public wxEvtHandler
{
public:

	DrawTree();
	virtual ~DrawTree();
	void setCanvasProperties (wxWindow *parent, DrawingCanvas * & canvas, wxSFDiagramManager * & m_canvas);
	void setGraphProperties (ogdf::Graph *g, GraphAttributes *ga, node r);
	void draw(bool rbtree = false);

private:

	static const long ID_GENERAL_TREE_DRAWING_CANVAS;

	DrawingCanvas *drawingCanvas;
	wxSFDiagramManager m_drawingCanvas;
	ogdf::Graph *G;
	ogdf::GraphAttributes *GA;
	node root;
	wxSize drawingAreaSize;
	map <node, wxSFShapeBase*> circleShapeData;

	void bfsDrawNodes (node t);
	void bfsDrawEdges (node t);
	void drawEdgeFromParent (node t);
	void bfsLabelNodes (node t);
	void MoveShapesFromNegatives();
	void MakeLeftMargin();
};

#endif // DRAWTREE_H_INCLUDED
