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

errno_t WindowControlsEx::createNewWindow(WINDOW_CONFIG& windConfig)
{
	if (!this->canCreateWindow()) { this->showWindowCreationError(); return TASK_FAILURE; }
    
    if (!this->registerWindow()) return TASK_FAILURE;

	return TASK_SUCCESS;
}

errno_t WindowControlsEx::createNewWindowEx(WNDCLASSEX& wcex)
{
	return errno_t();
}

errno_t WindowControlsEx::registerWindow()
{
    /*LPWNDCLASSW wex = { };
    BOOL existed = GetClassInfo(this->_hInst, this->CLASS_NAME, wex);
    if (existed) return TASK_SUCCESS;*/

    /*LPWNDCLASSEXA lpwcex = { };
    BOOL existed = GetClassInfoExA(this->_hInst, (LPCSTR)this->CLASS_NAME, lpwcex);
    if (existed) return TASK_SUCCESS;*/

    LOG_WCHAR((LPCSTR)this->CLASS_NAME);


    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = NULL;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = this->_hInst;
    wcex.lpszClassName = this->CLASS_NAME;
    wcex.hIcon = 0;
    wcex.hIconSm = 0;

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    //RegisterClassEx(&wcex);
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            TEXT("Call to RegisterClassEx failed!"),
            TEXT("Windows Desktop Guided Tour"),
            NULL);
        return TASK_FAILURE;
    }

    return TASK_SUCCESS;
}
