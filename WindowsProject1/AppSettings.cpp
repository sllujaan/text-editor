
#include "AppSettings.h"


AppSettings::AppSettings(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;
    this->createWindow();

    //this->createComboBox();
}

AppSettings::~AppSettings()
{

}

LRESULT AppSettings::WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;


    switch (message)
    {
    case WM_CREATE:
        AppSettings::centerWindow(hwnd);

        //disable parent window
        EnableWindow(hwnd, FALSE);

        break;

    case WM_COMMAND:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
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

int AppSettings::createWindow()
{
    
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->WndProcSettings;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIcon = LoadIcon(hInst, IDI_SHIELD);

    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Settings",    // Window text
        WS_MINIMIZEBOX | WS_SYSMENU,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 500,

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

    ShowWindow(hwnd, nCmdShowGlobal);


    //handleSettingsComboBoxWindow(hwnd);



    this->hWndSettings = hwnd;
    return 1;
}

