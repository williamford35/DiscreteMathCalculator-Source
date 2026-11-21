#ifndef DEBUGDIALOG_H_INCLUDED
#define DEBUGDIALOG_H_INCLUDED

#include <wx/msgdlg.h>
#include <wx/string.h>

void debugDialog(const wxString& message)
{
	wxString caption;

	caption.append(wxT("Debug Information"));

	wxMessageDialog dialog(NULL, message, caption, wxOK | wxICON_INFORMATION );
	dialog.ShowModal();
}

#endif // DEBUGDIALOG_H_INCLUDED
