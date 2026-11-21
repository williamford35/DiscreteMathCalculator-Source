#ifndef ISOMORPHISMOUTPUT_H
#define ISOMORPHISMOUTPUT_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

class IsomorphismOutput : public wxDialog
{
protected:
	wxStaticBitmap* IsomorphismStaticBitmap;
	wxListCtrl* IsomorphismListCtrl;
	wxPanel* IsomorphismButtonPanel;
	wxButton* IsomorphismCopyButton;
	wxButton* IsomorphismCloseButton;

protected:
	virtual void OnCopyButtonPress (wxCommandEvent& event);
	virtual void OnCloseButtonPress (wxCommandEvent& event);

public:
	IsomorphismOutput (wxWindow* parent, int *lab1, wxString *v1, int *lab2, wxString *v2, long n, const char* imageFileName, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxDIALOG_NO_PARENT);
	virtual ~IsomorphismOutput();

private:
	void fillListCtrl();
	void InsertItemInReportView(int i);

	int *label1, *label2;
	wxString *vg1, *vg2;
	long numVertices;
};

#endif	// ISOMORPHISMOUTPUT_H
