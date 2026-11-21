#ifndef HEAPDRAWINGCANVAS_H
#define HEAPDRAWINGCANVAS_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/menu.h>
#include <wx/scrolwin.h>
#include <wx/statusbr.h>

#include "trees/binary/BinaryTree.h"
#include "trees/draw/DrawTree.h"
#include "common/DrawingCanvas.h"
#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"

#include <wx/vector.h>

using namespace std;

class HeapDrawingCanvas : public wxFrame
{
public:
	enum HeapType
	{
		MINIMUM,
		MAXIMUM
	};

	HeapDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, const wxVector<int>& data, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL);
	virtual ~HeapDrawingCanvas();
	void buildHeap (HeapType type);
	void makeTreeFromGMLFile (const wxString& gmlFileName);

private:
	DrawTree *drawOnCanvas;
	wxSFDiagramManager *heapDrawingCanvas_Manager;
	DrawingCanvas* heapDrawingCanvas;

	enum UpdateType
	{
		PUSH,
		POP
	};

	void buildMyHeapTree (int pos);
	void outputHeapArray();
	void update (UpdateType updateType);
	bool recreateHeap();
	virtual void OnCloseWindow (wxCloseEvent& event);
	bool closeFrame();
	virtual void OnQuit (wxCommandEvent& event);

	wxString heapTitle;
	wxWindow* myParent;
	BinaryTree *myBinaryTree;
	wxVector<int> inputData;
	HeapType heapType;
	int heapLast;
	MenuEventHandler *menuHandler;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;

	long ID_HEAP_DRAWING_CANVAS_PRINT_MENUITEM,
	     ID_HEAP_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	     ID_HEAP_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	     ID_HEAP_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	     ID_HEAP_DRAWING_CANVAS_COPY_IMAGE_MENUITEM;
	static const long ID_HEAP_DRAWING_CANVAS_HELP_MENUITEM;

protected:
	wxPanel* heapControlPanel;
	wxPanel* heapContentsPanel;
	wxStaticText* correspondingArrayStaticText;
	wxTextCtrl* correspondingArrayTextCtrl;
	wxPanel* heapPushPopPanel;
	wxTextCtrl* heapPushPopInputTextCtrl;
	wxButton* heapPushButton;
	wxButton* heapPopButton;
	wxSlider* heapDrawingCanvasSlider;
	wxPanel* heapDrawingCanvasClosePanel;
	wxButton* heapDrawingCanvasCloseButton;
	wxMenuBar* heapDrawingCanvasMenuBar;
	wxMenu* heapDrawingCanvasFileMenu;
	wxMenuItem* heapDrawingCanvasPrintMenuItem;
	wxMenuItem* heapDrawingCanvasPrintPreviewMenuItem;
	wxMenuItem* heapDrawingCanvasPageSetupMenuItem;
	wxMenuItem* heapDrawingCanvasSaveAsMenuItem;
	wxMenu* heapDrawingCanvasEditMenu;
	wxMenuItem* heapDrawingCanvasCopyImageMenuItem;

protected:
	virtual void OnHeapCanvasCloseButtonClick (wxCommandEvent& event);
	virtual void handleMenuChoice (wxCommandEvent& event);
	void OnheapDrawingCanvasPopButtonClick (wxCommandEvent& event);
	void OnheapDrawingCanvasPushButtonClick (wxCommandEvent& event);
	void OnheapDrawingCanvasExitMenuItemSelected (wxCommandEvent& event);
	void OnheapDrawingCanvasSliderCmdScroll (wxScrollEvent& event);
	void OnHeapDrawingCanvasHelpMenuItemSelected(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif // HEAPDRAWINGCANVAS_H
