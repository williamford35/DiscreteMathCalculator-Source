/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/build/GridGraphInput.h"
#include "common/ImageFiles.h"
#include "common/report.h"

const long GridGraphInputDialog::ID_GRID_GRAPH_STATICBITMAP = wxNewId();

GridGraphInputDialog::GridGraphInputDialog (wxWindow* parent, long *width, long *height, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), gridGraphWidth (width), gridGraphHeight (height)
{
	wxBoxSizer* gridGraphInputBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (gridGraphInputBoxSizer);

	GridGraphImage = new wxStaticBitmap (this, ID_GRID_GRAPH_STATICBITMAP, wxBitmap (wxImage (imagefile ("gridgraph.png") ) ), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T ("ID_GRID_GRAPH_STATICBITMAP") );

	gridGraphInputBoxSizer->Add (GridGraphImage, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	weightValuePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (300,-1) ), wxTAB_TRAVERSAL);

	gridGraphInputBoxSizer->Add (weightValuePanel, 0, wxALL, WXC_FROM_DIP (5) );

	wxBoxSizer* weightValueBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	weightValuePanel->SetSizer (weightValueBoxSizer);

	gridGraphWidthLabel = new wxStaticText (weightValuePanel, wxID_ANY, _ ("Width in vertices"), wxDefaultPosition, wxDLG_UNIT (weightValuePanel, wxSize (-1,-1) ), 0);

	weightValueBoxSizer->Add (gridGraphWidthLabel, 0, wxALL, WXC_FROM_DIP (5) );

	gridGraphWidthTextCtrl = new wxTextCtrl (weightValuePanel, wxID_ANY, wxT (""), wxDefaultPosition,
	      wxDLG_UNIT (weightValuePanel, wxSize (50,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	gridGraphWidthTextCtrl->SetHint (wxT ("") );
#endif

	weightValueBoxSizer->Add (gridGraphWidthTextCtrl, 0, wxALL, WXC_FROM_DIP (5) );

	heightValuePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (300,-1) ), wxTAB_TRAVERSAL);

	gridGraphInputBoxSizer->Add (heightValuePanel, 0, wxALL, WXC_FROM_DIP (5) );

	wxBoxSizer* heightValueBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	heightValuePanel->SetSizer (heightValueBoxSizer);

	gridGraphHeightLabel = new wxStaticText (heightValuePanel, wxID_ANY, _ ("Height in vertices"), wxDefaultPosition, wxDLG_UNIT (heightValuePanel, wxSize (-1,-1) ), 0);

	heightValueBoxSizer->Add (gridGraphHeightLabel, 0, wxALL, WXC_FROM_DIP (5) );

	gridGraphHeightTextCtrl = new wxTextCtrl (heightValuePanel, wxID_ANY, wxT (""), wxDefaultPosition,
	      wxDLG_UNIT (heightValuePanel, wxSize (50,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	gridGraphHeightTextCtrl->SetHint (wxT ("") );
#endif

	heightValueBoxSizer->Add (gridGraphHeightTextCtrl, 0, wxALL, WXC_FROM_DIP (5) );

	controlPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (300,-1) ), wxTAB_TRAVERSAL);

	gridGraphInputBoxSizer->Add (controlPanel, 0, wxALL, WXC_FROM_DIP (5) );

	wxBoxSizer* controlsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	controlPanel->SetSizer (controlsBoxSizer);

	controlsBoxSizer->Add (0, 0, 10, wxALL, WXC_FROM_DIP (1) );

	cancelGridGraphBuild = new wxButton (controlPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxDLG_UNIT (controlPanel, wxSize (-1,-1) ), 0);

	controlsBoxSizer->Add (cancelGridGraphBuild, 1, wxALL, WXC_FROM_DIP (5) );

	gridGraphBuild = new wxButton (controlPanel, wxID_ANY, _ ("Build"), wxDefaultPosition, wxDLG_UNIT (controlPanel, wxSize (-1,-1) ), 0);
	gridGraphBuild->SetDefault();

	controlsBoxSizer->Add (gridGraphBuild, 0, wxALL, WXC_FROM_DIP (5) );

	// SetName(wxT("GridGraphInputDialog"));
	SetSize (300,200);

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	if (GetParent() )
	{
		CentreOnParent (wxBOTH);
	}
	else
	{
		CentreOnScreen (wxBOTH);
	}

#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this) )
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif

	// Connect events
	cancelGridGraphBuild->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GridGraphInputDialog::OnGridGraphCancelButtonPress), NULL, this);
	gridGraphBuild->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GridGraphInputDialog::OnGridGraphBuildButtonPress), NULL, this);

}

GridGraphInputDialog::~GridGraphInputDialog()
{
	cancelGridGraphBuild->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GridGraphInputDialog::OnGridGraphCancelButtonPress), NULL, this);
	gridGraphBuild->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (GridGraphInputDialog::OnGridGraphBuildButtonPress), NULL, this);
}

void GridGraphInputDialog::OnGridGraphCancelButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void GridGraphInputDialog::OnGridGraphBuildButtonPress (wxCommandEvent& event)
{
	wxString widthstr, heightstr;
	long width, height;

	widthstr = gridGraphWidthTextCtrl->GetValue();

	if (widthstr.length() == 0)
	{
		reportError (wxT("There is no value for the width."), wxT("Error"));
		return;
	}

	if (!widthstr.ToLong (&width) )
	{
		reportError (wxT ("The width is not a valid integer."), wxT ("Error") );
		return;
	}

	heightstr = gridGraphHeightTextCtrl->GetValue();

	if (heightstr.length() == 0)
	{
		reportError (wxT("There is no value for the height."), wxT("Error"));
		return;
	}

	if (!heightstr.ToLong (&height) )
	{
		reportError (wxT ("The height is not a valid integer."), wxT ("Error") );
		return;
	}

	*gridGraphWidth = width;
	*gridGraphHeight = height;

	EndModal (wxID_OK);
}
