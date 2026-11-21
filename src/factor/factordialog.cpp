/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "factor/factordialog.h"
#include "common/report.h"
#include "common/ImageFiles.h"
#include <wx/process.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <iostream>
using namespace std;

FactorDialog::FactorDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style)
{
	wxFont thisFont(14, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Times New Roman"));
	this->SetFont(thisFont);
	wxBoxSizer* FactorDialogBoxSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(FactorDialogBoxSizer);

	FactorDialogStaticBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap (wxImage (imagefile ("tired-man.png"))), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0 );
	FactorDialogBoxSizer->Add(FactorDialogStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP(0));
	IntegerToFactorPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
	FactorDialogBoxSizer->Add(IntegerToFactorPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
	wxFlexGridSizer* IntegerToFactorFlexGridSizer = new wxFlexGridSizer(1, 2, 0, 0);
	IntegerToFactorFlexGridSizer->SetFlexibleDirection( wxBOTH );
	IntegerToFactorFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	IntegerToFactorFlexGridSizer->AddGrowableCol(1);
	IntegerToFactorPanel->SetSizer(IntegerToFactorFlexGridSizer);

	IntegerToFactorStaticTextCtrl = new wxStaticText(IntegerToFactorPanel, wxID_ANY, _("Integer to Factor"),
	      wxDefaultPosition, wxDLG_UNIT(IntegerToFactorPanel, wxSize(-1,-1)), 0);
	wxFont IntegerToFactorStaticTextCtrlFont(14, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Times New Roman"));
	IntegerToFactorStaticTextCtrl->SetFont(IntegerToFactorStaticTextCtrlFont);

	IntegerToFactorFlexGridSizer->Add(IntegerToFactorStaticTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP(5));

	IntegerToFactorTextCtrl = new wxTextCtrl(
	    IntegerToFactorPanel,
	    wxID_ANY,
	    wxT(""),
	    wxDefaultPosition,
	    wxDefaultSize,
	    wxTE_PROCESS_ENTER);

	wxFont IntegerToFactorTextCtrlFont(
	    14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
	    wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
	IntegerToFactorTextCtrl->SetFont(IntegerToFactorTextCtrlFont);

	IntegerToFactorFlexGridSizer->Add(
	    IntegerToFactorTextCtrl, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

	FactorResultsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);

	FactorDialogBoxSizer->Add(FactorResultsPanel, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
	wxFlexGridSizer* FactorsFlexGridSizer = new wxFlexGridSizer(1, 2, 0, 0);
	FactorsFlexGridSizer->SetFlexibleDirection( wxBOTH );
	FactorsFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	FactorsFlexGridSizer->AddGrowableCol(1);
	// FactorsFlexGridSizer->AddGrowableRow(1);
	FactorResultsPanel->SetSizer(FactorsFlexGridSizer);

	FactorsLabelStaticText = new wxStaticText(FactorResultsPanel, wxID_ANY, _("Factors"), wxDefaultPosition, wxDLG_UNIT(FactorResultsPanel, wxSize(-1,-1)), wxALIGN_CENTRE);
	wxFont FactorsLabelStaticTextFont(14, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Times New Roman"));
	FactorsLabelStaticText->SetFont(FactorsLabelStaticTextFont);

	FactorsFlexGridSizer->Add(
	    FactorsLabelStaticText, 0, wxALL | wxALIGN_CENTER, FromDIP(5));

	FactorResultsTextCtrl = new wxTextCtrl(
	    FactorResultsPanel,
	    wxID_ANY,
	    wxT(""),
	    wxDefaultPosition,
	    wxDefaultSize,
	    wxTE_READONLY);

	wxFont FactorResultsTextCtrlFont(
	    14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
	    wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
	FactorResultsTextCtrl->SetFont(FactorResultsTextCtrlFont);

	FactorsFlexGridSizer->Add(
	    FactorResultsTextCtrl, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

	FactorDialogControlPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);

	FactorDialogBoxSizer->Add(FactorDialogControlPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP(10));

	wxBoxSizer* FactorDialogControlPanelBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	FactorDialogControlPanel->SetSizer(FactorDialogControlPanelBoxSizer);

	FactorDialogControlPanelBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
	FactorDialogControlPanelBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
	FactorDialogControlPanelBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
	FactorDialogControlPanelBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
	FactorDialogControlPanelBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

	FactorDialogFactorButton = new wxButton(FactorDialogControlPanel, wxID_ANY, _("Factor"), wxDefaultPosition, wxDLG_UNIT(FactorDialogControlPanel, wxSize(-1,-1)), 0);
	wxFont FactorDialogFactorButtonFont(14, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Times New Roman"));
	FactorDialogFactorButton->SetFont(FactorDialogFactorButtonFont);

	FactorDialogControlPanelBoxSizer->Add(FactorDialogFactorButton, 0, wxALL, WXC_FROM_DIP(5));

	FactorDialogCloseButton = new wxButton(FactorDialogControlPanel, wxID_ANY, _("Close"), wxDefaultPosition, wxDLG_UNIT(FactorDialogControlPanel, wxSize(-1,-1)), 0);

	FactorDialogControlPanelBoxSizer->Add(FactorDialogCloseButton, 0, wxALL, WXC_FROM_DIP(5));

	SetName(wxT("FactorDialog"));

	SetMinSize(FromDIP(wxSize(700, 450)));
	SetSize(FromDIP(wxSize(700, 450)));

	Layout();

	if(GetParent())
	{
		CentreOnParent(wxBOTH);
	}
	else
	{
		CentreOnScreen(wxBOTH);
	}

#if 0

	if(!wxPersistenceManager::Get().Find(this))
	{
		wxPersistenceManager::Get().RegisterAndRestore(this);
	}
	else
	{
		wxPersistenceManager::Get().Restore(this);
	}

#endif
	// Connect events
	FactorDialogFactorButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FactorDialog::OnFactorDialogFactorButtonPressed), NULL, this);
	FactorDialogCloseButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FactorDialog::OnFactorDialogCloseButtonPressed), NULL, this);
}

FactorDialog::~FactorDialog()
{
	FactorDialogFactorButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FactorDialog::OnFactorDialogFactorButtonPressed), NULL, this);
	FactorDialogCloseButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FactorDialog::OnFactorDialogCloseButtonPressed), NULL, this);
}

