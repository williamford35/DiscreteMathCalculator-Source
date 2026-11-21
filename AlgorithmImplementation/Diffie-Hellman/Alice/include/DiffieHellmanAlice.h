#ifndef DIFFIEHELLMANALICE_H
#define DIFFIEHELLMANALICE_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/socket.h>
#include <wx/menu.h>
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

enum
{
	// menu items
	ALICE_QUIT = 1000,
	ALICE_START,

	// id for socket
	ALICE_ID
};

#include <miracl/big.h>

class DiffieHellmanAlice : public wxFrame
{
protected:
	wxStaticBitmap* AliceStaticBitmap;
	wxPanel* AliceConnectPanel;
	wxTextCtrl* AliceReportTextCtrl;
	wxPanel* AliceClosePanel;
	wxButton* AliceCloseButton;
	wxMenu* m_menuFile;
	wxMenu *m_menuSocket;
	wxMenuBar *m_menuBar;
	wxPanel* DiffieHellmanAliceSendMessagePanel;
	wxButton* DiffieHellmanAliceSendMessageButton;
	wxTextCtrl* DiffieHellmanAliceSendMessageTextCtrl;
	wxPanel* DiffieHellmanNumBitsForEncryptionPanel;
	wxStaticText* DiffieHellmanNumBitsForEncryptionStaticText;
	wxTextCtrl* DiffieHellmanNumBitsForEncryptionTextCtrl;

protected:
	void OnQuit (wxCommandEvent& event);
	// socket event handlers
	void OnConnectToServer (wxCommandEvent& event);
	void OnSocketEvent (wxSocketEvent& event);
	void OnSendMsgButtonPress (wxCommandEvent& event);

public:
	DiffieHellmanAlice (wxWindow* parent, const char *image, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE);
	virtual ~DiffieHellmanAlice();

private:
	Big p, g, a, b, g_a_mod_p, g_b_mod_p, secretKey;
	miracl *mip;
	bool firstCharIsZero;
	wxSocketBase *sock;
	long bitsForEncryption;

	void sendData (const Big& data, char ch, wxSocketBase *m_sock);

	wxDECLARE_EVENT_TABLE();
};

#endif	// DIFFIEHELLMANALICE_H
