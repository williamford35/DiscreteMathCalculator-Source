#ifndef ISOMORPHISMDRAWINGCANVAS_H
#define ISOMORPHISMDRAWINGCANVAS_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

//(*Headers(DisplayAlgorithmResults)
#include "common/DrawingCanvas.h"
//*)

#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include "graphs/build/BuildGraph.h"
#include "graphs/draw/DrawGraph.h"
#include "common/MenuEventHandler.h"
#include "graphs/GraphType.h"
#include "trees/general/GeneralTree.h"

using namespace std;
using namespace ogdf;

extern void testForSubgraphIsomorphism (ogdf::GraphAttributes *GA1, ogdf::Graph *G1,
                                        ogdf::GraphAttributes *GA2, ogdf::Graph *G2);

extern void testForIsomorphism (ogdf::GraphAttributes *GA1, ogdf::Graph *G1,
                                ogdf::GraphAttributes *GA2, ogdf::Graph *G2);

class IsomorphismDrawingCanvas : public wxFrame
{
protected:
	wxPanel* graphsPanel;
	DrawingCanvas* graphDrawing1, *graphDrawing2;
	wxSlider* IsomorphismDrawingCanvasSlider;
	wxPanel* IsomorphismDrawingCanvasTestPanel;
	wxRadioBox* IsomorphismDrawingCanvasInputG1RadioBox;
	wxRadioBox* IsomorphismDrawingCanvasInputG2RadioBox;
	wxButton* IsomorphismDrawingCanvasInputG1Button;
	wxButton* IsomorphismDrawingCanvasInputG2Button;
	wxButton* IsomorphismDrawingCanvasRunIsomorphismTestButton;
	wxPanel* IsomorphismDrawingCanvasCloseCancelPanel;
	wxButton* IsomorphismDrawingCanvasCancelButton;
	wxButton* IsomorphismDrawingCanvasCloseButton;
	wxMenuBar* IsomorphismDrawingCanvasMenuBar;
	wxMenuItem* IsomorphismDrawingCanvasCopyImageMenuItem;
	wxMenu* IsomorphismDrawingCanvasHelpMenu;
	wxMenuItem* IsomorphismDrawingCanvasHelpMenuItem;

	static const long ID_ISOMORPHISM_DRAWING_CANVAS_HELP_MENUITEM;
	static const long ID_ISOMORPHISM_DRAWING_CANVAS_SLIDER;

protected:
	virtual void OnInputG1ButtonPress (wxCommandEvent& event);
	virtual void OnInputG2ButtonPressed (wxCommandEvent& event);
	virtual void OnRunIsomorphismTestButtonPress (wxCommandEvent& event);
	virtual void OnIsomorphismDrawingCanvasSliderCmdScroll(wxScrollEvent& event);
	virtual void OnCancelButtonPress (wxCommandEvent& event);
	virtual void OnCloseButtonPress (wxCommandEvent& event);
	virtual void OnCloseWindow (wxCloseEvent& event);
	virtual void OnQuit (wxCommandEvent& event);
	bool closeFrame();
	void OnIsomorphismDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);

public:
	IsomorphismDrawingCanvas (wxWindow* parent, bool subgrIsomorphism = false, GraphLayout currLayout=forcedirected,wxWindowID id = wxID_ANY, const wxString& title = _ (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX);
	virtual ~IsomorphismDrawingCanvas();

private:
	wxSFDiagramManager m_canvas1, m_canvas2;
	BuildGraph *builtDisplayTwoGraphsResults;
	DrawGraph *drawDisplayTwoGraphsResults;
	GraphType graphType;
	wxString displayResultsTitle;
	wxWindow* myParent;
	BuildGraph *myGraph1, *myGraph2;
	bool subgraphIsomorphism;
	GraphLayout layout;

	void EnterFromGRPH(int graphNo);
	void EnterFromGML(int graphNo);

	wxDECLARE_EVENT_TABLE();
};

#endif // ISOMORPHISMDRAWINGCANVAS_H
