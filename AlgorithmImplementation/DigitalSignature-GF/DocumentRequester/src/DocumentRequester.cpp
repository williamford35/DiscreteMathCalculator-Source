/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DocumentRequester.h"
#include "report.h"
#include "ImageFiles.h"
#include "TextDisplay.h"

#include <wx/mimetype.h>
#include <wx/display.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// the application icon
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__)
// #include "mondrian.xpm"
#endif

#define BUFSIZE 4096

#define ACTUALRUN
// #undef ACTUALRUN

BEGIN_EVENT_TABLE (DocumentRequester, wxFrame)
	EVT_MENU (RECEIVER_QUIT, DocumentRequester::OnQuit)
	EVT_MENU (RECEIVER_START, DocumentRequester::OnOpenConnection)
	EVT_SOCKET (RECEIVER_ID, DocumentRequester::OnSocketEvent)
END_EVENT_TABLE()

DocumentRequester::DocumentRequester (wxWindow* parent, const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style), publicKey (0)
{
	wxFlexGridSizer* DocumentRequesterFlexGridSizer = new wxFlexGridSizer (5, 1, 0, 0);
	DocumentRequesterFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	DocumentRequesterFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	DocumentRequesterFlexGridSizer->AddGrowableCol (0);
	DocumentRequesterFlexGridSizer->AddGrowableRow (3);
	this->SetSizer (DocumentRequesterFlexGridSizer);

	DocumentRequesterStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);

	DocumentRequesterFlexGridSizer->Add (DocumentRequesterStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	OnDocumentRequesterChangeOneBitCheckBox = new wxCheckBox (this, wxID_ANY, _ ("Change 1 random bit"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);
	OnDocumentRequesterChangeOneBitCheckBox->SetValue (false);
	DocumentRequesterFlexGridSizer->Add (OnDocumentRequesterChangeOneBitCheckBox, 0, wxALL, WXC_FROM_DIP (5) );
	OnDocumentRequesterChangeOneBitCheckBox->Enable (false);

	DocumentRequesterRequestDocPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DocumentRequesterFlexGridSizer->Add (DocumentRequesterRequestDocPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* DocumentRequesterRequestDocPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DocumentRequesterRequestDocPanel->SetSizer (DocumentRequesterRequestDocPanelBoxSizer);

	wxArrayString DocumentRequesterRequestDocComboBoxArr;
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("License.txt") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("PublicKeyCryptography.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("CatalanNumbers.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("BellNumbers.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("BinaryTrees.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("Graphs.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("RelationsAndPartialOrders.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("GaloisFieldGF(2^m).pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("EllipticCurves.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("EllipticCurveCryptography.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("AckermannFunction.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("DiscreteMathCalculatorDescription.txt") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("GrahamsNumber.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("NP-complete.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("PascalsTriangle.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("Recursion.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("RedBlackTrees.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("GraphTraversals.pdf") );
	DocumentRequesterRequestDocComboBoxArr.Add (wxT ("ParallelProgrammingPrimer.pdf") );
	DocumentRequesterRequestDocComboBox= new wxComboBox (DocumentRequesterRequestDocPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (DocumentRequesterRequestDocPanel, wxSize (-1,-1) ),
	      DocumentRequesterRequestDocComboBoxArr, wxCB_READONLY);
#if wxVERSION_NUMBER >= 3000
	DocumentRequesterRequestDocComboBox->SetHint (wxT ("") );
#endif

	DocumentRequesterRequestDocPanelBoxSizer->Add (DocumentRequesterRequestDocComboBox, 2, wxALL, WXC_FROM_DIP (5) );
	DocumentRequesterRequestDocComboBox->Enable (false);

	DocumentRequesterRequestDocButton = new wxButton (DocumentRequesterRequestDocPanel, wxID_ANY, _ ("Request document"), wxDefaultPosition, wxDLG_UNIT (DocumentRequesterRequestDocPanel, wxSize (-1,-1) ), 0);

	DocumentRequesterRequestDocPanelBoxSizer->Add (DocumentRequesterRequestDocButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );
	DocumentRequesterRequestDocButton->Enable (true);

	DocumentRequesterRequestDocPanelBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	DocumentRequesterRequestReportTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTE_MULTILINE|wxHSCROLL);

	DocumentRequesterFlexGridSizer->Add (DocumentRequesterRequestReportTextCtrl, 1, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	DocumentRequesterButtonPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	DocumentRequesterFlexGridSizer->Add (DocumentRequesterButtonPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* DocumentRequesterButtonPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DocumentRequesterButtonPanel->SetSizer (DocumentRequesterButtonPanelBoxSizer);

	DocumentRequesterButtonPanelBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	DocumentRequesterViewDocumentButton
	   = new wxButton (DocumentRequesterButtonPanel, wxID_ANY, _ ("View"), wxDefaultPosition, wxDLG_UNIT (DocumentRequesterButtonPanel, wxSize (-1,-1) ), 0);

	DocumentRequesterButtonPanelBoxSizer->Add (DocumentRequesterViewDocumentButton, 0, wxALL, WXC_FROM_DIP (5) );
	DocumentRequesterViewDocumentButton->Enable (true);

	// SetName (wxT ("DocumentRequester") );
	SetSize (750,750);
	SetTitle (wxT ("Document Requester") );

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

    const int windowIndex = 0;
    const int x = left + windowIndex * (width + gap);

    SetPosition(wxPoint(x, top));

	if (GetSizer() )
	{
		Layout();
	}

/* Window persistence disabled for consistent sizing. */
	m_menuFile = new wxMenu();
	m_menuFile->Append (RECEIVER_QUIT, _ ("E&xit\tAlt-X"), _ ("Quit conversation") );

	m_menuSocket = new wxMenu();
	m_menuSocket->Append (RECEIVER_START, _ ("&Connect with Document Server"), _ ("Connect with Document Server") );

	m_menuBar = new wxMenuBar();
	m_menuBar->Append (m_menuFile, _ ("&File") );
	m_menuBar->Append (m_menuSocket, _ ("&Begin") );
	this->SetMenuBar (m_menuBar);

	// Connect events
	DocumentRequesterRequestDocButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DocumentRequester::OnDocumentRequesterRequestDocButtonPress), NULL, this);
	DocumentRequesterViewDocumentButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DocumentRequester::OnDocumentRequesterViewDocumentButtonPress), NULL, this);

	mip=mirsys (1000,2);
	// assure that Sender has started
#ifdef _WIN32
	Sleep(3);
#else
	sleep(3);
#endif
#ifdef __APPLE__
    wxString dataDirectory =
        wxStandardPaths::Get().GetUserDataDir();

    wxFileName commonFile(
        dataDirectory,
        wxT("common.ecs"));

    outputCurveParameters(
        commonFile.GetFullPath().mb_str());
#else
#ifdef ACTUALRUN
    outputCurveParameters("./common.ecs");
#else
    outputCurveParameters("../../../../../usr/resource/common.ecs");
#endif
#endif
}

DocumentRequester::~DocumentRequester()
{
	DocumentRequesterRequestDocButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DocumentRequester::OnDocumentRequesterRequestDocButtonPress), NULL, this);
	DocumentRequesterViewDocumentButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DocumentRequester::OnDocumentRequesterViewDocumentButtonPress), NULL, this);
	rmECSfiles();
	Close (true);
}

void DocumentRequester::OnDocumentRequesterRequestDocButtonPress (wxCommandEvent& event)
{
	unsigned char ch;

	selection = DocumentRequesterRequestDocComboBox->GetStringSelection();
	fileNameSelected = selection;
#ifdef __APPLE__
    // On macOS the server locates the document inside the application bundle.
    // Send only the document filename.
#else
#ifdef ACTUALRUN
    selection = wxT("../DigitalSignatureDocs/") + selection;
#else
    selection = wxT("../../../../../usr/DigitalSignatureDocs/") + selection;
#endif
#endif
	ch = 'F';
	sock->WriteMsg (&ch, 1);
	sock->WriteMsg (selection.mb_str (wxConvUTF8), selection.length() );
}

void DocumentRequester::OnDocumentRequesterViewDocumentButtonPress (wxCommandEvent& event)
{
#ifdef __APPLE__
    if (haveDocument == false)
    {
        report(wxT("No document has been received."), wxT("Note"));
        return;
    }

    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxFileName selectedFile(selection);

    wxString documentPath =
        resourceDirectory +
        wxT("/../DigitalSignatureDocs/") +
        selectedFile.GetFullName();

    // Locate the shared source documents in development builds.
    if (!wxFileExists(documentPath))
    {
        wxFileName searchDir;
        searchDir.AssignDir(resourceDirectory);

        for (int i = 0; i < 12; ++i)
        {
            wxString candidatePath =
                searchDir.GetPathWithSep() +
                wxT("usr/DigitalSignatureDocs/") +
                selectedFile.GetFullName();

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

    wxString extension =
        selectedFile.GetExt().Lower();

    if (extension == wxT("pdf"))
    {
        if (!wxLaunchDefaultApplication(documentPath))
        {
            report(wxT("The PDF reader did not execute properly."),
                   wxT("Note"));
        }
        return;
    }

    wxTextFile doc(documentPath);

    if (!doc.Open())
    {
        report(wxT("Unable to open the document."),
               wxT("Note"));
        return;
    }

    TextDisplay *frame =
        new TextDisplay(this,
                        wxT("Document Title"),
                        doc);

    frame->Show(true);

#else

    wxString fileName, extension, evincepath;
    wxString wxmessage(message.c_str(), wxConvUTF8);
    bool foundEvincePath = false;

    if (haveDocument == false)
    {
        report(wxT("No document has been received."), wxT("Note"));
        return;
    }

    extension = selection.Right(4);

    if (extension.IsSameAs(wxT(".pdf")))
    {
        wxFileType* pdfFileType =
            wxTheMimeTypesManager->GetFileTypeFromExtension(
                wxT("pdf"));

        if (pdfFileType != NULL)
        {
#ifdef _WIN32
#ifdef ACTUALRUN
            if (wxFile::Exists(
                    wxT("./Evince-2.32.0.145/bin/evince.exe ")))
            {
                evincepath =
                    wxT("./Evince-2.32.0.145/bin/evince.exe ");
                foundEvincePath = true;
            }
#else
            if (wxFile::Exists(
                    wxT("../../../../../usr/resource/Evince-2.32.0.145/bin/evince.exe ")))
            {
                evincepath =
                    wxT("../../../../../usr/resource/Evince-2.32.0.145/bin/evince.exe ");
                foundEvincePath = true;
            }
#endif
#endif

#if defined(__LINUX__)
            if (wxFile::Exists(wxT("/usr/bin/evince")))
            {
                evincepath = wxT("/usr/bin/evince ");
                foundEvincePath = true;
            }
            else if (wxFile::Exists(wxT("/usr/local/bin/evince")))
            {
                evincepath = wxT("/usr/local/bin/evince ");
                foundEvincePath = true;
            }
#endif

            if (foundEvincePath == false)
            {
                report(wxT("Cannot locate evince PDF viewer."),
                       wxT("Note"));
                return;
            }

            long rc =
                wxExecute(evincepath + selection,
                          wxEXEC_ASYNC);

            if (rc < 0)
            {
                report(wxT("The pdf reader did not execute properly."),
                       wxT("Note"));
                return;
            }
        }
    }
    else
    {
        wxTextFile doc(selection);
        doc.Open();

        TextDisplay *frame =
            new TextDisplay(this,
                            wxT("Document Title"),
                            doc);

        frame->Show(true);
    }

#endif
}

void DocumentRequester::OnQuit (wxCommandEvent& event)
{
	rmECSfiles();
	Close (true);
}

void DocumentRequester::OnOpenConnection (wxCommandEvent& event)
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

void DocumentRequester::OnSocketEvent (wxSocketEvent& event)
{
	sock = event.GetSocket();

	unsigned char ch;

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
		DocumentRequesterRequestReportTextCtrl->AppendText (_ ("\nEstablished contact with document server.\n\n") );
		sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
		DocumentRequesterRequestDocComboBox->Enable (true);
		DocumentRequesterRequestDocButton->Enable (true);

		break;
	}

	case wxSOCKET_INPUT:
	{
		sock->ReadMsg (&ch,1 );
		if (ch != 'S')
		{
			report(wxT("Did not get a proper response from document server."), wxT("Note"));
			return;
		}
		DocumentRequesterRequestReportTextCtrl->AppendText(fileNameSelected + wxT(" received.\n"));
		DocumentRequesterRequestReportTextCtrl->AppendText(wxT("Verify document.\n"));
		ecsver(selection);
		haveDocument = true;
		break;
	}


	case wxSOCKET_LOST:
	{
		sock->Destroy();
		DocumentRequesterRequestReportTextCtrl->AppendText (wxT("\n\nConnection closed.\n"));

		break;
	}
	}
	// Enable input events again.
	sock->SetNotify (wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
}

void DocumentRequester::ecsver (const wxString& document)
{
    long wxExecuteReturnValue;
    wxString tmp;
    wxString callingString;
    wxArrayString ellipticCurveVerifierResultsStdout;
    wxArrayString ellipticCurveVerifierProcessStderr;

#ifdef __APPLE__
    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxString dataDirectory =
        wxStandardPaths::Get().GetUserDataDir();

    wxString ecsverExecutable =
        resourceDirectory + wxT("/ecsver");

    wxFileName documentFile(document);
    wxString documentName = documentFile.GetFullName();

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
        ecsverExecutable,
        dataDirectory,
        documentPath);

#else

    tmp = selection;

#ifdef ACTUALRUN
    tmp.Replace(wxT("../usr/DigitalSignatureDocs/"),
                wxT(""), false);

    wxString ellipticCurveVerifierProcess =
        wxT("./ecsver ");

    callingString =
        ellipticCurveVerifierProcess +
        wxT(". ") +
        wxT("../DigitalSignatureDocs/") +
        tmp;
#else
    tmp.Replace(
        wxT("../../../../../usr/DigitalSignatureDocs/"),
        wxT(""), false);

    wxString ellipticCurveVerifierProcess =
        wxT("../../../../../usr/resource/ecsver ");

    callingString =
        ellipticCurveVerifierProcess +
        wxT("../../../../../usr/resource ") +
        wxT("../../../../../usr/DigitalSignatureDocs/") +
        tmp;
#endif

#endif

    wxExecuteReturnValue =
        wxExecute(callingString,
                  ellipticCurveVerifierResultsStdout,
                  ellipticCurveVerifierProcessStderr,
                  wxEXEC_SYNC |
                  wxEXEC_NOEVENTS |
                  wxEXEC_HIDE_CONSOLE);

    displayStdOut(
        ellipticCurveVerifierResultsStdout,
        DocumentRequesterRequestReportTextCtrl);

    DocumentRequesterRequestReportTextCtrl->AppendText(
        _("---------------------------------\n"));
}

void DocumentRequester::outputCurveParameters(const char *appfile)
{
	string line;
	ifstream appoutputfile(appfile);
	int i = 0;

	while (getline(appoutputfile, line))
	{
		switch (i)
		{
		case BITS:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("Number of bits = ") + wxString(line) + wxT("\n"));
			break;

		case P:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("Prime = ") + wxString(line) + wxT("\n"));
			break;

		case A:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("A =") + wxString(line) + wxT("\n"));
			break;

		case B:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("B = ") + wxString(line) + wxT("\n"));
			break;

		case ORDER:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("Basepoint order = ") + wxString(line) + wxT("\n"));
			break;

		case X:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("x = ") + wxString(line) + wxT("\n"));
			break;

		case Y:
			DocumentRequesterRequestReportTextCtrl->AppendText(wxT("y =") + wxString(line) + wxT("\n"));
			break;
		}
		i++;
	}

	appoutputfile.close();
}

