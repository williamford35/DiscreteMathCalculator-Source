/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "counting-enumeration/ResultMagnitude2.h"
#include "common/ImageFiles.h"

//(*InternalHeaders(ResultMagnitude2)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(ResultMagnitude2)
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_STATICBITMAP = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_N_STATICTEXT = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_N_TEXTCTRL = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_N_PANEL = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_K_STATICTEXT = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_K_TEXTCTRL = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE_K_PANEL = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_CLOSE_BUTTON = wxNewId();
const long ResultMagnitude2::ID_RESULT_MAGNITUDE2_CLOSE_BUTTON_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (ResultMagnitude2,wxDialog)
	//(*EventTable(ResultMagnitude2)
	//*)
END_EVENT_TABLE()

ResultMagnitude2::ResultMagnitude2 (wxWindow* parent,int p,wxWindowID id,const wxPoint& pos,const wxSize& size) :
	page (p)
{
	//(*Initialize(ResultMagnitude2)
	wxBoxSizer* resultMagnitude2ResultsBoxSizer;
	wxBoxSizer* resultMagnitude2NBoxSizer;
	wxFlexGridSizer* resultMagnitude2FlexGridSizer;
	wxBoxSizer* resultMagnitude2KBoxSizer;
	wxBoxSizer* resultMagnitude2CloseBoxSizer;
	wxString Npart, Kpart;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("wxID_ANY"));
	resultMagnitude2FlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	resultMagnitude2FlexGridSizer->AddGrowableCol (0);
	resultMagnitude2FlexGridSizer->AddGrowableRow (0);
	resultMagnitude2FlexGridSizer->AddGrowableRow (1);
	resultMagnitude2ResultsBoxSizer = new wxBoxSizer (wxVERTICAL);
	resultMagnitude2StaticBitmap = new wxStaticBitmap (this, ID_RESULT_MAGNITUDE2_STATICBITMAP, wxBitmap (wxImage (imagefile ("approxeq.png"))), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_RESULT_MAGNITUDE2_STATICBITMAP"));
	resultMagnitude2ResultsBoxSizer->Add (resultMagnitude2StaticBitmap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	resultMagnitude2NPanel = new wxPanel (this, ID_RESULT_MAGNITUDE2_N_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_RESULT_MAGNITUDE2_N_PANEL"));
	resultMagnitude2NBoxSizer = new wxBoxSizer (wxHORIZONTAL);

	if (p == 3)
	{
		Npart = wxT ("Number of subsets");
		Kpart = wxT ("Number of k-element subsets");
	}
	else
	{
		Npart = wxT ("B(n)");
		Kpart = wxT ("S(n,k)");
	}

	resultMagnitude2NStaticText = new wxStaticText (resultMagnitude2NPanel, ID_RESULT_MAGNITUDE2_N_STATICTEXT, Npart, wxDefaultPosition, wxDefaultSize, 0, _T ("ID_RESULT_MAGNITUDE2_N_STATICTEXT"));
	resultMagnitude2NBoxSizer->Add (resultMagnitude2NStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	resultMagnitude2NTextCtrl = new wxTextCtrl (resultMagnitude2NPanel, ID_RESULT_MAGNITUDE2_N_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RESULT_MAGNITUDE2_N_TEXTCTRL"));
	resultMagnitude2NBoxSizer->Add (resultMagnitude2NTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	resultMagnitude2NPanel->SetSizer (resultMagnitude2NBoxSizer);
	resultMagnitude2NBoxSizer->Fit (resultMagnitude2NPanel);
	resultMagnitude2NBoxSizer->SetSizeHints (resultMagnitude2NPanel);
	resultMagnitude2ResultsBoxSizer->Add (resultMagnitude2NPanel, 1, wxALL|wxEXPAND, 1);
	resultMagnitude2KPanel = new wxPanel (this, ID_RESULT_MAGNITUDE_K_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_RESULT_MAGNITUDE_K_PANEL"));
	resultMagnitude2KBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	resultMagnitude2KStaticText = new wxStaticText (resultMagnitude2KPanel, ID_RESULT_MAGNITUDE2_K_STATICTEXT, Kpart, wxDefaultPosition, wxDefaultSize, 0, _T ("ID_RESULT_MAGNITUDE2_K_STATICTEXT"));
	resultMagnitude2KBoxSizer->Add (resultMagnitude2KStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	resultMagnitude2KTextCtrl = new wxTextCtrl (resultMagnitude2KPanel, ID_RESULT_MAGNITUDE2_K_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RESULT_MAGNITUDE2_K_TEXTCTRL"));
	resultMagnitude2KBoxSizer->Add (resultMagnitude2KTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	resultMagnitude2KPanel->SetSizer (resultMagnitude2KBoxSizer);
	resultMagnitude2KBoxSizer->Fit (resultMagnitude2KPanel);
	resultMagnitude2KBoxSizer->SetSizeHints (resultMagnitude2KPanel);
	resultMagnitude2ResultsBoxSizer->Add (resultMagnitude2KPanel, 1, wxALL|wxEXPAND, 1);
	resultMagnitude2FlexGridSizer->Add (resultMagnitude2ResultsBoxSizer, 1, wxALL|wxEXPAND, 1);
	resultMagnitude2CloseButtonPanel = new wxPanel (this, ID_RESULT_MAGNITUDE2_CLOSE_BUTTON_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_RESULT_MAGNITUDE2_CLOSE_BUTTON_PANEL"));
	resultMagnitude2CloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	resultMagnitude2CloseBoxSizer->Add (0,0,1, wxALL|wxEXPAND, 5);
	resultMagnitude2CloseBoxSizer->Add (0,0,1, wxALL|wxEXPAND, 5);
	resultMagnitude2CloseButton = new wxButton (resultMagnitude2CloseButtonPanel, ID_RESULT_MAGNITUDE2_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_RESULT_MAGNITUDE2_CLOSE_BUTTON"));
	resultMagnitude2CloseBoxSizer->Add (resultMagnitude2CloseButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	resultMagnitude2CloseButtonPanel->SetSizer (resultMagnitude2CloseBoxSizer);
	resultMagnitude2CloseBoxSizer->Fit (resultMagnitude2CloseButtonPanel);
	resultMagnitude2CloseBoxSizer->SetSizeHints (resultMagnitude2CloseButtonPanel);
	resultMagnitude2FlexGridSizer->Add (resultMagnitude2CloseButtonPanel, 1, wxALL|wxEXPAND, 1);
	SetSizer (resultMagnitude2FlexGridSizer);
	resultMagnitude2FlexGridSizer->Fit (this);
	resultMagnitude2FlexGridSizer->SetSizeHints (this);
	//*)
	Connect (ID_RESULT_MAGNITUDE2_CLOSE_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &ResultMagnitude2::OnresultMagnitude2CloseButtonClick);
}

ResultMagnitude2::~ResultMagnitude2()
{
	//(*Destroy(ResultMagnitude2)
	//*)
}


void ResultMagnitude2::OnresultMagnitude2CloseButtonClick (wxCommandEvent& event)
{
	Close();
}

void ResultMagnitude2::writeApproximations (const wxString& nvalue,
      const wxString& kvalue)
{
	if (nvalue.length() != 0)
		resultMagnitude2NTextCtrl->SetValue (nvalue);

	if (kvalue.length() != 0)
		resultMagnitude2KTextCtrl->SetValue (kvalue);
}
