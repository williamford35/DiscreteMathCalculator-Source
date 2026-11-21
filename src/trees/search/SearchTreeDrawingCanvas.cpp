/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/search/SearchTreeDrawingCanvas.h"
#include "common/ImageFiles.h"
#include "common/report.h"
// #include "images/discmathcalc.xpm"
#include "DiscreteMathCalculatorApp.h"
#include "DiscreteMathCalculatorMain.h"
#include <wx/bitmap.h>
#include <wx/image.h>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

using namespace ogdf;

#include <wx/intl.h>
#include <wx/string.h>

#include <iostream>
using namespace std;

BEGIN_EVENT_TABLE (SearchTreeDrawingCanvas,wxFrame)
	EVT_CLOSE (SearchTreeDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, SearchTreeDrawingCanvas::OnQuit)
END_EVENT_TABLE()

const long SearchTreeDrawingCanvas::ID_SEARCH_TREE_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

SearchTreeDrawingCanvas::SearchTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, BalanceType t, const wxVector<int>& input, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), searchTreeType (t), windowTitle (canvasTitle), myParent(parent)
{
	wxMenuItem* searchTreeDrawingCanvasHelpMenuItem;
	wxMenu* searchTreeDrawingCanvasHelpMenu;

	wxIcon FrameIcon;
	DrawingCanvas::GMLTypeLabel gmlLabelType;
	FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
	SetIcon (FrameIcon);

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties ( (wxPanel *) this, searchTreeDrawingCanvas,
	                                    searchTreeDrawingCanvas_Manager);
	// searchTreeDrawingCanvas->SetSize (-1,-1);

	if (searchTreeType == AVL)
		gmlLabelType = DrawingCanvas::AVL;
	else if (searchTreeType == RedBlack)
		gmlLabelType = DrawingCanvas::RedBlack;
	else
		gmlLabelType = DrawingCanvas::None;

	searchTreeDrawingCanvas->setGMLTreeTypeLabel (gmlLabelType);


	wxFlexGridSizer* searchTreeDrawingCanvasFlexGridSizer = new wxFlexGridSizer (5, 1, 0, 0);
	searchTreeDrawingCanvasFlexGridSizer->SetFlexibleDirection (wxBOTH);
	searchTreeDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	searchTreeDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	searchTreeDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (searchTreeDrawingCanvasFlexGridSizer);

	searchTreeDrawingCanvasFlexGridSizer->Add (searchTreeDrawingCanvas, 1, wxALL|wxEXPAND, 1);

	searchTreeDrawingCanvasTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,85), wxTE_MULTILINE|wxHSCROLL);

	searchTreeDrawingCanvasFlexGridSizer->Add (searchTreeDrawingCanvasTextCtrl, 0, wxALL|wxEXPAND, 5);

	searchTreeDrawingCanvasControlPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	searchTreeDrawingCanvasFlexGridSizer->Add (searchTreeDrawingCanvasControlPanel, 0, wxALL, 5);

	wxBoxSizer*  searchTreeDrawingCanvasControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	searchTreeDrawingCanvasControlPanel->SetSizer (searchTreeDrawingCanvasControlBoxSizer);

	wxArrayString searchTreeDrawingCanvasScanChoiceRadioBoxArr;
	searchTreeDrawingCanvasScanChoiceRadioBoxArr.Add (_ ("LNR"));
	searchTreeDrawingCanvasScanChoiceRadioBoxArr.Add (_ ("RNL"));
	searchTreeDrawingCanvasScanChoiceRadioBox = new wxRadioBox (searchTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Scan"), wxDefaultPosition, wxSize (-1,-1), searchTreeDrawingCanvasScanChoiceRadioBoxArr, 1, wxRA_SPECIFY_COLS);
	searchTreeDrawingCanvasScanChoiceRadioBox->SetSelection (0);

	searchTreeDrawingCanvasControlBoxSizer->Add (searchTreeDrawingCanvasScanChoiceRadioBox, 0, wxALL|wxALIGN_TOP, 5);

	searchTreeDrawingCanvasGoButton = new wxButton (searchTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Go"), wxDefaultPosition, wxSize (-1,-1), 0);

	searchTreeDrawingCanvasControlBoxSizer->Add (searchTreeDrawingCanvasGoButton, 0, wxALL|wxALIGN_CENTER, 5);

	searchTreeDrawingCanvasVerticalLine = new wxStaticLine (searchTreeDrawingCanvasControlPanel, wxID_ANY,
	      wxDefaultPosition, wxSize (-1,-1), wxLI_VERTICAL);
	searchTreeDrawingCanvasControlBoxSizer->Add (searchTreeDrawingCanvasVerticalLine, 0, wxALL|wxEXPAND, 5);

	searchTreeDrawingCanvasInputDataTextCtrl = new wxTextCtrl (searchTreeDrawingCanvasControlPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	searchTreeDrawingCanvasInputDataTextCtrl->SetHint (wxT (""));
#endif

	searchTreeDrawingCanvasControlBoxSizer->Add (searchTreeDrawingCanvasInputDataTextCtrl, 0, wxALL|wxALIGN_CENTER, 5);

	searchTreeDrawingCanvasAddButton = new wxButton (searchTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Add"), wxDefaultPosition, wxSize (-1,-1), 0);

	searchTreeDrawingCanvasControlBoxSizer->Add (searchTreeDrawingCanvasAddButton, 0, wxALL|wxALIGN_CENTER, 5);

	searchTreeDrawingCanvasEraseButton = new wxButton (searchTreeDrawingCanvasControlPanel, wxID_ANY, _ ("Erase"), wxDefaultPosition, wxSize (-1,-1), 0);

	searchTreeDrawingCanvasControlBoxSizer->Add (searchTreeDrawingCanvasEraseButton, 0, wxALL|wxALIGN_CENTER, 5);

	searchTreeDrawingCanvasSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);

	searchTreeDrawingCanvasFlexGridSizer->Add (searchTreeDrawingCanvasSlider, 0, wxALL|wxEXPAND, 5);

	searchTreeDrawingPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	searchTreeDrawingCanvasFlexGridSizer->Add (searchTreeDrawingPanel, 0, wxALL, 5);

	wxBoxSizer* searchTreeDrawingPanelCloseButtonBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	searchTreeDrawingPanel->SetSizer (searchTreeDrawingPanelCloseButtonBoxSizer);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (0, 0, 1, wxALL, 5);

	searchTreeDrawingCanvasCloseButton = new wxButton (searchTreeDrawingPanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);

	searchTreeDrawingPanelCloseButtonBoxSizer->Add (searchTreeDrawingCanvasCloseButton, 1, wxALL, 1);

	searchTreeDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (searchTreeDrawingCanvasMenuBar);

	searchTreeDrawingCanvasFileMenu = new wxMenu();
	searchTreeDrawingCanvasMenuBar->Append (searchTreeDrawingCanvasFileMenu, _ ("File"));

	searchTreeDrawingCanvasPrintMenuItem = new wxMenuItem (searchTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Print...\t*P"), wxT (""), wxITEM_NORMAL);
	searchTreeDrawingCanvasFileMenu->Append (searchTreeDrawingCanvasPrintMenuItem);

	/*	searchTreeDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (searchTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Print Preview"), wxT (""), wxITEM_NORMAL);
		searchTreeDrawingCanvasFileMenu->Append (searchTreeDrawingCanvasPrintPreviewMenuItem);*/

	searchTreeDrawingCanvasPageSetupMenuItem = new wxMenuItem (searchTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Page Setup..."), wxT (""), wxITEM_NORMAL);
	searchTreeDrawingCanvasFileMenu->Append (searchTreeDrawingCanvasPageSetupMenuItem);

	searchTreeDrawingCanvasSaveAsMenuItem = new wxMenuItem (searchTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Save as..."), wxT (""), wxITEM_NORMAL);
	searchTreeDrawingCanvasFileMenu->Append (searchTreeDrawingCanvasSaveAsMenuItem);

	searchTreeDrawingCanvasEditMenu = new wxMenu();
	searchTreeDrawingCanvasMenuBar->Append (searchTreeDrawingCanvasEditMenu, _ ("Edit"));

	searchTreeDrawingCanvasCopyImageMenuItem = new wxMenuItem (searchTreeDrawingCanvasEditMenu, wxID_ANY, _ ("Copy Image\t*C"), wxT (""), wxITEM_NORMAL);
	searchTreeDrawingCanvasEditMenu->Append (searchTreeDrawingCanvasCopyImageMenuItem);

	searchTreeDrawingCanvasHelpMenu = new wxMenu();
	searchTreeDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	searchTreeDrawingCanvasHelpMenuItem = new wxMenuItem (searchTreeDrawingCanvasHelpMenu, ID_SEARCH_TREE_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	searchTreeDrawingCanvasHelpMenu->Append (searchTreeDrawingCanvasHelpMenuItem);
	searchTreeDrawingCanvasMenuBar->Append (searchTreeDrawingCanvasHelpMenu, _ ("&Help"));

	// SetName (wxT ("SearchTreeDrawingCanvas"));
	// SetSizeHints (1024,768);

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	SetSize (1024,768);
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

	ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_MENUITEM = searchTreeDrawingCanvasPrintMenuItem->GetId();
	// ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = searchTreeDrawingCanvasPrintPreviewMenuItem->GetId();
	ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = -1;
	ID_SEARCH_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM = searchTreeDrawingCanvasPageSetupMenuItem->GetId();
	ID_SEARCH_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM = searchTreeDrawingCanvasSaveAsMenuItem->GetId();
	ID_SEARCH_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM = searchTreeDrawingCanvasCopyImageMenuItem->GetId();

	// Connect events
	searchTreeDrawingCanvasGoButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasGoButtonPressed), NULL, this);
	searchTreeDrawingCanvasAddButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasAddButtonPressed), NULL, this);
	searchTreeDrawingCanvasEraseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasEraseButtonPressed), NULL, this);
	searchTreeDrawingCanvasSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasSliderMoved), NULL, this);
	searchTreeDrawingCanvasCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchtreedrawingcanvasclosebuttonButtonPressed), NULL, this);
	Connect (ID_SEARCH_TREE_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasHelpMenuItemSelected));
	this->Connect (searchTreeDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	// this->Connect (searchTreeDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (searchTreeDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (searchTreeDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Connect (searchTreeDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);

	SetLabel (title);
	dirty = true;
	figureSaved = false;
	haveAskedAboutSavingFile = false;
	searchTreeDrawingCanvas->getDirty (&dirty);
	myBinaryTree = new BinaryTree (canvasTitle);
	myBinaryTree->buildBinaryTree_init();
	buildSearchTree (input);
	searchTreeDrawingCanvas->setSliderAddress (searchTreeDrawingCanvasSlider);
	searchTreeDrawingCanvas->setGA (&myBinaryTree->GA);
	menuHandler = new MenuEventHandler (searchTreeDrawingCanvas,
	                                    ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	                                    ID_SEARCH_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_SEARCH_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_SEARCH_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_SEARCH_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, undirtree);

}

