///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

// #include "noname.h"

///////////////////////////////////////////////////////////////////////////

ExpressionTreeDrawingCanvas::ExpressionTreeDrawingCanvas( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* expressionDrawingCanvasFlexGridSizer;
	expressionDrawingCanvasFlexGridSizer = new wxFlexGridSizer( 5, 1, 0, 0 );
	expressionDrawingCanvasFlexGridSizer->AddGrowableCol( 0 );
	expressionDrawingCanvasFlexGridSizer->AddGrowableRow( 0 );
	expressionDrawingCanvasFlexGridSizer->SetFlexibleDirection( wxBOTH );
	expressionDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	drawOnCanvas = new DrawTree();
	drawOnCanvas->setCanvasProperties (expressionTreeDrawingCanvasPanel, expressionTreeDrawingCanvas,
	                                   expressionTreeDrawingCanvas_Manager);
	expressionDrawingCanvasFlexGridSizer->Add( expressionTreeDrawingCanvas, 0, wxALL|wxEXPAND, 1 );

	expressionDrawingCanvasStaticText = new wxStaticText( this, wxID_ANY, wxT("Postfix Expression and the Expression Value"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	expressionDrawingCanvasStaticText->Wrap( -1 );
	expressionDrawingCanvasFlexGridSizer->Add( expressionDrawingCanvasStaticText, 0, wxALL|wxEXPAND, 1 );

	expressionDrawingCanvasTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_MULTILINE|wxHSCROLL );
	expressionDrawingCanvasFlexGridSizer->Add( expressionDrawingCanvasTextCtrl, 0, wxALL|wxEXPAND, 1 );

	expressionDrawingCanvasSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	expressionDrawingCanvasFlexGridSizer->Add( expressionDrawingCanvasSlider, 0, wxALL|wxEXPAND, 1 );

	expressionDrawingCanvasClosePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* expressionDrawingCanvasCloseBoxSizer;
	expressionDrawingCanvasCloseBoxSizer = new wxBoxSizer( wxHORIZONTAL );


	expressionDrawingCanvasCloseBoxSizer->Add( 0, 0, 1, wxEXPAND, 1 );


	expressionDrawingCanvasCloseBoxSizer->Add( 0, 0, 1, wxEXPAND, 1 );


	expressionDrawingCanvasCloseBoxSizer->Add( 0, 0, 1, wxEXPAND, 1 );


	expressionDrawingCanvasCloseBoxSizer->Add( 0, 0, 1, wxEXPAND, 1 );

	expressionDrawingCanvasCloseButton = new wxButton( expressionDrawingCanvasClosePanel, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	expressionDrawingCanvasCloseButton->SetDefault();
	expressionDrawingCanvasCloseBoxSizer->Add( expressionDrawingCanvasCloseButton, 0, wxALL, 1 );


	expressionDrawingCanvasClosePanel->SetSizer( expressionDrawingCanvasCloseBoxSizer );
	expressionDrawingCanvasClosePanel->Layout();
	expressionDrawingCanvasCloseBoxSizer->Fit( expressionDrawingCanvasClosePanel );
	expressionDrawingCanvasFlexGridSizer->Add( expressionDrawingCanvasClosePanel, 1, wxEXPAND | wxALL, 1 );


	this->SetSizer( expressionDrawingCanvasFlexGridSizer );
	this->Layout();
	expressionDrawingCanvasMenuBar = new wxMenuBar( 0 );
	expressionDrawingCanvasFileMenu = new wxMenu();
	wxMenuItem* expressionDrawingCanvasPrintMenuItem;
	expressionDrawingCanvasPrintMenuItem = new wxMenuItem( expressionDrawingCanvasFileMenu, wxID_ANY, wxString( wxT("Print..") ) + wxT('\t') + wxT("*P"), wxEmptyString, wxITEM_NORMAL );
	expressionDrawingCanvasFileMenu->Append( expressionDrawingCanvasPrintMenuItem );

	wxMenuItem* expressionDrawingCanvasPrintPreviewMenuItem;
	expressionDrawingCanvasPrintPreviewMenuItem = new wxMenuItem( expressionDrawingCanvasFileMenu, wxID_ANY, wxString( wxT("Print Preview") ), wxEmptyString, wxITEM_NORMAL );
	expressionDrawingCanvasFileMenu->Append( expressionDrawingCanvasPrintPreviewMenuItem );

	wxMenuItem* expressionDrawingCanvasPageSetupMenuItem;
	expressionDrawingCanvasPageSetupMenuItem = new wxMenuItem( expressionDrawingCanvasFileMenu, wxID_ANY, wxString( wxT("Page Setup...") ), wxEmptyString, wxITEM_NORMAL );
	expressionDrawingCanvasFileMenu->Append( expressionDrawingCanvasPageSetupMenuItem );

	wxMenuItem* expressionDrawingCanvasSaveAsMenuItem;
	expressionDrawingCanvasSaveAsMenuItem = new wxMenuItem( expressionDrawingCanvasFileMenu, wxID_ANY, wxString( wxT("Save as...") ), wxEmptyString, wxITEM_NORMAL );
	expressionDrawingCanvasFileMenu->Append( expressionDrawingCanvasSaveAsMenuItem );

	expressionDrawingCanvasMenuBar->Append( expressionDrawingCanvasFileMenu, wxT("MyMenu") );

	expressionDrawingCanvasEditMenu = new wxMenu();
	wxMenuItem* expressionDrawingCanvasCopyImageMenuItem;
	expressionDrawingCanvasCopyImageMenuItem = new wxMenuItem( expressionDrawingCanvasEditMenu, wxID_ANY, wxString( wxT("Copy Image") ) + wxT('\t') + wxT("*C"), wxEmptyString, wxITEM_NORMAL );
	expressionDrawingCanvasEditMenu->Append( expressionDrawingCanvasCopyImageMenuItem );

	expressionDrawingCanvasMenuBar->Append( expressionDrawingCanvasEditMenu, wxT("MyMenu") );

	this->SetMenuBar( expressionDrawingCanvasMenuBar );


	SetLabel (title);
	expressionTree = new BinaryTree (title);
	expressionTreeDrawingCanvas->setSliderAddress (expressionTreeDrawingCanvasSlider);
	expressionTreeDrawingCanvas->setGA (&expressionTree->GA);
	figureSaved = false;
	menuHandler = new MenuEventHandler (expressionTreeDrawingCanvas,
	                                    expressionDrawingCanvasPrintMenuItem->GetId(),
	                                    expressionDrawingCanvasPrintPreviewMenuItem->GetId(),
	                                    expressionDrawingCanvasPageSetupMenuItem->GetId(),
	                                    expressionDrawingCanvasSaveAsMenuItem->GetId(),
	                                    expressionDrawingCanvasCopyImageMenuItem->GetId(),
	                                    &figureSaved);
	dirty = true;
	expressionTreeDrawingCanvas->getDirty (&dirty);
	this->Centre( wxBOTH );

	// Connect Events
	expressionDrawingCanvasSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasSliderCmdScroll ), NULL, this );
	expressionDrawingCanvasCloseButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasCloseButtonClick ), NULL, this );
	this->Connect( expressionDrawingCanvasPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Connect( expressionDrawingCanvasPrintPreviewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Connect( expressionDrawingCanvasPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Connect( expressionDrawingCanvasSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Connect( expressionDrawingCanvasCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
}

ExpressionTreeDrawingCanvas::~ExpressionTreeDrawingCanvas()
{
	// Disconnect Events
	expressionDrawingCanvasSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasSliderCmdScroll ), NULL, this );
	expressionDrawingCanvasCloseButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasCloseButtonClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ExpressionTreeDrawingCanvas::handleMenuChoice ) );

}

