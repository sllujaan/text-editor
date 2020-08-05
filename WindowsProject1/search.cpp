#include "search.h"


Search::Search(HWND hWnd, HINSTANCE hInstance, int nCmdShow, HWND richEdit)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;
    this->hwndRichEditParent = richEdit;

    this->createWindow();
    this->initEditControl();
    this->initSearchButton();


}

Search::~Search()
{
    MessageBox(NULL,
        TEXT("~Search"),
        TEXT("~Search"),
        NULL);
}

LRESULT Search::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CREATE:
        this->centerWindow(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDI_SEARCH_BUTTON:
            this->handleSearchText();
            break;
        default:
            break;
        }

        switch (HIWORD(wParam))
        {
        case EN_CHANGE:
            this->handleEnableDisableSearchButton();
            break;
        default:
            break;
        }

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

    //  Call the original window procedure for default processing. 
    //return CallWindowProc(this->lpfnMainWndProc, hwnd, message, wParam, lParam);
    return 0;
    //return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Search::WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    Search* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<Search*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<Search*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

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
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
        2, 2, 200, 24,   // set size in WM_SIZE message 
        this->hWndSearch,         // parent window 
        NULL,   // edit control ID 
        (HINSTANCE)GetWindowLongPtr(this->hWndParent, GWLP_HINSTANCE),
        NULL);        // pointer not needed 


    
    this->hWndEditControl = hwndEdit;

    //formating edit control..
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)this->getFont(18), TRUE);

    SetFocus(hWndEditControl);

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
    (HMENU)IDI_SEARCH_BUTTON,       // No menu.
    (HINSTANCE)GetWindowLongPtr(this->hWndSearch, GWLP_HINSTANCE),
    NULL);      // Pointer not needed.


    //formating button text..
    SendMessage(hwndButton, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);

    this->hwndButton = hwndButton;

    EnableWindow(hwndButton, FALSE);
}

HFONT Search::getFont(size_t size)
{
    HFONT hFont = CreateFont(int(size), 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

    return hFont;
}

void Search::handleEnableDisableSearchButton()
{
    int length = GetWindowTextLength(this->hWndEditControl);
    if(length == 0) EnableWindow(this->hwndButton, FALSE);
    else EnableWindow(this->hwndButton, TRUE);
}

void Search::handleSearchText()
{
    //get edit window length--
    const int size = GetWindowTextLength(this->hWndEditControl);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(this->hWndEditControl, (LPWSTR)text, size + 1);

    //SendMessage(this->hwndRichEditParent, EM_SETSEL, 0, -1);

    
    //CHARRANGE selectionRange;

    //SendMessage(this->hwndRichEditParent, EM_EXGETSEL, 0, (LPARAM)&selectionRange);
    
    FINDTEXTEX ftex;
    
    ftex.chrg.cpMin = 0;
    ftex.chrg.cpMax = -1; //-1 means select entire text
    ftex.lpstrText = text;

    LRESULT lr = SendMessage(this->hwndRichEditParent, EM_FINDTEXTEXW, (WPARAM)FR_DOWN, (LPARAM)&ftex);
    
    if (lr >= 0)
    {
        SendMessage(this->hwndRichEditParent, EM_EXSETSEL, 0, (LPARAM)&ftex.chrgText);

        SendMessage(this->hwndRichEditParent, EM_HIDESELECTION, (LPARAM)FALSE, 0);
        
        //SetFocus(this->hWndParent);
    }
    else {
        MessageBox(
            this->hWndSearch,
            (LPCWSTR)L"Not found.",
            (LPCWSTR)L"Search Result",
            MB_OK
        );
    }
    
    

}

void Search::createWindow()
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Search Window";

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

    //RegisterClassEx(&wcex);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            TEXT("Call to RegisterClassEx failed!"),
            TEXT("Windows Desktop Guided Tour"),
            NULL);
    }


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
        this        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->hWndParent,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return;
    }

    ShowWindow(hwnd, this->nCmdShowGlobal);


    //handleSettingsComboBoxWindow(hwnd);



    this->hWndSearch = hwnd;
}

LRESULT WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return LRESULT();
}
