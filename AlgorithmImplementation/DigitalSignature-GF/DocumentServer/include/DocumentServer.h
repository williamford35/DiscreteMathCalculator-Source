#ifndef DIGITAL_SIGNATURE_SENDER_H
#define DIGITAL_SIGNATURE_SENDER_H

#include <wx/frame.h>
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/socket.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
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

#include "miracl/big.h"

#include <string>
using namespace std;

enum
{
	// menu items
	SENDER_QUIT = 1000,
	SENDER_START,

	// id for sockets
	SENDER_ID,
	SENDER_SERVERSOCKET_ID
};

enum
{
	BITS,
	P,
	A,
	B,
	ORDER,
	X,
	Y
};

class DocumentServer : public wxFrame
{
protected:
	wxStaticBitmap* DocumentServerStaticBitmap;
	wxMenu* m_menuFile;
	wxMenu* m_menuSocket;
	wxMenuBar* m_menuBar;

protected:
	void OnServerEvent (wxSocketEvent& event);
	void OnSocketEvent (wxSocketEvent& event);
	void OnServerStart (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);

public:
	DocumentServer (wxWindow* parent, const char *image, wxWindowID id = wxID_ANY, const wxString& title = _ ("Digital Signature Sender"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE);
	virtual ~DocumentServer();
	wxTextCtrl* DocumentServerTextCtrl;

private:
	wxSocketServer *m_server;
	Big p, q, g, privateKey, publicKey;
	Big r, s;
	miracl *mip;
	wxSocketBase *sock;
	wxString primeStr;

	void buildCurve();
	void ecsgen();
	void ecsign(const wxString& message);
	void sendData (const Big& data, char ch, wxSocketBase *m_sock);
	void outputCurveParameters(const char *appfile);
	void displayStdOut(wxArrayString& arr, wxTextCtrl *t);

	wxDECLARE_EVENT_TABLE();
};

#endif	// DIGITAL_SIGNATURE_SENDER_H
