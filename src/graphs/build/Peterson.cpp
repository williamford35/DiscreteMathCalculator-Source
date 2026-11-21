/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/build/Peterson.h"
#include "common/ImageFiles.h"
#include "common/report.h"

#include <wx/msgdlg.h>

//(*InternalHeaders(Peterson)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(Peterson)
const long Peterson::ID_PETERSON_GRAPH_STATICBITMAP = wxNewId();
const long Peterson::ID_N_STATICTEXT = wxNewId();
const long Peterson::ID_N_TEXTCTRL = wxNewId();
const long Peterson::ID_N_PANEL = wxNewId();
const long Peterson::ID_K_STATICTEXT = wxNewId();
const long Peterson::ID_K_TEXTCTRL = wxNewId();
const long Peterson::ID_K_PANEL = wxNewId();
const long Peterson::ID_PETERSON_BUILD_BUTTON = wxNewId();
const long Peterson::ID_PETERSON_CANCEL_BUTTON = wxNewId();
const long Peterson::ID_CONTR0L_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (Peterson,wxDialog)
	//(*EventTable(Peterson)
	//*)
END_EVENT_TABLE()

Peterson::Peterson (wxWindow* parent, int *myn, int *myk, wxWindowID id,const wxPoint& pos,const wxSize& size) :
	n (myn), k (myk)
{
	//(*Initialize(Peterson)
	wxBoxSizer* kBoxSizer;
	wxBoxSizer* controlBoxSizer;
	wxFlexGridSizer* petersonGraphFlexGridSizer;
	wxBoxSizer* nBoxSizer;

	Create (parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("id"));
	SetClientSize (wxDefaultSize);
	Move (wxDefaultPosition);
	petersonGraphFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	petersonGraph = new wxStaticBitmap (this, ID_PETERSON_GRAPH_STATICBITMAP, wxBitmap (wxImage (imagefile ("peterson.png"))), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_PETERSON_GRAPH_STATICBITMAP"));
	petersonGraphFlexGridSizer->Add (petersonGraph, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	nPanel = new wxPanel (this, ID_N_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_N_PANEL"));
	nBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	nStaticText = new wxStaticText (nPanel, ID_N_STATICTEXT, _ ("n"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T ("ID_N_STATICTEXT"));
	nBoxSizer->Add (nStaticText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
	nTextCtrl = new wxTextCtrl (nPanel, ID_N_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_N_TEXTCTRL"));
	nBoxSizer->Add (nTextCtrl, 10, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	nPanel->SetSizer (nBoxSizer);
	nBoxSizer->Fit (nPanel);
	nBoxSizer->SetSizeHints (nPanel);
	petersonGraphFlexGridSizer->Add (nPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	kPanel = new wxPanel (this, ID_K_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_K_PANEL"));
	kBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	kStaticText = new wxStaticText (kPanel, ID_K_STATICTEXT, _ ("k"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T ("ID_K_STATICTEXT"));
	kBoxSizer->Add (kStaticText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 1);
	kTextCtrl = new wxTextCtrl (kPanel, ID_K_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_K_TEXTCTRL"));
	kBoxSizer->Add (kTextCtrl, 10, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	kPanel->SetSizer (kBoxSizer);
	kBoxSizer->Fit (kPanel);
	kBoxSizer->SetSizeHints (kPanel);
	petersonGraphFlexGridSizer->Add (kPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	controlPanel = new wxPanel (this, ID_CONTR0L_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_CONTR0L_PANEL"));
	controlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	controlBoxSizer->Add (-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	petersonBuildButton = new wxButton (controlPanel, ID_PETERSON_BUILD_BUTTON, _ ("Build"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_PETERSON_BUILD_BUTTON"));
	petersonBuildButton->SetDefault();
	controlBoxSizer->Add (petersonBuildButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	petersonCancelButton = new wxButton (controlPanel, ID_PETERSON_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_PETERSON_CANCEL_BUTTON"));
	controlBoxSizer->Add (petersonCancelButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	controlPanel->SetSizer (controlBoxSizer);
	controlBoxSizer->Fit (controlPanel);
	controlBoxSizer->SetSizeHints (controlPanel);
	petersonGraphFlexGridSizer->Add (controlPanel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
	SetSizer (petersonGraphFlexGridSizer);
	petersonGraphFlexGridSizer->Fit (this);
	petersonGraphFlexGridSizer->SetSizeHints (this);

	Connect (ID_PETERSON_BUILD_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &Peterson::OnpetersonBuildButtonClick);
	Connect (ID_PETERSON_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &Peterson::OnpetersonCancelButtonClick);
	//*)
}

Peterson::~Peterson()
{
	//(*Destroy(Peterson)
	//*)
}


void Peterson::OnpetersonBuildButtonClick (wxCommandEvent& event)
{
	wxString nStr,kStr;
	long longn, longk;

	nStr = nTextCtrl->GetValue();

	if (!nStr.ToLong (&longn) || nStr.length() == 0)
	{
		report (wxT ("n is not a valid number."), "Note");
		return;
	}

	kStr = kTextCtrl->GetValue();
	kStr.ToLong (&longk);

	if (!nStr.ToLong (&longn) || kStr.length() == 0)
	{
		report (wxT ("k is not a valid number."),"Note");
		return;
	}

	if (longk >= longn/2)
	{
		report (wxT ("The value of k must be < n/2."),"Note");
		return;
	}

	*n = (int) longn;
	*k = (int) longk;

	EndModal (wxID_OK);
}

void Peterson::OnpetersonCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
