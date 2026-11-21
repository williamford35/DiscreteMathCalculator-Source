/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DiscreteMathCalculatorMain.h"
#include "trees/huffman/HuffmanDrawingCanvas.h"
#include "common/ImageFiles.h"
#include "common/report.h"
#include <wx/bitmap.h>
#include <wx/msgdlg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

BEGIN_EVENT_TABLE (HuffmanDrawingCanvas,wxFrame)
	EVT_CLOSE (HuffmanDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, HuffmanDrawingCanvas::OnQuit)
END_EVENT_TABLE()

const long HuffmanDrawingCanvas::ID_HUFFMAN_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

HuffmanDrawingCanvas::HuffmanDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), treeTitle (canvasTitle), myParent(parent)
{
	wxMenuItem* huffmanDrawingCanvasHelpMenuItem;
	wxMenu* huffmanDrawingCanvasHelpMenu;

	wxIcon FrameIcon;
	FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
	SetIcon (FrameIcon);

	wxFlexGridSizer* huffmanDrawingCanvasFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	huffmanDrawingCanvasFlexGridSizer->SetFlexibleDirection (wxBOTH);
	huffmanDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	huffmanDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	huffmanDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (huffmanDrawingCanvasFlexGridSizer);

	drawOnCanvas = new DrawHuffmanTree();
	drawOnCanvas->setCanvasProperties ( (wxPanel *) this, huffmanDrawingCanvas,
	                                    huffmanDrawingCanvas_Manager);

	huffmanDrawingCanvas->SetSizeHints(1024, 600);
	huffmanDrawingCanvasFlexGridSizer->Add (huffmanDrawingCanvas, 1, wxALL|wxEXPAND, 1);

	huffmanDataPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (1024,-1), wxTAB_TRAVERSAL);

	huffmanDrawingCanvasFlexGridSizer->Add (huffmanDataPanel, 0, wxALL, 5);

	wxFlexGridSizer* huffmanDataPanelFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	huffmanDataPanelFlexGridSizer->SetFlexibleDirection (wxBOTH);
	huffmanDataPanelFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	huffmanDataPanelFlexGridSizer->AddGrowableCol (0);
	huffmanDataPanelFlexGridSizer->AddGrowableRow (0);
	huffmanDataPanel->SetSizer (huffmanDataPanelFlexGridSizer);

	huffmanBitCodesStaticText = new wxStaticText (huffmanDataPanel, wxID_ANY, _ ("Huffman Bit Codes"), wxDefaultPosition, wxSize (-1,-1), 0);

	huffmanDataPanelFlexGridSizer->Add (huffmanBitCodesStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	huffmanBitCodesTextCtrl = new wxTextCtrl (huffmanDataPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,125), wxTE_MULTILINE|wxHSCROLL);

	huffmanDataPanelFlexGridSizer->Add (huffmanBitCodesTextCtrl, 1, wxALL|wxEXPAND, 1);

	huffmanDrawingCanvasSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);

	huffmanDrawingCanvasFlexGridSizer->Add (huffmanDrawingCanvasSlider, 0, wxALL|wxEXPAND, 5);

	huffmanDrawingCanvasClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	huffmanDrawingCanvasFlexGridSizer->Add (huffmanDrawingCanvasClosePanel, 0, wxALL|wxEXPAND, 1);

	wxBoxSizer* huffmanDrawingCanvasClosePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	huffmanDrawingCanvasClosePanel->SetSizer (huffmanDrawingCanvasClosePanelBoxSizer);

	huffmanDrawingCanvasClosePanelBoxSizer->Add (0, 0, 1, wxALL, 1);

	huffmanDrawingCanvasClosePanelBoxSizer->Add (0, 0, 1, wxALL, 1);

	huffmanDrawingCanvasClosePanelBoxSizer->Add (0, 0, 1, wxALL, 1);

	huffmanDrawingCanvasClosePanelBoxSizer->Add (0, 0, 1, wxALL, 1);

	huffmanDrawingCanvasCloseButton = new wxButton (huffmanDrawingCanvasClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);

	huffmanDrawingCanvasClosePanelBoxSizer->Add (huffmanDrawingCanvasCloseButton, 0, wxALL, 1);

	huffmanDrawingCanvasClosePanelBoxSizer->Add (0, 0, 1, wxALL, 1);

	huffmanDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (huffmanDrawingCanvasMenuBar);

	huffmanDrawingCanvasFileMenu = new wxMenu();
	huffmanDrawingCanvasMenuBar->Append (huffmanDrawingCanvasFileMenu, _ ("File"));

	huffmanDrawingCanvasPrintMenuItem = new wxMenuItem (huffmanDrawingCanvasFileMenu, wxID_ANY, _ ("Print...\t*P"), wxT (""), wxITEM_NORMAL);
	huffmanDrawingCanvasFileMenu->Append (huffmanDrawingCanvasPrintMenuItem);

	/*huffmanDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (huffmanDrawingCanvasFileMenu, wxID_ANY, _ ("Print Preview"), wxT (""), wxITEM_NORMAL);
	huffmanDrawingCanvasFileMenu->Append (huffmanDrawingCanvasPrintPreviewMenuItem);*/

	huffmanDrawingCanvasPageSetupMenuItem = new wxMenuItem (huffmanDrawingCanvasFileMenu, wxID_ANY, _ ("Page Setup..."), wxT (""), wxITEM_NORMAL);
	huffmanDrawingCanvasFileMenu->Append (huffmanDrawingCanvasPageSetupMenuItem);

	huffmanDrawingCanvasSaveAsMenuItem = new wxMenuItem (huffmanDrawingCanvasFileMenu, wxID_ANY, _ ("Save as..."), wxT (""), wxITEM_NORMAL);
	huffmanDrawingCanvasFileMenu->Append (huffmanDrawingCanvasSaveAsMenuItem);

	huffmanDrawingCanvasEditMenu = new wxMenu();
	huffmanDrawingCanvasMenuBar->Append (huffmanDrawingCanvasEditMenu, _ ("Edit"));

	huffmanDrawingCanvasCopyImageMenuItem = new wxMenuItem (huffmanDrawingCanvasEditMenu, wxID_ANY, _ ("Copy Image\t*C"), wxT (""), wxITEM_NORMAL);
	huffmanDrawingCanvasEditMenu->Append (huffmanDrawingCanvasCopyImageMenuItem);

	CentreOnParent (wxBOTH);

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	CentreOnParent (wxBOTH);
#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this))
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif
	// Connect events
	huffmanDrawingCanvasSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (HuffmanDrawingCanvas::OnhuffmanDrawingCanvasSliderCmdScrollChanged), NULL, this);
	huffmanDrawingCanvasCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HuffmanDrawingCanvas::OnHuffmanDrawingCanvasCloseButtonPress), NULL, this);
	Connect (ID_HUFFMAN_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (HuffmanDrawingCanvas::OnHuffmanDrawingCanvasHelpMenuItemSelected));
	this->Connect (huffmanDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	// this->Connect (huffmanDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (huffmanDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (huffmanDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (huffmanDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);

	ID_HUFFMAN_DRAWING_CANVAS_PRINT_MENUITEM = huffmanDrawingCanvasPrintMenuItem->GetId();
	ID_HUFFMAN_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = -1;
	ID_HUFFMAN_DRAWING_CANVAS_PAGESETUP_MENUITEM = huffmanDrawingCanvasPageSetupMenuItem->GetId();
	ID_HUFFMAN_DRAWING_CANVAS_SAVE_AS_MENUITEM = huffmanDrawingCanvasSaveAsMenuItem->GetId();
	ID_HUFFMAN_DRAWING_CANVAS_COPY_IMAGE_MENUITEM = huffmanDrawingCanvasCopyImageMenuItem->GetId();

	huffmanDrawingCanvasHelpMenu = new wxMenu();
	huffmanDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	huffmanDrawingCanvasHelpMenuItem = new wxMenuItem (huffmanDrawingCanvasHelpMenu, ID_HUFFMAN_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	huffmanDrawingCanvasHelpMenu->Append (huffmanDrawingCanvasHelpMenuItem);
	huffmanDrawingCanvasMenuBar->Append (huffmanDrawingCanvasHelpMenu, _ ("&Help"));

	SetLabel (title);
	myHuffmanTree = new HuffmanTree (canvasTitle);
	huffmanDrawingCanvas->setSliderAddress (huffmanDrawingCanvasSlider);
	huffmanDrawingCanvas->setGA (&myHuffmanTree->GA);
	dirty = true;
	figureSaved = false;
	haveAskedAboutSavingFile = false;
	menuHandler = new MenuEventHandler (huffmanDrawingCanvas,
	                                    ID_HUFFMAN_DRAWING_CANVAS_PRINT_MENUITEM,
	                                    ID_HUFFMAN_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_HUFFMAN_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_HUFFMAN_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_HUFFMAN_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, undirtree);
	huffmanDrawingCanvas->getDirty (&dirty);
}

HuffmanDrawingCanvas::~HuffmanDrawingCanvas()
{
	huffmanDrawingCanvasSlider->Disconnect (wxEVT_SCROLL_CHANGED, wxScrollEventHandler (HuffmanDrawingCanvas::OnhuffmanDrawingCanvasSliderCmdScrollChanged), NULL, this);
	huffmanDrawingCanvasCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HuffmanDrawingCanvas::OnHuffmanDrawingCanvasCloseButtonPress), NULL, this);
	this->Disconnect (huffmanDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	// this->Disconnect (huffmanDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (huffmanDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (huffmanDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (huffmanDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HuffmanDrawingCanvas::handleMenuChoice), NULL, this);
}

