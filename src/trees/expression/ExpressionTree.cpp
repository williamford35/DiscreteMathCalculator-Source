/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/expression/ExpressionTree.h"
#include "trees/binary/BinaryTree.h"
#include <stdio.h>
#include <wx/textctrl.h>
#include <wx/msgdlg.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int parseExpression (const char *exp, wxVector<wxString> *btrLines, long long int *value);

extern int parseError;

//(*InternalHeaders(ExpressionTree)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ExpressionTree)
const long ExpressionTree::ID_TITLE_STATICTEXT = wxNewId();
const long ExpressionTree::ID_POSTFIXOUTPUT_TEXTCTRL = wxNewId();
const long ExpressionTree::ID_BUILD_BUTTON = wxNewId();
const long ExpressionTree::ID_CLEAR_BUTTON = wxNewId();
const long ExpressionTree::ID_CANCEL_BUTTON = wxNewId();
const long ExpressionTree::ID_EXPRESSIONTREE_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (ExpressionTree,wxDialog)
	//(*EventTable(ExpressionTree)
	//*)
END_EVENT_TABLE()

ExpressionTree::ExpressionTree (wxWindow* parent, stringstream *parserOut, wxVector<wxString> *btrL,
                                long long int *value) :
	parserOutput (parserOut), btrLines (btrL), expValue (value)
{
	//(*Initialize(ExpressionTree)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer (wxVERTICAL);
	TitleStaticText = new wxStaticText (this, ID_TITLE_STATICTEXT, _ ("Enter an Infix Expression (+, -, *, /, %, ^, unary -)"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_TITLE_STATICTEXT"));
	BoxSizer1->Add (TitleStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	ExpressionTreePanel = new wxPanel (this, ID_EXPRESSIONTREE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_EXPRESSIONTREE_PANEL"));
	BoxSizer2 = new wxBoxSizer (wxHORIZONTAL);
	PostfixOutputTextCtrl = new wxTextCtrl (ExpressionTreePanel, ID_POSTFIXOUTPUT_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize (400,200), wxTE_MULTILINE, wxDefaultValidator, _T ("ID_POSTFIXOUTPUT_TEXTCTRL"));
	BoxSizer2->Add (PostfixOutputTextCtrl, 4, wxALL|wxEXPAND, 1);
	BuildButton = new wxButton (ExpressionTreePanel, ID_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BUILD_BUTTON"));
	BuildButton->SetDefault();
	BoxSizer2->Add (BuildButton, 1, wxALL|wxALIGN_BOTTOM, 3);
	ClearButton = new wxButton (ExpressionTreePanel, ID_CLEAR_BUTTON, _ ("Clear"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_CLEAR_BUTTON"));
	BoxSizer2->Add (ClearButton, 1, wxALL|wxALIGN_BOTTOM, 3);
	CancelButton = new wxButton (ExpressionTreePanel, ID_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_CANCEL_BUTTON"));
	BoxSizer2->Add (CancelButton, 1, wxALL|wxALIGN_BOTTOM, 3);
	ExpressionTreePanel->SetSizer (BoxSizer2);
	BoxSizer2->Fit (ExpressionTreePanel);
	BoxSizer2->SetSizeHints (ExpressionTreePanel);
	BoxSizer1->Add (ExpressionTreePanel, 6, wxALL|wxEXPAND, 1);
	SetSizer (BoxSizer1);
	BoxSizer1->Fit (this);
	BoxSizer1->SetSizeHints (this);
	//*)
	Connect (ID_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &ExpressionTree::OnBuildButtonClick);
	Connect (ID_CLEAR_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &ExpressionTree::OnClearButtonClick);
	Connect (ID_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &ExpressionTree::OnCancelButtonClick);
}

ExpressionTree::~ExpressionTree()
{
	//(*Destroy(ExpressionTree)
	//*)
}

void ExpressionTree::OnBuildButtonClick (wxCommandEvent& event)
{
	wxString theExpression, outputResults;
	// parserOutput is output from flex/bison
	// expOut is the "btr" description of the expression tree
	streambuf *parserOutsbuf, *backupCout = std::cout.rdbuf();
	string line;

	theExpression = PostfixOutputTextCtrl->GetValue();

	if (theExpression.length() == 0)
		EndModal (wxID_CANCEL);

	theExpression.Append (wxT ("\04\n"));
	parserOutsbuf = parserOutput->rdbuf();	// get parserOutput's streambuf
	cout.rdbuf (parserOutsbuf);					// assign streambuf to cout
	const char *s = theExpression.mb_str (wxConvUTF8).data();

	if (parseExpression (s, btrLines, expValue) != 0)
	{
		// rewind parserOutput
		parserOutput->seekg (0,ios::beg);

		// output syntax error
		PostfixOutputTextCtrl->AppendText (wxString (parserOutput->str().c_str(),wxConvUTF8));

		cout.rdbuf (backupCout);	// restore cout's original streambuf
		// clear parserOutput
		parserOutput->str ("");
		return;	// Let's try again
	}

	wxString q = wxString (parserOutput->str().c_str(),wxConvUTF8);
	cout.rdbuf (backupCout);	// restore cout's original streambuf
	EndModal (wxID_OK);
}

void ExpressionTree::OnCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}


void ExpressionTree::OnClearButtonClick (wxCommandEvent& event)
{
	PostfixOutputTextCtrl->ChangeValue (wxT (""));
}
