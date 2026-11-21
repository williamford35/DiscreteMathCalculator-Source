
/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "RelationsPartialOrders/PartialOrderDialog.h"
#include "common/ImageFiles.h"
#include "graphs/algorithms/InputIntegerData.h"
#include "graphs/build/BuildGraph.h"

#include <wx/msgdlg.h>

#include <algorithm>
using namespace std;

PartialOrderDialog::PartialOrderDialog (wxWindow* parent, const char *imageFileName, vector<RelationGraphEdge>* posetEdge, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog (parent, id, title, pos, size, style), diGraphToBuild (posetEdge), digraphNodeMap (dig)
{
	wxFlexGridSizer* partialOrderDialogFlexGridSizer = new wxFlexGridSizer (6, 1, 0, 0);
	partialOrderDialogFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	partialOrderDialogFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	partialOrderDialogFlexGridSizer->AddGrowableCol (0);
	partialOrderDialogFlexGridSizer->AddGrowableRow (0);
	partialOrderDialogFlexGridSizer->AddGrowableRow (1);
	partialOrderDialogFlexGridSizer->AddGrowableRow (2);
	partialOrderDialogFlexGridSizer->AddGrowableRow (3);
	partialOrderDialogFlexGridSizer->AddGrowableRow (4);
	partialOrderDialogFlexGridSizer->AddGrowableRow (5);
	this->SetSizer (partialOrderDialogFlexGridSizer);

	partialOrderDialogStaticBitmap = new wxStaticBitmap (this, wxID_ANY, wxBitmap (wxImage (imagefile (imageFileName) ) ), wxDefaultPosition, wxSize (-1,-1), 0 );
	partialOrderDialogFlexGridSizer->Add (partialOrderDialogStaticBitmap, 1, wxALL, 1);
	partialOrderDialogStaticText = new wxStaticText (this, wxID_ANY, _ ("Hasse Diagram"), wxDefaultPosition, wxSize (-1,-1), 0);
	wxFont partialOrderDialogStaticTextFont (24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("Times New Roman") );
	partialOrderDialogStaticText->SetFont (partialOrderDialogStaticTextFont);
	partialOrderDialogFlexGridSizer->Add (partialOrderDialogStaticText, 0, wxALL, 1);
	partialOrderDialogPowersetPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	partialOrderDialogFlexGridSizer->Add (partialOrderDialogPowersetPanel, 0, wxALL, 5);
	wxBoxSizer* partialOrderDialogPowersetBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	partialOrderDialogPowersetPanel->SetSizer (partialOrderDialogPowersetBoxSizer);
	partialOrderDialogPowersetLabelStaticText = new wxStaticText (partialOrderDialogPowersetPanel, wxID_ANY, _ ("Powerset ordered by inclusion"), wxDefaultPosition, wxSize (-1,-1), 0);
	partialOrderDialogPowersetBoxSizer->Add (partialOrderDialogPowersetLabelStaticText, 0, wxALL, 4);
	partialOrderDialogPowersetBoxSizer->Add(0, -1, 1, wxALL, 5);
	partialOrderDialogPowersetNStaticText = new wxStaticText (partialOrderDialogPowersetPanel, wxID_ANY, _ ("n ="), wxDefaultPosition, wxSize (-1,-1), 0);
	partialOrderDialogPowersetBoxSizer->Add (partialOrderDialogPowersetNStaticText, 0, wxALL, 5);
	partialOrderDialogPowersetNvalueTextCtrl = new wxTextCtrl (partialOrderDialogPowersetPanel, wxID_ANY,
	      wxT (""), wxDefaultPosition, wxSize (50,-1), 0);
#if wxVERSION_NUMBER >= 3000
	partialOrderDialogPowersetNvalueTextCtrl->SetHint (wxT ("") );
#endif
	partialOrderDialogPowersetBoxSizer->Add (partialOrderDialogPowersetNvalueTextCtrl, 0, wxALL, 6);
	partialOrderDialogPowersetBuildLattice = new wxButton (partialOrderDialogPowersetPanel, wxID_ANY, _ ("Display lattice"), wxDefaultPosition, wxSize (-1,-1), 0);
	partialOrderDialogPowersetBoxSizer->Add (partialOrderDialogPowersetBuildLattice, 0, wxRIGHT|wxTOP|wxBOTTOM, 4);
	partialOrderDialogDivisibilityPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	partialOrderDialogFlexGridSizer->Add (partialOrderDialogDivisibilityPanel, 0, wxALL|wxEXPAND, 1);
	wxBoxSizer* partialOrderDialogDivisibilityBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	partialOrderDialogDivisibilityPanel->SetSizer (partialOrderDialogDivisibilityBoxSizer);
partialOrderDialogDivisibilityLabelStaticText = new wxStaticText (
    partialOrderDialogDivisibilityPanel,
    wxID_ANY,
    _("Divisibility"),
    wxDefaultPosition,
    wxDefaultSize,
    0);

	partialOrderDialogDivisibilityBoxSizer->Add (
	    partialOrderDialogDivisibilityLabelStaticText,
	    0, wxALL | wxALIGN_CENTER_VERTICAL, 4);

	partialOrderDialogInputDivisibilityInputDataButton = new wxButton (
	    partialOrderDialogDivisibilityPanel,
	    wxID_ANY,
	    _("Input data"),
	    wxDefaultPosition,
	    wxDefaultSize,
	    0);

	partialOrderDialogDivisibilityBoxSizer->Add (
    partialOrderDialogInputDivisibilityInputDataButton,
    	0, wxALL | wxALIGN_CENTER_VERTICAL, 4);
	partialOrderDialogButtonsPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	partialOrderDialogFlexGridSizer->Add (partialOrderDialogButtonsPanel, 0, wxALL, 5);
	wxBoxSizer* partialOrderDialogButtonsPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	partialOrderDialogButtonsPanel->SetSizer (partialOrderDialogButtonsPanelBoxSizer);
	partialOrderDialogButtonsPanelBoxSizer->Add (390, 0, 1, wxALL, 5);
	partialOrderDialogCancelButton = new wxButton (partialOrderDialogButtonsPanel, wxID_ANY, _ ("Cancel"), wxDefaultPosition, wxSize (-1,-1), 0);
	partialOrderDialogButtonsPanelBoxSizer->Add (partialOrderDialogCancelButton, 0, wxALL, 4);

	SetName (wxT ("PartialOrderDialog") );
	SetSizeHints (550,650);

	if ( GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	CentreOnParent (wxBOTH);
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
	partialOrderDialogPowersetBuildLattice->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PartialOrderDialog::OnPartialOrderDialogPowersetBuildLatticePress), NULL, this);
	partialOrderDialogInputDivisibilityInputDataButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PartialOrderDialog::OnDivisibilityInputDataButtonPress), NULL, this);
	partialOrderDialogCancelButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PartialOrderDialog::OnCancelPartialOrderDialogButtonPress), NULL, this);
}

