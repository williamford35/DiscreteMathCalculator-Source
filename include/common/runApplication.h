#ifndef RUNAPPLICATION_H
#define RUNAPPLICATION_H

#include <wx/string.h>
#include <wx/arrstr.h>

wxArrayString runApplication(const wxString& appl, wxString arg[], int n, bool sync = true);

#endif // RUNAPPLICATION_H

