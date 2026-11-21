/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/isomorphism/IsomorphismDrawingCanvas.h"
#include "graphs/algorithms/isomorphism/IsomorphismOutput.h"
#include "DiscreteMathCalculatorMain.h"

const long IsomorphismDrawingCanvas::ID_ISOMORPHISM_DRAWING_CANVAS_HELP_MENUITEM = wxNewId();
const long IsomorphismDrawingCanvas::ID_ISOMORPHISM_DRAWING_CANVAS_SLIDER = wxNewId();

BEGIN_EVENT_TABLE (IsomorphismDrawingCanvas,wxFrame)
	EVT_CLOSE (IsomorphismDrawingCanvas::OnCloseWindow)
	EVT_MENU (wxID_EXIT, IsomorphismDrawingCanvas::OnQuit)
END_EVENT_TABLE()

IsomorphismDrawingCanvas::IsomorphismDrawingCanvas (wxWindow* parent, bool subgrIsomorphism, GraphLayout currLayout, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), myParent (parent), myGraph1 (NULL), myGraph2 (NULL), subgraphIsomorphism(subgrIsomorphism)
{
	layout = currLayout;

	wxFlexGridSizer* IsomorphismDrawingCanvasFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	IsomorphismDrawingCanvasFlexGridSizer->SetFlexibleDirection (wxBOTH);
	IsomorphismDrawingCanvasFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	IsomorphismDrawingCanvasFlexGridSizer->AddGrowableCol (0);
	IsomorphismDrawingCanvasFlexGridSizer->AddGrowableRow (0);
	IsomorphismDrawingCanvasFlexGridSizer->AddGrowableRow (1);
	IsomorphismDrawingCanvasFlexGridSizer->AddGrowableRow (2);
	this->SetSizer (IsomorphismDrawingCanvasFlexGridSizer);

	if (subgrIsomorphism)
		SetTitle(wxT("Test for Subgraph Isomorphsim"));
	else
		SetTitle(wxT("Test for Graph Isomorphsim"));

	SetClientSize (wxSize (1280,720));
	wxPanel* graphsPanel;

	graphsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,500), wxTAB_TRAVERSAL);

	IsomorphismDrawingCanvasFlexGridSizer->Add (graphsPanel, 2, wxALL|wxEXPAND, 5);

	wxBoxSizer* graphsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	graphsPanel->SetSizer (graphsBoxSizer);

	graphDrawing1 = new DrawingCanvas (&m_canvas1, graphsPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,500), wxHSCROLL|wxVSCROLL);
	graphDrawing1->SetScrollRate (5, 5);
	graphsBoxSizer->Add ((wxScrolledWindow *)(m_canvas1.GetShapeCanvas()),1,wxALL|wxEXPAND, 1);
	graphDrawing2 = new DrawingCanvas (&m_canvas2, graphsPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,500), wxHSCROLL|wxVSCROLL);
	graphDrawing2->SetScrollRate (5, 5);
	graphsBoxSizer->Add ((wxScrolledWindow *)(m_canvas2.GetShapeCanvas()), 1, wxALL|wxEXPAND, 1);

	IsomorphismDrawingCanvasSlider = new wxSlider (this, ID_ISOMORPHISM_DRAWING_CANVAS_SLIDER, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);
	IsomorphismDrawingCanvasFlexGridSizer->Add (IsomorphismDrawingCanvasSlider, 1, wxALL|wxEXPAND, 5);

	IsomorphismDrawingCanvasTestPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,150), wxTAB_TRAVERSAL);
	IsomorphismDrawingCanvasFlexGridSizer->Add (IsomorphismDrawingCanvasTestPanel, 0, wxALL|wxEXPAND, 5);

	wxGridSizer* IsomorphismDrawingCanvasTestPanelGridSizer = new wxGridSizer (2, 3, 0, 0);
	IsomorphismDrawingCanvasTestPanel->SetSizer (IsomorphismDrawingCanvasTestPanelGridSizer);

	wxArrayString IsomorphismDrawingCanvasInputG1RadioBoxArr;
	IsomorphismDrawingCanvasInputG1RadioBoxArr.Add (_ ("GML"));
	IsomorphismDrawingCanvasInputG1RadioBoxArr.Add (_ ("GRPH"));
	IsomorphismDrawingCanvasInputG1RadioBox = new wxRadioBox (IsomorphismDrawingCanvasTestPanel, wxID_ANY, _ ("Input format"), wxDefaultPosition, wxSize (-1,80), IsomorphismDrawingCanvasInputG1RadioBoxArr, 1, 0);
	IsomorphismDrawingCanvasInputG1RadioBox->SetSelection (0);

	IsomorphismDrawingCanvasTestPanelGridSizer->Add (IsomorphismDrawingCanvasInputG1RadioBox, 0, wxALL, 5);

	wxArrayString IsomorphismDrawingCanvasInputG2RadioBoxArr;
	IsomorphismDrawingCanvasInputG2RadioBoxArr.Add (_ ("GML"));
	IsomorphismDrawingCanvasInputG2RadioBoxArr.Add (_ ("GRPH"));
	IsomorphismDrawingCanvasInputG2RadioBox = new wxRadioBox (IsomorphismDrawingCanvasTestPanel, wxID_ANY, _ ("Input format"), wxDefaultPosition, wxSize (-1,80), IsomorphismDrawingCanvasInputG2RadioBoxArr, 1, 0);
	IsomorphismDrawingCanvasInputG2RadioBox->SetSelection (0);

	IsomorphismDrawingCanvasTestPanelGridSizer->Add (IsomorphismDrawingCanvasInputG2RadioBox, 0, wxALL, 5);

	IsomorphismDrawingCanvasTestPanelGridSizer->Add (0, 0, 1, wxALL, 5);

	IsomorphismDrawingCanvasInputG1Button = new wxButton (IsomorphismDrawingCanvasTestPanel, wxID_ANY, _ ("Input undir G1"), wxDefaultPosition, wxSize (-1,-1), 0);

	IsomorphismDrawingCanvasTestPanelGridSizer->Add (IsomorphismDrawingCanvasInputG1Button, 0, wxALL, 35);

	IsomorphismDrawingCanvasInputG2Button = new wxButton (IsomorphismDrawingCanvasTestPanel, wxID_ANY, _ ("Input undir G2"), wxDefaultPosition, wxSize (-1,-1), 0);

	IsomorphismDrawingCanvasTestPanelGridSizer->Add (IsomorphismDrawingCanvasInputG2Button, 0, wxALL, 35);

	IsomorphismDrawingCanvasRunIsomorphismTestButton = new wxButton (IsomorphismDrawingCanvasTestPanel, wxID_ANY, _ ("Run isomorphism test"), wxDefaultPosition, wxSize (-1,-1), 0);

	IsomorphismDrawingCanvasTestPanelGridSizer->Add (IsomorphismDrawingCanvasRunIsomorphismTestButton, 0, wxALL, 35);

	IsomorphismDrawingCanvasCloseCancelPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,50), wxTAB_TRAVERSAL);

	IsomorphismDrawingCanvasFlexGridSizer->Add (IsomorphismDrawingCanvasCloseCancelPanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* IsomorphismDrawingCanvasCloseCancelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	IsomorphismDrawingCanvasCloseCancelPanel->SetSizer (IsomorphismDrawingCanvasCloseCancelBoxSizer);

	IsomorphismDrawingCanvasCloseCancelBoxSizer->Add (0, 0, 1, wxALL, 5);

	IsomorphismDrawingCanvasCancelButton = new wxButton (IsomorphismDrawingCanvasCloseCancelPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);

	IsomorphismDrawingCanvasCloseCancelBoxSizer->Add (IsomorphismDrawingCanvasCancelButton, 0, wxALL, 5);

	IsomorphismDrawingCanvasCloseButton = new wxButton (IsomorphismDrawingCanvasCloseCancelPanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);
	IsomorphismDrawingCanvasCloseButton->SetDefault();

	IsomorphismDrawingCanvasCloseCancelBoxSizer->Add (IsomorphismDrawingCanvasCloseButton, 0, wxALL, 5);

	IsomorphismDrawingCanvasMenuBar = new wxMenuBar (0);
	this->SetMenuBar (IsomorphismDrawingCanvasMenuBar);

	IsomorphismDrawingCanvasHelpMenu = new wxMenu();
	IsomorphismDrawingCanvasHelpMenu->Append (wxID_ABOUT);
	IsomorphismDrawingCanvasHelpMenuItem = new wxMenuItem (IsomorphismDrawingCanvasHelpMenu, ID_ISOMORPHISM_DRAWING_CANVAS_HELP_MENUITEM, _ ("View Help\tF1"), _ ("Obtain help for the application."), wxITEM_NORMAL);
	IsomorphismDrawingCanvasHelpMenu->Append (IsomorphismDrawingCanvasHelpMenuItem);
	IsomorphismDrawingCanvasMenuBar->Append (IsomorphismDrawingCanvasHelpMenu, _ ("&Help"));

	graphDrawing1->setSliderAddress (IsomorphismDrawingCanvasSlider);
	graphDrawing2->setSliderAddress (IsomorphismDrawingCanvasSlider);

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
	Connect (ID_ISOMORPHISM_DRAWING_CANVAS_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction) &IsomorphismDrawingCanvas::OnIsomorphismDrawingCanvasSliderCmdScroll);
	IsomorphismDrawingCanvasInputG1Button->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnInputG1ButtonPress), NULL, this);
	IsomorphismDrawingCanvasInputG2Button->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnInputG2ButtonPressed), NULL, this);
	IsomorphismDrawingCanvasRunIsomorphismTestButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnRunIsomorphismTestButtonPress), NULL, this);
	IsomorphismDrawingCanvasCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnCancelButtonPress), NULL, this);
	IsomorphismDrawingCanvasCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnCloseButtonPress), NULL, this);
	Connect (ID_ISOMORPHISM_DRAWING_CANVAS_HELP_MENUITEM,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler (IsomorphismDrawingCanvas::OnIsomorphismDrawingCanvasHelpMenuItemSelected));
}

