#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "RSAFunctions.h"
#include "DiffieHellmanAlice.h"
#include "Blowfish.h"
#include "ImageFiles.h"
#include "report.h"
#include "miracl/big.h"
#include "elliptic-curves/securedomain.h"

#include <wx/bitmap.h>
#include <wx/log.h>

#include <string>
#include <fstream>
using namespace std;

BEGIN_EVENT_TABLE (DiffieHellmanAlice, wxFrame)
	EVT_MENU (ALICE_QUIT, DiffieHellmanAlice::OnQuit)
	EVT_MENU (ALICE_START, DiffieHellmanAlice::OnConnectToServer)
	EVT_SOCKET (ALICE_ID, DiffieHellmanAlice::OnSocketEvent)
END_EVENT_TABLE()

DiffieHellmanAlice::DiffieHellmanAlice (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style)
{
	SetWindowStyleFlag(GetWindowStyleFlag() | wxSTAY_ON_TOP);
	wxBoxSizer* AliceBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (AliceBoxSizer);

	AliceStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ),wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );
	AliceBoxSizer->Add (AliceStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiffieHellmanAliceSendMessagePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);
	AliceBoxSizer->Add (DiffieHellmanAliceSendMessagePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );
	wxBoxSizer* DiffieHellmanAliceSendMessagePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiffieHellmanAliceSendMessagePanel->SetSizer (DiffieHellmanAliceSendMessagePanelBoxSizer);
	DiffieHellmanAliceSendMessageButton = new wxButton (DiffieHellmanAliceSendMessagePanel, wxID_ANY, _ ("Send message"), wxDefaultPosition, wxDLG_UNIT (DiffieHellmanAliceSendMessagePanel, wxSize (-1,-1) ), 0);
	DiffieHellmanAliceSendMessageButton->Enable (false);
	DiffieHellmanAliceSendMessagePanelBoxSizer->Add (DiffieHellmanAliceSendMessageButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );
	DiffieHellmanAliceSendMessageTextCtrl = new wxTextCtrl (DiffieHellmanAliceSendMessagePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiffieHellmanAliceSendMessagePanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiffieHellmanAliceSendMessageTextCtrl->SetHint (wxT ("") );
#endif
	DiffieHellmanAliceSendMessagePanelBoxSizer->Add (DiffieHellmanAliceSendMessageTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	AliceConnectPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	AliceBoxSizer->Add (AliceConnectPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* AliceConnectPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	AliceConnectPanel->SetSizer (AliceConnectPanelBoxSizer);
	AliceReportTextCtrl = new wxTextCtrl (AliceConnectPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (AliceConnectPanel, wxSize (300,200) ), wxTE_MULTILINE|wxHSCROLL);
#if wxVERSION_NUMBER >= 3000
	AliceReportTextCtrl->SetHint (wxT ("") );
#endif

	AliceConnectPanelBoxSizer->Add (AliceReportTextCtrl, 1, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	SetName (wxT ("DiffieHellmanAlice") );
	SetTitle (wxT ("Alice") );

	// Position Alice on the left side of the screen.
	wxSize screenSize = wxGetDisplaySize();

	int x1 = 20;
	int y1 = 100;

	SetPosition(wxPoint(x1, y1));


	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

// Keep Alice at its designated position.
// Do not restore a previously saved window position.

	m_menuFile = new wxMenu();
	m_menuFile->Append (ALICE_QUIT, _ ("E&xit\tAlt-X"), _ ("Quit conversation") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (ALICE_START, _ ("&Connect_with_Bob"), _ ("Connect with Bob") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	SetMenuBar (m_menuBar);

	assignAliceParameters();

	// Connect events
	DiffieHellmanAliceSendMessageButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiffieHellmanAlice::OnSendMsgButtonPress), NULL, this);

	mip=mirsys (100,2);
}

DiffieHellmanAlice::~DiffieHellmanAlice()
{
	DiffieHellmanAliceSendMessageButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiffieHellmanAlice::OnSendMsgButtonPress), NULL, this);
}

void DiffieHellmanAlice::OnConnectToServer (wxCommandEvent& event)
{
	wxIPV4address addr;
	addr.Hostname (wxT ("localhost") );
	addr.Service (3000);

	// Create the socket
	wxSocketClient* sock = new wxSocketClient();

	// Set up the event handler and subscribe to most events
	sock->SetEventHandler (*this, ALICE_ID);
	sock->SetNotify (wxSOCKET_CONNECTION_FLAG |
	                 wxSOCKET_INPUT_FLAG |
	                 wxSOCKET_LOST_FLAG);
	sock->Notify (true);

	// try to make a connection
	sock->Connect (addr, false);
}

void DiffieHellmanAlice::OnQuit (wxCommandEvent& WXUNUSED (event) )
{
	delete mip;
	Close (true);
}

void DiffieHellmanAlice::OnSocketEvent (wxSocketEvent& event)
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
		AliceReportTextCtrl->AppendText (_ ("Established a connection to Bob.\n\n") );
		bufX << bPx;
		bufY << bPy;
		sprintf(buf, "b*P = (%s,%s)\n", bufX, bufY);
		sendData(buf, 'b', sock);
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
		case 'a':
			AliceReportTextCtrl->AppendText (wxT ("Received a*P = ") );
			len = strlen(buf);
			buf[len-1] = 0;
			AliceReportTextCtrl->AppendText (wxString(buf));
			AliceReportTextCtrl->AppendText(abPstr);
			AliceReportTextCtrl->AppendText(secretKeystr);
			AliceReportTextCtrl->AppendText (wxT("_______________________________________\n\n"));
			DiffieHellmanAliceSendMessageButton->Enable (true);
			break;

		case 'm':
			string k;
			char key[500], decryptedMsg[100];
			wxString encMsg;

			key << secretKey;
			k = prepareKey (key);

			string encryptedMsg(buf, numberBytesReceived-padding);

			convertStringToHex(encryptedMsg, encMsg);
			AliceReportTextCtrl->AppendText(wxT("Encrypted message from Bob:\n"));
			AliceReportTextCtrl->AppendText(encMsg + wxT("\n"));

			try
			{
				// decrypt msg with BlowFish
				CBlowFish msgFromBob ( (unsigned char *) k.c_str(), k.length() );

				msgFromBob.Decrypt ( (unsigned char *) buf, (unsigned char *) decryptedMsg, numberBytesReceived);
			}
			catch (const blowfishexception& be)
			{
				report (wxString (be.what().c_str() ), wxT ("Note") );
				sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
				return;
			}

			decryptedMsg[numberBytesReceived-padding] = 0;
			wxString text (decryptedMsg);

			AliceReportTextCtrl->AppendText(wxT("Decrypted message from Bob:\n"));
			AliceReportTextCtrl->AppendText (text + wxT ("\n\n") );
			break;
		}
	}
		// Enable input events again.
	sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
	break;

	case wxSOCKET_LOST:
	{
		sock->Destroy();
		AliceReportTextCtrl->AppendText (_ ("Connection closed.\n") );
		break;
	}
	}
}

