#ifndef DFSSEARCH_H
#define DFSSEARCH_H

//(*HeadersPCH(DfsSearch)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
//*)
//(*Headers(DfsSearch)
//*)

#include "graphs/GraphType.h"

class DfsSearch: public wxDialog
{
public:

	DfsSearch (wxWindow* parent,wxString *v, GraphType type,
	           wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~DfsSearch();

	//(*Declarations(DfsSearch)
	wxPanel* dfsSearchControlPanel;
	wxRadioButton* dfsSearchDfsRadioButton;
	wxPanel* dfsSearchDfsPanel;
	wxButton* dfsSearchRunButton;
	wxStaticBitmap* dfsSearchStaticBitmap;
	wxPanel* dfsSearchDfsVisitPanel;
	wxTextCtrl* dfsStartTextCtrl;
	wxTextCtrl* dfsSearchDfsEnter_t_TextCtrl;
	wxPanel* dfsSearchEnter_s_t_Panel;
	wxButton* dfsVisitCancelButton;
	wxTextCtrl* dfsSearchDfsEnter_s_TextCtrl;
	wxRadioButton* dfsSearchDfsVisitRadioButton;
	//*)

protected:

	//(*Identifiers(DfsSearch)
	static const long ID_DFS_SEARCH_DFS_RADIO_RADIOBUTTON;
	static const long ID_DFS_START_TEXTCTRL;
	static const long ID_DFS_SEARCH_DFS_PANEL;
	static const long ID_DFS_SEARCH_DFS_VISIT_RADIOBUTTON;
	static const long ID_DFS_SEARCH_DFS_ENTER_S_TEXTCTRL;
	static const long ID_DFS_SEARCH_DFS_ENTER_T_TEXTCTRL;
	static const long ID_DFS_SEARCH_ENTER_S_T_PANEL;
	static const long ID_DFS_SEARCH_DFS_VISIT_PANEL;
	static const long ID_DFS_SEARCH_STATICBITMAP;
	static const long ID_DFS_SEARCH_RUN_BUTTON;
	static const long ID_DFS_VISIT_CANCEL_BUTTON;
	static const long ID_DFS_SEARCH_CONTROL_PANEL;
	//*)

private:

	//(*Handlers(DfsSearch)
	void OndfsSearchRunButtonClick (wxCommandEvent& event);
	void OndfsVisitCancelButtonClick (wxCommandEvent& event);
	void OndfsSearchDfsRadioButtonSelect (wxCommandEvent& event);
	void OndfsSearchDfsVisitRadioButtonSelect (wxCommandEvent& event);
	//*)

	wxString *vertex;
	GraphType graphType;

	DECLARE_EVENT_TABLE()
};

#endif
