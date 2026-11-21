#ifndef DISCRETEMATHCALCULATOR_PARTIALORDERDIALOG__H
#define DISCRETEMATHCALCULATOR_PARTIALORDERDIALOG__H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include "RelationsPartialOrders/RelationInput.h"
#include "RelationsPartialOrders/pset.h"
#include "common/matrix.h"
#include <lemon/concepts/digraph.h>
#include <lemon/maps.h>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>
#include <lemon/bfs.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;
using namespace ogdf;

#include "graphs/algorithms/ShowSequence.h"

class PartialOrderDialog : public wxDialog
{
private:
	vector<RelationGraphEdge>* diGraphToBuild;
	vector<RelationGraphEdge> relation;
	map<wxString,int > stringToVertexIndex;
	map<int,wxString> vertexIndexToString;
	lemon::ListDigraph dig;
	lemon::ListDigraph::NodeMap<wxString> digraphNodeMap;
	map<wxString, lemon::ListDigraph::Node> wxString2DiNode;
	void buildHasseDiagram();
	void buildGraphEdges(const matrix<int>& P);
	void buildDivisibilityRelation(vector<int>& dataValues);
	void buildPowersetRelation(const vector<std::set<char> >& latticeVertex);
	wxString makeSetNotation(const std::set<char>& s);

	typedef lemon::ListDigraph::Node Node;

protected:
	wxStaticBitmap* partialOrderDialogStaticBitmap;
	wxStaticText* partialOrderDialogStaticText;
	wxPanel* partialOrderDialogPowersetPanel;
	wxStaticText* partialOrderDialogPowersetLabelStaticText;
	wxStaticText* partialOrderDialogPowersetNStaticText;
	wxTextCtrl* partialOrderDialogPowersetNvalueTextCtrl;
	wxButton* partialOrderDialogPowersetBuildLattice;
	wxPanel* partialOrderDialogDivisibilityPanel;
	wxStaticText* partialOrderDialogDivisibilityLabelStaticText;
	wxButton* partialOrderDialogInputDivisibilityInputDataButton;
	wxButton* partialOrderDialogInputDivisibilityRandomDataButton;
	wxPanel* partialOrderDialogButtonsPanel;
	wxButton* partialOrderDialogCancelButton;
	wxButton* partialOrderDialogDisplayButton;

	virtual void OnPartialOrderDialogPowersetBuildLatticePress(wxCommandEvent& event);
	virtual void OnDivisibilityInputDataButtonPress (wxCommandEvent& event);
	virtual void OnCancelPartialOrderDialogButtonPress (wxCommandEvent& event);

public:
	PartialOrderDialog (wxWindow* parent, const char *imageFileName, vector<RelationGraphEdge>* posetEdge, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition,
	                    const wxSize& size = wxSize (500,450), long style = wxDEFAULT_DIALOG_STYLE);
	// bool relationPartialOrderGraphInProcess();
	virtual ~PartialOrderDialog();
    std::vector<wxString> getAllVertices() const;
};

#endif // DISCRETEMATHCALCULATOR_PARTIALORDERDIALOG__H
