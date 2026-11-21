#ifndef BINARYTREEDRAWINGCANVAS_H
#define BINARYTREEDRAWINGCANVAS_H

//(*HeadersPCH(BinaryTreeDrawingCanvas)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/radiobox.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)
//(*Headers(BinaryTreeDrawingCanvas)
//*)

#include <wx/string.h>
#include "trees/binary/BinaryTree.h"
#include "wx/wxsf/wxShapeFramework.h"
#include "common/DrawingCanvas.h"
#include "trees/draw/DrawTree.h"
#include "common/MenuEventHandler.h"

#include <wx/vector.h>

class BinaryTreeDrawingCanvas: public wxFrame
{
public:

	BinaryTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxTAB_TRAVERSAL);
	virtual ~BinaryTreeDrawingCanvas();
	bool ProcessEvent (wxEvent& event);

	//(*Declarations(BinaryTreeDrawingCanvas)
	wxMenuBar* binaryTreeDrawingCanvasMenuBar;
	wxPanel* binaryTreeDrawingCanvasScanPanel;
	wxRadioBox* binaryTreeDrawingCanvasScanRadioBox;
	wxTextCtrl* binaryTreeDrawingCanvasScanTextCtrl;
	wxButton* binaryTreeDrawingCanvasScanGoButton;
	wxButton* binaryTreeDrawingCanvasCloseButton;
	wxStatusBar* binaryTreeDrawingCanvasStatusBar;
	wxSlider* binaryTreeDrawingCanvasSlider;
	wxPanel* binaryTreeDrawingCanvasPanel;
	//*)

	void makeTreeFromLines (const wxVector<wxString>& bTreeData);
	void makeTreeFromBTRFile (const wxString& btrFileName);
	void makeTreeFromGMLFile (const wxString& gmlFileName);

protected:

	//(*Identifiers(BinaryTreeDrawingCanvas)
	static const long ID_BINARY_TREE_DRAWING_CANVAS;
	static const long ID_BINARY_TREE_SCAN_TEXTCTRL;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_SCAN_RADIOBOX;
	static const long ID_BINARY_TREE_CANVAS_SCAN_GO_BUTTON;
	static const long ID_BINARY_TREE_CANVAS_CLOSE_BUTTON;
	static const long ID_BINARY_TREE_CANVAS_SCAN_PANEL;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_SLIDER;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_PANEL;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_EXIT_MENUITEM;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_STATUSBAR;
	static const long ID_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM;

	//*)

private:

	void handleMenuChoice (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);
	void OnbinaryTreeDrawingCanvasScanGoButtonClick (wxCommandEvent& event);
	void OnbinaryTreeDrawingCanvasCloseButtonClick (wxCommandEvent& event);
	void OnbinaryTreeDrawingCanvasExitMenuItemSelected (wxCommandEvent& event);
	void OnbinaryTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event);
	void OnBinaryTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);
	bool closeFrame();
	BinaryTree *myBinaryTree;
	wxString treeTitle;
	wxWindow *myParent;

	DrawTree *drawOnCanvas;
	wxSFDiagramManager *binaryTreeDrawingCanvas_Manager;
	DrawingCanvas* binaryTreeDrawingCanvas;
	MenuEventHandler *menuHandler;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;

	DECLARE_EVENT_TABLE()
};

#endif
