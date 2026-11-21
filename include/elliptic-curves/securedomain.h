// header from wxCrafter

#ifndef CONSTRUCTSECUREVERIFYABLECURVE_H
#define CONSTRUCTSECUREVERIFYABLECURVE_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statline.h>
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

#include <iostream>
#include <cmath>
#include <cstring>
#include "miracl/complex.h"
#include "miracl/flpoly.h"
#include "miracl/poly.h"
#include "miracl/big.h"
#include "miracl/ecn.h"
#include "miracl/zzn.h"
#include "elliptic-curves/securedomain.h"

#include <wx/textctrl.h>
#include <wx/string.h>
#include "elliptic-curves/defineEllipticCurve.h"
#include "elliptic-curves/ecurvelib.h"
#include "common/report.h"

using namespace std;

class SecureDomain : public wxDialog
{
protected:
	wxStaticText* BuildEllipticCurveEquationLabel;
	wxStaticText* BuildEllipticCurvePrimeLabel;
	wxTextCtrl* BuildEllipticCurvePrimeTextCtrl;
	wxStaticText* BuildEllipticCurveSeedStaticText;
	wxTextCtrl* BuildEllipticCurveSeedTextCtrl;
	wxStaticLine* componentSeparationStaticLine1;
	wxStaticLine* componentSeparationStaticLine2;
	wxStaticText* BuildEllipticCurveAStaticText;
	wxTextCtrl* BuildEllipticCurveATextCtrl;
	wxStaticText* BuildEllipticCurveBStaticText;
	wxTextCtrl* BuildEllipticCurveBTextCtrl;
	wxStaticText* BuildEllipticCurveOrderStaticText;
	wxTextCtrl* BuildEllipticCurveOrderTextCtrl;
	wxStaticText* BuildEllipticCurveBasepointOrderStaticText;
	wxTextCtrl* BuildEllipticCurveBasepointOrderTextCtrl;
	wxStaticText* BuildEllipticCurveCofactorLabel;
	wxTextCtrl* BuildEllipticCurveCofactorTextCtrl;
	wxStaticText* BuildEllipticCurvePxLabel;
	wxTextCtrl* BuildEllipticCurvePxTextCtrl;
	wxStaticText* BuildEllipticCurvePyLabel;
	wxTextCtrl* BuildEllipticCurvePyTextCtrl;
	wxPanel* BuildEllipticCurveControlPanel;
	wxButton* BuildEllipticCurveButton;
	wxButton* BuildEllipticCurveCancelButton;
	wxButton* BuildEllipticCurveSaveButton;
	wxStaticText* SchoofTimeTakenStaticText;
protected:
	virtual void OnBuildEllipticCurveButtonPress(wxCommandEvent& event);
	virtual void OnCancelDialogButtonPress(wxCommandEvent& event);
	virtual void OnSaveAsCurrentCurveButtonPress(wxCommandEvent& event);
public:
	SecureDomain(wxWindow* parent, vector<wxTextCtrl *> v,
	             int b, Big& p, Big& userSeed, wxWindowID id = wxID_ANY, const wxString& title = wxT (""),
	             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (-1,-1),
	             long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	wxTextCtrl* BuildEllipticCurveTimeTakenTextCtrl;
	virtual ~SecureDomain();
private:
	FPoly T[25];  // Reduced class Polynomial.
	BOOL three = false;
	char *s;
	long precision;
	Big order, xcoor, ycoor, value, basepointOrder, cofactor;
	Big prime, seed, A, B;
	vector<wxTextCtrl *> writeNewCurveValues;
	void WriteResult (wxTextCtrl* t, const Big& v);
	void CreateTheCurve();

	// Big  GetBigScalar (wxTextCtrl* t);
	Complex Fz (Complex z);
	Complex F (int j,Big A,Big B,Big C,Big D,int N);
	int base;

	int geti (Big D);
	int getk (Big D);
	int getN (Big D);
	void class_poly (Complex& lam,Float *Fi2,Big A,Big B,Big C,Big D,BOOL conj,BOOL P1363);
	int groups (Complex& lam,Float *Fi2,unsigned long D,BOOL doit,BOOL P1363);
	BOOL isaD (unsigned long d,int pm8,Big k);
	Big floor (Big N,Big D);
	BOOL isacm (Big p,unsigned long D,Big &W,Big &V);
	BOOL get_curve (Complex& lam,Float *Fi2,const Big& r,const Big& other_r,const Big& ord,unsigned long D,const Big& p,const Big& W,int m,BOOL always,
	                Big& prime, Big& A, Big& B, Big& ptOrder, Big& xcoor, Big& ycoor, Big& cofactor);
	void eval_cmpower (Big& oldn,Big& n,char op);
	void eval_cmproduct (Big& oldn,Big& n,char op);
	void eval_cmsum (Big& oldn,Big& n,char op);
	void eval (Big& t);
	bool cm (Big& prime, Big& A, Big& B, Big& ptOrder, Big& Gx, Big& Gy, Big& cofactor, int m);
};

#endif // secure domain header