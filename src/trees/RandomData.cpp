/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "trees/RandomData.h"
#include "common/report.h"

#include <string.h>
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////
RandomData::RandomData (wxWindow* parent, wxVector<int> *inputData, bool isForTree, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog (parent, id, title, pos, size, style),
	randomData (inputData)
{
	wxWindow::SetClientSize (-1,-1);

	wxFlexGridSizer* randomDataFlexGridSizer;
	randomDataFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	randomDataFlexGridSizer->AddGrowableCol (0);
	randomDataFlexGridSizer->AddGrowableRow (1);
	randomDataFlexGridSizer->SetFlexibleDirection (wxBOTH);

	ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* randomDataFlexGridSizer2;
	randomDataFlexGridSizer2 = new wxFlexGridSizer (1, 3, 0, 0);
	randomDataFlexGridSizer2->AddGrowableCol (2);
	randomDataFlexGridSizer2->SetFlexibleDirection (wxBOTH);

	ID_RANDOM_DATA_NUMBER_OF_NODES_STATICTEXT = new wxStaticText (ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL, wxID_ANY, wxT ("Number of Nodes"), wxDefaultPosition, wxSize (125, 20), 0);
	randomDataFlexGridSizer2->Add (ID_RANDOM_DATA_NUMBER_OF_NODES_STATICTEXT, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	ID_RANDOM_DATA_NUMBER_OF_NODES_TEXTCTRL = new wxTextCtrl (ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL,
	      wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(35,-1), 0);
	randomDataFlexGridSizer2->Add (ID_RANDOM_DATA_NUMBER_OF_NODES_TEXTCTRL, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL->SetSizer (randomDataFlexGridSizer2);
	ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL->Layout();
	randomDataFlexGridSizer2->Fit (ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL);
	randomDataFlexGridSizer->Add (ID_RANDOM_DATA_NUMBER_OF_NODES_PANEL, 1, wxALL|wxEXPAND, 1);

	ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL = new wxTextCtrl (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL);
	ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL->SetFont (wxFont (12, 74, 90, 90, false, wxT ("Courier New")));

	randomDataFlexGridSizer->Add (ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL, 1, wxALL|wxEXPAND, 1);

	ID_RANDOM_DATA_BUTTONS_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxFlexGridSizer* randomDataFlexGridSizer3;
	randomDataFlexGridSizer3 = new wxFlexGridSizer (1, 4, 0, 0);
	randomDataFlexGridSizer3->AddGrowableCol (0);
	randomDataFlexGridSizer3->SetFlexibleDirection (wxBOTH);
	randomDataFlexGridSizer3->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	ID_RANDOM_DATA_GENERATE_DATA_BUTTON = new wxButton (ID_RANDOM_DATA_BUTTONS_PANEL, wxID_ANY, wxT ("Generate Random Data"), wxDefaultPosition, wxDefaultSize, 0);
	ID_RANDOM_DATA_GENERATE_DATA_BUTTON->SetDefault();
	randomDataFlexGridSizer3->Add (ID_RANDOM_DATA_GENERATE_DATA_BUTTON, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

	if (isForTree)
		ID_RANDOM_DATA_BUILD_BUTTON = new wxButton (ID_RANDOM_DATA_BUTTONS_PANEL, wxID_ANY,
		      wxT ("Build Tree"), wxDefaultPosition, wxDefaultSize, 0);
	else
		ID_RANDOM_DATA_BUILD_BUTTON = new wxButton (ID_RANDOM_DATA_BUTTONS_PANEL, wxID_ANY,
		      wxT ("Build Heap"), wxDefaultPosition, wxDefaultSize, 0);
	ID_RANDOM_DATA_BUILD_BUTTON->SetDefault();
	randomDataFlexGridSizer3->Add (ID_RANDOM_DATA_BUILD_BUTTON, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_RANDOM_DATA_CANCEL_BUTTON = new wxButton (ID_RANDOM_DATA_BUTTONS_PANEL, wxID_ANY, wxT ("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	ID_RANDOM_DATA_CANCEL_BUTTON->SetDefault();
	randomDataFlexGridSizer3->Add (ID_RANDOM_DATA_CANCEL_BUTTON, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_RANDOM_DATA_BUTTONS_PANEL->SetSizer (randomDataFlexGridSizer3);
	ID_RANDOM_DATA_BUTTONS_PANEL->Layout();
	randomDataFlexGridSizer3->Fit (ID_RANDOM_DATA_BUTTONS_PANEL);
	randomDataFlexGridSizer->Add (ID_RANDOM_DATA_BUTTONS_PANEL, 1, wxALL|wxEXPAND, 1);

	this->SetSizer (randomDataFlexGridSizer);
	this->Layout();

	this->Centre (1);

	// Connect Events
	ID_RANDOM_DATA_GENERATE_DATA_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RandomData::OnRandomDataGenerateClick), NULL, this);
	ID_RANDOM_DATA_BUILD_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RandomData::OnRandomDataBuildClick), NULL, this);
	ID_RANDOM_DATA_CANCEL_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RandomData::OnRandomDataCancelClick), NULL, this);
}

RandomData::~RandomData()
{
	// Disconnect Events
	ID_RANDOM_DATA_GENERATE_DATA_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RandomData::OnRandomDataGenerateClick), NULL, this);
	ID_RANDOM_DATA_BUILD_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RandomData::OnRandomDataBuildClick), NULL, this);
	ID_RANDOM_DATA_CANCEL_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (RandomData::OnRandomDataCancelClick), NULL, this);
}

void RandomData::OnRandomDataGenerateClick (wxCommandEvent& event)
{
	CRandomMersenne rnd;
	wxString strNumberOfNodes;
	int i, randomInt;
	wxString ostr;

	strNumberOfNodes = ID_RANDOM_DATA_NUMBER_OF_NODES_TEXTCTRL->GetValue();

	if (strNumberOfNodes == wxT (""))
	{
		report(wxT ("You have not entered the number of nodes in the tree."),"Note");
		return;
	}

	if (!randomData->empty())
		randomData->clear();

	strNumberOfNodes.ToLong (&numberOfNodes);

	for (i=0; i < numberOfNodes; i++)
	{
		randomInt = rnd.IRandomX (0, 999);
		randomData->push_back (randomInt);
	}

	displayRandomData();
}

void RandomData::OnRandomDataCancelClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void RandomData::OnRandomDataBuildClick (wxCommandEvent& event)
{
	if (randomData->empty())
	{
		report(wxT ("The binary search tree has no nodes."),"Note");
		return;
	}
	else
		EndModal (wxID_OK);
}

void RandomData::displayRandomData()
{
	int count = 1;
	wxString randomValue;

	ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL->ChangeValue (wxT (""));

	for (int i=0; i < numberOfNodes; i++)
	{
		randomValue.Printf (wxT ("%6d"), (*randomData) [i]);
		ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL->AppendText (randomValue);

		if (count == 8)
		{
			ID_RANDOM_DATA_DATA_DISPLAY_TEXTCTRL->AppendText (wxT ("\n"));
			count = 1;
		}
		else
			count++;
	}
}