PartialOrderDialog::~PartialOrderDialog()
{
	partialOrderDialogPowersetBuildLattice->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PartialOrderDialog::OnPartialOrderDialogPowersetBuildLatticePress), NULL, this);
	partialOrderDialogInputDivisibilityInputDataButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PartialOrderDialog::OnDivisibilityInputDataButtonPress), NULL, this);
	partialOrderDialogCancelButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (PartialOrderDialog::OnCancelPartialOrderDialogButtonPress), NULL, this);
}

void PartialOrderDialog::OnPartialOrderDialogPowersetBuildLatticePress (wxCommandEvent& event)
{
	wxString nstr;
	long n;
	Node node, node_u, node_v;
	wxString srcVertex, destVertex;
	vector<set<char> > latticeVertex;

	// setInProcess();
	nstr = partialOrderDialogPowersetNvalueTextCtrl->GetValue();

	if (nstr.Length() == 0)
	{
		wxMessageDialog dialog (this, wxT ("You have not entered the set size."), wxT ("Note"), wxOK | wxICON_INFORMATION);
		dialog.ShowModal();
		return;
	}

	nstr.ToLong (&n);
	latticeVertex = powerset (n);
	buildPowersetRelation (latticeVertex);
	buildHasseDiagram();
	// clearSetInProcess();
	EndModal (wxID_OK);
}

