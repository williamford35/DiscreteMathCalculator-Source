#ifndef RANDOMGRAPH_H
#define RANDOMGRAPH_H

//(*HeadersPCH(RandomGraph)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
//(*Headers(RandomGraph)
//*)

#include "graphs/GraphType.h"

class RandomGraph: public wxDialog
{
public:

	RandomGraph (wxWindow* parent, GraphType gtype, RandomGraphType *rtype,
	             RandomDigraphType *rdtype, int *m, int *n,
	             wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxSize(650,400));
	virtual ~RandomGraph();

	//(*Declarations(RandomGraph)
	wxButton* caancelButton;
	wxPanel* randomGraphControlPanel;
	wxStaticText* randomGraphVerticesStaticText;
	wxPanel* randomGraphEdgesPanel;
	wxTextCtrl* randomGraphVerticesTextCtrl;
	wxStaticText* randomGraphEdgesStaticText;
	wxButton* buildButton;
	wxTextCtrl* randomGraphEdgesTextCtrl;
	wxRadioBox* graphTypeRadioBox;
	wxPanel* randomGraphVertexPanel;
	//*)

protected:

	//(*Identifiers(RandomGraph)
	static const long ID_RANDOM_GRAPH_VERTICES_STATICTEXT;
	static const long ID_RANDOM_GRAPH_VERTICES_TEXTCTRL;
	static const long ID_RANDOM_GRAPH_VERTEX_PANEL;
	static const long ID_RANDOM_GRAPH_EDGES_STATICTEXT;
	static const long ID_RANDOM_GRAPH_EDGES_TEXTCTRL;
	static const long ID_RANDOM_GRAPH_EDGES_PANEL;
	static const long ID_GRAPH_TYPE_RADIOBOX;
	static const long ID_BUILD_BUTTON;
	static const long ID_CANCEL_BUTTON;
	static const long ID_RANDOM_GRAPH_CONTROL_PANEL;
	//*)

private:

	//(*Handlers(RandomGraph)
	void OnrandomGraphBuildButtonClick (wxCommandEvent& event);
	void OnrandomGraphCancelButtonClick (wxCommandEvent& event);
	void OngraphTypeRadioBoxSelect (wxCommandEvent& event);
	//*)

	GraphType graphType;
	RandomGraphType *randomGraphType;
	RandomDigraphType *randomDigraphType;
	int *numberOfVertices, *numberOfEdges;
	bool biPartite, isAcyclic;

	DECLARE_EVENT_TABLE()
};

#endif
