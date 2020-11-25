#include "AppLog.h"

void LOG_LAST_ERROR()
{
	DWORD errorMessageID = GetLastError();

	if (errorMessageID == 0) { LOG_WCHAR(L"No Recent Error was recorded."); return; }

	LPTSTR errorText = NULL;

	FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		NULL,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 

	if(errorText == NULL) { LOG_WCHAR(L"FormatMessage Failed."); return; }

	LOG_WCHAR(errorText);
	// release memory allocated by FormatMessage()
	LocalFree(errorText);
	return;
}
