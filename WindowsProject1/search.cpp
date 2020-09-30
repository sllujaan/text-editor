#include "search.h"


Search::Search(HWND hWnd, HINSTANCE hInstance, int nCmdShow, HWND richEdit) : WindowControls(NULL, hInstance)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;
    this->hwndRichEditParent = richEdit;
}

void Search::initWindow()
{
    this->createWindow();
    this->initEditControl();
    this->initSearchButton();

    this->initRadioBtns();
    //this->initTabCtrl();
}

Search::~Search()
{
    MessageBox(NULL,
        TEXT("~Search"),
        TEXT("~Search"),
        NULL);
}

void Search::registerWindow()
{
    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->WndProcSearch;
    //wcex.cbClsExtra = 0;
    //wcex.cbWndExtra = 0;
    wcex.hInstance = this->hInst;
    wcex.lpszClassName = this->CLASS_NAME;

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
    }

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

    case WM_NOTIFY:
        OnNotify(hwnd, message, wParam, lParam);
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

    case WM_KEYDOWN:
        /*switch (wParam)
        {
        case VK_RETURN:
            this->handleSearchText();
            break;
        default:
            break;
        }
        OutputDebugStringW((LPCWSTR)L"_WM_KEYDOWN searchlll_\r\n");*/
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
        this->hWndSearch = NULL;
        
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

