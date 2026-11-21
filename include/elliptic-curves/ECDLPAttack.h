#ifndef __ECDLPATTACK_H__
#define __ECDLPATTACK_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ECDLPAttack
///////////////////////////////////////////////////////////////////////////////
class ECDLPAttack : public wxFrame
{
private:

protected:
	wxStaticBitmap* ECDLPAttackStaticBitMap;
	wxButton* MonitorLineButton;
	wxStaticText* CapturedTextLabelStaticText;
	wxPanel* AliceCapturedDataPanel;
	wxStaticText* AliceCapturedDataStaticText;
	wxTextCtrl* AliceCapturedDataTextCtrl;
	wxPanel* BobCapturedDataPanel;
	wxStaticText* BobCapturedDataStaticText;
	wxTextCtrl* BobCapturedDataTextCtrl;
	wxStaticText* MessagesCapturedStaticText;
	wxPanel* AliceToBobCapturedMessagesPanel;
	wxStaticText* AliceToBobCapturedMessagesStaticText;
	wxTextCtrl* AliceToBobCapturedMessagesTextCtrl;
	wxPanel* BobToAliceCapturedMessagePanel;
	wxStaticText* BobToAliceCapturedMessagesStaticText;
	wxTextCtrl* BobToAliceCapturedMessagesTextCtrl;
	wxButton* ECDLPAttackCloseButton;

	// Virtual event handlers, overide them in your derived class
	virtual void OnECDLPAttackMonitorLineButtonPress( wxCommandEvent& event );
	virtual void OnCloseECDLPAttackButtonPress( wxCommandEvent& event );


public:

	ECDLPAttack( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

	~ECDLPAttack();

};

#endif //__ECDLPATTACK_H__
