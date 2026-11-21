/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/general/GeneralTreeDrawingCanvas.h"
#include "common/ImageFiles.h"
#include "DiscreteMathCalculatorMain.h"

#include <wx/intl.h>
#include <wx/string.h>

#include "trees/btrFileFormatException.h"
#include "trees/general/EquivalentBinaryTree.h"
// #include "images/discmathcalc.xpm"
#include <wx/bitmap.h>

const long GeneralTreeDrawingCanvas::ID_GENERAL_TREE_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (GeneralTreeDrawingCanvas,wxFrame)
	EVT_CLOSE (GeneralTreeDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, GeneralTreeDrawingCanvas::OnQuit)
END_EVENT_TABLE()

GeneralTreeDrawingCanvas::GeneralTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), treeTitle (canvasTitle), myParent (parent)
{
	wxMenu* generalTreeDrawingCanvasHelpMenu;
	wxMenuItem* generalTreeDrawingCanvasHelpMenuItem;

	wxFlexGridSizer* generalTreeDrawingCanvasFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	generalTreeDrawingCanvasFlexGridSizer->SetFlexibleDirection (wxBOTH);
	generalTreeDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	generalTreeDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	generalTreeDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (generalTreeDrawingCanvasFlexGridSizer);

	wxIcon FrameIcon;
	FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
	SetIcon (FrameIcon);
	SetClientSize (wxSize (1024,768));

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties ( (wxPanel *) this, generalTreeDrawingCanvas,
	                                    generalTreeDrawingCanvas_Manager);

	generalTreeDrawingCanvas->SetSize (-1,450);
	generalTreeDrawingCanvasFlexGridSizer->Add (generalTreeDrawingCanvas, 1, wxALL|wxEXPAND, 1);

	generalTreeDrawingCanvasPrimaryPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	generalTreeDrawingCanvasFlexGridSizer->Add (generalTreeDrawingCanvasPrimaryPanel, 2, wxALL|wxEXPAND, 1);

	wxFlexGridSizer* generalTreeDrawingCanvasPrimaryPanelFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->SetFlexibleDirection (wxBOTH);
	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->AddGrowableCol (0);
	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->AddGrowableRow (0);
	generalTreeDrawingCanvasPrimaryPanel->SetSizer (generalTreeDrawingCanvasPrimaryPanelFlexGridSizer);

	generalTreeDrawingCanvasTraversalPanel = new wxPanel (generalTreeDrawingCanvasPrimaryPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->Add (generalTreeDrawingCanvasTraversalPanel, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* generalTreeDrawingCanvasTraversalBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	generalTreeDrawingCanvasTraversalPanel->SetSizer (generalTreeDrawingCanvasTraversalBoxSizer);

	generalTreeDrawingCanvasTraversalStaticText = new wxStaticText (generalTreeDrawingCanvasTraversalPanel, wxID_ANY, _ ("Traversal Output"), wxDefaultPosition, wxSize (-1,-1), wxALIGN_CENTRE|wxALIGN_RIGHT);

	generalTreeDrawingCanvasTraversalBoxSizer->Add (generalTreeDrawingCanvasTraversalStaticText, 1, wxALL|wxALIGN_CENTER, 1);

	generalTreeDrawingCanvasTraversalTextCtrl = new wxTextCtrl (generalTreeDrawingCanvasTraversalPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,65), wxTE_MULTILINE|wxHSCROLL);

	generalTreeDrawingCanvasTraversalBoxSizer->Add (generalTreeDrawingCanvasTraversalTextCtrl, 5, wxALL, 1);

	generalTreeDrawingCanvasControlPanel = new wxPanel (generalTreeDrawingCanvasPrimaryPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->Add (generalTreeDrawingCanvasControlPanel, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* generalTreeDrawingCanvasControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	generalTreeDrawingCanvasControlPanel->SetSizer (generalTreeDrawingCanvasControlBoxSizer);

	wxArrayString generalTreeDrawingCanvasTraversalRadioBoxArr;
	generalTreeDrawingCanvasTraversalRadioBoxArr.Add (_ ("Depth-first"));
	generalTreeDrawingCanvasTraversalRadioBoxArr.Add (_ ("Breadth-first"));
	generalTreeDrawingCanvasTraversalRadioBox = new wxRadioBox (generalTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Traversal"), wxDefaultPosition, wxSize (-1,80), generalTreeDrawingCanvasTraversalRadioBoxArr, 1, 0);
	generalTreeDrawingCanvasTraversalRadioBox->SetSelection (0);

	generalTreeDrawingCanvasControlBoxSizer->Add (generalTreeDrawingCanvasTraversalRadioBox, 1, wxALL, 1);

	generalTreeDrawingCanvasGoButton = new wxButton (generalTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Go"), wxDefaultPosition, wxSize (-1,-1), 0);

	generalTreeDrawingCanvasControlBoxSizer->Add (generalTreeDrawingCanvasGoButton, 1, wxALL|wxALIGN_CENTER, 1);

	generalTreeDrawingCanvasControlBoxSizer->Add (0, 0, 1, wxALL, 1);

	generalTreeDrawingCanvasControlBoxSizer->Add (0, 0, 1, wxALL, 1);

	generalTreeDrawingCanvasBuildEquivBinaryTreeButton = new wxButton (generalTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Build Equivalent Binary Tree"), wxDefaultPosition, wxSize (-1,-1), 0);

	generalTreeDrawingCanvasControlBoxSizer->Add (generalTreeDrawingCanvasBuildEquivBinaryTreeButton, 1, wxALL|wxALIGN_CENTER, 1);

	generalTreeDrawingCanvasControlBoxSizer->Add (0, 0, 1, wxALL, 1);

	generalTreeDrawingCanvasControlBoxSizer->Add (0, 0, 1, wxALL, 1);

	generalTreeDrawingCanvasSlider = new wxSlider (generalTreeDrawingCanvasPrimaryPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);

	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->Add (generalTreeDrawingCanvasSlider, 1, wxALL|wxEXPAND, 1);

	generalTreeDrawingCanvasClosePanel = new wxPanel (generalTreeDrawingCanvasPrimaryPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	generalTreeDrawingCanvasPrimaryPanelFlexGridSizer->Add (generalTreeDrawingCanvasClosePanel, 1, wxALL|wxEXPAND, 1);

	wxBoxSizer* generalTreeDrawingCanvasCloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	generalTreeDrawingCanvasClosePanel->SetSizer (generalTreeDrawingCanvasCloseBoxSizer);

	generalTreeDrawingCanvasCloseBoxSizer->Add (0, 0, 7, wxALL, 1);

	generalTreeDrawingCanvasCloseButton = new wxButton (generalTreeDrawingCanvasClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);

	generalTreeDrawingCanvasCloseBoxSizer->Add (generalTreeDrawingCanvasCloseButton, 1, wxALL, 1);

	generalTreeDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxALL, 1);

	generalTreeDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (generalTreeDrawingCanvasMenuBar);

	generalTreeDrawingCanvasFileMenu = new wxMenu();
	generalTreeDrawingCanvasMenuBar->Append(generalTreeDrawingCanvasFileMenu, wxT("&File"));

	generalTreeDrawingCanvasPrintMenuItem = new wxMenuItem (generalTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Print...\t*P"), wxT (""), wxITEM_NORMAL);
	generalTreeDrawingCanvasFileMenu->Append (generalTreeDrawingCanvasPrintMenuItem);

	generalTreeDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (generalTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Print Preview"), wxT (""), wxITEM_NORMAL);
	generalTreeDrawingCanvasFileMenu->Append (generalTreeDrawingCanvasPrintPreviewMenuItem);

	generalTreeDrawingCanvasPageSetupMenuItem = new wxMenuItem (generalTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Page Setup..."), wxT (""), wxITEM_NORMAL);
	generalTreeDrawingCanvasFileMenu->Append (generalTreeDrawingCanvasPageSetupMenuItem);

	generalTreeDrawingCanvasSaveAsMenuItem = new wxMenuItem (generalTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Save as..."), wxT (""), wxITEM_NORMAL);
	generalTreeDrawingCanvasFileMenu->Append (generalTreeDrawingCanvasSaveAsMenuItem);

	generalTreeDrawingCanvasEditMenu = new wxMenu();
	generalTreeDrawingCanvasMenuBar->Append (generalTreeDrawingCanvasEditMenu, _ ("Edit"));

	generalTreeDrawingCanvasCopyImageMenuItem = new wxMenuItem (generalTreeDrawingCanvasEditMenu, wxID_ANY, _ ("Copy Image\t*C"), wxT (""), wxITEM_NORMAL);
	generalTreeDrawingCanvasEditMenu->Append (generalTreeDrawingCanvasCopyImageMenuItem);

	generalTreeDrawingCanvasHelpMenu = new wxMenu();
	generalTreeDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	generalTreeDrawingCanvasHelpMenuItem = new wxMenuItem (generalTreeDrawingCanvasHelpMenu, ID_GENERAL_TREE_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	generalTreeDrawingCanvasHelpMenu->Append (generalTreeDrawingCanvasHelpMenuItem);
	generalTreeDrawingCanvasMenuBar->Append (generalTreeDrawingCanvasHelpMenu, _ ("&Help"));

	SetName (wxT ("GeneralTreeDrawingCanvas"));
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

	ID_GENERAL_TREE_DRAWING_CANVAS_PRINT_MENUITEM = generalTreeDrawingCanvasPrintMenuItem->GetId();
	ID_GENERAL_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = generalTreeDrawingCanvasPrintPreviewMenuItem->GetId();
	ID_GENERAL_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM = generalTreeDrawingCanvasPageSetupMenuItem->GetId();
	ID_GENERAL_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM = generalTreeDrawingCanvasSaveAsMenuItem->GetId();
	ID_GENERAL_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM = generalTreeDrawingCanvasCopyImageMenuItem->GetId();

	// Connect events
	generalTreeDrawingCanvasGoButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasGoButtonPress), NULL, this);
	generalTreeDrawingCanvasBuildEquivBinaryTreeButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasBuildEquivBinaryTreeButtonPress), NULL, this);
	generalTreeDrawingCanvasCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasCloseButtonPress), NULL, this);
	generalTreeDrawingCanvasSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (GeneralTreeDrawingCanvas::OngeneralTreeDrawingCanvasSliderCmdScroll), NULL, this);
	Connect (ID_GENERAL_TREE_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (GeneralTreeDrawingCanvas::OngeneralTreeDrawingCanvasHelpMenuItemSelected));
	this->Connect (generalTreeDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (generalTreeDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (generalTreeDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (generalTreeDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (generalTreeDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);

	SetLabel (title);
	generalTreeDrawingCanvas->setGA (&myGeneralTree->GA);
	dirty = true;
	figureSaved = false;
	haveAskedAboutSavingFile = false;
	haveClosedParent = false;
	generalTreeDrawingCanvas->getDirty (&dirty);
	myGeneralTree = new GeneralTree (generalTreeDrawingCanvas, canvasTitle);
	generalTreeDrawingCanvas->setSliderAddress (generalTreeDrawingCanvasSlider);
	generalTreeDrawingCanvas->setGA (&myGeneralTree->GA);
	menuHandler = new MenuEventHandler (generalTreeDrawingCanvas,
	                                    ID_GENERAL_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	                                    ID_GENERAL_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_GENERAL_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_GENERAL_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_GENERAL_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, undirtree);
}

GeneralTreeDrawingCanvas::~GeneralTreeDrawingCanvas()
{
	generalTreeDrawingCanvasGoButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasGoButtonPress), NULL, this);
	generalTreeDrawingCanvasBuildEquivBinaryTreeButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasBuildEquivBinaryTreeButtonPress), NULL, this);
	generalTreeDrawingCanvasCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasCloseButtonPress), NULL, this);
	this->Disconnect (generalTreeDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (generalTreeDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (generalTreeDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (generalTreeDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (generalTreeDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (GeneralTreeDrawingCanvas::handleMenuChoice), NULL, this);
	generalTreeDrawingCanvasSlider->Disconnect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (GeneralTreeDrawingCanvas::OngeneralTreeDrawingCanvasSliderCmdScroll), NULL, this);
}

bool GeneralTreeDrawingCanvas::ProcessEvent (wxEvent& event)
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

void GeneralTreeDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,1);
}

void GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasBuildEquivBinaryTreeButtonPress (wxCommandEvent& event)
{
	EquivalentBinaryTree *equivalentBinaryTree;
	wxString title;

	if (treeTitle.Length() == 0)
		title= wxT("Equivalent Binary Tree");
	else
		title = treeTitle + wxT (" (Equivalent Binary Tree)");

	equivalentBinaryTree = new EquivalentBinaryTree (myParent, treeTitle, & (myGeneralTree->G),
	      & (myGeneralTree->GA), myGeneralTree->getRoot(), wxID_ANY, title);
	equivalentBinaryTree->buildEquivalentTree();

	equivalentBinaryTree->Show();
	wxTheApp->SetTopWindow (equivalentBinaryTree);
}

void GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasCloseButtonPress (wxCommandEvent& event)
{
	if (dirty && !figureSaved)
	{
		wxString message;
		int answer;

		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Discrete Mathematics Calculator"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxCANCEL)
			return;

		if (answer == wxYES)
		{
			if (!menuHandler->handleSaveAs(2))
				return;
		}
	}

	Destroy();
}

void GeneralTreeDrawingCanvas::OnGeneralTreeDrawingCanvasGoButtonPress (wxCommandEvent& event)
{
	vector<string> scanOutput;
	wxString nodeLabel;
	int pos;

	pos = generalTreeDrawingCanvasTraversalRadioBox->GetSelection();

	if (pos == 0)
		myGeneralTree->depthFirstSearch (scanOutput);
	else
		myGeneralTree->breadthFirstSearch (scanOutput);

	for (unsigned int i=0; i < scanOutput.size(); i++)
	{
		nodeLabel.Printf (wxT ("%s  "), scanOutput[i].c_str());
		generalTreeDrawingCanvasTraversalTextCtrl->AppendText (nodeLabel);
	}

	generalTreeDrawingCanvasTraversalTextCtrl->AppendText (wxT ("\n"));
	generalTreeDrawingCanvasTraversalTextCtrl->SetInsertionPoint (0);
}

void GeneralTreeDrawingCanvas::OngeneralTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	generalTreeDrawingCanvas->SetScale (double (generalTreeDrawingCanvasSlider->GetValue()) /50);
	generalTreeDrawingCanvas->Refresh (false);
}

void GeneralTreeDrawingCanvas::makeTreeFromLines (const wxVector<wxString>& gTreeData)
{
	myGeneralTree->buildTreeGTR_init();

	for (unsigned int i=0; i < gTreeData.size(); i++)
		myGeneralTree->buildGTR_node (gTreeData[i]);

	myGeneralTree->buildTreeGTR_end();
	drawOnCanvas->setGraphProperties (& (myGeneralTree->G), & (myGeneralTree->GA), myGeneralTree->getRoot());
	drawOnCanvas->draw();
}

void GeneralTreeDrawingCanvas::makeTreeFromGTRFile (const wxString& gtrFileName)
{
	myGeneralTree->buildTreeGTR_init();
	myGeneralTree->buildTreeGTR (gtrFileName);
	drawOnCanvas->setGraphProperties (& (myGeneralTree->G), & (myGeneralTree->GA), myGeneralTree->getRoot());
	drawOnCanvas->draw();
}

void GeneralTreeDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	myGeneralTree->buildTreeGTR_init();
	myGeneralTree->buildGeneralTreeGML (gmlFileName);
	node t = myGeneralTree->getRoot();
	drawOnCanvas->setGraphProperties (& (myGeneralTree->G), & (myGeneralTree->GA), myGeneralTree->getRoot());
	drawOnCanvas->draw();
	figureSaved = true;
}

bool GeneralTreeDrawingCanvas::closeFrame()
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("General tree figure"),
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

void GeneralTreeDrawingCanvas::OnQuit (wxCommandEvent& event)
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

void GeneralTreeDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
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

void GeneralTreeDrawingCanvas::OngeneralTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
