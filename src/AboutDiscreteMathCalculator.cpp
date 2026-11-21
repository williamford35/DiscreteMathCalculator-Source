/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "AboutDiscreteMathCalculator.h"
#include "common/ImageFiles.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/msgdlg.h>

#include "NTL/tools.h"

#define FOR_DISTRIBUTION
// #undef FOR_DISTRIBUTION

NTL_CLIENT

//IdInit(AboutDiscreteMathCalculator)
const long AboutDiscreteMathCalculator::ID_ABOUT_STATICBITMAP = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_DESCRIPTION_STATICTEXT = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_DESCRIPTION_PANEL = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_INFORMATION_STATICTEXT = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_INFORMATION_PANEL = wxNewId();
const long AboutDiscreteMathCalculator::ID_THANKS_TO_STATICTEXT = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_THANKS_TO_PANEL = wxNewId();
const long AboutDiscreteMathCalculator::ID_LICENSE_STATICTEXT = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_LICENSE_PANEL = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_NOTEBOOK = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_NOTEBOOK_PANEL = wxNewId();
const long AboutDiscreteMathCalculator::ID_CLOSE_BUTTON = wxNewId();
const long AboutDiscreteMathCalculator::ID_ABOUT_CLOSE_PANEL = wxNewId();
//

BEGIN_EVENT_TABLE (AboutDiscreteMathCalculator,wxDialog)
	//(*EventTable(AboutDiscreteMathCalculator)
	//*)
END_EVENT_TABLE()

