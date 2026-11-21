/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/intl.h>
#include <wx/string.h>

#include "DiscreteMathCalculatorMain.h"
#include "trees/binary/BinaryTreeDrawingCanvas.h"
#include "trees/general/EquivalentBinaryTree.h"
#include "common/ImageFiles.h"
// #include "images/discmathcalc.xpm"

bool isNil (const node t, const GraphAttributes& GA)
{
	return GA.width (t) == 0.0;
}

BEGIN_EVENT_TABLE (EquivalentBinaryTree,wxFrame)
	EVT_CLOSE (EquivalentBinaryTree::OnCloseWindow)
	EVT_MENU (wxID_EXIT, EquivalentBinaryTree::OnQuit)
END_EVENT_TABLE()

const long EquivalentBinaryTree::ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

EquivalentBinaryTree::EquivalentBinaryTree (wxWindow* parent, const wxString& canvasTitle, Graph *genG, GraphAttributes *genGA, node genRoot, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), generalG (genG), generalGA (genGA), generalRoot (genRoot), treeTitle (canvasTitle), myParent(parent)
{
	wxMenuItem* equivalentBinaryTreeDrawingCanvasHelpMenuItem;
	wxMenu* equivalentBinaryTreeDrawingCanvasHelpMenu;

	wxFlexGridSizer* equivalentBinaryTreeFlexGridSizer = new wxFlexGridSizer (5, 1, 0, 0);
	equivalentBinaryTreeFlexGridSizer->SetFlexibleDirection (wxBOTH);
	equivalentBinaryTreeFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	equivalentBinaryTreeFlexGridSizer->AddGrowableCol (0);
	equivalentBinaryTreeFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (equivalentBinaryTreeFlexGridSizer);

	SetClientSize (wxSize (1024,768));
	wxIcon FrameIcon;
	FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
	SetIcon (FrameIcon);

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties ( (wxPanel *) this, equivalentBinaryTreeCanvas,
	                                    generalTreeDrawingCanvas_Manager);
	equivalentBinaryTreeCanvas->SetSize (-1,450);

	equivalentBinaryTreeFlexGridSizer->Add (equivalentBinaryTreeCanvas, 0, wxALL|wxEXPAND, 5);

	equivalentBinaryTreeTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	equivalentBinaryTreeTextCtrl->SetHint (wxT (""));
#endif

	equivalentBinaryTreeFlexGridSizer->Add (equivalentBinaryTreeTextCtrl, 0, wxALL|wxEXPAND, 5);

	equivalentBinaryTreeControlPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	equivalentBinaryTreeFlexGridSizer->Add (equivalentBinaryTreeControlPanel, 0, wxALL, 5);

	wxBoxSizer* equivalentBinaryTreeControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	equivalentBinaryTreeControlPanel->SetSizer (equivalentBinaryTreeControlBoxSizer);

	equivalentBinaryTreeStaticText = new wxStaticText (equivalentBinaryTreeControlPanel, wxID_ANY, _ ("Preorder Traversal"), wxDefaultPosition, wxSize (-1,-1), 0);

	equivalentBinaryTreeControlBoxSizer->Add (equivalentBinaryTreeStaticText, 0, wxALL, 5);

	equivalentBinaryTreeGoButton = new wxButton (equivalentBinaryTreeControlPanel, wxID_ANY, _ ("Go"), wxDefaultPosition, wxSize (-1,-1), 0);
	equivalentBinaryTreeGoButton->SetDefault();

	equivalentBinaryTreeControlBoxSizer->Add (equivalentBinaryTreeGoButton, 0, wxALL, 5);

	equivalentBinaryTreeSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);

	equivalentBinaryTreeFlexGridSizer->Add (equivalentBinaryTreeSlider, 0, wxALL|wxEXPAND, 5);

	equivalentBinaryTreeClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	equivalentBinaryTreeFlexGridSizer->Add (equivalentBinaryTreeClosePanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* equivalentBinaryTreeCloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	equivalentBinaryTreeClosePanel->SetSizer (equivalentBinaryTreeCloseBoxSizer);

	equivalentBinaryTreeCloseBoxSizer->Add (0, 0, 5, wxALL, 1);

	equivalentBinaryTreeCloseButton = new wxButton (equivalentBinaryTreeClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);

	equivalentBinaryTreeCloseBoxSizer->Add (equivalentBinaryTreeCloseButton, 1, wxALL, 5);

	equivalentBinaryTreeCloseBoxSizer->Add (0, 0, 1, wxALL, 1);

	equivalentBinaryTreeDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (equivalentBinaryTreeDrawingCanvasMenuBar);

	equivalentBinaryTreeDrawingCanvasFileMenu = new wxMenu();
	equivalentBinaryTreeDrawingCanvasMenuBar->Append (equivalentBinaryTreeDrawingCanvasFileMenu, _ ("File"));

	equivalentBinaryTreeDrawingCanvasPrintMenuItem = new wxMenuItem (equivalentBinaryTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Print...\t*P"), wxT (""), wxITEM_NORMAL);
	equivalentBinaryTreeDrawingCanvasFileMenu->Append (equivalentBinaryTreeDrawingCanvasPrintMenuItem);

	equivalentBinaryTreeDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (equivalentBinaryTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Print Preview"), wxT (""), wxITEM_NORMAL);
	equivalentBinaryTreeDrawingCanvasFileMenu->Append (equivalentBinaryTreeDrawingCanvasPrintPreviewMenuItem);

	equivalentBinaryTreeDrawingCanvasPageSetupMenuItem = new wxMenuItem (equivalentBinaryTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Page Setup..."), wxT (""), wxITEM_NORMAL);
	equivalentBinaryTreeDrawingCanvasFileMenu->Append (equivalentBinaryTreeDrawingCanvasPageSetupMenuItem);

	equivalentBinaryTreeDrawingCanvasSaveAsMenuItem = new wxMenuItem (equivalentBinaryTreeDrawingCanvasFileMenu, wxID_ANY, _ ("Save as..."), wxT (""), wxITEM_NORMAL);
	equivalentBinaryTreeDrawingCanvasFileMenu->Append (equivalentBinaryTreeDrawingCanvasSaveAsMenuItem);

	equivalentBinaryTreeDrawingCanvasEditMenu = new wxMenu();
	equivalentBinaryTreeDrawingCanvasMenuBar->Append (equivalentBinaryTreeDrawingCanvasEditMenu, _ ("Edit"));

	equivalentBinaryTreeDrawingCanvasCopyImageMenuItem = new wxMenuItem (equivalentBinaryTreeDrawingCanvasEditMenu, wxID_ANY, _ ("Copy Image\t*C"), wxT (""), wxITEM_NORMAL);
	equivalentBinaryTreeDrawingCanvasEditMenu->Append (equivalentBinaryTreeDrawingCanvasCopyImageMenuItem);

	equivalentBinaryTreeDrawingCanvasHelpMenu = new wxMenu();
	equivalentBinaryTreeDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	equivalentBinaryTreeDrawingCanvasHelpMenuItem = new wxMenuItem (equivalentBinaryTreeDrawingCanvasHelpMenu, ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	equivalentBinaryTreeDrawingCanvasHelpMenu->Append (equivalentBinaryTreeDrawingCanvasHelpMenuItem);
	equivalentBinaryTreeDrawingCanvasMenuBar->Append (equivalentBinaryTreeDrawingCanvasHelpMenu, _ ("&Help"));

	SetName (wxT ("EquivalentBinaryTree"));

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

	ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM = equivalentBinaryTreeDrawingCanvasPrintMenuItem->GetId();
	ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM = equivalentBinaryTreeDrawingCanvasPrintPreviewMenuItem->GetId();
	ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM = equivalentBinaryTreeDrawingCanvasPageSetupMenuItem->GetId();
	ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM = equivalentBinaryTreeDrawingCanvasSaveAsMenuItem->GetId();
	ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM = equivalentBinaryTreeDrawingCanvasCopyImageMenuItem->GetId();

	// Connect events
	equivalentBinaryTreeGoButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeTraversalButtonClick), NULL, this);
	equivalentBinaryTreeSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeDrawingCanvasSliderCmdScroll), NULL, this);
	equivalentBinaryTreeCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeExitMenuItemSelected), NULL, this);
	Connect (ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeHelpMenuItemSelected));
	this->Connect (equivalentBinaryTreeDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Connect (equivalentBinaryTreeDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Connect (equivalentBinaryTreeDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Connect (equivalentBinaryTreeDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED,
	               wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Connect (equivalentBinaryTreeDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);

	SetLabel (title);
	dirty = true;
	figureSaved = false;
	equivalentBinaryTreeCanvas->getDirty (&dirty);
	equivalentBinaryTreeCanvas->setSliderAddress (equivalentBinaryTreeSlider);
	menuHandler = new MenuEventHandler (equivalentBinaryTreeCanvas,
	                                    ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PRINT_MENUITEM,
	                                    ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PRINT_PREVIEW_MENUITEM,
	                                    ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_PAGESETUP_MENUITEM,
	                                    ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM,
	                                    ID_EQUIVALENT_BINARY_TREE_DRAWING_CANVAS_COPY_IMAGE_MENUITEM,
	                                    &figureSaved, undirtree);
}

EquivalentBinaryTree::~EquivalentBinaryTree()
{
	equivalentBinaryTreeGoButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeTraversalButtonClick), NULL, this);
	equivalentBinaryTreeSlider->Disconnect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeDrawingCanvasSliderCmdScroll), NULL, this);
	equivalentBinaryTreeCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (EquivalentBinaryTree::OnequivalentBinaryTreeExitMenuItemSelected), NULL, this);
	this->Disconnect (equivalentBinaryTreeDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Disconnect (equivalentBinaryTreeDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Disconnect (equivalentBinaryTreeDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Disconnect (equivalentBinaryTreeDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);
	this->Disconnect (equivalentBinaryTreeDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (EquivalentBinaryTree::handleMenuChoice), NULL, this);

}
void EquivalentBinaryTree::OnequivalentBinaryTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	equivalentBinaryTreeCanvas->SetScale (double (equivalentBinaryTreeSlider->GetValue()) /50);
	equivalentBinaryTreeCanvas->Refresh (false);
}

void EquivalentBinaryTree::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event, 3);
}

