/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/utils.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "common/report.h"

#include <iostream>
using namespace std;

wxArrayString runApplication(const wxString& appl, wxString arg[], int n, bool sync)
{
    wxString application;
    wxArrayString processStdout, processStderr;
    int wxExecuteReturnValue, i;

#ifdef __APPLE__

    /*
       wxStandardPaths::Get().GetResourcesDir() returns:

       DiscreteMathCalculator.app/Contents/Resources
    */

    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxFileName executable(resourceDirectory, wxEmptyString);
    executable.SetFullName(appl);

    application = wxT("\"") +
                  executable.GetFullPath() +
                  wxT("\" ");

#else

    application = wxT("../../usr/resource/") + appl + wxT(" ");

#endif

    if ((*arg).Length() != 0)
        if (n == 1)
            application += arg[0];
        else
            for (i = 0; i < n; i++)
                if (i <= n - 2)
                    application += arg[i] + wxT(" ");
                else
                    application += arg[i];

    if (sync)
        wxBeginBusyCursor();

    if (sync)
        wxExecuteReturnValue =
            wxExecute(application,
                      processStdout,
                      processStderr,
                      wxEXEC_SYNC | wxEXEC_NODISABLE);
    else
        wxExecuteReturnValue = wxExecute(application);

    if (sync)
        wxEndBusyCursor();

    if (wxExecuteReturnValue == -1)
    {
        report(wxT("Internal error: wxExecute did not run"),
               wxT("Note"));
        return wxArrayString();
    }

    if (processStderr.GetCount() == 0)
        return processStdout;
    else
        return processStderr;
}