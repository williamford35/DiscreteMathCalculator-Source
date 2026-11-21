/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/draw/DrawHuffmanTree.h"
#include <ogdf/basic/Queue.h>
#include <ogdf/basic/geometry.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wx/font.h>

#include <string>
using namespace std;

const long DrawHuffmanTree::ID_HUFFMAN_TREE_DRAWING_CANVAS = wxNewId();

DrawHuffmanTree::DrawHuffmanTree()
{
}

DrawHuffmanTree::~DrawHuffmanTree()
{}

void DrawHuffmanTree::setCanvasProperties (wxPanel *parent, DrawingCanvas * & canvas, wxSFDiagramManager * & m_canvas)
{
	m_drawingCanvas.AcceptShape (wxT ("wxSFRectShape"));
	m_drawingCanvas.AcceptShape (wxT ("wxSFLineShape"));
	m_drawingCanvas.AcceptShape (wxT ("wxSFTextShape"));
	drawingCanvas = new DrawingCanvas (&m_drawingCanvas, parent,
	                                   ID_HUFFMAN_TREE_DRAWING_CANVAS, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL);
	drawingCanvas->RemoveStyle (wxSFShapeCanvas::sfsMULTI_SELECTION);
	drawingCanvas->RemoveStyle (wxSFShapeCanvas::sfsMULTI_SIZE_CHANGE);
	drawingCanvas->RemoveStyle (wxSFShapeCanvas::sfsDND);
	drawingCanvas->RemoveStyle (wxSFShapeCanvas::sfsHOVERING);
	drawingCanvas->RemoveStyle (wxSFShapeCanvas::sfsUNDOREDO);
	drawingCanvas->RemoveStyle (wxSFShapeCanvas::sfsHIGHLIGHTING);

	drawingCanvas->SetCanvasColour (wxColour (wxT ("WHITE")));

	canvas = drawingCanvas;
	m_canvas = &m_drawingCanvas;
}

void DrawHuffmanTree::setGraphProperties (Graph *g, GraphAttributes *ga, node r)
{
	G = g;
	GA = ga;
	root = r;
}

