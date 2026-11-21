#ifndef MINCOSTARBORESCENCE_H
#define MINCOSTARBORESCENCE_H

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
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

class MinCostArborescenceDialog : public wxDialog
{
protected:
	wxStaticBitmap* MinCostArborescenceStaticBitmap;
	wxPanel* MinCostArborescenceSourceVertexPanel;
	wxStaticText* MinCostArborescenceSourceVertexStaticText;
	wxTextCtrl* MinCostArborescenceSourceVertexTextCtrl;
	wxPanel* MinCostArborescenceButtonPanel;
	wxButton* MinCostArborescenceOKButton;
	wxButton* MinCostArborescenceCancelButton;

protected:
	virtual void OnOKButtonPressed(wxCommandEvent& event);
	virtual void OnCancelButtonPressed(wxCommandEvent& event);

public:
	MinCostArborescenceDialog(wxWindow* parent, wxString *source, const char *imageFileName, wxWindowID id = wxID_ANY, const wxString& title = wxT(""),
        const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(420,350), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX);
	virtual ~MinCostArborescenceDialog();

private:
	void reportError (const wxString& err);

	wxString *sourceVertex;
};

#endif // MINCOSTARBORESCENCE_H
