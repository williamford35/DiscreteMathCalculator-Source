#ifndef DISPLAYALGORITHMRESULTS_H
#define DISPLAYALGORITHMRESULTS_H

//(*HeadersPCH(DisplayAlgorithmResults)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>
//*)

#include <wx/statline.h>
#include <wx/textctrl.h>

//(*Headers(DisplayAlgorithmResults)
#include "common/DrawingCanvas.h"
//*)

#include "wx/wxsf/wxShapeFramework.h"
#include "common/MenuEventHandler.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include "graphs/build/BuildGraph.h"
#include "graphs/draw/DrawGraph.h"
#include "common/MenuEventHandler.h"
#include "graphs/GraphType.h"
#include "common/report.h"

using namespace std;
using namespace ogdf;

class DisplayAlgorithmResults: public wxFrame
{
public:
	enum Algorithm
	{
		DFSTREE,
		DFSPATH,
		TOPOLOGICALSORT,
		BFSTREE,
		BFSPATH,
		COMPONENTS,
		EULERTOUR,
		STABLEMARRIAGE,
		CLIQUE,
		INDEPENDENTSET,
		VERTEXCOVER,
		HAMILTONIAN_CYCLE,
		MINIMUM_PATH,
		PRIM,
		GRAPHCOLORING,
		TSP,
		MAXFLOW,
		CARDMATCHING,
		WEIGHTEDMATCHING,
		REFLEXIVECLOSURE,
		SYMMETRICCLOSURE,
		TRANSITIVECLOSURE,
		PERMUTATIONS,
		MINCOSTARBORESCENCE,
		LONGESTPATH,
		GRPHSEQUENCE,
		GENERALINFORMATION,
        HAMILTONIAN_PATH
	};

	DisplayAlgorithmResults (wxWindow* parent,BuildGraph *builtGraph, GraphType type,
	                         GraphLayout layout, Algorithm myAlgorithmType,
	                         const wxString& algorithmLenLabel = wxT (""),
	                         wxWindowID id=wxID_ANY,
	                         const wxPoint& pos=wxDefaultPosition,
	                         const wxSize& size=wxSize(1024,900));
	virtual ~DisplayAlgorithmResults();
	void writeOutputLine (const wxString& line);
	wxTextCtrl*  getDisplayAlgorithmResultsTextCtrl ();
	wxStaticText* getDisplayAlgorithmResultsStaticText();

	//(*Declarations(DisplayAlgorithmResults)
	wxMenu* displayAlgorithmResultsFileMenu;
	wxMenu* displayAlgorithmResultsEditMenu;
	wxStatusBar* displayAlgorithmResultsStatusBar;
	wxPanel* displayAlgorithmResultsPanel;
	wxStaticBitmap* frameFigure;
	wxSlider* displayAlgorithmResultsSlider;
	wxMenuBar* displayAlgorithmResultsMenuBar;
	DrawingCanvas* graphDrawing;
	wxButton* displayAlgorithmResultsCloseButton;
	wxMenuItem *displayAlgorithmResultsPrintMenuItem;
	wxMenuItem *displayAlgorithmResultsPrintPreviewMenuItem;
	wxMenuItem *displayAlgorithmResultsPageSetupMenuItem;
	wxMenuItem *displayAlgorithmResultsSaveAsMenuItem;
	wxMenuItem *displayAlgorithmResultsExitMenuItem;
	wxMenuItem *displayAlgorithmResultsCopyImageMenuItem;
	wxMenu* displayAlgorithmResultsHelpMenu;
	wxMenuItem* displayAlgorithmResultsHelpMenuItem;
	wxStaticLine *myLineSeparator;
	wxStaticText* DisplayAlgorithmResultsOutputStaticText;
	wxTextCtrl* displayAlgorithmResultsOutputTextCtrl;
    wxButton* saveAsFigure;
    wxButton* saveTextOutput;

protected:
	static const long ID_FRAME_ALGORITHM_FIGURE;
	static const long ID_FRAME_FIGURE_DFSPATH;
	static const long ID_FRAME_FIGURE_BFSTREE;
	static const long ID_FRAME_FIGURE_BFSPATH;
	static const long ID_GRAPH_DRAWING;
	static const long ID_ALGORITHM_RESULTS_SLIDER;
	static const long ID_ALGORITHM_RESULTS_CLOSE_BUTTON;
	static const long ID_ALGORITHM_SAVEAS_FIGURE_AND_ALGORITHM_TEXT_BUTTON;
	static const long ID_ALGORITHM_RESULTS_PANEL;
	static const long ID_ALGORITHM_RESULTS_STATUSBAR;
	static const long ID_ALGORITHM_RESULTS_PRINT_MENUITEM;
	static const long ID_ALGORITHM_RESULTS_CANVAS_PRINT_PREVIEW_MENUITEM;
	static const long ID_ALGORITHM_RESULTS_CANVAS_PAGESETUP_MENUITEM;
	static const long ID_ALGORITHM_RESULTS_CANVAS_SAVE_AS_MENUITEM;
	static const long ID_ALGORITHM_RESULTS_CANVAS_EXIT_MENUITEM;
	static const long ID_ALGORITHM_RESULTS_COPY_IMAGE_MENUITEM;
	static const long ID_GRAPH_ALGORITHM_CANVAS_HELP_MENUITEM;
	static const long ID_ALGORITHM_RESULTS_STATICLINE;

private:

	//(*Handlers(DisplayAlgorithmResults)
	void OndisplayAlgorithmResultsSaveAsFigureAndTextButtonClick (wxCommandEvent& event);
	void OndisplayAlgorithmResultsCloseButtonClick (wxCommandEvent& event);
	//*)
	void OndisplayAlgorithmResultsSliderCmdScroll (wxScrollEvent& event);
	void OnDisplayAlgorithmResultsHelpMenuItemSelected (wxCommandEvent& event);
	void handleMenuChoice (wxCommandEvent& event);
	void reportError (const wxString& err);
	void OnCloseWindow (wxCloseEvent& event);
	// void OnQuit (wxCommandEvent& event);
	bool closeFrame();

	wxSFDiagramManager m_canvas;
	bool dirty;
	MenuEventHandler *menuHandler;
	ogdf::GraphAttributes *GA;
	ogdf::Graph *G;
	BuildGraph *builtDisplayAlgorithmResults;
	DrawGraph *drawDisplayAlgorithmResults;
	GraphType graphType;
	GraphLayout graphLayout;
	Algorithm algorithmType;
	bool figureSaved;
	bool haveAskedAboutSavingFile;
	wxString displayResultsTitle;
	wxWindow* myParent;
	bool hideTextOutput;
	bool useStandardColor = false;
	bool doNotSaveAsGML = true;

	DECLARE_EVENT_TABLE()
};

#endif   // DISPLAYALGORITHMRESULTS_H
