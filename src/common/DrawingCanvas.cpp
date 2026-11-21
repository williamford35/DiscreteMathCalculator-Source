/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/clipbrd.h>
#include <wx/textfile.h>

#include <ogdf/fileformats/GraphIO.h>

#include "common/DrawingCanvas.h"
#include "common/report.h"

const long DrawingCanvas::ID_GRID_ON_OFF_MENUITEM = wxNewId();
const long DrawingCanvas::ID_SCALE_TO_VIEW_ALL_MENUITEM = wxNewId();
const long DrawingCanvas::ID_MOVE_SHAPES_MENUITEM = wxNewId();
const long DrawingCanvas::ID_COPY_IMAGE_RC_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (DrawingCanvas, wxSFShapeCanvas)
	// EVT_MENU	(ID_GRID_ON_OFF_MENUITEM, DrawingCanvas::OnGridOnOff)
	EVT_MENU	(ID_SCALE_TO_VIEW_ALL_MENUITEM, DrawingCanvas::OnScaleToViewAll)
	// EVT_MENU	(ID_MOVE_SHAPES_MENUITEM, DrawingCanvas::OnMoveShapes)
	EVT_MENU	(ID_COPY_IMAGE_RC_MENUITEM, DrawingCanvas::OnCanvasCopyImageRC)
END_EVENT_TABLE()

DrawingCanvas::DrawingCanvas (wxSFDiagramManager *canvas, wxWindow *parent,
                              wxWindowID id,const wxPoint &pos,
                              const wxSize &size, long style) :
	wxSFShapeCanvas (canvas, parent, id, pos, size, style), m_canvas (canvas)
{
	canvasPopupMenu.Append (ID_SCALE_TO_VIEW_ALL_MENUITEM, wxT ("Scale to View All") );
	canvasPopupMenu.Append (ID_COPY_IMAGE_RC_MENUITEM, wxT ("Copy Image") );
	gmlTypeLabel = None;
}

DrawingCanvas::~DrawingCanvas()
{}

void DrawingCanvas::OnLeftDown (wxMouseEvent &event)
{}

void DrawingCanvas::OnRightDown (wxMouseEvent &event)
{
	wxSFShapeCanvas::OnRightDown (event);
	PopupMenu (&canvasPopupMenu);
}

void DrawingCanvas::setSliderAddress (wxSlider *canvasSldr)
{
	canvasSlider = canvasSldr;
}

void DrawingCanvas::setGA (GraphAttributes *gattr)
{
	GA = gattr;
}

void DrawingCanvas::setG (Graph *g)
{
	G = g;
}

/*void DrawingCanvas::OnGridOnOff(wxCommandEvent& event)
{
if (canvasPopupMenu.FindItemByPosition(0)->IsChecked())
{
	AddStyle(wxSFShapeCanvas::sfsGRID_SHOW);
	AddStyle(wxSFShapeCanvas::sfsGRID_USE);
}
else
{
	RemoveStyle (wxSFShapeCanvas::sfsGRID_SHOW);
	RemoveStyle (wxSFShapeCanvas::sfsGRID_USE);
}

Refresh(false);
}*/

void DrawingCanvas::OnScaleToViewAll (wxCommandEvent& event)
{
	SetScaleToViewAll();
	SetScale (GetScale() *.98);
	Refresh (false);

	canvasSlider->SetValue (int (GetScale() *50) );
}

void DrawingCanvas::OnMoveShapes (wxCommandEvent& event)
{
}

void DrawingCanvas::OnCanvasCopyImageRC (wxCommandEvent& event)
{
	copyToClipboard();
}

void DrawingCanvas::copyToClipboard()
{
	// create memory DC to draw the canvas content into
	wxRect bmpBB = GetTotalBoundingBox();
	bmpBB.Inflate (GetGrid() );

	wxBitmap outbmp (bmpBB.GetRight(), bmpBB.GetBottom() );
	wxMemoryDC dc (outbmp);

	//wxSFScaledPaintDC outdc(outbmp, 1);
	wxSFScaledDC outdc ( (wxWindowDC*) &dc, GetScale() );

	if (outdc.IsOk() )
	{
		DrawBackground (outdc, sfNOT_FROM_PAINT);
		DrawForeground (outdc, sfNOT_FROM_PAINT);
		DrawContent (outdc, sfNOT_FROM_PAINT);
	}
	else
	{
		wxMessageBox (wxT ("Could not create clipboard output."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
		return;
	}

	if (wxTheClipboard->Open() )
	{
		wxTheClipboard->SetData (new wxBitmapDataObject (outbmp) );
		wxTheClipboard->Close();
	}
	else
		wxMessageBox (wxT ("Could not open the clipboard."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
}

void DrawingCanvas::SaveCanvasToGraphicFormat (GraphicOutputType type, const wxString& file)
{
	wxBitmapType imageType;

	switch (type)
	{
	case BMP:
		imageType = wxBITMAP_TYPE_BMP;
		break;

	case JPG:
		imageType = wxBITMAP_TYPE_JPEG;
		break;

	case GIF:
		imageType = wxBITMAP_TYPE_GIF;
		break;

	case PNG:
		imageType = wxBITMAP_TYPE_PNG;
		break;

	case TIFF:
		imageType = wxBITMAP_TYPE_TIFF;
		break;
	}

	SaveCanvasToImage (file, imageType, sfWITH_BACKGROUND);
}

void DrawingCanvas::handleSaveAsGML (const wxString& filename, GraphType graphType)
{
	string file ( (const char *) filename.c_str() );
	wxTextFile gmlFile;
	std::ofstream gmlOut ( (const char *) file.c_str() );
	wxString typeOfGraph, lastLine;

	/* bool t = GA->directed();
	long a = GA->attributes();
	Graph g = GA->constGraph(); */

	GraphIO::writeGML (*GA, gmlOut);
	gmlOut.close();

	if (graphType == undirtree)
		return;

	if (graphType != notagraph)
	{
		if (!gmlFile.Open (filename) )
		{
			wxString message, caption;

			message.append (
			   wxT ("Failure to properly create the GML file for the graph.") );
			caption.append (wxT ("Error") );

			wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
			dialog.ShowModal();

			return;
		}
		else
		{
			switch (graphType)
			{
			case undir:
				typeOfGraph = wxT ("# undir");
				break;

			case undirw:
				typeOfGraph = wxT ("# undirw");
				break;

			case digraph:
				typeOfGraph = wxT ("# digraph");
				break;

			case digraphw:
				typeOfGraph = wxT ("# digraphw");
				break;

			case undirtree:		/* cannot occur here */
				break;

			case notagraph:		/* cannot occur here */
				break;
			}

			lastLine = gmlFile.GetLastLine();
			gmlFile.AddLine(typeOfGraph);
			gmlFile.Write();
			gmlFile.Close();
		}
	}
	else
	{
		report (wxT ("Internal error. The image to be saved is not that of a graph or a tree.\nPlease report this error to wford333@gmail.com "),wxT ("Error") );

		return;
	}

	*ptr_dirty = false;
	// wxMessageBox (wxString::Format (wxT ("The GML file has been saved to '%s'."), filename.GetData() ), wxT ("ShapeFramework") );
}

void DrawingCanvas::getDirty (bool *d)
{
	ptr_dirty = d;
}

void DrawingCanvas::setGMLTreeTypeLabel (GMLTypeLabel t)
{
	gmlTypeLabel = t;
}

wxSFDiagramManager* DrawingCanvas::getSFDiagramManager()
{
	return m_canvas;
}
