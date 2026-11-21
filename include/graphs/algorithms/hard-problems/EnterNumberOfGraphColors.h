#ifndef ENTERNUMBEROFGRAPHCOLORS_H
#define ENTERNUMBEROFGRAPHCOLORS_H

//(*Headers(EnterNumberOfGraphColors)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class EnterNumberOfGraphColors: public wxDialog
{
public:

	EnterNumberOfGraphColors (wxWindow* parent,int *ncolors,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~EnterNumberOfGraphColors();

	//(*Declarations(EnterNumberOfGraphColors)
	wxPanel* GraphColoringEnterNumberOfColorsPanel;
	wxTextCtrl* GraphColoringNumberOfColorsTextCtrl;
	wxButton* GraphColoringColorButton;
	wxStaticText* GraphColoringNumberOfColorsStaticText;
	wxStaticBitmap* GraphColoringStaticBitmap;
	wxPanel* GraphColoringControlPanel;
	wxButton* GraphColoringCancelButton;
	//*)

protected:

	//(*Identifiers(EnterNumberOfGraphColors)
	static const long ID_GRAPH_COLORING_STATICBITMAP;
	static const long ID_GRAPH_COLORING_NUMBER_OF_COLORS_STATICTEXT;
	static const long ID_GRAPH_COLORING_NUMBER_OF_COLORS_TEXTCTRL;
	static const long ID_GRAPH_COLORING_ENTER_NUMBER_OF_COLORS_PANEL;
	static const long ID_GRAPH_COLORING_COLOR_BUTTON;
	static const long ID_GRAPH_COLORING_CANCEL_BUTTON;
	static const long ID_GRAPH_COLORING_CONTROL_PANEL;
	//*)

private:

	//(*Handlers(EnterNumberOfGraphColors)
	void OnGraphColoringColorButtonClick (wxCommandEvent& event);
	void OnGraphColoringCancelButtonClick (wxCommandEvent& event);
	//*)

	int *ptrNumColors;

	DECLARE_EVENT_TABLE()
};

#endif
