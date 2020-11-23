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

errno_t WindowControlsEx::registerWindow(WNDCLASSEX& wcex)
{
    //check if window already registered---
    WNDCLASS lpwcx = {};
    BOOL exists = GetClassInfo(this->_hInst, wcex.lpszClassName, &lpwcx);
    if (exists) return TASK_SUCCESS; //{ LOG_WCHAR(L"window already exists."); }

    wstring s1(wcex.lpszClassName);
    wstring s2(L"Failed to Register the following window: ");
    s2 += s1;

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            s2.c_str(),
            TEXT("Windows Registration!"),
            MB_ICONERROR);
        return TASK_FAILURE;
    }

    return TASK_SUCCESS;
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

errno_t WindowControlsEx::applyConsistentStyle(HWND hwnd)
{
    SendMessage(hwnd, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    return TASK_SUCCESS;
}

HFONT WindowControlsEx::getFont(size_t size, LPCWSTR fontFamily, LPCWSTR fontStyle)
{
    int fontWeigh = FW_NORMAL;
    BOOL italic = FALSE;

    if (fontStyle == L"Bold") {
        fontWeigh = FW_BOLD;
    }
    else if (fontStyle == L"Italic") {
        italic = TRUE;
    }
    else if (fontStyle == L"Bold Italic") {
        fontWeigh = FW_BOLD;
        italic = TRUE;
    }
    else if (fontStyle == L"Light") {
        fontWeigh = FW_LIGHT;
    }



    HFONT hFont = CreateFont(int(size), 0, 0, 0, fontWeigh, italic, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontFamily);

    return hFont;
}

HWND WindowControlsEx::getListViewDetailsControl()
{
    // CreateListView: Creates a list-view control in report view.
    // Returns the handle to the new control
    // TO DO:  The calling procedure should determine whether the handle is NULL, in case 
    // of an error in creation.
    //
    // HINST hInst: The global handle to the applicadtion instance.
    // HWND  hWndParent: The handle to the control's parent window. 
    //

    //INITCOMMONCONTROLSEX structure ensures that the common controls DLL is loaded.
    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    // Create the list-view window in report view with label editing enabled.
    HWND hWndListView = CreateWindow(WC_LISTVIEW,
        L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
        200, 200,
        400,
        400,
        this->_hwndSelf,
        NULL,
        this->_hInst,
        NULL);

    return hWndListView;
}

errno_t WindowControlsEx::InitListViewColumns(HWND hWndListView)
{

    // InitListViewColumns: Adds columns to a list-view control.
    // hWndListView:        Handle to the list-view control. 
    // Returns TRUE if successful, and FALSE otherwise.

    WCHAR szText[2];     // Temporary buffer.
    memset(szText, 0, 2);
    szText[0] = L'A';
    szText[1] = L'\0';
    LVCOLUMN lvc;
    int iCol;

    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text,
    // and subitem members of the structure are valid.
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Add the columns.

    for (iCol = 0; iCol < 2; iCol++)
    {
        lvc.iSubItem = iCol;
        lvc.pszText = szText;
        lvc.cx = 100;               // Width of column in pixels.

        if (iCol < 2)
            lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
        else
            lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.


        // Load the names of the column headings from the string resources.
        /*LoadString(this->_hInst,
            IDS_FIRSTCOLUMN + iCol,
            szText,
            sizeof(szText) / sizeof(szText[0]));*/

        // Insert the columns into the list view.
        if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1) {
            return TASK_FAILURE;
        }
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
