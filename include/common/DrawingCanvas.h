#ifndef DRAWINGCANVAS_H_INCLUDED
#define DRAWINGCANVAS_H_INCLUDED

#include <wx/wxsf/wxShapeFramework.h>
#include <wx/event.h>
#include <wx/menu.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include "common/DrawingCanvas.h"
#include "graphs/GraphType.h"

#include <string>
using namespace std;

using namespace ogdf;

class DrawingCanvas: public wxSFShapeCanvas
{
public:

	enum GraphicOutputType
	{
		BMP,
		JPG,
		PNG,
		GIF,
		TIFF
	};

	enum GMLTypeLabel
	{
		AVL,
		RedBlack,
		None
	};

	DrawingCanvas (wxSFDiagramManager *m_canvas, wxWindow *parent,
	               wxWindowID id=-1,const wxPoint &pos=wxDefaultPosition,
	               const wxSize &size=wxDefaultSize, long style=wxHSCROLL|wxVSCROLL);
	virtual ~DrawingCanvas();
	virtual void OnLeftDown (wxMouseEvent &event);
	virtual void OnRightDown (wxMouseEvent &event);
	void setSliderAddress (wxSlider *canvasSldr);
	void setGA (GraphAttributes *gattr);
	void setG (Graph *g);
	void copyToClipboard();
	void SaveCanvasToGraphicFormat (GraphicOutputType type, const wxString& file);
	void handleSaveAsGML (const wxString& filename, GraphType graphType = notagraph);
	void getDirty (bool *d);
	void setGMLTreeTypeLabel (GMLTypeLabel t);
	wxSFDiagramManager* getSFDiagramManager();

private:

	static const long ID_GRID_ON_OFF_MENUITEM;
	static const long ID_SCALE_TO_VIEW_ALL_MENUITEM;
	static const long ID_MOVE_SHAPES_MENUITEM;
	static const long ID_COPY_IMAGE_RC_MENUITEM;

	// void OnGridOnOff(wxCommandEvent& event);
	void OnScaleToViewAll (wxCommandEvent& event);
	void OnMoveShapes (wxCommandEvent& event);
	void OnCanvasCopyImageRC (wxCommandEvent& event);
	bool moveShapes;
	bool *ptr_dirty;
	GMLTypeLabel gmlTypeLabel;
	wxMenu canvasPopupMenu;
	wxSlider *canvasSlider;
	GraphAttributes *GA;
	Graph *G;
	wxSFDiagramManager *m_canvas;
	wxMenuItem *allowShapeMovement;

	DECLARE_EVENT_TABLE()
};

#endif // DRAWINGCANVAS_H_INCLUDED
