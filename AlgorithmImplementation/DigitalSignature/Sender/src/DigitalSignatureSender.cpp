
#include <wx/menu.h>
#include <wx/file.h>

#include "DigitalSignatureSender.h"
#include "DigitalSignatureFileTransfer.h"
#include "ImageFiles.h"
#include "RSAFunctions.h"
#include "mrshs.h"

#include "report.h"
#include "randomc.h"

#include <limits>

// #include <iostream>
#include <fstream>
using namespace std;

// the application icon
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__)
#endif

#define QBITS 160
#define PBITS 1024
#define BUFSIZE 4096

BEGIN_EVENT_TABLE (DigitalSignatureSender, wxFrame)
	EVT_MENU (SENDER_START,DigitalSignatureSender::OnServerStart)
	EVT_MENU (SENDER_QUIT, DigitalSignatureSender::OnQuit)
	EVT_SOCKET (SENDER_ID, DigitalSignatureSender::OnServerEvent)
	EVT_SOCKET (SENDER_SERVERSOCKET_ID, DigitalSignatureSender::OnSocketEvent)
END_EVENT_TABLE()

DigitalSignatureSender::DigitalSignatureSender (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), m_server (NULL), sentaDocument (false)
{
	wxFlexGridSizer* DigitalSignatureSenderFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	DigitalSignatureSenderFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	DigitalSignatureSenderFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	DigitalSignatureSenderFlexGridSizer->AddGrowableCol (0);
	DigitalSignatureSenderFlexGridSizer->AddGrowableRow (1);
	this->SetSizer (DigitalSignatureSenderFlexGridSizer);

	DigitalSignatureSenderStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile ("digital-signature-sender.png")) ), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );

	DigitalSignatureSenderFlexGridSizer->Add (DigitalSignatureSenderStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DigitalSignatureSenderTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (this, wxSize (300,150) ), wxTE_MULTILINE|wxHSCROLL);

	DigitalSignatureSenderFlexGridSizer->Add (DigitalSignatureSenderTextCtrl, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	m_menuFile = new wxMenu();
	m_menuFile->Append (SENDER_QUIT, _ ("E&xit\tAlt-X"), _ ("Quit digital signing") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (SENDER_START, _ ("&Connect with Receiver"), _ ("Connect with Receiver") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	SetMenuBar (m_menuBar);

	// SetName (wxT ("Digital Signature Sender") );
	SetTitle (wxT ("Digital Signature Sender") );
	SetSize (1024,768);

	wxSize screenSize = wxGetDisplaySize();
	int x2, y2;

	x2 = screenSize.GetWidth() - 775;
	y2 = 100;
	SetPosition (wxPoint (x2, y2) );

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	/*if (GetParent() )
	{
		CentreOnParent (wxBOTH);
	}
	else
	{
		CentreOnScreen (wxBOTH);
	}*/

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
	mip=mirsys (1000,2);
}

DigitalSignatureSender::~DigitalSignatureSender()
{
	if (m_server != NULL)
		delete m_server;
}

void DigitalSignatureSender::OnServerStart (wxCommandEvent& WXUNUSED (event) )
{
	wxString bitLength;
	// Create the address - defaults to localhost
	wxIPV4address addr;

	addr.Service (3001);

	// Create the socket, we maintain a class pointer so we can shut it down
	m_server = new wxSocketServer (addr);

	// We use Ok() here to see if the server is really listening
	if (! m_server->IsOk() )
	{
		return;
	}

	DigitalSignatureSenderTextCtrl->AppendText (_ ("Waiting for a document request.\n\n") );

	// Setup the event handler and subscribe to connection events
	m_server->SetEventHandler (*this, SENDER_ID);
	m_server->SetNotify (wxSOCKET_CONNECTION_FLAG);
	m_server->Notify (true);
}

void DigitalSignatureSender::OnServerEvent (wxSocketEvent& event)
{
	char buf[1024];
	unsigned char ch;
	int len;

	// Accept the new connection and get the socket pointer
	wxSocketBase* sock = m_server->Accept (false);

	// Tell the new socket how and where to process its events
	sock->SetEventHandler (*this, SENDER_SERVERSOCKET_ID);
	sock->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify (true);

	dssetup();
	dssgen();
	// send the public key to the receiver
	ch = 'k';
	sock->WriteMsg (&ch, 1);
	len = to_binary (publicKey,1024,buf,FALSE);
	sock->WriteMsg (buf, len);
}

void DigitalSignatureSender::OnSocketEvent (wxSocketEvent& event)
{
	sock = event.GetSocket();
	unsigned char ch;
	char buf[100];
	int numberBytesReceived;
	// ifstream rf;
	char *buffer;
#ifndef _WIN32
	bool continueWriting;
#endif
	ifstream fp;
	streamsize bytesRemaining;
	long bytesRead;
	char fileName[100];

	sock->SetNotify (wxSOCKET_LOST_FLAG);
	sock->SetFlags (wxSOCKET_WAITALL);
	sock->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);

	// Process the event
	switch (event.GetSocketEvent() )
	{
	case wxSOCKET_CONNECTION:
	{
		break;
	}

	case wxSOCKET_OUTPUT:
	{
		break;
	}

	case wxSOCKET_INPUT:
	{
		sock->ReadMsg (&ch,1 );

		if (ch == 'F')
		{
			sock->ReadMsg (buf, 100);
			numberBytesReceived = sock->LastReadCount();
			buf[numberBytesReceived] = 0;
			DigitalSignatureSenderTextCtrl->AppendText (wxT ("File requested:\n\n") );
			DigitalSignatureSenderTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );

			string requestedFile = DigitalSignatureFileTransfer(string(buf)).c_str();
			// cout << "requestedFile = " << requestedFile << endl;
			fp.open (requestedFile.c_str(), ios_base::in | ios_base::binary );
			if (fp.is_open() == false)
			{
				report(
    wxT("File not found:\n") +
    wxString::FromUTF8(requestedFile.c_str()),
    wxT("Note")
);
				return;
			}

			ch = 'S';
			sock->WriteMsg (&ch, 1);

			// send file
			fp.ignore ( std::numeric_limits<std::streamsize>::max() );
			bytesRemaining = fp.gcount();
			fp.clear();   //  Since ignore will have set eof.
			fp.seekg ( 0, std::ios_base::beg );
			DigitalSignatureSenderTextCtrl->AppendText (wxT ("Sending the file length followed by the file in 4K byte blocks.\n\n") );
			sock->WriteMsg (&bytesRemaining, sizeof (int) );

#ifndef _WIN32
			buffer = new char[BUFSIZE];
			continueWriting = true;

			while (continueWriting)
			{
				fp.read ( buffer, BUFSIZE);
				bytesRead = fp.gcount();
				bytesRemaining -= bytesRead;

				if (bytesRemaining <= 0)
					continueWriting = false;

				do
				{
					sock->WriteMsg (buffer, bytesRead);
				}
				while (sock->Error() );
			}

#else
			buffer = new char[bytesRemaining];

			if (buffer == NULL)
			{
				report (wxT ("Memory allocation error reading the file. Perhaps it is too large."), wxT ("Note") );
				sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
				return;
			}

			fp.read(buffer, bytesRemaining);
			bytesRead = fp.gcount();

			if (bytesRead != bytesRemaining)
			{
				report (wxT ("Error reading the file. Perhaps it is too large."), wxT ("Note") );
				sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
				return;
			}

			sock->WriteMsg (buffer, bytesRead);
#endif
			fp.close ();
			delete[] buffer;

			DigitalSignatureSenderTextCtrl->AppendText (wxT ("Signing the file and sending p, q, g, r, s\n\n") );
			// sign file and send signature
			dssign (DigitalSignatureFileTransfer (string(buf) ) );

			// send the big integers required
			if (sentaDocument == false)
			{
				sendData (p, 'p', sock);
				sendData (q, 'q', sock);
				sendData (g, 'g', sock);
				sentaDocument = true;
			}

			// cout << "r = " << r << endl;
			// cout << "s = " << s << endl;
			sendData (r, 'r', sock);
			sendData (s, 's', sock);
		}
		else
		{
			report (wxT ("Unknown request."), wxT ("Note") );
			sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
			sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
			return;
		}
	}

		// Enable input events again.
	sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

	break;

	case wxSOCKET_LOST:
	{
		sock->Destroy();
		DigitalSignatureSenderTextCtrl->AppendText (_ ("Connection closed.\n") );

		break;
	}
	}
}

