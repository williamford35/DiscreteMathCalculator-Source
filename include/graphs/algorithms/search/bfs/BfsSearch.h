#ifndef BFSDFS_H
#define BFSDFS_H

//(*HeadersPCH(BfsSearch)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
//*)
//(*Headers(BfsSearch)
//*)

#include <wx/string.h>
#include <wx/msgdlg.h>
#include <graphs/GraphType.h>

class BfsSearch: public wxDialog
{
public:

	BfsSearch (wxWindow* parent, wxString *v, GraphType graphType,
	           wxWindowID id=wxID_ANY,
	           const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~BfsSearch();

	//(*Declarations(BfsSearch)
	wxTextCtrl* bfs_t_TextCtrl;
	wxStaticBitmap* bfsStaticBitmap;
	wxPanel* bfs_s_t_Panel;
	wxPanel* bfs_s_Panel;
	wxPanel* bfs_s_t_Text_Panel;
	wxButton* bfsCancelButton;
	wxPanel* buildBFSPanel;
	wxTextCtrl* bfs_s_TextCtrl;
	wxRadioButton* bfs_s_t_RadioButton;
	wxPanel* brsRunCancelPanel;
	wxTextCtrl* bfs_s_2_TextCtrl;
	wxButton* bfsRunButton;
	wxRadioButton* bfs_s_RadioButton;
	wxPanel* bfsControlPanel;
	//*)

protected:

	//(*Identifiers(BfsSearch)
	static const long ID_BFS_S_RADIOBUTTON;
	static const long ID_BFS_S_TEXTCTRL;
	static const long ID_BFS_S_PANEL;
	static const long ID_BFS_S_T_RADIOBUTTON;
	static const long ID_BFS_S_2_TEXTCTRL;
	static const long ID_BFS_T_TEXTCTRL;
	static const long ID_BFS_S_T_TEXT_PANEL;
	static const long ID_BFS_S_T_PANEL;
	static const long ID_BUILD_BFS_PANEL;
	static const long ID_BFS_STATICBITMAP;
	static const long ID_BFS_RUN_BUTTON;
	static const long ID_BFS_CANCEL_BUTTON;
	static const long ID_BFS_RUN_CANCEL_PANEL;
	static const long ID_BFS_CONTROL_PANEL;
	//*)

private:

	//(*Handlers(BfsSearch)
	void OnbfsRunButtonClick (wxCommandEvent& event);
	void OnbfsCancelButtonClick (wxCommandEvent& event);
	void Onbfs_s_RadioButtonSelect (wxCommandEvent& event);
	void Onbfs_s_t_RadioButtonSelect (wxCommandEvent& event);
	//*)

	wxString *vertex;
	GraphType graphType;

	DECLARE_EVENT_TABLE()
};

#endif
