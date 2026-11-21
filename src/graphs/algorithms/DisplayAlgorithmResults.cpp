/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/DisplayAlgorithmResults.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"

#include "DiscreteMathCalculatorMain.h"
#include "common/DrawingCanvas.h"
#include "common/ImageFiles.h"
// #include "images/discmathcalc.xpm"
#include "graphs/algorithms/GraphAlgorithmCanvas.h"
#include "graphs/algorithms/DigraphAlgorithmCanvas.h"

//(*InternalHeadersPCH(DisplayAlgorithmResults)
#include <wx/intl.h>
#include <wx/string.h>
//*)
//(*InternalHeaders(DisplayAlgorithmResults)

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/stdpaths.h>

//*)
//(*IdInit(DisplayAlgorithmResults)

const long DisplayAlgorithmResults::ID_FRAME_ALGORITHM_FIGURE = wxNewId();
const long DisplayAlgorithmResults::ID_FRAME_FIGURE_DFSPATH = wxNewId();
const long DisplayAlgorithmResults::ID_FRAME_FIGURE_BFSTREE = wxNewId();
const long DisplayAlgorithmResults::ID_FRAME_FIGURE_BFSPATH = wxNewId();
const long DisplayAlgorithmResults::ID_GRAPH_DRAWING = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_SLIDER = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_CLOSE_BUTTON = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_SAVEAS_FIGURE_AND_ALGORITHM_TEXT_BUTTON = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_PANEL = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_STATUSBAR = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_PRINT_MENUITEM = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_CANVAS_PRINT_PREVIEW_MENUITEM = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_CANVAS_PAGESETUP_MENUITEM = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_CANVAS_SAVE_AS_MENUITEM = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_CANVAS_EXIT_MENUITEM = wxNewId();
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_COPY_IMAGE_MENUITEM = wxNewId();
const long DisplayAlgorithmResults::ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM = wxNewId();
//*)
const long DisplayAlgorithmResults::ID_ALGORITHM_RESULTS_STATICLINE = wxNewId();

BEGIN_EVENT_TABLE (DisplayAlgorithmResults,wxFrame)
	EVT_CLOSE (DisplayAlgorithmResults::OnCloseWindow)
	// EVT_MENU (wxID_EXIT, DisplayAlgorithmResults::OnQuit)
END_EVENT_TABLE()

