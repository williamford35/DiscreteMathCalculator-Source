/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DiscreteMathCalculatorMain.h"
#include "trees/heap/Heap.h"
#include "common/report.h"
#include <wx/textfile.h>
#include "trees/heap/HeapDrawingCanvas.h"
#include "common/ImageFiles.h"
// #include "images/discmathcalc.xpm"
#include <wx/bitmap.h>

#include <string>

using namespace std;
using namespace ogdf;

//(*InternalHeadersPCH(HeapDrawingCanvas)
#include <wx/intl.h>
#include <wx/string.h>
//*)
//(*InternalHeaders(HeapDrawingCanvas)
//*)

const long HeapDrawingCanvas::ID_HEAP_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (HeapDrawingCanvas,wxFrame)
	EVT_CLOSE (HeapDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, HeapDrawingCanvas::OnQuit)
END_EVENT_TABLE()

HeapDrawingCanvas::HeapDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, const wxVector<int>& data, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), inputData (data), heapTitle (canvasTitle), myParent(parent)
{

	wxMenu* heapDrawingCanvasHelpMenu;
	wxMenuItem* heapDrawingCanvasHelpMenuItem;

	wxIcon FrameIcon;
	FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
	SetIcon (FrameIcon);

	this->SetSizeHints (wxDefaultSize, wxDefaultSize);

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties (this, heapDrawingCanvas,
	                                   heapDrawingCanvas_Manager);
	wxFlexGridSizer* heapDrawingCanvasFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	heapDrawingCanvasFlexGridSizer->SetFlexibleDirection (wxBOTH);
	heapDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	heapDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	heapDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (heapDrawingCanvasFlexGridSizer);

	heapDrawingCanvasFlexGridSizer->Add (heapDrawingCanvas, 0, wxALL|wxEXPAND, 5);

	heapControlPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (1024,-1), wxTAB_TRAVERSAL);

	heapDrawingCanvasFlexGridSizer->Add (heapControlPanel, 0, wxALL, 5);

	wxFlexGridSizer* heapControlPanelFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	heapControlPanelFlexGridSizer->SetFlexibleDirection (wxBOTH);
	heapControlPanelFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	heapControlPanel->SetSizer (heapControlPanelFlexGridSizer);

	heapContentsPanel = new wxPanel (heapControlPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	heapControlPanelFlexGridSizer->Add (heapContentsPanel, 0, wxALL, 5);

	wxBoxSizer* heapContentsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	heapContentsPanel->SetSizer (heapContentsBoxSizer);

	correspondingArrayStaticText = new wxStaticText (heapContentsPanel, wxID_ANY, _ ("Corresponding Array"), wxDefaultPosition, wxSize (-1,-1), 0);

	heapContentsBoxSizer->Add (correspondingArrayStaticText, 1, wxALL, 1);

	correspondingArrayTextCtrl = new wxTextCtrl (heapContentsPanel, wxID_ANY, wxT (""), wxDefaultPosition,
	      wxSize (50,-1), 0);

	heapContentsBoxSizer->Add (correspondingArrayTextCtrl, 6, wxALL, 1);
	heapPushPopPanel = new wxPanel (heapControlPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	heapControlPanelFlexGridSizer->Add (heapPushPopPanel, 0, wxALL, 5);

	wxBoxSizer* heapPushPopBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	heapPushPopPanel->SetSizer (heapPushPopBoxSizer);

	heapPushPopInputTextCtrl = new wxTextCtrl (heapPushPopPanel, wxID_ANY, wxT (""), wxDefaultPosition,
	      wxSize (50,-1), 0);

	heapPushPopBoxSizer->Add (heapPushPopInputTextCtrl, 0, wxALL, 5);

	heapPushButton = new wxButton (heapPushPopPanel, wxID_ANY, _ ("Push"), wxDefaultPosition, wxSize (-1,-1), 0);

	heapPushPopBoxSizer->Add (heapPushButton, 0, wxALL, 5);

	heapPopButton = new wxButton (heapPushPopPanel, wxID_ANY, _ ("Pop"), wxDefaultPosition, wxSize (-1,-1), 0);

	heapPushPopBoxSizer->Add (heapPopButton, 0, wxALL, 5);

	heapDrawingCanvasSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);

	heapDrawingCanvasFlexGridSizer->Add (heapDrawingCanvasSlider, 0, wxALL|wxEXPAND, 1);

	heapDrawingCanvasClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1, -1), wxTAB_TRAVERSAL);

	heapDrawingCanvasFlexGridSizer->Add (heapDrawingCanvasClosePanel, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* heapDrawingCanvasCloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	heapDrawingCanvasClosePanel->SetSizer (heapDrawingCanvasCloseBoxSizer);

	heapDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);

	heapDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);

	heapDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);

	heapDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);

	heapDrawingCanvasCloseButton = new wxButton (heapDrawingCanvasClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1, -1), 0);
	heapDrawingCanvasCloseButton->SetDefault();

	heapDrawingCanvasCloseBoxSizer->Add (heapDrawingCanvasCloseButton, 0, wxALL, 5);

	heapDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (heapDrawingCanvasMenuBar);

	heapDrawingCanvasFileMenu = new wxMenu();
	heapDrawingCanvasMenuBar->Append (heapDrawingCanvasFileMenu, _ ("File"));

	heapDrawingCanvasPrintMenuItem = new wxMenuItem (heapDrawingCanvasFileMenu, wxID_ANY, _ ("Print...\t*P"), wxT (""), wxITEM_NORMAL);
	heapDrawingCanvasFileMenu->Append (heapDrawingCanvasPrintMenuItem);

	/*heapDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (heapDrawingCanvasFileMenu, wxID_ANY, _ ("Print Preview"), wxT (""), wxITEM_NORMAL);
	heapDrawingCanvasFileMenu->Append (heapDrawingCanvasPrintPreviewMenuItem);*/

	heapDrawingCanvasPageSetupMenuItem = new wxMenuItem (heapDrawingCanvasFileMenu, wxID_ANY, _ ("Page Setup..."), wxT (""), wxITEM_NORMAL);
	heapDrawingCanvasFileMenu->Append (heapDrawingCanvasPageSetupMenuItem);

	heapDrawingCanvasSaveAsMenuItem = new wxMenuItem (heapDrawingCanvasFileMenu, wxID_ANY, _ ("Save as..."), wxT (""), wxITEM_NORMAL);
	heapDrawingCanvasFileMenu->Append (heapDrawingCanvasSaveAsMenuItem);

	heapDrawingCanvasEditMenu = new wxMenu();
	heapDrawingCanvasMenuBar->Append (heapDrawingCanvasEditMenu, _ ("Edit"));

	heapDrawingCanvasCopyImageMenuItem = new wxMenuItem (heapDrawingCanvasEditMenu, wxID_ANY, _ ("Copy Image\t*C"), wxT (""), wxITEM_NORMAL);
	heapDrawingCanvasEditMenu->Append (heapDrawingCanvasCopyImageMenuItem);

	heapDrawingCanvasHelpMenu = new wxMenu();
	heapDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	heapDrawingCanvasHelpMenuItem = new wxMenuItem (heapDrawingCanvasHelpMenu, ID_HEAP_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	heapDrawingCanvasHelpMenu->Append (heapDrawingCanvasHelpMenuItem);
	heapDrawingCanvasMenuBar->Append (heapDrawingCanvasHelpMenu, _ ("&Help"));

	ID_HEAP_DRAWING_CANVAS_PRINT_MENUITEM = heapDrawingCanvasPrintMenuItem->GetId();
	ID_HEAP_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = -1;
	ID_HEAP_DRAWING_CANVAS_PAGESETUP_MENUITEM = heapDrawingCanvasPageSetupMenuItem->GetId();
	ID_HEAP_DRAWING_CANVAS_SAVE_AS_MENUITEM = heapDrawingCanvasSaveAsMenuItem->GetId();
	ID_HEAP_DRAWING_CANVAS_COPY_IMAGE_MENUITEM = heapDrawingCanvasCopyImageMenuItem->GetId();

	SetLabel (title);
	myBinaryTree = new BinaryTree (canvasTitle);
	heapDrawingCanvas->setSliderAddress (heapDrawingCanvasSlider);
	heapDrawingCanvas->setGA (&myBinaryTree->GA);
	dirty = true;
	figureSaved = false;
	haveAskedAboutSavingFile = false;
	heapDrawingCanvas->getDirty (&dirty);

	menuHandler = new MenuEventHandler (heapDrawingCanvas,
	                                    ID_HEAP_DRAWING_CANVAS_PRINT_MENUITEM,
	                                    ID_HEAP_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_HEAP_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_HEAP_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_HEAP_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, undirtree);
	Centre (wxBOTH);

	// Connect events
	heapDrawingCanvasSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (HeapDrawingCanvas::OnheapDrawingCanvasSliderCmdScroll), NULL, this);
	heapDrawingCanvasCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HeapDrawingCanvas::OnHeapCanvasCloseButtonClick), NULL, this);
	this->Connect (heapDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	Connect (ID_HEAP_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (HeapDrawingCanvas::OnHeapDrawingCanvasHelpMenuItemSelected));
	// this->Connect (heapDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (heapDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (heapDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (heapDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (heapPushButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HeapDrawingCanvas::OnheapDrawingCanvasPushButtonClick), NULL, this);
	this->Connect (heapPopButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HeapDrawingCanvas::OnheapDrawingCanvasPopButtonClick), NULL, this);
}