void PartialOrderDialog::OnDivisibilityInputDataButtonPress (wxCommandEvent& event)
{
	vector<int> dataValues;

	// setInProcess();
	InputIntegerData  dataEntered (this, &dataValues);

	if (dataEntered.ShowModal() == wxID_CANCEL)
		return;

	if (dataValues.empty() )
		return;

	buildDivisibilityRelation (dataValues);
	buildHasseDiagram();

	// clearSetInProcess();
	EndModal (wxID_OK);
}

void PartialOrderDialog::OnCancelPartialOrderDialogButtonPress (wxCommandEvent& event)
{
	// clearSetInProcess();
	EndModal (wxID_CANCEL);
}

std::vector<wxString> PartialOrderDialog::getAllVertices() const
{
    std::vector<wxString> result;
    for (lemon::ListDigraph::NodeIt n(dig); n != lemon::INVALID; ++n)
        result.push_back(digraphNodeMap[n]);
    return result;
}

void PartialOrderDialog::buildHasseDiagram()
{
	Bfs<ListDigraph> bfs_test (dig);
	Node src, dest, rowNode;
	wxString srcLabel, destLabel;
	int i, j, k;
	int numVertices = lemon::countNodes (dig);

	// setInProcess();
	matrix<int> adjMatrix (numVertices, numVertices, 0);

	// build the adjacency matrix
	for (i = 0; i < numVertices; i++)
	{
		rowNode = dig.nodeFromId (i);

		for (ListDigraph::OutArcIt a (dig, rowNode); a!=INVALID; ++a)
			adjMatrix[i][ dig.id ( (dig.target (a) ) )] = 1;
	}

	matrix<int> reachabilityMatrix (numVertices, numVertices, 0);

	for (i = 0; i < numVertices; i++)
	{
		for (j =0; j < numVertices; j++)
			if (i != j)
			{
				bfs_test.run (dig.nodeFromId (i) );

				if (bfs_test.reached (dig.nodeFromId (j) ) )
					reachabilityMatrix[i][j] = 1;
			}
	}

	// build the adjacency matrix for the transitive reduction of the acyclic digraph.
	for (i = 0; i < numVertices; i++)
		for (j = 0; j < numVertices; j++)
			if (reachabilityMatrix[i][j] == 1)
			{
				src = dig.nodeFromId (i);
				srcLabel = digraphNodeMap[src];
				dest = dig.nodeFromId (j);
				destLabel = digraphNodeMap[dest];
			}

    // An edge u->v is a cover exactly when no third vertex w
    // satisfies u->w and w->v. Read only from the original reachability.
    matrix<int> R = reachabilityMatrix;
    for (i = 0; i < numVertices; ++i)
        for (j = 0; j < numVertices; ++j)
            if (reachabilityMatrix[i][j])
                for (k = 0; k < numVertices; ++k)
                    if (k != i && k != j &&
                        reachabilityMatrix[i][k] && reachabilityMatrix[k][j])
                    {
                        R[i][j] = 0;
                        break;
                    }

    // The edges point from greater to lesser elements.
    wxString analysis = wxT("PARTIAL ORDER ANALYSIS\n\n");
	analysis += wxString::Format(
    	wxT("Number of elements: %d\n\n"), numVertices);
    wxString minimal, maximal, least, greatest;
    for (i = 0; i < numVertices; ++i)
    {
        bool hasLower = false, hasUpper = false;
        bool reachesAll = true, reachedByAll = true;
        for (j = 0; j < numVertices; ++j)
        {
            if (i == j) continue;
            if (reachabilityMatrix[i][j]) hasLower = true;
            if (reachabilityMatrix[j][i]) hasUpper = true;
            if (!reachabilityMatrix[i][j]) reachesAll = false;
            if (!reachabilityMatrix[j][i]) reachedByAll = false;
        }
        const wxString name = digraphNodeMap[dig.nodeFromId(i)];
        if (!hasLower) { if (!minimal.empty()) minimal += wxT(", "); minimal += name; }
        if (!hasUpper) { if (!maximal.empty()) maximal += wxT(", "); maximal += name; }
        if (reachedByAll) least = name;
        if (reachesAll) greatest = name;
    }
    analysis += wxT("Minimal elements: {") + minimal + wxT("}\n");
    analysis += wxT("Maximal elements: {") + maximal + wxT("}\n");
    analysis += wxT("Least element: ") + (least.empty() ? wxT("None") : least) + wxT("\n");
    analysis += wxT("Greatest element: ") + (greatest.empty() ? wxT("None") : greatest);
    wxMessageBox(analysis, wxT("Partial Order Analysis"), wxOK | wxICON_INFORMATION, this);

	// build the edge set for the Hasse diagram
	buildGraphEdges (R);
	// clearSetInProcess();
}

