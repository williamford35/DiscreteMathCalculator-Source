/***************************************************************
 * Name:      CellArrayTestMain.h
 * Purpose:   Defines Application Frame
 * Author:    William Ford (wford@pacific.edu)
 * Created:   2009-06-16
 * Copyright: William Ford (http://www1.pacific.edu/~wford)
 * License:
 **************************************************************/

#ifndef ENTERHUFFMANDATA_H
#define ENTERHUFFMANDATA_H

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "trees/huffman/HuffmanNode.h"
#include "trees/huffman/HuffmanTree.h"

class EnterHuffmanData: public wxDialog
{
public:

	EnterHuffmanData (wxWindow* parent, int id, vector<HuffmanNode> *hnodes);
	virtual ~EnterHuffmanData();

private:

	//(*Handlers(CellArrayTestDialog)
	void OnBuildButtonClick (wxCommandEvent& event);
	void OnCancelButtonClick (wxCommandEvent& event);
	//*)

	static long ID_PANEL[26];
	static long ID_STATICTEXT[26];
	static long ID_TEXTCTRL[26];
	static const long ID_BUTTON_PANEL;
	static const long ID_BUILD_BUTTON;
	static const long ID_CANCEL_BUTTON;
	//*)

	wxTextCtrl *freq[26];
	wxPanel *makeCell (wxTextCtrl *freq[], int i);
	vector<HuffmanNode> *huffmanNodes;

	DECLARE_EVENT_TABLE()
};

#endif // ENTERHUFFMANDATA_H
