#ifndef HUFFMANDRAWINGCANVAS_H
#define HUFFMANDRAWINGCANVAS_H

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
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/menu.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include "trees/huffman/HuffmanNode.h"
#include "trees/huffman/HuffmanTree.h"
#include "common/DrawingCanvas.h"
#include "wx/wxsf/wxShapeFramework.h"
#include "trees/draw/DrawHuffmanTree.h"
#include "common/MenuEventHandler.h"

#include <vector>
using namespace std;

class HuffmanDrawingCanvas : public wxFrame
{
public:
	HuffmanDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX|wxTAB_TRAVERSAL);
	virtual ~HuffmanDrawingCanvas();

	void buildHuffmanTree (const vector<HuffmanNode>& fromHuffmanDialog);
	void makeTreeFromGMLFile (const wxString& gmlFileName);

protected:
	wxPanel* huffmanDataPanel;
	wxStaticText* huffmanBitCodesStaticText;
	wxTextCtrl* huffmanBitCodesTextCtrl;
	wxSlider* huffmanDrawingCanvasSlider;
	wxPanel* huffmanDrawingCanvasClosePanel;
	wxButton* huffmanDrawingCanvasCloseButton;
	wxMenuBar* huffmanDrawingCanvasMenuBar;
	wxMenu* huffmanDrawingCanvasFileMenu;
	wxMenuItem* huffmanDrawingCanvasPrintMenuItem;
	wxMenuItem* huffmanDrawingCanvasPrintPreviewMenuItem;
	wxMenuItem* huffmanDrawingCanvasPageSetupMenuItem;
	wxMenuItem* huffmanDrawingCanvasSaveAsMenuItem;
	wxMenu* huffmanDrawingCanvasEditMenu;
	wxMenuItem* huffmanDrawingCanvasCopyImageMenuItem;

private:
	void OnhuffmanDrawingCanvasSliderCmdScrollChanged (wxScrollEvent& event);
	void OnHuffmanDrawingCanvasCloseButtonPress (wxCommandEvent& event);
	void OnHuffmanDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event);
	void handleMenuChoice (wxCommandEvent& event);
	virtual void OnCloseWindow (wxCloseEvent& event);
	bool closeFrame();
	virtual void OnQuit (wxCommandEvent& event);

	HuffmanTree *myHuffmanTree;
	wxString treeTitle;
	DrawHuffmanTree *drawOnCanvas;
	wxSFDiagramManager *huffmanDrawingCanvas_Manager;
	DrawingCanvas* huffmanDrawingCanvas;
	MenuEventHandler *menuHandler;
	bool dirty;
	bool figureSaved;
	bool haveAskedAboutSavingFile;
	wxWindow* myParent;

	long ID_HUFFMAN_DRAWING_CANVAS_PRINT_MENUITEM,
	     ID_HUFFMAN_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	     ID_HUFFMAN_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	     ID_HUFFMAN_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	     ID_HUFFMAN_DRAWING_CANVAS_COPY_IMAGE_MENUITEM;
	static const long ID_HUFFMAN_DRAWING_CANVAS_HELP_MENUITEM;

	DECLARE_EVENT_TABLE()
};

#endif // HUFFMANDRAWINGCANVAS_H
