/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "common/report.h"

void reportError (const wxString& msg, const wxString& caption)
{
	wxMessageDialog dialog (NULL, msg, caption, wxICON_ERROR | wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}

void report (const wxString& msg, const wxString& caption)
{
	wxMessageDialog dialog (NULL, msg, caption,  wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}
