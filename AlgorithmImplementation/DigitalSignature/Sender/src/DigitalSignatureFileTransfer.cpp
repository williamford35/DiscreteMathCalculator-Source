#include "DigitalSignatureFileTransfer.h"

#ifdef __APPLE__
#include <wx/filename.h>
#include <wx/stdpaths.h>
#endif

string DigitalSignatureFileTransfer(const string& f)
{
#ifdef __APPLE__

    // Locate the Sender executable.
    wxFileName executable(
        wxStandardPaths::Get().GetExecutablePath()
    );

    // Sender is installed in Contents/Resources.
    wxString resourcesDir = executable.GetPath();

    // Documents are in Contents/Resources/DigitalSignatureDocs.
    wxFileName document(resourcesDir, wxEmptyString);
    document.AppendDir("DigitalSignatureDocs");
    document.SetFullName(wxString::FromUTF8(f.c_str()));

    return document.GetFullPath().ToStdString();

#elif defined(__linux__) || defined(_WIN32)

    return string("../DigitalSignatureDocs/") + f;

#else

    return f;

#endif
}
