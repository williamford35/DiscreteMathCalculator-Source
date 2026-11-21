/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/tokenzr.h>
#include <algorithm>
#include <functional>
#include <wx/msgdlg.h>

#include "graphs/algorithms/InputIntegerData.h"

InputIntegerData::InputIntegerData( wxWindow* parent, vector<int>* dataVals, bool forPartialOrderDivisibilityInput, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) :
	wxDialog( parent, id, title, pos, size, style ), integerData(dataVals)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* InputIntegerDataBoxSizer;
	InputIntegerDataBoxSizer = new wxBoxSizer( wxVERTICAL );

	if (forPartialOrderDivisibilityInput)
		InputIntegerDataStaticText = new wxStaticText( this, wxID_ANY, wxT("Integer Divisibility Data"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	else
		InputIntegerDataStaticText = new wxStaticText( this, wxID_ANY, wxT("Graphical Sequence Integer Data"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );

	InputIntegerDataStaticText->Wrap( -1 );
	InputIntegerDataBoxSizer->Add( InputIntegerDataStaticText, 0, wxALL|wxEXPAND, 1 );

	InputIntegerDataTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE );
	InputIntegerDataBoxSizer->Add( InputIntegerDataTextCtrl, 2, wxALL|wxEXPAND, 1 );

	InputIntegerDataButtonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* InputIntegerDataButtonsPanelBoxSizer;
	InputIntegerDataButtonsPanelBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	InputIntegerDataButtonsPanelBoxSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	InputIntegerDataCancelButton = new wxButton( InputIntegerDataButtonsPanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	InputIntegerDataButtonsPanelBoxSizer->Add( InputIntegerDataCancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );

	if (forPartialOrderDivisibilityInput)
		InputIntegerDataDisplayButton = new wxButton( InputIntegerDataButtonsPanel, wxID_ANY, wxT("Display"), wxDefaultPosition, wxDefaultSize, 0 );
	else
		InputIntegerDataDisplayButton = new wxButton( InputIntegerDataButtonsPanel, wxID_ANY, wxT("Test"), wxDefaultPosition, wxDefaultSize, 0 );

	InputIntegerDataButtonsPanelBoxSizer->Add( InputIntegerDataDisplayButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	InputIntegerDataButtonsPanel->SetSizer( InputIntegerDataButtonsPanelBoxSizer );
	InputIntegerDataButtonsPanel->Layout();
	InputIntegerDataButtonsPanelBoxSizer->Fit( InputIntegerDataButtonsPanel );
	InputIntegerDataBoxSizer->Add( InputIntegerDataButtonsPanel, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( InputIntegerDataBoxSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	InputIntegerDataCancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputIntegerData::OnCancelButtonPress ), NULL, this );
	InputIntegerDataDisplayButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputIntegerData::OnDisplayButtonPress ), NULL, this );
}

void InputIntegerData::EnableRepeatedGraphicalSequenceTests()
{
    repeatGraphicalSequenceTests = true;
    InputIntegerDataTextCtrl->SetValue(wxT("3 3 2 2 1 1"));
    SetTitle(wxT("Graphical Sequence Test"));
}

InputIntegerData::~InputIntegerData()
{
	// Disconnect Events
	InputIntegerDataCancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputIntegerData::OnCancelButtonPress ), NULL, this );
	InputIntegerDataDisplayButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputIntegerData::OnDisplayButtonPress ), NULL, this );
}

void InputIntegerData::OnCancelButtonPress( wxCommandEvent& event )
{
	EndModal (wxID_CANCEL);
}

void InputIntegerData::OnDisplayButtonPress(
    wxCommandEvent& event)
{
    wxStringTokenizer in(InputIntegerDataTextCtrl->GetValue());
    wxString token;
    long value;

    integerData->clear();

    while (in.HasMoreTokens())
    {
        token = in.GetNextToken();

        if (!token.ToLong(&value))
        {
            wxMessageBox(
                token + wxT(" is not an integer."),
                wxT("Input Error"),
                wxOK | wxICON_ERROR,
                this
            );

            integerData->clear();
            return;
        }

        integerData->push_back(static_cast<int>(value));
    }

    if (repeatGraphicalSequenceTests)
    {
        if (integerData->empty())
        {
            wxMessageBox(
                wxT("Enter a sequence of integers."),
                wxT("Graphical Sequence"),
                wxOK | wxICON_INFORMATION,
                this
            );
            return;
        }

        std::vector<int> values = *integerData;

        // A degree sequence is graphical if and only if
        // the Havel-Hakimi reduction reaches all zeros.
        const int n = static_cast<int>(values.size());

        bool graphical = true;
        long long sum = 0;

        for (int degree : values)
        {
            if (degree < 0 || degree >= n)
                graphical = false;

            sum += degree;
        }

        if (sum % 2 != 0)
            graphical = false;

        while (graphical)
        {
            std::sort(values.begin(), values.end(),
                      std::greater<int>());

            if (values.front() == 0)
                break;

            int degree = values.front();
            values.erase(values.begin());

            if (degree > static_cast<int>(values.size()))
            {
                graphical = false;
                break;
            }

            for (int i = 0; i < degree; ++i)
            {
                --values[i];

                if (values[i] < 0)
                    graphical = false;
            }
        }

        wxMessageBox(
            graphical
                ? wxT("This is a graphical sequence.")
                : wxT("This is NOT a graphical sequence."),
            wxT("Graphical Sequence Test"),
            wxOK | wxICON_INFORMATION,
            this
        );

        // Do not close the dialog.
        return;
    }

    EndModal(wxID_OK);
}
