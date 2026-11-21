/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DiffieHellmanBob.h"
#include "RSAFunctions.h"
#include "ImageFiles.h"
#include "report.h"
#include "Blowfish.h"

#include <wx/msgdlg.h>

#include <sstream>
#include <iostream>
using namespace std;

BEGIN_EVENT_TABLE (DiffieHellmanBob, wxFrame)
	EVT_MENU (BOB_START, DiffieHellmanBob::OnServerStart)
	EVT_MENU (BOB_QUIT,  DiffieHellmanBob::OnQuit)
	EVT_SOCKET (BOB_ID,  DiffieHellmanBob::OnServerEvent)
	EVT_SOCKET (BOB_SERVERSOCKET_ID,  DiffieHellmanBob::OnSocketEvent)
END_EVENT_TABLE()

DiffieHellmanBob::DiffieHellmanBob (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style)
{
	wxBoxSizer* BobBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (BobBoxSizer);

	m_server = NULL;

	BobStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ),
	                                      wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );
	BobBoxSizer->Add (BobStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiffieHellmanBobSendMessagePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);
	BobBoxSizer->Add (DiffieHellmanBobSendMessagePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );
	wxBoxSizer* DiffieHellmanBobSendMessagePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiffieHellmanBobSendMessagePanel->SetSizer (DiffieHellmanBobSendMessagePanelBoxSizer);
	DiffieHellmanBobSendMessageButton = new wxButton (DiffieHellmanBobSendMessagePanel, wxID_ANY, _ ("Send message"), wxDefaultPosition, wxDLG_UNIT (DiffieHellmanBobSendMessagePanel, wxSize (-1,-1) ), 0);
	DiffieHellmanBobSendMessageButton->Enable (false);
	DiffieHellmanBobSendMessagePanelBoxSizer->Add (DiffieHellmanBobSendMessageButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );
	DiffieHellmanBobSendMessageTextCtrl = new wxTextCtrl (DiffieHellmanBobSendMessagePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiffieHellmanBobSendMessagePanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiffieHellmanBobSendMessageTextCtrl->SetHint (wxT ("") );
#endif
	DiffieHellmanBobSendMessagePanelBoxSizer->Add (DiffieHellmanBobSendMessageTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	BobReportPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	BobBoxSizer->Add (BobReportPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* BobReportPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	BobReportPanel->SetSizer (BobReportPanelBoxSizer);

	BobReportTextCtrl = new wxTextCtrl (BobReportPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (BobReportPanel, wxSize (300,200) ), wxTE_MULTILINE|wxHSCROLL);

	BobReportPanelBoxSizer->Add (BobReportTextCtrl, 1, wxALL, WXC_FROM_DIP (5) );

	BobClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	BobBoxSizer->Add (BobClosePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	m_menuFile = new wxMenu();
	m_menuFile->Append (BOB_QUIT, _ ("E&xit\tAlt-X"), _ ("Quit conversation") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (BOB_START, _ ("&Connect_with_Alice"), _ ("Connect with Alice") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	SetMenuBar (m_menuBar);

	SetName (wxT ("DiffieHellmanBob") );
	SetTitle (wxT ("Bob") );
	SetSize (1024,768);

	wxSize screenSize = wxGetDisplaySize();
	int x2, y2;

	x2 = screenSize.GetWidth() /2;
	y2 = 100;
	SetPosition (wxPoint (x2, y2) );

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this) )
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif
	// Connect events
	DiffieHellmanBobSendMessageButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiffieHellmanBob::OnSendMsgButtonPress), NULL, this);

	mip=mirsys (100,2);
}

DiffieHellmanBob::~DiffieHellmanBob()
{
	if (m_server != NULL)
		delete m_server;
	DiffieHellmanBobSendMessageButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiffieHellmanBob::OnSendMsgButtonPress), NULL, this);
}

void DiffieHellmanBob::OnServerStart (wxCommandEvent& WXUNUSED (event) )
{
	wxString bitLength;
	// Create the address - defaults to localhost
	wxIPV4address addr;

	addr.Service (3000);

	// Create the socket, we maintain a class pointer so we can shut it down
	m_server = new wxSocketServer (addr);

	// We use Ok() here to see if the server is really listening
	if (! m_server->IsOk() )
	{
		return;
	}

	BobReportTextCtrl->AppendText (_ ("Bob waiting for Alice to connect.\n") );

	// Setup the event handler and subscribe to connection events
	m_server->SetEventHandler (*this, BOB_ID);
	m_server->SetNotify (wxSOCKET_CONNECTION_FLAG);
	m_server->Notify (true);
}

void DiffieHellmanBob::OnServerEvent (wxSocketEvent& event)
{
	// Accept the new connection and get the socket pointer
	wxSocketBase* sock = m_server->Accept (false);

	// Tell the new socket how and where to process its events
	sock->SetEventHandler (*this, BOB_SERVERSOCKET_ID);
	sock->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify (true);
}

