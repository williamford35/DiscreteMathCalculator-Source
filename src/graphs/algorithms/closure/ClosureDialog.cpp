/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "graphs/algorithms/closure/ClosureDialog.h"
#include "common/ImageFiles.h"

///////////////////////////////////////////////////////////////////////////

ClosureDialog::ClosureDialog(wxWindow* parent,
                             const char* imageFileName,
                             ClosureType* ctype,
                             wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style)
	: wxDialog(parent, id, title, pos, size, style)
	, selectedClosureType(ctype)
{
	// this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	SetClientSize(wxSize(325, 350));

	wxFlexGridSizer* ClosureFlexGridSizer;
	ClosureFlexGridSizer = new wxFlexGridSizer(4, 1, 1, 1);
	ClosureFlexGridSizer->AddGrowableCol(0);
	ClosureFlexGridSizer->AddGrowableRow(1);
	ClosureFlexGridSizer->AddGrowableRow(2);
	ClosureFlexGridSizer->SetFlexibleDirection(wxBOTH);
	ClosureFlexGridSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	ClosureStaticBitmap = new wxStaticBitmap(
	   this, wxID_ANY, wxBitmap(wxImage(imagefile(imageFileName))), wxDefaultPosition, wxSize(-1, -1), 0);
	ClosureFlexGridSizer->Add(ClosureStaticBitmap, 1, wxALL | wxALIGN_CENTER, 1);

	ClosureStaticTextPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	wxBoxSizer* ClosureTextBoxSizer;
	ClosureTextBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	ClosureStaticText = new wxStaticText(
	   ClosureStaticTextPanel, wxID_ANY, wxT("Graph Closure"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	ClosureStaticText->Wrap(1);
	ClosureStaticText->SetFont(wxFont(24, 70, 90, 90, false, wxT("Times New Roman")));

	ClosureTextBoxSizer->Add(ClosureStaticText, 1, wxTOP, 5);

	ClosureStaticTextPanel->SetSizer(ClosureTextBoxSizer);
	ClosureStaticTextPanel->Layout();
	ClosureFlexGridSizer->Add(ClosureStaticTextPanel, 0, wxALL | wxEXPAND, 1);

	ClosureRadioBoxPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* ClosureRadioBoxSizer;
	ClosureRadioBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	wxString ClosureRadioBoxChoices[] = { wxT("Reflexive"), wxT("Symmetric"), wxT("Transitive") };
	int ClosureRadioBoxNChoices = sizeof(ClosureRadioBoxChoices) / sizeof(wxString);
	ClosureRadioBox = new wxRadioBox(ClosureRadioBoxPanel,
	                                 wxID_ANY,
	                                 wxT("Type of Closure"),
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 ClosureRadioBoxNChoices,
	                                 ClosureRadioBoxChoices,
	                                 1,
	                                 wxRA_SPECIFY_COLS);
	ClosureRadioBox->SetSelection(2);
	ClosureRadioBoxSizer->Add(ClosureRadioBox, 0, wxALL, 5);

	ClosureRadioBoxPanel->SetSizer(ClosureRadioBoxSizer);
	ClosureRadioBoxPanel->Layout();
	ClosureRadioBoxSizer->Fit(ClosureRadioBoxPanel);
	ClosureFlexGridSizer->Add(ClosureRadioBoxPanel, 0, wxALL | wxEXPAND | wxALIGN_LEFT, 1);

	ClosureButtonPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 50), wxTAB_TRAVERSAL);
	wxBoxSizer* ClosureButtonsBoxSizer;
	ClosureButtonsBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	ClosureButtonsBoxSizer->Add(100, 0, 1, wxEXPAND, 5);

	ClosureProcessButton =
	   new wxButton(ClosureButtonPanel, wxID_ANY, wxT("Process"), wxDefaultPosition, wxDefaultSize, 0);
	ClosureProcessButton->SetDefault();
	ClosureButtonsBoxSizer->Add(ClosureProcessButton, wxALIGN_TOP, wxALIGN_CENTER, 1);
	ClosureButtonsBoxSizer->Add(0, 0, 1, wxEXPAND, 1);

	ClosureCancelButton = new wxButton(ClosureButtonPanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	ClosureButtonsBoxSizer->Add(ClosureCancelButton, 0, wxALIGN_CENTER_VERTICAL, 5);
	ClosureButtonsBoxSizer->Add(10, 0, 1, wxEXPAND, 5);

	ClosureButtonPanel->SetSizer(ClosureButtonsBoxSizer);
	ClosureButtonPanel->Layout();
	ClosureButtonsBoxSizer->Fit(ClosureButtonPanel);
	ClosureFlexGridSizer->Add(ClosureButtonPanel, 0, wxALIGN_CENTER, 1);

	this->SetSizer(ClosureFlexGridSizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	ClosureProcessButton->Connect(
	   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClosureDialog::ClosureProcessButtonPress), NULL, this);
	ClosureCancelButton->Connect(
	   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClosureDialog::ClosureCancelButtonPress), NULL, this);
}

ClosureDialog::~ClosureDialog()
{
	// Disconnect Events
	ClosureProcessButton->Disconnect(
	   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClosureDialog::ClosureProcessButtonPress), NULL, this);
	ClosureCancelButton->Disconnect(
	   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClosureDialog::ClosureCancelButtonPress), NULL, this);
}

void ClosureDialog::ClosureProcessButtonPress(wxCommandEvent& event)
{
	int selection = ClosureRadioBox->GetSelection();

	*selectedClosureType = ClosureType(selection);

	EndModal(wxID_OK);
}

void ClosureDialog::ClosureCancelButtonPress(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}