void DiffieHellmanAlice::sendData (char *data, char ch, wxSocketBase *m_sock)
{
	m_sock->WriteMsg (&ch, 1);
	m_sock->WriteMsg (data, strlen(data));
	m_sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
}

void DiffieHellmanAlice::OnSendMsgButtonPress (wxCommandEvent& event)
{
	wxString msg;
	char buf[500];
	int len;
	unsigned char padding;
	string s;

	msg = DiffieHellmanAliceSendMessageTextCtrl->GetValue();
	len = msg.Length();

	if (len == 0)
		return;

	buf << secretKey;
	s = prepareKey (buf);

	try
	{
		// encrypt msg with BlowFish
		CBlowFish msgToBob ( (unsigned char *) s.c_str(), s.length() );

		s = msg.ToStdString();
		s = prepareMsg (s, padding);
		len = s.length();
		msgToBob.Encrypt ( (unsigned char *) s.c_str(), (unsigned char *) buf, len);
		sock->WriteMsg ("m", 1);
		sock->WriteMsg (&padding, 1 );
		sock->WriteMsg (buf, len);
	}
	catch (const blowfishexception& be)
	{
		report (wxString (be.what().c_str() ), wxT ("Note") );
	}
}

#define FOR_RESOURCES
// #undef FOR_RESOURCES

void DiffieHellmanAlice::assignAliceParameters()
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
			AliceReportTextCtrl->AppendText (wxT ("p = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_SEED:
			seed = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("seed = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_ACOEF:
			A = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("A = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_BCOEF:
			B = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("B = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_ORDER:
			order = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("Curve order = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_BASEPOINT_X:
			Px = Big(cstring);
			strcpy(dstring,cstring);
			break;
		case CURVE_BASEPOINT_Y:
			Py = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("P = (")+wxString(dstring)+wxT(","));
			AliceReportTextCtrl->AppendText (wxString(cstring) +wxT(")")+ wxT("\n"));
			break;
		case CURVE_BASEPOINT_ORDER:
			basepointOrder = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("basepoint order = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case CURVE_COFACTOR:
			cofactor = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("cofactor = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			AliceReportTextCtrl->AppendText (wxT("_______________________________________\n\n"));
			break;
		case BOB_A:
			a = Big(cstring);
			break;
		case ALICE_B:
			b = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("b = "));
			AliceReportTextCtrl->AppendText (wxString(cstring) + wxT("\n"));
			break;
		case AP_X:
			aPx = Big(cstring);
			break;
		case AP_Y:
			aPy = Big(cstring);
			break;
		case BP_X:
			bPx = Big(cstring);
			strcpy(dstring,cstring);
			break;
		case BP_Y:
			bPy = Big(cstring);
			AliceReportTextCtrl->AppendText (wxT ("b*P = (")+wxString(dstring)+wxT(","));
			AliceReportTextCtrl->AppendText (wxString(cstring) +wxT(")")+ wxT("\n"));
			AliceReportTextCtrl->AppendText (wxT("_______________________________________\n\n"));
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
