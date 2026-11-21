/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/WidestPath/WidestPathDrawingCanvas.h"
#include "graphs/draw/DrawGraph.h"
#include "DiscreteMathCalculatorMain.h"
#include "common/DrawingCanvas.h"
#include "common/ImageFiles.h"
// #include "images/discmathcalc.xpm"
#include "graphs/algorithms/GraphAlgorithmCanvas.h"
#include "common/ImageFiles.h"

const long WidestPathDrawingCanvas::ID_MAXSPANNINGTREE_DRAWING = wxNewId();
const long WidestPathDrawingCanvas::ID_WIDESTPATH_DRAWING = wxNewId();
const long WidestPathDrawingCanvas::ID_WIDESTPATH_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();

BEGIN_EVENT_TABLE (WidestPathDrawingCanvas,wxFrame)
	EVT_CLOSE (WidestPathDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, WidestPathDrawingCanvas::OnQuit)
END_EVENT_TABLE()

WidestPathDrawingCanvas::WidestPathDrawingCanvas (wxWindow* parent, BuildGraph *mSpanningTree, BuildGraph *widPath, BuildGraph *combGraphs, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), myParent (parent),
	  buildMaxSpanningTree (mSpanningTree), buildWidestPath (widPath), combinedGraphs(combGraphs)
{
	SetClientSize (wxSize (-1,-1) );
	{
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("combcalc.xpm") ) ) );
		SetIcon (FrameIcon);
	}

	wxFlexGridSizer* WidestPathFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	WidestPathFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	WidestPathFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	WidestPathFlexGridSizer->AddGrowableCol (0);
	WidestPathFlexGridSizer->AddGrowableCol (1);
	WidestPathFlexGridSizer->AddGrowableRow (1);
	this->SetSizer (WidestPathFlexGridSizer);

	MaximumSpanningTreeStaticText = new wxStaticText (this, wxID_ANY, _ ("Maximum spanning tree"), wxDefaultPosition, wxSize (-1,-1), 0);

	WidestPathFlexGridSizer->Add (MaximumSpanningTreeStaticText, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);

	WidestPathStaticText = new wxStaticText (this, wxID_ANY, _ ("Widest path"), wxDefaultPosition, wxSize (-1,-1), 0);

	WidestPathFlexGridSizer->Add (WidestPathStaticText, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);

	MaximumSpanningTreeGraph = new DrawingCanvas (&maxspanningtree_canvas, this, ID_MAXSPANNINGTREE_DRAWING, wxDefaultPosition, wxSize(512,384), wxHSCROLL|wxVSCROLL);
	MaximumSpanningTreeGraph->SetScrollRate (5, 5);

	WidestPathFlexGridSizer->Add (MaximumSpanningTreeGraph, 1, wxALL|wxEXPAND, 5);

	WidestPathGraph = new DrawingCanvas (&widestpath_canvas, this, ID_WIDESTPATH_DRAWING, wxDefaultPosition, wxSize(512,384), wxHSCROLL|wxVSCROLL);
	WidestPathGraph->SetScrollRate (5, 5);

	printGraphsCanvas = new DrawingCanvas (&print_canvas, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL);
	printGraphsCanvas->SetScrollRate (5, 5);

	WidestPathFlexGridSizer->Add (WidestPathGraph, 1, wxALL|wxEXPAND, 5);

	MaximumSpanningTreeSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);
	WidestPathFlexGridSizer->Add (MaximumSpanningTreeSlider, 0, wxALL|wxEXPAND, 5);

	WidestPathSlider = new wxSlider (this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);
	WidestPathFlexGridSizer->Add (WidestPathSlider, 0, wxALL|wxEXPAND, 5);
	// Textual results beneath the two graph sliders.
	MaximumSpanningTreeResult = new wxStaticText(
		this, wxID_ANY, wxEmptyString);

	WidestPathFlexGridSizer->Add(
		MaximumSpanningTreeResult, 0, wxALL | wxEXPAND, 5);

	WidestPathResult = new wxStaticText(
		this, wxID_ANY, wxEmptyString);

