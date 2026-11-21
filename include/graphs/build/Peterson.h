#ifndef PETERSON_H
#define PETERSON_H

//(*Headers(Peterson)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class Peterson: public wxDialog
{
public:

	Peterson (wxWindow* parent, int *myn, int *myk, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~Peterson();

	//(*Declarations(Peterson)
	wxStaticText* nStaticText;
	wxPanel* nPanel;
	wxStaticBitmap* petersonGraph;
	wxPanel* controlPanel;
	wxPanel* kPanel;
	wxStaticText* kStaticText;
	wxButton* petersonCancelButton;
	wxTextCtrl* kTextCtrl;
	wxTextCtrl* nTextCtrl;
	wxButton* petersonBuildButton;
	//*)

protected:

	//(*Identifiers(Peterson)
	static const long ID_PETERSON_GRAPH_STATICBITMAP;
	static const long ID_N_STATICTEXT;
	static const long ID_N_TEXTCTRL;
	static const long ID_N_PANEL;
	static const long ID_K_STATICTEXT;
	static const long ID_K_TEXTCTRL;
	static const long ID_K_PANEL;
	static const long ID_PETERSON_BUILD_BUTTON;
	static const long ID_PETERSON_CANCEL_BUTTON;
	static const long ID_CONTR0L_PANEL;
	//*)

private:

	//(*Handlers(Peterson)
	void OnpetersonBuildButtonClick (wxCommandEvent& event);
	void OnpetersonCancelButtonClick (wxCommandEvent& event);
	//*)

	int *n, *k;

	DECLARE_EVENT_TABLE()
};

#endif
