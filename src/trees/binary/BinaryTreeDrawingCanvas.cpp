/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/app.h>
#include <wx/textfile.h>
#include "DiscreteMathCalculatorMain.h"
#include "trees/binary/BinaryTreeDrawingCanvas.h"
#include "trees/btrFileFormatException.h"
#include "common/ImageFiles.h"
// #include "images/discmathcalc.xpm"
#include <wx/bitmap.h>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

using namespace ogdf;

//(*InternalHeadersPCH(BinaryTreeDrawingCanvas)
#include <wx/intl.h>
#include <wx/string.h>
//*)
//(*InternalHeaders(BinaryTreeDrawingCanvas)
//*)

//(*IdInit(BinaryTreeDrawingCanvas)
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_SCAN_TEXTCTRL = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_SCAN_RADIOBOX = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_CANVAS_SCAN_GO_BUTTON = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_CANVAS_CLOSE_BUTTON = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_CANVAS_SCAN_PANEL = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_SLIDER = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_PANEL = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_EXIT_MENUITEM = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM = wxNewId();
const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

const long BinaryTreeDrawingCanvas::ID_BINARY_TREE_DRAWING_CANVAS_STATUSBAR = wxNewId();
//*)

BEGIN_EVENT_TABLE (BinaryTreeDrawingCanvas,wxFrame)
	//(*EventTable(BinaryTreeDrawingCanvas)
	//*)
	EVT_CLOSE (BinaryTreeDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, BinaryTreeDrawingCanvas::OnQuit)
END_EVENT_TABLE()

