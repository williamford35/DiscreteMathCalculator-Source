/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/EnterData.h"

#include <wx/tokenzr.h>

const long EnterData::ID_STATIC_TEXT = wxNewId();
const long EnterData::ID_TEXT_CTRL = wxNewId();
const long EnterData::ID_BUILD_BUTTON = wxNewId();
const long EnterData::ID_CANCEL_BUTTON = wxNewId();

BEGIN_EVENT_TABLE (EnterData,wxDialog)
	//(*EventTable(EnterData)
	//*)
END_EVENT_TABLE()

EnterData::EnterData (wxWindow* parent, wxVector<int> *inputData, wxString *err)
{
	wxButton *buildButton, *cancelButton;
	wxPanel *enterDataPanel,*buttonsPanel,*buttonFillerPanel;
	wxStaticText *dataLabelStaticText;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	this->SetSizeHints (wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* EnterDataFlexGridSizer1;
	EnterDataFlexGridSizer1 = new wxFlexGridSizer (2, 1, 0, 0);
	EnterDataFlexGridSizer1->AddGrowableRow (0);
	EnterDataFlexGridSizer1->SetFlexibleDirection (wxBOTH);
	EnterDataFlexGridSizer1->SetFlexibleDirection (wxBOTH);
	EnterDataFlexGridSizer1->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	enterDataPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* EnterDataFlexGridSizer2;
	EnterDataFlexGridSizer2 = new wxFlexGridSizer (2, 1, 0, 0);
	EnterDataFlexGridSizer2->AddGrowableRow (1);
	EnterDataFlexGridSizer2->SetFlexibleDirection (wxBOTH);
	EnterDataFlexGridSizer2->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	dataLabelStaticText = new wxStaticText (enterDataPanel, wxID_ANY, wxT ("Enter data separated by whitespace."), wxDefaultPosition, wxDefaultSize, 0);
	//dataLabelStaticText->Wrap( 0 );
	EnterDataFlexGridSizer2->Add (dataLabelStaticText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	dataTextCtrl = new wxTextCtrl (enterDataPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (500,400), wxTE_MULTILINE|wxHSCROLL);
	EnterDataFlexGridSizer2->Add (dataTextCtrl, 1, wxALL, 5);

	enterDataPanel->SetSizer (EnterDataFlexGridSizer2);
	enterDataPanel->Layout();
	EnterDataFlexGridSizer2->Fit (enterDataPanel);
	EnterDataFlexGridSizer1->Add (enterDataPanel, 1, wxALL, 5);

	buttonsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* EnterDataFlexGridSizer3;
	EnterDataFlexGridSizer3 = new wxFlexGridSizer (0, 3, 0, 0);
	EnterDataFlexGridSizer3->AddGrowableCol (0);
	EnterDataFlexGridSizer3->AddGrowableRow (0);
	EnterDataFlexGridSizer3->SetFlexibleDirection (wxBOTH);
	EnterDataFlexGridSizer3->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	buttonFillerPanel = new wxPanel (buttonsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	EnterDataFlexGridSizer3->Add (buttonFillerPanel, 1, wxALL, 5);

	buildButton = new wxButton (buttonsPanel, ID_BUILD_BUTTON, wxT ("Build"), wxDefaultPosition, wxDefaultSize, 0);
	buildButton->SetDefault();
	EnterDataFlexGridSizer3->Add (buildButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	cancelButton = new wxButton (buttonsPanel, ID_CANCEL_BUTTON, wxT ("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	EnterDataFlexGridSizer3->Add (cancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	buttonsPanel->SetSizer (EnterDataFlexGridSizer3);
	buttonsPanel->Layout();
	EnterDataFlexGridSizer3->Fit (buttonsPanel);
	EnterDataFlexGridSizer1->Add (buttonsPanel, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

	this->SetSizer (EnterDataFlexGridSizer1);
	this->Layout();
	EnterDataFlexGridSizer1->Fit (this);

	Connect (ID_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterData::OnenterBuildTreeButtonClick);
	Connect (ID_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterData::OnenterDataCancelButtonClick);

	inputData->clear();
	data = inputData;
	error = err;
	*error = wxT ("");

	this->Centre (wxBOTH);
}

EnterData::~EnterData()
{
}

void EnterData::OnenterBuildTreeButtonClick (wxCommandEvent& event)
{
	wxStringTokenizer in (dataTextCtrl->GetValue());
	wxString s;
	long value;

	while (in.HasMoreTokens())
	{
		s = in.GetNextToken();

		if (!s.ToLong (&value))
		{
			error->Printf (wxT ("%s is not an integer."), s.c_str());
			break;
		}

		data->push_back (value);
	}

	if (error->length() == 0)
		EndModal (wxID_OK);
	else
	{
		data->clear();
		EndModal (wxID_CANCEL);
	}
}

void EnterData::OnenterDataCancelButtonClick (wxCommandEvent& event)
{
	data->clear();
	EndModal (wxID_CANCEL);
}
