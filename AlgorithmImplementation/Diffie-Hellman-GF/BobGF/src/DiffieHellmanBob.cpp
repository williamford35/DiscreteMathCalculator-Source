#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "DiffieHellmanBob.h"
#include "RSAFunctions.h"
#include "ImageFiles.h"
#include "report.h"
#include "Blowfish.h"

#include <fstream>
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
	SetWindowStyleFlag(GetWindowStyleFlag() | wxSTAY_ON_TOP);
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

	// Position Bob on the right side of the screen.
	wxSize screenSize = wxGetDisplaySize();

	int x2 = screenSize.GetWidth() / 2;
	int y2 = 100;

	SetPosition(wxPoint(x2, y2));

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	assignBobParameters ();
	// assignBobParameters();

	// Keep Bob at its designated position.
	// Do not restore a previously saved window position.

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

	BobReportTextCtrl->AppendText (_ ("Bob waiting for Alice to connect.\n\n") );

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
	char bufX[512], bufY[512];
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

	case wxSOCKET_CONNECTION:
	{
		BobReportTextCtrl->AppendText (_ ("Established a connection to Alice.\n") );
		break;
	}

	case wxSOCKET_INPUT:
	{
		sock->ReadMsg (&ch,1 );
		if (ch == 'm')
			sock->ReadMsg (&padding, 1);

		sock->ReadMsg (buf, 1024);
		numberBytesReceived = sock->LastReadCount();

		if (ch != 'm')
		{
			len = strlen (buf);
			buf[len] = '\n';
			buf[len + 1] = 0;
		}
		switch (ch)
		{
		case 'b':
			BobReportTextCtrl->AppendText (wxT ("Received b*P = ") );
			len = strlen(buf);
			buf[len-1] = 0;
			BobReportTextCtrl->AppendText (wxString(buf));
			BobReportTextCtrl->AppendText(abPstr);
			BobReportTextCtrl->AppendText(secretKeystr);
			bufX << aPx;
			bufY << aPy;
			sprintf(buf, "a*P = (%s,%s)\n", bufX, bufY);
			sendData(buf, 'a', sock);
			BobReportTextCtrl->AppendText (wxT("_______________________________________\n\n"));
			DiffieHellmanBobSendMessageButton->Enable (true);
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

void DiffieHellmanBob::sendData (char *data, char ch, wxSocketBase *m_sock)
{
	m_sock->WriteMsg (&ch, 1);
	m_sock->WriteMsg (data, strlen(data));
	m_sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
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

#define FOR_RESOURCES
// #undef FOR_RESOURCES

void DiffieHellmanBob::assignBobParameters()
{
	string line;
	char cstring[256], dstring[256], buf[256];
	int i = 0, len;
	ifstream curveParameters;

#ifdef __APPLE__
wxFileName paramsFile(
    wxGetHomeDir() +
        wxT("/Library/Application Support/DiscreteMathCalculator"),
    wxT("gfparams.txt")
);

curveParameters.open(paramsFile.GetFullPath().mb_str());
#elif __LINUX__
#ifdef FOR_RESOURCES
	curveParameters.open("./gfparams.txt");
#else
	curveParameters.open("../../../../../usr/resource/gfparams.txt");
#endif
#else
#ifdef FOR_RESOURCES
	curveParameters.open("./gfparams.txt");
#else
	curveParameters.open("../../../../../usr/resource/gfparams.txt");
#endif
#endif
	while (getline(curveParameters, line, ','))
	{
		// convert line from std::string to char*
		strcpy(cstring, line.c_str());

		switch (curveComputations(i))
		{
		case CURVE_PRIME:
			p = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("p = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_SEED:
			seed = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("seed = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_ACOEF:
			A = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("A = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_BCOEF:
			B = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("B = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_ORDER:
			order = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("Curve order = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_BASEPOINT_X:
			Px = Big(cstring);
			strcpy(dstring,cstring);
			break;
		case CURVE_BASEPOINT_Y:
			Py = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("P = (")+wxString(dstring)+wxT(","));
			BobReportTextCtrl->AppendText (wxString(cstring) +wxT(")")+ wxT("\n"));
			break;
		case CURVE_BASEPOINT_ORDER:
			basepointOrder = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("basepoint order = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_COFACTOR:
			cofactor = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("cofactor = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			BobReportTextCtrl->AppendText (wxT("_______________________________________\n"));
			break;
		case BOB_A:
			a = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("\na = "));
			BobReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case ALICE_B:
			b = Big(cstring);
			break;
		case AP_X:
			aPx = Big(cstring);
			strcpy(dstring,cstring);
			break;
		case AP_Y:
			aPy = Big(cstring);
			BobReportTextCtrl->AppendText (wxT ("a*P = (")+wxString(dstring)+wxT(","));
			BobReportTextCtrl->AppendText (wxString(cstring) +wxT(")")+ wxT("\n"));
			BobReportTextCtrl->AppendText (wxT("_______________________________________\n\n"));
			break;
		case BP_X:
			bPx = Big(cstring);
			break;
		case BP_Y:
			bPy = Big(cstring);
			break;
		case ABP_X:
			abPx = Big(cstring);
			strcpy(dstring,cstring);
			break;
		case ABP_Y:
			len = strlen(cstring);
			cstring[len-1] = 0;
			abPy = Big(cstring);
			sprintf(buf, "a*b*P = (%s, %s)\n", dstring, cstring);
			abPstr = wxString(buf);
			sprintf(buf, "secret key = x-coordinate of a*b*P = %s\n", dstring);
			secretKeystr = wxString(buf);
			break;
		}
		i++;
	}

	curveParameters.close();
	secretKey = abPx;
}
