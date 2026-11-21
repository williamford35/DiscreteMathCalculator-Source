#ifndef GRAPHEDGESTYLES_H_INCLUDED
#define GRAPHEDGESTYLES_H_INCLUDED

#include <wx/pen.h>
#include <wx/colour.h>

const wxPen wxNORMAL_PEN = wxPen (*wxBLACK, 1, wxSOLID);
const wxPen wxTREE_EDGE_PEN = wxPen (*wxBLACK, 3, wxSOLID);
const wxPen wxBACK_EDGE_PEN = wxPen (*wxBLACK, 2, wxDOT);
const wxPen wxCROSS_EDGE_PEN = wxPen (wxColor (135,61,141), 2, wxSHORT_DASH);
const wxPen wxNOFLOW_EDGE_PEN = wxPen (*wxBLACK, 2, wxSHORT_DASH);
const wxPen wxFORWARD_EDGE_PEN = wxPen (*wxBLUE, 2, wxDOT_DASH);
const wxPen wxwxDASHED_GRAPH_EDGE_PEN = wxPen (*wxBLACK,1,wxPENSTYLE_DOT);
const wxPen wxSHORTEST_PATH_PEN = wxPen (wxColor(153,153,255),3,wxSOLID);
const wxPen wxHIDE_PEN = wxPen (*wxBLACK,10,wxSOLID);
const wxPen HAMILTONIAN_PATH_PEN = wxPen (wxColor (90,0,0),3,wxSOLID);

#endif // GRAPHEDGESTYLES_H_INCLUDED