HeapDrawingCanvas::~HeapDrawingCanvas()
{
	heapDrawingCanvasSlider->Disconnect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (HeapDrawingCanvas::OnheapDrawingCanvasSliderCmdScroll), NULL, this);
	heapDrawingCanvasCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HeapDrawingCanvas::OnHeapCanvasCloseButtonClick), NULL, this);
	this->Disconnect (heapDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	// this->Disconnect (heapDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (heapDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (heapDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (heapDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (HeapDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (heapPushButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HeapDrawingCanvas::OnheapDrawingCanvasPushButtonClick), NULL, this);
	this->Disconnect (heapPopButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (HeapDrawingCanvas::OnheapDrawingCanvasPopButtonClick), NULL, this);
}

void HeapDrawingCanvas::OnheapDrawingCanvasPushButtonClick (wxCommandEvent& event)
{
	update (PUSH);
}

void HeapDrawingCanvas::OnheapDrawingCanvasPopButtonClick (wxCommandEvent& event)
{
	update (POP);
}

void HeapDrawingCanvas::OnHeapCanvasCloseButtonClick (wxCommandEvent& event)
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		if (heapTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), heapTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Heap figure"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
			menuHandler->handleSaveAs(6);
		else if (answer == wxCANCEL)
			return;
	}

	Destroy();
}

void HeapDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,6);
}

void HeapDrawingCanvas::OnheapDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	heapDrawingCanvas->SetScale (double (heapDrawingCanvasSlider->GetValue()) /50);
	heapDrawingCanvas->Refresh (false);
}

void HeapDrawingCanvas::buildHeap (HeapType type)
{
	heapType = type;

	if (heapType == MINIMUM)
		makeHeap (inputData, less<int>());
	else
		makeHeap (inputData, greater<int>());

	myBinaryTree->buildBinaryTree_init();
	buildMyHeapTree (0);
	myBinaryTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw();
	outputHeapArray();
}

void HeapDrawingCanvas::buildMyHeapTree (int pos)
{
	wxString line;

	if ( (unsigned int) pos >= inputData.size())
		return;

	buildMyHeapTree (2*pos + 1);
	buildMyHeapTree (2*pos + 2);

	line.Printf (wxT ("%d "), inputData[pos]);

	if ( (unsigned int) (2*pos+1) < inputData.size())
		line += wxT ("1 ");
	else
		line += wxT ("0 ");

	if ( (unsigned int) (2*pos+2) < inputData.size())
		line += wxT ("1");
	else
		line += wxT ("0");

	myBinaryTree->buildBTR_node (line);
}

void HeapDrawingCanvas::outputHeapArray()
{
	wxString heapEntry;

	correspondingArrayTextCtrl->ChangeValue (wxT (""));

	for (unsigned int i=0; i < inputData.size(); i++)
	{
		heapEntry.Printf (wxT ("%6d"), inputData[i]);
		correspondingArrayTextCtrl->AppendText (heapEntry);
	}
}

