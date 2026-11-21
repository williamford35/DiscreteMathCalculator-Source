/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/draw/DrawTree.h"
#include <ogdf/basic/Queue.h>
#include <ogdf/basic/geometry.h>
#include <stdio.h>
#include <string.h>

#include <string>
using namespace std;

#include <ogdf/fileformats/GraphIO.h>

const long DrawTree::ID_GENERAL_TREE_DRAWING_CANVAS = wxNewId();

DrawTree::DrawTree()
{
}

DrawTree::~DrawTree()
{}

void DrawTree::setCanvasProperties (wxWindow *parent, DrawingCanvas * & canvas, wxSFDiagramManager * & m_canvas)
{
	m_drawingCanvas.AcceptShape (wxT ("wxSFCircleShape"));
	m_drawingCanvas.AcceptShape (wxT ("wxSFLineShape"));
	m_drawingCanvas.AcceptShape (wxT ("wxSFTextShape"));
	drawingCanvas = new DrawingCanvas (&m_drawingCanvas, parent,
	                                   ID_GENERAL_TREE_DRAWING_CANVAS, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL);
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

void DrawTree::setGraphProperties (Graph *g, GraphAttributes *ga, node r)
{
	G = g;
	GA = ga;
	root = r;
}

void DrawTree::draw(bool rbtree)
{
	node t = root, labelNode = NULL;
	double x, y;
	wxSFTextShape *treeLabel;
	wxPoint p;
	wxString labelText;
	wxFont labelFont (24, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

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
		if (GA->shape (*iter) == Shape::Rect)
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

	if (rbtree)
		for(node v : G->nodes)
		{
			size_t pos;

			if ((pos = GA->label(v).find_first_of('@')) != std::string::npos)
			{
				if (GA->label(v)[pos+1] == 'R')
					GA->fillColor(v) = ogdf::Color::Name::Red;
				else
					GA->fillColor(v) = ogdf::Color::Name::Lightgray;

				GA->label(v) = (GA->label(v)).erase(pos, 2);
			}
		}
}

wxPoint getPixelCoordinates (double x, double y, int virtualScreenWidth)
{
	return wxPoint (virtualScreenWidth/2 + x, 10 + y);
}

void DrawTree::bfsDrawNodes (node t)
{
	Queue<node> q;
	node parent = t, child;
	List<edge> children;
	ListIterator<edge> iter;
	double x, y, currNodeWidth;
	int screenWidth, graphWidth, virtualScreenWidth;
	wxPoint p;
	wxSFShapeBase* pShape;
	wxSFTextShape*textShape;
	wxString nodeLabel;
	wxString atchar;
	wxFont labelFont (9, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

	if (t == NULL)
		return;

	circleShapeData.clear();
	drawingAreaSize = drawingCanvas->GetClientSize();
	screenWidth = drawingAreaSize.GetWidth();
	graphWidth = (GA->boundingBox()).width();

	if (screenWidth < graphWidth)
		//virtualScreenWidth = graphWidth + GA->width(t);
		virtualScreenWidth = graphWidth;
	else
		//virtualScreenWidth = screenWidth + GA->width(t);
		virtualScreenWidth = screenWidth;

	q.append (parent);

	while (!q.empty())
	{
		parent = q.pop();

		currNodeWidth = GA->width (parent);

		if (currNodeWidth == 0.0)
			continue;

		x = GA->x (parent);
		y = GA->y (parent);
		//p = getPixelCoordinates(x, y, virtualScreenWidth);
		p = wxPoint (x,y);

		pShape = m_drawingCanvas.AddShape (CLASSINFO (wxSFCircleShape), p);
		wxString s = wxString (GA->label (parent).c_str(),wxConvUTF8);

		if (pShape)
		{
			//((wxSFCircleShape*)pShape)->SetRectSize(nodeWidth, nodeHeight);
			textShape = new wxSFTextShape();
			nodeLabel = wxString (GA->label (parent).c_str(),wxConvUTF8);
			atchar = nodeLabel.AfterFirst ( (wchar_t) '@');

			if (atchar.Length() != 0)
			{
				if (atchar.IsSameAs ( (wchar_t) 'R'))
				{
					( (wxSFCircleShape*) pShape)->SetFill (*wxRED);
				}

				else
				{
					( (wxSFCircleShape*) pShape)->SetFill (*wxLIGHT_GREY);
				}

				nodeLabel = nodeLabel.BeforeLast('@');
			}
			else
				( (wxSFCircleShape*) pShape)->SetFill (wxColour (255, 215, 0));

			textShape->SetText (nodeLabel);
			textShape->SetFont (labelFont);

			// set alignment
			textShape->SetVAlign (wxSFShapeBase::valignMIDDLE);
			textShape->SetHAlign (wxSFShapeBase::halignCENTER);
			// set required shape style(s)
			// textShape->SetStyle(wxSFShapeBase::sfsALWAYS_INSIDE |  wxSFShapeBase::sfsHOVERING);
			textShape->SetStyle (wxSFShapeBase::sfsALWAYS_INSIDE);
			// textShape->SetStyle(wxSFShapeBase::sfsHOVERING);
			pShape->AddChild (textShape);
			circleShapeData[parent] = pShape;
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

void DrawTree::bfsDrawEdges (node t)
{
	Queue<node> q;
	node parent = t, child;
	List<edge> children;
	ListIterator<edge> iter;
	wxSFShapeBase *treeEdge;
	long parentId, childId;
	double currNodeWidth;
	wxRect boundingRect;

	q.append (parent);

	while (!q.empty())
	{
		parent = q.pop();
		currNodeWidth = GA->width (parent);

		if (currNodeWidth == 0.0)
			continue;

		parentId = circleShapeData[parent]->GetId();

		parent->outEdges (children);
		iter = children.begin();

		while (iter != children.end())
		{
			child = (**iter).target();
			currNodeWidth = GA->width (child);

			if (currNodeWidth != 0.0)
			{
				childId = circleShapeData[child]->GetId();
				treeEdge = m_drawingCanvas.CreateConnection (parentId, childId, true);
				q.append (child);
			}

			iter++;
		}
	}

	//boundingRect = drawingCanvas->GetTotalBoundingBox();
	MoveShapesFromNegatives();
	MakeLeftMargin();
	m_drawingCanvas.UpdateAll();
	//drawingCanvas->MoveShapesFromNegatives();
	//drawingCanvas->SetScaleToViewAll();
	drawingCanvas->UpdateVirtualSize();
}

void DrawTree::MoveShapesFromNegatives()
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

void DrawTree::MakeLeftMargin()
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