AboutDiscreteMathCalculator::AboutDiscreteMathCalculator (wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//Initialize(AboutDiscreteMathCalculator)
	wxFlexGridSizer* aboutNotebookFlexGridSizer;
	wxFlexGridSizer* aboutFlexGridSizer;
	wxFlexGridSizer* aboutCloseFlexGridSizer;
	wxTextFile aboutTopicFile;
	wxString topic, topicContent;

	Create (parent, wxID_ANY, _ ("About Discrete Mathematics Calculator"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	aboutFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	aboutFlexGridSizer->AddGrowableCol (0);
	aboutFlexGridSizer->AddGrowableRow (0);
	aboutFlexGridSizer->AddGrowableRow (1);
	aboutStaticBitmap = new wxStaticBitmap (this, ID_ABOUT_STATICBITMAP, wxBitmap (wxImage (imagefile ("about.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ABOUT_STATICBITMAP"));
	aboutFlexGridSizer->Add (aboutStaticBitmap, 1, wxALL|wxEXPAND, 1);
	aboutNotebookPanel = new wxPanel (this, ID_ABOUT_NOTEBOOK_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ABOUT_NOTEBOOK_PANEL"));
	aboutNotebookFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	aboutNotebookFlexGridSizer->AddGrowableCol (0);
	aboutNotebookFlexGridSizer->AddGrowableRow (0);
	aboutNotebookFlexGridSizer->AddGrowableRow (1);
	aboutNotebook = new wxNotebook (aboutNotebookPanel, ID_ABOUT_NOTEBOOK, wxDefaultPosition, wxDefaultSize, 0, _T ("ID_ABOUT_NOTEBOOK"));

	aboutDescriptionPanel = new wxPanel (aboutNotebook, ID_ABOUT_DESCRIPTION_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ABOUT_DESCRIPTION_PANEL"));
	topic = aboutTopic("Description.txt");

	if (!aboutTopicFile.Open(topic))
	{

		throw FileErrorObject("");
	}

	topicContent = readAboutFile(aboutTopicFile);
	aboutDescriptionStaticText = new wxStaticText (aboutDescriptionPanel, ID_ABOUT_DESCRIPTION_STATICTEXT, topicContent, wxPoint (8,8), wxDefaultSize, 0, _T ("ID_ABOUT_DESCRIPTION_STATICTEXT"));

	aboutInformationPanel = new wxPanel (aboutNotebook, ID_ABOUT_INFORMATION_PANEL, wxPoint (24,8), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ABOUT_INFORMATION_PANEL"));
	topic = aboutTopic("Information.txt");
	aboutTopicFile.Clear();

	if (!aboutTopicFile.Open(topic))
	{
		throw FileErrorObject("");
	}

	topicContent = readAboutFile(aboutTopicFile);
	aboutInformationStaticText = new wxStaticText (aboutInformationPanel, ID_ABOUT_INFORMATION_STATICTEXT, topicContent, wxPoint (8,8), wxDefaultSize, 0, _T ("ID_ABOUT_INFORMATION_STATICTEXT"));

	aboutThanksToPanel = new wxPanel (aboutNotebook, ID_ABOUT_THANKS_TO_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ABOUT_THANKS_TO_PANEL"));
	topic = aboutTopic("Thanks-to.txt");
	aboutTopicFile.Clear();

	if (!aboutTopicFile.Open(topic))
	{
		throw FileErrorObject("");
	}

	topicContent = readAboutFile(aboutTopicFile);
	thanksToStaticText = new wxStaticText (aboutThanksToPanel, ID_THANKS_TO_STATICTEXT, topicContent, wxPoint (8,8), wxDefaultSize, 0, _T ("ID_THANKS_TO_STATICTEXT"));

	aboutLicensePanel = new wxPanel (aboutNotebook, ID_ABOUT_LICENSE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ABOUT_LICENSE_PANEL"));
	topic = aboutTopic("License.txt");
	aboutTopicFile.Clear();

	if (!aboutTopicFile.Open(topic))
	{
		throw FileErrorObject("");
	}

	topicContent = readAboutFile(aboutTopicFile);
	licenseStaticText = new wxStaticText (aboutLicensePanel, ID_LICENSE_STATICTEXT, topicContent, wxPoint (8,8), wxDefaultSize, 0, _T ("ID_LICENSE_STATICTEXT"));

	aboutNotebook->AddPage (aboutDescriptionPanel, _ ("Description"), false);
	aboutNotebook->AddPage (aboutInformationPanel, _ ("Information"), false);
	aboutNotebook->AddPage (aboutThanksToPanel, _ ("Thanks to..."), true);
	aboutNotebook->AddPage (aboutLicensePanel, _ ("License"), false);
	aboutNotebookFlexGridSizer->Add (aboutNotebook, 1, wxALL|wxEXPAND, 1);
	aboutNotebookPanel->SetSizer (aboutNotebookFlexGridSizer);
	aboutNotebookFlexGridSizer->Fit (aboutNotebookPanel);
	aboutNotebookFlexGridSizer->SetSizeHints (aboutNotebookPanel);
	aboutFlexGridSizer->Add (aboutNotebookPanel, 1, wxALL|wxEXPAND, 1);
	aboutClosePanel = new wxPanel (this, ID_ABOUT_CLOSE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ABOUT_CLOSE_PANEL"));
	aboutCloseFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	aboutCloseFlexGridSizer->AddGrowableCol (1);
	aboutCloseFlexGridSizer->Add (-1,-1,1, wxALL|wxEXPAND, 1);
	closeButton = new wxButton (aboutClosePanel, ID_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_CLOSE_BUTTON"));
	aboutCloseFlexGridSizer->Add (closeButton, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 1);
	aboutClosePanel->SetSizer (aboutCloseFlexGridSizer);
	aboutCloseFlexGridSizer->Fit (aboutClosePanel);
	aboutCloseFlexGridSizer->SetSizeHints (aboutClosePanel);
	aboutFlexGridSizer->Add (aboutClosePanel, 1, wxALL|wxEXPAND, 1);
	SetSizer (aboutFlexGridSizer);
	aboutFlexGridSizer->Fit (this);
	aboutFlexGridSizer->SetSizeHints (this);

	Connect (ID_CLOSE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &AboutDiscreteMathCalculator::OncloseButtonClick);

	Centre();
}

AboutDiscreteMathCalculator::~AboutDiscreteMathCalculator()
{
	//(*Destroy(AboutDiscreteMathCalculator)
	//*)
}

void AboutDiscreteMathCalculator::OncloseButtonClick (wxCommandEvent& event)
{
	Close();
}

wxString AboutDiscreteMathCalculator::aboutTopic(const char* topic)
{
#ifdef __APPLE__
		CFBundleRef bundle = CFBundleGetMainBundle();

		if (bundle != NULL)
		{
			CFURLRef bundleURL = CFBundleCopyBundleURL(bundle);

			if (bundleURL != NULL)
			{
					CFURLRef contentsURL =
						CFURLCreateCopyAppendingPathComponent(
							kCFAllocatorDefault,
							bundleURL,
							CFSTR("Contents"),
							true);

					if (contentsURL != NULL)
					{
						CFURLRef usrURL =
							CFURLCreateCopyAppendingPathComponent(
									kCFAllocatorDefault,
									contentsURL,
									CFSTR("usr/About-DiscreteMathCalculator"),
									true);

						if (usrURL != NULL)
						{
							char path[4096];

							if (CFURLGetFileSystemRepresentation(
										usrURL,
										true,
										reinterpret_cast<UInt8*>(path),
										sizeof(path)))
							{
									CFRelease(usrURL);
									CFRelease(contentsURL);
									CFRelease(bundleURL);

									return wxString::FromUTF8(path)
										+ wxFILE_SEP_PATH
										+ wxString::FromUTF8(topic);
							}

							CFRelease(usrURL);
						}

						CFRelease(contentsURL);
					}

					CFRelease(bundleURL);
			}
		}
	#endif

	#ifdef FOR_DISTRIBUTION
		wxString g = wxT("../usr/About-DiscreteMathCalculator/");
	#else
		wxString g = wxT("../../usr/About-DiscreteMathCalculator/");
	#endif

		return g + wxString::FromUTF8(topic);
	}

	wxString AboutDiscreteMathCalculator::readAboutFile(wxTextFile& topic)
	{
	wxString fileLine, fileContent;

	for (fileLine = topic.GetFirstLine(); !topic.Eof(); fileLine = topic.GetNextLine())
	{
		fileContent += fileLine;
		fileContent += wxT("\n");
	}

	return fileContent;
}
