#ifndef EQUIVALENTBINARYTREE_H
#define EQUIVALENTBINARYTREE_H

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
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/menu.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <wx/string.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/tree/TreeLayout.h>
#include "common/DrawingCanvas.h"
#include "wx/wxsf/wxShapeFramework.h"
#include "trees/draw/DrawTree.h"
#include "common/MenuEventHandler.h"

#include <map>
#include <string>

using namespace ogdf;
using namespace std;

class EquivalentBinaryTree : public wxFrame
{
public:
	EquivalentBinaryTree (wxWindow* parent, const wxString& canvasTitle, Graph *genG, GraphAttributes *genGA, node genRoot, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX);
	virtual ~EquivalentBinaryTree();

	void buildEquivalentTree();

protected:
	wxTextCtrl* equivalentBinaryTreeTextCtrl;
	wxPanel* equivalentBinaryTreeControlPanel;
	wxStaticText* equivalentBinaryTreeStaticText;
	wxButton* equivalentBinaryTreeGoButton;
	wxSlider* equivalentBinaryTreeSlider;
	wxPanel* equivalentBinaryTreeClosePanel;
	wxButton* equivalentBinaryTreeCloseButton;
	wxMenuBar* equivalentBinaryTreeDrawingCanvasMenuBar;
	wxMenu* equivalentBinaryTreeDrawingCanvasFileMenu;
	wxMenuItem* equivalentBinaryTreeDrawingCanvasPrintMenuItem;
	wxMenuItem* equivalentBinaryTreeDrawingCanvasPrintPreviewMenuItem;
	wxMenuItem* equivalentBinaryTreeDrawingCanvasPageSetupMenuItem;
	wxMenuItem* equivalentBinaryTreeDrawingCanvasSaveAsMenuItem;
	wxMenu* equivalentBinaryTreeDrawingCanvasEditMenu;
	wxMenuItem* equivalentBinaryTreeDrawingCanvasCopyImageMenuItem;

protected:
	void OnequivalentBinaryTreeTraversalButtonClick (wxCommandEvent& event);
	void OnequivalentBinaryTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event);
	void OnequivalentBinaryTreeExitMenuItemSelected (wxCommandEvent& event);
	void OnequivalentBinaryTreeHelpMenuItemSelected (wxCommandEvent& event);
	void handleMenuChoice (wxCommandEvent& event);
	virtual void OnCloseWindow (wxCloseEvent& event);
	bool closeFrame();
	virtual void OnQuit (wxCommandEvent& event);

private:

	long ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	     ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	     ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	     ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	     ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM;
	static const long ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM;

	void init();
	node createNode (const string& label);
	void insertNullSubtree (node currNode);
	void insertEdge (node parent, node child);
	node findLeftmostChild (node t);
	node findRightSibling (node t);
	void dfs (node t);
	void process (node t);
	void completeBuild();
	void preorder();
	void NLR (node t);

	DrawingCanvas* equivalentBinaryTreeCanvas;
	Graph *generalG;
	GraphAttributes *generalGA;
	node generalRoot, root;
	node nil;
	Graph G;
	GraphAttributes GA;
	map<node, node> binTreeBuildInfo;
	wxString treeTitle;
	DrawTree *drawOnCanvas;
	MenuEventHandler *menuHandler;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;
	wxWindow* myParent;

	// create wxSF diagram manager
	wxSFDiagramManager *generalTreeDrawingCanvas_Manager;

	DECLARE_EVENT_TABLE()
};

#endif // EQUIVALENTBINARYTREE_H