void DrawHuffmanTree::draw()
{
	node t = root, labelNode = NULL;
	wxFont labelFont (24, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	double x, y;
	wxSFTextShape *treeLabel;
	wxPoint p;
	wxString labelText;

	if (G->numberOfNodes() == 0)
		return;

	bfsDrawNodes (t);
	bfsDrawEdges (t);

	List<node> allTreeNodes;
	ListIterator<node> iter;

	G->allNodes (allTreeNodes);

	iter = allTreeNodes.begin();

	while (iter != allTreeNodes.end())
	{
		if (GA->shape (*iter) == ogdf::Shape::Ellipse)
		{
			labelNode = *iter;
			break;
		}

		iter++;
	}

	if (iter != allTreeNodes.end())
	{
		double treeWidth;
		int width, height;

		treeWidth = GA->boundingBox().width();
		labelText = wxString (GA->label (labelNode).c_str(),wxConvUTF8);
		drawingCanvas->GetTextExtent (labelText, &width, &height);
		x = (treeWidth - width) /2;
		y = GA->y (labelNode);
		p = wxPoint (x,y);
		treeLabel = (wxSFTextShape *) m_drawingCanvas.AddShape (CLASSINFO (wxSFTextShape), p);
		treeLabel->SetText (labelText);
		treeLabel->SetFont (labelFont);
	}
}

void DrawHuffmanTree::bfsDrawNodes (node t)
{
	Queue<node> q;
	node parent = t, child;
	List<edge> children;
	ListIterator<edge> iter;
	double x, y, nodeWidth, nodeHeight, rootX;
	wxPoint p;
	wxSFShapeBase* pShape;
	wxSFTextShape*textShape;
	wxString nodeLabel;
	wxFont labelFont = wxFont (9, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

	if (t == NULL)
		return;

	nodeWidth = GA->width (t);
	nodeHeight = GA->height (t);
	rectShapeData.clear();
	rootX = GA->x (t);
	q.append (parent);

	while (!q.empty())
	{
		parent = q.pop();

		x = GA->x (parent);
		y = GA->y (parent);
		p = wxPoint (x,y);

		pShape = m_drawingCanvas.AddShape (CLASSINFO (wxSFRectShape), p);

		if (pShape)
		{
			( (wxSFCircleShape*) pShape)->SetRectSize (nodeWidth, nodeHeight);
			( (wxSFRectShape*) pShape)->SetFill (wxColour (255, 215, 0));
			textShape = new wxSFTextShape();
			nodeLabel = wxString (GA->label (parent).c_str(),wxConvUTF8);
			textShape->SetText (nodeLabel);
			textShape->SetFont (labelFont);

			// set alignment
			textShape->SetVAlign (wxSFShapeBase::valignMIDDLE);
			textShape->SetHAlign (wxSFShapeBase::halignCENTER);
			// textShape->SetStyle(wxSFShapeBase::sfsHOVERING);
			pShape->AddChild (textShape);
			rectShapeData[parent] = pShape;
		}

		parent->outEdges (children);
		iter = children.begin();

		while (iter != children.end())
		{
			child = (**iter).target();
			q.append (child);
			iter++;
		}
	}
}

void DrawHuffmanTree::bfsDrawEdges (node t)
{
	Queue<node> q;
	node parent = t, child;
	List<edge> children;
	ListIterator<edge> iter;
	wxSFShapeBase *treeEdge;
	long parentId, childId;
	wxString edgeLabel;
	wxSFTextShape *edgeLabelShape;
	wxFont labelFont = wxFont (9, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

	q.append (parent);

	while (!q.empty())
	{
		parent = q.pop();

		parentId = rectShapeData[parent]->GetId();

		parent->outEdges (children);
		iter = children.begin();

		while (iter != children.end())
		{
			edgeLabel = wxString (GA->label (*iter).c_str(),wxConvUTF8);
			edgeLabel = wxT ("   ") + edgeLabel;
			edgeLabelShape = new wxSFTextShape();
			edgeLabelShape->SetText (edgeLabel);
			edgeLabelShape->SetFont (labelFont);
			edgeLabelShape->SetVAlign (wxSFShapeBase::valignMIDDLE);
			edgeLabelShape->SetHAlign (wxSFShapeBase::halignLEFT);
			// edgeLabelShape->SetStyle(wxSFShapeBase::sfsHOVERING);

			child = (**iter).target();
			childId = rectShapeData[child]->GetId();
			treeEdge = m_drawingCanvas.CreateConnection (parentId, childId, true);
			treeEdge->AddChild (edgeLabelShape);
			q.append (child);
			iter++;
		}
	}

	MoveShapesFromNegatives();
	MakeLeftMargin();
	m_drawingCanvas.UpdateAll();
	drawingCanvas->UpdateVirtualSize();
}

void DrawHuffmanTree::MoveShapesFromNegatives()
{
	wxSFShapeBase *pShape;
	wxRealPoint shapePos;
	double minx = 0, miny = 0;

	// find the maximal negative position value
	ShapeList shapes;
	m_drawingCanvas.GetShapes (CLASSINFO (wxSFShapeBase), shapes);

	ShapeList::compatibility_iterator node = shapes.GetFirst();

	while (node)
	{
		shapePos = node->GetData()->GetAbsolutePosition();

		if (node == shapes.GetFirst())
		{
			minx = shapePos.x;
			miny = shapePos.y;
		}
		else
		{
			if (shapePos.x < minx) minx = shapePos.x;

			if (shapePos.y < miny) miny = shapePos.y;
		}

		node = node->GetNext();
	}

	shapes.clear();
	m_drawingCanvas.GetShapes (CLASSINFO (wxSFShapeBase), shapes);

	// move all parents shape so they (and their children) will be located in the positive values only
	if ( (minx < 0) || (miny < 0))
	{
		node = shapes.GetFirst();

		while (node)
		{
			pShape = node->GetData();

			if (pShape->GetParentShape() == NULL)
			{
				if (minx < 0) pShape->MoveBy (abs ( (int) minx), 0);

				if (miny < 0) pShape->MoveBy (0, abs ( (int) miny));
			}

			node = node->GetNext();
		}
	}
}

void DrawHuffmanTree::MakeLeftMargin()
{
	wxSFShapeBase *pShape;
	ShapeList shapes;
	m_drawingCanvas.GetShapes (CLASSINFO (wxSFShapeBase), shapes);

	ShapeList::compatibility_iterator node = shapes.GetFirst();

	shapes.clear();
	m_drawingCanvas.GetShapes (CLASSINFO (wxSFShapeBase), shapes);

	// move all parent shapes so they (and their children) will be located in the positive values only
	node = shapes.GetFirst();

	while (node)
	{
		pShape = node->GetData();

		if (pShape->GetParentShape() == NULL)
		{
			pShape->MoveBy (50, 0);
			pShape->MoveBy (0, 50);
		}

		node = node->GetNext();
	}
}
