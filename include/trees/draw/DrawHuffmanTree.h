#ifndef DRAWHUFFMANTREE_H_INCLUDED
#define DRAWHUFFMANTREE_H_INCLUDED

#include "wx/wxsf/wxShapeFramework.h"
#include "common/DrawingCanvas.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>
#include <map>

using namespace std;
using namespace ogdf;

class DrawHuffmanTree: public wxEvtHandler
{
public:

	DrawHuffmanTree();
	virtual ~DrawHuffmanTree();
	void setCanvasProperties (wxPanel *parent, DrawingCanvas * & canvas, wxSFDiagramManager * & m_canvas);
	void setGraphProperties (ogdf::Graph *g, GraphAttributes *ga, node r);
	void draw();

private:

	static const long ID_HUFFMAN_TREE_DRAWING_CANVAS;

	DrawingCanvas *drawingCanvas;
	wxSFDiagramManager m_drawingCanvas;
	ogdf::Graph *G;
	GraphAttributes *GA;
	node root;
	wxSize drawingAreaSize;
	map <node, wxSFShapeBase*> rectShapeData;

	void bfsDrawNodes (node t);
	void bfsDrawEdges (node t);
	void drawEdgeFromParent (node t);
	void bfsLabelNodes (node t);
	void MoveShapesFromNegatives();
	void MakeLeftMargin();
};

#endif // DRAWHUFFMANTREE_H_INCLUDED
