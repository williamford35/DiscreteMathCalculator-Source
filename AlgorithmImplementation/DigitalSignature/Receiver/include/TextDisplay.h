#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/textfile.h>

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

class TextDisplay : public wxFrame
{
protected:
	wxTextCtrl* TextDisplayContentTextCtrl;

protected:

public:
	TextDisplay(wxWindow* parent, const wxString& heading, wxTextFile& doc, wxWindowID id = wxID_ANY, const wxString& title = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1024,768), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX);
	virtual ~TextDisplay();
};

#endif	// TEXTDISPLAY_H