WidestPathFlexGridSizer->Add(
    WidestPathResult, 0, wxALL | wxEXPAND, 5);
	FillerPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	WidestPathFlexGridSizer->Add (FillerPanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* FillerBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	FillerPanel->SetSizer (FillerBoxSizer);

	FillerBoxSizer->Add (0, 0, 1, wxALL, 5);

	ClosePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);

	WidestPathFlexGridSizer->Add(ClosePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));

	wxBoxSizer* CloseBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	ClosePanel->SetSizer(CloseBoxSizer);

	CloseBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

	SaveMaximumSpanningTreeButton = new wxButton(ClosePanel, wxID_ANY, _("Save spanning tree"), wxDefaultPosition, wxDLG_UNIT(ClosePanel, wxSize(-1,-1)), 0);

	CloseBoxSizer->Add(SaveMaximumSpanningTreeButton, 0, wxALL, WXC_FROM_DIP(5));

	CloseBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

	SaveWidestPathButton = new wxButton(ClosePanel, wxID_ANY, _("Save widest path"), wxDefaultPosition, wxDLG_UNIT(ClosePanel, wxSize(-1,-1)), 0);

	CloseBoxSizer->Add(SaveWidestPathButton, 0, wxALL, WXC_FROM_DIP(5));

	CloseBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

	CloseButton = new wxButton(ClosePanel, wxID_ANY, _("Close"), wxDefaultPosition, wxDLG_UNIT(ClosePanel, wxSize(-1,-1)), 0);
	CloseButton->SetDefault();

	CloseBoxSizer->Add(CloseButton, 0, wxALL, WXC_FROM_DIP(5));

	WidestPathDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (WidestPathDrawingCanvasMenuBar);

	WidestPathDrawingCanvasHelpMenu = new wxMenu();
	WidestPathDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	WidestPathDrawingCanvasHelpMenuItem = new wxMenuItem (WidestPathDrawingCanvasHelpMenu, ID_WIDESTPATH_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	WidestPathDrawingCanvasHelpMenu->Append (WidestPathDrawingCanvasHelpMenuItem);
	WidestPathDrawingCanvasMenuBar->Append (WidestPathDrawingCanvasHelpMenu, _ ("&Help"));

	MaximumSpanningTreeGraph->setSliderAddress (MaximumSpanningTreeSlider);
	WidestPathGraph->setSliderAddress (WidestPathSlider);

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	if (GetParent() )
	{
		CentreOnParent (wxBOTH);
	}
	else
	{
		CentreOnScreen (wxBOTH);
	}

#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this) )
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif
	// Connect events
	SaveMaximumSpanningTreeButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WidestPathDrawingCanvas::OnSaveSpanningTreeButtonPress), NULL, this);
	SaveWidestPathButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WidestPathDrawingCanvas::OnSaveWidestPathButtonPress), NULL, this);
	CloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (WidestPathDrawingCanvas::OnWidestPathDrawingCanvasExitMenuItemSelected), NULL, this);
	MaximumSpanningTreeSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (WidestPathDrawingCanvas::OnMaximumSpanningTreeSliderCmdScroll), NULL, this);
	WidestPathSlider->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler (WidestPathDrawingCanvas::OnWidestPathSliderCmdScroll), NULL, this);
	Connect (ID_WIDESTPATH_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (WidestPathDrawingCanvas::OnWidestPathDrawingCanvasHelpMenuItemSelected));
	dirty = true;
	haveAskedAboutSavingFile = false;

	// prepareToPrintGraphs();

	printGraphsCanvas->getDirty (&dirty);
}

