/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "DiscreteMathCalculatorApp.h"

//(*AppHeaders
// #include <unistd.h>

#include "DiscreteMathCalculatorMain.h"

#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#ifdef __APPLE__
#include "CFBundle.h"
#endif
//*)

#include "miracl/big.h"

wxIMPLEMENT_APP (DiscreteMathCalculatorApp);

bool DiscreteMathCalculatorApp::OnInit()
{
#ifdef _WIN64
	wxString exeDirectory;
	wchar_t path[2048];
	wxString exePath;

	GetModuleFileName (NULL, path, 2048);
	exePath = wxString (path);

	exeDirectory = exePath.substr (0, exePath.rfind (wxT ("\\") ) );

	if (!SetCurrentDirectory (exeDirectory.c_str() ) )
	{
		wxString message, caption;

		message.Printf (wxT ("Cannot change into the application directory %s"), exeDirectory.c_str() );
		caption.append (wxT ("Error") );

		wxMessageDialog dialog (NULL, message, caption, wxOK | wxICON_ERROR);
		dialog.ShowModal();
		exit (1);
	}
#elif __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyBundleURL (mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath (resourcesURL, kCFURLPOSIXPathStyle);
	CFRelease (resourcesURL);
	char path[PATH_MAX];

	CFStringGetCString (str, path, FILENAME_MAX, kCFStringEncodingASCII);
	CFRelease (str);

	if (chdir (path) != 0)
		reportError (wxT ("Cannot change into the directory containing the application. Serious! Please report.") );
#else
	char path[PATH_MAX+1], *p;

	if (!exe_name (path) )
		reportError (wxT ("Cannot locate the directory containing the application. Serious! Please report.") );
	if ( (p = strrchr (path, '/') ) )
		* (p+1) = '\0';

	if (chdir (path) != 0)
		reportError (wxT ("Cannot change into the directory containing the application. Serious! Please report.") );

#endif
	//(*AppInitialize
	wxImage::AddHandler (new wxPNGHandler);
	wxImage::AddHandler (new wxJPEGHandler);
	wxImage::AddHandler (new wxXPMHandler);
	wxXmlResource::Get()->InitAllHandlers();
	SetAppName (wxT ("DiscreteMathCalculator") );
	SetVendorName (wxT ("wxWidgets") );

#if defined(_WIN64) || defined(__LINUX__)
	DiscreteMathCalculatorFrame* Frame = new DiscreteMathCalculatorFrame (0);
#else
	DiscreteMathCalculatorFrame* Frame = new DiscreteMathCalculatorFrame (NULL,
	      wxID_ANY,
	      wxT ("DiscreteMathCalculator"),
	      wxPoint (30,30),
	      wxDefaultSize,
	      wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);
#endif

#ifdef __LINUX__
	Frame->SetClientSize(1400,800);
#else
	Frame->SetClientSize(1200,800);
#endif
	Frame->Show();
	SetTopWindow (Frame);
	SetExitOnFrameDelete (true);

	return true;
}

int DiscreteMathCalculatorApp::OnExit()
{
	return 0;
}

#ifdef __LINUX__
bool DiscreteMathCalculatorApp::exe_name (char *str)
{
	FILE *fp;
	char buf[4096], *p;

	*str = '\0';

	if (! (fp = fopen ("/proc/self/maps", "r") ) )
		return false;

	fgets (buf, sizeof (buf), fp);
	fclose (fp);

	* (p = strchr (buf, '\n') ) = '\0';

	while (*p != ' ')
		p--;

	strncpy (str, p+1, PATH_MAX);
	str[PATH_MAX] = '\0';

	return true;
}
#endif

void DiscreteMathCalculatorApp::reportError (const wxString& errmsg)
{
	wxString caption;

	caption.append (wxT ("Error") );

	wxMessageDialog dialog (NULL, errmsg, caption, wxOK | wxICON_ERROR);
	dialog.ShowModal();
	exit (1);
}
