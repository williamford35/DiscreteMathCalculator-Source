/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "elliptic-curves/FindECBasepoint.h"
#include "elliptic-curves/ecurvelib.h"
#include "elliptic-curves/collectBigFactors.h"
#include "common/report.h"

#include <vector>

using namespace std;

FindBasePointDialog::FindBasePointDialog (wxWindow* parent, const Big& prime, const Big& Acoor, const Big& Bcoor, ECn *bpt, bool hveOrder, Big *order, bool hvePtOrder, Big *ptorder, bool hveCofactor, Big *cofact, int b, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), p (prime), A (Acoor), B (Bcoor), basePoint (bpt), curveOrder (order), cofactor (cofact), n (ptorder), haveCurveOrder (hveOrder), havePointOrder (hvePtOrder), haveCofactor (hveCofactor), base (b)
{
	wxBoxSizer* FindBasePointBoxSizer = new wxBoxSizer (wxVERTICAL);
	this->SetSizer (FindBasePointBoxSizer);

	FindBasePointLabelStaticText = new wxStaticText (this, wxID_ANY, _ ("Choose a prime factor of the elliptic curve order:"), wxDefaultPosition, wxSize (-1,-1), 0);

	if (havePointOrder)
		FindBasePointLabelStaticText->Hide();

	FindBasePointBoxSizer->Add (FindBasePointLabelStaticText, 0, wxALL, 5);

	FindBasePointFactoredOrderPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	FindBasePointBoxSizer->Add (FindBasePointFactoredOrderPanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* FindBasePointFactoredOrderBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	FindBasePointFactoredOrderPanel->SetSizer (FindBasePointFactoredOrderBoxSizer);

	FindBasePointFactoredLabelStaticText = new wxStaticText (FindBasePointFactoredOrderPanel, wxID_ANY, _ ("Curve order ="), wxDefaultPosition, wxSize (-1,-1), 0);

	FindBasePointFactoredOrderBoxSizer->Add (FindBasePointFactoredLabelStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	FindCurveOrderTextCtrl = new wxTextCtrl (FindBasePointFactoredOrderPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	FindCurveOrderTextCtrl->SetHint (wxT ("") );
#endif

	FindBasePointFactoredOrderBoxSizer->Add (FindCurveOrderTextCtrl, 2, wxALL|wxEXPAND, 5);

	FindBasePointChoosePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	FindBasePointBoxSizer->Add (FindBasePointChoosePanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* FindBasePointChooseBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	FindBasePointChoosePanel->SetSizer (FindBasePointChooseBoxSizer);

	FindBasePointChooseSelectNStaticText = new wxStaticText (FindBasePointChoosePanel, wxID_ANY, _ ("Choice for n:"), wxDefaultPosition, wxSize (-1,-1), 0);

	FindBasePointChooseBoxSizer->Add (FindBasePointChooseSelectNStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	FindBasePointChooseSelectNTextCtrl = new wxTextCtrl (FindBasePointChoosePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	FindBasePointChooseSelectNTextCtrl->SetHint (wxT ("") );
#endif

	FindBasePointChooseBoxSizer->Add (FindBasePointChooseSelectNTextCtrl, 3, wxALL, 5);

	FindBasePointChooseBoxSizer->Add (0, 0, 1, wxALL, 5);

	FindBasePointGenerateButton = new wxButton (this, wxID_ANY, _ ("Generate base point"), wxDefaultPosition, wxSize (-1,-1), 0);

	FindBasePointBoxSizer->Add (FindBasePointGenerateButton, 0, wxALL, 5);

	FindBasePointValuePanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	FindBasePointBoxSizer->Add (FindBasePointValuePanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* FindBasePointValueBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	FindBasePointValuePanel->SetSizer (FindBasePointValueBoxSizer);

	FindBasePointXCoorLabelStaticText = new wxStaticText (FindBasePointValuePanel, wxID_ANY, _ ("Base point:  x:"), wxDefaultPosition, wxSize (-1,-1), 0);

	FindBasePointValueBoxSizer->Add (FindBasePointXCoorLabelStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	FindBasePointXCoorTextCtrl = new wxTextCtrl (FindBasePointValuePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	FindBasePointXCoorTextCtrl->SetHint (wxT ("") );
#endif

	FindBasePointValueBoxSizer->Add (FindBasePointXCoorTextCtrl, 1, wxALL, 5);

	FindBasePointYCoorLabelStaticText = new wxStaticText (FindBasePointValuePanel, wxID_ANY, _ ("y:"), wxDefaultPosition, wxSize (-1,-1), 0);

	FindBasePointValueBoxSizer->Add (FindBasePointYCoorLabelStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	FindBasePointYCoorTextCtrl = new wxTextCtrl (FindBasePointValuePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxSize (-1,-1), 0);
#if wxVERSION_NUMBER >= 3000
	FindBasePointYCoorTextCtrl->SetHint (wxT ("") );
#endif

	FindBasePointValueBoxSizer->Add (FindBasePointYCoorTextCtrl, 1, wxALL|wxALIGN_CENTER, 5);

	FindBasePointCloseCancelPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	FindBasePointBoxSizer->Add (FindBasePointCloseCancelPanel, 0, wxALL|wxEXPAND, 5);

	wxBoxSizer* FindBasePointCloseCancelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	FindBasePointCloseCancelPanel->SetSizer (FindBasePointCloseCancelBoxSizer);

	FindBasePointCloseCancelBoxSizer->Add (0, 0, 2, wxALL, 5);

	FindBasePointCancelButton = new wxButton (FindBasePointCloseCancelPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);

	FindBasePointCloseCancelBoxSizer->Add (FindBasePointCancelButton, 0, wxALL, 5);

	FindBasePointCloseButton = new wxButton (FindBasePointCloseCancelPanel, wxID_ANY, _ ("Close"), wxDefaultPosition, wxSize (-1,-1), 0);
	FindBasePointCloseButton->SetDefault();

	FindBasePointCloseCancelBoxSizer->Add (FindBasePointCloseButton, 0, wxALL, 5);

	SetName (wxT ("FindBasePointDialog") );
	SetSize (500,350);

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
	FindBasePointGenerateButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (FindBasePointDialog::OnGenerateBasePointButtonPress), NULL, this);
	FindBasePointCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (FindBasePointDialog::OnCancelBasePointDialog), NULL, this);
	FindBasePointCloseButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (FindBasePointDialog::OnCloseBasePointDialog), NULL, this);

	if (haveCurveOrder)
	{
		big p = mirvar(0);

		p = curveOrder->getbig();
		if (!isprime(p))
			haveCurveOrder = false;
	}
	if ( (havePointOrder == false) || (haveCurveOrder == false))
		factorOrder();
	else
	{
		wxString sbuf;
		char buf[625];

		FindBasePointLabelStaticText->Hide();
		FindBasePointChooseSelectNStaticText->SetLabel (wxT ("Point order =") );
		buf << *curveOrder;
		FindCurveOrderTextCtrl->SetValue (buf);
		buf << *n;
		sbuf = wxString::FromUTF8 (buf);
		FindBasePointChooseSelectNTextCtrl->SetValue (sbuf);
	}

	miracl *mip = get_mip();

	mip->IOBASE = b;
}

FindBasePointDialog::~FindBasePointDialog()
{
	FindBasePointGenerateButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (FindBasePointDialog::OnGenerateBasePointButtonPress), NULL, this);
	FindBasePointCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (FindBasePointDialog::OnCancelBasePointDialog), NULL, this);
	FindBasePointCloseButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (FindBasePointDialog::OnCloseBasePointDialog), NULL, this);
}

void FindBasePointDialog::OnGenerateBasePointButtonPress (wxCommandEvent& event)
{
	wxString s;
	char buf[300];
	char *ptr;

	if (havePointOrder)
	{
		s = FindBasePointChooseSelectNTextCtrl->GetValue();
		strcpy (buf, (const char *) s.mb_str() );
		*n = Big (buf);
	}
	else
	{
		s = FindBasePointChooseSelectNTextCtrl->GetValue();

		if (s.length() == 0)
		{
			report (wxT ("You must select a prime factor of the curve order (preferably the largest"), wxT ("Note") );
			return;
		}
		else
		{
			strcpy (buf, (const char *) s.mb_str() );
			ptr = buf;

			while (*ptr != '\0')
			{
				if (*ptr == ' ')
					*ptr = '\0';

				ptr++;
			}

			*n = Big (buf);
		}
	}

	*basePoint = findBasePoint (A, B, p, *n, *curveOrder);

	if (basePoint->iszero() )
		return;

	basePoint->get (X, Y);
	buf << X;
	s = wxString::FromUTF8 (buf);
	FindBasePointXCoorTextCtrl->SetValue (s);
	buf << Y;
	s = wxString::FromUTF8 (buf);
	FindBasePointYCoorTextCtrl->SetValue (s);
}

void FindBasePointDialog::OnCancelBasePointDialog (wxCommandEvent& event)
{
	EndModal (wxID_CANCEL);
}

void FindBasePointDialog::OnCloseBasePointDialog (wxCommandEvent& event)
{
	if (!haveCofactor)
		*cofactor = *curveOrder/ (*n);

	EndModal (wxID_OK);
}

void FindBasePointDialog::factorOrder()
{
	Big tmp_p;
	Big order;
	int i, j, k;
	char buf[625];
	vector<Factor> factorization;
	wxString s;
	miracl *mip = get_mip();

	if (haveCurveOrder == false)
		if (haveCofactor)
		{
			*curveOrder = (*cofactor) * (*n);
			buf << *curveOrder;
			FindCurveOrderTextCtrl->SetValue (buf);
			haveCurveOrder = true;
		}
		else
		{
			try
			{
				tmp_p = p;
				order = schoof (A, B, p);
				p = tmp_p;
			}

			catch (const schoofException& se)
			{
				report (se.what().c_str(), wxT ("Note") );
				EndModal (wxID_CANCEL);
			}

			*curveOrder = order;
			buf << *curveOrder;
			FindCurveOrderTextCtrl->SetValue (buf);
		}
	else
	{
		buf << *curveOrder;
		FindCurveOrderTextCtrl->SetValue (buf);
	}

	if (havePointOrder == false)
	{
		mip->IOBASE = 10;
		factorization = collectBigFactors (order);

		for (i=0; i < factorization.size(); i++)
		{
			k = factorization[i].k;
			j = 0;

			Big x = factorization[i].number;

			while (j < k)
			{
				if (base == 10)
					mip->IOBASE = 10;
				else
					mip->IOBASE = 16;

				buf << factorization[i].number;
				s += (wxString) buf + wxT (" ");
				j++;
			}

			mip->IOBASE = base;
			FindCurveOrderTextCtrl->SetValue (s);
			mip->IOBASE = base;
		}
	}
	else
	{
		buf << *n;
		FindBasePointChooseSelectNTextCtrl->SetValue (buf);
	}
}