SearchTreeDrawingCanvas::~SearchTreeDrawingCanvas()
{
	searchTreeDrawingCanvasGoButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasGoButtonPressed), NULL, this);
	searchTreeDrawingCanvasAddButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasAddButtonPressed), NULL, this);
	searchTreeDrawingCanvasEraseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasEraseButtonPressed), NULL, this);
	searchTreeDrawingCanvasSlider->Disconnect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasSliderMoved), NULL, this);
	searchTreeDrawingCanvasCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SearchTreeDrawingCanvas::OnSearchtreedrawingcanvasclosebuttonButtonPressed), NULL, this);
	this->Disconnect (searchTreeDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	// this->Disconnect (searchTreeDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (searchTreeDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (searchTreeDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);
	this->Disconnect (searchTreeDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (SearchTreeDrawingCanvas::handleMenuChoice), NULL, this);

	delete myBinaryTree;

	switch (searchTreeType)
	{
	case AVL:
		delete avlTree;
		break;

	case RedBlack:
		delete rbTree;
		break;

	case SearchTree:
		delete searchTree;
		break;

	case NotBalanced:
		return;
	}
}

void SearchTreeDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,6);
}

void SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasGoButtonPressed (wxCommandEvent& event)
{
	int pos = searchTreeDrawingCanvasScanChoiceRadioBox->GetSelection();
	wxVector<string> treeNodes;
	wxString treeSize, dataValue, beforeAt;

	if (pos == 0)
		myBinaryTree->inorder (treeNodes);
	else
		myBinaryTree->reverseInorder (treeNodes);

	int tsize = treeNodes.size();
	treeSize.Printf (wxT ("Tree size %d nodes: "), tsize);
	searchTreeDrawingCanvasTextCtrl->AppendText (treeSize);

	for (unsigned int i=0; i < treeNodes.size(); i++)
	{
		dataValue = wxString::FromUTF8 ( (char *) treeNodes[i].c_str());
		dataValue.Printf (wxT ("%6s"),dataValue);
		beforeAt = dataValue.BeforeFirst ( (wchar_t) '@');

		if (beforeAt.Length() != 0)
			dataValue = beforeAt;

		searchTreeDrawingCanvasTextCtrl->AppendText (dataValue);
	}

	searchTreeDrawingCanvasTextCtrl->AppendText (wxT ("\n"));
	searchTreeDrawingCanvasTextCtrl->SetSelection (0,0);
}

