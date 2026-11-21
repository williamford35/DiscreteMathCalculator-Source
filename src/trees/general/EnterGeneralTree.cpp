/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/general/EnterGeneralTree.h"

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/msgdlg.h>

long EnterGeneralTree::ID_NODE_PANEL[16];
long EnterGeneralTree::ID_NODE_NUMBER_STATIC_TEXT[16];
long EnterGeneralTree::ID_NODE_NAME_TEXTCTRL[16];
long EnterGeneralTree::ID_PARENT_INDEX_TEXTCTRL[16];
const long EnterGeneralTree::ID_NODE_NAME_STATICTEXT = wxNewId();
const long EnterGeneralTree::ID_PARENT_INDEX_STATICTEXT = wxNewId();
const long EnterGeneralTree::ID_FILLER_PANEL1 = wxNewId();
const long EnterGeneralTree::ID_FILLER_PANEL2 = wxNewId();
const long EnterGeneralTree::ID_FILLER_PANEL3 = wxNewId();
const long EnterGeneralTree::ID_GENERAL_TREE_BUILD_BUTTON = wxNewId();
const long EnterGeneralTree::ID_GENERAL_TREE_CANCEL_BUTTON = wxNewId();
const long EnterGeneralTree::ID_GENERAL_TREE_BUTTON_PANEL = wxNewId();

BEGIN_EVENT_TABLE (EnterGeneralTree,wxDialog)
	//(*EventTable(EnterGeneralTree)
	//*)
END_EVENT_TABLE()

