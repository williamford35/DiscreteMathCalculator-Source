/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/menu.h>
#include <wx/file.h>
#include <wx/display.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

#include "DocumentServer.h"
#include "ImageFiles.h"
#include "report.h"

#include <limits>

#include <iostream>
#include <fstream>
using namespace std;

// the application icon
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__)
#endif

#define QBITS 160
#define PBITS 1024
#define BUFSIZE 4096

BEGIN_EVENT_TABLE (DocumentServer, wxFrame)
	EVT_MENU (SENDER_START,DocumentServer::OnServerStart)
	EVT_MENU (SENDER_QUIT, DocumentServer::OnQuit)
	EVT_SOCKET (SENDER_ID, DocumentServer::OnServerEvent)
	EVT_SOCKET (SENDER_SERVERSOCKET_ID, DocumentServer::OnSocketEvent)
END_EVENT_TABLE()

#define FORACTUAL
// #undef FORACTUAL

DocumentServer::DocumentServer (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), m_server (NULL)
{
	wxFlexGridSizer* DocumentServerFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	DocumentServerFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	DocumentServerFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	DocumentServerFlexGridSizer->AddGrowableCol (0);
	DocumentServerFlexGridSizer->AddGrowableRow (1);
	this->SetSizer (DocumentServerFlexGridSizer);

	DocumentServerStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile ("ECDSA1.png")) ), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );

	DocumentServerFlexGridSizer->Add (DocumentServerStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	DocumentServerTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (this, wxSize (300,150) ), wxTE_MULTILINE|wxHSCROLL);

	DocumentServerFlexGridSizer->Add (DocumentServerTextCtrl, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	m_menuFile = new wxMenu();
	m_menuFile->Append (SENDER_QUIT, _ ("E&xit₩tAlt-X"), _ ("Quit digital signing") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (SENDER_START, _ ("&Connect with Document Receiver"), _ ("Connect with Document Receiver") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	SetMenuBar (m_menuBar);

	SetTitle (wxT ("Document Server") );
	SetSize (750,750);

    // Position the document demonstration windows side by side.
    int displayIndex = wxDisplay::GetFromWindow(this);
    if (displayIndex == wxNOT_FOUND)
        displayIndex = 0;

    wxDisplay display(displayIndex);
    wxRect area = display.GetClientArea();

    const int gap = 20;
    const int width = GetSize().GetWidth();
    const int height = GetSize().GetHeight();

    const int pairWidth = 2 * width + gap;
    const int left = area.x + wxMax(0, (area.width - pairWidth) / 2);
    const int top = area.y + wxMax(0, (area.height - height) / 2);

    const int windowIndex = 1;
    const int x = left + windowIndex * (width + gap);

    SetPosition(wxPoint(x, top));

	if (GetSizer() )
	{
		Layout();
	}

	/*if (GetParent() )
	{
		CentreOnParent (wxBOTH);
	}
	else
	{
		CentreOnScreen (wxBOTH);
	}*/

/* Window persistence temporarily disabled for layout testing. */

#ifdef __APPLE__
wxString primeDirectory =
    wxStandardPaths::Get().GetUserDataDir();
wxFileName primeFile(primeDirectory, wxT("prime.dat"));
ifstream primeStrIn(primeFile.GetFullPath().mb_str());
#else
#ifdef FORACTUAL
ifstream primeStrIn("../../usr/resource/prime.dat");
#else
ifstream primeStrIn("../../../../../usr/resource/prime.dat");
#endif
#endif
	char prime[128];

	primeStrIn >> prime;
	primeStrIn.close();
	primeStr = wxString(prime);
	buildCurve();
#ifdef __APPLE__
wxString dataDirectory =
    wxStandardPaths::Get().GetUserDataDir();
wxFileName commonFile(dataDirectory, wxT("common.ecs"));
outputCurveParameters(commonFile.GetFullPath().mb_str());
#else
#ifdef FORACTUAL
outputCurveParameters("../../usr/resource/common.ecs");
#else
outputCurveParameters("../../../../../usr/resource/common.ecs");
#endif
#endif
	ecsgen();

	/* #ifdef FORACTUAL
	   remove("../../usr/resource/prime.dat");
	#else
	   remove("../../../../../usr/resource/prime.dat");
	#endif */
	mip=mirsys (1000,2);
}

DocumentServer::~DocumentServer()
{
	if (m_server != NULL)
		delete m_server;
}

void DocumentServer::OnServerStart (wxCommandEvent& WXUNUSED (event) )
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

	DocumentServerTextCtrl->AppendText (_ ("\nWaiting for a document request.\n") );
	DocumentServerTextCtrl->AppendText (_ ("---------------------------------\n") );

	// Setup the event handler and subscribe to connection events
	m_server->SetEventHandler (*this, SENDER_ID);
	m_server->SetNotify (wxSOCKET_CONNECTION_FLAG);
	m_server->Notify (true);
}

void DocumentServer::OnServerEvent (wxSocketEvent& event)
{
	// Accept the new connection and get the socket pointer
	wxSocketBase* sock = m_server->Accept (false);

	// Tell the new socket how and where to process its events
	sock->SetEventHandler (*this, SENDER_SERVERSOCKET_ID);
	sock->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify (true);

	// send the "ready" to the receiver
	// ch = 'r';
	// sock->WriteMsg (&ch, 1);
}

void DocumentServer::OnSocketEvent (wxSocketEvent& event)
{
	sock = event.GetSocket();
	unsigned char ch;
	char buf[100];
	wxString fname;
	int numberBytesReceived;
	// ifstream rf;
#ifndef _WIN32
	bool continueWriting;
#endif
	ifstream fp;

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
			DocumentServerTextCtrl->AppendText (wxT ("File requested: ") );
			fname = wxString(buf);
#ifdef __APPLE__
    // DocumentRequester sends only the document filename on macOS.
    DocumentServerTextCtrl->AppendText(fname + wxT("\n"));
#else
#ifdef FORACTUAL
    fname.Replace(wxT("../usr/DigitalSignatureDocs/"),
                  wxT(""), false);
    DocumentServerTextCtrl->AppendText(fname + wxT("\n"));
#else
    fname.Replace(wxT("../../../../../usr/DigitalSignatureDocs/"),
                  wxT(""), false);
    DocumentServerTextCtrl->AppendText(fname + wxT("\n"));
#endif
#endif
			DocumentServerTextCtrl->AppendText (wxT("\nSigning the file with\n"));
			ecsign(fname);
			DocumentServerTextCtrl->AppendText (wxT ("\nSending the file.\n") );
			DocumentServerTextCtrl->AppendText (_ ("---------------------------------\n") );
			char ch = 'S';
			//do
			//{
			sock->WriteMsg (&ch, 1);
			//}
			//while (sock->Error() );
			break;
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
		DocumentServerTextCtrl->AppendText (wxT("\nConnection closed.\n"));

		break;
	}
	}
}

