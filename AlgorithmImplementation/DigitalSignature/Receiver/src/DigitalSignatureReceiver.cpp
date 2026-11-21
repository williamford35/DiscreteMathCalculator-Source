#include "DigitalSignatureReceiver.h"
#include "report.h"
#include "ImageFiles.h"
#include "RSAFunctions.h"
#include "mrshs.h"
#include "TextDisplay.h"

#include <wx/mimetype.h>

#include <fstream>
#include <iostream>
using namespace std;

#ifndef _WIN32
#include <unistd.h>
#endif

// the application icon
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__)
// #include "mondrian.xpm"
#endif

#define BUFSIZE 4096

#include <string>

BEGIN_EVENT_TABLE (DigitalSignatureReceiver, wxFrame)
	EVT_MENU (RECEIVER_QUIT, DigitalSignatureReceiver::OnQuit)
	EVT_MENU (RECEIVER_START, DigitalSignatureReceiver::OnOpenConnection)
	EVT_SOCKET (RECEIVER_ID, DigitalSignatureReceiver::OnSocketEvent)
END_EVENT_TABLE()

DigitalSignatureReceiver::DigitalSignatureReceiver (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), publicKey (0), makeRandomBitError (false)
{
	wxFlexGridSizer* DigitalSignatureReceiverFlexGridSizer = new wxFlexGridSizer (5, 1, 0, 0);
	DigitalSignatureReceiverFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	DigitalSignatureReceiverFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	DigitalSignatureReceiverFlexGridSizer->AddGrowableCol (0);
	DigitalSignatureReceiverFlexGridSizer->AddGrowableRow (3);
	this->SetSizer (DigitalSignatureReceiverFlexGridSizer);

	DigitalSignatureReceiverStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);

	DigitalSignatureReceiverFlexGridSizer->Add (DigitalSignatureReceiverStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	OnDigitalSignatureReceiverChangeOneBitCheckBox = new wxCheckBox (this, wxID_ANY, _ ("Change 1 random bit"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);
	OnDigitalSignatureReceiverChangeOneBitCheckBox->SetValue (false);
	DigitalSignatureReceiverFlexGridSizer->Add (OnDigitalSignatureReceiverChangeOneBitCheckBox, 0, wxALL, WXC_FROM_DIP (5) );
	OnDigitalSignatureReceiverChangeOneBitCheckBox->Enable (false);

	DigitalSignatureReceiverRequestDocPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DigitalSignatureReceiverFlexGridSizer->Add (DigitalSignatureReceiverRequestDocPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* DigitalSignatureReceiverRequestDocPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DigitalSignatureReceiverRequestDocPanel->SetSizer (DigitalSignatureReceiverRequestDocPanelBoxSizer);

	wxArrayString DigitalSignatureReceiverRequestDocComboBoxArr;
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("Preamble.txt") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("PublicKeyCryptography.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("CatalanNumbers.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("BellNumbers.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("BinaryTrees.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("Graphs.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("RelationsAndPartialOrders.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("GaloisFieldGF(2^m).pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("EllipticCurves.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("EllipticCurveCryptography.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("AckermannFunction.pdf") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("DiscreteMathCalculatorDescription.txt") );
	DigitalSignatureReceiverRequestDocComboBoxArr.Add (wxT ("GrahamsNumber.pdf") );
	DigitalSignatureReceiverRequestDocComboBox= new wxComboBox (DigitalSignatureReceiverRequestDocPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DigitalSignatureReceiverRequestDocPanel, wxSize (-1,-1) ),
	      DigitalSignatureReceiverRequestDocComboBoxArr, wxCB_READONLY);
#if wxVERSION_NUMBER >= 3000
	DigitalSignatureReceiverRequestDocComboBox->SetHint (wxT ("") );
#endif

	DigitalSignatureReceiverRequestDocPanelBoxSizer->Add (DigitalSignatureReceiverRequestDocComboBox, 2, wxALL, WXC_FROM_DIP (5) );
	DigitalSignatureReceiverRequestDocComboBox->Enable (false);

	DigitalSignatureReceiverRequestDocButton = new wxButton (DigitalSignatureReceiverRequestDocPanel, wxID_ANY, _ ("Request document"), wxDefaultPosition, wxDLG_UNIT (DigitalSignatureReceiverRequestDocPanel, wxSize (-1,-1) ), 0);

	DigitalSignatureReceiverRequestDocPanelBoxSizer->Add (DigitalSignatureReceiverRequestDocButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );
	DigitalSignatureReceiverRequestDocButton->Enable (false);

	DigitalSignatureReceiverRequestDocPanelBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	DigitalSignatureReceiverRequestReportTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTE_MULTILINE|wxHSCROLL);

	DigitalSignatureReceiverFlexGridSizer->Add (DigitalSignatureReceiverRequestReportTextCtrl, 1, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	DigitalSignatureReceiverButtonPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DigitalSignatureReceiverFlexGridSizer->Add (DigitalSignatureReceiverButtonPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* DigitalSignatureReceiverButtonPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DigitalSignatureReceiverButtonPanel->SetSizer (DigitalSignatureReceiverButtonPanelBoxSizer);

	DigitalSignatureReceiverButtonPanelBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	DigitalSignatureReceiverViewDocumentButton
	   = new wxButton (DigitalSignatureReceiverButtonPanel, wxID_ANY, _ ("View"), wxDefaultPosition, wxDLG_UNIT (DigitalSignatureReceiverButtonPanel, wxSize (-1,-1) ), 0);

	DigitalSignatureReceiverButtonPanelBoxSizer->Add (DigitalSignatureReceiverViewDocumentButton, 0, wxALL, WXC_FROM_DIP (5) );
	DigitalSignatureReceiverViewDocumentButton->Enable (false);

	// SetName (wxT ("DigitalSignatureReceiver") );
	SetSize (1024,800);
	SetTitle (wxT ("Digital Signature Receiver") );

	wxSize screenSize = wxGetDisplaySize();
	int x2, y2;

	x2 = 200;
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
	m_menuFile = new wxMenu();
	m_menuFile->Append (RECEIVER_QUIT, _ ("E&xit\tAlt-X"), _ ("Quit conversation") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (RECEIVER_START, _ ("&Connect with Document Server"), _ ("Connect with Document Server") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	this->SetMenuBar (m_menuBar);

	// Connect events
	DigitalSignatureReceiverRequestDocButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigitalSignatureReceiver::OnDigitalSignatureReceiverRequestDocButtonPress), NULL, this);
	DigitalSignatureReceiverViewDocumentButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigitalSignatureReceiver::OnDigitalSignatureReceiverViewDocumentButtonPress), NULL, this);

	mip=mirsys (1000,2);
}

DigitalSignatureReceiver::~DigitalSignatureReceiver()
{
	DigitalSignatureReceiverRequestDocButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigitalSignatureReceiver::OnDigitalSignatureReceiverRequestDocButtonPress), NULL, this);
	DigitalSignatureReceiverViewDocumentButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigitalSignatureReceiver::OnDigitalSignatureReceiverViewDocumentButtonPress), NULL, this);
}

void DigitalSignatureReceiver::OnDigitalSignatureReceiverRequestDocButtonPress (wxCommandEvent& event)
{
	unsigned char ch;
	wxString selection;

	if (OnDigitalSignatureReceiverChangeOneBitCheckBox->GetValue() )
		makeRandomBitError = true;

	selection = DigitalSignatureReceiverRequestDocComboBox->GetStringSelection();
	ch = 'F';
	sock->WriteMsg (&ch, 1);
	sock->WriteMsg (selection.mb_str (wxConvUTF8), selection.length() );
}

#define VIEW_RESOURCES
// #undef VIEW_RESOURCES

void DigitalSignatureReceiver::OnDigitalSignatureReceiverViewDocumentButtonPress (wxCommandEvent& event)
{
	wxString fileName, extension;

    wxString wxmessage(message.c_str(), wxConvUTF8);

    extension = wxmessage.Right(4);

    if (extension.IsSameAs(wxT(".pdf")))
    {
        fileName = wxmessage;

        if (!wxLaunchDefaultApplication(fileName))
        {
            report(
                wxT("Unable to open the PDF document using the default PDF viewer."),
                wxT("Note")
            );
            return;
        }
    }

	else
	{
		wxTextFile doc(message);
		doc.Open();

		TextDisplay *frame = new TextDisplay(this, wxT("Document Title"), doc);

		frame->Show(true);
	}
}

void DigitalSignatureReceiver::OnQuit (wxCommandEvent& event)
{
	Close (true);
}

void DigitalSignatureReceiver::OnOpenConnection (wxCommandEvent& event)
{
	wxIPV4address addr;
	addr.Hostname (wxT ("localhost") );
	addr.Service (3001);

	// Create the socket
	wxSocketClient* sock = new wxSocketClient();

	// Set up the event handler and subscribe to most events
	sock->SetEventHandler (*this, RECEIVER_ID);
	sock->SetNotify (wxSOCKET_CONNECTION_FLAG);
	sock->Notify (true);

	// try to make a connection
	sock->Connect (addr, false);
}

void DigitalSignatureReceiver::OnSocketEvent (wxSocketEvent& event)
{
	sock = event.GetSocket();

	Big data;
	unsigned char ch;
	int len, numberBytesReceived;
	char buf[1025];
	FILE *fp;
	int bytesRemaining;
	static short r_Or_s;
	int bitInError;
#ifndef _WIN32
	int bytesRead;
#endif
	char *buffer;
#ifndef _WIN32
	bool continueWriting;
#endif
	char fileReceived[100];
	wxString str;
#ifndef _WIN32
	char tempfileTemplate[] = "/tmp/tmpdir.XXXXXX";
	char *tmp_dirname;
#else
	wstring TempPath;
	wchar_t wcharPath[MAX_PATH];
#endif

	sock->SetNotify (wxSOCKET_LOST_FLAG);
	// sock->SetFlags (wxSOCKET_WAITALL);
	sock->SetFlags ( wxSOCKET_WAITALL_READ );

	// Process the event
	switch (event.GetSocketEvent() )
	{
	case wxSOCKET_OUTPUT:
	{
		break;
	}

	case wxSOCKET_CONNECTION:
	{
		DigitalSignatureReceiverRequestReportTextCtrl->AppendText (_ ("Established contact with document server.\n\n") );
		sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
		DigitalSignatureReceiverRequestDocComboBox->Enable (true);
		OnDigitalSignatureReceiverChangeOneBitCheckBox->Enable (true);
		DigitalSignatureReceiverRequestDocButton->Enable (true);

		break;
	}

	case wxSOCKET_INPUT:
	{
		sock->ReadMsg (&ch,1 );

		switch (ch)
		{
		case 'k':
			// received public key from document server
			sock->ReadMsg (buf, 1024);
			numberBytesReceived = sock->LastReadCount();
			data = from_binary (numberBytesReceived,buf);
			publicKey = data;
			buf << data;
			len = strlen (buf);
			buf[len] = '\n';
			buf[len + 1] = '\n';
			buf[len + 2] = 0;
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("1024-bit public key = \n") );
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) );

			break;

		case 'e':
			// requested file not available
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("That file is not available.\n") );

			break;

		case 'S':
			// Output to the specified file
			sock->ReadMsg (&bytesRemaining, sizeof (int) );
			str.Truncate (0);
			str += DigitalSignatureReceiverRequestDocComboBox->GetStringSelection();

#ifdef _WIN32

			if (GetTempPathW (MAX_PATH, wcharPath) )
				TempPath = wcharPath;

			str = wxString (TempPath) + str;
#else
			tmp_dirname = mkdtemp (tempfileTemplate);

			if (tmp_dirname == NULL)
			{
				report (wxT ("Cannot save the temporary file."), wxT ("Note") );
				return;
			}

			str = wxString (tmp_dirname) + str;
#endif
			message = str.ToStdString();
			strncpy (fileReceived, (const char*) str.mb_str (wxConvUTF8), 100);
			if ( (fp = fopen (fileReceived, "wb") ) == NULL)
			{
				report (wxT ("Cannot write the file!"), wxT ("Note") );
				return;
			}

#ifndef _WIN32
			buffer = new char[BUFSIZE];
			continueWriting = true;

			while (continueWriting)
			{
				do
				{
					sock->ReadMsg (buffer, BUFSIZE);
				}
				while (sock->Error() );

				bytesRead = sock->LastCount();
				bytesRemaining -= bytesRead;

				if (bytesRemaining <= 0)
					continueWriting = false;

				fwrite (buffer, 1, bytesRead, fp);
			}

#else
			/* On windows, read the whole file at one time and send it
			 * through the socket. I am doing this because I could not
			 * get the code for Mac and Linux to work on windows.
			 * The transfer using a send and receive thread, where the
			 * file is compressed, sent through the socket, then
			 * uncompressed simply would not work due to very, very
			 * annoying "Can't read from inflate stream" errors.
			*/
			buffer = new char[bytesRemaining];
			sock->ReadMsg (buffer, bytesRemaining);

			if (sock->LastReadCount() != bytesRemaining)
			{
				report (wxT ("Error in file transmission."), wxT ("Note") );
				return;
			}

			fwrite (buffer, 1, bytesRemaining, fp);
#endif
			delete[] buffer;
			fclose (fp);

			break;

		case 'p':
			sock->ReadMsg (buf, 500);
			numberBytesReceived = sock->LastReadCount();
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Receiving shared values:\n\n") );
			p = from_binary (numberBytesReceived,buf);
			buf << p;
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("p = ") );
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
			break;

		case 'q':
			sock->ReadMsg (buf, 500);
			numberBytesReceived = sock->LastReadCount();
			q = from_binary (numberBytesReceived,buf);
			buf << q;
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("q = ") );
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
			break;

		case 'g':
			sock->ReadMsg (buf, 500);
			numberBytesReceived = sock->LastReadCount();
			g = from_binary (numberBytesReceived,buf);
			buf << g;
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("g = ") );
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
			break;

		case 'r':
			sock->ReadMsg (buf, 500);
			numberBytesReceived = sock->LastReadCount();
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Receiving signature:\n\n") );
			r = from_binary (numberBytesReceived,buf);

			if (makeRandomBitError)
			{
				r_Or_s = rnd.IRandomX (0, 1);

				if (r_Or_s == 0)
				{
					int numBits = bits (r);
					Big mask, t;

					bitInError = rnd.IRandomX (0,numBits-1);
					str.Printf (wxT ("Bit %d in error."), bitInError);
					DigitalSignatureReceiverRequestReportTextCtrl->AppendText (str + wxT ("\n\n") );
					mask = Big (1) << bitInError;
					t = r;
					r = lxor (r, mask);

					if (t == r)
						report (wxT ("Internal error: r did not change."), wxT ("Note") );

					makeRandomBitError = false;
				}
			}

			buf << r;
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("r = ") );
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );
			break;

		case 's':
			sock->ReadMsg (buf, 500);
			numberBytesReceived = sock->LastReadCount();
			s = from_binary (numberBytesReceived,buf);

			if (makeRandomBitError && (r_Or_s == 1) )
			{
				Big mask, t;
				int numBits;

				numBits = bits (s);
				bitInError = rnd.IRandomX (0,numBits-1);
				str.Printf (wxT ("Bit %d in error."), bitInError);
				DigitalSignatureReceiverRequestReportTextCtrl->AppendText (str + wxT ("\n\n") );
				mask = Big (1) << bitInError;
				t = s;
				s = lxor (s, mask);

				if (t == s)
					report (wxT ("Internal error: r did not change."), wxT ("Note") );

				makeRandomBitError = false;
			}

			buf << s;
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("s = ") );
			DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) + wxT ("\n\n") );

			if (VerifyDocument() )
			{
				DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Document verified.\n--------------------------------------\n\n") );
				lastFileVerified = true;
				DigitalSignatureReceiverViewDocumentButton->Enable (true);
			}
			else
			{
				DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Document does not verify.\n--------------------------------------\n\n") );
				lastFileVerified = false;
				DigitalSignatureReceiverViewDocumentButton->Enable (false);

				// delete the file transfered from the sender
				if (!::wxRemoveFile (message) )
					report (wxT ("Internal error. Cannot delete a file that does not verify."), wxT ("Note") );
			}

			break;
		}

		break;
	}

	case wxSOCKET_LOST:
	{
		sock->Destroy();
		DigitalSignatureReceiverRequestReportTextCtrl->AppendText (_ ("Connection closed.\n") );

		break;
	}

	}

	// Enable input events again.
	sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
}

