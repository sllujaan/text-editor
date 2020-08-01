
#include "AppSettings.h"


AppSettings::AppSettings(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;

    this->createWindow();
    this->createGroupBox();
    this->createListView();

    this->createGroupBoxSample();
    this->initSampleText();
    this->insertListViewItems(5);
    this->createTooltilp();

    this->handleFocuses();

    
    
}

AppSettings::~AppSettings()
{

}

LRESULT AppSettings::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;
    NMLVDISPINFO* plvdi;



    
    OutputDebugStringW((LPCWSTR)L"setings procedure---------------------------------------\r\n");

    


    switch (message)
    {
    case WM_NOTIFY:
        this->HandleWM_NOTIFY(lParam);
        break;

    case WM_CREATE:
        this->centerWindow(hwnd);

        //disable parent window
        EnableWindow(this->hWndParent, FALSE);
        
        break;

    case WM_COMMAND:
        OutputDebugStringW((LPCWSTR)L"*********COMMAND*************__\r\n");
        switch (LOWORD(wParam))
        {
        case IDM_CODE_SAMPLES:
            OutputDebugStringW((LPCWSTR)L"IDM_CODE_SAMPLES called____++++__\r\n");
            break;
        default:
            break;
        }

        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;

    case WM_CLOSE:
        //Enable parent window
        EnableWindow(this->hWndParent, TRUE);

        DestroyWindow(hwnd);
        return 1;
        break;

    case WM_DESTROY:

        DestroyWindow(hwnd);
        
        //PostQuitMessage(0);
        break;
    default:
        OutputDebugStringW((LPCWSTR)L"=================default===============\r\n");
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }


    return 0;
}

LRESULT AppSettings::WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    AppSettings* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<AppSettings*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<AppSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

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

LRESULT AppSettings::SubClassListViewProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    OutputDebugStringW((LPCWSTR)L"00000000000000LIST_VIEW1111111111111111\r\n");
    return LRESULT();
}

void AppSettings::centerWindow(HWND hwnd)
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

    return;
}

void AppSettings::initListFontSize()
{

    LPCWSTR fontSize[] = { L"8", L"10", L"12", L"14", L"16", L"18", L"20", L"22", L"24", L"26"};

    

}

void AppSettings::createListView()
{
    // CreateListView: Creates a list-view control in report view.
    // Returns the handle to the new control
    // TO DO:  The calling procedure should determine whether the handle is NULL, in case 
    // of an error in creation.
    //
    // HINST hInst: The global handle to the applicadtion instance.
    // HWND  hWndParent: The handle to the control's parent window. 
    //

    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    RECT rcClient;                       // The parent window's client area.

    GetClientRect(this->hWndGroupBox, &rcClient);

    // Create the list-view window in report view with label editing enabled.
    HWND hWndListView = CreateWindow(WC_LISTVIEW,
        L"List view",
        WS_CHILD | WS_VISIBLE | LVS_LIST | WS_BORDER | LVS_SINGLESEL,
        10, 20,
        (rcClient.right - rcClient.left) - 20,
        (rcClient.bottom - rcClient.top) - 30,
        this->hWndGroupBox,
        (HMENU)IDM_CODE_SAMPLES,
        this->hInst,
        NULL);

    

    this->hwndListView = hWndListView;
    //this->insertListViewItems(10);

    /*lpfnSettingWndProc = (WNDPROC)SetWindowLongPtr(this->hwndListView,
        GWLP_WNDPROC, (LONG_PTR)this->SubClassListViewProc);*/

}

void AppSettings::insertListViewItems(int cItems)
{
    LVITEM lvI;

    // Initialize LVITEM members that are common to all items.
    lvI.pszText = (wchar_t*)L"8"; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;
    lvI.iItem = 0;

    ListView_InsertItem(this->hwndListView, &lvI);


    for (int i = 9; i <= 72; i++) {
        lvI.iItem = i;
        std::wstring str = std::to_wstring(i);
        lvI.pszText = (wchar_t*)str.c_str();
        ListView_InsertItem(this->hwndListView, &lvI);
    }
    
    //MessageBox(0, TEXT("inserting itmes list view "), 0, 0);



}

void AppSettings::initListView()
{
    
}

void AppSettings::createGroupBox()
{
    HWND hwndGroupBox = this->getGroupBox(L"Font", 10, 10, 300, 300);

    this->hWndGroupBox = hwndGroupBox;

    /*HWND hGr = CreateWindow(L"BUTTON", L"My GRoup Box",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        10, 10, 200, 200, this->hWndSettings, (HMENU)-1, this->hInst, NULL);*/

}

void AppSettings::createGroupBoxSample()
{
    HWND hwndGroupBox = this->getGroupBox(L"Sample", 10, 320, 300, 100);

    this->hWndGroupBoxSample = hwndGroupBox;
}

void AppSettings::initListViewSample()
{

}

