#ifndef DIJKSTRAINPUT_H
#define DIJKSTRAINPUT_H

//(*Headers(OptimumPathInput)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
//*)

#include <graphs/GraphType.h>

#include <wx/string.h>
#include <wx/msgdlg.h>

class OptimumPathInput: public wxDialog
{
public:

	OptimumPathInput (wxWindow* parent,wxString *s, wxString *t, bool isDijkstra = true,
	                  GraphType graphType = undirw, wxWindowID id=wxID_ANY);
	virtual ~OptimumPathInput();

	//(*Declarations(OptimumPathInput)
	wxTextCtrl* DijkstraStartingVertexTextCtrl;
	wxPanel* DijkstraVerticesPanel;
	wxButton* DijkstraCancelButton;
	wxPanel* DijkstraPanel;
	wxStaticText* DijkstraFinalVertexStaticText2;
	wxButton* DijkstraExecuteButton;
	wxTextCtrl* DijkstraFinalVertexTextCtrl;
	wxStaticBitmap* DijkstraStaticBitmap;
	wxStaticBitmap* WidestPathStaticBitmap;
	wxPanel* DijkstraButtonsPanel;
	wxPanel* DijkstraFinalVertexPanel;
	wxPanel* DijkstraStartingVertexPanel;
	wxStaticText* DijkstraStartingVertexStaticText;
	//*)

protected:

	//(*Identifiers(OptimumPathInput)
	static const long ID_DIJKSTRA_STATICBITMAP;
	static const long ID_WIDESTPATH_STATICBITMAP;
	static const long ID_DIJKSTRA_STARTING_VERTEX_STATICTEXT;
	static const long ID_DIJKSTRA_STARTING_VERTEX_TEXTCTRL;
	static const long ID_DIJKSTRA_STARTING_VERTEX_PANEL;
	static const long ID_DIJKSTRA_STARGING_VERTEX_STATICTEXT;
	static const long ID_DIJKSTRA_FINAL_VERTEX_TEXTCTRL;
	static const long ID_DIJKSTRA_FINAL_VERTEX_PANEL;
	static const long ID_DIJKSTRA_VERTICES_PANEL;
	static const long ID_DIJKSTRA_EXECUTE_BUTTON;
	static const long ID_DIJKSTRA_CANCEL_BUTTON;
	static const long ID_DIJKSTRA_BUTTONS_PANEL;
	static const long ID_DIJKSTRA_PANEL;
	//*)

private:

	//(*Handlers(OptimumPathInput)
	void OnDijkstraExecuteButtonClick (wxCommandEvent& event);
	void OnDijkstraCancelButtonClick (wxCommandEvent& event);
	//*)
	wxString *startingVertex, *finalVertex;
	GraphType grphType;

	DECLARE_EVENT_TABLE()
};

#endif
