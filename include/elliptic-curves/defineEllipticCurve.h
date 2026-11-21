#ifndef DEFINE_ELLIPTIC_CURVE_H
#define DEFINE_ELLIPTIC_CURVE_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <miracl/big.h>
#include <miracl/ecn.h>

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

struct CurveData
{
	Big p;
	Big A;
	Big B;
	Big order;
	Big Gx;
	Big Gy;
	Big n;
	Big cofactor;
	Big seed;
};

class DefineEllipticCurve : public wxDialog
{
protected:
	// wxStaticText* DefineOwnEllipticCurveStaticText;
	// wxPanel* DefineEllipticCurveCoefficientsPanel;
	// wxStaticText* DefineOwnEllipticCurveEquationStaticText;
	// wxStaticText* DefineOwnEllipticCurveACoefficientStaticText;
	// wxTextCtrl* DefineOwnEllipticCurveACoefficientTextCtrl;
	// wxStaticText* DefineOwnEllipticCurveBCoefficientStaticText;
	// wxTextCtrl* DefineOwnEllipticCurveBCoeficientTextCtrl;
	// wxStaticText* DefineOwnEllipticCurvePrimeStaticText;
	// wxTextCtrl* DefineOwnEllipticCurvePrimeTextCtrl;
	// wxButton* DefineOwnEllipticCurveSelectButton;
	wxStaticLine* ChooseEllipticCurveStaticLine;
	wxStaticText* SelectEllipticCurveStaticText;
	wxListView* SelectEllipticCurveListCtrl;
	wxPanel* DefineEllipticCurveCancelClosePanel;
	wxButton* DefineEllipticCurveCancelButton;
	wxButton* DefineEllipticCurveSelectPredefinedButton;

protected:
	virtual void OnChooseEllipticCurveCancelButtonPress (wxCommandEvent& event);
	virtual void OnChooseEllipticCurvePredefinedButtonPress (wxCommandEvent& event);

public:
	DefineEllipticCurve (wxWindow* parent, int *i, CurveData *cdata, int b, wxWindowID id = wxID_ANY,
	                     const wxString& title = _ ("Choose an elliptic curve"),
	                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (-1,-1),
	                     long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);
	virtual ~DefineEllipticCurve();

private:
	Big *p;
	int *chosen;
	CurveData *myCurveData;
	void fillListCtrl();
	void InsertItemInReportView (int i);
	int base;

	wxString note[22] = {wxT ("1. (simple)"), wxT ("2. (simple)"), wxT ("3. (simple)"),
	                     wxT ("4. (instructional)"), wxT ("5. (instructional)"), wxT("6. (instructional)"), wxT ("7. (prime192v1)"),
	                     wxT ("8. (secp224r1)"), wxT ("9.(secp384r1)"), wxT ("10. (secp521r1)"), wxT ("11. (prime192v2)"),
	                     wxT ("12. (prime192v3)"), wxT ("13. (prime239v1)"), wxT ("14. (prime239v2)"), wxT ("15. (prime239v3)"),
	                     wxT ("16. (prime256v1)"), wxT ("17. (secp112r1)"), wxT ("18. (secp112r2)"), wxT ("19. (secp128r1)"),
	                     wxT ("20. (secp128r2)"), wxT ("21. (secp160r1)"), wxT ("22. (secp160r2)")
	                    };
	wxString bits[22] = {wxT ("7"), wxT ("8"), wxT ("16"),
	                     wxT ("30"), wxT ("60"), wxT("80"), wxT ("192"),
	                     wxT ("224"), wxT ("384"), wxT ("521"), wxT ("192"),
	                     wxT("192"), wxT("239"), wxT ("239"), wxT ("239"),
	                     wxT("256"), wxT("112"), wxT ("112"), wxT ("128"),
	                     wxT("128"), wxT("160"), wxT ("160")
	                    };
	wxString prime10[22] = {wxT ("101"), wxT ("211"), wxT ("39857"),
	                        wxT ("734650211"), wxT ("678077912765174123"),
	                        wxT ("743865571034356289362307"),
	                        wxT ("6277101735386680763835789423207666416083908700390324961279"),
	                        wxT ("26959946667150639794667015087019630673557916260026308143510066298881"),
	                        wxT ("39402006196394479212279040100143613805079739270465446667948293404245721771496870329047266088258938001861606973112319"),
	                        wxT ("6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151"),
	                        wxT ("6277101735386680763835789423207666416083908700390324961279"),
	                        wxT ("6277101735386680763835789423207666416083908700390324961279"),
	                        wxT ("883423532389192164791648750360308885314476597252960362792450860609699839"),
	                        wxT ("883423532389192164791648750360308885314476597252960362792450860609699839"),
	                        wxT ("883423532389192164791648750360308885314476597252960362792450860609699839"),
	                        wxT("115792089210356248762697446949407573530086143415290314195533631308867097853951"),
	                        wxT ("4451685225093714772084598273548427"),
	                        wxT ("4451685225093714772084598273548427"),
	                        wxT ("340282366762482138434845932244680310783"),
	                        wxT ("340282366762482138434845932244680310783"),
	                        wxT ("1461501637330902918203684832716283019653785059327"),
	                        wxT ("1461501637330902918203684832716283019651637554291")
	                       };

	wxString prime16[22] = {wxT ("65"), wxT ("89"), wxT ("CF9D"),
	                        wxT ("27D79477"), wxT ("9690444E068116B"),
	                        wxT ("9D85088D670A746FD983"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF"),
	                        wxT ("1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"),
	                        wxT ("7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFF"),
	                        wxT ("7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFF"),
	                        wxT ("7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFF"),
	                        wxT("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF"),
	                        wxT ("DB7C2ABF62E35E668076BEAD208B"),
	                        wxT ("DB7C2ABF62E35E668076BEAD208B"),
	                        wxT ("FFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFF"),
	                        wxT ("FFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFF"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF"),
	                        wxT ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFAC73")
	                       };
};

#endif // DEFINE_ELLIPTIC_CURVE_H