void AppSettings::initSampleText()
{

    RECT rcClient;                       // The parent window's client area.

    GetClientRect(this->hWndGroupBoxSample, &rcClient);


    HWND hwndStatic = CreateWindowEx(
        0,
        L"static",      // Button text 
        L"AaBbYyZz",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,  // Styles
        10,         // x position 
        20,         // y position
        (rcClient.right - rcClient.left) - 20,  // window width
        (rcClient.bottom - rcClient.top) - 30,  // window height
        this->hWndGroupBoxSample,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(this->hWndGroupBoxSample, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.



    SendMessage(hwndStatic, WM_SETFONT, (WPARAM)this->getFont(14), TRUE);

    this->hWndGroupBoxSampleText = hwndStatic;
   
}

void AppSettings::createTooltilp()
{
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | WS_VISIBLE | TTS_NOPREFIX | TTS_ALWAYSTIP, 0, 0, 0, 0, this->hwndListView, NULL, 0, NULL);

    TTTOOLINFO ti = { 0 };
    ti.cbSize = sizeof(TTTOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = this->hwndListView;
    ti.lpszText = (wchar_t*)L"Tooltip string";
    GetClientRect(this->hwndListView, &ti.rect);

    if (!SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)&ti))
        MessageBox(0, TEXT("Failed: TTM_ADDTOOL"), 0, 0);

}

void AppSettings::HandleWM_NOTIFY(LPARAM lParam)
{
    LPNMITEMACTIVATE lpnmitem;
    std::wstring str;
    int fontSize;

    int index = ListView_GetSelectionMark(this->hwndListView);
    if (index == -1) return;

    fontSize = index + 8;

    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(fontSize), TRUE);

    return;
    //below code is just for reference pupose

    //switch (((LPNMHDR)lParam)->code)
    //{
    //case NM_CLICK:
    //    lpnmitem = (LPNMITEMACTIVATE)lParam;
    //    lpnmitem->iItem;

    //    //fontSize = this->getFontSizeFromIndex(lpnmitem->iItem+8);

    //    //SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(fontSize), TRUE);

    //    ///*if (lpnmitem->iItem == FONT_SIZE_8) {
    //    //    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(18), TRUE);
    //    //    OutputDebugStringW((LPCWSTR)L"FONT_SIZE_8\r\n");
    //    //}*/

    //    //str = L"" + std::to_wstring(lpnmitem->iItem);

    //    //OutputDebugStringW( (LPCWSTR)str.c_str() );
    //    //
    //    //OutputDebugStringW((LPCWSTR)L"------++++++++LVN_BEGINDRAG-------------------\r\n");
    //    break;
    //default:
    //    break;
    //}


}

int AppSettings::getFontSizeFromIndex(int index)
{
    if (index % 2 == 0) return ++index;
    else return index;
}

void AppSettings::handleFocuses()
{

    //SendMessage(this->hwndListView, LVM_EDITLABEL, 0, 0);
    ListView_EditLabel(this->hwndListView, 0);
    //SetFocus(this->hwndListView);
    //ListView_SetSelectionMark(this->hwndListView, (72-8));
}

HWND AppSettings::getGroupBox(LPCWSTR name, int posX, int posY, int width, int height)
{
    HWND hwndGroupBox = CreateWindowEx(
        0,
        L"BUTTON",      // Button text 
        name,
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER,  // Styles
        posX,         // x position 
        posY,         // y position 
        width,        // Button width
        height,        // Button height
        this->hWndSettings,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(this->hWndSettings, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    SendMessage(hwndGroupBox, WM_SETFONT, (WPARAM)this->getFont(14), TRUE);

    return hwndGroupBox;
}

HFONT AppSettings::getFont(size_t size)
{
    HFONT hFont = CreateFont(int(size), 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

    return hFont;
}



void AppSettings::createComboBox()
{
    // Create the Combobox
//
// Uses the CreateWindow function to create a child window of 
// the application window. The WC_COMBOBOX window style specifies  
// that it is a combobox.

    int xpos = 100;            // Horizontal position of the window.
    int ypos = 100;            // Vertical position of the window.
    int nwidth = 200;          // Width of the window
    int nheight = 200;         // Height of the window
    HWND hwndParent = this->hWndSettings; // Handle to the parent window

    HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        xpos, ypos, nwidth, nheight, hwndParent, NULL, NULL,
        NULL);



    //loading combobox with data
    TCHAR Planets[9][10] =
    {
        TEXT("Mercury"), TEXT("Venus"), TEXT("Terra"), TEXT("Mars"),
        TEXT("Jupiter"), TEXT("Saturn"), TEXT("Uranus"), TEXT("Neptune"),
        TEXT("Pluto??")
    };

    TCHAR A[16];
    int  k = 0;

    memset(&A, 0, sizeof(A));
    for (k = 0; k <= 8; k += 1)
    {
        wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Planets[k]);

        // Add string to combobox.
        SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
    }

    // Send the CB_SETCURSEL message to display an initial item 
    //  in the selection field  
    SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

}


void AppSettings::createWindow()
{
    
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->WndProcSettings;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = this->hInst;
    wcex.lpszClassName = CLASS_NAME;
    //wcex.hIcon = LoadIcon(this->hInst, IDI_SHIELD);

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Settings",    // Window text
        WS_SYSMENU,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 500,

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
    }

    ShowWindow(hwnd, this->nCmdShowGlobal);


    //handleSettingsComboBoxWindow(hwnd);



    this->hWndSettings = hwnd;

}

