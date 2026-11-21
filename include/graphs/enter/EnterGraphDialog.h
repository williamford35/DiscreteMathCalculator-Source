#ifndef ENTERGRAPHDIALOG_H
#define ENTERGRAPHDIALOG_H

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include "graphs/GraphType.h"

#include <vector>
using namespace std;

struct GraphEdgeTextCtrls
{
	wxTextCtrl *edgeSource;
	wxTextCtrl *edgeDest;
	wxTextCtrl *edgeWeight;

	GraphEdgeTextCtrls()
	{}

	GraphEdgeTextCtrls (wxTextCtrl *src, wxTextCtrl *dest, wxTextCtrl *weight = NULL) :
		edgeSource (src), edgeDest (dest), edgeWeight (weight)
	{}
};

struct GraphEdge
{
	wxString edgeSource;
	wxString edgeDest;
	wxString edgeWeight;

	GraphEdge()
	{}

	GraphEdge (const wxString src, const wxString& dest, wxString &weight) :
		edgeSource (src), edgeDest (dest), edgeWeight (weight)
	{}
};


class EnterGraphDialog: public wxDialog
{
public:
	EnterGraphDialog (wxWindow* parent,wxWindowID id, GraphType gt);
	virtual ~EnterGraphDialog();
	vector<GraphEdge> getGraphParameters();

private:

	void OnenterGraphBuildButtonClick (wxCommandEvent& event);
	void OnCancel (wxCommandEvent& event);

	long ID_ENTER_GRAPH_EDGE_STATICTEXT[36];
	long ID_ENTER_GRAPH_SOURCE_VERTEX_TEXTCTRL[36];
	long ID_ENTER_GRAPH_DESTINATION_VERTEX_TEXTCTRL[36];
	long ID_ENTER_GRAPH_EDGE_ENTRY_PANEL[36];
	long ID_ENTER_GRAPH_EDGE_PANEL[36];
	long ID_ENTER_GRAPH_WEIGHT_STATICTEXT[36];
	long ID_ENTER_GRAPH_EDGE_WEIGHT_TEXTCTRL[36];
	long ID_ENTER_GRAPH_WEIGHT_ENTRY_PANEL[36];
	long ID_ENTER_GRAPH_VERTEX_EDGE_ENTRY_PANEL[36];
	static const long ID_ENTER_GRAPH_STATICBITMAP;
	static const long ID_ENTER_GRAPH_BUILD_BUTTON;
	static const long ID_ENTER_GRAPH_CANCEL_BUTTON;
	static const long ID_ENTER_GRAPH_BUTTONS_PANEL;
	static const long ID_ENTER_GRAPH_BUTTON_PANEL;
	static const long ID_ENTER_GRAPH_BITMAP_PANEL;

	wxPanel* buildGraphEntry (int i);
	wxString buildStringID (const char *id, int i);

	wxStaticText* enterGraphEdgeStaticText;
	wxButton* enterGraphBuildButton;
	wxStaticBitmap* enterGraphStaticBitmap;
	wxPanel* enterBitMapPanel;
	wxPanel* enterGraphButtonsPanel;
	wxTextCtrl* enterGraphSourceVertexTextCtrl;
	wxTextCtrl* enterGraphDestinationVertexTextCtrl;
	wxStaticText* enterGraphWeightStaticText;
	wxPanel* enterGraphEdgeEntryPanel;
	wxTextCtrl* enterGraphEdgeWeightTextCtrl;
	wxPanel* enterGraphWeightEntryPanel;
	wxPanel* enterGraphButtonPanel;
	wxButton* enterGraphCancelButton;
	wxPanel* enterGraphEdgePanel;

	vector<GraphEdgeTextCtrls> edge;
	vector<GraphEdge> graphToBuild;
	GraphType graphType;

	DECLARE_EVENT_TABLE()
};

#endif // ENTERGRAPHDIALOG_H