BinaryTreeDrawingCanvas::BinaryTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : myParent(parent)
{
	//(*Initialize(BinaryTreeDrawingCanvas)
	wxMenu* binaryTreeDrawingCanvasFileMenu;
	wxFlexGridSizer* binaryTreeDrawingCanvasScanFlexGridSizer;
	wxFlexGridSizer* binaryTreeDrawingCanvasFlexGridSizer;
	wxMenuItem* binaryTreeDrawingCanvasExitMenuItem;
	wxMenuItem* binaryTreeDrawingCanvasSaveAsMenuItem;
	wxMenu* binaryTreeDrawingCanvasEditMenu;
	wxMenuItem* binaryTreeDrawingCanvasCopyImageMenuItem;
	wxMenuItem* binaryTreeDrawingCanvasPrintPreviewMenuItem;
	wxMenuItem* binaryTreeDrawingCanvasPrintMenuItem;
	wxMenuItem* binaryTreeDrawingCanvasPagsetupMenuItem;
	wxMenu* binaryTreeDrawingCanvasHelpMenu;
	wxMenuItem* binaryTreeDrawingCanvasHelpMenuItem;

	Create (parent, wxID_ANY, _ ("Binary Tree"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T ("id"));
	SetClientSize (wxSize (1024,768));
	{
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
		SetIcon (FrameIcon);
	}
	Move (wxDefaultPosition);
	binaryTreeDrawingCanvasPanel = new wxPanel (this, ID_BINARY_TREE_DRAWING_CANVAS_PANEL, wxPoint (144,216), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BINARY_TREE_DRAWING_CANVAS_PANEL"));
	binaryTreeDrawingCanvasFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	binaryTreeDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	binaryTreeDrawingCanvasFlexGridSizer->AddGrowableRow (0);

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties (binaryTreeDrawingCanvasPanel, binaryTreeDrawingCanvas,
	                                   binaryTreeDrawingCanvas_Manager);

	binaryTreeDrawingCanvasFlexGridSizer->Add (binaryTreeDrawingCanvas, 1, wxALL|wxEXPAND, 2);
	binaryTreeDrawingCanvasScanTextCtrl = new wxTextCtrl (binaryTreeDrawingCanvasPanel, ID_BINARY_TREE_SCAN_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize (-1,125), wxTE_MULTILINE, wxDefaultValidator, _T ("ID_BINARY_TREE_SCAN_TEXTCTRL"));
	binaryTreeDrawingCanvasFlexGridSizer->Add (binaryTreeDrawingCanvasScanTextCtrl, 1, wxALL|wxEXPAND, 2);
	binaryTreeDrawingCanvasScanPanel = new wxPanel (binaryTreeDrawingCanvasPanel, ID_BINARY_TREE_CANVAS_SCAN_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BINARY_TREE_CANVAS_SCAN_PANEL"));
	binaryTreeDrawingCanvasScanFlexGridSizer = new wxFlexGridSizer (1, 5, 0, 0);
	binaryTreeDrawingCanvasScanFlexGridSizer->AddGrowableCol (2);
	binaryTreeDrawingCanvasScanFlexGridSizer->AddGrowableCol (3);
	binaryTreeDrawingCanvasScanFlexGridSizer->AddGrowableCol (4);
	wxString __wxRadioBoxChoices_1[7] =
	{
		_ ("Inorder"),
		_ ("Postorder"),
		_ ("Preorder"),
		_ ("Level Order"),
		_ ("Reverse Inorder"),
		_ ("Reverse Postorder"),
		_ ("Reverse Preorder")
	};
	binaryTreeDrawingCanvasScanRadioBox = new wxRadioBox (binaryTreeDrawingCanvasScanPanel, ID_BINARY_TREE_DRAWING_CANVAS_SCAN_RADIOBOX, _ ("Scan"), wxDefaultPosition, wxDefaultSize, 7, __wxRadioBoxChoices_1, 3, wxRA_SPECIFY_ROWS, wxDefaultValidator, _T ("ID_BINARY_TREE_DRAWING_CANVAS_SCAN_RADIOBOX"));
	binaryTreeDrawingCanvasScanFlexGridSizer->Add (binaryTreeDrawingCanvasScanRadioBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	binaryTreeDrawingCanvasScanGoButton = new wxButton (binaryTreeDrawingCanvasScanPanel, ID_BINARY_TREE_CANVAS_SCAN_GO_BUTTON, _ ("Go"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BINARY_TREE_CANVAS_SCAN_GO_BUTTON"));
	binaryTreeDrawingCanvasScanGoButton->SetDefault();
	binaryTreeDrawingCanvasScanFlexGridSizer->Add (binaryTreeDrawingCanvasScanGoButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	binaryTreeDrawingCanvasScanFlexGridSizer->Add (0,0,1, wxALL|wxEXPAND, 2);
	binaryTreeDrawingCanvasCloseButton = new wxButton (binaryTreeDrawingCanvasScanPanel, ID_BINARY_TREE_CANVAS_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BINARY_TREE_CANVAS_CLOSE_BUTTON"));
	binaryTreeDrawingCanvasScanFlexGridSizer->Add (binaryTreeDrawingCanvasCloseButton, 10, wxALIGN_CENTER, 5);
	binaryTreeDrawingCanvasScanFlexGridSizer->Add (0,0,1, wxALL|wxEXPAND, 2);
	binaryTreeDrawingCanvasScanPanel->SetSizer (binaryTreeDrawingCanvasScanFlexGridSizer);
	binaryTreeDrawingCanvasScanFlexGridSizer->Fit (binaryTreeDrawingCanvasScanPanel);
	binaryTreeDrawingCanvasScanFlexGridSizer->SetSizeHints (binaryTreeDrawingCanvasScanPanel);
	binaryTreeDrawingCanvasFlexGridSizer->Add (binaryTreeDrawingCanvasScanPanel, 1, wxALL|wxEXPAND, 2);
	binaryTreeDrawingCanvasSlider = new wxSlider (binaryTreeDrawingCanvasPanel, ID_BINARY_TREE_DRAWING_CANVAS_SLIDER, 50, 2, 99, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BINARY_TREE_DRAWING_CANVAS_SLIDER"));
	binaryTreeDrawingCanvasSlider->SetToolTip (_ ("Set canvas scale"));
	binaryTreeDrawingCanvasFlexGridSizer->Add (binaryTreeDrawingCanvasSlider, 1, wxALL|wxEXPAND, 2);
	binaryTreeDrawingCanvasPanel->SetSizer (binaryTreeDrawingCanvasFlexGridSizer);
	binaryTreeDrawingCanvasFlexGridSizer->Fit (binaryTreeDrawingCanvasPanel);
	binaryTreeDrawingCanvasFlexGridSizer->SetSizeHints (binaryTreeDrawingCanvasPanel);

	binaryTreeDrawingCanvasMenuBar = new wxMenuBar();
	binaryTreeDrawingCanvasFileMenu = new wxMenu();
	binaryTreeDrawingCanvasMenuBar->Append(binaryTreeDrawingCanvasFileMenu, wxT("&File"));

	binaryTreeDrawingCanvasPrintMenuItem = new wxMenuItem (binaryTreeDrawingCanvasFileMenu, ID_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM, _ ("Print...\tCTRL-P"), wxEmptyString, wxITEM_NORMAL);
	binaryTreeDrawingCanvasFileMenu->Append (binaryTreeDrawingCanvasPrintMenuItem);
	/*	binaryTreeDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (binaryTreeDrawingCanvasFileMenu, ID_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM, _ ("Print Preview"), wxEmptyString, wxITEM_NORMAL);
		binaryTreeDrawingCanvasFileMenu->Append (binaryTreeDrawingCanvasPrintPreviewMenuItem);*/
	binaryTreeDrawingCanvasPagsetupMenuItem = new wxMenuItem (binaryTreeDrawingCanvasFileMenu, ID_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM, _ ("Page Setup..."), wxEmptyString, wxITEM_NORMAL);
	binaryTreeDrawingCanvasFileMenu->Append (binaryTreeDrawingCanvasPagsetupMenuItem);
	binaryTreeDrawingCanvasSaveAsMenuItem = new wxMenuItem (binaryTreeDrawingCanvasFileMenu, ID_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM, _ ("Save as..."), wxEmptyString, wxITEM_NORMAL);
	binaryTreeDrawingCanvasFileMenu->Append (binaryTreeDrawingCanvasSaveAsMenuItem);

	binaryTreeDrawingCanvasEditMenu = new wxMenu();
	binaryTreeDrawingCanvasCopyImageMenuItem = new wxMenuItem (binaryTreeDrawingCanvasEditMenu, ID_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM, _ ("Copy Image\tCTRL-C"), wxEmptyString, wxITEM_NORMAL);
	binaryTreeDrawingCanvasEditMenu->Append (binaryTreeDrawingCanvasCopyImageMenuItem);
	binaryTreeDrawingCanvasMenuBar->Append (binaryTreeDrawingCanvasEditMenu, _ ("&Edit"));

	binaryTreeDrawingCanvasHelpMenu = new wxMenu();
	binaryTreeDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	binaryTreeDrawingCanvasHelpMenuItem = new wxMenuItem (binaryTreeDrawingCanvasHelpMenu, ID_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	binaryTreeDrawingCanvasHelpMenu->Append (binaryTreeDrawingCanvasHelpMenuItem);
	binaryTreeDrawingCanvasMenuBar->Append (binaryTreeDrawingCanvasHelpMenu, _ ("&Help"));

	SetMenuBar (binaryTreeDrawingCanvasMenuBar);

	binaryTreeDrawingCanvasStatusBar = new wxStatusBar (this, ID_BINARY_TREE_DRAWING_CANVAS_STATUSBAR, 0, _T ("ID_BINARY_TREE_DRAWING_CANVAS_STATUSBAR"));
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	binaryTreeDrawingCanvasStatusBar->SetFieldsCount (1,__wxStatusBarWidths_1);
	binaryTreeDrawingCanvasStatusBar->SetStatusStyles (1,__wxStatusBarStyles_1);
	SetStatusBar (binaryTreeDrawingCanvasStatusBar);
	//*)

	Connect (ID_BINARY_TREE_CANVAS_SCAN_GO_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasScanGoButtonClick);
	Connect (ID_BINARY_TREE_CANVAS_CLOSE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasCloseButtonClick);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::handleMenuChoice);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::handleMenuChoice);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::handleMenuChoice);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &BinaryTreeDrawingCanvas::handleMenuChoice);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &BinaryTreeDrawingCanvas::handleMenuChoice);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_EXIT_MENUITEM,wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasExitMenuItemSelected);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_SLIDER,wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction) &BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasSliderCmdScroll);
	Connect (ID_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (BinaryTreeDrawingCanvas::OnBinaryTreeDrawingCanvasHelpMenuItemSelected));

	SetLabel (title);
	myBinaryTree = new BinaryTree (canvasTitle);
	treeTitle = title;
	dirty = true;
	haveAskedAboutSavingFile = false;
	binaryTreeDrawingCanvas->getDirty (&dirty);
	binaryTreeDrawingCanvas->setSliderAddress (binaryTreeDrawingCanvasSlider);
	binaryTreeDrawingCanvas->setGA (&myBinaryTree->GA);
	figureSaved = false;
	menuHandler = new MenuEventHandler (binaryTreeDrawingCanvas,
	                                    ID_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	                                    ID_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, undirtree);
}

BinaryTreeDrawingCanvas::~BinaryTreeDrawingCanvas()
{
	delete myBinaryTree;
}

bool BinaryTreeDrawingCanvas::ProcessEvent (wxEvent& event)
{
	try
	{
		return wxEvtHandler::ProcessEvent (event);
	}

	catch (const btrFileFormatException& bteffe)
	{
		wxString message, caption;

		message.append (wxString (bteffe.what().c_str(),wxConvUTF8));
		caption.append (wxT ("Error"));

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
		dialog.ShowModal();

		Destroy();
	}

	return true;
}

void BinaryTreeDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,1);
}

void BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasExitMenuItemSelected (wxCommandEvent& event)
{
	if (dirty && !figureSaved)
	{
		wxString message;
		int answer;

		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Binary tree figure"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxCANCEL)
			return;

		if (answer == wxYES)
		{
			if (!menuHandler->handleSaveAs(1))
				return;
		}
	}

	Destroy();
}

void BinaryTreeDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
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

void BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasScanGoButtonClick (wxCommandEvent& event)
{
	int pos;
	wxVector<string> scanList;

	pos = binaryTreeDrawingCanvasScanRadioBox->GetSelection();

	switch (pos)
	{
	case 0:
		myBinaryTree->inorder (scanList);
		break;

	case 1:
		myBinaryTree->postorder (scanList);
		break;

	case 2:
		myBinaryTree->preorder (scanList);
		break;

	case 3:
		myBinaryTree->levelorder (scanList);
		break;

	case 4:
		myBinaryTree->reverseInorder (scanList);
		break;

	case 5:
		myBinaryTree->reversePostorder (scanList);
		break;

	case 6:
		myBinaryTree->reversePreorder (scanList);
		break;
	}

	for (unsigned int i=0; i < scanList.size(); i++)
	{
		binaryTreeDrawingCanvasScanTextCtrl->AppendText (wxString (scanList[i].c_str(),wxConvUTF8));
		binaryTreeDrawingCanvasScanTextCtrl->AppendText (wxT ("  "));
	}

	binaryTreeDrawingCanvasScanTextCtrl->AppendText (wxT ("\n"));
}

void BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasCloseButtonClick (wxCommandEvent& event)
{
	if (dirty && !figureSaved)
	{
		wxString message;
		int answer;

		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Binary tree figure"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxCANCEL)
			return;

		if (answer == wxYES)
		{
			if (!menuHandler->handleSaveAs(1))
				return;
		}
	}

	Destroy();
}

void BinaryTreeDrawingCanvas::OnbinaryTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	binaryTreeDrawingCanvas->SetScale (double (binaryTreeDrawingCanvasSlider->GetValue()) /50);
	binaryTreeDrawingCanvas->Refresh (false);
}

void BinaryTreeDrawingCanvas::makeTreeFromLines (const wxVector<wxString>& bTreeData)
{
	myBinaryTree->buildBinaryTree_init();

	for (unsigned int i=0; i < bTreeData.size(); i++)
		myBinaryTree->buildBTR_node (bTreeData[i]);

	myBinaryTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw();
}

void BinaryTreeDrawingCanvas::makeTreeFromBTRFile (const wxString& btrFileName)
{
	wxTextFile btrInput (btrFileName);
	wxString line;

	if (!btrInput.Open())
	{
		string errMsg ("Cannot open ");

		errMsg += string ( (const char *) btrFileName.c_str());

		throw btrFileFormatException (errMsg);
	}

	myBinaryTree->buildBinaryTree_init();

	for (line = btrInput.GetFirstLine(); !btrInput.Eof(); line = btrInput.GetNextLine())
		myBinaryTree->buildBTR_node (line);

	myBinaryTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw();
}

void BinaryTreeDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	myBinaryTree->buildBinaryTree_init();
	myBinaryTree->buildBinaryTreeGML (gmlFileName);
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw();
	figureSaved = true;
}

bool BinaryTreeDrawingCanvas::closeFrame()
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Binary tree figure"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
		{
			if (!menuHandler->handleSaveAs(1))
				return false;
		}
		else if (answer == wxCANCEL)
			return false;
	}

	return true;
}

void BinaryTreeDrawingCanvas::OnQuit (wxCommandEvent& event)
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

void BinaryTreeDrawingCanvas::OnBinaryTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