void EquivalentBinaryTree::OnequivalentBinaryTreeTraversalButtonClick (wxCommandEvent& event)
{
	preorder();
}

void EquivalentBinaryTree::init()
{
	GA.init (G,
	         GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
	         GraphAttributes::nodeLabel | GraphAttributes::nodeStyle | GraphAttributes::nodeType |
	         GraphAttributes::edgeStyle | GraphAttributes::edgeStyle |
	         GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate |
	         GraphAttributes::edgeArrow | GraphAttributes::edgeType);

	nil = G.newNode();
	GA.label (nil) = string ("");
	GA.strokeWidth (nil) = 0.0;
	GA.width (nil) = 0.0;
	GA.height (nil) = 0.0;
	root = nil;
}

void EquivalentBinaryTree::completeBuild()
{
	TreeLayout btreeLayout;

	btreeLayout.siblingDistance (35.0);
	btreeLayout.levelDistance (40.0);
	btreeLayout.subtreeDistance (75.0);
	btreeLayout.rootSelection (TreeLayout::RootSelectionType::Source);
	btreeLayout.orientation (Orientation::topToBottom);
	btreeLayout.call (GA);
	drawOnCanvas->setGraphProperties (&G, &GA, root);
	drawOnCanvas->draw();
}

node EquivalentBinaryTree::createNode (const string& label)
{
	node t = G.newNode();

	GA.shape (t) = Shape::Ellipse;
	GA.label (t) = label;
	GA.strokeWidth (t) = 1.0;
	GA.width (t) = 25.0;
	GA.height (t) = 25.0;

	return t;
}

