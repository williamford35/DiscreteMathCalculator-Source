#include "ImageFiles.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>

wxString imagefile(const char *f)
{
    const wxString filename = wxString::FromUTF8(f);

    wxFileName executable(wxStandardPaths::Get().GetExecutablePath());
    const wxString directory = executable.GetPath();

    const wxString directories[] = {
        directory + "/../images",
        directory + "/../../../usr/images",
        directory + "/../Resources/images",
        directory + "/../Resources/usr/images",
        "usr/images"
    };

    for (const wxString& candidate : directories)
    {
        wxFileName path(candidate, filename);

        if (path.FileExists())
            return path.GetFullPath();
    }

    return filename;
}
