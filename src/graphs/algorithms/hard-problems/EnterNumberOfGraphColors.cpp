/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/hard-problems/EnterNumberOfGraphColors.h"
#include "common/ImageFiles.h"

//(*InternalHeaders(EnterNumberOfGraphColors)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

#include <wx/msgdlg.h>

//(*IdInit(EnterNumberOfGraphColors)
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_STATICBITMAP = wxNewId();
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_NUMBER_OF_COLORS_STATICTEXT = wxNewId();
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_NUMBER_OF_COLORS_TEXTCTRL = wxNewId();
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_ENTER_NUMBER_OF_COLORS_PANEL = wxNewId();
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_COLOR_BUTTON = wxNewId();
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_CANCEL_BUTTON = wxNewId();
const long EnterNumberOfGraphColors::ID_GRAPH_COLORING_CONTROL_PANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE (EnterNumberOfGraphColors,wxDialog)
	//(*EventTable(EnterNumberOfGraphColors)
	//*)
END_EVENT_TABLE()

EnterNumberOfGraphColors::EnterNumberOfGraphColors (wxWindow* parent,int *ncolors,wxWindowID id,const wxPoint& pos,const wxSize& size) :
	ptrNumColors (ncolors)
{
	//(*Initialize(EnterNumberOfGraphColors)
	wxBoxSizer* GraphColoringControlBoxSizer;
	wxBoxSizer* GraphColoringNumberOfColorsBoxSizer;
	wxFlexGridSizer* GraphColoringFlexGridSizer;

	Create (parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T ("id"));
	SetClientSize (wxSize (340,250));
	Move (wxDefaultPosition);
	GraphColoringFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	GraphColoringStaticBitmap = new wxStaticBitmap (this, ID_GRAPH_COLORING_STATICBITMAP, wxBitmap (wxImage (imagefile ("graph-coloring.png"))), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_GRAPH_COLORING_STATICBITMAP"));
	GraphColoringFlexGridSizer->Add (GraphColoringStaticBitmap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GraphColoringEnterNumberOfColorsPanel = new wxPanel (this, ID_GRAPH_COLORING_ENTER_NUMBER_OF_COLORS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_GRAPH_COLORING_ENTER_NUMBER_OF_COLORS_PANEL"));
	GraphColoringNumberOfColorsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GraphColoringNumberOfColorsStaticText = new wxStaticText (GraphColoringEnterNumberOfColorsPanel, ID_GRAPH_COLORING_NUMBER_OF_COLORS_STATICTEXT, _ ("Maximum number of colors"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_GRAPH_COLORING_NUMBER_OF_COLORS_STATICTEXT"));
	GraphColoringNumberOfColorsBoxSizer->Add (GraphColoringNumberOfColorsStaticText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GraphColoringNumberOfColorsTextCtrl = new wxTextCtrl (GraphColoringEnterNumberOfColorsPanel, ID_GRAPH_COLORING_NUMBER_OF_COLORS_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_GRAPH_COLORING_NUMBER_OF_COLORS_TEXTCTRL"));
	GraphColoringNumberOfColorsBoxSizer->Add (GraphColoringNumberOfColorsTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GraphColoringEnterNumberOfColorsPanel->SetSizer (GraphColoringNumberOfColorsBoxSizer);
	GraphColoringNumberOfColorsBoxSizer->Fit (GraphColoringEnterNumberOfColorsPanel);
	GraphColoringNumberOfColorsBoxSizer->SetSizeHints (GraphColoringEnterNumberOfColorsPanel);
	GraphColoringFlexGridSizer->Add (GraphColoringEnterNumberOfColorsPanel, 1, wxALL|wxEXPAND, 5);
	GraphColoringControlPanel = new wxPanel (this, ID_GRAPH_COLORING_CONTROL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_GRAPH_COLORING_CONTROL_PANEL"));
	GraphColoringControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	GraphColoringControlBoxSizer->Add (0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GraphColoringColorButton = new wxButton (GraphColoringControlPanel, ID_GRAPH_COLORING_COLOR_BUTTON, _ ("Color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_GRAPH_COLORING_COLOR_BUTTON"));
	GraphColoringColorButton->SetDefault();
	GraphColoringControlBoxSizer->Add (GraphColoringColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GraphColoringCancelButton = new wxButton (GraphColoringControlPanel, ID_GRAPH_COLORING_CANCEL_BUTTON, _ ("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T ("ID_GRAPH_COLORING_CANCEL_BUTTON"));
	GraphColoringControlBoxSizer->Add (GraphColoringCancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	GraphColoringControlPanel->SetSizer (GraphColoringControlBoxSizer);
	GraphColoringControlBoxSizer->Fit (GraphColoringControlPanel);
	GraphColoringControlBoxSizer->SetSizeHints (GraphColoringControlPanel);
	GraphColoringFlexGridSizer->Add (GraphColoringControlPanel, 1, wxALL|wxEXPAND, 5);
	SetSizer (GraphColoringFlexGridSizer);
	SetSizer (GraphColoringFlexGridSizer);
	Layout();
	//*)
	Connect (ID_GRAPH_COLORING_COLOR_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterNumberOfGraphColors::OnGraphColoringColorButtonClick);
	Connect (ID_GRAPH_COLORING_CANCEL_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &EnterNumberOfGraphColors::OnGraphColoringCancelButtonClick);
}

EnterNumberOfGraphColors::~EnterNumberOfGraphColors()
{
	//(*Destroy(EnterNumberOfGraphColors)
	//*)
}

void EnterNumberOfGraphColors::OnGraphColoringColorButtonClick (wxCommandEvent& event)
{
	wxString numberOfColors;
	long requestedNumberOfColors;

	numberOfColors = GraphColoringNumberOfColorsTextCtrl->GetValue();

	if (numberOfColors.ToLong (&requestedNumberOfColors))
	{
		*ptrNumColors = (int) requestedNumberOfColors;
		EndModal (wxID_OK);
	}
	else
	{
		wxMessageDialog dialog (NULL, wxT ("The entry is not a nonnegative integer."), wxT ("Error"), wxOK | wxICON_ERROR);
		dialog.ShowModal();
	}
}

void EnterNumberOfGraphColors::OnGraphColoringCancelButtonClick (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}