DisplayAlgorithmResults::DisplayAlgorithmResults (wxWindow* parent, BuildGraph *builtGraph, GraphType type, GraphLayout layout, Algorithm myAlgorithmType,
      const wxString& displayAlgorithmResulsLenLabel, wxWindowID id, const wxPoint& pos,const wxSize& size) :
	builtDisplayAlgorithmResults (builtGraph), graphType (type), graphLayout (layout), algorithmType (myAlgorithmType), myParent (parent),
	hideTextOutput(false)
{
	//(*Initialize(DisplayAlgorithmResults)
	wxFlexGridSizer* displayAlgorithmResultsFlexGridSizer;
	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T ("wxID_ANY") );
	SetClientSize (wxSize (1024,900) );
	{
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap (wxBitmap (wxImage (imagefile ("discmathcalc.xpm") ) ) );
		SetIcon (FrameIcon);
	}
	displayAlgorithmResultsPanel = new wxPanel (this, ID_ALGORITHM_RESULTS_PANEL, wxPoint (224,216), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ALGORITHM_RESULTS_PANEL") );
	displayAlgorithmResultsPanel->SetBackgroundColour (*wxWHITE);

	displayAlgorithmResultsFlexGridSizer = new wxFlexGridSizer (8, 1, 0, 0);
	displayAlgorithmResultsFlexGridSizer->AddGrowableCol (0);
	displayAlgorithmResultsFlexGridSizer->AddGrowableRow (2);

	wxString staticText;
	doNotSaveAsGML = true;
	switch (algorithmType)
	{
	case DFSTREE:
		if (graphType == digraph || graphType == digraphw)
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("DFS-Tree-digraph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}
		else
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("DFS-Tree-graph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}

		staticText = wxT ("DFS results");
		break;

	case DFSPATH:
		if (graphType == digraph || graphType == digraphw)
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("DFS-Path-digraph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}
		else
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("DFS-Path-graph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}

		staticText = wxT ("DFS path results");
		break;

	case BFSTREE:
		if (graphType == digraph || graphType == digraphw)
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("BFS-Tree-digraph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}
		else
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("BFS-Tree-graph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}

		staticText = wxT ("BFS results");
		break;

	case BFSPATH:
		if (graphType == digraph || graphType == digraphw)
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("BFS-Path-digraph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}
		else
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("BFS-Path-graph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
			useStandardColor = true;
		}

		staticText = wxT ("BFS path results");
		break;

	case COMPONENTS:
		if (graphType == digraph || graphType == digraphw)
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("Strong-components.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		}
		else
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("Graph-components.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		}
		break;

	case EULERTOUR:
		if (graphType == digraph || graphType == digraphw)
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("DigraphEuler.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		}
		else
		{
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("Euler.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
			displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		}
		break;

	case STABLEMARRIAGE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("stable-marriage.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case CLIQUE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("clique.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case INDEPENDENTSET:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("independent-set.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case VERTEXCOVER:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("vertexcover.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case HAMILTONIAN_PATH:

	case HAMILTONIAN_CYCLE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("hamcycle.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		useStandardColor = true;
	}
	break;

	case MINIMUM_PATH:
	{
		if (graphType == undirw || graphType == undir)
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY,
			                                  wxBitmap (wxImage (imagefile ("Dijkstra-undir.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		else
			frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY,
			                                  wxBitmap (wxImage (imagefile ("Dijkstra-digraph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		useStandardColor = true;
	}
	staticText = wxT ("Minimum path");
	break;

	case PRIM:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("Prim.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		useStandardColor = true;
	}
	break;

	case GRAPHCOLORING:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("coloredPetersonGraph.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case TSP:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("salesman.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	staticText = wxT ("Optimal path");
	break;

	case MAXFLOW:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("MaximumFlow.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case CARDMATCHING:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("cardmatching.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	staticText = wxT ("Maximum Independent Edge Set");
	break;

	case WEIGHTEDMATCHING:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("weightedmatching.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	staticText = wxT ("Maximum Weighted Independent Edge Set");
	break;

	case REFLEXIVECLOSURE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("reflexiveclosure.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case SYMMETRICCLOSURE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("symmetricclosure.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case TRANSITIVECLOSURE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("transitiveclosure.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case MINCOSTARBORESCENCE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("mincostarborescence.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case LONGESTPATH:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("longestpath.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;

	case GRPHSEQUENCE:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile ("GraphSequence.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
		useStandardColor = true;
	}
	break;

	case GENERALINFORMATION:
	{
		frameFigure = new wxStaticBitmap (displayAlgorithmResultsPanel, wxID_ANY, wxBitmap (wxImage (imagefile (" GraphSequence.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
		displayAlgorithmResultsFlexGridSizer->Add (frameFigure, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}
	break;
	}

	myLineSeparator = new wxStaticLine (displayAlgorithmResultsPanel, ID_ALGORITHM_RESULTS_STATICLINE, wxDefaultPosition, wxSize (3,-1), wxLI_HORIZONTAL, _T ("ID_ALGORITHM_RESULTS_STATICLINE") );
	displayAlgorithmResultsFlexGridSizer->Add (myLineSeparator, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);

	graphDrawing = new DrawingCanvas (&m_canvas, displayAlgorithmResultsPanel, ID_GRAPH_DRAWING, wxDefaultPosition, wxSize(1000,400), wxHSCROLL|wxVSCROLL);
	displayAlgorithmResultsFlexGridSizer->Add (graphDrawing, 1, wxALL|wxEXPAND, 1);
	displayAlgorithmResultsSlider = new wxSlider (displayAlgorithmResultsPanel, ID_ALGORITHM_RESULTS_SLIDER, 50, 2, 99, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ALGORITHM_RESULTS_SLIDER") );
	displayAlgorithmResultsFlexGridSizer->Add (displayAlgorithmResultsSlider, 1, wxALL|wxEXPAND, 1);

	DisplayAlgorithmResultsOutputStaticText = new wxStaticText (displayAlgorithmResultsPanel, wxID_ANY, _ ("Algorithm output"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);
	displayAlgorithmResultsFlexGridSizer->Add (DisplayAlgorithmResultsOutputStaticText, 0, wxALL, WXC_FROM_DIP (5) );
	displayAlgorithmResultsOutputTextCtrl = new wxTextCtrl (displayAlgorithmResultsPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,110) ), wxTE_PROCESS_TAB|wxTE_MULTILINE|wxHSCROLL);
#if wxVERSION_NUMBER >= 3000
	displayAlgorithmResultsOutputTextCtrl->SetHint (wxT ("") );
#endif
	displayAlgorithmResultsFlexGridSizer->Add (displayAlgorithmResultsOutputTextCtrl, 1, wxALL|wxEXPAND, 1);
	if (hideTextOutput)
	{
		DisplayAlgorithmResultsOutputStaticText->Hide();
		displayAlgorithmResultsOutputTextCtrl->Hide();
	}

	saveAsFigure = new wxButton (displayAlgorithmResultsPanel, ID_ALGORITHM_SAVEAS_FIGURE_AND_ALGORITHM_TEXT_BUTTON, _ ("Save figure and text results..."),
        wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ALGORITHM_SAVEAS_FIGURE_AND_ALGORITHM_TEXT_BUTTON") );
	displayAlgorithmResultsFlexGridSizer->Add (saveAsFigure, 1, wxALL|wxALIGN_RIGHT, 1);
	// Stable Marriage does not support saving figure and text results.
	if (algorithmType == STABLEMARRIAGE)
	{
		saveAsFigure->Hide();
	}

	displayAlgorithmResultsCloseButton = new wxButton (displayAlgorithmResultsPanel, ID_ALGORITHM_RESULTS_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ALGORITHM_RESULTS_CLOSE_BUTTON") );
	displayAlgorithmResultsFlexGridSizer->Add (displayAlgorithmResultsCloseButton, 1, wxALL|wxALIGN_RIGHT, 1);
	displayAlgorithmResultsPanel->SetSizer (displayAlgorithmResultsFlexGridSizer);
	displayAlgorithmResultsFlexGridSizer->Fit (displayAlgorithmResultsPanel);
	displayAlgorithmResultsFlexGridSizer->SetSizeHints (displayAlgorithmResultsPanel);
	displayAlgorithmResultsMenuBar = new wxMenuBar (0);
	this->SetMenuBar (displayAlgorithmResultsMenuBar);

	displayAlgorithmResultsFileMenu = new wxMenu();
	displayAlgorithmResultsMenuBar->Append (displayAlgorithmResultsFileMenu, _ ("File") );

	displayAlgorithmResultsPrintMenuItem = new wxMenuItem (displayAlgorithmResultsFileMenu, wxID_ANY,wxT("Print...\tCTRL+P"), wxEmptyString, wxITEM_NORMAL);
	displayAlgorithmResultsFileMenu->Append (displayAlgorithmResultsPrintMenuItem);

	displayAlgorithmResultsPrintPreviewMenuItem = new wxMenuItem (displayAlgorithmResultsFileMenu, wxID_ANY, _ ("Print Preview...\tSHIFT-CTRL-P"), wxT (""), wxITEM_NORMAL);
	// displayAlgorithmResultsFileMenu->Append (displayAlgorithmResultsPrintPreviewMenuItem);

	displayAlgorithmResultsPageSetupMenuItem = new wxMenuItem (displayAlgorithmResultsFileMenu, wxID_ANY, _ ("Page Setup..."), wxT (""), wxITEM_NORMAL);
	displayAlgorithmResultsFileMenu->Append (displayAlgorithmResultsPageSetupMenuItem);

	displayAlgorithmResultsSaveAsMenuItem = new wxMenuItem (displayAlgorithmResultsFileMenu, wxID_ANY,
	      wxT("Save As...\tCTRL-SHIFT-S"), wxT (""), wxITEM_NORMAL);
	// displayAlgorithmResultsFileMenu->Append (displayAlgorithmResultsSaveAsMenuItem);

	displayAlgorithmResultsEditMenu = new wxMenu();
	displayAlgorithmResultsMenuBar->Append (displayAlgorithmResultsEditMenu, _ ("Edit") );

	displayAlgorithmResultsCopyImageMenuItem = new wxMenuItem (displayAlgorithmResultsEditMenu, wxID_ANY, _ ("Copy Image...\tCTRL-C"), wxT (""), wxITEM_NORMAL);
	displayAlgorithmResultsEditMenu->Append (displayAlgorithmResultsCopyImageMenuItem);

	displayAlgorithmResultsHelpMenu = new wxMenu();
	displayAlgorithmResultsHelpMenu->Append (wxID_ABOUT);
	displayAlgorithmResultsHelpMenuItem = new wxMenuItem (displayAlgorithmResultsHelpMenu, ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	displayAlgorithmResultsHelpMenu->Append (displayAlgorithmResultsHelpMenuItem);
	displayAlgorithmResultsMenuBar->Append (displayAlgorithmResultsHelpMenu, _ ("&Help") );

	displayAlgorithmResultsStatusBar = new wxStatusBar (this, ID_ALGORITHM_RESULTS_STATUSBAR, 0, _T ("ID_ALGORITHM_RESULTS_STATUSBAR") );
	int __wxStatusBarWidths_1[1] = { -10 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	displayAlgorithmResultsStatusBar->SetFieldsCount (1,__wxStatusBarWidths_1);
	displayAlgorithmResultsStatusBar->SetStatusStyles (1,__wxStatusBarStyles_1);
	SetStatusBar (displayAlgorithmResultsStatusBar);
	//*)

	Connect (ID_ALGORITHM_SAVEAS_FIGURE_AND_ALGORITHM_TEXT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &DisplayAlgorithmResults::OndisplayAlgorithmResultsSaveAsFigureAndTextButtonClick);
	displayAlgorithmResultsCloseButton->Bind(
    wxEVT_BUTTON,
    &DisplayAlgorithmResults::OndisplayAlgorithmResultsCloseButtonClick,this);
	Connect (ID_ALGORITHM_RESULTS_SLIDER,wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction) &DisplayAlgorithmResults::OndisplayAlgorithmResultsSliderCmdScroll);
	this->Connect (displayAlgorithmResultsPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (DisplayAlgorithmResults::handleMenuChoice), NULL, this);
	this->Connect (displayAlgorithmResultsPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (DisplayAlgorithmResults::handleMenuChoice), NULL, this);
	this->Connect (displayAlgorithmResultsCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (DisplayAlgorithmResults::handleMenuChoice), NULL, this);

	Connect (ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (DisplayAlgorithmResults::OnDisplayAlgorithmResultsHelpMenuItemSelected) );

	graphDrawing->setSliderAddress (displayAlgorithmResultsSlider);
	figureSaved = false;
	menuHandler = new MenuEventHandler (graphDrawing,
	                                    displayAlgorithmResultsPrintMenuItem->GetId(),
	                                    displayAlgorithmResultsPrintPreviewMenuItem->GetId(),
	                                    displayAlgorithmResultsPageSetupMenuItem->GetId(),
	                                    displayAlgorithmResultsSaveAsMenuItem->GetId(),
	                                    displayAlgorithmResultsCopyImageMenuItem->GetId(),
	                                    &figureSaved, graphType);

	haveAskedAboutSavingFile = false;
	displayResultsTitle = wxT ("Algorithm Results");
	SetLabel (displayResultsTitle);
	GA = builtDisplayAlgorithmResults->getGraphAttributes();
	graphDrawing->setGA (GA);
	G = builtDisplayAlgorithmResults->getGraph();
	drawDisplayAlgorithmResults = new DrawGraph (GA, G, &m_canvas, graphDrawing,
	      graphType, graphLayout);
	if (useStandardColor)
		drawDisplayAlgorithmResults->draw(false,true);
	else
		drawDisplayAlgorithmResults->draw();
	// Make Stable Marriage and closure results windows taller.
	if (algorithmType == STABLEMARRIAGE ||
		algorithmType == REFLEXIVECLOSURE ||
		algorithmType == SYMMETRICCLOSURE ||
		algorithmType == TRANSITIVECLOSURE)
	{
    SetClientSize(wxSize(1024, 1050));
	}

    Centre();
}

DisplayAlgorithmResults::~DisplayAlgorithmResults()
{
	Disconnect (ID_ALGORITHM_SAVEAS_FIGURE_AND_ALGORITHM_TEXT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &DisplayAlgorithmResults::OndisplayAlgorithmResultsSaveAsFigureAndTextButtonClick);
	displayAlgorithmResultsCloseButton->Unbind(
		wxEVT_BUTTON,
		&DisplayAlgorithmResults::OndisplayAlgorithmResultsCloseButtonClick,
		this
	);
	Disconnect (ID_ALGORITHM_RESULTS_SLIDER,wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction) &DisplayAlgorithmResults::OndisplayAlgorithmResultsSliderCmdScroll);
	this->Disconnect (displayAlgorithmResultsPrintMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (DisplayAlgorithmResults::handleMenuChoice), NULL, this);
	this->Disconnect (displayAlgorithmResultsPageSetupMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (DisplayAlgorithmResults::handleMenuChoice), NULL, this);
	this->Disconnect (displayAlgorithmResultsCopyImageMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (DisplayAlgorithmResults::handleMenuChoice), NULL, this);
}

void DisplayAlgorithmResults::writeOutputLine (const wxString& line)
{
	displayAlgorithmResultsOutputTextCtrl->WriteText (line);
}

wxTextCtrl*  DisplayAlgorithmResults::getDisplayAlgorithmResultsTextCtrl ()
{
	return displayAlgorithmResultsOutputTextCtrl;
}

wxStaticText * DisplayAlgorithmResults::getDisplayAlgorithmResultsStaticText()
{
	return DisplayAlgorithmResultsOutputStaticText;
}

void DisplayAlgorithmResults::OndisplayAlgorithmResultsCloseButtonClick (wxCommandEvent& event)
{
    if (closeFrame())
        Destroy();
}

void DisplayAlgorithmResults::OndisplayAlgorithmResultsSliderCmdScroll (wxScrollEvent& event)
{
	graphDrawing->SetScale (double (displayAlgorithmResultsSlider->GetValue() ) /50);
	graphDrawing->Refresh (false);
}

void DisplayAlgorithmResults::handleMenuChoice (wxCommandEvent& event)
{
	menuHandler->process (event,3);
}

bool DisplayAlgorithmResults::closeFrame()
{
    // Stable Marriage has no Save Figure and Text Results button.
    // Allow its results window to close without asking about saving.
    if (algorithmType == STABLEMARRIAGE)
        return true;

	wxString message;
	int answer;

	if (!haveAskedAboutSavingFile)
	{
        message.Printf (wxT ("Do you want to use the button to save figure?") );

		answer = wxMessageBox (message, wxT ("Algorithm result"),
		                       wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
            return false;
        else if (answer == wxNO)
            return true;
		else
			return false;
	}

    return true;
}

void DisplayAlgorithmResults::OnCloseWindow (wxCloseEvent& event)
{
	bool closeTheFrame;

	if (!haveAskedAboutSavingFile)
	{
		closeTheFrame = closeFrame();

		if (closeTheFrame)
			Destroy();
		else
			return;
	}
    else
        Destroy();
}

void DisplayAlgorithmResults::OnDisplayAlgorithmResultsHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}

void DisplayAlgorithmResults::OndisplayAlgorithmResultsSaveAsFigureAndTextButtonClick (wxCommandEvent& event)
{
	wxString formatSelection;
    wxString textOfAlgorithmResult, str;
    wxTextFile textOfAlgorithmResultOut;

	formatSelection.Append ("PNG file (*.png)|*.png|" );
	formatSelection.Append ("BMP file (*.bmp)|*.bmp|" );
	formatSelection.Append ("JPG file (*.jpg)|*.jpg" );

wxString defaultDirectory = wxStandardPaths::Get().GetDocumentsDir();

	wxString filename = wxFileSelector(
	    wxT("Select a name for the graphics file."),
	    defaultDirectory,
	    wxT("Untitled"),
	    wxT("png"),
	    formatSelection,
	    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (filename.empty() )
    {
        haveAskedAboutSavingFile = false;
		return;
    }

	wxString extension = filename.AfterLast ('.');

	if (extension == wxT(""))
	{
		//  extension not specified
		wxMessageBox (wxT ("An extension must be added to the file name."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
        haveAskedAboutSavingFile = false;
        return;
	}

	extension = extension.MakeLower();
    if (extension == wxT("bmp"))
		graphDrawing->SaveCanvasToGraphicFormat (DrawingCanvas::BMP, filename);
	else if (extension == wxT("jpg"))
		graphDrawing->SaveCanvasToGraphicFormat (DrawingCanvas::JPG, filename);
	else if (extension == wxT ("png") )
		graphDrawing->SaveCanvasToGraphicFormat (DrawingCanvas::PNG, filename);
	else
    {
		wxMessageBox (wxT ("Unsupported graphics file format."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
        haveAskedAboutSavingFile = false;
        return;
    }

    textOfAlgorithmResult = displayAlgorithmResultsOutputTextCtrl->GetValue();
    if (textOfAlgorithmResult.Length() == 0)
    {
        haveAskedAboutSavingFile = true;
        return;
    }

    // strip off the graphics extension
    filename = filename.BeforeLast('.');
    // add ".txt"
    filename = filename + wxT(".txt");
    textOfAlgorithmResultOut.Create(filename);
    
    textOfAlgorithmResultOut.AddLine(textOfAlgorithmResult);
    textOfAlgorithmResultOut.Write();
    textOfAlgorithmResultOut.Close();
    figureSaved = true;
    haveAskedAboutSavingFile = true;
}
