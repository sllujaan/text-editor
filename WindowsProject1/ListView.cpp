#include "ListView.h"

LRESULT ListView::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HCURSOR hCurs1;
    PAINTSTRUCT ps;
    HDC hdc = NULL;
    RECT _rect;
    _rect.left = 200;
    _rect.top = 200;
    _rect.right = 300;
    _rect.bottom = 300;

    switch (message)
    {
    case WM_CREATE:
        hCurs1 = LoadCursor(NULL, IDC_ARROW);
        SetCursor(hCurs1);
        break;
    case WM_COMMAND:
        break;
    case WM_LBUTTONDOWN:
        LOG_WCHAR(L"WM_LBUTTONDOWN");
        //MoveWindow(this->_hwndStatic, 10, 10, 50, 50, TRUE);
        /*hdc = BeginPaint(this->_hwndSelf, &ps);
        SetBkColor(hdc, RGB(3, 3, 3));
        EndPaint(this->_hwndSelf, &ps);*/
        break;
    case WM_KEYUP:
        SetCapture(this->_hwndStatic);
        hCurs1 = LoadCursor(NULL, IDC_WAIT);
        SetCursor(hCurs1);
        break;
    case WM_MOUSEMOVE:
        LOG_WCHAR(L"WM_MOUSEMOVE");
        //this->handleMouseCursor();
        this->handleResizer();
        this->resizeStatic();
        break;
    case WM_SETCURSOR:
        // Create a standard hourglass cursor.
        break;

    case WM_PAINT:
        
        
        hdc = BeginPaint(this->_hwndSelf, &ps);
        // All painting occurs here, between BeginPaintand EndPaint.
        //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 3));
        //FillRect(hdc, &_rect, (HBRUSH)(COLOR_WINDOW + 3));
        //DrawText(hdc, L"some text", 9, &_rect, DT_CENTER);
        //TextOut(hdc, 200, 200, L"Hello, Windows!", 15);
        //SetRect()
        SetRect(&_rect, 0, 0, 100, 100);
        EndPaint(this->_hwndSelf, &ps);

        break;
    default:
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT ListView::wndProcListView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ListView* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<ListView*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<ListView*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    }

    if (pThis) {
        // Now that we have recovered our "this" pointer, let the
        // member function finish the job.
        return pThis->runProc(hwnd, message, wParam, lParam);
    }

    // We don't know what our "this" pointer is, so just do the default
    // thing. Hopefully, we didn't need to customize the behavior yet.

    return DefWindowProc(hwnd, message, wParam, lParam);
}

ListView::ListView(HWND hwnd, int nCmdShow) : WindowControlsEx(hwnd, nCmdShow)
{

}

errno_t ListView::initWindow()
{
    this->_createWindow();
    this->handleStaticWindows();

    return TASK_SUCCESS;
}

errno_t ListView::_createWindow()
{
	if (!this->canCreateWindow()) { this->showWindowCreationError(); return TASK_FAILURE; }
	
    if (this->registerWindow(*this->getWindowClass()) == TASK_FAILURE) return TASK_FAILURE;

    //if (this->createWindow(wndConfig) == TASK_FAILURE) return TASK_FAILURE;
    this->_hwndSelf = this->getListViewWindow();
    if (this->_hwndSelf == NULL) return TASK_FAILURE;

    return TASK_SUCCESS;
}

HWND ListView::getStaticWindow(LPCWSTR text, size_t posX, size_t posY)
{
    HWND hwndStatic = CreateWindowEx(0,
        L"STATIC",  // Predefined class; Unicode assumed 
        text,      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,  // Styles 
        (int)posX,         // x position 
        (int)posY,         // y position 
100,        // Button width
100,        // Button height
this->_hwndSelf,     // Parent window
NULL,       // No menu.
(HINSTANCE)GetWindowLongPtr(_hwndSelf, GWLP_HINSTANCE),
NULL);      // Pointer not needed.

this->applyConsistentStyle(hwndStatic);



return hwndStatic;
}

