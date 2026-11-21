/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "report.h"

void report (const wxString& msg, const wxString& caption)
{
	wxMessageDialog dialog (NULL, msg, caption, wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}
