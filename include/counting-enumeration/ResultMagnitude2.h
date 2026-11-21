#ifndef RESULTMAGNITUDE2_H
#define RESULTMAGNITUDE2_H

//(*Headers(ResultMagnitude2)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class ResultMagnitude2: public wxDialog
{
public:

	ResultMagnitude2 (wxWindow* parent,int p,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~ResultMagnitude2();
	void writeApproximations (const wxString& nvalue,
	                          const wxString& kvalue = wxT (""));

	//(*Declarations(ResultMagnitude2)
	wxPanel* resultMagnitude2NPanel;
	wxTextCtrl* resultMagnitude2KTextCtrl;
	wxStaticBitmap* resultMagnitude2StaticBitmap;
	wxStaticText* resultMagnitude2NStaticText;
	wxPanel* resultMagnitude2CloseButtonPanel;
	wxButton* resultMagnitude2CloseButton;
	wxStaticText* resultMagnitude2KStaticText;
	wxTextCtrl* resultMagnitude2NTextCtrl;
	wxPanel* resultMagnitude2KPanel;
	//*)

protected:

	//(*Identifiers(ResultMagnitude2)
	static const long ID_RESULT_MAGNITUDE2_STATICBITMAP;
	static const long ID_RESULT_MAGNITUDE2_N_STATICTEXT;
	static const long ID_RESULT_MAGNITUDE2_N_TEXTCTRL;
	static const long ID_RESULT_MAGNITUDE2_N_PANEL;
	static const long ID_RESULT_MAGNITUDE2_K_STATICTEXT;
	static const long ID_RESULT_MAGNITUDE2_K_TEXTCTRL;
	static const long ID_RESULT_MAGNITUDE_K_PANEL;
	static const long ID_RESULT_MAGNITUDE2_CLOSE_BUTTON;
	static const long ID_RESULT_MAGNITUDE2_CLOSE_BUTTON_PANEL;
	//*)

private:

	//(*Handlers(ResultMagnitude2)
	void OnresultMagnitude2CloseButtonClick (wxCommandEvent& event);
	//*)

	int page;

	DECLARE_EVENT_TABLE()
};

#endif
