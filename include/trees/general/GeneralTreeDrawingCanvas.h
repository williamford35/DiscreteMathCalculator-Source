#ifndef GENERALTREEDRAWINGCANVAS_H
#define GENERALTREEDRAWINGCANVAS_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/menu.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include "common/DrawingCanvas.h"
#include "wx/wxsf/wxShapeFramework.h"
#include <wx/string.h>
#include "trees/general/GeneralTree.h"
#include "trees/draw/DrawTree.h"
#include "common/MenuEventHandler.h"

class GeneralTreeDrawingCanvas : public wxFrame
{
public:
	GeneralTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX);
	virtual ~GeneralTreeDrawingCanvas();
	bool ProcessEvent (wxEvent& event);

	void makeTreeFromLines (const wxVector<wxString>& gTreeData);
	void makeTreeFromGTRFile (const wxString& gtrFileName);
	void makeTreeFromGMLFile (const wxString& gmlFileName);

protected:
	wxPanel* generalTreeDrawingCanvasPrimaryPanel;
	wxPanel* generalTreeDrawingCanvasTraversalPanel;
	wxStaticText* generalTreeDrawingCanvasTraversalStaticText;
	wxTextCtrl* generalTreeDrawingCanvasTraversalTextCtrl;
	wxPanel* generalTreeDrawingCanvasControlPanel;
	wxRadioBox* generalTreeDrawingCanvasTraversalRadioBox;
	wxButton* generalTreeDrawingCanvasGoButton;
	wxButton* generalTreeDrawingCanvasBuildEquivBinaryTreeButton;
	wxSlider* generalTreeDrawingCanvasSlider;
	wxPanel* generalTreeDrawingCanvasClosePanel;
	wxButton* generalTreeDrawingCanvasCloseButton;
	wxMenuBar* generalTreeDrawingCanvasMenuBar;
	wxMenu* generalTreeDrawingCanvasFileMenu;
	wxMenuItem* generalTreeDrawingCanvasPrintMenuItem;
	wxMenuItem* generalTreeDrawingCanvasPrintPreviewMenuItem;
	wxMenuItem* generalTreeDrawingCanvasPageSetupMenuItem;
	wxMenuItem* generalTreeDrawingCanvasSaveAsMenuItem;
	wxMenu* generalTreeDrawingCanvasEditMenu;
	wxMenuItem* generalTreeDrawingCanvasCopyImageMenuItem;

protected:
	void OnGeneralTreeDrawingCanvasGoButtonPress (wxCommandEvent& event);
	void OnGeneralTreeDrawingCanvasBuildEquivBinaryTreeButtonPress (wxCommandEvent& event);
	void OnGeneralTreeDrawingCanvasCloseButtonPress (wxCommandEvent& event);
	void handleMenuChoice (wxCommandEvent& event);
	void OngeneralTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event);
	void OngeneralTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);
	void OnQuit (wxCommandEvent& event);
	bool closeFrame();

private:

	long ID_GENERAL_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	     ID_GENERAL_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	     ID_GENERAL_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	     ID_GENERAL_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	     ID_GENERAL_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM;

	static const long ID_GENERAL_TREE_DRAWING_CANVAS_HELP_MENUITEM;

	GeneralTree *myGeneralTree;
	wxString treeTitle;
	DrawTree *drawOnCanvas;
	wxSFDiagramManager *generalTreeDrawingCanvas_Manager;
	DrawingCanvas* generalTreeDrawingCanvas;
	MenuEventHandler *menuHandler;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;
	bool haveClosedParent;
	wxWindow *myParent;

	DECLARE_EVENT_TABLE()
};

#endif // GENERALTREEDRAWINGCANVAS_H
