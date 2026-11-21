/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DiscreteMathCalculatorMain.h"
#include "trees/expression/ExpressionTreeDrawingCanvas.h"
#include "common/ImageFiles.h"

#include <wx/textfile.h>

const long ExpressionTreeDrawingCanvas::ID_EXPRESSION_TREE_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (ExpressionTreeDrawingCanvas,wxFrame)
	EVT_CLOSE (ExpressionTreeDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, ExpressionTreeDrawingCanvas::OnQuit)
END_EVENT_TABLE()

ExpressionTreeDrawingCanvas::ExpressionTreeDrawingCanvas (wxWindow* parent, const wxString& canvasTitle, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) :
	wxFrame (parent, id, title, pos, size, style), treeTitle (canvasTitle), myParent(parent)
{
	wxMenu* expressionTreeDrawingCanvasHelpMenu;
	wxMenuItem* expressionTreeDrawingCanvasHelpMenuItem;

	wxIcon FrameIcon;
	FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm"))));
	SetIcon (FrameIcon);

	this->SetSizeHints (wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* expressionDrawingCanvasFlexGridSizer;
	expressionDrawingCanvasFlexGridSizer = new wxFlexGridSizer (5, 1, 0, 0);
	expressionDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	expressionDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	expressionDrawingCanvasFlexGridSizer->SetFlexibleDirection (wxBOTH);
	expressionDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties ((wxPanel *)this, expressionTreeDrawingCanvas,
	                                   expressionTreeDrawingCanvas_Manager);
	expressionDrawingCanvasFlexGridSizer->Add (expressionTreeDrawingCanvas, 0, wxALL|wxEXPAND, 1);

	expressionDrawingCanvasStaticText = new wxStaticText (this, wxID_ANY, wxT ("Postfix Expression and the Expression Value"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	expressionDrawingCanvasStaticText->Wrap (-1);
	expressionDrawingCanvasFlexGridSizer->Add (expressionDrawingCanvasStaticText, 0, wxALL|wxEXPAND, 1);

	expressionDrawingCanvasTextCtrl = new wxTextCtrl (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (-1,100), wxTE_MULTILINE|wxHSCROLL);
	expressionDrawingCanvasFlexGridSizer->Add (expressionDrawingCanvasTextCtrl, 0, wxALL|wxEXPAND, 1);

	expressionDrawingCanvasSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	expressionDrawingCanvasFlexGridSizer->Add (expressionDrawingCanvasSlider, 0, wxALL|wxEXPAND, 1);

	expressionDrawingCanvasClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,35), wxTAB_TRAVERSAL);
	wxBoxSizer* expressionDrawingCanvasCloseBoxSizer;
	expressionDrawingCanvasCloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	expressionDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);
	expressionDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);
	expressionDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);
	expressionDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);
	expressionDrawingCanvasCloseButton = new wxButton (expressionDrawingCanvasClosePanel, wxID_ANY, wxT ("Close"), wxDefaultPosition, wxDefaultSize, 0);
	expressionDrawingCanvasCloseButton->SetDefault();
	expressionDrawingCanvasCloseBoxSizer->Add (expressionDrawingCanvasCloseButton, 0, wxALL, 1);
	expressionDrawingCanvasCloseBoxSizer->Add (0, 0, 1, wxEXPAND, 1);
	expressionDrawingCanvasClosePanel->SetSizer (expressionDrawingCanvasCloseBoxSizer);
	expressionDrawingCanvasClosePanel->Layout();
	expressionDrawingCanvasCloseBoxSizer->Fit (expressionDrawingCanvasClosePanel);
	expressionDrawingCanvasFlexGridSizer->Add (expressionDrawingCanvasClosePanel, 1, wxEXPAND | wxALL, 1);
	this->SetSizer (expressionDrawingCanvasFlexGridSizer);
	this->Layout();

	expressionDrawingCanvasMenuBar = new wxMenuBar (0);
	expressionDrawingCanvasFileMenu = new wxMenu();
	expressionDrawingCanvasMenuBar->Append (expressionDrawingCanvasFileMenu, wxT ("File"));

	wxMenuItem* expressionDrawingCanvasPrintMenuItem;
	expressionDrawingCanvasPrintMenuItem = new wxMenuItem (expressionDrawingCanvasFileMenu, wxID_ANY, wxString (wxT ("Print..")) + wxT ('\t') + wxT ("*P"), wxEmptyString, wxITEM_NORMAL);
	expressionDrawingCanvasFileMenu->Append (expressionDrawingCanvasPrintMenuItem);

	wxMenuItem* expressionDrawingCanvasPrintPreviewMenuItem;
	expressionDrawingCanvasPrintPreviewMenuItem = new wxMenuItem (expressionDrawingCanvasFileMenu, wxID_ANY, wxString (wxT ("Print Preview")), wxEmptyString, wxITEM_NORMAL);
	expressionDrawingCanvasFileMenu->Append (expressionDrawingCanvasPrintPreviewMenuItem);

	wxMenuItem* expressionDrawingCanvasPageSetupMenuItem;
	expressionDrawingCanvasPageSetupMenuItem = new wxMenuItem (expressionDrawingCanvasFileMenu, wxID_ANY, wxString (wxT ("Page Setup...")), wxEmptyString, wxITEM_NORMAL);
	expressionDrawingCanvasFileMenu->Append (expressionDrawingCanvasPageSetupMenuItem);

	wxMenuItem* expressionDrawingCanvasSaveAsMenuItem;
	expressionDrawingCanvasSaveAsMenuItem = new wxMenuItem (expressionDrawingCanvasFileMenu, wxID_ANY, wxString (wxT ("Save as...")), wxEmptyString, wxITEM_NORMAL);
	expressionDrawingCanvasFileMenu->Append (expressionDrawingCanvasSaveAsMenuItem);

	expressionDrawingCanvasEditMenu = new wxMenu();
	wxMenuItem* expressionDrawingCanvasCopyImageMenuItem;
	expressionDrawingCanvasCopyImageMenuItem = new wxMenuItem (expressionDrawingCanvasEditMenu, wxID_ANY, wxString (wxT ("Copy Image")) + wxT ('\t') + wxT ("*C"), wxEmptyString, wxITEM_NORMAL);
	expressionDrawingCanvasEditMenu->Append (expressionDrawingCanvasCopyImageMenuItem);

	expressionDrawingCanvasMenuBar->Append (expressionDrawingCanvasEditMenu, wxT ("Edit"));

	expressionTreeDrawingCanvasHelpMenu = new wxMenu();
	expressionTreeDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	expressionTreeDrawingCanvasHelpMenuItem = new wxMenuItem (expressionTreeDrawingCanvasHelpMenu, ID_EXPRESSION_TREE_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	expressionTreeDrawingCanvasHelpMenu->Append (expressionTreeDrawingCanvasHelpMenuItem);
	expressionDrawingCanvasMenuBar->Append (expressionTreeDrawingCanvasHelpMenu, _ ("&Help"));

	this->SetMenuBar (expressionDrawingCanvasMenuBar);

	SetLabel (title);
	expressionTree = new BinaryTree (canvasTitle);
	expressionTreeDrawingCanvas->setSliderAddress (expressionDrawingCanvasSlider);
	expressionTreeDrawingCanvas->setGA (&expressionTree->GA);
	EXPRESSIONDRAWINGCANVASPRINTMENUITEM_ID = expressionDrawingCanvasPrintMenuItem->GetId();
	EXPRESSIONDRAWINGCANVASPRINTPREVIEWMENUITEM_ID = expressionDrawingCanvasPrintPreviewMenuItem->GetId();
	EXPRESSIONDRAWINGCANVASPAGESETUPMENUITEM_ID = expressionDrawingCanvasPageSetupMenuItem->GetId();
	EXPRESSIONDRAWINGCANVASSAVEASMENUITEM_ID = expressionDrawingCanvasSaveAsMenuItem->GetId();
	EXPRESSIONDRAWINGCANVASCOPYIMAGEMENUITEM_ID = expressionDrawingCanvasCopyImageMenuItem->GetId();

	menuHandler = new MenuEventHandler (expressionTreeDrawingCanvas,
	                                    EXPRESSIONDRAWINGCANVASPRINTMENUITEM_ID,
	                                    EXPRESSIONDRAWINGCANVASPRINTPREVIEWMENUITEM_ID,
	                                    EXPRESSIONDRAWINGCANVASPAGESETUPMENUITEM_ID,
	                                    EXPRESSIONDRAWINGCANVASSAVEASMENUITEM_ID,
	                                    EXPRESSIONDRAWINGCANVASCOPYIMAGEMENUITEM_ID,
	                                    &figureSaved, undirtree);
	dirty = true;
	figureSaved = false;
	haveAskedAboutSavingFile = false;
	expressionTreeDrawingCanvas->getDirty (&dirty);
	this->Centre (wxBOTH);

	// Connect Events
	expressionDrawingCanvasSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasSliderCmdScroll), NULL, this);
	expressionDrawingCanvasCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasCloseButtonClick), NULL, this);
	Connect (ID_EXPRESSION_TREE_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasHelpMenuItemSelected));
	this->Connect (expressionDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Connect (expressionDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Connect (expressionDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Connect (expressionDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Connect (expressionDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
}

ExpressionTreeDrawingCanvas::~ExpressionTreeDrawingCanvas()
{
	// Disconnect Events
	expressionDrawingCanvasSlider->Disconnect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasSliderCmdScroll), NULL, this);
	expressionDrawingCanvasCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasCloseButtonClick), NULL, this);
	this->Disconnect (wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Disconnect (wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Disconnect (wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Disconnect (wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));
	this->Disconnect (wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (ExpressionTreeDrawingCanvas::handleMenuChoice));

}

void ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	expressionTreeDrawingCanvas->SetScale (double (expressionDrawingCanvasSlider->GetValue()) /50);
	expressionTreeDrawingCanvas->Refresh (false);
}

void ExpressionTreeDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	wxString graphicsFileName;

	menuHandler->process (event,2);

	if (event.GetId() == EXPRESSIONDRAWINGCANVASSAVEASMENUITEM_ID)
	{
		graphicsFileName = menuHandler->getGraphicsFileName();

		if (graphicsFileName.length() != 0)
		{
			wxTextFile gmlFile;

			if (!gmlFile.Open (graphicsFileName))
			{
				wxString message, caption;

				message.append (wxT ("Failure to append expression value to GML file.\nPlease report this error to wford333@gmail.com"));
				caption.append (wxT ("Error"));

				wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
				dialog.ShowModal();
			}
			else
			{
				wxString valueMsg;

				valueMsg.Printf (wxT ("# The value of the expression is %ld"), expValue);
				gmlFile.AddLine (valueMsg);
				gmlFile.Write();
				gmlFile.Close();
			}
		}
	}
}

