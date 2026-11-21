/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include "common/MenuEventHandler.h"
#include "DiscreteMathCalculatorMain.h"

#include <wx/string.h>
#include <wx/filedlg.h>

MenuEventHandler::MenuEventHandler (DrawingCanvas *canvas, const long idPrnt,
                                    const long idPrintPrev, const long idPgSetup, const long idSaveTo,
                                    const long idCpyImage, bool *haveSavedFigure,
                                    GraphType gtype) :
	drawingCanvas (canvas), idPrint (idPrnt),idPrintPreview (idPrintPrev),
	idPageSetup (idPgSetup), idSaveAs (idSaveTo),
	idCopyImage (idCpyImage), figureSaved (haveSavedFigure), graphType (gtype)
{
}

MenuEventHandler::~MenuEventHandler()
{
}

void MenuEventHandler::process (wxCommandEvent& event,int page)
{
	int id = event.GetId();
	wxPrintData* g_printData = new wxPrintData;

	cout << "In MenuEventHandler::process: page = " << endl;
	if (id == idPrint)
	{
		g_printData->SetPaperId (wxPAPER_LETTER);
		g_printData->SetOrientation (wxLANDSCAPE);
		drawingCanvas->SetPrintMode (wxSFShapeCanvas::prnMAP_TO_PAGE);
		drawingCanvas->Print (true);
	}
	else if (id == idPrintPreview)
	{
		g_printData->SetPaperId (wxPAPER_LETTER);
		g_printData->SetOrientation (wxLANDSCAPE);
		drawingCanvas->SetPrintMode (wxSFShapeCanvas::prnFIT_TO_PAGE);
		drawingCanvas->PrintPreview();
	}
	else if (id == idPageSetup)
	{
		g_printData->SetPaperId (wxPAPER_LETTER);
		drawingCanvas->SetPrintMode (wxSFShapeCanvas::prnMAP_TO_PAGE);
		drawingCanvas->PageSetup();
	}
	else if (id == idSaveAs)
		handleSaveAs(page);
	else
		drawingCanvas->copyToClipboard();

	delete g_printData;
}

bool MenuEventHandler::handleSaveAs(int page)
{
	wxString formatSelection;

	// common to all save formats
    // "BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif"
	if (page != 3)
		formatSelection.Append ("GML files (*.gml)|*.gml|" );
	formatSelection.Append ("PNG file (*.png)|*.png|" );
	formatSelection.Append ("BMP file (*.bmp)|*.bmp|" );
	formatSelection.Append ("JPG file (*.jpg)|*.jpg" );
	// formatSelection.Append ("TIFF file (*.tiff)|*.tiff" );

	wxString filename = wxFileSelector ("Select a format.",wxEmptyString, wxEmptyString,wxEmptyString, formatSelection,
	                                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (filename.empty() )
		return false;

	wxString extension = filename.AfterLast ('.');

	if (extension == wxT(""))
	{
		//  extension not specified
		wxMessageBox (wxT ("An extension must be added to the file name."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
	}

	extension = extension.MakeLower();
	fileName = wxT ("");

	if (extension == wxT ("gml") )
	{
		drawingCanvas->handleSaveAsGML (filename, graphType);
		fileName = filename;
	}
	else if (extension == wxT("bmp"))
		drawingCanvas->SaveCanvasToGraphicFormat (DrawingCanvas::BMP, filename);
	else if (extension == wxT("jpg"))
		drawingCanvas->SaveCanvasToGraphicFormat (DrawingCanvas::JPG, filename);
	else if (extension == wxT ("png") )
		drawingCanvas->SaveCanvasToGraphicFormat (DrawingCanvas::PNG, filename);
	else
		/*if (extension == wxT("tiff"))
			drawingCanvas->SaveCanvasToGraphicFormat (DrawingCanvas::TIFF, filename);
		else*/
	{
		wxMessageBox (wxT ("Unsupported graphics file format."), wxT ("Discrete Mathematics Calculator"), wxOK | wxICON_WARNING);
		return false;
	}

	*figureSaved = true;
	return true;
}

void MenuEventHandler::handleCopyImage()
{
	drawingCanvas->copyToClipboard();
}

wxString MenuEventHandler::getGraphicsFileName()
{
	return fileName;
}
