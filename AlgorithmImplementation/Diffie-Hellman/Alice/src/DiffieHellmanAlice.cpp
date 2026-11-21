/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "RSAFunctions.h"
#include "DiffieHellmanAlice.h"
#include "Blowfish.h"
#include "ImageFiles.h"
#include "report.h"
#include "miracl/big.h"

#include <wx/bitmap.h>
#include <wx/log.h>

#include <string>

BEGIN_EVENT_TABLE (DiffieHellmanAlice, wxFrame)
	EVT_MENU (ALICE_QUIT, DiffieHellmanAlice::OnQuit)
	EVT_MENU (ALICE_START, DiffieHellmanAlice::OnConnectToServer)
	EVT_SOCKET (ALICE_ID, DiffieHellmanAlice::OnSocketEvent)
END_EVENT_TABLE()

DiffieHellmanAlice::DiffieHellmanAlice (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), bitsForEncryption(0)
{
	wxBoxSizer* AliceBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (AliceBoxSizer);

	AliceStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ),wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );

	AliceBoxSizer->Add (AliceStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );
	DiffieHellmanNumBitsForEncryptionPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	AliceBoxSizer->Add (DiffieHellmanNumBitsForEncryptionPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* DiffieHellmanNumBitsForEncryptionPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DiffieHellmanNumBitsForEncryptionPanel->SetSizer (DiffieHellmanNumBitsForEncryptionPanelBoxSizer);

	DiffieHellmanNumBitsForEncryptionStaticText = new wxStaticText (DiffieHellmanNumBitsForEncryptionPanel,
	   wxID_ANY, _ ("Number of bits to use for encryption (128 maximun)"), wxDefaultPosition,
	   wxDLG_UNIT (DiffieHellmanNumBitsForEncryptionPanel, wxSize (-1,-1) ), 0);

	DiffieHellmanNumBitsForEncryptionPanelBoxSizer->Add (DiffieHellmanNumBitsForEncryptionStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DiffieHellmanNumBitsForEncryptionTextCtrl = new wxTextCtrl (DiffieHellmanNumBitsForEncryptionPanel,
	   wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DiffieHellmanNumBitsForEncryptionPanel,
	      wxSize (25,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	DiffieHellmanNumBitsForEncryptionTextCtrl->SetHint (wxT ("") );
#endif

	DiffieHellmanNumBitsForEncryptionPanelBoxSizer->Add (DiffieHellmanNumBitsForEncryptionTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );
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
	SetSize (1024,768);

	wxSize screenSize = wxGetDisplaySize();
	int x1, y1;

	x1 = screenSize.GetWidth() /6;
	y1 = 100;
	SetPosition (wxPoint (x1, y1) );


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
	m_menuFile = new wxMenu();
	m_menuFile->Append (ALICE_QUIT, _ ("E&xit\tAlt-X"), _ ("Quit conversation") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (ALICE_START, _ ("&Connect_with_Bob"), _ ("Connect with Bob") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	SetMenuBar (m_menuBar);

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
	wxString nbits;

	// cout << "OnConnectToServer" << endl;
	nbits = DiffieHellmanNumBitsForEncryptionTextCtrl->GetValue();
	if (nbits.ToLong(&bitsForEncryption) == false)
	{
		report(wxT("The number of bits is not a proper integer."), wxT("Note"));
		return;
	}
	// cout << "bitsForEncryption = " << bitsForEncryption << endl;

	if (bitsForEncryption == 0)
	{
		report(wxT("Designate the number of bits to use for encryption of messages."), wxT("Note"));
		return;
	}

	p = generatePrime (bitsForEncryption);
	// cout << "p = " << p << endl;
	g = getGenerator(p, bitsForEncryption);
	// cout << "g = " << g << endl;
	a = generatePrime (bitsForEncryption);
	// cout << "a = " << a << endl;

	// Create the socket
	wxSocketClient* sock = new wxSocketClient();

	// Set up the event handler and subscribe to most events
	sock->SetEventHandler (*this, ALICE_ID);
	sock->SetNotify (wxSOCKET_CONNECTION_FLAG |
	                 wxSOCKET_INPUT_FLAG |
	                 wxSOCKET_LOST_FLAG);
	sock->Notify (true);

	// try to make a connection
	// cout << "sock->Connect (addr, false)" << endl;
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
	Big data;
	wxString s;
	unsigned char ch, padding;
	int len, numberBytesReceived;
	char buf[1024];

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
		AliceReportTextCtrl->AppendText (_ ("Established a connection to Bob.\n") );
		buf << p;
		ch = 'p';
		sock->WriteMsg (&ch, 1);
		len = to_binary (p,1024,buf,FALSE);
		// cout << "len(p) = " << len << endl;
		sock->WriteMsg (buf, len);

		buf << g;
		ch = 'g';
		sock->WriteMsg (&ch, 1);
		len = to_binary (g,1024,buf,FALSE);
		// cout << "len(g) = " << len << endl;
		sock->WriteMsg (buf, len);
		sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

		buf << a;
		ch = 'A';
		sock->WriteMsg (&ch, 1);
		g_a_mod_p = pow(g,a,p);
		len = to_binary (g_a_mod_p,1024,buf,FALSE);
		// cout << "len(g_a_mod_p) = " << len << endl;
		sock->WriteMsg (buf, len);
		sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
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
			data = from_binary (numberBytesReceived,buf);

			buf << data;
			len = strlen (buf);
			buf[len] = '\n';
			buf[len + 1] = 0;
		}

		switch (ch)
		{
		case 'B':
			g_b_mod_p = data;
			AliceReportTextCtrl->AppendText (wxT ("Received g^b (mod p) =\n") );
			AliceReportTextCtrl->AppendText (buf);
			DiffieHellmanAliceSendMessageButton->Enable (true);
			// cout << "Computing the secret key" << endl;
			// compute the shared secret key (g^b (mod p))^a (mod p)
			secretKey = pow (g_b_mod_p, a, p);
			// cout << "secret key = " << secretKey << endl;
			AliceReportTextCtrl->AppendText (wxT ("Shared secret key:\n") );
			buf << secretKey;
			AliceReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );

			break;

		case 'm':
			string k;
			char key[500], decryptedMsg[100];
			wxString encMsg;

			key << secretKey;
			k = prepareKey (key);

			string encryptedMsg (buf, numberBytesReceived-padding);

			convertStringToHex (encryptedMsg, encMsg);
			AliceReportTextCtrl->AppendText (wxT ("Encrypted message from Bob:\n") );
			AliceReportTextCtrl->AppendText (encMsg + wxT ("\n") );

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

			AliceReportTextCtrl->AppendText (wxT ("Decrypted message from Bob:\n") );
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

void DiffieHellmanAlice::sendData (const Big& data, char ch, wxSocketBase *m_sock)
{
	int len;
	char buf[1024];

	m_sock->WriteMsg (&ch, 1);
	len = to_binary (data,1024,buf,FALSE);
	m_sock->WriteMsg (buf, len);
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