void HeapDrawingCanvas::update (UpdateType updateType)
{
	wxString strUpdateValue = heapPushPopInputTextCtrl->GetValue();
	long updateValue;

	if (updateType == PUSH && !strUpdateValue.ToLong (&updateValue))
	{
		reportError (wxT ("Your update value is not a valid integer."),"Note");
		return;
	}

	dirty = true;
	myBinaryTree->clear();
	myBinaryTree->buildBinaryTree_init();
	heapDrawingCanvas->SelectAll();
	heapDrawingCanvas->Cut();

	switch (updateType)
	{
	case PUSH:
		inputData.push_back ( (int) updateValue);

		if (heapType == MINIMUM)
			pushHeap (inputData, inputData.size(), less<int>());
		else
			pushHeap (inputData, inputData.size(), greater<int>());

		break;

	case    POP:
		inputData[0] = inputData[inputData.size()-1];
		inputData.pop_back();

		if (heapType == MINIMUM)
			popHeap (inputData, inputData.size(), less<int>());
		else
			popHeap (inputData, inputData.size(), greater<int>());

		break;
	}

	buildMyHeapTree (0);
	myBinaryTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw();
	outputHeapArray();
	heapPushPopInputTextCtrl->ChangeValue (wxT (""));
}

void HeapDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	wxTextFile treeFile (gmlFileName);

	myBinaryTree->buildBinaryTree_init();
	myBinaryTree->buildBinaryTreeGML (gmlFileName);

	if (!recreateHeap())
		return;

	heapTitle = myBinaryTree->getTreeLabel();
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw();
	figureSaved = true;
}

bool HeapDrawingCanvas::recreateHeap()
{
	wxVector<string> v;  // inputData

	myBinaryTree->levelorder (v);

	inputData.clear();

	unsigned int n = v.size();

	for (unsigned int i=0; i < n; i++)
		inputData.push_back (atoi (v[i].c_str()));

	if (isHeap (inputData, n, less_equal<int>()))
		heapType = MINIMUM;
	else if (isHeap (inputData, n, greater_equal<int>()))
		heapType = MAXIMUM;
	else
	{
		wxString message, caption;

		message.append (wxT ("This figure is not a heap."));
		caption.append (wxT ("Warning"));

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return false;
	}

	outputHeapArray();
	return true;
}

bool HeapDrawingCanvas::closeFrame()
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		message.Printf (wxT ("Save figure?"));
		answer = wxMessageBox (message, wxT ("Heap figure"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
		{
			if (!menuHandler->handleSaveAs(2))
				return false;
		}
		else if (answer == wxCANCEL)
			return false;
	}

	return true;
}

void HeapDrawingCanvas::OnheapDrawingCanvasExitMenuItemSelected (wxCommandEvent& event)
{
	if (closeFrame())
		Close(true);
}

void HeapDrawingCanvas::OnQuit (wxCommandEvent& event)
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

void HeapDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
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

void HeapDrawingCanvas::OnHeapDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
