#ifndef SEARCHTREEDRAWINGCANVAS_H
#define SEARCHTREEDRAWINGCANVAS_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/slider.h>
#include <wx/menu.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <wx/vector.h>

#include "trees/binary/BinaryTree.h"
#include "trees/search/stree.h"
#include "trees/search/AVLTree.h"
#include "trees/search/RBTree.h"
#include "trees/draw/DrawTree.h"
#include "common/DrawingCanvas.h"
#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"

using namespace std;

class SearchTreeDrawingCanvas : public wxFrame
{
public:
	enum BalanceType
	{
		SearchTree,
		AVL,
		RedBlack,
		NotBalanced
	};

	SearchTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, BalanceType t, const wxVector<int>& input, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE);
	virtual ~SearchTreeDrawingCanvas();
	void makeTreeFromGMLFile (const wxString& gmlFileName);

protected:
	wxTextCtrl* searchTreeDrawingCanvasTextCtrl;
	wxPanel* searchTreeDrawingCanvasControlPanel;
	wxRadioBox* searchTreeDrawingCanvasScanChoiceRadioBox;
	wxButton* searchTreeDrawingCanvasGoButton;
	wxStaticLine* searchTreeDrawingCanvasVerticalLine;
	wxTextCtrl* searchTreeDrawingCanvasInputDataTextCtrl;
	wxButton* searchTreeDrawingCanvasAddButton;
	wxButton* searchTreeDrawingCanvasEraseButton;
	wxSlider* searchTreeDrawingCanvasSlider;
	wxPanel* searchTreeDrawingPanel;
	wxButton* searchTreeDrawingCanvasCloseButton;
	wxMenuBar* searchTreeDrawingCanvasMenuBar;
	wxMenu* searchTreeDrawingCanvasFileMenu;
	wxMenuItem* searchTreeDrawingCanvasPrintMenuItem;
	wxMenuItem* searchTreeDrawingCanvasPrintPreviewMenuItem;
	wxMenuItem* searchTreeDrawingCanvasPageSetupMenuItem;
	wxMenuItem* searchTreeDrawingCanvasSaveAsMenuItem;
	wxMenu* searchTreeDrawingCanvasEditMenu;
	wxMenuItem* searchTreeDrawingCanvasCopyImageMenuItem;

private:
	virtual void OnSearchTreeDrawingCanvasGoButtonPressed (wxCommandEvent& event);
	virtual void OnSearchTreeDrawingCanvasAddButtonPressed (wxCommandEvent& event);
	virtual void OnSearchTreeDrawingCanvasEraseButtonPressed (wxCommandEvent& event);
	virtual void OnSearchTreeDrawingCanvasSliderMoved (wxScrollEvent& event);
	virtual void OnSearchtreedrawingcanvasclosebuttonButtonPressed (wxCommandEvent& event);
	virtual void handleMenuChoice (wxCommandEvent& event);
	virtual void OnCloseWindow (wxCloseEvent& event);
	bool closeFrame();
	virtual void OnQuit (wxCommandEvent& event);
	void OnSearchTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);

	enum UpdateType
	{
		Add,
		Erase
	};

	void buildMySearchTree (stnode *t);
	void buildMyAVLTree (AVLNode *t);
	void buildMyRedBlackTree (RBNode *t);
	void buildSearchTree (const wxVector<int>& data);
	void update (UpdateType updateType);

	BinaryTree *myBinaryTree;
	BalanceType searchTreeType;
	stree *searchTree;
	AVLTree *avlTree;
	RBTree *rbTree;
	GraphAttributes GA;
	wxString windowTitle;
	wxSFDiagramManager *searchTreeDrawingCanvas_Manager;
	DrawingCanvas* searchTreeDrawingCanvas;
	DrawTree *drawOnCanvas;
	MenuEventHandler *menuHandler;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;
	wxWindow* myParent;

	long ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	     ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	     ID_SEARCH_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	     ID_SEARCH_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	     ID_SEARCH_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM;
	static const long ID_SEARCH_TREE_DRAWING_CANVAS_HELP_MENUITEM;

	DECLARE_EVENT_TABLE()
};

#endif //SEARCHTREEDRAWINGCANVAS_H
