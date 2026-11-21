#ifndef DIGRAPHICALSEQUENCE_H
#define DIGRAPHICALSEQUENCE_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <vector>
using namespace std;

class DigraphicalSequence : public wxDialog
{
protected:
	wxStaticBitmap* DigraphicalRealizationStaticBitmap;
	wxPanel* DigraphicalRealization_n_Panel;
	wxStaticText* DigraphicalRealization_n_StaticText;
	wxTextCtrl* DigraphicalRealization_n_TextCtrl;
	wxPanel* DigraphicalRealizationInDegreePanel;
	wxStaticText* DigraphicalRealizationInDegreeStaticText;
	wxTextCtrl* DigraphicalRealizationInDegreeTextCtrl;
	wxPanel* DigraphicalRealizationOutDegreePanel;
	wxStaticText* DigraphicalRealizationOutDegreetaticText;
	wxTextCtrl* DigraphicalRealizationOutDegreeTextCtrl;
	wxPanel* DigraphicalRealizationButtonPanel;
	wxButton* DigraphicalRealizationCancelButton;
	wxButton* DigraphicalRealizationTestButton;

protected:
	virtual void OnDigraphicalRealizationCancelButtonPress(wxCommandEvent& event);
	virtual void OnDigraphicalRealizationTestButtonPress(wxCommandEvent& event);

public:
	DigraphicalSequence(wxWindow* parent, const char *imageFileName, vector<int> *x, vector<int> *y, wxWindowID id = wxID_ANY, const wxString& title = _("Digraphical realization"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (400,375), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	virtual ~DigraphicalSequence();

private:
	vector<int> *a, *b;
};

#endif	// DIGRAPHICALSEQUENCE_H
