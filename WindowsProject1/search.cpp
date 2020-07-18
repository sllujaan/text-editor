#include "search.h"


Search::Search(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;

    this->createWindow();
    this->initEditControl();
    this->initSearchButton();
}

Search::~Search()
{

}

LRESULT Search::WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CREATE:
        Search::centerWindow(hwnd);
        break;

    case WM_COMMAND:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;

    case WM_SIZE:

        OutputDebugStringW((LPCWSTR)L"WM_SIZE searcg called_______<<><><>___\r\n");
        

        MoveWindow( (HWND)lParam ,
            0, 0,                  // starting x- and y-coordinates 
            LOWORD(lParam),        // width of client area 
            HIWORD(lParam),        // height of client area 
            TRUE);                 // repaint window 
        break;

    case WM_DESTROY:
        DestroyWindow(hwnd);
        //PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }


    return 0;
}

void Search::centerWindow(HWND hwnd)
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
    int nX = rectWindow.right;
    int nY = rectWindow.bottom;




    MoveWindow(hwnd, nX, nY, nWidth, nHeight, FALSE);

    OutputDebugStringW((LPCWSTR)L"window centered");

    return;
}

void Search::initEditControl()
{
    HWND hwndEdit = CreateWindowEx(
        0, L"EDIT",   // predefined class 
        NULL,         // no window title 
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        2, 2, 200, 24,   // set size in WM_SIZE message 
        this->hWndSearch,         // parent window 
        NULL,   // edit control ID 
        (HINSTANCE)GetWindowLongPtr(this->hWndParent, GWLP_HINSTANCE),
        NULL);        // pointer not needed 


    //formating edit control..
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)this->getFont(18), TRUE);

}

void Search::initSearchButton()
{
    HWND hwndButton = CreateWindow( 
    L"BUTTON",  // Predefined class; Unicode assumed 
    L"Search",      // Button text 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
    204,         // x position 
    1,         // y position 
    80,        // Button width
    26,        // Button height
    this->hWndSearch,     // Parent window
    NULL,       // No menu.
    (HINSTANCE)GetWindowLongPtr(this->hWndSearch, GWLP_HINSTANCE),
    NULL);      // Pointer not needed.


    //formating button text..
    SendMessage(hwndButton, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
}

HFONT Search::getFont(size_t size)
{
    HFONT hFont = CreateFont(size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

    return hFont;
}

int Search::createWindow()
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->WndProcSearch;
    //wcex.cbClsExtra = 0;
    //wcex.cbWndExtra = 0;
    wcex.hInstance = this->hInst;
    wcex.lpszClassName = CLASS_NAME;

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Search",    // Window text
        WS_MINIMIZEBOX | WS_SYSMENU,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 302, 66,

        this->hWndParent,       // Parent window    
        NULL,       // Menu
        this->hInst,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->hWndParent,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return 0;
    }

    ShowWindow(hwnd, this->nCmdShowGlobal);


    //handleSettingsComboBoxWindow(hwnd);



    this->hWndSearch = hwnd;
    return 1;
}

LRESULT WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return LRESULT();
}
