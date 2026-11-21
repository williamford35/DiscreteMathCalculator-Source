/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/digraphicalsequence.h"
#include "common/ImageFiles.h"
#include "common/report.h"

DigraphicalSequence::DigraphicalSequence (wxWindow* parent, const char *imageFileName, vector<int> *x, vector<int> *y, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), a (x), b (y)
{
	wxBoxSizer* DigraphicalRealizationBoxSizer = new wxBoxSizer (wxVERTICAL);

	this->SetSizer (DigraphicalRealizationBoxSizer);
	DigraphicalRealizationStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (imageFileName) ) ), wxDefaultPosition, wxSize (-1,-1), 0);
	DigraphicalRealizationBoxSizer->Add (DigraphicalRealizationStaticBitmap, 0, wxALL|wxALIGN_CENTER, 5);
	DigraphicalRealization_n_Panel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	DigraphicalRealizationBoxSizer->Add (DigraphicalRealization_n_Panel, 0, wxALL, 5);

	wxBoxSizer* DigraphicalRealization_n_PanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DigraphicalRealization_n_Panel->SetSizer (DigraphicalRealization_n_PanelBoxSizer);
	DigraphicalRealization_n_StaticText = new wxStaticText (DigraphicalRealization_n_Panel, wxID_ANY, _ ("n ="), wxDefaultPosition, wxSize (-1,-1), 0);
	DigraphicalRealization_n_PanelBoxSizer->Add (DigraphicalRealization_n_StaticText, 0, wxALL|wxALIGN_LEFT, 5);
	DigraphicalRealization_n_TextCtrl = new wxTextCtrl (DigraphicalRealization_n_Panel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxSize (25,-1), 0);
#if wxVERSION_NUMBER >= 3000
	DigraphicalRealization_n_TextCtrl->SetHint (wxT ("") );
#endif
	DigraphicalRealization_n_PanelBoxSizer->Add (DigraphicalRealization_n_TextCtrl, 1, wxALL, 1);
	DigraphicalRealizationInDegreePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	DigraphicalRealizationBoxSizer->Add (DigraphicalRealizationInDegreePanel, 0, wxALL, 5);

	wxBoxSizer* DigraphicalRealizationInDegreeBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DigraphicalRealizationInDegreePanel->SetSizer (DigraphicalRealizationInDegreeBoxSizer);
	DigraphicalRealizationInDegreeStaticText = new wxStaticText (DigraphicalRealizationInDegreePanel, wxID_ANY, _ ("in-degrees"), wxDefaultPosition, wxSize (-1,-1), 0);
	DigraphicalRealizationInDegreeBoxSizer->Add (DigraphicalRealizationInDegreeStaticText, 1, wxALL|wxALIGN_LEFT, 1);
	DigraphicalRealizationInDegreeTextCtrl = new wxTextCtrl (DigraphicalRealizationInDegreePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	DigraphicalRealizationInDegreeTextCtrl->SetHint (wxT ("") );
#endif
	DigraphicalRealizationInDegreeBoxSizer->Add (DigraphicalRealizationInDegreeTextCtrl, 3, wxALL, 1);
	DigraphicalRealizationOutDegreePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	DigraphicalRealizationBoxSizer->Add (DigraphicalRealizationOutDegreePanel, 0, wxALL, 5);

	wxBoxSizer* DigraphicalRealizationOutDegreeBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DigraphicalRealizationOutDegreePanel->SetSizer (DigraphicalRealizationOutDegreeBoxSizer);
	DigraphicalRealizationOutDegreetaticText = new wxStaticText (DigraphicalRealizationOutDegreePanel, wxID_ANY, _ ("out-degrees"), wxDefaultPosition, wxSize (-1,-1), 0);
	DigraphicalRealizationOutDegreeBoxSizer->Add (DigraphicalRealizationOutDegreetaticText, 1, wxALL|wxALIGN_LEFT, 1);
	DigraphicalRealizationOutDegreeTextCtrl = new wxTextCtrl (DigraphicalRealizationOutDegreePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	DigraphicalRealizationOutDegreeTextCtrl->SetHint (wxT ("") );
#endif

	DigraphicalRealizationOutDegreeBoxSizer->Add (DigraphicalRealizationOutDegreeTextCtrl, 3, wxALL, 1);
	DigraphicalRealizationButtonPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	DigraphicalRealizationBoxSizer->Add (DigraphicalRealizationButtonPanel, 0, wxALL, 5);

	wxBoxSizer* DigraphicalRealizationButtonBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	DigraphicalRealizationButtonPanel->SetSizer (DigraphicalRealizationButtonBoxSizer);
	DigraphicalRealizationButtonBoxSizer->Add (200, -1, 1, wxALL|wxALIGN_BOTTOM, 1);
	DigraphicalRealizationCancelButton = new wxButton (DigraphicalRealizationButtonPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);
	DigraphicalRealizationButtonBoxSizer->Add (DigraphicalRealizationCancelButton, 0, wxALL, 5);
	DigraphicalRealizationTestButton = new wxButton (DigraphicalRealizationButtonPanel, wxID_ANY, _ ("Test"), wxDefaultPosition, wxSize (-1,-1), 0);
	DigraphicalRealizationButtonBoxSizer->Add (DigraphicalRealizationTestButton, 0, wxALL, 5);

	// SetSizeHints(400,375);
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
	DigraphicalRealizationCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigraphicalSequence::OnDigraphicalRealizationCancelButtonPress), NULL, this);
	DigraphicalRealizationTestButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigraphicalSequence::OnDigraphicalRealizationTestButtonPress), NULL, this);
}

DigraphicalSequence::~DigraphicalSequence()
{
	DigraphicalRealizationCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigraphicalSequence::OnDigraphicalRealizationCancelButtonPress), NULL, this);
	DigraphicalRealizationTestButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DigraphicalSequence::OnDigraphicalRealizationTestButtonPress), NULL, this);
}

void DigraphicalSequence::OnDigraphicalRealizationCancelButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void DigraphicalSequence::OnDigraphicalRealizationTestButtonPress (wxCommandEvent& event)
{
	long n;
	wxStringTokenizer in (DigraphicalRealizationInDegreeTextCtrl->GetValue()),
	                  out (DigraphicalRealizationOutDegreeTextCtrl->GetValue());
	wxString s_in, s_out, error;
	long in_value, out_value;

	if (!(DigraphicalRealization_n_TextCtrl->GetValue()).ToLong(&n) && (n >= 0))
	{
		::report(wxT("n must be a nonnegative integer."),"Note");
		return;
	}

	while (in.HasMoreTokens() && out.HasMoreTokens())
	{
		s_in = in.GetNextToken();
		s_out = out.GetNextToken();

		// assume nobody will enter a negative  in- out out-degree
		if (!s_in.ToLong (&in_value))
		{
			::report(wxT("All in-degree values must be nonnegative integers."),"Note");
			return;
		}
		if (!s_out.ToLong (&out_value))
		{
			::report(wxT("All out-degree values must be nonnegative integers."),"Note");
			return;
		}
		a->push_back ((int)in_value);
		b->push_back ((int)out_value);
	}

	if (in.HasMoreTokens() || out.HasMoreTokens())
	{
		::report(wxT("Each list must have n values."),"Note");
		return;
	}

	EndModal (wxID_OK);
}
