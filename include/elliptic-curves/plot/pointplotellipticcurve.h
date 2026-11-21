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

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

#include <vector>
using namespace std;


class PointPlotEllipticCurve : public wxDialog
{
protected:
	wxPanel* EllipticCurvePlotsPanel;
	wxStaticBitmap* PointPlotEllipticCurveStaticBitmap;
	wxStaticText* EllipticCurvePlotsLabelStaticText;
	wxPanel* EllipticCurvePlotCoefficientsPanel;
	wxStaticText* EllipticCurvePlotEquationStaticText;
	wxStaticText* EllipticCurveXLabelCoefficientStaticText;
	wxTextCtrl* EllipticCurveXCoefficientTextCtrl;
	wxStaticText* EllipticCurveConstantLabelCoefficientStaticText;
	wxTextCtrl* EllipticCurveConstantCoefficientTextCtrl;
	wxStaticText* m_staticText77;
	wxTextCtrl* EllipticCurvePointPlotPrime;
	wxPanel* EllipticCurvePlotActionPanel;
	wxButton* EllipticCurvePlotActionButton;
	wxPanel* EllipticCurveClosePanel;
	wxButton* EllipticCurveCloseButton;

protected:
	virtual void OnEllipticCurvePlotButtonPress (wxCommandEvent& event);
	virtual void OnEllipticCurveCloseButtonPress (wxCommandEvent& event);

public:
	PointPlotEllipticCurve (wxWindow* parent, const char *image,
	                        wxWindowID id = wxID_ANY, const wxString& title =_ ("Plot of elliptic curve"),
	                        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (725,500),
	                        long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);
	virtual ~PointPlotEllipticCurve();

private:
	bool GetIntValue (wxTextCtrl *t, long *v, const wxString& msg);
	wxString GetTemporaryFileName();
	void writePointData (long p, const wxString& fname,
	                     const vector<pair<long,long> >& v);
	void findAxesRanges (const vector<pair<long,long> >& v,
	                     long range[4]);
};
