/////////////////////////////////////////////////////////////////////////////
// Name:        DiscreteMathCalculatorApp.h
// Purpose:
// Author:      William Ford (wford333@gmail.com)
// Created:
// Copyright:   (c) 2023 William Ford
// Licence:     GNU General Public Licence version 3
/////////////////////////////////////////////////////////////////////////////

#ifndef DISCRETEMATHCALCULATORAPP_H
#define DISCRETEMATHCALCULATORAPP_H

#include <wx/app.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/string.h>

class DiscreteMathCalculatorApp : public wxApp
{
public:
	virtual bool OnInit();
	int OnExit();
#ifdef __LINUX__
	static bool exe_name (char *str);
#endif

private:
	void reportError (const wxString& errmsg);
};

#endif // DISCRETEMATHCALCULATORAPP_H
