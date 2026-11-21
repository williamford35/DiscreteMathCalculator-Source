/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/draw/DrawGraph.h"
#include "graphs/GraphType.h"
#include "graphs/build/GraphEdgeStyles.h"

#include <ogdf/basic/Queue.h>
#include <ogdf/basic/geometry.h>

#include <string>

#include "RelationsPartialOrders/PartialOrderDialog.h"

using namespace std;
extern bool relationPartialOrderGraphInProcess();

DrawGraph::DrawGraph (GraphAttributes *gattr, Graph *grph,
                      wxSFDiagramManager *mgr, DrawingCanvas *canvas,
                      GraphType gtype, GraphLayout layout) :
	GA (gattr), G (grph), m_drawingCanvas (mgr),
	drawingCanvas (canvas), graphType (gtype),
	graphLayout (layout)
{
	m_drawingCanvas->AcceptShape (wxT ("wxSFRectShape"));
	m_drawingCanvas->AcceptShape (wxT ("wxSFCircleShape"));
	m_drawingCanvas->AcceptShape (wxT ("wxSFLineShape"));
	m_drawingCanvas->AcceptShape (wxT ("wxSFTextShape"));

	canvas->RemoveStyle (wxSFShapeCanvas::sfsMULTI_SELECTION);
	canvas->RemoveStyle (wxSFShapeCanvas::sfsMULTI_SIZE_CHANGE);
	canvas->RemoveStyle (wxSFShapeCanvas::sfsDND);
	canvas->RemoveStyle (wxSFShapeCanvas::sfsHOVERING);
	canvas->RemoveStyle (wxSFShapeCanvas::sfsUNDOREDO);
	canvas->RemoveStyle (wxSFShapeCanvas::sfsHIGHLIGHTING);

	if ( (graphType == digraph) || (graphType == digraphw))
		GA->directed() = true;
	else
		GA->directed() = false;

	vertexCreated.clear();

	drawingCanvas->SetCanvasColour (wxColour (wxT ("WHITE")));
}

DrawGraph::~DrawGraph()
{
	/*int n, i;

	n = c.size();

	if (n != 0)
	{
		for (i=0;i < n;i++)
			delete c[i];
		c.clear();
	}

	n = segment.size();

	if (n != 0)
	{
		for (i=0;i < n;i++)
			delete segment[i];
		segment.clear();
	}*/
}