WidestPathDrawingCanvas::~WidestPathDrawingCanvas()
{
	SaveMaximumSpanningTreeButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WidestPathDrawingCanvas::OnSaveSpanningTreeButtonPress), NULL, this);
	SaveWidestPathButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WidestPathDrawingCanvas::OnSaveWidestPathButtonPress), NULL, this);
	CloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (WidestPathDrawingCanvas::OnWidestPathDrawingCanvasExitMenuItemSelected), NULL, this);
	MaximumSpanningTreeSlider->Disconnect (wxEVT_SCROLL_CHANGED, wxScrollEventHandler (WidestPathDrawingCanvas::OnMaximumSpanningTreeSliderCmdScroll), NULL, this);
	WidestPathSlider->Disconnect (wxEVT_SCROLL_CHANGED, wxScrollEventHandler (WidestPathDrawingCanvas::OnWidestPathSliderCmdScroll), NULL, this);
}

bool WidestPathDrawingCanvas::closeFrame()
{
	/*	wxString message;
		int answer;

		if (dirty && !figureSaved)
		{
			message.Printf (wxT ("Save results?"));

			answer = wxMessageBox (message, wxT ("Widest path algorithm result"),
			                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

			if (answer == wxYES)
			{
				if (!menuHandler->handleSaveAs(3))
					return false;
			}
			else if (answer == wxCANCEL)
				return false;
		}*/

	return true;
}

void WidestPathDrawingCanvas::OnQuit (wxCommandEvent& event)
{
	bool closeTheFrame = false;

	if (!haveAskedAboutSavingFile)
	{
		haveAskedAboutSavingFile = true;
		closeTheFrame = closeFrame();

		if (closeTheFrame)
			((GraphAlgorithmCanvas *)myParent)->Close(true);
		else
		{
			haveAskedAboutSavingFile = false;
			return;
		}
	}
	else
		((GraphAlgorithmCanvas *)myParent)->Close(true);
}

void WidestPathDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
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

void WidestPathDrawingCanvas::OnWidestPathDrawingCanvasExitMenuItemSelected (wxCommandEvent& event)
{
	if (closeFrame())
		Destroy();
}

void WidestPathDrawingCanvas::OnMaximumSpanningTreeSliderCmdScroll (wxScrollEvent& event)
{
	MaximumSpanningTreeGraph->SetScale (double (MaximumSpanningTreeSlider->GetValue()) /50);
	MaximumSpanningTreeGraph->Refresh (false);
}

void WidestPathDrawingCanvas::OnWidestPathSliderCmdScroll (wxScrollEvent& event)
{
	WidestPathGraph->SetScale (double (WidestPathSlider->GetValue()) /50);
	WidestPathGraph->Refresh (false);
}

void WidestPathDrawingCanvas::OnWidestPathDrawingCanvasHelpMenuItemSelected(wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}

/*void WidestPathDrawingCanvas::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,2);
}*/

void WidestPathDrawingCanvas::SetResults(
    const wxString& treeResult,
    const wxString& pathResult)
{
    MaximumSpanningTreeResult->SetLabel(treeResult);
    WidestPathResult->SetLabel(pathResult);

    Layout();
}

void WidestPathDrawingCanvas::DrawMaximumSpanningTree()
{
	buildMaxSpanningTree->setDrawingCanvas (MaximumSpanningTreeGraph);
	MaximumSpanningTreeGraph->setGA (buildMaxSpanningTree->getGraphAttributes() );
	drawMaxSpanningTree = new DrawGraph (buildMaxSpanningTree->getGraphAttributes(), buildMaxSpanningTree->getGraph(), MaximumSpanningTreeGraph->getSFDiagramManager(),
	                                     MaximumSpanningTreeGraph, undirw, hierarchical);
	drawMaxSpanningTree->draw(false,true);
	MaximumSpanningTreeGraph->RefreshCanvas (true, MaximumSpanningTreeGraph->GetRect() );
	MaximumSpanningTreeGraph->Refresh (false);
	// delete drawMaxSpanningTree;
}

