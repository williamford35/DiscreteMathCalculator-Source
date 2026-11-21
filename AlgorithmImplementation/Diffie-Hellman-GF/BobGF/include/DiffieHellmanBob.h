#ifndef BOB__H
#define BOB__H

#include <miracl/big.h>
#include "miracl/ecn.h"

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/busyinfo.h>
#include <wx/socket.h>
#include <wx/stattext.h>
#include <wx/menu.h>

#include <string>
using namespace std;

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

#if wxUSE_IPV6
typedef wxIPV6address IPaddress;
#else
typedef wxIPV4address IPaddress;
#endif

enum
{
	// menu items
	BOB_QUIT = 1000,
	BOB_START,

	// id for sockets
	BOB_ID,
	BOB_SERVERSOCKET_ID
};

enum curveComputations
{
	CURVE_PRIME,
	CURVE_SEED,
	CURVE_ACOEF,
	CURVE_BCOEF,
	CURVE_ORDER,
	CURVE_BASEPOINT_X,
	CURVE_BASEPOINT_Y,
	CURVE_BASEPOINT_ORDER,
	CURVE_COFACTOR,
	BOB_A,
	ALICE_B,
	AP_X,
	AP_Y,
	BP_X,
	BP_Y,
	ABP_X,
	ABP_Y
};

class DiffieHellmanBob : public wxFrame
{
protected:
	wxStaticBitmap* BobStaticBitmap;
	wxPanel* BobReportPanel;
	wxTextCtrl* BobReportTextCtrl;
	wxPanel* BobClosePanel;
	wxMenu* m_menuFile;
	wxMenu* m_menuSocket;
	wxMenuBar* m_menuBar;
	wxPanel* DiffieHellmanBobSendMessagePanel;
	wxButton* DiffieHellmanBobSendMessageButton;
	wxTextCtrl* DiffieHellmanBobSendMessageTextCtrl;

protected:
	void OnServerEvent (wxSocketEvent& event);
	void OnSocketEvent (wxSocketEvent& event);
	void OnServerStart (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);
	void OnSendMsgButtonPress (wxCommandEvent& event);

public:
	DiffieHellmanBob (wxWindow* parent, const char *image, wxWindowID id = wxID_ANY, const wxString& title = wxT (""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (700,500), long style = wxDEFAULT_FRAME_STYLE);
	virtual ~DiffieHellmanBob();

private:
	wxSocketServer *m_server;
	long bitsForEncryption;
	Big p, seed, A, B, order, Px, Py;
	Big secretKey;
	Big basepointOrder, cofactor, a, b;
	Big aPx, aPy, bPx, bPy;
	Big abPx, abPy;
	wxString abPstr, secretKeystr;
	miracl *mip;
	wxSocketBase *sock;

	void sendData (char *data, char ch, wxSocketBase *m_sock);
	void assignBobParameters();
	wxDECLARE_EVENT_TABLE();
};

#endif	// BOB__H
