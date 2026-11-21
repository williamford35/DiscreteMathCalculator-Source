#ifndef DRAWGRAPH_H_INCLUDED
#define DRAWGRAPH_H_INCLUDED

#include "wx/wxsf/wxShapeFramework.h"
#include <wx/string.h>
#include <wx/textctrl.h>
#include "common/DrawingCanvas.h"
#include "graphs/GraphType.h"
#include "RelationsPartialOrders/PartialOrderDialog.h"
#include <vector>
#include <map>
#include <set>

using namespace std;

class DrawGraph
{
public:
	DrawGraph (GraphAttributes *gattr, ogdf::Graph *grph, wxSFDiagramManager *mgr,
	           DrawingCanvas *canvas, GraphType gtype, GraphLayout layout);
	~DrawGraph();
	void draw (bool hideEdges = false, bool useStandardVertexColor = false);
	GraphAttributes *getGA();
	int getMaxVertexWidth();
	void setTypeAndLayout (GraphType type, GraphLayout layout);

private:
	enum Color
	{
		WHITE,
		BLACK
	};

	void drawVertices (node t, bool hideEdges = false, bool useStandardColorVertices = false);
	void drawEdge (edge e, bool hideEdges = false);
	void drawOrthogonalEdge (edge e, bool hideEdges = false);
	void drawLineSegment(const wxRealPoint& start, const wxRealPoint& end,
	                     bool drawArrow,
	                     const wxString& weight = wxT (""),
	                     const wxPen& pen = *wxBLACK_PEN);
	bool labelSegment(int n, int i);
	void handleLoop(node srcVertex, bool drawArrow,
	                const wxString& weight, const wxPen& pen);
	void MakeLeftMargin();
	void MoveShapesFromNegatives();


	GraphAttributes *GA;
	Graph *G;
	wxSFDiagramManager *m_drawingCanvas;
	DrawingCanvas *drawingCanvas;
	GraphType graphType;
	GraphLayout graphLayout;
	wxSize drawingAreaSize;
	map<node, Color> nodeColor;
	map<node, wxSFShapeBase *> vertexShape;
	int maxVertexWidth;
	map<wxSFTextShape *, wxRect> labels;
	std::set<wxString> vertexCreated;
};

#endif // DRAWGRAPH_H_INCLUDED
