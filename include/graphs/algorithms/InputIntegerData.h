#ifndef __INPUTINTEGERDATA_H__
#define __INPUTINTEGERDATA_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/dialog.h>

#include <vector>

using namespace std;

class InputIntegerData : public wxDialog
{
private:
	vector<int>* integerData;
    bool repeatGraphicalSequenceTests = false;

protected:
	wxStaticText* InputIntegerDataStaticText;
	wxTextCtrl* InputIntegerDataTextCtrl;
	wxPanel* InputIntegerDataButtonsPanel;
	wxButton* InputIntegerDataCancelButton;
	wxButton* InputIntegerDataDisplayButton;

	virtual void OnCancelButtonPress( wxCommandEvent& event );
	virtual void OnDisplayButtonPress( wxCommandEvent& event );

public:
	InputIntegerData( wxWindow* parent, vector<int>* dataVals, bool forPartialOrderDivisibilityInput = true, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,300 ), long style = wxDEFAULT_DIALOG_STYLE );
    void EnableRepeatedGraphicalSequenceTests();

    ~InputIntegerData();
};

#endif //__INPUTINTEGERDATA_H__
