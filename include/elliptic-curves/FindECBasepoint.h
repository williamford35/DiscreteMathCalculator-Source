#ifndef FINDECBASEPOINT_H
#define FINDECBASEPOINT_H

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
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#include <miracl/big.h>
#include <miracl/ecn.h>
#include "elliptic-curves/schoof.h"
#include "elliptic-curves/schoofException.h"
#include "elliptic-curves/collectBigFactors.h"

class FindBasePointDialog : public wxDialog
{
protected:
	wxStaticText* FindBasePointLabelStaticText;
	wxPanel* FindBasePointFactoredOrderPanel;
	wxStaticText* FindBasePointFactoredLabelStaticText;
	wxTextCtrl* FindCurveOrderTextCtrl;
	wxPanel* FindBasePointChoosePanel;
	wxStaticText* FindBasePointChooseSelectNStaticText;
	wxTextCtrl* FindBasePointChooseSelectNTextCtrl;
	wxButton* FindBasePointGenerateButton;
	wxPanel* FindBasePointValuePanel;
	wxStaticText* FindBasePointXCoorLabelStaticText;
	wxTextCtrl* FindBasePointXCoorTextCtrl;
	wxStaticText* FindBasePointYCoorLabelStaticText;
	wxTextCtrl* FindBasePointYCoorTextCtrl;
	wxPanel* FindBasePointCloseCancelPanel;
	wxButton* FindBasePointCancelButton;
	wxButton* FindBasePointCloseButton;

protected:
	virtual void OnGenerateBasePointButtonPress (wxCommandEvent& event);
	virtual void OnCancelBasePointDialog (wxCommandEvent& event);
	virtual void OnCloseBasePointDialog (wxCommandEvent& event);

public:
	FindBasePointDialog (wxWindow* parent, const Big& prime, const Big& Acoor, const Big& Bcoor, ECn *bpt, bool hveOrder, Big *order, bool hvePtOrder, Big *ptorder, bool hveCofactor, Big *cofact, int b, wxWindowID id = wxID_ANY, const wxString& title = _ ("Find Base Point Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize (500,400), long style = wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX);
	virtual ~FindBasePointDialog();

private:

	void factorOrder();

	Big *curveOrder;
	Big *n;
	Big *cofactor;
	ECn *basePoint;
	Big p, A, B;
	Big X, Y;
	bool haveCurveOrder, havePointOrder, haveCofactor;
	int base;
};

#endif	// FINDECBASEPOINT_H
