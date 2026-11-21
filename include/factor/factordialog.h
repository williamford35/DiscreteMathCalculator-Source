#ifndef _DIALOG_AND_FRAMECREATION_FACTORDIALOG_FACTORDIALOG_BASE_CLASSES_H
#define _DIALOG_AND_FRAMECREATION_FACTORDIALOG_FACTORDIALOG_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif
class FactorDialog : public wxDialog
{
protected:
	wxStaticBitmap* FactorDialogStaticBitmap;
	wxPanel* IntegerToFactorPanel;
	wxStaticText* IntegerToFactorStaticTextCtrl;
	wxTextCtrl* IntegerToFactorTextCtrl;
	wxPanel* FactorResultsPanel;
	wxStaticText* FactorsLabelStaticText;
	wxTextCtrl* FactorResultsTextCtrl;
	wxPanel* FactorDialogControlPanel;
	wxButton* FactorDialogFactorButton;
	wxButton* FactorDialogCloseButton;
	long wxExecuteReturnValue;

protected:
	void OnFactorDialogCanelButtonPressed(wxCommandEvent& event) ;
	void OnFactorDialogFactorButtonPressed(wxCommandEvent& event);
	void OnFactorDialogCloseButtonPressed(wxCommandEvent& event);

public:
	FactorDialog(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX);
	virtual ~FactorDialog();
};

#endif