bool DigitalSignatureReceiver::VerifyDocument ()

{
	Big u1, u2, h, v, w;
	ifstream fin;
	char buf[1024];
	wxString str;

	if (r>=q || s>=q)
		return false;

	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("r<q and s<q\n") );
	w = inverse (s,q);
	buf << w;
	str.Printf (wxT ("w = inverse(s,q) = %s\n"), wxString (buf) );
	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (str);

	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Computing the hash value of the document:\n") );
	fin.open (message.c_str(), ios::binary|ios::in);

	h = bihash (fin);
	buf << h;
	DigitalSignatureReceiverRequestReportTextCtrl->AppendText ( (wxT ("h (receiver) = \n") ) );
	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxString (buf) + wxT ("\n") );

	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Computing u1 = (h*s) mod q.\n") );
	u1= (h*w) %q;
	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("Computing u2 = (r*s) mod q.\n") );
	u2= (r*w) %q;
	v=pow (g,u1,publicKey,u2,p); // v=g^u1*y^u2 mod p
	v %= q;
	buf << v;
	str.Printf (wxT ("v = ((g^u1)*(publicKey^u2) mod p) mod q = %s\n"), wxString (buf) );
	DigitalSignatureReceiverRequestReportTextCtrl->AppendText (str);

	if (v == r)
	{
		DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("v = r\n\n") );
		return true;
	}
	else
	{
		DigitalSignatureReceiverRequestReportTextCtrl->AppendText (wxT ("v != r\n\n") );
		return false;
	}
}
