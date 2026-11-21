#ifndef GRIDGRAPHDIALOGINPUT_H
#define GRIDGRAPHDIALOGINPUT_H

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

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

class GridGraphInputDialog : public wxDialog
{
protected:
	wxStaticBitmap* GridGraphImage;
	wxPanel* weightValuePanel;
	wxStaticText* gridGraphWidthLabel;
	wxTextCtrl* gridGraphWidthTextCtrl;
	wxPanel* heightValuePanel;
	wxStaticText* gridGraphHeightLabel;
	wxTextCtrl* gridGraphHeightTextCtrl;
	wxPanel* controlPanel;
	wxButton* cancelGridGraphBuild;
	wxButton* gridGraphBuild;

	static const long ID_GRID_GRAPH_STATICBITMAP;

protected:
	virtual void OnGridGraphCancelButtonPress (wxCommandEvent& event);
	virtual void OnGridGraphBuildButtonPress (wxCommandEvent& event);

public:
	GridGraphInputDialog (wxWindow* parent, long *width, long *height, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size=wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	virtual ~GridGraphInputDialog();

private:
	long *gridGraphWidth, *gridGraphHeight;
};

#endif	// GRIDGRAPHDIALOGINPUT_H
