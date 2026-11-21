#ifndef ENTERGENERALTREE2_H
#define ENTERGENERALTREE2_H

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/vector.h>

struct NodeEntry
{
	wxTextCtrl *labelTextCtrl;
	wxTextCtrl *parentIndexTextCtrl;

	NodeEntry()
	{}

	NodeEntry (wxTextCtrl *label, wxTextCtrl *parentIndex) :
		labelTextCtrl (label), parentIndexTextCtrl (parentIndex)
	{}
};

class EnterGeneralTree: public wxDialog
{
public:

	EnterGeneralTree (wxWindow* parent, wxVector<wxString> *gtrLines);
	virtual ~EnterGeneralTree();

	wxPanel* FillerPanel3;
	wxPanel* FillerPanel2;
	wxButton* BuildButton;
	wxPanel* ButtonPanel;
	wxPanel* FillerPanel1;
	wxButton* CancelButton;

protected:

	static long ID_NODE_PANEL[16];
	static long ID_NODE_NUMBER_STATIC_TEXT[16];
	static long ID_NODE_NAME_TEXTCTRL[16];
	static long ID_PARENT_INDEX_TEXTCTRL[16];
	static const long ID_NODE_NAME_STATICTEXT;
	static const long ID_PARENT_INDEX_STATICTEXT;
	static const long ID_FILLER_PANEL1;
	static const long ID_FILLER_PANEL2;
	static const long ID_FILLER_PANEL3;
	static const long ID_GENERAL_TREE_BUILD_BUTTON;
	static const long ID_GENERAL_TREE_CANCEL_BUTTON;
	static const long ID_GENERAL_TREE_BUTTON_PANEL;

private:

	void OnGeneralTreeBuildButtonClick (wxCommandEvent& event);
	void OnGeneralTreeCancelButtonClick (wxCommandEvent& event);
	void makeColumnLabels (int i, wxGridSizer *grid);
	void addCell (int i, wxGridSizer *grid);
	void reportError (const wxString& errMsg);

	wxVector<NodeEntry> nodeEntry;
	wxVector<wxString> *gtrLinesFromDialog;

	DECLARE_EVENT_TABLE()
};

#endif
