#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#ifndef WX_PRECOMP
//(*HeadersPCH(AboutDialog)
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(AboutDialog)
#include <wx/notebook.h>
//*)

class AboutDialog: public wxDialog
{
public:

	AboutDialog (wxWindow* parent);
	virtual ~AboutDialog();

	//(*Declarations(AboutDialog)
	wxStaticText* thanksToStaticText;
	wxButton* closeButton;
	wxPanel* thanksToPanel;
	wxStaticText* informationStaticText;
	wxPanel* licensePanel;
	wxStaticText* descriptionStaticText;
	wxPanel* descriptionPanel;
	wxPanel* informationPanel;
	wxStaticText* licenseStaticText;
	//*)

protected:

	//(*Identifiers(AboutDialog)
	//*)

private:

	//(*Handlers(AboutDialog)
	void OncloseButtonClick (wxCommandEvent& event);
	//*)

	DECLARE_EVENT_TABLE()
};

#endif
