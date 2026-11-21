/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/isomorphism/IsomorphismOutput.h"
#include "common/ImageFiles.h"

#include "wx/clipbrd.h"

IsomorphismOutput::IsomorphismOutput (wxWindow* parent, int* lab1, wxString *v1, int* lab2, wxString *v2, long n, const char* imageFileName, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), label1 (lab1), label2 (lab2), numVertices(n), vg1(v1), vg2(v2)
{
	SetClientSize (wxSize (500,500));

	wxFlexGridSizer* IsomorphismFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	IsomorphismFlexGridSizer->SetFlexibleDirection (wxBOTH);
	IsomorphismFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);
	IsomorphismFlexGridSizer->AddGrowableCol (0);
	//IsomorphismFlexGridSizer->AddGrowableRow (0);
	IsomorphismFlexGridSizer->AddGrowableRow (1);
	this->SetSizer (IsomorphismFlexGridSizer);

	IsomorphismStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (imageFileName))), wxDefaultPosition, wxSize (-1, -1), 0);

	IsomorphismFlexGridSizer->Add (IsomorphismStaticBitmap, 1, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL, 1);

	IsomorphismListCtrl = new wxListCtrl (this, wxID_ANY, wxDefaultPosition, wxSize (-1,250), wxLC_EDIT_LABELS|wxLC_REPORT|wxVSCROLL);
	fillListCtrl ();

	IsomorphismFlexGridSizer->Add (IsomorphismListCtrl, 5, wxALL|wxEXPAND, 1);

	IsomorphismButtonPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	IsomorphismFlexGridSizer->Add (IsomorphismButtonPanel, 1, wxALL|wxALIGN_RIGHT, 5);

	wxBoxSizer* IsomorphismButtons = new wxBoxSizer (wxHORIZONTAL);
	IsomorphismButtonPanel->SetSizer (IsomorphismButtons);

	IsomorphismCopyButton = new wxButton (IsomorphismButtonPanel, wxID_ANY, _ ("Copy"), wxDefaultPosition, wxSize (-1,-1), 0);

	IsomorphismButtons->Add (IsomorphismCopyButton, 0, wxALL, 5);

	IsomorphismCloseButton = new wxButton (IsomorphismButtonPanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);
	IsomorphismCloseButton->SetDefault();

	IsomorphismButtons->Add (IsomorphismCloseButton, 0, wxALL, 5);

	SetName (wxT ("IsomorphismOutput"));

	if (GetSizer())
	{
		GetSizer()->Fit (this);
	}

	CentreOnParent (wxBOTH);
#if wxVERSION_NUMBER >= 2900

	if (!wxPersistenceManager::Get().Find (this))
	{
		wxPersistenceManager::Get().RegisterAndRestore (this);
	}
	else
	{
		wxPersistenceManager::Get().Restore (this);
	}

#endif
	// Connect events
	IsomorphismCopyButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismOutput::OnCopyButtonPress), NULL, this);
	IsomorphismCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismOutput::OnCloseButtonPress), NULL, this);
}

IsomorphismOutput::~IsomorphismOutput()
{
	IsomorphismCopyButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismOutput::OnCopyButtonPress), NULL, this);
	IsomorphismCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (IsomorphismOutput::OnCloseButtonPress), NULL, this);
}

void IsomorphismOutput::OnCopyButtonPress (wxCommandEvent& event)
{
	wxString buffer;

	for (int i=0; i < numVertices; i++)
	{
		buffer += vg1[label1[i]];
		buffer += wxT(" --> ");
		buffer += vg2[label2[i]];
		buffer += wxT("\n");
	}

	if (wxTheClipboard->Open())
	{
		// This data objects are held by the clipboard,
		// so do not delete them in the app.
		wxTheClipboard->SetData (new wxTextDataObject (buffer));
		wxTheClipboard->Close();
	}
}

void IsomorphismOutput::OnCloseButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_OK);
}

void IsomorphismOutput::fillListCtrl ()
{
	// Add the two columns
	wxListItem col0, col1;

	col0.SetId (0);
	col0.SetText (_ ("Graph 1"));
	col0.SetAlign (wxLIST_FORMAT_CENTRE);
	col0.SetImage (-1);
	col0.SetWidth (250);
	IsomorphismListCtrl->InsertColumn (0, col0);

	col1.SetId (1);
	col1.SetText (_ ("Graph 2"));
	col1.SetAlign (wxLIST_FORMAT_CENTRE);
	col1.SetImage (-1);
	col1.SetWidth (250);
	IsomorphismListCtrl->InsertColumn (1, col1);

	for (int i=0; i < numVertices; i++)
	{

		InsertItemInReportView(i);
	}
}

void IsomorphismOutput::InsertItemInReportView (int i)
{
	wxString buf;

	buf = vg1[label1[i]];
	long tmp = IsomorphismListCtrl->InsertItem (i, buf, 0);
	IsomorphismListCtrl->SetItemData (tmp, i);

	buf = vg1[label1[i]];
	IsomorphismListCtrl->SetItem (tmp, 0, buf);

	buf = vg2[label2[i]];
	IsomorphismListCtrl->SetItem (tmp, 1, buf);
}
