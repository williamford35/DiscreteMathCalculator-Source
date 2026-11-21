#include <wx/string.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "ImageFiles.h"

wxString imagefile(const char *f)
{
    wxString filename = wxString::FromUTF8(f);

    wxFileName executable(wxStandardPaths::Get().GetExecutablePath());
    wxString executableDir = executable.GetPath();

    wxString directories[] = {
        executableDir + "/../images",
        executableDir + "/../../../usr/images",
        executableDir + "/../Resources/images",
        executableDir + "/../Resources/usr/images",
        "usr/images"
    };

    for (const auto& directory : directories)
    {
        wxFileName candidate(directory, filename);

        if (candidate.FileExists())
            return candidate.GetFullPath();
    }

    return wxFileName("usr/images", filename).GetFullPath();
}
