#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

//(*Headers(ExpressionTree)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include "trees/binary/BinaryTree.h"
#include <wx/string.h>
#include <wx/vector.h>

#include <sstream>
using namespace std;

struct parser;

class ExpressionTree: public wxDialog
{
public:

	ExpressionTree (wxWindow* parent, stringstream *parserOut, wxVector<wxString> *btrL,
	                long long int *value);
	virtual ~ExpressionTree();

	//(*Declarations(ExpressionTree)
	wxButton* BuildButton;
	wxPanel* ExpressionTreePanel;
	wxStaticText* TitleStaticText;
	wxButton* ClearButton;
	wxButton* CancelButton;
	wxTextCtrl* PostfixOutputTextCtrl;
	//*)

protected:

	//(*Identifiers(ExpressionTree)
	static const long ID_TITLE_STATICTEXT;
	static const long ID_POSTFIXOUTPUT_TEXTCTRL;
	static const long ID_BUILD_BUTTON;
	static const long ID_CLEAR_BUTTON;
	static const long ID_CANCEL_BUTTON;
	static const long ID_EXPRESSIONTREE_PANEL;
	//*)

private:

	//(*Handlers(ExpressionTree)
	void OnBuildButtonClick (wxCommandEvent& event);
	void OnClearButtonClick (wxCommandEvent& event);
	void OnCancelButtonClick (wxCommandEvent& event);
	//*)

	stringstream *parserOutput;
	wxVector<wxString> *btrLines;
	long long int *expValue;

	DECLARE_EVENT_TABLE()
};

#endif