IsomorphismDrawingCanvas::~IsomorphismDrawingCanvas()
{
	IsomorphismDrawingCanvasInputG1Button->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnInputG1ButtonPress), NULL, this);
	IsomorphismDrawingCanvasInputG2Button->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnInputG2ButtonPressed), NULL, this);
	IsomorphismDrawingCanvasRunIsomorphismTestButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnRunIsomorphismTestButtonPress), NULL, this);
	IsomorphismDrawingCanvasCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnCancelButtonPress), NULL, this);
	IsomorphismDrawingCanvasCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismDrawingCanvas::OnCloseButtonPress), NULL, this);
}

void IsomorphismDrawingCanvas::OnInputG1ButtonPress (wxCommandEvent& event)
{
	if (IsomorphismDrawingCanvasInputG1RadioBox->GetSelection() == 0)
		EnterFromGML (1);
	else
		EnterFromGRPH (1);
}

void IsomorphismDrawingCanvas::OnInputG2ButtonPressed (wxCommandEvent& event)
{
	if (IsomorphismDrawingCanvasInputG2RadioBox->GetSelection() == 0)
		EnterFromGML (2);
	else
		EnterFromGRPH (2);
}

void IsomorphismDrawingCanvas::OnRunIsomorphismTestButtonPress (wxCommandEvent& event)
{
	if ((myGraph1 == NULL || (myGraph2 == NULL)))
	{
		wxMessageDialog dialog (this, wxT("You must define two graphs before testing for isomorphism."), wxT("Note"), wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return;
	}

	if (!subgraphIsomorphism)
	{
		if (myGraph1->getGraph()->numberOfNodes() <= myGraph2->getGraph()->numberOfNodes())
			testForIsomorphism(myGraph1->getGraphAttributes(), myGraph1->getGraph(),
			                   myGraph2->getGraphAttributes(), myGraph2->getGraph());
	}
	else
	{
		if (myGraph1->getGraph()->numberOfNodes() <= myGraph2->getGraph()->numberOfNodes())
			testForSubgraphIsomorphism(myGraph1->getGraphAttributes(), myGraph1->getGraph(),
			                           myGraph2->getGraphAttributes(), myGraph2->getGraph());
	}
}

void IsomorphismDrawingCanvas::OnCancelButtonPress (wxCommandEvent& event)
{
	Destroy();
}

void IsomorphismDrawingCanvas::OnCloseButtonPress (wxCommandEvent& event)
{
	Destroy();
}

void IsomorphismDrawingCanvas::OnQuit (wxCommandEvent& event)
{
	( (DiscreteMathCalculatorFrame *) myParent)->Close (true);
}

void IsomorphismDrawingCanvas::OnCloseWindow (wxCloseEvent& event)
{
	Destroy();
}

void IsomorphismDrawingCanvas::OnIsomorphismDrawingCanvasHelpMenuItemSelected (wxCommandEvent& event)
{
	((DiscreteMathCalculatorFrame *)myParent)->getHelp();
}

void IsomorphismDrawingCanvas::OnIsomorphismDrawingCanvasSliderCmdScroll (wxScrollEvent& event)
{
	graphDrawing1->SetScale (double (IsomorphismDrawingCanvasSlider->GetValue()) /50);
	graphDrawing1->Refresh (false);
	graphDrawing2->SetScale (double (IsomorphismDrawingCanvasSlider->GetValue()) /50);
	graphDrawing2->Refresh (false);
}

void IsomorphismDrawingCanvas::EnterFromGRPH (int graphNo)
{
	GraphType gtype;
	bool parallelEdges;

	wxString filename = wxFileSelector (wxT ("Choose a GRPH file specifying your graph."),wxT (""), wxT (""),wxT (""),
	                                    wxT ("GRPH file (*.grph)|*.grph"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (filename.empty())
		return;

	if (graphNo == 1)
	{
		m_canvas1.Clear();
		myGraph1 = new BuildGraph (wxT (""), undir, layout, false);

		myGraph1->setDrawingCanvas (graphDrawing1);
		myGraph1->buildFromGRPH (filename, false, true);

		graphDrawing1->setGA (myGraph1->getGraphAttributes());
		graphDrawing1->RefreshCanvas (true, graphDrawing1->GetRect());
		graphDrawing1->Refresh (false);
	}
	else
	{
		m_canvas2.Clear();
		myGraph2 = new BuildGraph (wxT (""), undir, layout, false);

		myGraph2->setDrawingCanvas (graphDrawing2);
		myGraph2->buildFromGRPH (filename, false, true);

		graphDrawing2->setGA (myGraph2->getGraphAttributes());
		graphDrawing2->RefreshCanvas (true, graphDrawing2->GetRect());
		graphDrawing2->Refresh (false);
	}
}

void IsomorphismDrawingCanvas::EnterFromGML (int graphNo)
{
	bool parallelEdges;
	GraphType gtype;

	wxString filename = wxFileSelector (wxT ("Choose a GML file specifying your graph."),wxT (""), wxT (""),wxT (""),
	                                    wxT ("GML file (*.gml)|*.gml"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (filename.empty())
		return;

	if (graphNo == 1)
	{
		m_canvas1.Clear();
		myGraph1 = new BuildGraph (wxT (""), undir, layout, false);

		myGraph1->setDrawingCanvas (graphDrawing1);
		myGraph1->buildFromGML (filename, &gtype, &parallelEdges, false);

		graphDrawing1->setGA (myGraph1->getGraphAttributes());
		graphDrawing1->RefreshCanvas (true, graphDrawing1->GetRect());
		graphDrawing1->Refresh (false);
	}
	else
	{
		m_canvas2.Clear();
		myGraph2 = new BuildGraph (wxT (""), undir, layout, false);

		myGraph2->setDrawingCanvas (graphDrawing2);
		myGraph2->buildFromGML (filename, &gtype, &parallelEdges, false);

		graphDrawing2->setGA (myGraph2->getGraphAttributes());
		graphDrawing2->RefreshCanvas (true, graphDrawing2->GetRect());
		graphDrawing2->Refresh (false);
	}
}