void PartialOrderDialog::buildGraphEdges (const matrix<int>& P)
{
	int i, j, numVertices;
	RelationGraphEdge e;
	Node src, dest;
	wxString srcVertex, destVertex;

	// setInProcess();
	numVertices = P.rows();

	for (i = 0; i < numVertices; i++)
	{
		src = dig.nodeFromId (i);
		srcVertex = digraphNodeMap[src];

		for (j = 0; j < numVertices; j++)
			if (P[i][j] == 1)
			{
				dest = dig.nodeFromId (j);
				destVertex = digraphNodeMap[dest];
				e.edgeSource = srcVertex;
				e.edgeDest = destVertex;
				diGraphToBuild->push_back (e);
			}
	}

	// clearSetInProcess();
}

void PartialOrderDialog::buildDivisibilityRelation (vector<int>& dataValues)
{
	int n, i, j, nextElt, currentValue;
	bool insertedSrc, insertedDest;
	Node node, node_u, node_v;
	set<int> builtVertex;
	wxString srcVertex, destVertex;
	vector<int>::iterator iter;

	// setInProcess();

    // Deduplicate input and create all nodes, including incomparable elements.
    std::sort(dataValues.begin(), dataValues.end());
    dataValues.erase(std::unique(dataValues.begin(), dataValues.end()), dataValues.end());
    for (int value : dataValues)
    {
        wxString label = wxString::Format(wxT("%d"), value);
        Node v = dig.addNode();
        digraphNodeMap[v] = label;
        wxString2DiNode[label] = v;
        builtVertex.insert(value);
    }

	// build the partial order
	n = dataValues.size();

	if ( (iter = find (dataValues.begin(), dataValues.end(), 0) ) != dataValues.end() )
	{
		dataValues.erase (iter);
		sort (dataValues.begin(), dataValues.end(), std::greater<int>() );
		dataValues.insert (dataValues.begin(), 0);
	}
	else
		// sort(dataValues.begin(), dataValues.end());
		sort (dataValues.begin(), dataValues.end(), std::greater<int>() );

	for (i = 0; i < n; i++)
	{
		currentValue = dataValues[i];

		for (j = i+1; j < n; j++)
		{
			nextElt = dataValues[j];
			insertedSrc = false;
			insertedDest = false;

			if (dataValues[j] != 0 && (currentValue % dataValues[j] ) == 0)
			{
				if (nextElt == currentValue)
					continue;

				if (builtVertex.find (currentValue) == builtVertex.end() )
				{
					builtVertex.insert (currentValue);
					node = dig.addNode();
					srcVertex.Printf (wxT ("%d"), currentValue);
					digraphNodeMap[node] = srcVertex;
					wxString2DiNode[srcVertex] = node;
					insertedSrc = true;
				}

				if (builtVertex.find (dataValues[j]) == builtVertex.end() )
				{
					builtVertex.insert (nextElt);
					node = dig.addNode();
					destVertex.Printf (wxT ("%d"), nextElt);
					digraphNodeMap[node] = destVertex;
					wxString2DiNode[destVertex] = node;
					insertedDest = true;
				}

				if (insertedSrc == false)
					srcVertex.Printf (wxT ("%d"), currentValue);

				if (insertedDest == false)
					destVertex.Printf (wxT ("%d"), dataValues[j]);

				node_u = wxString2DiNode[srcVertex];
				node_v = wxString2DiNode[destVertex];
				dig.addArc (node_u, node_v);
			}
		}
	}

	// clearSetInProcess();
}