void ExpressionTreeDrawingCanvas::OnExpressionTreeDrawingCanvasCloseButtonClick(wxCommandEvent& event)
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
			if (!menuHandler->handleSaveAs())
				return;
		}
	}

	Destroy();
}

void ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	expressionTreeDrawingCanvas->SetScale (double (expressionTreeDrawingCanvasSlider->GetValue()) /50);
	expressionTreeDrawingCanvas->Refresh (false);
}

void ExpressionTreeDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	wxString graphicsFileName;

	menuHandler->process (event);

	if (event.GetId() == ID_EXPRESSION_TREE_DRAWING_CANVAS_SAVE_AS_MENUITEM)
	{
		graphicsFileName = menuHandler->getGraphicsFileName();

		if (graphicsFileName.length() != 0)
		{
			wxTextFile gmlFile;

			if (!gmlFile.Open (graphicsFileName))
			{
				wxString message, caption;

				message.append (wxT ("Failure to append expression value to GML file.\nPlease report this error to wford@pacific.edu"));
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

void ExpressionTreeDrawingCanvas::OnexpressionTreeDrawingCanvasExitMenuItemSelected (wxCommandEvent& event)
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
			if (!menuHandler->handleSaveAs())
				return;
		}
	}

	Destroy();
}

void ExpressionTreeDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
{
	wxCommandEvent cmdevent;
	OnexpressionTreeDrawingCanvasExitMenuItemSelected (cmdevent);
}

void ExpressionTreeDrawingCanvas::buildExpressionTree (stringstream& parserOutput,
      const wxVector<wxString>& fromExpressionTreeDialog, long int value)
{
	wxString s = wxString (parserOutput.str().c_str(),wxConvUTF8);
	expressionTreeDrawingCanvasPostfixValueOutputTextCtrl->AppendText (wxString (parserOutput.str().c_str(),wxConvUTF8));
	expValue = value;

	expressionTree->buildBinaryTree_init();

	for (unsigned int i=0; i < fromExpressionTreeDialog.size(); i++)
		expressionTree->buildBTR_node (fromExpressionTreeDialog[i]);

	expressionTree->buildTreeBTR_end();
	drawOnCanvas->setGraphProperties (& (expressionTree->G), & (expressionTree->GA), expressionTree->getRoot());
	drawOnCanvas->draw();
}

DrawingCanvas *ExpressionTreeDrawingCanvas::getCanvas()
{
	return expressionTreeDrawingCanvas;
}

void ExpressionTreeDrawingCanvas::makeTreeFromGMLFile (const wxString& gmlFileName)
{
	wxVector<String> postorderTraversal;
	wxString expr, valueLine, strValue;

	expressionTree->buildBinaryTree_init();
	expressionTree->buildBinaryTreeGML (gmlFileName);
	drawOnCanvas->setGraphProperties (& (expressionTree->G), & (expressionTree->GA), expressionTree->getRoot());
	drawOnCanvas->draw();
	expressionTree->postorder (postorderTraversal);

	expr.Append (wxT ("The postfix expression is: "));

	for (unsigned int i=0; i < postorderTraversal.size(); i++)
	{
		expr.Append (wxString (postorderTraversal[i].cstr(),wxConvUTF8));
		expr.Append (wxT ("  "));
	}

	expr.Append (wxT ("\n\n"));

	// lookup the expression value in comment in the last line of the file
	wxTextFile gmlFile;

	gmlFile.Open (gmlFileName);
	valueLine = gmlFile.GetLastLine();
	strValue = valueLine.AfterLast ( (wxChar) ' ');
	expr.Append (wxT ("The value of the expression is ") + strValue);
	expressionTreeDrawingCanvasPostfixValueOutputTextCtrl->ChangeValue (expr);
	figureSaved = true;
}

