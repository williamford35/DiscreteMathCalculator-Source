#ifndef LONGESTPATHINPUT_H
#define LONGESTPATHINPUT_H

//(*Headers(DijkstraInput)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
//*)

#include <wx/string.h>
#include <wx/msgdlg.h>

class LongestPathInput: public wxDialog
{
public:

	LongestPathInput (wxWindow* parent,wxString *s, wxString *t, wxWindowID id=wxID_ANY);
	virtual ~LongestPathInput();

	//(*Declarations(LongestPathInput)
	wxTextCtrl* LongestPathStartingVertexTextCtrl;
	wxPanel* LongestPathVerticesPanel;
	wxButton* LongestPathCancelButton;
	wxPanel* LongestPathPanel;
	wxStaticText* LongestPathFinalVertexStaticText2;
	wxButton* LongestPathExecuteButton;
	wxTextCtrl* LongestPathFinalVertexTextCtrl;
	wxStaticBitmap* LongestPathStaticBitmap;
	wxPanel* LongestPathButtonsPanel;
	wxPanel* LongestPathFinalVertexPanel;
	wxPanel* LongestPathStartingVertexPanel;
	wxStaticText* LongestPathStartingVertexStaticText;
	//*)

protected:

	//(*Identifiers(LongestPathInput)
	static const long ID_LONGESTPATH_STATICBITMAP;
	static const long ID_LONGESTPATH_STARTING_VERTEX_STATICTEXT;
	static const long ID_LONGESTPATH_STARTING_VERTEX_TEXTCTRL;
	static const long ID_LONGESTPATH_STARTING_VERTEX_PANEL;
	static const long ID_LONGESTPATH_STARGING_VERTEX_STATICTEXT;
	static const long ID_LONGESTPATH_FINAL_VERTEX_TEXTCTRL;
	static const long ID_LONGESTPATH_FINAL_VERTEX_PANEL;
	static const long ID_LONGESTPATH_VERTICES_PANEL;
	static const long ID_LONGESTPATH_EXECUTE_BUTTON;
	static const long ID_LONGESTPATH_CANCEL_BUTTON;
	static const long ID_LONGESTPATH_BUTTONS_PANEL;
	static const long ID_LONGESTPATH_PANEL;
	//*)

private:

	//(*Handlers(LongestPathInput)
	void OnLongestPathExecuteButtonClick (wxCommandEvent& event);
	void OnLongestPathCancelButtonClick (wxCommandEvent& event);
	//*)
	wxString *startingVertex, *finalVertex;

	DECLARE_EVENT_TABLE()
};

#endif // LONGESTPATHINPUT_H
