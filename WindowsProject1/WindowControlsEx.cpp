#include "WindowControlsEx.h"

BOOL WindowControlsEx::canCreateWindow()
{
	if (!this) return FALSE;
	if (this->_hwndParent == NULL || this->_hInst == NULL) return FALSE;
	return TRUE;
}

void WindowControlsEx::showWindowCreationError()
{
	MessageBox(NULL,
		(LPCWSTR)L"Window Creation Failed! Either HWND or HINSTANCE is NULL.\r\n\r\nConsider initializing the following constructor:\r\n\tWindowControlsEx(HWND hwnd, int nCmdShow)",
		(LPCWSTR)L"WindowControls",
		MB_ICONERROR);
}

WindowControlsEx::WindowControlsEx(HWND hwnd, int nCmdShow)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtrA(hwnd, GWLP_HINSTANCE);
	if (hInst == NULL) LOG_WCHAR(L"WindowControlsEx HINSTANCE is NULL");
	this->_hInst = hInst;
	this->_hwndParent = hwnd;
	this->_nCmd = nCmdShow;
}

errno_t WindowControlsEx::createNewWindow()
{
	if (!this->canCreateWindow()) { this->showWindowCreationError(); return TASK_FAILURE; }
	
	return TASK_SUCCESS;
}
