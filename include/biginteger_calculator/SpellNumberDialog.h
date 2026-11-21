///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
///////////////////////////////////////////////////////////////////////////

#ifndef SPELLNUMBERDIALOG_H
#define SPELLNUMBERDIALOG_H

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class SpellNumberDialog
///////////////////////////////////////////////////////////////////////////////
class SpellNumberDialog : public wxDialog
{
private:
	static char *name1[], *name2[], *name3[];
	int lflag;
	char errormsg[50];

	void spellNumber (char *numberToSpell);
	void convert (char *);
	void convertexp (char *);
	int	number (const char *, int);
	void pfract (int);
	void toobig (void);
	int	unit (int, const char *);
	void printfToTextCtrl (char *formatStr, ...);


protected:
	wxTextCtrl* ID_SPELL_NUMBER_TEXTCTRL;
	wxButton* ID_SPELL_NUMBER_CLOSE_BUTTON;

	// Virtual event handlers, overide them in your derived class
	virtual void OnSpellNumberCloseButtonClick (wxCommandEvent& event)
	{
		Close();
	}


public:

	SpellNumberDialog (wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT ("Spell Number"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (400,500), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxRAISED_BORDER);
	~SpellNumberDialog();
	void writeSpelledValue (char *numberToBeSpelled);
};

#endif //SPELLNUMBERDIALOG_H
