/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "TextDisplay.h"
#include <wx/persist.h>

TextDisplay::TextDisplay (wxWindow* parent, const wxString& heading, wxTextFile& doc, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style)
{
	wxString line;

	wxFlexGridSizer* TextDisplayFlexGridSizer = new wxFlexGridSizer (1, 1, 0, 0);
	TextDisplayFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	TextDisplayFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	TextDisplayFlexGridSizer->AddGrowableCol (0);
	TextDisplayFlexGridSizer->AddGrowableRow (0);
	this->SetSizer (TextDisplayFlexGridSizer);

	TextDisplayContentTextCtrl = new wxTextCtrl (this, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (this, wxSize (300,400) ), wxTE_MULTILINE|wxTE_CHARWRAP);

	TextDisplayFlexGridSizer->Add (TextDisplayContentTextCtrl, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	// SetName (wxT ("TextDisplay") );
	SetSize (1024,768);

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	if (GetParent() )
	{
		CentreOnParent (wxBOTH);
	}
	else
	{
		CentreOnScreen (wxBOTH);
	}

#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this) )
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif

	for ( line = doc.GetFirstLine(); !doc.Eof(); line = doc.GetNextLine() )
	{
		TextDisplayContentTextCtrl->AppendText(line + wxT("\n"));
	}
	TextDisplayContentTextCtrl->AppendText(line + wxT("\n"));
	TextDisplayContentTextCtrl->SetSelection(0,0);
}

TextDisplay::~TextDisplay()
{
}
