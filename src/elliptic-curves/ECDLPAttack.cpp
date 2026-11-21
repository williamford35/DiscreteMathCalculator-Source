#include "elliptic-curves/ECDLPAttack.h"

///////////////////////////////////////////////////////////////////////////

ECDLPAttack::ECDLPAttack( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* ECDLPAttackBoxSizer;
	ECDLPAttackBoxSizer = new wxBoxSizer( wxVERTICAL );

	ECDLPAttackStaticBitMap = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("ECDLPAttacker.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	ECDLPAttackBoxSizer->Add( ECDLPAttackStaticBitMap, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	MonitorLineButton = new wxButton( this, wxID_ANY, wxT("Monitor Line"), wxDefaultPosition, wxDefaultSize, 0 );
	ECDLPAttackBoxSizer->Add( MonitorLineButton, 0, wxALL, 5 );

	CapturedTextLabelStaticText = new wxStaticText( this, wxID_ANY, wxT("Captured Data"), wxDefaultPosition, wxDefaultSize, 0 );
	CapturedTextLabelStaticText->Wrap( -1 );
	CapturedTextLabelStaticText->SetFont( wxFont( 14, 70, 90, 90, false, wxEmptyString ) );

	ECDLPAttackBoxSizer->Add( CapturedTextLabelStaticText, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	AliceCapturedDataPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* AliceCapturedDataBoxSizer;
	AliceCapturedDataBoxSizer = new wxBoxSizer( wxHORIZONTAL );

	AliceCapturedDataStaticText = new wxStaticText( AliceCapturedDataPanel, wxID_ANY, wxT("from Alice"), wxDefaultPosition, wxDefaultSize, 0 );
	AliceCapturedDataStaticText->Wrap( -1 );
	AliceCapturedDataBoxSizer->Add( AliceCapturedDataStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	AliceCapturedDataTextCtrl = new wxTextCtrl( AliceCapturedDataPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 450,75 ), 0|wxHSCROLL|wxVSCROLL );
	AliceCapturedDataBoxSizer->Add( AliceCapturedDataTextCtrl, 0, wxALL, 5 );


	AliceCapturedDataPanel->SetSizer( AliceCapturedDataBoxSizer );
	AliceCapturedDataPanel->Layout();
	AliceCapturedDataBoxSizer->Fit( AliceCapturedDataPanel );
	ECDLPAttackBoxSizer->Add( AliceCapturedDataPanel, 1, wxEXPAND | wxALL, 5 );

	BobCapturedDataPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* BobCaputuredDataBoxSizer;
	BobCaputuredDataBoxSizer = new wxBoxSizer( wxHORIZONTAL );

	BobCapturedDataStaticText = new wxStaticText( BobCapturedDataPanel, wxID_ANY, wxT("from Bob"), wxDefaultPosition, wxDefaultSize, 0 );
	BobCapturedDataStaticText->Wrap( -1 );
	BobCaputuredDataBoxSizer->Add( BobCapturedDataStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	BobCapturedDataTextCtrl = new wxTextCtrl( BobCapturedDataPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 450,75 ), 0|wxHSCROLL|wxVSCROLL );
	BobCaputuredDataBoxSizer->Add( BobCapturedDataTextCtrl, 0, wxALL, 5 );


	BobCapturedDataPanel->SetSizer( BobCaputuredDataBoxSizer );
	BobCapturedDataPanel->Layout();
	BobCaputuredDataBoxSizer->Fit( BobCapturedDataPanel );
	ECDLPAttackBoxSizer->Add( BobCapturedDataPanel, 1, wxEXPAND | wxALL, 5 );

	MessagesCapturedStaticText = new wxStaticText( this, wxID_ANY, wxT("Messages Captured"), wxDefaultPosition, wxDefaultSize, 0 );
	MessagesCapturedStaticText->Wrap( -1 );
	MessagesCapturedStaticText->SetFont( wxFont( 14, 70, 90, 90, false, wxEmptyString ) );

	ECDLPAttackBoxSizer->Add( MessagesCapturedStaticText, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	AliceToBobCapturedMessagesPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* AliceToBobCapturedMessagesPanelBoxSizer;
	AliceToBobCapturedMessagesPanelBoxSizer = new wxBoxSizer( wxHORIZONTAL );

	AliceToBobCapturedMessagesStaticText = new wxStaticText( AliceToBobCapturedMessagesPanel, wxID_ANY, wxT("AliceToBob"), wxDefaultPosition, wxDefaultSize, 0 );
	AliceToBobCapturedMessagesStaticText->Wrap( -1 );
	AliceToBobCapturedMessagesPanelBoxSizer->Add( AliceToBobCapturedMessagesStaticText, 0, wxALL, 5 );

	AliceToBobCapturedMessagesTextCtrl = new wxTextCtrl( AliceToBobCapturedMessagesPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 450,-1 ), 0|wxHSCROLL );
	AliceToBobCapturedMessagesPanelBoxSizer->Add( AliceToBobCapturedMessagesTextCtrl, 0, wxALL, 5 );


	AliceToBobCapturedMessagesPanel->SetSizer( AliceToBobCapturedMessagesPanelBoxSizer );
	AliceToBobCapturedMessagesPanel->Layout();
	AliceToBobCapturedMessagesPanelBoxSizer->Fit( AliceToBobCapturedMessagesPanel );
	ECDLPAttackBoxSizer->Add( AliceToBobCapturedMessagesPanel, 1, wxEXPAND | wxALL, 5 );

	BobToAliceCapturedMessagePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* BobToAliceCapturedMessagesPanelBoxSizer;
	BobToAliceCapturedMessagesPanelBoxSizer = new wxBoxSizer( wxHORIZONTAL );

	BobToAliceCapturedMessagesStaticText = new wxStaticText( BobToAliceCapturedMessagePanel, wxID_ANY, wxT("BobToAlice"), wxDefaultPosition, wxDefaultSize, 0 );
	BobToAliceCapturedMessagesStaticText->Wrap( -1 );
	BobToAliceCapturedMessagesPanelBoxSizer->Add( BobToAliceCapturedMessagesStaticText, 0, wxALL, 5 );

	BobToAliceCapturedMessagesTextCtrl = new wxTextCtrl( BobToAliceCapturedMessagePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxHSCROLL );
	BobToAliceCapturedMessagesTextCtrl->SetMinSize( wxSize( 450,-1 ) );

	BobToAliceCapturedMessagesPanelBoxSizer->Add( BobToAliceCapturedMessagesTextCtrl, 0, wxALL, 5 );


	BobToAliceCapturedMessagePanel->SetSizer( BobToAliceCapturedMessagesPanelBoxSizer );
	BobToAliceCapturedMessagePanel->Layout();
	BobToAliceCapturedMessagesPanelBoxSizer->Fit( BobToAliceCapturedMessagePanel );
	ECDLPAttackBoxSizer->Add( BobToAliceCapturedMessagePanel, 1, wxEXPAND | wxALL, 5 );

	ECDLPAttackCloseButton = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	ECDLPAttackBoxSizer->Add( ECDLPAttackCloseButton, 0, wxALL|wxALIGN_RIGHT, 5 );


	this->SetSizer( ECDLPAttackBoxSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	MonitorLineButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ECDLPAttack::OnECDLPAttackMonitorLineButtonPress ), NULL, this );
	ECDLPAttackCloseButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ECDLPAttack::OnCloseECDLPAttackButtonPress ), NULL, this );
}

ECDLPAttack::~ECDLPAttack()
{
	// Disconnect Events
	MonitorLineButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ECDLPAttack::OnECDLPAttackMonitorLineButtonPress ), NULL, this );
	ECDLPAttackCloseButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ECDLPAttack::OnCloseECDLPAttackButtonPress ), NULL, this );

}

void ECDLPAttack::OnECDLPAttackMonitorLineButtonPress( wxCommandEvent& event )
{

}

void ECDLPAttack::OnCloseECDLPAttackButtonPress( wxCommandEvent& event )
{

}