void ExpressionTreeDrawingCanvas::buildExpressionTree (stringstream& parserOutput,
      const wxVector<wxString>& fromExpressionTreeDialog, long int value)
{
	wxString s = wxString (parserOutput.str().c_str(),wxConvUTF8);
	expressionDrawingCanvasTextCtrl->AppendText (wxString (parserOutput.str().c_str(),wxConvUTF8));
	expValue = value;

	expressionTree->buildBinaryTree_init();

	for (unsigned int i=0; i < fromExpressionTreeDialog.size(); i++)
		expressionTree->buildBTR_node (fromExpressionTreeDialog[i]);

	expressionTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (expressionTree->G), & (expressionTree->GA), expressionTree->getRoot());
	drawOnCanvas->draw();
}

void ExpressionTreeDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	wxVector<string> postorderTraversal;
	wxString expr, valueLine, strValue;

	expressionTree->buildBinaryTree_init();
	expressionTree->buildBinaryTreeGML (gmlFileName);
	drawOnCanvas->setGraphProperties (& (expressionTree->G), & (expressionTree->GA), expressionTree->getRoot());
	drawOnCanvas->draw();
	expressionTree->postorder (postorderTraversal);

	expr.Append (wxT ("The postfix expression is: "));

	for (unsigned int i=0; i < postorderTraversal.size(); i++)
	{
		expr.Append (wxString (postorderTraversal[i].c_str(),wxConvUTF8));
		expr.Append (wxT ("  "));
	}

	expr.Append (wxT ("\n\n"));

	// lookup the expression value in comment in the last line of the file
	wxTextFile gmlFile;

	gmlFile.Open (gmlFileName);
	valueLine = gmlFile.GetLastLine();
	strValue = valueLine.AfterLast ( (wxChar) ' ');
	expr.Append (wxT ("The value of the expression is ") + strValue);
	expressionDrawingCanvasTextCtrl->ChangeValue (expr);
	figureSaved = true;
}

bool ExpressionTreeDrawingCanvas::closeFrame()
{
	wxString message;
	int answer;

	if (dirty && !figureSaved)
	{
		if (treeTitle.length() != 0)
			message.Printf (wxT ("Save %s?"), treeTitle.c_str());
		else
			message.Printf (wxT ("Save figure?"));

		answer = wxMessageBox (message, wxT ("Expression tree figure"),
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

void ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasCloseButtonClick (wxCommandEvent& event)
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

void ExpressionTreeDrawingCanvas::OnQuit (wxCommandEvent& event)
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

void ExpressionTreeDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
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

void ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}
