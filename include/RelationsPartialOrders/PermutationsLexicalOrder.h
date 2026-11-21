#ifndef _PARALLELS_DISCRETEMATHCALCULATOR_PERMUTATIONSLEXICALORDER_BASE_CLASSES_H
#define _PARALLELS_DISCRETEMATHCALCULATOR_PERMUTATIONSLEXICALORDER_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
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

// clang-format on

class PermutationsLexicalOrder : public wxDialog
{
private:
	void computeAndDisplayPermutationsLexicalOrder(int n);

protected:
	wxStaticBitmap* PermutationsLexicalOrderStaticBitMap;
	wxTextCtrl* PermutationsLexicalOrderTextCtrl;
	wxPanel* PermutationsLexicalOrderControlPanel;
	wxButton* PermutationsLexicalOrderCopyButton;
	wxButton* PermutationsLexicalOrderCloseButton;

protected:
	void OnPermutationsLexicalOrderCopyButtonPressed(wxCommandEvent& event);
	void OnPermutationsLexicalOrderCloseButtonPressed(wxCommandEvent& event);

public:
	PermutationsLexicalOrder(wxWindow* parent, long n, wxWindowID id = wxID_ANY,
	                         const wxString& title = _("PermutationsLexicalOrder"), const wxPoint& pos = wxDefaultPosition,
	                         const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	virtual ~PermutationsLexicalOrder();
};

#endif