void DigitalSignatureSender::OnQuit (wxCommandEvent& WXUNUSED (event) )
{
	Close (true);
}

/* generate p, q, and g */
void DigitalSignatureSender::dssetup()
{
	CRandomMersenne rnd;
	char rndDigit;
	string digit;
	long seed;
	Big n, t, s, h;
	wxString str;

	DigitalSignatureSenderTextCtrl->AppendText (wxT ("Digital signature setup:\n\n") );

	for (int i=0; i<9; i++)
	{
		rndDigit = (char) rnd.IRandomX (48, 57);
		digit += rndDigit;
	}

	seed = atoi (digit.c_str() );
	irand (seed);

	/* generate q */
	forever
	{
		n=rand (QBITS-1,2); /* 159 bit number, base 2 */
		q=2*n+1;

		while (!prime (q) ) q+=2;

		if (bits (q) >QBITS) continue;

		break;
	}

	str.Printf (wxT ("%d bit q:\n"), bits (q) );
	DigitalSignatureSenderTextCtrl->AppendText (str);
	writeBigInteger (q);

	/* generate p */
	t= (pow ( (Big) 2,PBITS)-1) / (2*q);
	s= (pow ( (Big) 2,PBITS-1)-1) / (2*q);
	forever
	{
		n=rand (t);

		if (n<s) continue;

		p=2*n*q+1;

		if (prime (p) ) break;
	}
	str.Printf (wxT ("%d bit prime p:\n"), bits (p) );
	DigitalSignatureSenderTextCtrl->AppendText (str);
	writeBigInteger (p);

	/* generate g */
	do
	{
		h=rand (p-1);
		g=pow (h, (p-1) /q,p);
	}
	while (g==1);

	str.Printf (wxT ("%d bit generator g:\n"), bits (g) );
	DigitalSignatureSenderTextCtrl->AppendText (str);
	writeBigInteger (g);
}

