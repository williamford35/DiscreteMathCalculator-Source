///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
///////////////////////////////////////////////////////////////////////////

#ifndef RESULTMAGNITUDE_H
#define RESULTMAGNITUDE_H

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ResultMagnitude
///////////////////////////////////////////////////////////////////////////////
class ResultMagnitude : public wxDialog
{
private:

protected:
	wxPanel* ID_RESULT_MAGNITUDE_PANEL;
	wxStaticBitmap* ID_RESULT_MAGNITUDE_STATICBITMAP;
	wxTextCtrl* ID_RESULT_MAGNITUDE_TEXTCTRL;
	wxButton* ID_CLOSE_BUTTON;

	// Virtual event handlers, overide them in your derived class
	virtual void OnButtonClick (wxCommandEvent& event)
	{
		Close();
	}

public:

	ResultMagnitude (wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT ("Result Magnitude"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
	~ResultMagnitude();
	void writeApproximation (const wxString& approx);
};

#endif //RESULTMAGNITUDE_H
