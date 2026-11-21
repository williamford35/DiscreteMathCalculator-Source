#ifndef ANALYZEGRAPH_H
#define ANALYZEGRAPH_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif
#include <wx/button.h>

#include "graphs/GraphType.h"

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

class AnalyzeGraph : public wxDialog
{
protected:
	wxNotebook* AnalyzeGraphNotebook;
	wxPanel* AnalyzeGraphGeneralPanel;
	wxStaticText* AnalyzeGraphGeneralGraphTypeStaticText;
	wxTextCtrl* AnalyzeGraphGeneralGraphTypeTextCtrl;
	wxStaticText* AnalyzeGraphGeneralNumberOfVerticesStaticText;
	wxTextCtrl* AnalyzeGraphGeneralNumberOfVerticesTextCtrl;
	wxStaticText* AnalyzeGraphGeneralNumberOfEdgesStaticText;
	wxTextCtrl* AnalyzeGraphGeneralNumberOfEdgesTextCtrl;
	wxStaticText* AnalyzeGraphGeneralNumberOfSelfLoopsStaticText;
	wxTextCtrl* AnalyzeGraphGeneralNumberOfSelfLoopsTextCtrl;
	wxStaticText* AnalyzeGraphGeneralNumberOfParallelEdgesStaticText;
	wxTextCtrl* AnalyzeGraphGeneralNumberOfParallelEdgesTextCtrl;
	wxButton* AnalyzeGraphCloseButton;
	wxPanel* AnalyzeGraphTopologyPanel;
	wxStaticText* AnalyzeGraphTopologyLayoutStaticText;
	wxTextCtrl* AnalyzeGraphTopologyLayoutTextCtrl;
	wxStaticText* AnalyzeGraphTopologyPlanarStaticText;
	wxTextCtrl* AnalyzeGraphTopologyPlanarTextCtrl;
	wxStaticText* AnalyzeGraphTopologyAcyclicStaticText;
	wxTextCtrl* AnalyzeGraphTopologyAcyclicTextCtrl;
	wxStaticText* AnalyzeGraphTopologyComponentsStaticText;
	wxTextCtrl* AnalyzeGraphTopologyComponentsTextCtrl;

protected:
	void OnAnalyzeGraphCloseButtonPressed(wxCommandEvent& event);

public:

	struct AnalyzeGraphData
	{
		GraphType gtype;
		int numVertices;
		int numEdges;
		int numSelfLoops;
		bool parallelEdges;
		GraphLayout glayout;
		bool planar;
		bool acyclic;
		int numComponents;
	};

	AnalyzeGraph (wxWindow* parent, const AnalyzeGraphData& gdata, wxWindowID id = wxID_ANY,
	              const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition,
	              const wxSize& size = wxSize (600,400),
	              long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCAPTION|wxCLOSE_BOX);
	virtual ~AnalyzeGraph();
private:

	void fillInTextFieldsWithData(const AnalyzeGraphData& graphData);
};

#endif // ANALYZEGRAPH_H
