/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/textfile.h>

#include "elliptic-curves/plot/pointplotellipticcurve.h"
#include "elliptic-curves/plot/NativeEllipticPointPlot.h"
#include "elliptic-curves/ecurvelib.h"
#include "common/report.h"
#include "common/ImageFiles.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <climits>

PointPlotEllipticCurve::PointPlotEllipticCurve (wxWindow* parent,  const char *image, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style)
{
	wxBoxSizer* PointPlotEllipticCurveBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (PointPlotEllipticCurveBoxSizer);

	EllipticCurvePlotsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	PointPlotEllipticCurveBoxSizer->Add (EllipticCurvePlotsPanel, 0, wxALL, WXC_FROM_DIP (1) );

	wxBoxSizer* EllipticCurvePlotsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurvePlotsPanel->SetSizer (EllipticCurvePlotsBoxSizer);

	PointPlotEllipticCurveStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (image) ) ), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0 );

	PointPlotEllipticCurveBoxSizer->Add (PointPlotEllipticCurveStaticBitmap, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	EllipticCurvePlotsLabelStaticText = new wxStaticText (this, wxID_ANY, _ ("y^2 = x^3 -7x + 10 (mod p), p = 127, 487"), wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), 0);

	PointPlotEllipticCurveBoxSizer->Add (EllipticCurvePlotsLabelStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	EllipticCurvePlotCoefficientsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	PointPlotEllipticCurveBoxSizer->Add (EllipticCurvePlotCoefficientsPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* EllipticCurvePlotCoefficientsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurvePlotCoefficientsPanel->SetSizer (EllipticCurvePlotCoefficientsBoxSizer);

	EllipticCurvePlotEquationStaticText = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("y^2 = x^3 + Ax + B (mod p)"), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (-1,-1) ), 0);

	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurvePlotEquationStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	EllipticCurveXLabelCoefficientStaticText = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("A ="), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (-1,-1) ), 0);

	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveXLabelCoefficientStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	EllipticCurveXCoefficientTextCtrl = new wxTextCtrl (EllipticCurvePlotCoefficientsPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (50,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveXCoefficientTextCtrl->SetHint (wxT ("") );
#endif

	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveXCoefficientTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	EllipticCurveConstantLabelCoefficientStaticText = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("B ="), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (-1,-1) ), 0);

	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveConstantLabelCoefficientStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	EllipticCurveConstantCoefficientTextCtrl = new wxTextCtrl (EllipticCurvePlotCoefficientsPanel, wxID_ANY,
	      wxT (""), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (50,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurveConstantCoefficientTextCtrl->SetHint (wxT ("") );
#endif

	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurveConstantCoefficientTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (10) );

	m_staticText77 = new wxStaticText (EllipticCurvePlotCoefficientsPanel, wxID_ANY, _ ("p ="), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (-1,-1) ), 0);

	EllipticCurvePlotCoefficientsBoxSizer->Add (m_staticText77, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	EllipticCurvePointPlotPrime = new wxTextCtrl (EllipticCurvePlotCoefficientsPanel,
	      wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotCoefficientsPanel, wxSize (50,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	EllipticCurvePointPlotPrime->SetHint (wxT ("") );
#endif

	EllipticCurvePlotCoefficientsBoxSizer->Add (EllipticCurvePointPlotPrime, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	EllipticCurvePlotActionPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	PointPlotEllipticCurveBoxSizer->Add (EllipticCurvePlotActionPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (5) );

	wxBoxSizer* EllipticCurvePlotActionBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurvePlotActionPanel->SetSizer (EllipticCurvePlotActionBoxSizer);

	EllipticCurvePlotActionBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );
	EllipticCurvePlotActionBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	EllipticCurvePlotActionBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	EllipticCurvePlotActionButton = new wxButton (EllipticCurvePlotActionPanel, wxID_ANY, _ ("Plot"), wxDefaultPosition, wxDLG_UNIT (EllipticCurvePlotActionPanel, wxSize (-1,-1) ), 0);

	EllipticCurvePlotActionBoxSizer->Add (EllipticCurvePlotActionButton, 0, wxALL, WXC_FROM_DIP (10) );

	EllipticCurveClosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (this, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	PointPlotEllipticCurveBoxSizer->Add (EllipticCurveClosePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* EllipticCurveCancelCloseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	EllipticCurveClosePanel->SetSizer (EllipticCurveCancelCloseBoxSizer);

	EllipticCurveCancelCloseBoxSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );
	EllipticCurveCloseButton = new wxButton (EllipticCurveClosePanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxDLG_UNIT (EllipticCurveClosePanel, wxSize (-1,-1) ), 0);

	EllipticCurveCancelCloseBoxSizer->Add (EllipticCurveCloseButton, 0, wxALL, WXC_FROM_DIP (5) );

	SetName (wxT ("PointPointPlotEllipticCurve") );
	SetSize (725,500);

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
	EllipticCurvePlotActionButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PointPlotEllipticCurve::OnEllipticCurvePlotButtonPress), NULL, this);
	EllipticCurveCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PointPlotEllipticCurve::OnEllipticCurveCloseButtonPress), NULL, this);

}

