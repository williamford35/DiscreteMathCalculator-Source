#ifndef DIGITAL_SIGNATURE_RECEIVER_H
#define DIGITAL_SIGNATURE_RECEIVER_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/combobox.h>
#include <wx/socket.h>
#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
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
	RECEIVER_QUIT = 1000,
	RECEIVER_START,

	// id for socket
	RECEIVER_ID
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

#include <miracl/big.h>

class DocumentRequester : public wxFrame
{
protected:
	wxStaticBitmap* DocumentRequesterStaticBitmap;
	wxCheckBox* OnDocumentRequesterChangeOneBitCheckBox;
	wxPanel* DocumentRequesterRequestDocPanel;
	wxComboBox* DocumentRequesterRequestDocComboBox;
	wxPanel* DocumentRequesterButtonPanel;
	wxButton* DocumentRequesterRequestDocButton;
	wxTextCtrl* DocumentRequesterRequestReportTextCtrl;
	wxButton* DocumentRequesterViewDocumentButton;
	wxMenu* m_menuFile;
	wxMenu* m_menuSocket;
	wxMenuBar *m_menuBar;

protected:
	void OnDocumentRequesterRequestDocButtonPress (wxCommandEvent& event);
	void OnDocumentRequesterViewDocumentButtonPress (wxCommandEvent& event);
	void OnQuit (wxCommandEvent& event);
	void OnOpenConnection (wxCommandEvent& event);
	void OnSocketEvent (wxSocketEvent& event);

public:
	DocumentRequester (wxWindow* parent, const char *image, wxWindowID id = wxID_ANY, const wxString& title = _ ("Digital Signature - Receiver"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (1024,768), long style = wxDEFAULT_FRAME_STYLE);
	virtual ~DocumentRequester();

private:
	Big publicKey, p, q, g, r, s;
	miracl *mip;
	wxSocketBase *sock;
	string message;
	wxString selection;
	char fileReceived[100];
	wxString fileNameSelected;
	bool haveDocument = false;

	void outputCurveParameters(const char *appfile);
	void ecsver(const wxString& document);
	void displayStdOut(wxArrayString& arr, wxTextCtrl *t);
	void rmECSfiles();
	void sendData (const Big& data, char ch, wxSocketBase *m_sock);

	wxDECLARE_EVENT_TABLE();
};

#endif	// DIGITAL_SIGNATURE_RECEIVER_H