void WidestPathDrawingCanvas::DrawWidestPath()
{
	buildWidestPath->setDrawingCanvas (WidestPathGraph);
	WidestPathGraph->setGA (buildWidestPath->getGraphAttributes());
	buildWidestPathGraph = new DrawGraph (buildWidestPath->getGraphAttributes(), buildWidestPath->getGraph(),
	                                      WidestPathGraph->getSFDiagramManager(),
	                                      WidestPathGraph, undirw, hierarchical);
	buildWidestPathGraph->draw(false, true);
	WidestPathGraph->RefreshCanvas (true, WidestPathGraph->GetRect());
	WidestPathGraph->Refresh (false);
	// delete buildWidestPathGraph;
}

void WidestPathDrawingCanvas::prepareToPrintGraphs()
{
	combinedGraphs->setDrawingCanvas (printGraphsCanvas);
	printGraphsCanvas->setGA (combinedGraphs->getGraphAttributes() );
	printGraphs = new DrawGraph (combinedGraphs->getGraphAttributes(), combinedGraphs->getGraph(), printGraphsCanvas->getSFDiagramManager(),
	                             printGraphsCanvas, undirw, hierarchical);
	printGraphs->draw();
	printGraphsCanvas->RefreshCanvas (true, printGraphsCanvas->GetRect());
	printGraphsCanvas->Refresh (false);
	delete printGraphs;
}

void WidestPathDrawingCanvas::OnSaveSpanningTreeButtonPress(wxCommandEvent& event)
{
	wxString formatSelection;

	// common to all save formats
	formatSelection.Append ("PNG file (*.png)|*.png|" );
	formatSelection.Append ("BMP file (*.bmp)|*.bmp|" );
	formatSelection.Append ("JPG file (*.jpg)|*.jpg" );

	wxString filename = wxFileSelector ("Select a format.",wxEmptyString,
        wxEmptyString, wxEmptyString, formatSelection,
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (filename.empty() )
		return;

	wxString extension = filename.AfterLast ('.');

	if (extension == wxT(""))
	{
		//  extension not specified
		wxMessageBox (wxT ("An extension must be added to the file name."),
		   wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
	}

	extension = extension.MakeLower();
	// fileName = wxT ("");
	if (extension == wxT("bmp"))
        MaximumSpanningTreeGraph->SaveCanvasToGraphicFormat (DrawingCanvas::BMP, filename);
	else if (extension == wxT("jpg"))
        MaximumSpanningTreeGraph->SaveCanvasToGraphicFormat (DrawingCanvas::JPG, filename);
	else if (extension == wxT ("png") )
        MaximumSpanningTreeGraph->SaveCanvasToGraphicFormat (DrawingCanvas::PNG, filename);
	else
		wxMessageBox (wxT ("Unsupported graphics file format."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
}

void WidestPathDrawingCanvas::OnSaveWidestPathButtonPress(wxCommandEvent& event)
{
	wxString formatSelection;

	// common to all save formats
	formatSelection.Append ("PNG file (*.png)|*.png|" );
	formatSelection.Append ("BMP file (*.bmp)|*.bmp|" );
	formatSelection.Append ("JPG file (*.jpg)|*.jpg" );

	wxString filename = wxFileSelector ("Select a format.",wxEmptyString,
        wxEmptyString, wxEmptyString, formatSelection,
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (filename.empty() )
		return;

	wxString extension = filename.AfterLast ('.');

	if (extension == wxT(""))
	{
		//  extension not specified
		wxMessageBox (wxT ("An extension must be added to the file name."),
		   wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
	}

	extension = extension.MakeLower();
	// fileName = wxT ("");
	if (extension == wxT("bmp"))
		WidestPathGraph->SaveCanvasToGraphicFormat (DrawingCanvas::BMP, filename);
	else if (extension == wxT("jpg"))
		WidestPathGraph->SaveCanvasToGraphicFormat (DrawingCanvas::JPG, filename);
	else if (extension == wxT ("png") )
		WidestPathGraph->SaveCanvasToGraphicFormat (DrawingCanvas::PNG, filename);
	else
        wxMessageBox (wxT ("Unsupported graphics file format."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
}