///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#include "common/randomc.h"
#include <wx/vector.h>

///////////////////////////////////////////////////////////////////////////////
/// Class RandomData
///////////////////////////////////////////////////////////////////////////////
class RandomData : public wxDialog
{
private:
	wxVector<int> *randomData;
	long numberOfNodes;

	void displayRandomData();

protected:
	wxPanel* ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL;
	wxStaticText* ID_RANDOM_DATA_NUMBER_OF_NODES_STATICTEXT;
	wxTextCtrl* ID_RANDOM_DATA_NUMBER_OF_NODES_TEXTCTRL;
	wxTextCtrl* ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL;
	wxPanel* ID_RANDOM_DATA_BUTTONS_PANEL;
	wxButton* ID_RANDOM_DATA_GENERATE_DATA_BUTTON;
	wxButton* ID_RANDOM_DATA_BUILD_BUTTON;
	wxButton* ID_RANDOM_DATA_CANCEL_BUTTON;

	// Virtual event handlers, overide them in your derived class
	virtual void OnRandomDataGenerateClick (wxCommandEvent& event);
	virtual void OnRandomDataBuildClick (wxCommandEvent& event);
	virtual void OnRandomDataCancelClick (wxCommandEvent& event);


public:

	RandomData (wxWindow* parent, wxVector<int> *inputData, bool isForTree = true, wxWindowID id = wxID_ANY, const wxString& title = wxT ("Number of Randomly Generated Nodes"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (570,350), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	~RandomData();

	static bool error;
	static char errMsg[];
};

#endif //RANDOMDATA_H