PointPlotEllipticCurve::~PointPlotEllipticCurve()
{
	EllipticCurvePlotActionButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PointPlotEllipticCurve::OnEllipticCurvePlotButtonPress), NULL, this);
	EllipticCurveCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PointPlotEllipticCurve::OnEllipticCurveCloseButtonPress), NULL, this);
}

void PointPlotEllipticCurve::OnEllipticCurvePlotButtonPress(
    wxCommandEvent& event)
{
    long a, b, prime;

    long* values[] = { &a, &b, &prime };

    wxTextCtrl* controls[] = {
        EllipticCurveXCoefficientTextCtrl,
        EllipticCurveConstantCoefficientTextCtrl,
        EllipticCurvePointPlotPrime
    };

    wxString messages[] = {
        "The coefficient A must be an integer.",
        "The coefficient B must be an integer.",
        "The prime p must be an integer."
    };

    for (int i = 0; i < 3; ++i)
    {
        if (!GetIntValue(controls[i], values[i], messages[i]))
            return;
    }

    if (prime < 2)
    {
        report("p must be at least 2.", "Invalid Prime");
        return;
    }

    if (prime > 1019)
    {
        wxMessageDialog question(
            this,
            "The plot will be very dense. Continue?",
            "Note",
            wxYES_NO | wxCANCEL);

        if (question.ShowModal() != wxID_YES)
            return;
    }

    // Use the application's existing elliptic-curve algorithm.
    std::vector<std::pair<long, long>> pts =
        ecBruteForceOrder(a, b, prime);

    // Display the points using native wxWidgets graphics.
    NativeEllipticPointPlot* plot =
        new NativeEllipticPointPlot(
            GetParent(), a, b, prime, pts);

    plot->Show();

    // Close the modal input dialog.
    EndModal(wxID_OK);

    // Bring the plot window to the foreground.
    plot->Raise();
    plot->SetFocus();
}


void PointPlotEllipticCurve::OnEllipticCurveCloseButtonPress (wxCommandEvent& event)
{
	EndModal (wxID_OK);
}

bool PointPlotEllipticCurve::GetIntValue (wxTextCtrl *t, long *v, const wxString& msg)
{
	wxString vstr = t->GetValue();

	if ( (vstr.Length() ==0) || (vstr.ToLong (v) == false) )
	{
		report (msg, wxT ("Note") );
		return false;
	}

	return true;
}

wxString PointPlotEllipticCurve::GetTemporaryFileName()
{
#ifdef _WIN32
	TCHAR lpTempPathBuffer[MAX_PATH], szTempFileName[MAX_PATH];

	GetTempPath (MAX_PATH, lpTempPathBuffer);
	//  Generates a temporary file name.
	GetTempFileName (lpTempPathBuffer, 		// directory for tmp files
	                 TEXT ("ecpointplot"),    // temp file name prefix
	                 0,                		// create unique name
	                 szTempFileName);  		// buffer for name
	return szTempFileName;
#else
	char tmpFileTemplate[50] = "ecpointplotXXXXXX";
	wxString unicodeFileName;
	int fd;

	//  Generates a temporary file.
	fd = mkstemp (tmpFileTemplate);

	if (fd == -1)
	{
		report (wxT ("Cannot create a required temporary file."), wxT ("Error") );
		return wxString();
	}

	unicodeFileName = wxString::FromUTF8 (tmpFileTemplate);
	return unicodeFileName;
#endif
}

void PointPlotEllipticCurve::writePointData (long p, const wxString& fname,
      const vector<pair<long,long> >& v)
{
	wxTextFile dataFile (fname);
	wxString line;
	double circsize;

	dataFile.Open();

	if ( (p >= 2) && (p <= 50) )
		circsize = 0.80;
	else if ( (p > 50) && (p <= 100) )
		circsize = 0.60;
	else if ( (p > 100) && (p <= 200) )
		circsize = 0.50;
	else
		circsize = 0.30;

	for (int i=0; i < v.size(); i++)
	{
		line.Printf (wxT ("%ld %ld %f"), v[i].first, v[i].second, circsize);
		dataFile.AddLine (line);
	}

	dataFile.Write();
	dataFile.Close();
}

void PointPlotEllipticCurve::findAxesRanges (const vector<pair<long,long> >& v,
      long range[4])
{
	long minx = LONG_MAX, maxx = LONG_MIN, miny = LONG_MAX, maxy = LONG_MIN;
	long xcoor, ycoor;

	for (int i=0; i <v.size(); i++)
	{
		xcoor = v[i].first;
		ycoor = v[i].second;

		if (xcoor < minx)
			minx = xcoor;
		else if (xcoor > maxx)
			maxx = xcoor;

		if (ycoor < miny)
			miny = ycoor;
		else if (ycoor > maxy)
			maxy = ycoor;
	}

	range[0] = minx - 1;
	range[1] = maxx + 1;
	range[2] = miny - 1;
	range[3] = maxy + 1;
}