void DocumentServer::OnQuit (wxCommandEvent& WXUNUSED (event) )
{
	Close (true);
}

void DocumentServer::buildCurve()
{
    long wxExecuteReturnValue, bitsForEncryption;
    wxString empty;
    wxString ellipticCurveSetupProcess;
    wxString callingString;
    wxArrayString ellipticCurveSetupResultsStdout,
                  ellipticCurveSetupProcessStderr;

#ifdef __APPLE__
    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxString dataDirectory =
        wxStandardPaths::Get().GetUserDataDir();

	wxString buildCurveExecutable =
    	resourceDirectory + wxT("/buildCurve");

    wxFileName commonFile(
        dataDirectory,
        wxT("common.ecs"));

    callingString = wxString::Format(
        wxT("\"%s\" -f %s -d -K2 -o \"%s\""),
        buildCurveExecutable,
        primeStr,
        commonFile.GetFullPath());

#else
#ifdef FORACTUAL
    ellipticCurveSetupProcess = wxT("./buildCurve ");

    callingString =
        ellipticCurveSetupProcess +
        wxT("-f ") +
        primeStr +
        wxT(" -d -K2 -o ./common.ecs");
#else
    ellipticCurveSetupProcess =
        wxT("../../../../../usr/resource/buildCurve ");

    callingString =
        ellipticCurveSetupProcess +
        wxT("-f ") +
        primeStr +
        wxT(" -d -K2 -o ../../../../../usr/resource/common.ecs");
#endif
#endif

    wxExecuteReturnValue =
        wxExecute(callingString,
                  ellipticCurveSetupResultsStdout,
                  ellipticCurveSetupProcessStderr,
                  wxEXEC_SYNC |
                  wxEXEC_NOEVENTS |
                  wxEXEC_HIDE_CONSOLE);


}

/* generate the public and private keys */
void DocumentServer::ecsgen()
{
	long wxExecuteReturnValue;
	wxString empty;
	wxString ellipticCurveSignatureGenerationProcess;
	wxString callingString;
	wxArrayString ellipticCurveSignatureGenerationResultsStdout, ellipticCurveSignatureGenerationProcessStderr;

#ifdef __APPLE__
wxString resourceDirectory =
    wxStandardPaths::Get().GetResourcesDir();
wxString dataDirectory =
    wxStandardPaths::Get().GetUserDataDir();

wxString ecsgenExecutable =
    resourceDirectory + wxT("/ecsgen");

callingString = wxString::Format(
    wxT("\"%s\" \"%s\""),
    ecsgenExecutable,
    dataDirectory);
#else
#ifdef FORACTUAL
ellipticCurveSignatureGenerationProcess = wxT("./ecsgen ");
callingString = ellipticCurveSignatureGenerationProcess + wxT("./");
#else
ellipticCurveSignatureGenerationProcess = wxT("../../../../../usr/resource/ecsgen ");
callingString = ellipticCurveSignatureGenerationProcess + wxT("../../../../../usr/resource");
#endif
#endif
	wxExecuteReturnValue = wxExecute (callingString,
	                                  ellipticCurveSignatureGenerationResultsStdout,
	                                  ellipticCurveSignatureGenerationProcessStderr,
	                                  wxEXEC_SYNC|wxEXEC_NOEVENTS |wxEXEC_HIDE_CONSOLE );
	displayStdOut(ellipticCurveSignatureGenerationResultsStdout, DocumentServerTextCtrl);
}

