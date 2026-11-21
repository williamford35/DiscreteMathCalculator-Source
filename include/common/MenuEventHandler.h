#ifndef MENUEVENTHANDLER_H_INCLUDED
#define MENUEVENTHANDLER_H_INCLUDED

#include "common/DrawingCanvas.h"
#include <wx/event.h>

#include <string>
using namespace std;

#include "common/DrawingCanvas.h"
#include "graphs/GraphType.h"

class MenuEventHandler: public wxEvtHandler
{
public:

	MenuEventHandler (DrawingCanvas *canvas, const long idPrnt,
	                  const long idPrintPrev, const long idPgSetup, const long idSaveTo,
	                  const long idCpyImage, bool *haveSavedFigure, GraphType gtype = notagraph);
	virtual ~MenuEventHandler();
	void process (wxCommandEvent& event, int page);
	bool handleSaveAs(int typeOfSave);
	wxString getGraphicsFileName();

private:
	void handleCopyImage();

	DrawingCanvas *drawingCanvas;
	long idPrint, idPrintPreview, idPageSetup, idSaveAs, idCopyImage;
	wxString fileName;
	bool *figureSaved;
	GraphType graphType;
};

#endif // MENUEVENTHANDLER_H_INCLUDED
