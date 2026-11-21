#include <wx/string.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/arrstr.h>

#include "ImageFiles.h"

wxString imagefile(const char *f)
{
    wxString filename = wxString::FromUTF8(f);

    wxFileName executable(
        wxStandardPaths::Get().GetExecutablePath()
    );

    wxString executableDir = executable.GetPath();

    wxArrayString directories;

    // Resources next to the executable.
    directories.Add(executableDir + "/images");

    // Resources inside a macOS application bundle.
    directories.Add(executableDir + "/../Resources/images");

    // Project resources when running from AliceGF/CMakeBuild.
    directories.Add(executableDir + "/../../../../../usr/images");

    // Original relative pathname.
    directories.Add("../images");

    for (size_t i = 0; i < directories.GetCount(); ++i)
    {
        wxFileName image(directories[i], filename);

        if (image.FileExists())
            return image.GetFullPath();
    }

    // Preserve the original fallback behavior.
    return "../images/" + filename;
}