void SearchTreeDrawingCanvas::update (UpdateType updateType)
{
	wxString strUpdateValue = searchTreeDrawingCanvasInputDataTextCtrl->GetValue();
	long updateValue;

	if (!strUpdateValue.ToLong (&updateValue))
	{
		report(wxT ("Your update value is not a valid integer."),"Note");
		return;
	}

	dirty = true;
	cout << "TextCtrl = " << searchTreeDrawingCanvasTextCtrl->GetValue() << endl;
	cout << "Do not Erase" << endl;
	// searchTreeDrawingCanvasTextCtrl->SetValue (wxT (""));
	myBinaryTree->clear();
	myBinaryTree->buildBinaryTree_init();
	searchTreeDrawingCanvas_Manager->Clear();
	searchTreeDrawingCanvas->Refresh (false);

	switch (searchTreeType)
	{
	case SearchTree:
		if (updateType == Add)
			searchTree->add (updateValue);
		else
			searchTree->erase (updateValue);

		buildMySearchTree (searchTree->getRoot());
		break;

	case    AVL:
		if (updateType == Add)
			avlTree->add (updateValue);
		else
			avlTree->erase (updateValue);

		buildMyAVLTree (avlTree->getRoot());
		break;

	case    RedBlack:
		if (updateType == Add)
			rbTree->add (updateValue);
		else
			rbTree->erase (updateValue);

		buildMyRedBlackTree (rbTree->getRoot());
		break;

	case NotBalanced:
		return; // will never happen
	}

	/*for(node v : (myBinaryTree->G).nodes)
	{
		string s;
		s = (myBinaryTree->GA).label(v); // @B missing
		int k;
		k = 5;
	}*/

	myBinaryTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw(searchTreeType == RedBlack);
	// searchTreeDrawingCanvasInputDataTextCtrl->ChangeValue (wxT (""));
}

void SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasAddButtonPressed (wxCommandEvent& event)
{
	update (Add);
}

void SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasEraseButtonPressed (wxCommandEvent& event)
{
	update (Erase);
}

void SearchTreeDrawingCanvas::buildMySearchTree (stnode *t)
{
	wxString line;

	if (t == NULL)
		return;

	buildMySearchTree (t->left);
	buildMySearchTree (t->right);

	line.Printf (wxT ("%d "), t->nodeValue);

	if (t->left != NULL)
		line += wxT ("1 ");
	else
		line += wxT ("0 ");

	if (t->right != NULL)
		line += wxT ("1");
	else
		line += wxT ("0");

	myBinaryTree->buildBTR_node (line);
}

void SearchTreeDrawingCanvas::buildMyAVLTree (AVLNode *t)
{
	wxString line;

	if (t == NULL)
		return;

	buildMyAVLTree (t->left);
	buildMyAVLTree (t->right);

	line.Printf (wxT ("%d "), t->nodeValue);

	if (t->left != NULL)
		line += wxT ("1 ");
	else
		line += wxT ("0 ");

	if (t->right != NULL)
		line += wxT ("1");
	else
		line += wxT ("0");

	myBinaryTree->buildBTR_node (line);
}

void SearchTreeDrawingCanvas::buildMyRedBlackTree (RBNode *t)
{
	wxString line;

	if (t == RBTree::NIL)
		return;

	buildMyRedBlackTree (t->left);
	buildMyRedBlackTree (t->right);

	if (t->color == RED)
		line.Printf (wxT ("%d@R "), t->nodeValue);
	else
		line.Printf (wxT ("%d@B "), t->nodeValue);

	if (t->left != RBTree::NIL)
		line += wxT ("1 ");
	else
		line += wxT ("0 ");

	if (t->right != RBTree::NIL)
		line += wxT ("1");
	else
		line += wxT ("0");

	myBinaryTree->buildBTR_node (line);
}

