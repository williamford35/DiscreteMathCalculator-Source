#ifndef PLOTELLIPTICCURVE_H
#define PLOTELLIPTICCURVE_H

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <string>

class PlotEllipticCurve : public wxDialog
{
protected:
	wxPanel* EllipticCurvePlotsPanel;
	wxStaticBitmap* EllipticCurvePlot1StaticBitmap;
	wxStaticBitmap* EllipticCurvePlot2StaticBitmap;
	wxStaticText* EllipticCurvePlotsLabelStaticText;
	wxPanel* EllipticCurvePlotCoefficientsPanel;
	wxStaticText* EllipticCurvePlotEquationStaticText;
	wxStaticText* EllipticCurveXLabelCoefficientStaticText;
	wxTextCtrl* EllipticCurveXCoefficientTextCtrl;
	wxStaticText* EllipticCurveConstantLabelCoefficientStaticText;
	wxTextCtrl* EllipticCurveConstantCoefficientTextCtrl;
	wxPanel* EllipticCurveXYPlotRangesPanel;
	wxStaticText* EllipticCurveXRangeLabelStaticText;
	wxTextCtrl* EllipticCurveXRangeLowTextCtrl;
	wxStaticText* EllipticCurveXRangeToStaticText;
	wxTextCtrl* EllipticCurveXRangeHighTextCtrl;
	wxStaticText* EllipticCurveYRangeLabelStaticText;
	wxTextCtrl* EllipticCurveYRangeLowTextCtrl;
	wxStaticText* EllipticCurveYRangeToStaticText;
	wxTextCtrl* EllipticCurveYRangeHighTextCtrl;
	wxPanel* EllipticCurvePlotActionPanel;
	wxButton* EllipticCurvePlotActionButton;
	wxPanel* EllipticCurveClosePanel;
	wxButton* EllipticCurveCancelButton;
	wxButton* EllipticCurveCloseButton;

protected:
	virtual void OnEllipticCurvePlotButtonPress (wxCommandEvent& event);
	virtual void OnEllipticCurveCancelButtonPress (wxCommandEvent& event);
	virtual void OnEllipticCurveCloseButtonPress (wxCommandEvent& event);

public:
	PlotEllipticCurve (wxWindow* parent, const char *image1, const char *image2, wxWindowID id = wxID_ANY, const wxString& title = _ ("Plot of elliptic curve"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (725,500), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);
	virtual ~PlotEllipticCurve();

private:
	bool GetFloatValue(wxTextCtrl *t, double *v, const wxString& msg);
};

#endif	// PLOTELLIPTICCURVE_H