LRESULT Search::searchEdit_wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Search* pThis;

    // Recover the "this" pointer stored in hwnd.
    pThis = reinterpret_cast<Search*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {

    case WM_KEYUP:
        LOG_WCHAR(L"search key up-->");
        switch (wParam)
        {
        case VK_RETURN:
            LOG_WCHAR(L"VK_RETURN");
            if(pThis->canSearch()) pThis->handleSearchText();
            break;
        default:
            break;
        }
        break;

    default:
        return CallWindowProc(pThis->searchEdit_oldProc, hwnd, message, wParam, lParam);
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
    HWND hwndEdit = this->getEditControl(10, 10, 200, 20);   
    this->hWndEditControl = hwndEdit;
    SetFocus(hWndEditControl);

    // Put the value in a safe place for future use
    SetWindowLongPtr(this->hWndEditControl, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    //register new procedure
    this->searchEdit_oldProc = (WNDPROC)SetWindowLongPtr(this->hWndEditControl, GWLP_WNDPROC, (LONG_PTR)this->searchEdit_wndProc);

}

void Search::initSearchButton()
{
    //HWND hwndButton = CreateWindow( 
    //L"BUTTON",  // Predefined class; Unicode assumed 
    //L"Search",      // Button text 
    //WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
    //204,         // x position 
    //1,         // y position 
    //80,        // Button width
    //26,        // Button height
    //this->hWndSearch,     // Parent window
    //(HMENU)IDI_SEARCH_BUTTON,       // No menu.
    //(HINSTANCE)GetWindowLongPtr(this->hWndSearch, GWLP_HINSTANCE),
    //NULL);      // Pointer not needed.


    //formating button text..
    //SendMessage(hwndButton, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);


    HWND hwndButton = this->getButton(this->hWndSearch, L"Search", (short)IDI_SEARCH_BUTTON, 214, 8);


    this->hwndButton = hwndButton;

    EnableWindow(hwndButton, FALSE);
}

void Search::initTabCtrl()
{
    TCITEM tie;
      // Temporary buffer for strings.

    HWND hwndTabCtrl = this->getTabControl(10, 200, 300, 200);

    if (!hwndTabCtrl) return;

    // Add tabs for each day of the week. 
    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    //tie.pszText = (LPWSTR)achTemp[0];



    // Load the day string from the string resources. Note that
    for (int i = 0; i < ARRAYSIZE(this->DAYS); i++) {

        //issue: tab control adds tabs in reverse order
        //therefore get the last array index in order to receive right array index.
        size_t lastIndex = ARRAYSIZE(this->DAYS) - i - 1;

        tie.pszText = (LPWSTR)this->DAYS[lastIndex];
        if (TabCtrl_InsertItem(hwndTabCtrl, 0, &tie) == -1)
        {
            DestroyWindow(hwndTabCtrl);
            LOG_WCHAR(L"tab insertItem failed.");
            return;
        }
    }

    HWND hwndStaticCtrl = this->DoCreateDisplayWindow(hwndTabCtrl);

    this->_hwndTabCtrl = hwndTabCtrl;
    this->_hwndTabDisp = hwndStaticCtrl;


    //for (int i = 0; i < DAYS_IN_WEEK; i++)
  //{
  //    // Load the day string from the string resources. Note that
  //    // g_hInst is the global instance handle.
  //    LoadString(g_hInst, IDS_SUNDAY + i,
  //        achTemp, sizeof(achTemp) / sizeof(achTemp[0]));
  //    if (TabCtrl_InsertItem(hwndTab, i, &tie) == -1)
  //    {
  //        DestroyWindow(hwndTab);
  //        return NULL;
  //    }
  //}

}

void Search::initRadioBtns()
{
    
    HWND groupBox = this->getGroupBox(this->hWndSearch, L"Direction", 10, 40, 284, 50);

    HWND hwndButton1 = this->getRadioButton(this->hWndSearch, L"Forward", 12, 30, 60); //height is 17 pixels
    HWND hwndButton2 = this->getRadioButton(this->hWndSearch, L"Back", 14, 110, 60);

    this->_hwndRadio_forward = hwndButton1;
    this->_hwndRadio_back = hwndButton2;

    SendMessage(this->_hwndRadio_forward, BM_SETCHECK, BST_CHECKED, 0);
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

    //rich edit lenght----
    const int richEditLen = GetWindowTextLength(this->hwndRichEditParent);

    
    CHARRANGE selectionRange;
    //check which direction to search forward or backward.
    LONG searchStartIndex = 0;
    //also define variable for EM_FINDTEXTEXW to when search backward or forward
    LONG searchDirection = 0;

    LRESULT radioState_forward = SendMessage(this->_hwndRadio_forward, BM_GETSTATE, 0, 0);
    if (radioState_forward == BST_CHECKED) {
        LOG_WCHAR(L"forward is checked");
        searchStartIndex = this->forwardStartIndex;
        searchDirection = (LONG)FR_DOWN;
    }
    else {
        LOG_WCHAR(L"back is checked");
        
        //LOG_INT(richEditLen);
        if(this->backStartIndex == 0) this->backStartIndex = richEditLen;
        searchStartIndex = this->backStartIndex;
        searchDirection = 0;
    }

    //LOG_INT(this->backStartIndex);

    FINDTEXTEX ftex;
    
    ftex.chrg.cpMin = searchStartIndex;
    ftex.chrg.cpMax = -1; //-1 means select entire text
    ftex.lpstrText = text;

    LRESULT lr = SendMessage(this->hwndRichEditParent, EM_FINDTEXTEXW, (WPARAM)searchDirection, (LPARAM)&ftex);
    
    if (lr >= 0)
    {
        SendMessage(this->hwndRichEditParent, EM_EXSETSEL, 0, (LPARAM)&ftex.chrgText);

        SendMessage(this->hwndRichEditParent, EM_HIDESELECTION, (LPARAM)FALSE, 0); 

        SendMessage(this->hwndRichEditParent, EM_EXGETSEL, 0, (LPARAM)&selectionRange);


        if (radioState_forward == BST_CHECKED) this->forwardStartIndex = selectionRange.cpMax;
        else this->backStartIndex = selectionRange.cpMin;
        //this->forwardStartIndex = selectionRange.cpMax;
        //this->backStartIndex = selectionRange.cpMin;
        //SetFocus(this->hWndParent);
    }
    else {

        ftex.chrg.cpMin = 0;
        ftex.chrg.cpMax = 0;
        SendMessage(this->hwndRichEditParent, EM_EXSETSEL, 0, (LPARAM)&ftex.chrgText);
        
        this->forwardStartIndex = 0;
        this->backStartIndex = richEditLen;

        //if (radioState_forward == BST_CHECKED) SetFocus(this->_hwndRadio_forward);
        //else SetFocus(this->_hwndRadio_back);

        MessageBox(
            this->hWndSearch,
            (LPCWSTR)L"Not found.",
            (LPCWSTR)L"Search Result",
            MB_OK
        );
        
        //SetFocus(this->hWndEditControl);
    }

    //SetFocus(this->hWndEditControl);


    //LRESULT radioState = SendMessage(this->_hwndRadio_forward, BM_GETSTATE, 0, 0);
    /*if (radioState == BST_CHECKED) {
        LOG_WCHAR(L"forward is checked");
    }*/
    

}

void Search::OnNotify(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (((LPNMHDR)lParam)->code)
    {
    case TCN_SELCHANGING:
        // Return FALSE to allow the selection to change.
        return;
        break;
    case TCN_SELCHANGE:
        {
            int iPage = TabCtrl_GetCurSel((HWND)((LPNMHDR)lParam)->hwndFrom);
            LOG_INT(iPage);

            SendMessage(this->_hwndTabDisp, WM_SETTEXT, 0, (LPARAM)this->DAYS[iPage]);
        }

        break;
    default:
        break;
    }
}

bool Search::canSearch()
{
    const int size = GetWindowTextLength(this->hWndEditControl);
    if (size < 1) return false;
    return true;
}


void Search::createWindow()
{

    if (this->hWndSearch == NULL) {
        HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            this->CLASS_NAME,                     // Window class
            L"Search",    // Window text
            WS_MINIMIZEBOX | WS_SYSMENU,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, 320, 140, //before: width 302, height 66

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

        this->hWndSearch = hwnd;

    }

    //give handle to window controls.
    this->setWindowControlsHandle(this->hWndSearch);

    ShowWindow(this->hWndSearch, this->nCmdShowGlobal);

    

}

LRESULT WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return LRESULT();
}