void ListView::handleMouseCursor()
{
    POINT curPos = {};
    GetCursorPos(&curPos);

    /*LOG_INT(curPos.x);
    LOG_INT(curPos.y);*/


    RECT rectStatic;

    GetWindowRect(this->_hwndStatic, &rectStatic);

    BOOL curOnStatic_X = (curPos.x > rectStatic.left) && (curPos.x < rectStatic.right) ? TRUE : FALSE;
    BOOL curOnStatic_Y = (curPos.y > rectStatic.top) && (curPos.y < rectStatic.bottom) ? TRUE : FALSE;
    BOOL curOnStatic = (curOnStatic_X && curOnStatic_Y) ? TRUE : FALSE;

    /*LOG_INT(curPos.x);
    LOG_INT(rectStatic.left);*/

    HCURSOR hCurs;

    if (curOnStatic) {
        LOG_WCHAR(L"on static");

        if (!this->mouseOnStatic) {
            this->mouseOnStatic = TRUE;


            // Create a standard hourglass cursor.
            hCurs = LoadCursor(NULL, IDC_WAIT);
            SetCursor(hCurs);
        }

    }
    else {
        LOG_WCHAR(L"static not");
        this->mouseOnStatic = FALSE;
        hCurs = LoadCursor(NULL, IDC_ARROW);
        SetCursor(hCurs);
    }



}

void ListView::handleResizer()
{
    POINT curPos = {};
    GetCursorPos(&curPos);

    RECT rectStatic;

    GetWindowRect(this->_hwndStatic, &rectStatic);

    BOOL isResizeArea = ((curPos.x < rectStatic.right) && (rectStatic.right - curPos.x) < 12) ? TRUE : FALSE;
    //BOOL curOnStatic_Y = (curPos.y > rectStatic.top) && (curPos.y < rectStatic.bottom) ? TRUE : FALSE;
    //BOOL curOnStatic = (curOnStatic_X && curOnStatic_Y) ? TRUE : FALSE;

    /*LOG_INT(curPos.x);
    LOG_INT(rectStatic.left);*/

    HCURSOR hCurs;

    if (isResizeArea) {

        if (!this->mouseOnStatic) {
            this->mouseOnStatic = TRUE;


            // Create a standard hourglass cursor.
            hCurs = LoadCursor(NULL, IDC_SIZEWE);
            SetCursor(hCurs);
        }

    }
    else {
        this->mouseOnStatic = FALSE;
        hCurs = LoadCursor(NULL, IDC_ARROW);
        SetCursor(hCurs);
    }

}

void ListView::resizeStatic()
{
    if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)  {

        POINT curPos = {};
        RECT rectStatic;
        GetCursorPos(&curPos);
        GetWindowRect(this->_hwndStatic, &rectStatic);

        INT newWidth = curPos.x - rectStatic.left;
        LOG_INT(newWidth);

        if (newWidth < 100) return;

        MoveWindow(this->_hwndStatic, 10, 10, newWidth+5, 100, TRUE);
        LOG_WCHAR(L"resizing......");
    }
}

void ListView::handleChangeJournals()
{
    HANDLE hVol = NULL;
    CreateFile(L"\\\\.\\C:\\Users\\SALMAN-ALTAF\\Desktop\\myWatchDir",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hVol == INVALID_HANDLE_VALUE)
    {
        LOG_WCHAR(L"CreateFile failed XXXXXXX");
        return;
    }
}

WNDCLASSEX* ListView::getWindowClass()
{
	WNDCLASSEX* wcex = new WNDCLASSEX();

    wcex->cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex->lpfnWndProc = this->wndProcListView;
    wcex->cbClsExtra = 0;
    wcex->cbWndExtra = 0;
    wcex->hInstance = this->_hInst;
    wcex->lpszClassName = this->CLASS_NAME;
    wcex->hIcon = 0;
    wcex->hCursor = NULL;   // LoadCursor(NULL, IDC_SIZEWE);
    wcex->hIconSm = 0;

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex->hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	return wcex;
}

errno_t ListView::handleStaticWindows()
{
    HWND _hwndStatic1 = this->getStaticWindow(L"static 1", 10, 10);
    this->_hwndStatic = _hwndStatic1;
    return TASK_SUCCESS;
}

HWND ListView::getListViewWindow()
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        L"List View",    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        100, 100, 400, 400,

        this->_hwndParent,       // Parent window    
        NULL,       // Menu
        this->_hInst,  // Instance handle
        this        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->_hwndParent,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return NULL;
    }

    return hwnd;
}