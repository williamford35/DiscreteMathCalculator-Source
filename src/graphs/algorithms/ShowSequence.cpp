/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/ShowSequence.h"
#include "common/ImageFiles.h"

//(*InternalHeaders(ShowSequence)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
#include <wx/clipbrd.h>
#include <wx/dataobj.h>

//(*IdInit(ShowSequence)
const long ShowSequence::ID_SHOW_SEQUENCE_STATICBITMAP = wxNewId();
const long ShowSequence::ID_SHOW_SEQUENCE_STATICTEXT = wxNewId();
const long ShowSequence::ID_SHOW_SEQUENCE_TEXTCTRL = wxNewId();
const long ShowSequence::ID_SHOW_SEQUENCE_COPY_BUTTON = wxNewId();
const long ShowSequence::ID_SHOW_SEQUENCE_CLOSE_BUTTON = wxNewId();
const long ShowSequence::ID_SHOW_SEQUENCE_CONTROL_PANEL = wxNewId();
const long ShowSequence::ID_SHOW_SEQUENCE_MAIN_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (ShowSequence,wxFrame)
	//(*EventTable(ShowSequence)
	//*)
END_EVENT_TABLE()

ShowSequence::ShowSequence (wxWindow* parent,const char *imageFileName,SequenceType seqtype,wxWindowID id,const wxPoint& pos,const wxSize& size, long style)
{
	//(*Initialize(ShowSequence)
	wxBoxSizer* ShowSequenceDisplayBoxSizer;
	wxBoxSizer* ShowSequenceDisplayControlBoxSizer;

	Create (parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T ("wxID_ANY"));
	SetClientSize (wxSize (700,400));
	ShowSequenceDisplayMainPanel = new wxPanel (this, ID_SHOW_SEQUENCE_MAIN_PANEL, wxPoint (136,232), wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_SHOW_SEQUENCE_MAIN_PANEL"));
	ShowSequenceDisplayBoxSizer = new wxBoxSizer (wxVERTICAL);

	if (seqtype != GENERALINFORMATION)
	{
		ShowSequenceDisplayStaticBitmap = new wxStaticBitmap (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICBITMAP, wxBitmap (wxImage (imagefile (imageFileName))), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_SHOW_SEQUENCE_STATICBITMAP"));
		ShowSequenceDisplayBoxSizer->Add (ShowSequenceDisplayStaticBitmap, 4, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	}

	switch (seqtype)
	{
	case BFS:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("BFS"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case DFS:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("DFS"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case TSP:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("Optimal Path"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case TOPOLOGICALSORT:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("Topological Sort"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case CARDMATCHING:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("Maximum Independent Edge Set"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case WEIGHTEDMATCHING:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("Maximum Weighted Independent Edge Set"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case PERMUTATIONS:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ ("Permutations in Lexicographic Order"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;

	case GENERALINFORMATION:
		ShowSequenceDisplayStaticText = new wxStaticText (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_STATICTEXT, _ (""), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_SHOW_SEQUENCE_STATICTEXT"));
		break;
	}

	wxFont ShowSequenceDisplayStaticTextFont (16,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T ("Lucida Bright"),wxFONTENCODING_DEFAULT);
	ShowSequenceDisplayStaticText->SetFont (ShowSequenceDisplayStaticTextFont);
	ShowSequenceDisplayBoxSizer->Add (ShowSequenceDisplayStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	ShowSequenceDisplayTextCtrl = new wxTextCtrl (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxHSCROLL, wxDefaultValidator, _T ("ID_SHOW_SEQUENCE_TEXTCTRL"));
	ShowSequenceDisplayBoxSizer->Add (ShowSequenceDisplayTextCtrl, 6, wxALL|wxEXPAND, 1);
	ShowSequenceDisplayControlPanel = new wxPanel (ShowSequenceDisplayMainPanel, ID_SHOW_SEQUENCE_CONTROL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_SHOW_SEQUENCE_CONTROL_PANEL"));
	ShowSequenceDisplayControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ShowSequenceDisplayControlBoxSizer->Add (0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	ShowSequenceDisplayCopyButton = new wxButton (ShowSequenceDisplayControlPanel, ID_SHOW_SEQUENCE_COPY_BUTTON, _ ("Copy"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_SHOW_SEQUENCE_COPY_BUTTON"));
	ShowSequenceDisplayControlBoxSizer->Add (ShowSequenceDisplayCopyButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	ShowSequenceDisplayCloseButton = new wxButton (ShowSequenceDisplayControlPanel, ID_SHOW_SEQUENCE_CLOSE_BUTTON, _ ("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_SHOW_SEQUENCE_CLOSE_BUTTON"));
	ShowSequenceDisplayControlBoxSizer->Add (ShowSequenceDisplayCloseButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	ShowSequenceDisplayControlPanel->SetSizer (ShowSequenceDisplayControlBoxSizer);
	ShowSequenceDisplayControlBoxSizer->Fit (ShowSequenceDisplayControlPanel);
	ShowSequenceDisplayControlBoxSizer->SetSizeHints (ShowSequenceDisplayControlPanel);
	ShowSequenceDisplayBoxSizer->Add (ShowSequenceDisplayControlPanel, 1, wxALL|wxEXPAND, 1);
	ShowSequenceDisplayMainPanel->SetSizer (ShowSequenceDisplayBoxSizer);
	ShowSequenceDisplayBoxSizer->Fit (ShowSequenceDisplayMainPanel);
	ShowSequenceDisplayBoxSizer->SetSizeHints (ShowSequenceDisplayMainPanel);
	//*)
	Connect (ID_SHOW_SEQUENCE_COPY_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &ShowSequence::OnShowSequenceCopyButtonClick);
	Connect (ID_SHOW_SEQUENCE_CLOSE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &ShowSequence::OnShowSequenceCloseButtonClick);
}

ShowSequence::~ShowSequence()
{}

void ShowSequence::WriteLine (const wxString& s)
{
	ShowSequenceDisplayTextCtrl->WriteText (s);
}

void ShowSequence::OnShowSequenceCopyButtonClick (wxCommandEvent& event)
{
	if (wxTheClipboard->Open())
	{
		// These data objects are held by the clipboard,
		// so do not delete them in the app.
		wxTheClipboard->SetData (new wxTextDataObject (ShowSequenceDisplayTextCtrl->GetValue()));
		wxTheClipboard->Close();
	}
}

void ShowSequence::OnShowSequenceCloseButtonClick (wxCommandEvent& event)
{
	Destroy();
}
