#include <wx/string.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "common/ImageFiles.h"

wxString imagefile(const char *f)
{
#ifdef __APPLE__

    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxFileName image(resourceDirectory, wxEmptyString);
    image.AppendDir("images");
    image.SetFullName(wxString::FromUTF8(f));

    return image.GetFullPath();

#else

    wxString g = wxT("../../usr/images/");
    g += wxString::FromUTF8(f);

    return g;

#endif
}
