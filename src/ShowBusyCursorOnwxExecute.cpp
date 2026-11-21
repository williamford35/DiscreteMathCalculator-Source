#include "ShowBusyCursorOnwxExecute.h"

ShowBusyCursorOnwxExecute::ShowBusyCursorOnwxExecute()
{}

ShowBusyCursorOnwxExecute::~ShowBusyCursorOnwxExecute()
{}

void ShowBusyCursorOnwxExecute::startBusyCursor()
{
	wxBeginBusyCursor();
}

void ShowBusyCursorOnwxExecute::stopBusyCursor()
{
	wxEndBusyCursor();
}