void EquivalentBinaryTree::insertNullSubtree (node currNode)
{
	node t = G.newNode();
	edge e;

	GA.shape (t) = Shape::Ellipse;
	GA.label (t) = "";
	GA.strokeWidth (t) = 0.0;
	GA.width (t) = 0.0;
	GA.height (t) = 0.0;
	e = G.newEdge (currNode,t);
	// GA.type (e) = Graph::EdgeType::association;
	GA.strokeWidth (e) = 0.0;
}

void EquivalentBinaryTree::insertEdge (node parent, node child)
{
	edge e = G.newEdge (parent, child);

	GA.arrowType (e) = ogdf::EdgeArrow::None;
	// GA.type (e) = Graph::EdgeType::association;
	GA.strokeWidth (e) = 1.0;
}

node EquivalentBinaryTree::findLeftmostChild (node t)
{
	List<edge> outgoingEdges;

	string whoamI = generalGA->label (t);
	t->outEdges (outgoingEdges);

	if (outgoingEdges.empty())
		return NULL;
	else
	{
		string leftChild = generalGA->label ( (** (outgoingEdges.begin())).target());
		return (** (outgoingEdges.begin())).target();
	}
}

node EquivalentBinaryTree::findRightSibling (node t)
{
	List<edge> incomingEdges, parentChildren;
	ListIterator<edge> currIter;
	edge e;
	node parent;

	string whoamI = generalGA->label(t);
	t->inEdges (incomingEdges);

	if (incomingEdges.empty())
		return NULL;    // t is the root of the general tree

	e = * (incomingEdges.begin());
	parent = e->source();

	parent->outEdges (parentChildren);
	currIter = parentChildren.begin();

	while (*currIter != e)
		currIter++;

	currIter++;

	if (currIter == parentChildren.end())
		return NULL;
	else
		return (*currIter)->target();
}

