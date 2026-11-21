#include "report.h"

void report (const wxString& msg, const wxString& caption)
{
	wxMessageDialog dialog (NULL, msg, caption, wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}
