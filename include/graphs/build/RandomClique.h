#ifndef RANDOMCLIQUE_H
#define RANDOMCLIQUE_H

//(*Headers(RandomClique)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class RandomClique: public wxDialog
{
public:

	RandomClique (wxWindow* parent, long *cSize, long *nvert, long *nedges,
	              wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
	virtual ~RandomClique();

	//(*Declarations(RandomClique)
	wxPanel* numberOfEdgesPanel;
	wxPanel* controlPanel;
	wxTextCtrl* numberOfEdgesTextCtrl;
	wxTextCtrl* cliqueSizeTextCtrl;
	wxStaticText* numberOfEdgesStaticText;
	wxStaticText* numberOfVerticesStaticText;
	wxPanel* numberOfVerticesPanel;
	wxPanel* cliqueSizePanel;
	wxButton* buildButton;
	wxStaticText* cliqueSizeStaticText;
	wxTextCtrl* numberOfVerticesTextCtrl;
	wxStaticText* cliqueDialogStaticText;
	wxButton* cancelButton;
	//*)

protected:

	//(*Identifiers(RandomClique)
	static const long ID_CLIQUE_DIALOG_STATICTEXT;
	static const long ID_CLIQUE_SIZE_STATICTEXT;
	static const long ID_CLIQUE_SIZE_TEXTCTRL;
	static const long ID_CLIQUE_SIZE_PANEL;
	static const long ID_NUMBER_OF_VERTICES_STATICTEXT;
	static const long ID_NUMBER_OF_VERTICES_TEXTCTRL;
	static const long ID_NUMBER_OF_VERTICES_PANEL;
	static const long ID_NUMBER_OF_EDGES_STATICTEXT;
	static const long ID_NUMBER_OF_EDGES_TEXTCTRL;
	static const long ID_NUMBER_OF_EDGES_PANEL;
	static const long ID_BUILD_BUTTON;
	static const long ID_CANCEL_BUTTON;
	static const long ID_CONTROL_PANEL;
	//*)

private:

	enum TextControlType
	{
		CLIQUE_SIZE,
		NUMBER_OF_VERTICES,
		NUMBER_OF_EDGES
	};

	//(*Handlers(RandomClique)
	void OnBuildRandomClique (wxCommandEvent& event);
	void OncancelButtonClick (wxCommandEvent& event);
	//*)

	bool checkValue (long& n, TextControlType type);

	long *cliqueSize, *numberOfVertices, *numberOfEdges;

	DECLARE_EVENT_TABLE()
};

#endif