void DiffieHellmanBob::OnSocketEvent (wxSocketEvent& event)
{
	sock = event.GetSocket();
	Big data;
	wxString s;
	unsigned char ch;
	int len, numberBytesReceived;
	char buf[1024];
	unsigned char padding;

	sock->SetNotify (wxSOCKET_LOST_FLAG);
	sock->SetFlags (wxSOCKET_WAITALL);

	// Process the event
	switch (event.GetSocketEvent() )
	{
	case wxSOCKET_OUTPUT:
	{
		break;
	}

	case wxSOCKET_INPUT:
	{
		sock->ReadMsg (&ch,1 );

		// cout <<  "ch = " << ch << endl;
		if (ch == 'm')
			sock->ReadMsg (&padding, 1);

		sock->ReadMsg (buf, 1024);
		numberBytesReceived = sock->LastReadCount();
		// cout << "numberBytesReceived-1 = " << numberBytesReceived << endl;

		if (ch != 'm')
		{
			// cout << "Just before data = from_binary (numberBytesReceived,buf)" << endl;
			data = from_binary (numberBytesReceived,buf);
			//cout << "Assigned to data." << endl;
			buf << data;
			len = strlen (buf);
			// cout << "len = " << len << endl;
			buf[len] = '\n';
			buf[len + 1] = 0;
		}

		switch (ch)
		{

		case 'p':
			p = data;
			BobReportTextCtrl->AppendText (wxT ("Received p =\n") );
			BobReportTextCtrl->AppendText (buf);
			break;

		case 'g':
			g = data;
			BobReportTextCtrl->AppendText (wxT ("Received g =\n") );
			BobReportTextCtrl->AppendText (buf);
			// cout << "numberBytesReceived-2 = " << numberBytesReceived << endl;
			// cout << "bits(p) = " << bits(p) << endl;
			b = generatePrime (bits(p));
			// cout << "b = " << b << endl;;
			break;

		case 'A':
			// cout << "Received g^a (mod p) = " << g_a_mod_p << endl;
			g_a_mod_p = data;
			BobReportTextCtrl->AppendText (wxT ("Received g^a (mod p) =\n") );
			BobReportTextCtrl->AppendText (wxString (buf) );
			// cout << "Computing pow(g,b,p)" << endl;
			g_b_mod_p = pow (g,b,p);
			// cout << "pow(g,b,p)" << pow(g,b,p) << endl;
			sendData (g_b_mod_p,'B',sock);
			DiffieHellmanBobSendMessageButton->Enable (true);
			// compute the shared secret key (g^a (mod p))^b (mod p)
			// cout << "Computing pow(g_a_mod_p,b,p)" << endl;
			secretKey = pow (g_a_mod_p, b, p);
			// cout << "secret key = " << secretKey << endl;
			BobReportTextCtrl->AppendText (wxT ("Shared secret key:\n") );
			buf << secretKey;
			BobReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
			break;

		case 'm':
			string k;
			char key[500], decryptedMsg[100];
			wxString encMsg;

			key << secretKey;
			k = prepareKey (key);

			string encryptedMsg(buf, numberBytesReceived-padding);

			convertStringToHex(encryptedMsg, encMsg);
			BobReportTextCtrl->AppendText(wxT("Encrypted message from Alice:\n"));
			BobReportTextCtrl->AppendText(encMsg + wxT("\n"));

			try
			{
				// decrypt msg with BlowFish
				CBlowFish msgFromAlice ( (unsigned char *) k.c_str(), k.length() );

				msgFromAlice.Decrypt ( (unsigned char *) buf, (unsigned char *) decryptedMsg, numberBytesReceived);
			}
			catch (const blowfishexception& be)
			{
				report (wxString (be.what().c_str() ), wxT ("Note") );
				sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
				return;
			}

			decryptedMsg[numberBytesReceived-padding] = 0;
			wxString text (decryptedMsg);

			BobReportTextCtrl->AppendText(wxT("Decrypted message from Alice:\n"));
			BobReportTextCtrl->AppendText (text + wxT ("\n\n") );
			break;
		}
	}

		// Enable input events again.
	sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

	break;

	case wxSOCKET_LOST:
	{
		sock->Destroy();
		BobReportTextCtrl->AppendText (_ ("Connection closed.\n") );

		break;
	}
	}
}

void DiffieHellmanBob::OnQuit (wxCommandEvent& WXUNUSED (event) )
{
	Close (true);
}

void DiffieHellmanBob::sendData (const Big& data, char ch, wxSocketBase *m_sock)
{
	int len;
	char buf[1024];

	m_sock->WriteMsg (&ch, 1);
	len = to_binary (data,1024,buf,FALSE);
	// cout << "len = to_binary (data,1024,buf,FALSE) = " << len << endl;
	m_sock->WriteMsg (buf, len);
}

void DiffieHellmanBob::OnSendMsgButtonPress (wxCommandEvent& event)
{
	wxString msg;
	char buf[500];
	int len;
	unsigned char padding;
	string s;

	msg = DiffieHellmanBobSendMessageTextCtrl->GetValue();
	len = msg.Length();

	if (len == 0)
		return;

	buf << secretKey;
	s = prepareKey (buf);
	// encrypt msg with BlowFish
	CBlowFish msgToAlice ( (unsigned char *) s.c_str(), s.length() );

	s = msg.ToStdString();
	s = prepareMsg (s, padding);
	len = s.length();
	msgToAlice.Encrypt ( (unsigned char *) s.c_str(), (unsigned char *) buf, len);
	sock->WriteMsg ("m", 1);
	sock->WriteMsg (&padding, sizeof (int) );
	sock->WriteMsg (buf, len);
}
