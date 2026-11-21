#ifndef __EXPRESSIONTREEDRAWINGCANVAS_H__
#define __EXPRESSIONTREEDRAWINGCANVAS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/event.h>

///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>

#include "common/DrawingCanvas.h"
#include "trees/binary/BinaryTree.h"
#include "trees/draw/DrawTree.h"
#include "common/MenuEventHandler.h"
#include <wx/vector.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// Class ExpressionTreeDrawingCanvas
///////////////////////////////////////////////////////////////////////////////
class ExpressionTreeDrawingCanvas : public wxFrame
{
private:
	DrawingCanvas* expressionTreeDrawingCanvas;
	BinaryTree *expressionTree;
	wxString treeTitle;
	wxWindow *myParent;
	DrawTree *drawOnCanvas;
	wxSFDiagramManager *expressionTreeDrawingCanvas_Manager;
	MenuEventHandler *menuHandler;
	long int expValue;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;

	long EXPRESSIONDRAWINGCANVASPRINTMENUITEM_ID,
	     EXPRESSIONDRAWINGCANVASPRINTPREVIEWMENUITEM_ID,
	     EXPRESSIONDRAWINGCANVASPAGESETUPMENUITEM_ID,
	     EXPRESSIONDRAWINGCANVASSAVEASMENUITEM_ID,
	     EXPRESSIONDRAWINGCANVASCOPYIMAGEMENUITEM_ID;

	static const long ID_EXPRESSION_TREE_DRAWING_CANVAS_HELP_MENUITEM;

protected:
	wxStaticText* expressionDrawingCanvasStaticText;
	wxTextCtrl* expressionDrawingCanvasTextCtrl;
	wxSlider* expressionDrawingCanvasSlider;
	wxPanel* expressionDrawingCanvasClosePanel;
	wxButton* expressionDrawingCanvasCloseButton;
	wxMenuBar* expressionDrawingCanvasMenuBar;
	wxMenu* expressionDrawingCanvasFileMenu;
	wxMenu* expressionDrawingCanvasEditMenu;

	void OnexpressionTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event);
	void OnExpressionTreeDrawingCanvasCloseButtonClick (wxCommandEvent& event);
	void OnexpressionTreeDrawingCanvasExitMenuItemSelected (wxCommandEvent& event);
	void OnExpressionTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);

	void handleMenuChoice (wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);
	void OnQuit (wxCommandEvent& event);
	bool closeFrame();

public:

	ExpressionTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxTAB_TRAVERSAL);
	~ExpressionTreeDrawingCanvas();
	void buildExpressionTree (stringstream& parserOutput,
	                          const wxVector<wxString>& fromExpressionTreeDialog, const long int value);
	void makeTreeFromGMLFile (const wxString& gmlFileName);

	DECLARE_EVENT_TABLE()
};

#endif //__EXPRESSIONTREEDRAWINGCANVAS_H__