void DocumentRequester::displayStdOut(wxArrayString& arr, wxTextCtrl *t)
{
	int numReturnedOutputs, i;

	numReturnedOutputs = arr.GetCount();
	t->AppendText(wxT("\n"));
	for (i=0; i<numReturnedOutputs; i++)
	{
		t->AppendText(arr[i] + wxT("\n"));
	}
}

void DocumentRequester::rmECSfiles()
{
#ifdef __APPLE__
    wxString dataDirectory =
        wxStandardPaths::Get().GetUserDataDir();

    wxFileName commonFile(
        dataDirectory,
        wxT("common.ecs"));

    wxFileName publicFile(
        dataDirectory,
        wxT("public.ecs"));

    wxFileName privateFile(
        dataDirectory,
        wxT("private.ecs"));

    if (wxFile::Exists(commonFile.GetFullPath()))
        wxRemoveFile(commonFile.GetFullPath());

    if (wxFile::Exists(publicFile.GetFullPath()))
        wxRemoveFile(publicFile.GetFullPath());

    if (wxFile::Exists(privateFile.GetFullPath()))
        wxRemoveFile(privateFile.GetFullPath());
		wxFileName selectedDocument(fileNameSelected);

		wxFileName signatureFile(
    dataDirectory,
    selectedDocument.GetName() + wxT(".ecs"));

if (wxFile::Exists(signatureFile.GetFullPath()))
    wxRemoveFile(signatureFile.GetFullPath());
#else

#ifdef ACTUALRUN
    remove("./common.ecs");
    remove("./public.ecs");
    remove("./private.ecs");
    system("find ../DigitalSignatureDocs -wholename '../DigitalSignatureDocs/*.ecs' -delete");
#else
    remove("../../../../../usr/resource/common.ecs");
    remove("../../../../../usr/resource/public.ecs");
    remove("../../../../../usr/resource/private.ecs");
    system("find ../../../../../usr/DigitalSignatureDocs -wholename '../../../../../usr/DigitalSignatureDocs/*.ecs' -delete");
#endif

#endif
}