void DrawGraph::draw (bool hideEdges, bool useStandardVertexColor)
{
	List<node> allGraphNodes;
	List<edge> allGraphEdges;
	ListIterator<node> vertexIter;
	ListIterator<edge> edgeIter;
	node t;
	edge e;
	int n, i;

	// color all vertices WHITE
	G->allNodes (allGraphNodes);
	vertexIter = allGraphNodes.begin();

	while (vertexIter != allGraphNodes.end())
	{
		nodeColor[*vertexIter] = WHITE;
		vertexIter++;
	}

	maxVertexWidth = 0;
	// now begin a breadth-first search of the entire graph
	// and draw it
	vertexIter = allGraphNodes.begin();

	while (vertexIter != allGraphNodes.end())
	{
		t = *vertexIter;

		if (GA->shape (t) == Shape::Ellipse)
		{
			int graphWidth, width, height;
			double x, y;
			wxString labelText;
			wxSFTextShape *graphLabel;
			wxFont labelFont (18, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

			graphWidth = GA->boundingBox().width();
			labelText = wxString (GA->label (t).c_str(),wxConvUTF8);
			drawingCanvas->GetTextExtent (labelText, &width, &height);
			//x = (graphWidth - width)/2;
			x = graphWidth + 10;
			y = height + 10;
			//x = GA->x(t);
			//y = GA->y(t);
			//y = GA->y(t);
			graphLabel = (wxSFTextShape *) (m_drawingCanvas->AddShape (CLASSINFO (wxSFTextShape),wxPoint (x,y)));
			labelText = wxString (GA->label (t).c_str(), wxConvUTF8);
			graphLabel->SetText (labelText);
			graphLabel->SetFont (labelFont);
			vertexIter++;
			continue;
		}

		if (nodeColor[t] == WHITE)
			drawVertices(t,hideEdges,useStandardVertexColor);

		vertexIter++;
	}

	G->allEdges (allGraphEdges);
	edgeIter = allGraphEdges.begin();

	while (edgeIter != allGraphEdges.end())
	{
		e = *edgeIter;

		switch (graphLayout)
		{
		case hierarchical:
			drawEdge (e,hideEdges);
			break;

		case forcedirected:
			drawEdge (e,hideEdges);
			break;

		case orthogonal:
			drawOrthogonalEdge (e, hideEdges);
			break;

		case circular:
			drawEdge (e,hideEdges);
			break;

		case tree:
			drawEdge(e,hideEdges);
		}

		edgeIter++;
	}

	m_drawingCanvas->MoveShapesFromNegatives();
	MakeLeftMargin();
	m_drawingCanvas->UpdateAll();
	drawingCanvas->UpdateVirtualSize();
}

void DrawGraph::drawVertices (node t, bool hideEdges, bool useStandardVertexColor)
{
	Queue<node> q;
#ifdef __APPLE__
	wxFont labelFont (9, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
#else
	wxFont labelFont (9, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
#endif
	node parent = t, child;
	double x, y;
	List<edge> children;
	ListIterator<edge> iter;
	wxString vertexLabel;
	wxPoint p;
	wxSFRectShape* pShape;
	wxSFCircleShape* cShape;
	wxSFShapeBase* pp;
	wxSFTextShape* textShape;
	double nodeWidth, nodeHeight;
	wxColour vertexColor = wxColour (255, 215, 0);
	string nodeColour;
	ogdf::Color colorOfNode;
	int numberOfShapes = 0;
	pair<set<wxString>::iterator,bool> isVertexUsed;

	q.append (parent);

	while (!q.empty())
	{
		parent = q.pop();
		nodeColor[parent] = BLACK;
		vertexLabel = wxString (GA->label (parent).c_str(),wxConvUTF8);
		nodeWidth = GA->width (parent);

		if (nodeWidth > maxVertexWidth)
			maxVertexWidth = nodeWidth;

		nodeHeight = GA->height (parent);
		x = GA->x (parent);
		y = GA->y (parent);
		p = wxPoint (x-nodeWidth/2,y-nodeHeight/2);

		if (hideEdges)
		{
			cShape = (wxSFCircleShape *) m_drawingCanvas->AddShape (CLASSINFO (wxSFCircleShape), p, false);
			pp = cShape;
		}
		else
		{
			pShape = (wxSFRectShape *) m_drawingCanvas->AddShape (CLASSINFO (wxSFRectShape), p, false);
			// pShape->SetStyle(wxSFShapeBase::sfsSHOW_HANDLES);
			pp = pShape;
			numberOfShapes++;
		}

		if (pp)
		{
			colorOfNode = GA->fillColor (parent);
			nodeColour = colorOfNode.toString();

			// do not want a node colored black
			if ((colorOfNode == ogdf::Color::Name::Black))
				colorOfNode = ogdf::Color(255,215,0);

			if (!hideEdges)
				( (wxSFRectShape*) pp)->SetRectSize (nodeWidth, nodeHeight);
			else
				( (wxSFCircleShape*) pp)->SetRectSize (nodeWidth, nodeHeight);

			if (nodeColour == (const char *) (""))
			{
				if (hideEdges)
					( (wxSFCircleShape*) pp)->SetFill (vertexColor);
				else
					( (wxSFRectShape*) pp)->SetFill (vertexColor);
			}
			else
			{
				if (!useStandardVertexColor)
					vertexColor = wxColour (wxString (nodeColour.c_str(), wxConvUTF8));

				if (hideEdges)
					( (wxSFCircleShape*) pp)->SetFill (vertexColor);
				else
					( (wxSFRectShape*) pp)->SetFill (vertexColor);
			}

			textShape = new wxSFTextShape();
			numberOfShapes++;
			vertexLabel = wxString (GA->label (parent).c_str(),wxConvUTF8);
			textShape->SetText (vertexLabel);
			textShape->SetFont (labelFont);

			// set alignment
			textShape->SetVAlign (wxSFShapeBase::valignMIDDLE);
			textShape->SetHAlign (wxSFShapeBase::halignCENTER);
			// textShape->SetStyle(wxSFShapeBase::sfsHOVERING);
			pp->AddChild (textShape);
			vertexShape[parent] = pp;
		}

		children.clear();
		parent->outEdges (children);
		iter = children.begin();

		while (iter != children.end())
		{
			child = (**iter).target();
			vertexLabel = wxString (GA->label (child).c_str(),wxConvUTF8);
			isVertexUsed = vertexCreated.insert (vertexLabel);

			if (isVertexUsed.second == true)
				if (nodeColor[child] == WHITE)
					q.append (child);

			iter++;
		}
	}

	// how many shapes are in the current canvas?
	/*wxSFShapeCanvas * sc =	m_drawingCanvas->GetShapeCanvas ();
	wxRect w = sc->GetTotalBoundingBox ();
	ShapeList llst;

	sc->GetShapesInside (w, llst);
	int mn = llst.size();*/
}

void DrawGraph::drawEdge (edge e, bool hideEdges)
{
	wxFont edgeFont (14, wxFONTFAMILY_DEFAULT,
      wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	wxColour weightColor = wxColour (0,0,0);    // BLACK
	node srcVertex, destVertex;
	long srcId, destId;
	double x, y;
	int edgeBendNumber, numberOfBends;
	wxSFLineShape *connectingEdge;
	wxSFLineShape *connEdge;
	wxSFTextShape* edgeLabelShape;
	wxString edgeLabel, labelText;
	bool drawArrow, drawEdgeWeight;
	wxSFSolidArrow *arrow;
	wxBrush brush/*(wxColour(0,0,0, wxALPHA_TRANSPARENT))*/;
	DPolyline edgeBends;
	DPoint dp;
	wxRealPoint *firstBend = NULL, *secondBend =NULL, startPt, endPt;
	wxPen edgeProperties;
	static wxRealPoint* p;
	wxSFRectShape *theVertex;
	wxColour edgewxColor;
	ogdf::Color edgeOGDFColor;
	ogdf::StrokeType edgeType;
	double strkWidth = GA->strokeWidth(e);

	edgeOGDFColor = GA->strokeColor(e);
	edgewxColor = wxColour(edgeOGDFColor.red(), edgeOGDFColor.green(), edgeOGDFColor.blue());
	switch (graphType)
	{
	case undir:
		drawArrow = drawEdgeWeight = false;
		break;

	case undirw:
		/*if (GA->arrowType (e) == EdgeArrow::Last)
			drawArrow = true;
		else
			drawArrow = false;*/
		drawArrow = false;
		drawEdgeWeight = true;
		break;

	case digraph:
		drawArrow = true;
		drawEdgeWeight = false;
		break;

	case digraphw:
		drawArrow = drawEdgeWeight = true;
		break;
	}

	srcVertex = e->source();
	destVertex = e->target();
	srcId = vertexShape[srcVertex]->GetId();
	destId = vertexShape[destVertex]->GetId();
	edgeBends = GA->bends (e);

	ListIterator<DPoint> bendsIter;

	numberOfBends = edgeBends.size();
	edgeBendNumber = 1;
	bendsIter = edgeBends.begin();

	while (bendsIter != edgeBends.end())
	{
		dp = *bendsIter;
		x = dp.m_x;
		y = dp.m_y;

		if (edgeBendNumber == 1)
			firstBend = new wxRealPoint (x,y);
		else
			secondBend = new wxRealPoint (x,y);

		edgeBendNumber++;
		bendsIter++;
	}

	edgeType = GA->strokeType(e);

	switch (edgeType)
	{
	case ogdf::StrokeType::None:
		edgeProperties = wxPen(edgewxColor, strkWidth, wxPENSTYLE_TRANSPARENT);
		break;

	case ogdf::StrokeType::Solid:
		edgeProperties = wxPen(edgewxColor, strkWidth, wxPENSTYLE_SOLID);
		break;

	case ogdf::StrokeType::Dash:
		edgeProperties = wxPen(edgewxColor, strkWidth, wxPENSTYLE_SHORT_DASH);
		break;

	case ogdf::StrokeType::Dot:
		edgeProperties = wxPen(edgewxColor, strkWidth, wxPENSTYLE_DOT);
		break;

	case ogdf::StrokeType::Dashdot:
		edgeProperties = wxPen(edgewxColor, strkWidth, wxPENSTYLE_DOT_DASH);
		break;
	}

	if (srcId == destId)
	{
		handleLoop(srcVertex, drawArrow, wxString(GA->label(e).c_str(),wxConvUTF8), edgeProperties);

		return;
	}
	else
		connectingEdge = (wxSFLineShape *) m_drawingCanvas->CreateConnection (srcId, destId, false);

	switch (numberOfBends)
	{
	case 0:  // nothing to do
		break;

	case 1:
		connectingEdge->GetControlPoints().Append (firstBend);
		break;

	case 2:
		connectingEdge->GetControlPoints().Append (firstBend);
		connectingEdge->GetControlPoints().Append (secondBend);
		break;
	}

	if (drawArrow)
	{
		connectingEdge->SetTrgArrow (CLASSINFO (wxSFSolidArrow));
		arrow = (wxSFSolidArrow *) connectingEdge->GetTrgArrow();
		brush = wxBrush (edgeProperties.GetColour());
		arrow->SetArrowFill (brush);
		arrow->SetArrowPen (edgeProperties);
	}

	if (drawEdgeWeight)
	{
		double weight, fractpart, intpart;

		edgeLabel = wxString (GA->label (e).c_str(),wxConvUTF8);
		edgeLabel.ToDouble (&weight);
		fractpart = modf (weight, &intpart);

		if (fractpart == 0.0)
		{
			int w;

			w = floor (weight);
			edgeLabel = wxString::Format (wxT ("%d"), w);
		}
		else
		{
			// edgeLabel.Clear();
			edgeLabel = wxString::Format (wxT ("%.2f"), weight);
		}

		edgeLabelShape = new wxSFTextShape();
		edgeLabelShape->SetFont (edgeFont);
		edgeLabelShape->SetTextColour (weightColor);
		startPt = connectingEdge->GetSrcPoint();
		endPt = connectingEdge->GetTrgPoint();

		connectingEdge->AcceptChild (wxT ("wxSFTextShape"));
		connectingEdge->AddChild (edgeLabelShape);

		if (startPt.y == endPt.y)
		{
			//horizontal edge
			edgeLabelShape->SetText (edgeLabel);
			edgeLabelShape->SetVAlign (wxSFShapeBase::valignTOP);
			edgeLabelShape->SetHAlign (wxSFShapeBase::halignCENTER);
		}
		else if (startPt.x == endPt.x)
		{
			// vertical edge
			edgeLabelShape->SetText (edgeLabel + wxT ("  "));
			edgeLabelShape->SetVAlign (wxSFShapeBase::valignMIDDLE);
			edgeLabelShape->SetHAlign (wxSFShapeBase::halignRIGHT);
		}
		else
		{
			//edgeLabelShape->SetText(edgeLabel + wxT("  "));
			edgeLabelShape->SetText (edgeLabel + wxT ("   "));
			edgeLabelShape->SetVAlign (wxSFShapeBase::valignMIDDLE);
			edgeLabelShape->SetHAlign (wxSFShapeBase::halignRIGHT);
		}
	}

	connectingEdge->SetLinePen (edgeProperties);

	if (edgeProperties == wxHIDE_PEN || hideEdges)
		connectingEdge->Show (false);

	connectingEdge->CreateHandles();
}

void DrawGraph::drawOrthogonalEdge(edge e, bool hideEdges)
{
	double x, y;
	int edgeBendNumber;
	int i, n;
	wxString edgeLabel;
	bool drawArrow, drawEdgeWeight, drawEndingWeight;
	DPolyline edgeBends;
	DPoint dp;
	vector<wxRealPoint> segmentPoints;

	switch (graphType)
	{
	case undir:
		drawArrow = drawEdgeWeight = false;
		break;

	case undirw:
		drawArrow = false;
		drawEdgeWeight = true;
		break;

	case digraph:
		drawArrow = true;
		drawEdgeWeight = false;
		break;

	case digraphw:
		drawArrow = drawEdgeWeight = true;
		break;
	}

	if (drawEdgeWeight)
		edgeLabel = wxString(GA->label(e).c_str(),wxConvUTF8);

	edgeBends = GA->bends(e);

	ListIterator<DPoint> bendsIter;

	edgeBendNumber = 0;
	bendsIter = edgeBends.begin();

	while (bendsIter != edgeBends.end())
	{
		dp = *bendsIter;
		x = dp.m_x;
		y = dp.m_y;
		segmentPoints.push_back(wxRealPoint(x,y));
		edgeBendNumber++;
		bendsIter++;
	}

	n = segmentPoints.size();

	if (n == 0)
		return;  // self-loops are not allowed

	for (i=1; i < n-1; i++)
	{
		if (hideEdges == false)
		{
			if (drawEdgeWeight)
			{
				if (labelSegment(n,i))
					drawLineSegment(segmentPoints[i-1], segmentPoints[i], false, edgeLabel);
				else
					drawLineSegment(segmentPoints[i-1], segmentPoints[i], false);
			}
			else
				drawLineSegment(segmentPoints[i-1], segmentPoints[i], false);
		}
	}

	drawEndingWeight = (n == 2);

	if (hideEdges == false)
	{
		if (drawEndingWeight && drawArrow)
			drawLineSegment(segmentPoints[n-2], segmentPoints[n-1], true, edgeLabel);
		else if (drawEndingWeight && !drawArrow)
			drawLineSegment(segmentPoints[n-2], segmentPoints[n-1], false, edgeLabel);
		else if (!drawEndingWeight && drawArrow)
			drawLineSegment(segmentPoints[n-2], segmentPoints[n-1], true);
		else
			drawLineSegment(segmentPoints[n-2], segmentPoints[n-1], false);
	}
}

bool DrawGraph::labelSegment(int n, int i)
{
	if (n == 2 && i == 0)
		return true;
	else if (n == 3)
	{
		if (i == 1)
			return true;
		else
			return false;
	}
	else if (n == 4)
	{
		if (i == 2)
			return true;
		else
			return false;
	}
	else if (n == 5)
	{
		if (i == 3)
			return true;
		else
			return false;
	}
	else if (n == 6)
	{
		if (i == 4)
			return true;
		else
			return false;
	}
	else
	{
		if (i == 5)
			return true;
		else
			return false;
	}

	return true;
}

void DrawGraph::drawLineSegment(const wxRealPoint& start, const wxRealPoint& end,
                                bool drawArrow,
                                const wxString& weight, const wxPen& pen)
{
	wxSFShapeBase *pShape;
	wxSFSolidArrow *arrow;
	wxSFTextShape *edgeLabelShape;
	wxString edgeLabel = weight;
	wxBrush brush;
	wxFont edgeFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	wxColour weightColor = wxColour(*wxRED);
	bool lineVertical;

	if (start.y != end.y)
		lineVertical = true;
	else
		lineVertical = false;

	pShape = m_drawingCanvas->AddShape(CLASSINFO(wxSFLineShape), wxPoint((int)(start.x), (int)(start.y)), sfDONT_SAVE_STATE);

	if( pShape )
	{
		if (drawArrow)
		{
			((wxSFLineShape*) pShape)->SetTrgArrow(CLASSINFO(wxSFSolidArrow));
			arrow = (wxSFSolidArrow *)(((wxSFLineShape *)pShape)->GetTrgArrow());
			brush = wxBrush(pen.GetColour());
			arrow->SetArrowFill(brush);
		}

		if (weight.length() != 0)
		{
			((wxSFLineShape*)pShape)->AcceptChild(wxT("wxSFTextShape"));
			edgeLabelShape = new wxSFTextShape();
			edgeLabelShape->SetFont(edgeFont);
			edgeLabelShape->SetTextColour(weightColor);
			((wxSFLineShape*) pShape)->AddChild(edgeLabelShape);

			if (lineVertical)
			{
				edgeLabelShape->SetVAlign(wxSFShapeBase::valignMIDDLE);
				edgeLabelShape->SetHAlign(wxSFShapeBase::halignRIGHT);
				edgeLabelShape->SetText(edgeLabel + wxT("  "));
			}
			else
			{
				edgeLabelShape->SetVAlign(wxSFShapeBase::valignBOTTOM);
				edgeLabelShape->SetHAlign(wxSFShapeBase::halignCENTER);
				edgeLabelShape->SetText(edgeLabel);
			}
		}

		((wxSFLineShape*)pShape)->SetLinePen(pen);
	}

	// set the line to be stand-alone
	((wxSFLineShape*) pShape)->SetStandAlone(true);

	((wxSFLineShape*) pShape)->SetSrcPoint(start);
	((wxSFLineShape*) pShape)->SetTrgPoint(end);

	pShape->AcceptChild(wxT("wxSFTextShape"));
	pShape->Refresh();
}

GraphAttributes *DrawGraph::getGA()
{
	return GA;
}

int DrawGraph::getMaxVertexWidth()
{
	return maxVertexWidth;
}

void DrawGraph::setTypeAndLayout(GraphType type, GraphLayout layout)
{
	graphType = type;
	graphLayout = layout;

	List<node> allGraphNodes;
	List<edge> allGraphEdges;
	ListIterator<node> vertexIter;
	ListIterator<edge> edgeIter;
	node t;

	vertexIter = allGraphNodes.begin();

	while (vertexIter != allGraphNodes.end())
	{
		t = *vertexIter;

		if (GA->shape(t) == Shape::Ellipse)
		{
			int graphWidth, width, height;
			double x, y;
			wxString labelText;
			wxSFTextShape *graphLabel;
			wxFont labelFont(16, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

			graphWidth = GA->boundingBox().width();
			labelText = wxString(GA->label(t).c_str(),wxConvUTF8);
			drawingCanvas->GetTextExtent(labelText, &width, &height);
			//x = (graphWidth - width)/2;
			x = graphWidth + 10;
			y = height + 10;
			//x = GA->x(t);
			//y = GA->y(t);
			//y = GA->y(t);
			graphLabel = (wxSFTextShape *)(m_drawingCanvas->AddShape(CLASSINFO(wxSFTextShape),wxPoint(x,y)));
			labelText = wxString(GA->label(t).c_str(), wxConvUTF8);
			graphLabel->SetText(labelText);
			graphLabel->SetFont(labelFont);
			vertexIter++;

			return;
		}
	}
}

void DrawGraph::MakeLeftMargin()
{
	wxSFShapeBase *pShape;
	ShapeList shapes;
	m_drawingCanvas->GetShapes(CLASSINFO(wxSFShapeBase), shapes);

	ShapeList::compatibility_iterator node = shapes.GetFirst();

	shapes.clear();
	m_drawingCanvas->GetShapes(CLASSINFO(wxSFShapeBase), shapes);

	// move all parent shapes so they (and their children) will be located in the positive values only
	node = shapes.GetFirst();

	while(node)
	{
		pShape = node->GetData();

		if(pShape->GetParentShape() == NULL)
		{
			pShape->MoveBy(50, 0);
			pShape->MoveBy(0, 50);
		}

		node = node->GetNext();
	}
}

void DrawGraph::handleLoop (node srcVertex, bool drawArrow,
                            const wxString& weight, const wxPen& pen)
{
	double x, y, w, h;
	wxRealPoint p1, p2, p3, p4, p5;
	wxSFRectShape *vertex = (wxSFRectShape *) vertexShape[srcVertex];
	wxSFCircleShape *c1, *c2, *c3;
	wxSFLineShape *segment1, *segment2, *segment3, *segment4;
	wxXS::RealPointList path;
	wxRealPoint vertexPosition;
	wxSFSolidArrow *arrow;
	wxSFTextShape *edgeLabelShape;
	wxString edgeLabel = weight;
	wxBrush brush;
	wxFont edgeFont (14, wxFONTFAMILY_DEFAULT,
       wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);	wxColour weightColor = wxColour (*wxBLACK);

	vertexPosition = vertex->GetAbsolutePosition();
	x = vertexPosition.x;
	y = vertexPosition.y;
	w = GA->width (srcVertex);
	h = GA->height (srcVertex);

	p1 = wxRealPoint (x+w/2, y);
	p2 = wxRealPoint (x+w/2, y-12);
	p3 = wxRealPoint (x-12, y-12);
	p4 = wxRealPoint (x-12, y+h/2);
	p5 = wxRealPoint (x, y+h/2);

	// vertex->SetStyle(wxSFShapeBase::sfsLOCK_CHILDREN);
	vertex->AddConnectionPoint(wxRealPoint(w/2*100, 0));
	vertex->AddConnectionPoint(wxRealPoint(0,w/2*100));

	c1 = (wxSFCircleShape *)m_drawingCanvas->AddShape (CLASSINFO(wxSFCircleShape), p2);
	c1->Scale(.25,.25);
	c1->MoveTo(p2);
	c1->AddConnectionPoint (wxSFConnectionPoint::cpTOPLEFT);
	segment1 = (wxSFLineShape *)m_drawingCanvas->CreateConnection(vertex->GetId(), c1->GetId());
	segment1->SetLinePen(pen);

	c2 = (wxSFCircleShape *) m_drawingCanvas->AddShape (CLASSINFO(wxSFCircleShape), p3);
	c2->Scale(0.25, 0.25);
	c2->MoveTo(p3);
	c2->AddConnectionPoint (wxSFConnectionPoint::cpTOPLEFT);
	segment2 = (wxSFLineShape *)m_drawingCanvas->CreateConnection(c1->GetId(), c2->GetId());
	segment2->SetLinePen(pen);

	c3 = (wxSFCircleShape *) m_drawingCanvas->AddShape (CLASSINFO(wxSFCircleShape), p4);
	c3->Scale(0.25, 0.25);
	c3->MoveTo(p4);
	c3->AddConnectionPoint (wxSFConnectionPoint::cpTOPLEFT);
	segment3 = (wxSFLineShape *)m_drawingCanvas->CreateConnection(c2->GetId(), c3->GetId());
	segment3->SetLinePen(pen);

	segment4 = (wxSFLineShape *)m_drawingCanvas->CreateConnection(c3->GetId(), vertex->GetId());
	segment4->SetLinePen(pen);

	if (drawArrow)
	{
		segment4->SetTrgArrow (CLASSINFO (wxSFSolidArrow));
		arrow = (wxSFSolidArrow *) segment4->GetTrgArrow();
		brush = wxBrush (pen.GetColour());
		arrow->SetArrowFill (brush);
		arrow->SetArrowPen (pen);
	}

	if (weight.Length() != 0)
	{
		segment3->AcceptChild (wxT ("wxSFTextShape"));
		edgeLabelShape = new wxSFTextShape();
		edgeLabelShape->SetFont (edgeFont);
		edgeLabelShape->SetTextColour (weightColor);
		edgeLabelShape->SetText (edgeLabel);
		segment3->AddChild (edgeLabelShape);
	}

	c1->Show(false);
	c2->Show(false);
	c3->Show(false);

	vertex->RemoveConnectionPoint(wxSFConnectionPoint::cpCUSTOM);
	vertex->RemoveConnectionPoint(wxSFConnectionPoint::cpCUSTOM);

	m_drawingCanvas->UpdateAll();
}

void DrawGraph::MoveShapesFromNegatives()
{
	wxSFShapeBase *pShape;
	wxRealPoint shapePos;
	double minx = 0, miny = 0;

	// find the maximal negative position value
	ShapeList shapes;
	m_drawingCanvas->GetShapes(CLASSINFO(wxSFShapeBase), shapes);

	ShapeList::compatibility_iterator node = shapes.GetFirst();

	while(node)
	{
		shapePos = node->GetData()->GetAbsolutePosition();

		if(node == shapes.GetFirst())
		{
			minx = shapePos.x;
			miny = shapePos.y;
		}
		else
		{
			if(shapePos.x < minx)minx = shapePos.x;

			if(shapePos.y < miny)miny = shapePos.y;
		}

		node = node->GetNext();
	}

	shapes.clear();
	m_drawingCanvas->GetShapes(CLASSINFO(wxSFShapeBase), shapes);

	// move all parent shapes so they (and their children) will be located in the positive values only
	if((minx < 0) || (miny < 0))
	{
		node = shapes.GetFirst();

		while(node)
		{
			pShape = node->GetData();

			if(pShape->GetParentShape() == NULL)
			{
				if(minx < 0)pShape->MoveBy(abs((int)minx), 0);

				if(miny < 0)pShape->MoveBy(0, abs((int)miny));
			}

			node = node->GetNext();
		}
	}
}
