#ifndef __CLOSUREDIALOG_H__
#define __CLOSUREDIALOG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>

class ClosureDialog : public wxDialog
{
protected:
	wxStaticBitmap* ClosureStaticBitmap;
	wxPanel* ClosureStaticTextPanel;
	wxStaticText* ClosureStaticText;
	wxPanel* ClosureRadioBoxPanel;
	wxRadioBox* ClosureRadioBox;
	wxPanel* ClosureButtonPanel;
	wxButton* ClosureProcessButton;
	wxButton* ClosureCancelButton;

	virtual void ClosureProcessButtonPress( wxCommandEvent& event );
	virtual void ClosureCancelButtonPress( wxCommandEvent& event );

public:
	enum ClosureType
	{
		REFLEXIVE,
		SYMMETRIC,
		TRANSITIVE
	};

	ClosureDialog( wxWindow* parent, const char *imageFileName, ClosureType *ctype, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
	~ClosureDialog();

private:

	ClosureType *selectedClosureType;
};

#endif //__CLOSUREDIALOG_H__