void SearchTreeDrawingCanvas::buildSearchTree (const wxVector<int>& data)
{
	if (data.empty())
   	return;

	if (searchTreeType == AVL)
	{
		avlTree = new AVLTree (data);
		buildMyAVLTree (avlTree->getRoot());
	}
	else if (searchTreeType == RedBlack)
	{
		rbTree = new RBTree (data);
		buildMyRedBlackTree (rbTree->getRoot());
	}
	else
	{
		searchTree= new stree (data);
		buildMySearchTree (searchTree->getRoot());
	}

	myBinaryTree->buildTreeBTR_end();

	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	drawOnCanvas->draw(searchTreeType == RedBlack);
}

void SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasSliderMoved (wxScrollEvent& event)
{
	searchTreeDrawingCanvas->SetScale (double (searchTreeDrawingCanvasSlider->GetValue()) /50);
	searchTreeDrawingCanvas->Refresh (false);
}

bool SearchTreeDrawingCanvas::closeFrame()
{
	wxString message, figureType;
	int answer;

	if (dirty && !figureSaved)
	{
		switch (searchTreeType)
		{
		case SearchTree:
			figureType = wxT("Search tree figure");
			break;

		case AVL:
			figureType = wxT("AVL tree figure");
			break;

		case RedBlack:
			figureType = wxT("Red-black tree figure");
			break;

		case NotBalanced:
			break;
		}

		if (windowTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), windowTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, figureType,
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

void SearchTreeDrawingCanvas::OnSearchtreedrawingcanvasclosebuttonButtonPressed (wxCommandEvent& event)
{
	if (closeFrame())
		Destroy();
}

void SearchTreeDrawingCanvas::OnQuit (wxCommandEvent& event)
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

void SearchTreeDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
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

void SearchTreeDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	wxVector<string> treeNodes;
	wxVector<int> data;
	bool rbtree = false;

	if (searchTreeType == RedBlack)
		rbtree = true;

	myBinaryTree->buildBinaryTreeGML (gmlFileName,rbtree);
	drawOnCanvas->setGraphProperties (& (myBinaryTree->G), & (myBinaryTree->GA), myBinaryTree->getRoot());
	myBinaryTree->levelorder (treeNodes);

	for (unsigned int i=0; i < treeNodes.size(); i++)
		data.push_back (atoi (treeNodes[i].c_str()));

	if (searchTreeType == AVL)
	{
		avlTree = new AVLTree (data);
		buildMyAVLTree (avlTree->getRoot());
	}
	else if (searchTreeType == RedBlack)
	{
		rbTree = new RBTree (data);
		// buildMyRedBlackTree (rbTree->getRoot());
	}
	else
	{
		searchTree= new stree (data);
		buildMySearchTree (searchTree->getRoot());
	}

	/*for(node v : (myBinaryTree->G).nodes)
	{
		string s;
		s = (myBinaryTree->GA).label(v); // @B missing
		int k;
		k = 5;
	}*/

	drawOnCanvas->draw(searchTreeType == RedBlack);
	figureSaved = true;
}

void SearchTreeDrawingCanvas::OnSearchTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
