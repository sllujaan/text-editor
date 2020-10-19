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

errno_t WindowControlsEx::createWindow(WINDOW_CONFIG& windConfig)
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        windConfig.title,    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        100, 100, (int)windConfig.width, (int)windConfig.height,

        this->_hwndParent,       // Parent window    
        NULL,       // Menu
        this->_hInst,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->_hwndParent,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return TASK_FAILURE;
    }

    return TASK_SUCCESS;
}

errno_t WindowControlsEx::centerWindow(HWND hwnd)
{
    RECT rectWindow;


    GetWindowRect(hwnd, &rectWindow);

    int nWidth = rectWindow.right - rectWindow.left;
    int nHeight = rectWindow.bottom - rectWindow.top;


    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    //screen center
    int nScreenWidthCenter = nScreenWidth / 2;
    int nScreenHeightCenter = nScreenHeight / 2;

    //center handwindow
    int nX = nScreenWidthCenter - (nWidth / 2);
    int nY = nScreenHeightCenter - (nHeight / 2);




    MoveWindow(hwnd, nX, nY, nWidth, nHeight, FALSE);

    OutputDebugStringW((LPCWSTR)L"window centered");

    return TASK_SUCCESS;
}

LRESULT WindowControlsEx::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, message, wParam, lParam);
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
    
    if (this->registerWindow() == TASK_FAILURE) return TASK_FAILURE;

    if (this->createWindow(windConfig) == TASK_FAILURE) return TASK_FAILURE;

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
    BOOL existed = GetClassInfoExA(this->_hInst, (LPCSTR)L"WINDOW", lpwcex);
    if (existed) return TASK_SUCCESS;*/



    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->wndProc;
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


    LOG_WCHAR(L"listVew regitered successfully.");
    return TASK_SUCCESS;
}
