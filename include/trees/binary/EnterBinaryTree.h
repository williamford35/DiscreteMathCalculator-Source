#ifndef ENTERBINARYTREE_H
#define ENTERBINARYTREE_H

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/vector.h>

struct Cell
{
	wxTextCtrl *node;
	wxRadioBox *lchild;
	wxRadioBox *rchild;

	Cell()
	{}

	Cell (wxTextCtrl *t, wxRadioBox *lch, wxRadioBox *rch) :
		node (t), lchild (lch), rchild (rch)
	{}
};

class EnterBinaryTree: public wxDialog
{
public:

	EnterBinaryTree (wxWindow* parent, wxVector<wxString> *btrLines);
	virtual ~EnterBinaryTree();

	wxButton* EnterBinaryTreeCancelButton;
	wxButton* EnterBinaryTreeBuildButton;
	wxPanel* EnterBinaryTreeDataPanel;

protected:

	static long ID_STATICTEXT[10];
	static long ID_TEXTCTRL[10];
	static long ID_RADIOBOX1[10];
	static long ID_RADIOBOX2[10];
	static const long ID_BUTTON_PANEL;
	static const long ID_ENTER_BINARY_TREE_BUILD_BUTTON;
	static const long ID_ENTER_BINARY_TREE_CANCEL_BUTTON;
	static const long ID_ENTER_BINARY_TREE_BUTTON_PANEL;

private:

	void OnEnterBinaryTreeBuildButtonClick (wxCommandEvent& event);
	void OnEnterBinaryTreeCancelButtonClick (wxCommandEvent& event);
	void addCell (int i, wxGridSizer *grid);

	wxVector<Cell> theCell;
	wxVector<wxString> *btrLinesFromDialog;

	DECLARE_EVENT_TABLE()
};

#endif