void HuffmanDrawingCanvas::OnhuffmanDrawingCanvasSliderCmdScrollChanged (wxScrollEvent& event)
{
	huffmanDrawingCanvas->SetScale (double (huffmanDrawingCanvasSlider->GetValue()) /50);
	huffmanDrawingCanvas->Refresh (false);
}

void HuffmanDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	int numberOfLines, response;
	ofstream bitCodes;
	wxString formatSelection;
	wxString bitCodeFileName, bitCodesLine;
	char cstring[128];

	menuHandler->process (event,3);

	wxMessageDialog dialog (this, wxT ("Do you want to save the bit codes?"),
	                        wxT("Note"), wxYES_NO | wxICON_QUESTION);
	response = dialog.ShowModal();
	if (response == wxID_NO)
		return;

	wxFileDialog saveBitodeFileDialog(this, _("Save bitcode file"), "", "",
	                                  "BITCODE files (*.bitcode)|*.bitcode", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (saveBitodeFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wxString str = saveBitodeFileDialog.GetPath();
	strncpy(cstring, (const char*)str.mb_str(wxConvUTF8), 127);
	bitCodes.open(cstring);
	if (!bitCodes.is_open())
	{
		report(wxT("Cannot create the bit code file."), wxT("Note"));
		return;
	}
	numberOfLines = huffmanBitCodesTextCtrl->GetNumberOfLines();
	for (int i=0; i < numberOfLines; i++)
	{
		bitCodesLine = huffmanBitCodesTextCtrl->GetLineText(i);
		strncpy(cstring, (const char*)bitCodesLine.mb_str(wxConvUTF8), 127);
		bitCodes << cstring << endl;
	}
	bitCodes.close();
}

void HuffmanDrawingCanvas::buildHuffmanTree (const vector<HuffmanNode>& fromHuffmanDialog)
{
	myHuffmanTree->buildHuffmanTree (fromHuffmanDialog);
	myHuffmanTree->outputBitCodes (huffmanBitCodesTextCtrl);
	drawOnCanvas->setGraphProperties (& (myHuffmanTree->G), & (myHuffmanTree->GA), myHuffmanTree->getRoot());
	drawOnCanvas->draw();
}

void HuffmanDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	myHuffmanTree->init();
	myHuffmanTree->buildFromGML (gmlFileName);
	drawOnCanvas->setGraphProperties (& (myHuffmanTree->G), & (myHuffmanTree->GA), myHuffmanTree->getRoot());
	drawOnCanvas->draw();
	myHuffmanTree->outputGMLData (huffmanBitCodesTextCtrl);
	figureSaved = true;
}

