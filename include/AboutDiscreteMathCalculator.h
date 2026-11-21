/////////////////////////////////////////////////////////////////////////////
// Name:        AboutDiscreteMathCalculator.h
// Purpose:
// Author:      William Ford (wford333@gmail.com)
// Created:
// Copyright:   (c) 2023 William Ford
// Licence:     GNU General Public Licence version 3
/////////////////////////////////////////////////////////////////////////////

#ifndef ABOUTDISCRETEMATHCALCULATOR_H
#define ABOUTDISCRETEMATHCALCULATOR_H

//(*HeadersPCH(AboutDiscreteMathCalculator)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/textfile.h>
//*)
//(*Headers(AboutDiscreteMathCalculator)
#include <wx/notebook.h>
//*)

class AboutDiscreteMathCalculator: public wxDialog
{
public:

	AboutDiscreteMathCalculator (wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~AboutDiscreteMathCalculator();

	//(*Declarations(AboutDiscreteMathCalculator)
	wxNotebook* aboutNotebook;
	wxStaticText* thanksToStaticText;
	wxButton* closeButton;
	wxPanel* aboutInformationPanel;
	wxStaticText* aboutDescriptionStaticText;
	wxPanel* aboutNotebookPanel;
	wxStaticBitmap* aboutStaticBitmap;
	wxStaticText* aboutInformationStaticText;
	wxPanel* aboutThanksToPanel;
	wxPanel* aboutDescriptionPanel;
	wxPanel* aboutClosePanel;
	wxStaticText* licenseStaticText;
	wxPanel* aboutLicensePanel;
	//*)

protected:

	//(*Identifiers(AboutDiscreteMathCalculator)
	static const long ID_ABOUT_STATICBITMAP;
	static const long ID_ABOUT_DESCRIPTION_STATICTEXT;
	static const long ID_ABOUT_DESCRIPTION_PANEL;
	static const long ID_ABOUT_INFORMATION_STATICTEXT;
	static const long ID_ABOUT_INFORMATION_PANEL;
	static const long ID_THANKS_TO_STATICTEXT;
	static const long ID_ABOUT_THANKS_TO_PANEL;
	static const long ID_LICENSE_STATICTEXT;
	static const long ID_ABOUT_LICENSE_PANEL;
	static const long ID_ABOUT_NOTEBOOK;
	static const long ID_ABOUT_NOTEBOOK_PANEL;
	static const long ID_CLOSE_BUTTON;
	static const long ID_ABOUT_CLOSE_PANEL;
	//*)

private:
	wxString aboutTopic(const char* topic);
	wxString readAboutFile(wxTextFile& topic);

	void OncloseButtonClick (wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif
