/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/huffman/EnterHuffmanData.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(EnterHuffmanData)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <vector>

using namespace std;

const long EnterHuffmanData::ID_BUILD_BUTTON = wxNewId();
const long EnterHuffmanData::ID_CANCEL_BUTTON = wxNewId();
const long EnterHuffmanData::ID_BUTTON_PANEL = wxNewId();
long EnterHuffmanData::ID_PANEL[26];
long EnterHuffmanData::ID_STATICTEXT[26];
long EnterHuffmanData::ID_TEXTCTRL[26];

BEGIN_EVENT_TABLE (EnterHuffmanData,wxDialog)
	//(*EventTable(EnterHuffmanData)
	//*)
END_EVENT_TABLE()

EnterHuffmanData::EnterHuffmanData (wxWindow* parent, int id, vector<HuffmanNode> *hnodes) :
	huffmanNodes (hnodes)
{
	wxFlexGridSizer* enterHuffmanDataFlexGridSizer;
	wxFlexGridSizer* buttonFlexGridSizer;
	wxGridSizer* cellArrayGridSizer;
	wxPanel *panel, *buttonPanel;
	wxButton *buildButton, *cancelButton;

	int i;

	for (i=0; i < 26; i++)
	{
		ID_PANEL[i] = wxNewId();
		ID_STATICTEXT[i] = wxNewId();
		ID_TEXTCTRL[i] = wxNewId();
	}

	id = -1;
	Create (parent, id, _ ("Enter Huffman Tree Data"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("id"));
	SetClientSize (wxSize (400,500));
	enterHuffmanDataFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	enterHuffmanDataFlexGridSizer->AddGrowableRow (0);
	cellArrayGridSizer = new wxGridSizer (4, 8, 0, 0);

	for (i=0; i < 26; i++)
	{
		panel = makeCell (freq, i);
		cellArrayGridSizer->Add (panel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	}

	for (i=0; i < 6; i++)
		cellArrayGridSizer->Add (0,0,1, wxALL|wxEXPAND, 5);

	enterHuffmanDataFlexGridSizer->Add (cellArrayGridSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	buttonPanel = new wxPanel (this, ID_BUTTON_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BUTTON_PANEL"));
	buttonFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	buttonFlexGridSizer->AddGrowableCol (0);
	buttonFlexGridSizer->Add (0,0,1, wxALL|wxEXPAND, 5);
	buildButton = new wxButton (buttonPanel, ID_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_BUILD_BUTTON"));
	buttonFlexGridSizer->Add (buildButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cancelButton = new wxButton (buttonPanel, ID_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_CANCEL_BUTTON"));
	buttonFlexGridSizer->Add (cancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	buttonPanel->SetSizer (buttonFlexGridSizer);
	buttonFlexGridSizer->Fit (buttonPanel);
	buttonFlexGridSizer->SetSizeHints (buttonPanel);
	enterHuffmanDataFlexGridSizer->Add (buttonPanel, 1, wxALL|wxEXPAND, 5);
	SetSizer (enterHuffmanDataFlexGridSizer);
	enterHuffmanDataFlexGridSizer->Fit (this);
	enterHuffmanDataFlexGridSizer->SetSizeHints (this);

	Connect (ID_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterHuffmanData::OnBuildButtonClick);
	Connect (ID_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterHuffmanData::OnCancelButtonClick);
	//*)
}

wxPanel *EnterHuffmanData::makeCell (wxTextCtrl *freq[], int i)
{
	wxBoxSizer *cellBoxSizer;
	wxPanel *panel;
	wxStaticText *letter;
	wxTextCtrl *cellFrequencyEntry;
	wxString panelName, staticTextName, strLetter;

	panelName.Printf (wxT ("ID_PANEL%d"), i+i);
	panel = new wxPanel (this, ID_PANEL[i], wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, panelName);
	cellBoxSizer = new wxBoxSizer (wxVERTICAL);
	staticTextName.Printf (wxT ("ID_STATICTEXT%d"), i+i);
	strLetter = (wxChar) ('A' + i);
	letter = new wxStaticText (panel, ID_STATICTEXT[i], strLetter, wxDefaultPosition, wxDefaultSize, 0, staticTextName);
	cellBoxSizer->Add (letter, 1, wxALL, 5);
	cellFrequencyEntry = new wxTextCtrl (panel, ID_TEXTCTRL[i], wxEmptyString, wxDefaultPosition,
	                                     wxSize(50,-1), 0, wxDefaultValidator, _T ("ID_TEXTCTRL1"));
	freq[i] = cellFrequencyEntry;
	cellBoxSizer->Add (cellFrequencyEntry, 1, wxALL, 5);
	panel->SetSizer (cellBoxSizer);
	cellBoxSizer->Fit (panel);
	cellBoxSizer->SetSizeHints (panel);

	return panel;
}

EnterHuffmanData::~EnterHuffmanData()
{
	for (int i=0; i < 26; i++)
		delete freq[i];
}

void EnterHuffmanData::OnBuildButtonClick (wxCommandEvent& event)
{
	HuffmanNode t;
	wxString textCtrlContents;
	long letterFreq;

	for (unsigned int i=0; i < 26; i++)
	{
		t.data = (wxChar) ('A' + i);
		t.myNodeInTree = NULL;

		textCtrlContents = freq[i]->GetValue();

		if (textCtrlContents != wxT (""))
		{
			textCtrlContents.ToLong (&letterFreq);
			t.frequency = letterFreq;
			huffmanNodes->push_back (t);
		}
	}

	if (huffmanNodes->size() != 0)
		EndModal (wxID_OK);
	else
		EndModal (wxID_CANCEL);
}

void EnterHuffmanData::OnCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
