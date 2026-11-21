#ifndef STABLEMARRIAGEDIALOG_BASE_CLASSES_H
#define STABLEMARRIAGEDIALOG_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/button.h>

#include "common/matrix.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"

#include <vector>
using namespace std;

class StableMarriageDialog : public wxDialog
{
protected:
	wxPanel* ID_MEN_WOMEN_LABEL_PANEL;
	wxPanel* ID_NUMBER_OF_MEN_AND_WOMEN_PANEL;
	wxStaticText* ID_N_STATICTEXT;
	wxTextCtrl* ID_N_TEXTCTRL;
	wxPanel* ID_PARTNERS_PANEL;
	wxListCtrl* ID_PARTNERS_LISTCTRL;
	wxPanel* ID_BUTTONS_PANEL;
	wxButton* ID_CREATE_PREFERENCES_BUTTON;
	wxButton* ID_MATCH_BUTTON;
	wxButton* ID_CLOSE_BUTTON;

protected:
	virtual void OncreatePreferencesButtonClick (wxCommandEvent& event);
	virtual void OnmatchButtonClick (wxCommandEvent& event);
	virtual void OncloseButtonClick (wxCommandEvent& event);

private:
	matrix<int> *menPref, *womenPref;
	int n;
	wxWindow* myParent;
	DisplayAlgorithmResults **pairingGraph;

	void InitWithMenAndWomen();
	void InsertItem (wxListCtrl* listCtrl, int i);
	void Shuffle (vector<int>& womenForMen, vector<int>& menForWomen);
	void drawMatching (const vector<int>& partners);


public:
	StableMarriageDialog (wxWindow* parent, DisplayAlgorithmResults **pairg, wxWindowID id = wxID_ANY, const wxString& title = _ ("Stable Marriage Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (930,500), long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~StableMarriageDialog();
};

#endif
