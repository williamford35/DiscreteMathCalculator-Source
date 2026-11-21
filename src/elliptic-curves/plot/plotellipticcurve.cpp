/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "elliptic-curves/plot/plotellipticcurve.h"
#include "elliptic-curves/plot/NativeEllipticPlot.h"
#include "elliptic-curves/ecurvelib.h"
#include "common/report.h"
#include "common/ImageFiles.h"
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
#include <wx/utils.h>

#ifdef _WIN64
#include <windows.h>
#include <tchar.h>
#endif

void constructEllipticCurve (char *eq, double a, double b)
{
	char buf[50];
	int aIsNeg = 0, bIsNeg = 0;

	if (a < 0)
		aIsNeg = 1;
	if (b < 0)
		bIsNeg = 1;

	sprintf(eq, "y^2 = x^3 ");
	if (aIsNeg)
	{
		a = -a;
		if (a == 1)
			strcpy(buf, "- x ");
		else
			sprintf(buf, "- %gx ", a);
		strcat(eq, buf);
	}
	else
	{
		if (a == 0)
			sprintf(buf, " ");
		else if (a == 1)
			strcpy(buf, "+ x ");
		else
			sprintf(buf, "+ %gx ", a);
		strcat(eq, buf);
	}
	if (bIsNeg)
	{
		b = -b;
		sprintf(buf, "- %g", b);
		strcat(eq, buf);
	}
	else
	{
		if (b == 0)
			sprintf(buf, " ", b);
		else
			sprintf(buf, "+ %g ", b);
		strcat(eq, buf);
	}
}

