#ifndef TREEISOMORPHISMDRAWINGCANVAS_H
#define TREEISOMORPHISMDRAWINGCANVAS_H

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
#include "trees/draw/DrawTree.h"
#include "common/MenuEventHandler.h"
#include "graphs/GraphType.h"
#include "trees/general/GeneralTree.h"
#include "trees/binary/BinaryTree.h"

using namespace std;
using namespace ogdf;

class TreeIsomorphismDrawingCanvas : public wxFrame
{
protected:
	wxPanel* treePanel;
	DrawingCanvas *treeDrawing1, *treeDrawing2;
	wxSlider* TreeIsomorphismDrawingCanvasSlider;
	wxPanel* TreeIsomorphismDrawingCanvasTestPanel;
	wxRadioBox* TreeIsomorphismDrawingCanvasInputT1RadioBox;
	wxRadioBox* TreeIsomorphismDrawingCanvasInputT2RadioBox;
	wxButton* TreeIsomorphismDrawingCanvasInputT1Button;
	wxButton* TreeIsomorphismDrawingCanvasInputT2Button;
	wxButton* TreeIsomorphismDrawingCanvasRunIsomorphismTestButton;
	wxPanel* TreeIsomorphismDrawingCanvasCloseCancelPanel;
	wxButton* TreeIsomorphismDrawingCanvasCancelButton;
	wxButton* TreeIsomorphismDrawingCanvasCloseButton;
	wxMenuBar* TreeIsomorphismDrawingCanvasMenuBar;
	wxMenuItem* TreeIsomorphismDrawingCanvasCopyImageMenuItem;
	wxMenu* TreeIsomorphismDrawingCanvasHelpMenu;
	wxMenuItem* TreeIsomorphismDrawingCanvasHelpMenuItem;

	static const long ID_TREE_ISOMORPHISM_DRAWING_CANVAS_HELP_MENUITEM;
	static const long ID_TREE_ISOMORPHISM_DRAWING_CANVAS_SLIDER;

protected:
	virtual void OnInputT1ButtonPressed (wxCommandEvent& event);
	virtual void OnInputT2ButtonPressed (wxCommandEvent& event);
	virtual void OnRunTreeIsomorphismTestButtonPress (wxCommandEvent& event);
	virtual void OnTreeIsomorphismDrawingCanvasSliderCmdScroll(wxScrollEvent& event);
	virtual void OnCancelButtonPress (wxCommandEvent& event);
	virtual void OnCloseButtonPress (wxCommandEvent& event);
	virtual void OnCloseWindow (wxCloseEvent& event);
	virtual void OnQuit (wxCommandEvent& event);
	bool closeFrame();
	void OnTreeIsomorphismDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);

public:
	TreeIsomorphismDrawingCanvas (wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _ ("Tree Isomorphism Test"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX);
	virtual ~TreeIsomorphismDrawingCanvas();

private:
	wxSFDiagramManager *m_canvas1, *m_canvas2;
	wxString displayResultsTitle;
	wxWindow* myParent;
	GeneralTree *myTree1, *myTree2;
	BinaryTree *myBinaryTree1, *myBinaryTree2;
	Graph *myGraph1, *myGraph2;
	GraphAttributes *myGA1, *myGA2;
	node myroot1, myroot2;
	DrawTree *tree1Drawing, *tree2Drawing;

	void EnterFromGTR(int treeNo);
	void EnterFromGML(int treeNo);
	void EnterFromBTR(int treeNo);

	wxDECLARE_EVENT_TABLE();
};

#endif // TREEISOMORPHISMDRAWINGCANVAS_H
