/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
///////////////////////////////////////////////////////////////////////////

#include "counting-enumeration/ResultMagnitude.h"
#include "common/ImageFiles.h"

///////////////////////////////////////////////////////////////////////////

ResultMagnitude::ResultMagnitude (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog (parent, id, title, pos, size, style)
{
	this->SetSizeHints (wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* resultMagnitudeFlexGridSizer;
	resultMagnitudeFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	resultMagnitudeFlexGridSizer->AddGrowableRow (0);
	resultMagnitudeFlexGridSizer->SetFlexibleDirection (wxBOTH);
	resultMagnitudeFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	ID_RESULT_MAGNITUDE_PANEL = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* resultMagnitudeBoxSizer;
	resultMagnitudeBoxSizer = new wxBoxSizer (wxHORIZONTAL);

	ID_RESULT_MAGNITUDE_STATICBITMAP = new wxStaticBitmap (ID_RESULT_MAGNITUDE_PANEL, wxID_ANY, wxBitmap (imagefile ("approxeq.png"), wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, 0);

	resultMagnitudeBoxSizer->Add (ID_RESULT_MAGNITUDE_STATICBITMAP, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	ID_RESULT_MAGNITUDE_TEXTCTRL = new wxTextCtrl (ID_RESULT_MAGNITUDE_PANEL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	resultMagnitudeBoxSizer->Add (ID_RESULT_MAGNITUDE_TEXTCTRL, 8, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_RESULT_MAGNITUDE_PANEL->SetSizer (resultMagnitudeBoxSizer);
	ID_RESULT_MAGNITUDE_PANEL->Layout();
	resultMagnitudeBoxSizer->Fit (ID_RESULT_MAGNITUDE_PANEL);
	resultMagnitudeFlexGridSizer->Add (ID_RESULT_MAGNITUDE_PANEL, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	ID_CLOSE_BUTTON = new wxButton (this, wxID_ANY, wxT ("Close"), wxDefaultPosition, wxDefaultSize, 0);
	ID_CLOSE_BUTTON->SetDefault();
	resultMagnitudeFlexGridSizer->Add (ID_CLOSE_BUTTON, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

	this->SetSizer (resultMagnitudeFlexGridSizer);
	this->Layout();
	resultMagnitudeFlexGridSizer->Fit (this);

	this->Centre (1);

	// Connect Events
	ID_CLOSE_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ResultMagnitude::OnButtonClick), NULL, this);
}

ResultMagnitude::~ResultMagnitude()
{
	// Disconnect Events
	ID_CLOSE_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ResultMagnitude::OnButtonClick), NULL, this);

}

void ResultMagnitude::writeApproximation (const wxString& approx)
{
	ID_RESULT_MAGNITUDE_TEXTCTRL->ChangeValue (approx);
}
