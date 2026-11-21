#ifndef SHOWSEQUENCE_H
#define SHOWSEQUENCE_H

//(*Headers(ShowSequence)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)

class ShowSequence: public wxFrame
{
public:

	enum SequenceType
	{
		BFS,
		DFS,
		TSP,
		TOPOLOGICALSORT,
		CARDMATCHING,
		WEIGHTEDMATCHING,
		PERMUTATIONS,
		GENERALINFORMATION
	};

	ShowSequence (wxWindow* parent,const char *imageFileName,SequenceType seqtype,
	              wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,
	              const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);
	virtual ~ShowSequence();

	//(*Declarations(ShowSequence)
	wxButton* ShowSequenceDisplayCopyButton;
	wxPanel* ShowSequenceDisplayMainPanel;
	wxStaticText* ShowSequenceDisplayStaticText;
	wxStaticBitmap* ShowSequenceDisplayStaticBitmap;
	wxButton* ShowSequenceDisplayCloseButton;
	wxTextCtrl* ShowSequenceDisplayTextCtrl;
	wxPanel* ShowSequenceDisplayControlPanel;
	//*)
	void WriteLine (const wxString& s);

protected:

	//(*Identifiers(ShowSequence)
	static const long ID_SHOW_SEQUENCE_STATICBITMAP;
	static const long ID_SHOW_SEQUENCE_STATICTEXT;
	static const long ID_SHOW_SEQUENCE_TEXTCTRL;
	static const long ID_SHOW_SEQUENCE_COPY_BUTTON;
	static const long ID_SHOW_SEQUENCE_CLOSE_BUTTON;
	static const long ID_SHOW_SEQUENCE_CONTROL_PANEL;
	static const long ID_SHOW_SEQUENCE_MAIN_PANEL;
	//*)

private:

	//(*Handlers(ShowSequence)
	void OnShowSequenceCopyButtonClick (wxCommandEvent& event);
	void OnShowSequenceCloseButtonClick (wxCommandEvent& event);
	//*)

	DECLARE_EVENT_TABLE()
};

#endif  // SHOWSEQUENCE_H