/* compute the digital signature */
void DocumentServer::ecsign (const wxString& message)
{
    long wxExecuteReturnValue;
    wxString tmp;
    wxString callingString;
    wxArrayString ellipticCurveSignerResultsStdout;
    wxArrayString ellipticCurveSignerProcessStderr;

#ifdef __APPLE__
    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxString dataDirectory =
        wxStandardPaths::Get().GetUserDataDir();

    wxString ecsignExecutable =
        resourceDirectory + wxT("/ecsign");

    wxFileName messageFile(message);
    wxString documentName = messageFile.GetFullName();

    wxString documentPath =
        resourceDirectory +
        wxT("/../DigitalSignatureDocs/") +
        documentName;

    // Development fallback: locate the shared source documents.
    if (!wxFileExists(documentPath))
    {
        wxFileName searchDir;
        searchDir.AssignDir(resourceDirectory);

        for (int i = 0; i < 12; ++i)
        {
            wxString candidatePath =
                searchDir.GetPathWithSep() +
                wxT("usr/DigitalSignatureDocs/") +
                documentName;

            wxFileName candidate(candidatePath);

            if (candidate.FileExists())
            {
                documentPath = candidate.GetFullPath();
                break;
            }

            if (searchDir.GetDirCount() == 0)
                break;

            searchDir.RemoveLastDir();
        }
    }

    callingString = wxString::Format(
        wxT("\"%s\" \"%s\" \"%s\""),
        ecsignExecutable,
        dataDirectory,
        documentPath);

#else
    tmp = message;

#ifdef FORACTUAL
    wxString ellipticCurveSignerProcess = wxT("./ecsign ");
    tmp.Replace(wxT("../usr/DigitalSignatureDocs/"),
                wxT(""), false);

    callingString =
        ellipticCurveSignerProcess +
        wxT("./ ") +
        wxT("../DigitalSignatureDocs/") +
        tmp;
#else
    wxString ellipticCurveSignerProcess =
        wxT("../../../../../usr/resource/ecsign ");

    tmp.Replace(
        wxT("../../../../../usr/DigitalSignatureDocs/"),
        wxT(""), false);

    callingString =
        ellipticCurveSignerProcess +
        wxT("../../../../../usr/resource ") +
        wxT("../../../../../usr/DigitalSignatureDocs/") +
        tmp;
#endif

#endif

    wxExecuteReturnValue =
        wxExecute(callingString,
                  ellipticCurveSignerResultsStdout,
                  ellipticCurveSignerProcessStderr,
                  wxEXEC_SYNC |
                  wxEXEC_NOEVENTS |
                  wxEXEC_HIDE_CONSOLE);

    displayStdOut(ellipticCurveSignerResultsStdout,
                  DocumentServerTextCtrl);
}

void DocumentServer::outputCurveParameters(const char *appfile)
{
	string line;
	ifstream appoutputfile(appfile);
	int i = 0;

	while (getline(appoutputfile, line))
	{
		switch (i)
		{
		case BITS:
			DocumentServerTextCtrl->AppendText(wxT("Number of bits = ") + wxString(line) + wxT("\n"));
			break;

		case P:
			DocumentServerTextCtrl->AppendText(wxT("Prime = ") + wxString(line) + wxT("\n"));
			break;

		case A:
			DocumentServerTextCtrl->AppendText(wxT("A =") + wxString(line) + wxT("\n"));
			break;

		case B:
			DocumentServerTextCtrl->AppendText(wxT("B = ") + wxString(line) + wxT("\n"));
			break;

		case ORDER:
			DocumentServerTextCtrl->AppendText(wxT("Basepoint order = ") + wxString(line) + wxT("\n"));
			break;

		case X:
			DocumentServerTextCtrl->AppendText(wxT("x = ") + wxString(line) + wxT("\n"));
			break;

		case Y:
			DocumentServerTextCtrl->AppendText(wxT("y =") + wxString(line) + wxT("\n"));
			break;
		}
		i++;
	}

	appoutputfile.close();
}

void DocumentServer::displayStdOut(wxArrayString& arr, wxTextCtrl *t)
{
	int numReturnedOutputs, i;

	numReturnedOutputs = arr.GetCount();

	t->AppendText(wxT("\n"));
	for (i=0; i<numReturnedOutputs; i++)
		t->AppendText(arr[i] + wxT("\n"));
}