/* generate the public and private keys */
void DigitalSignatureSender::dssgen()
{
	Big y;

	y=pow (g,q,p);

	if (y!=1)
	{
		report (wxT ("Problem - generator g is not of order q"), wxT ("Note") );
		return;
	}

	/* generate public/private keys */
	privateKey = rand (q);
	publicKey = pow (g,privateKey,p);
}

/* compute the digital signature */
void DigitalSignatureSender::dssign (const string& message)
{
	Big k, h;
	char buf[1024];

	s = Big (0);

	while (s == Big (0) )
	{
		/* calculate r */
		r = Big (0);

		while (r == Big (0) )
		{
			k = rand (q);
			r = pow (g,k,p);
			r %= q;
		}

		ifstream fin (message.c_str(), ifstream::in | ifstream::binary );

		h = bihash (fin);
		buf << h;
		DigitalSignatureSenderTextCtrl->AppendText ( (wxT ("h (sender) = \n") ) );
		DigitalSignatureSenderTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
		/* calculate s */
		k=inverse (k,q);
		s = ( (h+privateKey*r) *k) % q;
	}

	/* signature is r, s */
	wxString str(message.c_str(), wxConvUTF8);

	str.Printf (wxT ("%d bit r:\n"), bits (r) );
	DigitalSignatureSenderTextCtrl->AppendText (str);
	writeBigInteger (r);
	str.Printf (wxT ("%d bit s:\n"), bits (s) );
	DigitalSignatureSenderTextCtrl->AppendText (str);
	writeBigInteger (s);
}

void DigitalSignatureSender::sendData (const Big& data, char ch, wxSocketBase *m_sock)
{
	int len;
	char buf[1024];

	do
	{
		m_sock->WriteMsg (&ch, 1);
	}
	while (m_sock->Error() );

	len = to_binary (data,1024,buf,FALSE);

	do
	{
		m_sock->WriteMsg (buf, len);
	}
	while (m_sock->Error() );
}

void DigitalSignatureSender::writeBigInteger (const Big& r)
{
	char buf[1024];

	buf << r;
	DigitalSignatureSenderTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
}