void FactorDialog::OnFactorDialogCloseButtonPressed(wxCommandEvent& event)
{
	if ( wxProcess::Exists(wxExecuteReturnValue))
	{
		wxProcess::Kill(wxExecuteReturnValue,wxSIGNONE);
		EndModal (wxID_OK);
	}
	else
		EndModal(wxID_CANCEL);
}
void FactorDialog::OnFactorDialogFactorButtonPressed(wxCommandEvent& event)
{
	wxString factorProcess, factorResults = wxT(""), numberToBeFactored;
	wxArrayString factorProcessStdout, factorProcessStderr;
	size_t indx;
	size_t factorCount;

	numberToBeFactored = IntegerToFactorTextCtrl->GetValue();

	if (numberToBeFactored.Len() == 0)
	{
		report(wxT(" No  integer to be factored is entered."), wxT(""));
		return;
	}

	for (int i=0; i <numberToBeFactored.Len(); i++)
		if (!isdigit(numberToBeFactored[i]))
		{
			report(wxT("Incorrect integer format."), wxT("Note"));
			return;
		}

	#ifdef __APPLE__

    wxString resourceDirectory =
        wxStandardPaths::Get().GetResourcesDir();

    wxFileName qsExecutable(resourceDirectory, wxEmptyString);
    qsExecutable.SetFullName("qs");

    factorProcess =
        wxT("\"") + qsExecutable.GetFullPath() + wxT("\" ");

#else

    factorProcess = wxT("../../usr/resource/qs ");

#endif

	wxBeginBusyCursor();
#ifdef _WIN64
	wxExecuteReturnValue = wxExecute (factorProcess + wxT("  ") + wxT("-s ") + numberToBeFactored, factorProcessStdout,
	                                  factorProcessStderr,
	                                  wxEXEC_SYNC|wxEXEC_NODISABLE);
#else
	/*wxExecuteReturnValue = wxExecute (factorProcess, factorProcessStdout,
	                                  factorProcessStderr,
	                                  wxEXEC_SYNC|wxEXEC_NOEVENTS |wxEXEC_HIDE_CONSOLE );*/
	wxExecuteReturnValue = wxExecute (factorProcess + wxT("  ") + wxT("-s ") + numberToBeFactored, factorProcessStdout,
	                                  factorProcessStderr,
	                                  wxEXEC_SYNC|wxEXEC_NODISABLE);
#endif
	wxEndBusyCursor();

	if (wxExecuteReturnValue == -1)
	{
		reportError (wxT ("Internal error: wxExecute did not run"), wxT ("Error") );
		return;
	}

	factorCount = factorProcessStdout.GetCount();

	if (factorCount == 2)
		indx = 1;
	else
	{
		reportError(wxT("factorCount is incorrect."), wxT("Error"));
		return;
	}

	factorResults = factorProcessStdout[indx];
	FactorResultsTextCtrl->SetValue(factorResults);
}
