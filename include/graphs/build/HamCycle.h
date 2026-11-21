#ifndef HAMCYCLE_H
#define HAMCYCLE_H

//(*Headers(HamCycle)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/string.h>

class HamCycle: public wxDialog
{
public:

	HamCycle (wxWindow* parent,int *n,int *m,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~HamCycle();

	//(*Declarations(HamCycle)
	wxTextCtrl* hamCycleNumberOfVerticesTextCtrl;
	wxTextCtrl* hamCycleNumberOfEdgesTextCtrl;
	wxPanel* hamCycleNumberOfVerticesPanel;
	wxStaticText* HamCycleStaticText;
	wxPanel* hamCycleButtonPanel;
	wxButton* hamCycleCancelButton;
	wxPanel* hamCycleNumberOfEdgesPanel;
	wxButton* hamCycleBuildButton;
	wxStaticText* hamCycleNumberOfVerticesStaticText;
	wxStaticText* hamCycleNumberOfEdgesStaticText;
	//*)

protected:

	//(*Identifiers(HamCycle)
	static const long ID_HAM_CYCLE_STATICTEXT;
	static const long ID_HAM_CYCLE_NUMBER_OF_VERTICES_STATICTEXT;
	static const long ID_HAM_CYCLE_NUMBER_OF_VERTICES_TEXTCTRL;
	static const long ID_HAM_CYCLE_NUMBER_OF_VERTICES_PANEL;
	static const long ID_HAM_CYCLE_NUMBER_OF_EDGES_STATICTEXT;
	static const long ID_HAM_CYCLE_NUMBER_OF_EDGES_TEXTCTRL;
	static const long ID_HAM_CYCLE_NUMBER_OF_EDGES_PANEL;
	static const long ID_HAM_CYCLE_BUILD_BUTTON;
	static const long ID_HAM_CYCLE_CANCEL_BUTTON;
	static const long ID_HAM_CYCLE_BUTTON_PANEL;
	//*)

private:
	int *pnumberOfVertices, *pnumberOfEdges;
	//(*Handlers(HamCycle)
	void OnhamCycleBuildButtonClick (wxCommandEvent& event);
	void OnhamCycleCancelButtonClick (wxCommandEvent& event);
	//*)

	DECLARE_EVENT_TABLE()
};

#endif
