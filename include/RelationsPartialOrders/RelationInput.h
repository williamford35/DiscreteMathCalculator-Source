#ifndef DEVELOPMENT_RELATIONINPUT_H
#define DEVELOPMENT_RELATIONINPUT_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/stattext.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <vector>
using namespace std;

struct RelationInputCell
{
	wxTextCtrl *sourceTextCtrl;
	wxTextCtrl *destTextCtrl;

	RelationInputCell()
	{}

	RelationInputCell (wxTextCtrl *t1, wxTextCtrl *t2) :
		sourceTextCtrl (t1), destTextCtrl(t2)
	{}
};

struct RelationGraphEdge
{
	wxString edgeSource;
	wxString edgeDest;

	RelationGraphEdge()
	{}

	RelationGraphEdge (const wxString src, const wxString& dest) :
		edgeSource (src), edgeDest (dest)
	{}
};

class RelationInput : public wxDialog
{
protected:
	static long TEMPLATE1TEXTCTRL_ID[10];
	static long TEMPLATESTATICTEXT_ID[10];
	static long TEMPLATE2TEXTCTRL_ID[10];

	wxStaticBitmap* RelationInputStaticBitMap;

	wxPanel* RelationInputButtonsPanel;
	wxButton* RelationInputCancelButton;
	wxButton* RelationInputDisplayButton;
	wxPanel* RelationInputDataPanel;

	wxVector<RelationInputCell> theCell;
	vector<RelationGraphEdge> diGraphToBuild;

	void addCell (int i, wxGridSizer *grid);

protected:
	virtual void OnRelationInputCancelButtonPress (wxCommandEvent& event);
	virtual void OnRelationInputDisplayButtonPress (wxCommandEvent& event);

public:
	RelationInput (wxWindow* parent, const char *imageFileName);
	virtual ~RelationInput();
	vector<RelationGraphEdge> getGraphParameters();

	DECLARE_EVENT_TABLE()
};

#endif	// DEVELOPMENT_RELATIONINPUT_H
