#ifndef HAMPATHDIALOG_CLASS_H
#define HAMPATHDIALOG_CLASS_H

#include <wx/settings.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/string.h>

class HamPathDialog : public wxDialog
{
protected:
	wxStaticBitmap* ID_HAMILTONIAN_PATH_STATICBITMAP;
	wxPanel* ID_HAMTONIAN_PATH_N_PANEL;
	wxStaticText* ID_HAMILTONIAN_PATH_S_STATICTEXT;
	wxTextCtrl* ID_HAMILTONIAN_PATH_S_TEXTCTRL;
	wxPanel* ID_HAMILTONIAN_PATH_T_PANEL;
	wxStaticText* ID_HAMILTONIAN_PATH_T_STATICTEXT;
	wxTextCtrl* ID_HAMILTONIAN_PATH_T_TEXTCTRL;
	wxPanel* ID_HAMILTONIAN_PATH_BUTTON_PANEL;
	wxButton* ID_HAMILTONIAN_PATH_COMPUTE_BUTTON;
	wxButton* ID_HAMILTONIAN_PATH_CANCEL_BUTTON;

protected:
	virtual void OnComputeButtonClicked (wxCommandEvent& event);
	virtual void OnCancelButtonClicked (wxCommandEvent& event);

public:
	HamPathDialog (wxWindow* parent, wxString *v, const char *imageFileName, wxWindowID id = wxID_ANY, const wxString& title = _ ("Hamiltonian Path Input"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~HamPathDialog();

private:

	wxString *verticesSelected;
};

#endif // HAMPATHDIALOG_CLASS_H