void PartialOrderDialog::buildPowersetRelation (const vector<set<char> >& latticeVertex)
{
	Node node, node_u, node_v;
	vector<set<char> >::const_iterator iter = latticeVertex.begin(),
	                                   curriter, nextiter;
	// setInProcess();

	wxString srcVertex, destVertex;
	set<char> currentSet, nextSet;
	bool insertedSrc, insertedDest;
	set<set<char> > builtVertex;
    for (const std::set<char>& item : latticeVertex)
    {
        wxString label = makeSetNotation(item);
        Node v = dig.addNode();
        digraphNodeMap[v] = label;
        wxString2DiNode[label] = v;
        builtVertex.insert(item);
    }
	// build the partial order

	curriter = latticeVertex.begin();

	while (curriter != latticeVertex.end() )
	{
		currentSet = *curriter;
		nextiter = curriter;
		nextiter++;

		while (nextiter != latticeVertex.end() )
		{
			insertedSrc = false;
			insertedDest = false;
			nextSet = *nextiter;

			if (subset (nextSet, currentSet) )
			{
				if (builtVertex.find (currentSet) == builtVertex.end() )
				{
					builtVertex.insert (currentSet);
					srcVertex = makeSetNotation (currentSet);
					node = dig.addNode();
					digraphNodeMap[node] = srcVertex;
					wxString2DiNode[srcVertex] = node;
					insertedSrc = true;
				}

				if (builtVertex.find (nextSet) == builtVertex.end() )
				{
					builtVertex.insert (nextSet);
					destVertex = makeSetNotation (nextSet);
					node = dig.addNode();
					digraphNodeMap[node] = destVertex;
					wxString2DiNode[destVertex] = node;
					insertedDest = true;
				}

				if (insertedSrc == false)
					srcVertex = makeSetNotation (currentSet);

				if (insertedDest == false)
					destVertex = makeSetNotation (nextSet);

				node_u = wxString2DiNode[srcVertex];
				node_v = wxString2DiNode[destVertex];
				dig.addArc (node_u, node_v);
			}

			nextiter++;
		}

		curriter++;
	}
}

wxString PartialOrderDialog::makeSetNotation (const set<char>& s)
{
	int n = s.size();
	set<char>::const_iterator iter = s.begin();
	wxString notationCurrentSet;
	char c;

	if (n == 0)
		return wxT ("{}");

	notationCurrentSet = wxT ("{");

	while (iter != s.end() )
	{
		c = *iter;
		notationCurrentSet += c;
		notationCurrentSet += wxT (" ");
		iter++;
	}

	notationCurrentSet.Remove (notationCurrentSet.size()-1, 2);
	notationCurrentSet += wxT ("}");

	return notationCurrentSet;
}
