
#include "AppSettings.h"


AppSettings::AppSettings(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;

    this->createWindow();
    this->createGroupBox();
    this->createListView();
    
    
}

AppSettings::~AppSettings()
{

}

LRESULT AppSettings::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;


    switch (message)
    {
    case WM_CREATE:
        this->centerWindow(hwnd);

        //disable parent window
        EnableWindow(this->hWndParent, FALSE);
        
        break;

    case WM_COMMAND:

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
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
        10, 20,
        (rcClient.right - rcClient.left) - 20,
        (rcClient.bottom - rcClient.top) - 30,
        this->hWndGroupBox,
        (HMENU)IDM_CODE_SAMPLES,
        this->hInst,
        NULL);

    this->hwndListView = hWndListView;
    //this->insertListViewItems(10);

}

void AppSettings::insertListViewItems(int cItems)
{
    LVITEM lvI;

    // Initialize LVITEM members that are common to all items.
    lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;

    // Initialize LVITEM members that are different for each item.
    for (int index = 0; index < cItems; index++)
    {
        lvI.iItem = index;
        lvI.iImage = index;

        // Insert items into the list.
        if (ListView_InsertItem(this->hwndListView, &lvI) == -1) {
            //failed to insert itmes
            MessageBox(
                NULL,
                (LPCWSTR)L"Failed to insert items.",
                (LPCWSTR)L"List View",
                MB_OK
            );
        }
            
    }
}

void AppSettings::initListView()
{
    
}

void AppSettings::createGroupBox()
{
    HWND hwndGroupBox = CreateWindowEx(
        0,
        L"BUTTON",      // Button text 
        L"Font",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,  // Styles
        10,         // x position 
        10,         // y position 
        300,        // Button width
        300,        // Button height
        this->hWndSettings,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(this->hWndSettings, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    SendMessage(hwndGroupBox, WM_SETFONT, (WPARAM)this->getFont(12) , TRUE);

    this->hWndGroupBox = hwndGroupBox;

    /*HWND hGr = CreateWindow(L"BUTTON", L"My GRoup Box",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        10, 10, 200, 200, this->hWndSettings, (HMENU)-1, this->hInst, NULL);*/

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

