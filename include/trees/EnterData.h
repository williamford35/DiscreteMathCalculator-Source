#ifndef __ENTERDATA_H__
#define __ENTERDATA_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
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
#include <wx/string.h>
#include <wx/vector.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class EnterData
///////////////////////////////////////////////////////////////////////////////
class EnterData : public wxDialog
{
public:

	EnterData (wxWindow* parent, wxVector<int> *inputData, wxString *err);
	~EnterData();

protected:

	static const long ID_STATIC_TEXT;
	static const long ID_TEXT_CTRL;
	static const long ID_BUILD_BUTTON;
	static const long ID_CANCEL_BUTTON;

private:
	void OnenterDataCancelButtonClick (wxCommandEvent& event);
	void OnenterBuildTreeButtonClick (wxCommandEvent& event);

	wxTextCtrl *dataTextCtrl;
	wxVector<int> *data;
	wxString *error;

	DECLARE_EVENT_TABLE()
};

#endif //__ENTERDATA_H__
