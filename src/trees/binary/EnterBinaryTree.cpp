/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/binary/EnterBinaryTree.h"

//(*InternalHeaders(EnterBinaryTree)
#include <wx/intl.h>
#include <wx/string.h>
//*)

long EnterBinaryTree::ID_STATICTEXT[10];
long EnterBinaryTree::ID_TEXTCTRL[10];
long EnterBinaryTree::ID_RADIOBOX1[10];
long EnterBinaryTree::ID_RADIOBOX2[10];
const long EnterBinaryTree::ID_BUTTON_PANEL = wxNewId();
const long EnterBinaryTree::ID_ENTER_BINARY_TREE_BUILD_BUTTON = wxNewId();
const long EnterBinaryTree::ID_ENTER_BINARY_TREE_CANCEL_BUTTON = wxNewId();
const long EnterBinaryTree::ID_ENTER_BINARY_TREE_BUTTON_PANEL = wxNewId();

BEGIN_EVENT_TABLE (EnterBinaryTree,wxDialog)
	//(*EventTable(EnterBinaryTree)
	//*)
END_EVENT_TABLE()

EnterBinaryTree::EnterBinaryTree (wxWindow* parent, wxVector<wxString> *btrLines) :
	btrLinesFromDialog (btrLines)
{
	wxGridSizer* GridSizer;
	wxBoxSizer* BoxSizer;
	wxPanel* ButtonPanel;
	wxButton* BuildButton, *CancelButton;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	SetClientSize (wxDefaultSize);
	Move (wxDefaultPosition);
	GridSizer = new wxGridSizer (0, 2, 0, 0);

	for (int i = 0; i<10; i++)
	{
		ID_STATICTEXT[i] = wxNewId();
		ID_TEXTCTRL[i] = wxNewId();
		ID_RADIOBOX1[i] = wxNewId();
		ID_RADIOBOX2[i] = wxNewId();
		addCell (i, GridSizer);
	}

	GridSizer->Add (0,0,1, wxALL|wxEXPAND, 5);
	ButtonPanel = new wxPanel (this, ID_BUTTON_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BUTTON_PANEL"));
	BoxSizer = new wxBoxSizer (wxHORIZONTAL);
	BoxSizer->Add (0,0,1, wxALL|wxEXPAND, 5);
	BuildButton = new wxButton (ButtonPanel, ID_ENTER_BINARY_TREE_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ENTER_BINARY_TREE_BUILD_BUTTON"));
	BuildButton->SetDefault();
	BoxSizer->Add (BuildButton, 1, wxALL|wxALIGN_BOTTOM, 5);
	CancelButton = new wxButton (ButtonPanel, ID_ENTER_BINARY_TREE_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_ENTER_BINARY_TREE_CANCEL_BUTTON"));
	BoxSizer->Add (CancelButton, 1, wxALL|wxALIGN_BOTTOM, 5);
	ButtonPanel->SetSizer (BoxSizer);
	BoxSizer->Fit (ButtonPanel);
	BoxSizer->SetSizeHints (ButtonPanel);
	GridSizer->Add (ButtonPanel, 1, wxALL|wxEXPAND, 5);
	SetSizer (GridSizer);
	GridSizer->Fit (this);
	GridSizer->SetSizeHints (this);

	Connect (ID_ENTER_BINARY_TREE_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterBinaryTree::OnEnterBinaryTreeBuildButtonClick);
	Connect (ID_ENTER_BINARY_TREE_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterBinaryTree::OnEnterBinaryTreeCancelButtonClick);
}

EnterBinaryTree::~EnterBinaryTree()
{
	//(*Destroy(EnterBinaryTree)
	//*)
}


void EnterBinaryTree::OnEnterBinaryTreeBuildButtonClick (wxCommandEvent& event)
{
	int i;
	wxString parentLabel, leftChildTag, rightChildTag, line;
	Cell cell;

	for (i=0; i < 10; i++)
	{
		cell = theCell[i];
		parentLabel = (cell.node)->GetValue();

		if (parentLabel.length() == 0)
			break;

		if ( (cell.lchild)->GetSelection() == 0)
			leftChildTag = wxT (" 1 ");
		else
			leftChildTag = wxT (" 0 ");

		if ( (cell.rchild)->GetSelection() == 0)
			rightChildTag = wxT ("1");
		else
			rightChildTag = wxT ("0");

		line.Printf (wxT ("%s%s%s\n"), parentLabel.mb_str(),
		             leftChildTag.mb_str(), rightChildTag.mb_str());
		btrLinesFromDialog->push_back (line);
	}

	EndModal (wxID_OK);
}

void EnterBinaryTree::OnEnterBinaryTreeCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void EnterBinaryTree::addCell (int i, wxGridSizer *grid)
{
	wxPanel *panel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("wxID_ANY"));
	wxBoxSizer *BoxSizer = new wxBoxSizer (wxHORIZONTAL);
	wxStaticText *StaticText;
	wxTextCtrl *TextCtrl;
	wxRadioBox *RadioBox1,*RadioBox2;
	wxString nodeLabel, id;


	nodeLabel.Printf (wxT ("Node %d:"),i+1);
	id.Printf (wxT ("ID_STATICTEXT%d"),i+1);
	StaticText = new wxStaticText (panel, ID_STATICTEXT[i], nodeLabel, wxDefaultPosition, wxDefaultSize, 0,id);
	BoxSizer->Add (StaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	id.Printf (wxT ("ID_TEXTCTRL%d"),i+1);
	TextCtrl = new wxTextCtrl (panel, ID_TEXTCTRL[i], wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, id);
	BoxSizer->Add (TextCtrl, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	wxString __wxRadioBoxChoices_1[2] =
	{
		_ ("Yes"),
		_ ("No")
	};
	id.Printf (wxT ("ID_RADIOBOX1%d"),i+1);
	RadioBox1 = new wxRadioBox (panel, ID_RADIOBOX1[i], _ ("lChild"), wxDefaultPosition, wxSize(-1, 80), 2, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, id);
	BoxSizer->Add (RadioBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	wxString __wxRadioBoxChoices_2[2] =
	{
		_ ("Yes"),
		_ ("No")
	};
	id.Printf (wxT ("ID_RADIOBOX2%d"),i+1);
	RadioBox2 = new wxRadioBox (panel, ID_RADIOBOX2[i], _ ("rChild"), wxDefaultPosition, wxSize(-1, 80), 2, __wxRadioBoxChoices_2, 1, 0, wxDefaultValidator,id);
	BoxSizer->Add (RadioBox2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	panel->SetSizer (BoxSizer);
	BoxSizer->Fit (panel);
	BoxSizer->SetSizeHints (panel);
	theCell.push_back (Cell (TextCtrl,RadioBox1,RadioBox2));
	grid->Add (panel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
}