void EquivalentBinaryTree::buildEquivalentTree()
{
	init();

	if (generalRoot == NULL)
	{
		root = nil;
		return;
	}

	dfs (generalRoot);

	List<node> allTreeNodes;
	ListIterator<node> iter;

	G.allNodes (allTreeNodes);

	iter = allTreeNodes.begin();

	while (iter != allTreeNodes.end())
	{
		if ( ( (*iter)->indeg() == 0) && (GA.label (*iter) != ""))
		{
			root = *iter;
			break;
		}

		iter++;
	}

	completeBuild();
}

void EquivalentBinaryTree::process (node t)
{
	node binNode;
	node leftmostChild, rightSibling;
	node binaryTreeNodeLeftChild, binaryTreeNodeRightChild;
	map<node, node>::iterator iter;

	if ( (iter = binTreeBuildInfo.find (t)) == binTreeBuildInfo.end())
	{
		binNode = createNode (generalGA->label (t));
		binTreeBuildInfo[t] = binNode;
	}
	else
		binNode = (*iter).second;

	leftmostChild = findLeftmostChild (t);
	rightSibling = findRightSibling (t);

	if (leftmostChild != NULL)
	{
		binaryTreeNodeLeftChild = createNode (generalGA->label (leftmostChild));
		binTreeBuildInfo[leftmostChild] = binaryTreeNodeLeftChild;
		insertEdge (binNode, binaryTreeNodeLeftChild);
	}
	else
		insertNullSubtree (binNode);

	if (rightSibling != NULL)
	{
		binaryTreeNodeRightChild = createNode (generalGA->label (rightSibling));
		binTreeBuildInfo[rightSibling] = binaryTreeNodeRightChild;
		insertEdge (binNode, binaryTreeNodeRightChild);
	}
	else
		insertNullSubtree (binNode);
}

void EquivalentBinaryTree::dfs (node t)
{
	List<edge> children;
	ListIterator<edge> iter;
	node child;

	process (t);

	t->outEdges (children);   // get the edges from t
	iter = children.begin();

	while (iter != children.end())
	{
		child = (**iter).target();
		dfs (child);
		iter++;
	}
}

void EquivalentBinaryTree::preorder()
{
	node t = root;

	if (isNil (t, GA))
		return;

	NLR (t);
}

void EquivalentBinaryTree::NLR (node t)
{
	List<edge> children;
	ListIterator<edge> iter;
	node child;

	// output the current node
	equivalentBinaryTreeTextCtrl->AppendText (wxString (GA.label (t).c_str(),wxConvUTF8));
	equivalentBinaryTreeTextCtrl->AppendText (wxT ("  "));

	t->outEdges (children);   // get the 1-2 edges from t
	iter = children.begin();
	child = (**iter).target();

	if (!isNil (child, GA)) // left child
		NLR (child);

	iter++;
	child = (**iter).target();

	if (!isNil (child, GA)) // right child
		NLR (child);
}

bool EquivalentBinaryTree::closeFrame()
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Equivalent binary tree figure"),
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

void EquivalentBinaryTree::OnequivalentBinaryTreeExitMenuItemSelected (wxCommandEvent& event)
{
	if (closeFrame())
		Destroy();
}

void EquivalentBinaryTree::OnQuit (wxCommandEvent& event)
{
	bool closeTheFrame = false;

	event.Skip();
	if (!haveAskedAboutSavingFile)
	{
		haveAskedAboutSavingFile = true;
		closeTheFrame = closeFrame();
		if (closeTheFrame)
		{
			Destroy();
			((BinaryTreeDrawingCanvas *)myParent)->Close(true);
		}
		else
		{
			haveAskedAboutSavingFile = false;
			return;
		}
	}
	else
	{
		Destroy();
		((DiscreteMathCalculatorFrame *)myParent)->Close(true);
	}
}

void EquivalentBinaryTree::OnCloseWindow (wxCloseEvent& event)
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

void EquivalentBinaryTree::OnequivalentBinaryTreeHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