bool HuffmanDrawingCanvas::closeFrame()
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Huffman tree figure"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
		{
			if (!menuHandler->handleSaveAs(3))
				return false;
		}
		else if (answer == wxCANCEL)
			return false;
	}

	return true;
}

void HuffmanDrawingCanvas::OnHuffmanDrawingCanvasCloseButtonPress (wxCommandEvent& event)
{
	if (closeFrame())
		Close(true);
}

void HuffmanDrawingCanvas::OnQuit (wxCommandEvent& event)
{
	bool closeTheFrame = false;

	if (!haveAskedAboutSavingFile)
	{
		haveAskedAboutSavingFile = true;
		closeTheFrame = closeFrame();

		if (closeTheFrame)
			((DiscreteMathCalculatorFrame *)myParent)->Close(true);
		else
		{
			haveAskedAboutSavingFile = false;
			return;
		}
	}
	else
		((DiscreteMathCalculatorFrame *)myParent)->Close(true);
}

void HuffmanDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
{
	bool closeTheFrame;

	if (!haveAskedAboutSavingFile)
	{
		haveAskedAboutSavingFile = true;
		closeTheFrame = closeFrame();

		if (closeTheFrame)
			Destroy();
		else
		{
			haveAskedAboutSavingFile = false;
			return;
		}
	}
}

void HuffmanDrawingCanvas::OnHuffmanDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
