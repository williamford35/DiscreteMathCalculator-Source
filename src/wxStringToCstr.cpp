/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */
#include "wxStringToCstr.h"

const char* wxStringToCstr (const wxString& s)
{
	return (const char*) s.mb_str (wxConvUTF8);
}
