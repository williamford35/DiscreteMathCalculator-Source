#ifndef WIDESTPATHDRAWINGCANVAS_H
#define WIDESTPATHDRAWINGCANVAS_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"
#include "graphs/build/BuildGraph.h"

class WidestPathDrawingCanvas : public wxFrame
{
protected:
	wxStaticText* MaximumSpanningTreeStaticText;
	wxStaticText* WidestPathStaticText;
	DrawingCanvas* MaximumSpanningTreeGraph;
	DrawingCanvas* WidestPathGraph;
	wxSlider* MaximumSpanningTreeSlider;
	wxSlider* WidestPathSlider;
	wxMenuBar* WidestPathDrawingCanvasMenuBar;
	wxMenu* widestPathDrawingCanvasFileMenu;
	wxMenu* WidestPathDrawingCanvasHelpMenu;
	wxMenuItem* WidestPathDrawingCanvasHelpMenuItem;
	wxPanel* FillerPanel;
	wxPanel* ClosePanel;
	wxButton* SaveMaximumSpanningTreeButton;
	wxButton* SaveWidestPathButton;
	wxButton* CloseButton;

	static const long ID_WIDESTPATH_DRAWING_CANVAS_HELP_MENUITEM;
	static const long ID_MAXSPANNINGTREE_DRAWING;
	static const long ID_WIDESTPATH_DRAWING;

protected:
	virtual void OnWidestPathDrawingCanvasExitMenuItemSelected (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);
	virtual void OnSaveSpanningTreeButtonPress(wxCommandEvent& event);
	virtual void OnSaveWidestPathButtonPress(wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);
	virtual void OnMaximumSpanningTreeSliderCmdScroll(wxScrollEvent& event);
	virtual void OnWidestPathSliderCmdScroll(wxScrollEvent& event);
	void OnWidestPathDrawingCanvasHelpMenuItemSelected(wxCommandEvent& event);
	bool closeFrame();
	wxStaticText* MaximumSpanningTreeResult;
	wxStaticText* WidestPathResult;
public:
	WidestPathDrawingCanvas (wxWindow* parent, BuildGraph *mSpanningTree, BuildGraph *widPath, BuildGraph *combGraphs, wxWindowID id = wxID_ANY, const wxString& title = _ ("Widest Path"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE);
	virtual ~WidestPathDrawingCanvas();
	void DrawMaximumSpanningTree();
	void DrawWidestPath();
	void SetResults(const wxString& treeResult,
		const wxString& pathResult);
private:
	wxWindow* myParent;
	wxSFDiagramManager maxspanningtree_canvas,
	                   widestpath_canvas, print_canvas;
	bool dirty, haveAskedAboutSavingFile;
	BuildGraph *buildMaxSpanningTree, *buildWidestPath, *combinedGraphs;
	DrawGraph *drawMaxSpanningTree, *buildWidestPathGraph, *printGraphs;
	DrawingCanvas *printGraphsCanvas;

	void prepareToPrintGraphs();

	DECLARE_EVENT_TABLE()
};

#endif // WIDESTPATHDRAWINGCANVAS_H