EnterGeneralTree::EnterGeneralTree (wxWindow* parent, wxVector<wxString> *gtrLines) :
	gtrLinesFromDialog (gtrLines)
{
	wxGridSizer* GeneralTreeGridSizer;
	wxBoxSizer* ButtonsBoxSizer;
	wxStaticText *NodeNameStaticText1, *ParentIndexStaticText1,
	             *NodeNameStaticText2, *ParentIndexStaticText2;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	GeneralTreeGridSizer = new wxGridSizer (0, 4, 0, 0);
	NodeNameStaticText1 = new wxStaticText (this, ID_NODE_NAME_STATICTEXT, _ ("Node name"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_NODE_NAME_STATICTEXT"));
	GeneralTreeGridSizer->Add (NodeNameStaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ParentIndexStaticText1 = new wxStaticText (this, ID_PARENT_INDEX_STATICTEXT, _ ("Parent index"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_PARENT_INDEX_STATICTEXT"));
	GeneralTreeGridSizer->Add (ParentIndexStaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	NodeNameStaticText2 = new wxStaticText (this, ID_NODE_NAME_STATICTEXT, _ ("Node name"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_NODE_NAME_STATICTEXT"));
	GeneralTreeGridSizer->Add (NodeNameStaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ParentIndexStaticText2 = new wxStaticText (this, ID_PARENT_INDEX_STATICTEXT, _ ("Parent index"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_PARENT_INDEX_STATICTEXT"));
	GeneralTreeGridSizer->Add (ParentIndexStaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	for (int i = 0; i < 16; i++)
	{
		ID_NODE_PANEL[i] = wxNewId();
		ID_NODE_NUMBER_STATIC_TEXT[i] = wxNewId();
		ID_NODE_NAME_TEXTCTRL[i] = wxNewId();
		ID_PARENT_INDEX_TEXTCTRL[i] = wxNewId();
		addCell (i,GeneralTreeGridSizer);
	}

	FillerPanel1 = new wxPanel (this, ID_FILLER_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_FILLER_PANEL1"));
	GeneralTreeGridSizer->Add (FillerPanel1, 1, wxALL|wxEXPAND, 5);
	FillerPanel2 = new wxPanel (this, ID_FILLER_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_FILLER_PANEL2"));
	GeneralTreeGridSizer->Add (FillerPanel2, 1, wxALL|wxEXPAND, 5);
	FillerPanel3 = new wxPanel (this, ID_FILLER_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_FILLER_PANEL3"));
	GeneralTreeGridSizer->Add (FillerPanel3, 1, wxALL|wxEXPAND, 5);
	ButtonPanel = new wxPanel (this, ID_GENERAL_TREE_BUTTON_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_GENERAL_TREE_BUTTON_PANEL"));
	ButtonsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	BuildButton = new wxButton (ButtonPanel, ID_GENERAL_TREE_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_GENERAL_TREE_BUILD_BUTTON"));
	BuildButton->SetDefault();
	ButtonsBoxSizer->Add (BuildButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CancelButton = new wxButton (ButtonPanel, ID_GENERAL_TREE_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_GENERAL_TREE_CANCEL_BUTTON"));
	ButtonsBoxSizer->Add (CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonPanel->SetSizer (ButtonsBoxSizer);
	ButtonsBoxSizer->Fit (ButtonPanel);
	ButtonsBoxSizer->SetSizeHints (ButtonPanel);
	GeneralTreeGridSizer->Add (ButtonPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer (GeneralTreeGridSizer);
	GeneralTreeGridSizer->Fit (this);
	GeneralTreeGridSizer->SetSizeHints (this);

	Connect (ID_GENERAL_TREE_BUILD_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterGeneralTree::OnGeneralTreeBuildButtonClick);
	Connect (ID_GENERAL_TREE_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterGeneralTree::OnGeneralTreeCancelButtonClick);
}

EnterGeneralTree::~EnterGeneralTree()
{
	//(*Destroy(EnterGeneralTree)
	//*)
}

void EnterGeneralTree::addCell (int i, wxGridSizer *grid)
{
	wxPanel *nodePanel;
	wxBoxSizer *nodePanelBoxSizer;
	wxStaticText *nodeNumberStaticText;
	wxTextCtrl *nodeName, *parentIndex;
	wxString id, nodeNumber;

	id.Printf (wxT ("ID_NODE_PANEL%d"),i);
	nodePanel = new wxPanel (this, ID_NODE_PANEL[i], wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, id);
	nodePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	id.Printf (wxT ("ID_NODE_NUMBER_STATIC_TEXT%d"),i);
	nodeNumber.Printf (wxT ("%d"),i);
	nodeNumberStaticText = new wxStaticText (nodePanel, ID_NODE_NUMBER_STATIC_TEXT[i], nodeNumber, wxDefaultPosition, wxDefaultSize, 0, id);
	nodePanelBoxSizer->Add (nodeNumberStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	id.Printf (wxT ("ID_NODE_NAME_TEXTCTRL%d"),i);
	nodeName = new wxTextCtrl (nodePanel, ID_NODE_NAME_TEXTCTRL[i], wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, id);
	nodePanelBoxSizer->Add (nodeName, 8, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	nodePanel->SetSizer (nodePanelBoxSizer);
	nodePanelBoxSizer->Fit (nodePanel);
	nodePanelBoxSizer->SetSizeHints (nodePanel);
	grid->Add (nodePanel, 1, wxALL|wxEXPAND, 5);
	id.Printf (wxT ("ID_PARENT_INDEX_TEXTCTRL%d"),i);
	parentIndex = new wxTextCtrl (this, ID_PARENT_INDEX_TEXTCTRL[i], wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, id);
	grid->Add (parentIndex, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	nodeEntry.push_back (NodeEntry (nodeName, parentIndex));
}

void EnterGeneralTree::OnGeneralTreeBuildButtonClick (wxCommandEvent& event)
{
	int i;
	wxString nodeName, parentIndex, line, error;
	NodeEntry entry;
	long intTest;

	for (i=0; i < 16; i++)
	{
		entry = nodeEntry[i];
		nodeName = (entry.labelTextCtrl)->GetValue();

		if (nodeName.empty())
			break;

		parentIndex = (entry.parentIndexTextCtrl)->GetValue();

		if (parentIndex.empty())
		{
			error.Printf (wxT ("There is no parent index at entry %d"), i);
			reportError (error);
			nodeEntry.clear();
			gtrLinesFromDialog->clear();
			return;
		}

		if (i != 0 && !parentIndex.ToLong (&intTest))
		{
			error.Printf (wxT ("The parent index at entry %d is not an integer"), i);
			reportError (error);
			nodeEntry.clear();
			gtrLinesFromDialog->clear();
		}

		line.Printf (wxT ("%d %s %s"), i, nodeName.c_str(), parentIndex.c_str());
		gtrLinesFromDialog->push_back (line);
	}

	EndModal (wxID_OK);
}

void EnterGeneralTree::OnGeneralTreeCancelButtonClick (wxCommandEvent& event)
{
	nodeEntry.clear();
	gtrLinesFromDialog->clear();
	EndModal (wxID_CANCEL);
}

void EnterGeneralTree::reportError (const wxString& errMsg)
{
	wxString caption;

	caption.append (wxT ("Error"));

	wxMessageDialog dialog (this, errMsg, caption, wxOK | wxICON_ERROR);
	dialog.ShowModal();
}
