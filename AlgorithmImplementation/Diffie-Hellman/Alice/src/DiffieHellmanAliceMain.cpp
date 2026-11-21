/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#include <wx/msgdlg.h>

#include "DiffieHellmanAlice.h"
#include <miracl/big.h>

Miracl precision (5000,2);

#ifdef __APPLE__
#include <CFBundle.h>
#endif

#ifndef _WIN32
bool exe_name (char *str);
#endif

// application class
class DiffieHellmanAliceMainApp : public wxApp
{
public:
	// function called at the application initialization
	virtual bool OnInit();
	void reportError (const wxString& errmsg);
};

IMPLEMENT_APP (DiffieHellmanAliceMainApp);

bool DiffieHellmanAliceMainApp::OnInit()
{
#ifdef _WIN32
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

	wxImage::AddHandler (new wxPNGHandler);
	wxImage::AddHandler (new wxXPMHandler);
	wxXmlResource::Get()->InitAllHandlers();
	SetAppName (wxT ("DiffieHellmanAlice") );
	SetVendorName (wxT ("wxWidgets") );

	// create a new frame and set it as the top most application window
	SetTopWindow ( new DiffieHellmanAlice ( NULL, "DiffieHellmanAlice.png", -1, wxT (""), wxDefaultPosition, wxSize ( 1024, 768) ) );

	// show main frame
	GetTopWindow()->Show();

	// enter the application's main loop
	return true;
}

#ifndef _WIN32

bool exe_name (char *str)
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

void DiffieHellmanAliceMainApp::reportError (const wxString& errmsg)
{
	wxString caption;

	caption.append (wxT ("Error") );

	wxMessageDialog dialog (NULL, errmsg, caption, wxOK | wxICON_ERROR);
	dialog.ShowModal();
	exit (1);
}