PlotEllipticCurve::PlotEllipticCurve (wxWindow* parent,  const char *image1,  const char *image2, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style)
{
	wxBoxSizer* PlotEllipticCurveBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (PlotEllipticCurveBoxSizer);
	EllipticCurvePlotsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PlotEllipticCurveBoxSizer->Add (EllipticCurvePlotsPanel, 0, wxALL, 1);
	wxBoxSizer* EllipticCurvePlotsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurvePlotsPanel->SetSizer (EllipticCurvePlotsBoxSizer);
	EllipticCurvePlot1StaticBitmap = new wxStaticBitmap (EllipticCurvePlotsPanel,
	      wxID_ANY, wxBitmap (wxImage (imagefile ("elliptic-cubic-1.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
	EllipticCurvePlotsBoxSizer->Add (EllipticCurvePlot1StaticBitmap, 0, wxALL, 1);
	EllipticCurvePlot2StaticBitmap = new wxStaticBitmap (EllipticCurvePlotsPanel,
	      wxID_ANY, wxBitmap (wxImage (imagefile ("elliptic-cubic-2.png") ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
	EllipticCurvePlotsBoxSizer->Add (EllipticCurvePlot2StaticBitmap, 0, wxALL, 1);
	EllipticCurvePlotsLabelStaticText =
	   new wxStaticText (this, wxID_ANY, _ ("Elliptic Curves y^2 = x^3 - 4x +5 and y^2 = x^3 - x"), wxDefaultPosition, wxSize (-1,-1), 0);
	PlotEllipticCurveBoxSizer->Add (EllipticCurvePlotsLabelStaticText, 0, wxALL|wxALIGN_CENTER, 1);
	EllipticCurvePlotCoefficientsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PlotEllipticCurveBoxSizer->Add (EllipticCurvePlotCoefficientsPanel, 0, wxALL|wxEXPAND, 1);
	wxBoxSizer* EllipticCurvePlotCoefficientsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurvePlotCoefficientsPanel->SetSizer (EllipticCurvePlotCoefficientsBoxSizer);
	EllipticCurvePlotEquationStaticText = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("y^2 = x^3 + ax + b"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurvePlotEquationStaticText, 0, wxALL|wxALIGN_CENTER, 10);
	EllipticCurveXLabelCoefficientStaticText = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("A ="), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveXLabelCoefficientStaticText, 0, wxALL|wxALIGN_CENTER, 10);
	EllipticCurveXCoefficientTextCtrl = new wxTextCtrl (EllipticCurvePlotCoefficientsPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveXCoefficientTextCtrl->SetHint (wxT ("") );
#endif
	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveXCoefficientTextCtrl, 0, wxALL|wxALIGN_CENTER, 10);
	EllipticCurveConstantLabelCoefficientStaticText = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("B ="), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveConstantLabelCoefficientStaticText, 0, wxALL|wxALIGN_CENTER, 10);
	EllipticCurveConstantCoefficientTextCtrl = new wxTextCtrl (EllipticCurvePlotCoefficientsPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveConstantCoefficientTextCtrl->SetHint (wxT ("") );
#endif
	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveConstantCoefficientTextCtrl, 0, wxALL|wxALIGN_CENTER, 10);
	EllipticCurveXYPlotRangesPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PlotEllipticCurveBoxSizer->Add (EllipticCurveXYPlotRangesPanel, 0, wxALL|wxEXPAND, 1);
	wxBoxSizer* EllipticCurveXYPlotRangesBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurveXYPlotRangesPanel->SetSizer (EllipticCurveXYPlotRangesBoxSizer);
	EllipticCurveXRangeLabelStaticText = new wxStaticText (EllipticCurveXYPlotRangesPanel, wxID_ANY, _ ("x range:"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveXRangeLabelStaticText, 0, wxALL, 10);
	EllipticCurveXRangeLowTextCtrl = new wxTextCtrl (EllipticCurveXYPlotRangesPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveXRangeLowTextCtrl->SetHint (wxT ("") );
#endif
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveXRangeLowTextCtrl, 0, wxALL, 10);
	EllipticCurveXRangeToStaticText = new wxStaticText (EllipticCurveXYPlotRangesPanel, wxID_ANY, _ ("to"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveXRangeToStaticText, 0, wxALL, 10);
	EllipticCurveXRangeHighTextCtrl = new wxTextCtrl (EllipticCurveXYPlotRangesPanel, wxID_ANY,
	      wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveXRangeHighTextCtrl->SetHint (wxT ("") );
#endif
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveXRangeHighTextCtrl, 0, wxALL, 10);
	EllipticCurveYRangeLabelStaticText = new wxStaticText (EllipticCurveXYPlotRangesPanel, wxID_ANY, _ ("y range:"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveYRangeLabelStaticText, 0, wxALL, 10);
	EllipticCurveYRangeLowTextCtrl = new wxTextCtrl (EllipticCurveXYPlotRangesPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveYRangeLowTextCtrl->SetHint (wxT ("") );
#endif
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveYRangeLowTextCtrl, 0, wxALL, 10);
	EllipticCurveYRangeToStaticText = new wxStaticText (EllipticCurveXYPlotRangesPanel, wxID_ANY, _ ("to"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveYRangeToStaticText, 0, wxALL, 10);
	EllipticCurveYRangeHighTextCtrl = new wxTextCtrl (EllipticCurveXYPlotRangesPanel, wxID_ANY,
	      wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveYRangeHighTextCtrl->SetHint (wxT ("") );
#endif
	EllipticCurveXYPlotRangesBoxSizer->Add (EllipticCurveYRangeHighTextCtrl, 0, wxALL, 10);
	EllipticCurvePlotActionPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PlotEllipticCurveBoxSizer->Add (EllipticCurvePlotActionPanel, 0, wxALL|wxEXPAND, 5);
	wxBoxSizer* EllipticCurvePlotActionBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurvePlotActionPanel->SetSizer (EllipticCurvePlotActionBoxSizer);
	EllipticCurvePlotActionBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
	EllipticCurvePlotActionButton = new wxButton (EllipticCurvePlotActionPanel, wxID_ANY, _ ("Plot"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurvePlotActionBoxSizer->Add (EllipticCurvePlotActionButton, 0, wxALL, 10);
	EllipticCurveClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PlotEllipticCurveBoxSizer->Add (EllipticCurveClosePanel, 0, wxALL|wxEXPAND, 1);
	wxBoxSizer* EllipticCurveCancelCloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurveClosePanel->SetSizer (EllipticCurveCancelCloseBoxSizer);
	EllipticCurveCancelCloseBoxSizer->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
	EllipticCurveCancelButton = new wxButton (EllipticCurveClosePanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurveCancelCloseBoxSizer->Add (EllipticCurveCancelButton, 0, wxALL, 5);
	EllipticCurveCloseButton = new wxButton (EllipticCurveClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);
	EllipticCurveCancelCloseBoxSizer->Add (EllipticCurveCloseButton, 0, wxALL, 5);
	SetName (wxT ("PlotEllipticCurve") );
	SetSize (488,420);

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
	EllipticCurvePlotActionButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PlotEllipticCurve::OnEllipticCurvePlotButtonPress), NULL, this);
	EllipticCurveCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PlotEllipticCurve::OnEllipticCurveCancelButtonPress), NULL, this);
	EllipticCurveCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PlotEllipticCurve::OnEllipticCurveCloseButtonPress), NULL, this);
}

PlotEllipticCurve::~PlotEllipticCurve()
{
	EllipticCurvePlotActionButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PlotEllipticCurve::OnEllipticCurvePlotButtonPress), NULL, this);
	EllipticCurveCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PlotEllipticCurve::OnEllipticCurveCancelButtonPress), NULL, this);
	EllipticCurveCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PlotEllipticCurve::OnEllipticCurveCloseButtonPress), NULL, this);
}

void PlotEllipticCurve::OnEllipticCurvePlotButtonPress(
    wxCommandEvent& event)
{
    double a, b, xl, xh, yl, yh;

    double* values[] = {
        &a, &b, &xl, &xh, &yl, &yh
    };

    wxTextCtrl* controls[] = {
        EllipticCurveXCoefficientTextCtrl,
        EllipticCurveConstantCoefficientTextCtrl,
        EllipticCurveXRangeLowTextCtrl,
        EllipticCurveXRangeHighTextCtrl,
        EllipticCurveYRangeLowTextCtrl,
        EllipticCurveYRangeHighTextCtrl
    };

    wxString messages[] = {
        "Invalid coefficient A.",
        "Invalid coefficient B.",
        "Invalid minimum X coordinate.",
        "Invalid maximum X coordinate.",
        "Invalid minimum Y coordinate.",
        "Invalid maximum Y coordinate."
    };

    // Read and validate all six values.
    for (int i = 0; i < 6; ++i)
    {
        if (!GetFloatValue(
                controls[i], values[i], messages[i]))
            return;
    }

    // Validate plotting ranges.
    if (!(xl < xh) || !(yl < yh))
    {
        report(
            "Plotting ranges must have minimum < maximum.",
            "Invalid Plot Range");
        return;
    }

	// plot elliptic cubic curve and close the modal dialog
	 NativeEllipticPlot* plot =
		new NativeEllipticPlot(
			GetParent(), a, b, xl, xh, yl, yh);

	plot->Show();

	// Close the modal input dialog so the plot is accessible.
	EndModal(wxID_OK);

	// Bring the plot window to the front.
	plot->Raise();
	plot->SetFocus();
}

void PlotEllipticCurve::OnEllipticCurveCancelButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void PlotEllipticCurve::OnEllipticCurveCloseButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_OK);
}

bool PlotEllipticCurve::GetFloatValue (wxTextCtrl *t, double *v, const wxString& msg)
{
	wxString vstr = t->GetValue();

	if ( (vstr.Length() ==0) || (vstr.ToDouble (v) == false) )
	{
		report (msg, wxT ("Note") );
		return false;
	}

	return true;
}
